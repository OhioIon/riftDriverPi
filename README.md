# riftDriverPi
Small USB HID driver to activate Oculus Rift CV1 as extended display on Raspberry Pi 4. 
Each eye only sees half of the screen so whatever is put on that extended display needs to output content in Side-by-side (SBS) 3D format.

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

Step 2 - Configure kernel with increased framerate support
- The RPi kernel currently defaults to a limit of 85 Hz framerate for displays
- The Oculus Rift CV1 requires exactly 90 Hz framerate
- [As of Oct. 2020](https://github.com/raspberrypi/linux/issues/3677), the framerate limitation is configurable via a kernel parameter
- Add the following to the beginning of /boot/cmdline.txt

```shell
vc4.fkms_max_refresh_rate=90
```

Step 3 - Configure screen resolution manually via RPi config.txt
- The Rift CV1 does not function like a normal screen
- It will only send hotplug over HDMI if it is enabled via a USB driver (which is too late for RPi)
- Also the EDID does not contain a standard timing information but only a single detailed timing descriptor
- In order to make it work, we need to explicitly apply the detailed timing spec in config.txt and force the hotplug
- The following lines need to be set in /boot/config.txt

```shell
hdmi_force_hotplug:1=1
hdmi_group:1=2
hdmi_mode:1=87
hdmi_timings:1=2160 1  8  32  40 1200 0  50 2 220 0 0 0 90 0 296750000 3
```
You can also use the config.txt provided as reference and copy it with:

```shell
sudo cp ~/riftDriverPi/config.txt /boot
sudo reboot
```

Now there is a second screen in the RPi 4 Screen Layout Editor (Preferences -> Screen Configuration) with resolution "FIXEDMODE".

<b>Configure linux to use hidapi via libusb for Oculus Rift</b>
- This will avoid having to run the application as root to access the USB device
- Execute the following commands in a terminal

```shell
echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="2833", MODE="0666", GROUP="plugdev"' | sudo tee --append /etc/udev/rules.d/83-hmd.rules
sudo udevadm control --reload-rules
sudo reboot
```

<b>Enable the Oculus Rift CV1 screen</b>
- If the previous configuration steps have been completed, there is no need to repeat them
- In order to activate the Oculus Rift CV1 screen, just execute the riftDriverPi app
- The driver will enable the screen and send the keep-alive message cyclically

```shell
~/riftDriverPi/riftDriverPi
```
- You should see that the orange LED on the Rift turns white
- You should see the extended desktop through the Rift (but not nice since left and right eye see different parts)
- Test stereo view by watching a side-by-side video in full-screen on YouTube
  (https://www.youtube.com/results?search_query=3D+side+by+side)

Hint: Hitting return in the terminal will stop the application.
