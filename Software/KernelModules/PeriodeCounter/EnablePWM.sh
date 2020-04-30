#!/bin/bash

cd /sys/class/pwm/pwmchip0
echo 0 > export
echo 10000000 > pwm0/period
echo 8000000 > pwm0/duty_cycle
echo 1 > pwm0/enable
