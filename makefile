#Build
build:
	west build -b nrf52840dk/nrf52840 app --pristine

#Bluetooth build
buildB:
	west build -p always -b nrf52840dk/nrf52840 app

# Flash to device
flash: build
	west flash

flashB: buildB
	west flash