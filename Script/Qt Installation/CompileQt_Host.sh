#!/bin/bash

##### User settings ######
# Set DEVICE with
# 	linux-rasp-pi-g++ 	- Raspberry Pi 1 (+ Zero and Zero W)
# 	linux-rasp-pi2-g++	- Raspberry Pi 2
# 	linux-rasp-pi3-g++	- Raspberry Pi 3
# 	linux-rasp-pi3-vc4-g++	- Raspberry Pi 3 with VC4 driver

CORES=4
IP=192.168.178.52
DEVICE=linux-rasp-pi3-g++
TEMP_PATH=/home/kampi/RaspberryQt
SYSROOT_PATH=/home/kampi/Toolchain/RaspberryPi
TOOLCHAIN_PATH=/home/kampi/Toolchain/RaspberryPi
INSTALL_PATH=/usr/local
###########################

#### Colors #####
Red="\033[0;31m"
Green="\033[0;32m"
Yellow="\033[0;33m"
Cyan="\033[0;36m"
Reset="\033[0m"
#################

echo -e ${Yellow}"Install packages..."${Reset}
sudo apt-get update

echo -e ${Yellow}"Create directories..."${Reset}
mkdir -p ${TEMP_PATH}/log ${TEMP_PATH}/build
mkdir -p ${SYSROOT_PATH}/sysroot

cd ${TEMP_PATH}
echo -e ${Yellow}"Download Qt..."${Reset}
wget https://download.qt.io/archive/qt/5.12/5.12.8/single/qt-everywhere-src-5.12.8.tar.xz
tar xf qt-everywhere-src-5.12.8.tar.xz
cp -R qt-everywhere-src-5.12.8/qtbase/mkspecs/linux-arm-gnueabi-g++ qt-everywhere-src-5.12.8/qtbase/mkspecs/linux-arm-gnueabihf-g++
sed -i -e "s/arm-linux-gnueabi-/arm-linux-gnueabihf-/g" qt-everywhere-src-5.12.8/qtbase/mkspecs/linux-arm-gnueabihf-g++/qmake.conf

echo -e ${Yellow}"Download /lib..."${Reset}
rsync -avz root@${IP}:/lib ${SYSROOT_PATH}/sysroot | tee ${TEMP_PATH}/log/copy_lib.log
echo -e ${Yellow}"Download /usr/include..."${Reset}
rsync -avz root@${IP}:/usr/include ${SYSROOT_PATH}/sysroot/usr | tee ${TEMP_PATH}/log/copy_usr_include.log
echo -e ${Yellow}"Download /usr/lib..."${Reset}
rsync -avz root@${IP}:/usr/lib ${SYSROOT_PATH}/sysroot/usr | tee ${TEMP_PATH}/log/copy_usr_lib.log
echo -e ${Yellow}"Download /opt/vc..."${Reset}
rsync -avz root@${IP}:/opt/vc ${SYSROOT_PATH}/sysroot/opt | tee ${TEMP_PATH}/log/copy_opt_vc.log
echo -e ${Yellow}"Download /usr/local..."${Reset}
rsync -avz root@${IP}:/usr/local ${SYSROOT_PATH}/sysroot/usr | tee ${TEMP_PATH}/log/copy_usr_local.log

echo -e ${Yellow}"Change symlinks..."${Reset}
cd ${SYSROOT_PATH}
${TOOLCHAIN_PATH}/sysroot-relativelinks.py sysroot

echo -e ${Yellow}"Configure Qt..."${Reset}
cd ${TEMP_PATH}/build
../qt-everywhere-src-5.12.8/configure -opengl es2 -device ${DEVICE} -device-option CROSS_COMPILE=${TOOLCHAIN_PATH}/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf- -sysroot ${SYSROOT_PATH}/sysroot -prefix ${INSTALL_PATH}/RaspberryQt -opensource -confirm-license -no-gbm -skip qtscript -nomake tests -nomake examples -make libs -pkg-config -no-use-gold-linker -v | tee ${TEMP_PATH}/log/config.log

echo -e ${Yellow}"Build Qt..."${Reset}
make -j${CORES} | tee ${TEMP_PATH}/log/make.log
make install | tee ${TEMP_PATH}/log/install.log

echo -e ${Yellow}"Upload /lib..."${Reset}
#rsync -avz ${SYSROOT_PATH}/sysroot/lib root@${IP}:/ | tee ${TEMP_PATH}/log/copy_lib.log
echo -e ${Yellow}"Upload /usr/include..."${Reset}
#rsync -avz ${SYSROOT_PATH}/sysroot/usr/include root@${IP}:/usr | tee ${TEMP_PATH}/log/copy_usr_include.log
echo -e ${Yellow}"Upload /usr/lib..."${Reset}
#rsync -avz ${SYSROOT_PATH}/sysroot/usr/lib root@${IP}:/usr | tee ${TEMP_PATH}/log/copy_usr_lib.log
echo -e ${Yellow}"Upload /opt/vc..."${Reset}
#rsync -avz ${SYSROOT_PATH}/sysroot/opt/vc root@${IP}:/opt | tee ${TEMP_PATH}/log/copy_opt_vc.log
echo -e ${Yellow}"Upload /usr/local..."${Reset}
#rsync -avz ${SYSROOT_PATH}/sysroot/usr/local root@${IP}:/usr | tee ${TEMP_PATH}/log/upload_usr_local.log
