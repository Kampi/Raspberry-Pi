#!/bin/bash

##### User settings ######
CORES=4
VERSION=9.1
TARGET=arm-linux-gnueabihf
INSTALL_PATH=/home/kampi/Toolchain/RaspberryPi/GDBQt
###########################

#### Colors #####
Red="\033[0;31m"
Green="\033[0;32m"
Yellow="\033[0;33m"
Cyan="\033[0;36m"
Reset="\033[0m"
#################

echo -e ${Yellow}"Create directories..."${Reset}
mkdir -p ${INSTALL_PATH}/log ${INSTALL_PATH}/bin ${INSTALL_PATH}/temp

echo -e ${Yellow}"Download GDB sources..."${Reset}
cd ${INSTALL_PATH}/temp
wget https://ftp.gnu.org/gnu/gdb/gdb-${VERSION}.tar.xz
tar xf gdb-${VERSION}.tar.xz
mkdir -p ${INSTALL_PATH}/temp/gdb-${VERSION}/build
cd ${INSTALL_PATH}/temp/gdb-${VERSION}/build

echo -e ${Yellow}"Get Python executable..."${Reset}
export PYTHON=python3
export PYTHON_LIBDIR=$("${PYTHON}" -c "import sysconfig; print(sysconfig.get_config_var('LIBDIR'))")

echo -e ${Yellow}"Configure GDB..."${Reset}
../configure --prefix=${INSTALL_PATH} --target=${TARGET} --with-python=${PYTHON} LDFLAGS="-L${PYTHON_LIBDIR}" | tee ${INSTALL_PATH}/log/configure.log

echo -e ${Yellow}"Build GDB..."${Reset}
make -j${CORES} | tee ${INSTALL_PATH}/log/make.log
make -C gdb install | tee ${INSTALL_PATH}/log/install.log

echo -e ${Yellow}"Clean up..."${Reset}
rm -rf ${INSTALL_PATH}/temp
