#!/bin/bash
if [ -d bin ]
then 
    cd bin/release 
    ./cppnode $1 $2
    cd ..
    cd ..
fi