#!/bin/bash
export conf=~/DIT-168 
export FREQ=20
xhost +
select FOLDER in configs/*
do echo $FOLDER

 export conf=$PWD/$FOLDER
	break;


done

CONTINUE="CONTINUE"

touch "$CONTINUE"

select FILENAME in docker-compose.*.yml $CONTINUE
do
	if [ $FILENAME == $CONTINUE ] 
	then
		break;	
	fi
	
	FILES+=($FILENAME)
done

rm "$CONTINUE"

FILEARG=" -f docker-compose.yml "

for i in "${FILES[@]}"
do
	FILEARG+="-f $i " 
done

docker-compose $FILEARG up --build
