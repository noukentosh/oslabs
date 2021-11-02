#!/bin/bash

#COMPILE
g++ -c server.cpp
g++ -c client.cpp

#LINKING
g++ -o server server.o -pthread
g++ -o client client.o -pthread

#RUN
#./writer