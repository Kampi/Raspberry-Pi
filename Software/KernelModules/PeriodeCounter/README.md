# Periode counter for Raspberry Pi

```Note: No device tree support yet!```

This module implement a simple frequency counter, build as loadable module, for the raspberry Pi.

## Table of Contents

- [Build it](#build-it)
- [Enable PWM on raspberry pi](#Enable-pwm-on-raspberry-pi)
- [How do i use it](#how-do-i-use-it)
- [History](#history)

## Build it

- Get the sources for the kernel and the cross compiler from [here](https://github.com/raspberrypi)
- Export the variables `Raspberry` and `Raspberry_Kernel` for the cross compiler and the kernel sources

```
# Cross compiler
export Raspberry=~<Path>/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf

# Linux kernel
export Raspberry_Kernel=<Path>/linux
```

- Run `make`

## Enable PWM on raspberry pi

This example use the default `GPIO18` for a 100 Hz signal with 80% duty cycle.

1. ```$ cd /sys/class/pwm/pwmchip0```
2. ```$ echo 0 > export```
3. ```$ echo 10000000 > pwm0/period```
4. ```$ echo 8000000 > pwm0/duty_cycle```
5. ```$ echo 1 > pwm0/enable```

## How do I use it

1. Connect the pwm signal with `GPIO17`.
2. Load the module   
`$ insmod ReadPeriode.ko`
3. A device `/dev/PeriodeCounter` is now available.
- You can set the resolution by writing the unit to `/proc/PeriodeCounter/Resolution`. The units `ms`, `us` and `ns` are supported.   
```$ sudo echo ms > /proc/PeriodeCounter/Resolution```
- Now you can read out the periode. The result contain the periode.   
`$ sudo cat /dev/PeriodeCounter`

## History

| **Version**  | **Description**                                | **Date**       |
|:---------:|:------------------------------------------:|:----------:|
| 1.0       | First release                              | 06.08.2018 |

## Maintainer

- [Daniel Kampert](DanielKampert@kampis-elektroecke.de)