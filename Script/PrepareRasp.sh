#!/bin/bash

##### User settings ######
###########################

#### Colors #####
Red="\033[0;31m"
Green="\033[0;32m"
Yellow="\033[0;33m"
Cyan="\033[0;36m"
Reset="\033[0m"
#################

echo -e ${Yellow}"Install packages..."${Reset}
sudo sh -c "echo deb-src http://raspbian.raspberrypi.org/raspbian/ buster main contrib non-free rpi \ >> /etc/apt/sources.list"
sudo apt-get update
sudo apt-get -y dist-upgrade
sudo rpi-update
sudo apt-get -y build-dep qt5-qmake
sudo apt-get -y build-dep libqt5gui5
sudo apt-get -y build-dep libqt5webengine-data
sudo apt-get -y build-dep libqt5webkit5
sudo apt-get -y install libudev-dev libinput-dev libts-dev libxcb-xinerama0-dev libxcb-xinerama0 gdbserver

echo -e ${Yellow}"Create symlinks..."${Reset}
sudo ln -s /opt/vc/lib/libEGL.so /usr/lib/arm-linux-gnueabihf/libEGL.so.1.0.0
sudo ln -s /opt/vc/lib/libGLESv2.so /usr/lib/arm-linux-gnueabihf/libGLESv2.so.2.0.0
sudo ln -s /opt/vc/lib/libEGL.so /opt/vc/lib/libEGL.so.1
sudo ln -s /opt/vc/lib/libGLESv2.so /opt/vc/lib/libGLESv2.so.2

echo -e ${Yellow}"Create SSH directories..."${Reset}
sudo mkdir -p /root/.ssh
sudo chmod 700 /root/.ssh
mkdir -p /home/pi/.ssh
chmod 700 /home/pi/.ssh

echo -e ${Red}"Script finished. Please reboot!"${Reset}
