#!/bin/bash

source_files=(
	"colors.c"
	"FCFS.c"
	"HRRN.c"
	"LCFS.c"
	"MLF.c"
	"PRIOP.c"
	"queue.c"
	"RR.c"
	"scheduler.c"
	"SRTNP.c")
lib_files=(
	"colors.h"
	"FCFS.h"
	"HRRN.h"
	"LCFS.h"
	"MLF.h"
	"PRIOP.h"
	"process.h"
	"queue.h"
	"RR.h"
	"scheduler.h"
	"SRTNP.h"
	)

mkdir -p submission
mkdir -p submission/src
mkdir -p submission/lib


if [ -f "submission/studentinfo.txt" ]; then
	name=$(<submission/studentinfo.txt)
else 
	read -p "Bitte gib deinen vollständigen Namen an: " name
	echo $name > submission/studentinfo.txt
fi
for file in "${source_files[@]}"
do
	cp "src/$file" "submission/src/$file"
	echo -e "/*\n * Created by $name\n */" >> "submission/src/$file"
done

for file in "${lib_files[@]}"
do
	cp "lib/$file" "submission/lib/$file"
	echo -e "/*\n * Created by $name\n */" >> "submission/lib/$file"
done
name_subst="${name/ /_}"

zip -r "${name_subst}_HA1_submission.zip" submission