#! /bin/bash

source ~/WinlabEEG/scripts/variables.bash

USAGE="classRun\n\nSYNOPSIS\n\t./classRun.bash [-c] filename [-d] data"

if [ "$#" == "0" ]; then
	echo -e $USAGE
fi

update_var () {
	cat $HDIR/scripts/variables.bash | awk -v varName="$VAR_NAME" -v varval="$VAR_VAL" '{if ( $0 ~ varName ) { print varName"="varVal } else {print $0}}' > $HDIR/scripts/variables.bash
}

reset_var () {
	VAR_NAME="DRIVE_SPEED"
	VAR_VAL="$MIN_DRIVE_SPEED"
	update_var
	VAR_NAME="TURN_SPEED"
	VAR_VAL="$MIN_SPEED_SPEED"
	update_var
}

same_drive () {
	if [ $DRIVE_SPEED -lt $MAX_DRIVE_SPEED ]; then
		DRIVE_SPEED=$(($DRIVE_SPEED + $INCR_DRIVE_SPEED))
		SPEED=$DRIVE_SPEED
	fi
	VAR_NAME="DRIVE_SPEED"
	VAR_VAL="$DRIVE_SPEED"
	update_var
}

same_turn () {
	if [ $TURN_SPEED -lt $MAX_TURN_SPEED ]; then
		TURN_SPEED=$(($TURN_SPEED + $INCR_TURN_SPEED))
		SPEED=$TURN_SPEED
	fi
	VAR_NAME="TURN_SPEED"
	VAR_VAL="$TURN_SPEED"
	update_var
}

do_action () {
	if [ "$NEW_ACTION" == "$LAST_ACTION" ]; then
		case $NEW_ACTION in
			"1")
				same_drive
				;;
			"2")
				same_turn
				;;
			"3")	
				same_turn
				;;
			"4")
				same_drive
				;;
		esac
	else
		reset_var	
	fi
	$HDIR/scripts/runCommand $NEW_ACTION $LAST_ACTION $SPEED
}

while getopts ":c:d:" opt; do
	case $opt in
		c)
			echo "You have chosen CSV option with OPTARG = $OPTARG and 1 = $1"
			if [[ -e $OPTARG ]]; then
				echo "EEX"
				NEW_ACTION=`Rscript $HDIR/scripts/classifyCSV.r $OPTARG | awk 'BEGIN{ FS = "=" }{if ( $0 ~ '/OUTPUT/' ) { print $2 }}'`
				if [ "$NEW_ACTION" != "0" ]; then
					do_action
				fi				
				VAR_NAME="LAST_ACTION"
				VAR_VAL="$NEW_ACTION"
				update_var
			else
				echo "[ERROR] CSV filename does not exist"
				exit 0
			fi
			;;
		d)
			echo "You have chosen data option with OPTARG = $OPTARG and 1 = $1"
			shift 1
			if [ "$#" == "$NUM_SENSORS" ]; then
				echo "EED"
				NEW_ACTION=`Rscript $HDIR/scripts/classifyRAW.r $@ | awk 'BEGIN{ FS = "=" }{if ( $0 ~ '/OUTPUT/' ) { print $2 }}'`
				if [ "$NEW_ACTION" != "0" ]; then				
					do_action
				fi
				VAR_NAME="LAST_ACTION"
				VAR_VAL="$NEW_ACTION"
				update_var
			else
				echo "[ERROR] Invalid argument - Not correct amount of sensor data"
				exit 0
			fi
			;;
		\?)
			echo "[ERROR] Invalid Option -$OPTARG"
			exit 0
			;;
		:)
			echo "[ERROR] Option -$OPTARG requires an argument."
			exit 0
			;;
	esac
done
