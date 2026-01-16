init:
	cd ~/zephyr-projects/; source .venv/bin/activate; cd eie_nrf52840.git/
 
#Build
build: venv
	west build -b nrf52840dk/nrf52840 app --pristine

#Bluetooth build
buildB: venv
	west build -p always -b nrf52840dk/nrf52840 app

# Flash to device
flash: build
	west flash

flashB: buildB
	west flash
