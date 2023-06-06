#!/bin/bash

docker run --rm --name make -ti -v .:/src -v $PWD/../output:/output fairbrook/gcc-arm-linux-gnueabihf ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- O=/output ${@:1}
