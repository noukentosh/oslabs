#!/bin/bash

#COMPILE
g++ -c slave.cpp
g++ -c master.cpp

#LINKING
g++ -o slave slave.o -lcap
g++ -o master master.o -lcap

./slave
echo "---"

echo "Попытка записать возможности для slave без CAP_SETFCAP"
./master
echo "---"

echo "Даем возможность CAP_SETFCAP"
sudo setcap cap_setfcap=+eip ./master
echo "---"

echo "Попытка записать возможности для slave c CAP_SETFCAP"
./master
echo "---"

./slave
echo "---"