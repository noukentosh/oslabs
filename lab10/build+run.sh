#!/bin/bash

#COMPILE
g++ -c master.cpp
g++ -c slave.cpp

#LINKING
g++ -o master master.o -lev
g++ -o slave slave.o

#RUN
./master