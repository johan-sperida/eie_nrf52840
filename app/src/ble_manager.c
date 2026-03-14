#include "ble_manager.h"

#include <errno.h>
#include <string.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/sys/printk.h>

static const bt_addr_t TARGET_ADDR = {
    .val = {0x19, 0xC0, 0x9C, 0xDD, 0x3A, 0xD8}};

static const bt_addr_le_t TARGET_ADDR_LE = {.type = 1, .a = TARGET_ADDR};

// /* Controls Service: 7859530f-0184-4854-9ffd-69504534e434 */
// static struct bt_uuid_128 CONTROLS_SERVICE_UUID =
//     BT_UUID_INIT_128(BT_UUID_128_ENCODE(0x7859530f, 0x0184, 0x4854, 0x9ffd,
//     0x69504534e434));

// /* Lights Characteristic: 93e249c3-540c-47cf-b85a-61e27b9e2bfd */
// static struct bt_uuid_128 LIGHTS_CHAR_UUID =
//     BT_UUID_INIT_128(BT_UUID_128_ENCODE(0x93e249c3, 0x540c, 0x47cf, 0xb85a,
//     0x61e27b9e2bfd));

// /* Acceleration Characteristic: eea5d803-0013-4eb5-a50f-037642b17c93 */
// static struct bt_uuid_128 ACCELERATION_CHAR_UUID =
//     BT_UUID_INIT_128(BT_UUID_128_ENCODE(0xeea5d803, 0x0013, 0x4eb5, 0xa50f,
//     0x037642b17c93));

static struct bt_uuid_16 DISTANCE_SENSE_SERVICE_UUID = BT_UUID_INIT_16(0x180A);

static struct bt_uuid_128 DISTANCE_CHAR_UUID = BT_UUID_INIT_128(
    BT_UUID_128_ENCODE(0x044e6f5f, 0x4a31, 0x417a, 0x86d5, 0x886292a9ebb5));

static struct bt_conn *my_connection;
static struct bt_uuid_16 discover_uuid = BT_UUID_INIT_16(0);
static struct bt_gatt_discover_params discover_params;
static struct bt_gatt_subscribe_params subscribe_params;

static uint8_t discover_func(struct bt_conn *conn,
                             const struct bt_gatt_attr *attr,
                             struct bt_gatt_discover_params *params);
static uint8_t notify_func(struct bt_conn *conn,
                           struct bt_gatt_subscribe_params *params,
                           const void *data, uint16_t length);

static bool ble_get_adv_device_name_cb(struct bt_data *data, void *user_data) {
  char *name = user_data;

  if (data->type == BT_DATA_NAME_COMPLETE ||
      data->type == BT_DATA_NAME_SHORTENED) {
    memcpy(name, data->data, data->data_len);
    name[data->data_len] = '\0';
    return false;
  }

  return true;
}

static void ble_on_advertisement_received(const bt_addr_le_t *addr, int8_t rssi,
                                          uint8_t type,
                                          struct net_buf_simple *ad) {
  char addr_str[BT_ADDR_LE_STR_LEN];
  int err;

  if (my_connection) {
    return;
  }

  if (type != BT_GAP_ADV_TYPE_ADV_IND &&
      type != BT_GAP_ADV_TYPE_ADV_DIRECT_IND) {
    return;
  }

  char name[32] = {0};
  bt_data_parse(ad, ble_get_adv_device_name_cb, name);
  bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
  printk("Device found: %s (RSSI %d) - '%s'\n", addr_str, rssi, name);

  if (rssi < -40) {
    return;
  }
  printk("Passed rssi\n");

  if (memcmp(addr->a.val, TARGET_ADDR_LE.a.val, 6) != 0) {
    printk("failed compare\n");
    return;
  }
  printk("Passed compare\n");

  if (bt_le_scan_stop()) {
    return;
  }
  printk("Passed stop scan\n");

  err = bt_conn_le_create(addr, BT_CONN_LE_CREATE_CONN,
                          BT_LE_CONN_PARAM_DEFAULT, &my_connection);
  if (err) {
    printk("Create conn to %s failed (%d)\n", addr_str, err);
    ble_start_scanning();
  }
}

void ble_start_scanning(void) {
  int err;

  err = bt_le_scan_start(BT_LE_SCAN_PASSIVE, ble_on_advertisement_received);
  if (err) {
    printk("Scanning failed to start (err %d)\n", err);
    return;
  }

  printk("Scanning successfully started\n");
}

