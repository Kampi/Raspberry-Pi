# Raspberry Pi Weather Station

Weather station for the Raspberry Pi. This station uses weather data from [OpenWeatherMap](https://openweathermap.org) to visualize the current weather.
Otherwise send an [e-mail](DanielKampert@kampis-elektroecke.de) if you have any questions.

## Table of Contents

- [Weather](#weather)
    - [Table of Contents](#table-of-contents)
    - [Status of the project](#status-of-the-project)
    - [Prepare the raspberry pi](#prepare-the-raspberry-pi)
    - [Use it](#use-it)
	- [Image sources](#image-sources)
    - [History](#history)
    - [Maintainer](#maintainer)

## Status of the project

__Needs rework!__

## Prepare the raspberry pi

 - Install node-red
 - Install all neccessary node-red packages

## Use it

## Packages

node-red-contrib-input-split
node-red-dashboard
node-red-node-sqlite
node-red-contrib-fs-ops
mysql-server python-mysqldb

## Image sources

 - [White arrow](https://www.iconsdb.com/white-icons/arrow-up-8-64.html)
 - [Weather icons](https://github.com/manifestinteractive/weather-underground-icons)
 - [UI icons](https://material.io/tools/icons/?icon=delete&style=baseline)

## Raspberry Pi settings

 - Disable HDMI
 - Boot to desktop
 - Set Chromium with node-red flow to autostart
 - Remove local chromium data after closing the browser (Cookies menu)
 - Set language in config tool
 - W-LAN, 700 MHz clock
 - node-red autostart

## History

| **Version**   | **Description**                                | **Date**       |
|:---------:|:------------------------------------------:|:----------:|
| 0.1       | First release                              |            |

## Maintainer

- [Daniel Kampert](@Kampi)
