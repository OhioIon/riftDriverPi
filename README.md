# riftDriverPi
Small USB HID driver to activate Oculus Rift CV1 connected to Raspberry Pi 4

<b>How to compile the driver</b>
- Start RPi 4 and open a terminal
- Execute the following commands

```shell
sudo apt-get install libusb-1.0
git clone --recurse-submodules https://github.com/OhioIon/riftDriverPi
cd riftDriverPi
make
```

<b>How to make Raspbian recognize the Oculus Rift CV1 as screen</b>

Step 1 - Hardware connection
- Connect your normal screen to HDMI0
- Connect the Oculus Rift CV1 to HDMI1 and USB 3.0 port of the RPi 4

Step 2 - Rebuilding the kernel with increased framerate support
- The RPi kernel currently has a limitation of 85 Hz refresh rate for displays
- The Oculus Rift CV1 requires exactly 90 Hz refresh rate
- In order to get and build the kernel execute the following commands in a terminal
- This follows the instructions of https://www.raspberrypi.org/documentation/linux/kernel/building.md

```shell
sudo apt install git bc bison flex libssl-dev make
cd ~
git clone --depth=1 https://github.com/raspberrypi/linux
cd linux

sed -i 's/if (drm_mode_vrefresh(mode) > 85)/if (drm_mode_vrefresh(mode) > 90)/g' drivers/gpu/drm/vc4/vc4_firmware_kms.c

KERNEL=kernel7l
make bcm2711_defconfig
make -j4 zImage modules dtbs
sudo make modules_install
sudo cp arch/arm/boot/dts/*.dtb /boot/
sudo cp arch/arm/boot/dts/overlays/*.dtb* /boot/overlays/
sudo cp arch/arm/boot/dts/overlays/README /boot/overlays/
sudo cp arch/arm/boot/zImage /boot/$KERNEL.img
```

- ... TODO /boot/config.txt changes  
