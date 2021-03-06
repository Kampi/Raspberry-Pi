#!/bin/bash

##### User settings ######
IP=192.168.178.52
INSTALL_PATH=/usr/local
TOOLCHAIN_PATH=/home/kampi/Toolchain/RaspberryPi
###########################

#### Colors #####
Red="\033[0;31m"
Green="\033[0;32m"
Yellow="\033[0;33m"
Cyan="\033[0;36m"
Reset="\033[0m"
#################

echo -e ${Yellow}"Install packages..."${Reset}
sudo add-apt-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"
sudo apt update
sudo apt-get install -y flex libjasper-dev git cmake build-essential pkg-config libjpeg-dev libtiff5-dev libpng-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libatlas-base-dev gfortran wget unzip libz-dev zlib1g-dev gcc g++ git bison python gperf gdb-multiarch qt5-default texinfo make python3-dev

echo -e ${Yellow}"Generate SSH keys..."${Reset}
ssh-keygen -t rsa -C root@${IP} -P "" -f ~/.ssh/rpi_root_id_rsa
ssh-keygen -t rsa -C pi@${IP} -P "" -f ~/.ssh/rpi_pi_id_rsa
cat ~/.ssh/rpi_root_id_rsa.pub | ssh root@${IP} "cat >> .ssh/authorized_keys && chmod 600 .ssh/authorized_keys"
cat ~/.ssh/rpi_pi_id_rsa.pub | ssh pi@${IP} "cat >> .ssh/authorized_keys && chmod 600 .ssh/authorized_keys"

echo -e ${Yellow}"Download toolchain..."${Reset}
mkdir -p ${TOOLCHAIN_PATH}
cd ${TOOLCHAIN_PATH}
git clone https://github.com/raspberrypi/tools

echo -e ${Yellow}"Download Python script..."${Reset}
wget https://raw.githubusercontent.com/riscv/riscv-poky/master/scripts/sysroot-relativelinks.py
sudo chmod +x sysroot-relativelinks.py

echo -e ${Red}"Script finished!"${Reset}
