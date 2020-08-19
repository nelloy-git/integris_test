#!/bin/bash

dir="./build/Tests"

if [ -e $dir ] ; then
    rm -r $dir
fi

dir="./build"
mkdir $dir
cd $dir
cmake ..
make -j6
make test