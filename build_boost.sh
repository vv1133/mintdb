#!/bin/bash

machine=`uname -m | grep 64`

if [ -z $machine ];then
	bit=32
else
	bit=64
fi

cd boost
./bootstrap.sh --prefix=./
./bjam link=static threading=multi variant=release address-model=$bit toolset=gcc runtime-link=static
