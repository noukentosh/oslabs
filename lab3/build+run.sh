#!/bin/bash

#COMPILE
g++ -c lab.cpp
g++ -c lab_with_block.cpp
g++ -c lab_with_block2.cpp

#LINKING
g++ -o lab lab.o -lpthread
g++ -o lab_with_block lab_with_block.o -lpthread
g++ -o lab_with_block2 lab_with_block2.o -lpthread


#RUN
echo "";
echo "Обычная версия";
./lab
echo "";
echo "Версия с пустым pipeline";
./lab_with_block
echo "";
echo "Версия с полным pipeline";
./lab_with_block2