#!/bin/bash

#COMPILE
g++ -c lab.cpp
g++ -c lab2.cpp

#LINKING
g++ -o lab lab.o -pthread
g++ -o lab2 lab2.o -pthread

#RUN
./lab