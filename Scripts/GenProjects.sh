#!/bin/bash

if ! command -v cmake &> /dev/null
then
    echo "Cmake is missing, please install cmake version 3.20 and up."
    read -p "Press any key to continue..."
    exit 1
fi

if ! command -v pkg-config &> /dev/null
then
    echo "pkg-config is missing, please install pkg-config version 0.29 and up."
    read -p "Press any key to continue..."
    exit 1
fi

cmake -S ../ -B ../Build/
read -p "Press any key to continue..."
