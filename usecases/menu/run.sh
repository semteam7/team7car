#!/bin/bash
export conf=~/DIT-168 
export FREQ=20

PS3='> '
DEBUG=true

xhost + > /dev/null

echo "Select a configuration : " 

cd configs;
select FOLDER in *
do
	export conf=$PWD/$FOLDER
	break;
done
cd ..

$DEBUG && echo "Configuration path: $conf"

echo "Select which docker-compose files to include : "

select FILENAME in $(echo $(ls docker-compose.*.yml | sed 's/docker-compose.//;s/.yml//') 'CONTINUE')
do
	if [ $FILENAME == 'CONTINUE' ] 
	then
		break;	
	fi
	
	FILES+=($FILENAME)
done

FILEARG=" -f docker-compose.yml "

for i in "${FILES[@]}"
do
	FILEARG+="-f docker-compose.$i.yml " 
done

docker-compose $FILEARG up --build
