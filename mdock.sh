#!/bin/bash

docker run --name make -ti -v .:/src -v $PWD/../output:/output fairbrook/gcc-arm-linux-gnueabihf ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- O=/output ${@:1}
docker rm make
