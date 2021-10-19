#!/bin/bash

#COMPILE
g++ -c master.cpp
g++ -c slave.cpp

#LINKING
g++ -o master master.o -lpthread
g++ -o slave slave.o -lpthread

#RUN
./master