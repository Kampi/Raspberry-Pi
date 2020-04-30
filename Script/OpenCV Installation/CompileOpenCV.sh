#!/bin/bash

##### User settings ######
CORES=4
IP=192.168.178.52
TEMP_PATH=/home/kampi/OpenCV
SYSROOT_PATH=/home/kampi/Toolchain/RaspberryPi/sysroot
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

echo -e ${Yellow}"Create directories..."${Reset}
mkdir -p ${TEMP_PATH}/log
mkdir -p ${SYSROOT_PATH}

echo -e ${Yellow}"Download OpenCV sources..."${Reset}
cd ${TEMP_PATH}
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git

echo -e ${Yellow}"Download /lib..."${Reset}
rsync -avz root@${IP}:/lib ${SYSROOT_PATH} | tee ${TEMP_PATH}/log/copy_lib.log
echo -e ${Yellow}"Download /usr/include..."${Reset}
rsync -avz root@${IP}:/usr/include ${SYSROOT_PATH}/usr | tee ${TEMP_PATH}/log/copy_usr_include.log
echo -e ${Yellow}"Download /usr/lib..."${Reset}
rsync -avz root@${IP}:/usr/lib ${SYSROOT_PATH}/usr | tee ${TEMP_PATH}/log/copy_usr_lib.log
echo -e ${Yellow}"Download /opt/vc..."${Reset}
rsync -avz root@${IP}:/opt/vc ${SYSROOT_PATH}/opt | tee ${TEMP_PATH}/log/copy_opt_vc.log
echo -e ${Yellow}"Download /usr/local..."${Reset}
rsync -avz root@${IP}:/usr/local ${SYSROOT_PATH}/usr | tee ${TEMP_PATH}/log/copy_usr_local.log

echo -e ${Yellow}"Change symlinks..."${Reset}
${TOOLCHAIN_PATH}/sysroot-relativelinks.py ${SYSROOT_PATH}

echo -e ${Yellow}"Configure build..."${Reset}
cd ${TEMP_PATH}/opencv
mkdir -p ${TEMP_PATH}/opencv/build && cd build
cmake -D CMAKE_C_COMPILER=${TOOLCHAIN_PATH}/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc \
	-D CMAKE_CXX_COMPILER=${TOOLCHAIN_PATH}/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++ \
	-D CMAKE_C_FLAGS=${CMAKE_C_FLAGS} -mthumb -fdata-sections -Wa,--noexecstack -fsigned-char -Wno-psabi -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=vfpv4 \
	-D CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} -mthumb -fdata-sections -Wa,--noexecstack -fsigned-char -Wno-psabi -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=vfpv4 \
	-D CMAKE_BUILD_TYPE=RELEASE \
	-D CMAKE_INSTALL_PREFIX=${SYSROOT_PATH}${INSTALL_PATH}/OpenCV \
	-D CMAKE_TOOLCHAIN_FILE=../platforms/linux/arm-gnueabi.toolchain.cmake \
	-D OPENCV_EXTRA_MODULES_PATH=${TEMP_PATH}/opencv_contrib/modules \
 	-D OPENCV_ENABLE_NONFREE=OFF \
	-D ENABLE_NEON=ON \
	-D ENABLE_VFPV3=ON \
	-D WITH_GTK=ON \
	-D WITH_OPENGL=ON \
	-D WITH_V4L=ON \
	-D WITH_QT=OFF \
	-D BUILD_TESTS=OFF \
	-D BUILD_DOCS=OFF \
	-D BUILD_OPENCV_PYTHON2=OFF \
	-D BUILD_OPENCV_PYTHON3=OFF \
	-D BUILD_EXAMPLES=OFF \
	.. | tee ${TEMP_PATH}/log/configure.log

echo -e ${Yellow}"Build OpenCV..."${Reset}
make -j${CORES} | tee ${TEMP_PATH}/log/make.log
make install | tee ${TEMP_PATH}/log/install.log

echo -e ${Yellow}"Upload /lib..."${Reset}
rsync -avz ${SYSROOT_PATH}/lib root@${IP}:/ | tee ${TEMP_PATH}/log/copy_lib.log
echo -e ${Yellow}"Upload /usr/include..."${Reset}
rsync -avz ${SYSROOT_PATH}/usr/include root@${IP}:/usr | tee ${TEMP_PATH}/log/copy_usr_include.log
echo -e ${Yellow}"Upload /usr/lib..."${Reset}
rsync -avz ${SYSROOT_PATH}/usr/lib root@${IP}:/usr | tee ${TEMP_PATH}/log/copy_usr_lib.log
echo -e ${Yellow}"Upload /opt/vc..."${Reset}
rsync -avz ${SYSROOT_PATH}/opt/vc root@${IP}:/opt | tee ${TEMP_PATH}/log/copy_opt_vc.log
echo -e ${Yellow}"Upload /usr/local..."${Reset}
rsync -avz ${SYSROOT_PATH}/usr/local root@${IP}:/usr | tee ${TEMP_PATH}/log/upload_usr_local.log
