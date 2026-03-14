#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <stdbool.h>

int ble_init(void);
void ble_start_scanning(void);
bool ble_is_connected(void);

#endif
