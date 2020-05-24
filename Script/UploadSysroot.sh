#!/bin/bash

##### User settings ######
IP=192.168.178.52
TEMP_PATH=/tmp
SYSROOT_PATH=/home/kampi/Toolchain/RaspberryPi
###########################

#### Colors #####
Red="\033[0;31m"
Green="\033[0;32m"
Yellow="\033[0;33m"
Cyan="\033[0;36m"
Reset="\033[0m"
#################

mkdir -p ${TEMP_PATH}/log

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
