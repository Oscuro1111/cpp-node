#!/bin/bash
if [ -d "bin/release" ]
then 
		echo "checked build directory exist name: bin ...done [OK]"
else
		echo "creating building directory \"bin\""&& mkdir -p bin/release
fi


if [ -d temp ]
then
			echo "temp...[ok]"
else
			mkdir temp
			echo "temp dir is created"
fi


