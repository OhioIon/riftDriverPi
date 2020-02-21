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
- Connect your normal screen to HDMI0
- Connect the Oculus Rift CV1 to HDMI1 and an USB 3.0 port of the RPi 4
- ... TODO describe kernel changes & build and /boot/config.txt changes  
