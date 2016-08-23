#!/bin/bash

echo 'Building and compiling...'
cd src/assembler
make -f make_asm.mk
make -f make_asm.mk clean
sudo mkdir -p /etc/sasm
sudo cp optab.tab /etc/sasm
cd ../..
echo 'Done!'

echo 'Installing...'
sudo cp -rf bin/sasm /usr/bin
echo 'Done!'