static void ble_on_device_connected(struct bt_conn *conn, uint8_t err) {
  char addr[BT_ADDR_LE_STR_LEN];

  bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

  if (err) {
    printk("Failed to connect to %s %u %s\n", addr, err,
           bt_hci_err_to_str(err));

    bt_conn_unref(my_connection);
    my_connection = NULL;

    ble_start_scanning();
    return;
  }

  if (conn != my_connection) {
    return;
  }

  printk("Connected: %s\n", addr);

  discover_params.uuid = &DISTANCE_SENSE_SERVICE_UUID.uuid;
  discover_params.func = discover_func;
  discover_params.start_handle = BT_ATT_FIRST_ATTRIBUTE_HANDLE;
  discover_params.end_handle = BT_ATT_LAST_ATTRIBUTE_HANDLE;
  discover_params.type = BT_GATT_DISCOVER_PRIMARY;

  err = bt_gatt_discover(my_connection, &discover_params);
  if (err) {
    printk("Discover failed(err %d)\n", err);
    return;
  }
}

static void ble_on_device_disconnected(struct bt_conn *conn, uint8_t reason) {
  char addr[BT_ADDR_LE_STR_LEN];

  if (conn != my_connection) {
    return;
  }

  bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
  printk("Disconnected: %s, reason 0x%02x %s\n", addr, reason,
         bt_hci_err_to_str(reason));

  bt_conn_unref(my_connection);
  my_connection = NULL;

  ble_start_scanning();
}

static uint8_t discover_func(struct bt_conn *conn,
                             const struct bt_gatt_attr *attr,
                             struct bt_gatt_discover_params *params) {
  int err;

  if (!attr) {
    printk("Discover complete\n");
    (void)memset(params, 0, sizeof(*params));
    return BT_GATT_ITER_STOP;
  }

  printk("[ATTRIBUTE] handle %u\n", attr->handle);

  if (!bt_uuid_cmp(discover_params.uuid, &DISTANCE_SENSE_SERVICE_UUID.uuid)) {
    discover_params.uuid = &DISTANCE_CHAR_UUID.uuid;
    discover_params.start_handle = attr->handle + 1;
    discover_params.type = BT_GATT_DISCOVER_CHARACTERISTIC;

    err = bt_gatt_discover(conn, &discover_params);
    if (err) {
      printk("Discover failed (err %d)\n", err);
    }
  } else if (!bt_uuid_cmp(discover_params.uuid, &DISTANCE_CHAR_UUID.uuid)) {
    memcpy(&discover_uuid, BT_UUID_GATT_CCC, sizeof(discover_uuid));
    discover_params.uuid = &discover_uuid.uuid;
    discover_params.start_handle = attr->handle + 2;
    discover_params.type = BT_GATT_DISCOVER_DESCRIPTOR;
    subscribe_params.value_handle = bt_gatt_attr_value_handle(attr);

    err = bt_gatt_discover(conn, &discover_params);
    if (err) {
      printk("Discover failed (err %d)\n", err);
    }
  } else {
    subscribe_params.notify = notify_func;
    subscribe_params.value = BT_GATT_CCC_NOTIFY;
    subscribe_params.ccc_handle = attr->handle;

    err = bt_gatt_subscribe(conn, &subscribe_params);
    if (err && err != -EALREADY) {
      printk("Subscribe failed (err %d)\n", err);
    } else {
      printk("[SUBSCRIBED]\n");
    }

    return BT_GATT_ITER_STOP;
  }

  return BT_GATT_ITER_STOP;
}

static uint8_t notify_func(struct bt_conn *conn,
                           struct bt_gatt_subscribe_params *params,
                           const void *data, uint16_t length) {
  if (!data) {
    printk("[UNSUBSCRIBED]\n");
    params->value_handle = 0U;
    return BT_GATT_ITER_STOP;
  }

  return BT_GATT_ITER_CONTINUE;
}

int ble_init(void) {
  int err = bt_enable(NULL);
  if (err) {
    printk("Bluetooth init failed (err %d)\n", err);
    return err;
  }

  printk("Bluetooth initialized\n");
  return 0;
}

bool ble_is_connected(void) { return my_connection != NULL; }

BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = ble_on_device_connected,
    .disconnected = ble_on_device_disconnected,
};
