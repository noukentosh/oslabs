#!/bin/bash

#COMPILE
g++ -c writer.cpp
g++ -c reader.cpp

#LINKING
g++ -o writer writer.o -pthread
g++ -o reader reader.o -pthread

#RUN
#./writer