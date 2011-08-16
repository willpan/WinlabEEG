#!/bin/bash

. ~/.bashrc

USAGE="Usage: ./runCommand.bash newAction lastAction speed\n\tParameters:\n\t\tnewAction :: one of [1-4] representing the new action to take place\n\t\tlastAction :: one of [1-4] representing the last action that has taken place\n\t\tspeed :: speed in mm/s in which to perform the action\n\tNote about commands:\n\t\t1 :: Drive the iRobot Create forward\n\t\t2 :: Turn the iRobot Create rightwards\n\t\t3 :: Turn the iRobot Create leftwards\n\t\t4 :: Drive iRobot Create backwards\n\n\tN.B. If the lastAction does not match newAction, then the iRobot Create will brake. lastAction must equal newAction in order for the iRobot Create to execute the desired action."

if [ "$#" == "0" ]; then
	echo -e $USAGE
	exit 1
fi

if [ "$#" != "3" ]; then
	echo "[ERROR] Not Correct # of Arguments"
	echo -e $USAGE
	exit 0;
fi

if [ "$2" == "0" ]; then
	$2=$1
fi

if [ "$1" == "$2" ]; then
	case $1 in
		"1")
			echo "rosservice call /tank true $3 $3";;
			#rosservice call /tank true $3 $3;;
		"2")
			echo "rosservice call /turn true $3";;
			#rosservice call /turn true $3;;
		"3")
			echo "rosservice call /turn -- true -$3";;
			#rosservice call /turn -- true -$3;;
		"4")
			echo "rosservice call /tank -- true -$3 -$3";;
			#rosservice call /tank -- true -$3 -$3;;
		*)
			echo "[ERROR] Invalid Argument"
			echo -e $USAGE
			exit 0;
	esac
else
	echo "rosservice call /brake true"
	#rosservice call /brake true
fi
