#!/bin/bash

Images=20

mkdir "Calibration"

for ((Image=1;Image<=Images;Image++)); do
	raspistill -o "Calibration/$Image.jpg" --width 2592 --height 1944
done
