#!/bin/bash

#COMPILE
g++ -c lab.cpp

#LINKING
g++ -o lab lab.o -lpthread

#RUN
./lab