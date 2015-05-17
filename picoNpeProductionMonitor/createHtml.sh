#!/bin/bash
if [ "$1" == "" ]; then
	echo "No input file."
elif [ -e $1 ]; then
	cp index.md index.temp.md
	echo "Last updated on" `date` >> index.temp.md
	root -l -b -q plotPicoNpeProductionQa.C\(\"$1\"\) 
	pandoc --standalone --mathjax --from markdown --to html -o index.html index.temp.md
	rm index.temp.md
	scp *.png index.html pdsf.nersc.gov:~/www/run14/picoNpeProductionMonitor/
else 
	echo "Wrong input file."
fi
