#!/usr/bin/bash

qmake-qt5 -o Makefile kulanday.pro
make

if [ $? -eq 0 ]
then
    ./debug/kulanday
fi
