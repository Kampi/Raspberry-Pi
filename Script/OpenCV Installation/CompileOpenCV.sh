#!/bin/bash

##### User settings ######
CORES=4
IP=192.168.178.52
TEMP_PATH=/home/kampi/RaspberryOpenCV
SYSROOT_PATH=/home/kampi/Toolchain/RaspberryPi
TOOLCHAIN_PATH=/home/kampi/Toolchain/RaspberryPi
INSTALL_PATH=/usr/local
WITH_QT=ON
QT_PATH=/home/kampi/Toolchain/RaspberryPi/sysroot/usr/local/Qt
###########################

#### Colors #####
Red="\033[0;31m"
Green="\033[0;32m"
Yellow="\033[0;33m"
Cyan="\033[0;36m"
Reset="\033[0m"
#################

echo -e ${Yellow}"Create directories..."${Reset}
mkdir -p ${TEMP_PATH}/log ${SYSROOT_PATH}

echo -e ${Yellow}"Download OpenCV sources..."${Reset}
cd ${TEMP_PATH}
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git

echo -e ${Yellow}"Download /lib..."${Reset}
rsync -avz root@${IP}:/sysroot/lib ${SYSROOT_PATH} | tee ${TEMP_PATH}/log/copy_lib.log
echo -e ${Yellow}"Download /usr/include..."${Reset}
rsync -avz root@${IP}:/sysroot/usr/include ${SYSROOT_PATH}/sysroot/usr | tee ${TEMP_PATH}/log/copy_usr_include.log
echo -e ${Yellow}"Download /usr/lib..."${Reset}
rsync -avz root@${IP}:/usr/lib ${SYSROOT_PATH}/sysroot/usr | tee ${TEMP_PATH}/log/copy_usr_lib.log
echo -e ${Yellow}"Download /opt/vc..."${Reset}
rsync -avz root@${IP}:/opt/vc ${SYSROOT_PATH}/sysroot/opt | tee ${TEMP_PATH}/log/copy_opt_vc.log
echo -e ${Yellow}"Download /usr/local..."${Reset}
rsync -avz root@${IP}:/usr/local ${SYSROOT_PATH}/sysroot/usr | tee ${TEMP_PATH}/log/copy_usr_local.log

echo -e ${Yellow}"Change symlinks..."${Reset}
${TOOLCHAIN_PATH}/sysroot-relativelinks.py ${SYSROOT_PATH}

echo -e ${Yellow}"Configure build..."${Reset}
cd ${TEMP_PATH}/opencv
mkdir -p ${TEMP_PATH}/opencv/build && cd build
cmake -D CMAKE_C_COMPILER=${TOOLCHAIN_PATH}/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc \
	-D CMAKE_LIBRARY_PATH=${SYSROOT_PATH}/opt/vc/lib \
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
	-D WITH_QT=${WITH_QT} \
	-D Qt5_DIR=${QT_PATH}/lib/cmake/Qt5 \
	-D QT_QMAKE_EXECUTABLE=${QT_PATH}/bin/qmake \
	-D Qt5Concurrent_DIR=${QT_PATH}/lib/cmake/Qt5Concurrent \
	-D Qt5Core_DIR=${QT_PATH}/lib/cmake/Qt5Core \
	-D Qt5Gui_DIR=${QT_PATH}/lib/cmake/Qt5Gui \
	-D Qt5OpenGL_DIR=${QT_PATH}/lib/cmake/Qt5OpenGL \
	-D Qt5Test_DIR=${QT_PATH}/lib/cmake/Qt5Test \
	-D Qt5Widgets_DIR=${QT_PATH}/lib/cmake/Qt5Widgets \
	-D _qt5gui_OPENGL_INCLUDE_DIR=${SYSROOT_PATH}/opt/vc/include \
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
rsync -avz ${SYSROOT_PATH}/sysroot/lib root@${IP}:/ | tee ${TEMP_PATH}/log/copy_lib.log
ssh root@${IP} "chown -R root:root /lib"

echo -e ${Yellow}"Upload /usr/include..."${Reset}
rsync -avz ${SYSROOT_PATH}/sysroot/usr/include root@${IP}:/usr | tee ${TEMP_PATH}/log/copy_usr_include.log
ssh root@${IP} "chown -R root:root /usr/include"

echo -e ${Yellow}"Upload /usr/lib..."${Reset}
rsync -avz ${SYSROOT_PATH}/sysroot/usr/lib root@${IP}:/usr | tee ${TEMP_PATH}/log/copy_usr_lib.log
ssh root@${IP} "chown -R root:root /usr/lib"

echo -e ${Yellow}"Upload /opt/vc..."${Reset}
rsync -avz ${SYSROOT_PATH}/sysroot/opt/vc root@${IP}:/opt | tee ${TEMP_PATH}/log/copy_opt_vc.log
ssh root@${IP} "chown -R root:root /opt/vc"

echo -e ${Yellow}"Upload /usr/local..."${Reset}
rsync -avz ${SYSROOT_PATH}/sysroot/usr/local root@${IP}:/usr | tee ${TEMP_PATH}/log/upload_usr_local.log
ssh root@${IP} "chown -R root:root /usr/local"

echo -e ${Red}"Script finished!"${Reset}
