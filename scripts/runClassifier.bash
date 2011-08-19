#! /bin/bash

#. ~/WinlabEEG/scripts/variables.bash
SCRIPTS_DIR="~/WinlabEEG/scripts"

result=`Rscript $SCRIPTS_DIR/classifyCSV.R $1`
result=`echo $result | awk 'BEGIN{ FS = "=" }{if ( $0 ~ FINAL_DECISION ) { print $2 }}'`

case $result in
	"1")
		$SCRIPTS_DIR/runCmd.bash 1 ;;
	"2")
		$SCRIPTS_DIR/runCmd.bash 2 ;;
	*)
		echo "[WARNING] Not Able to make Cogent Decision"
		exit 1;
esac
