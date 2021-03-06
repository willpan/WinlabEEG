#! /bin/bash

. ~/.bashrc

export ROS_MASTER_URI=http://192.168.207.2:11311
export ROS_HOSTNAME=william-Q430-Q530
export ROS_IP=localhost

case $1 in
	"1")
		echo "rosservice call /tank true 200 200"
		rosservice call /tank true 200 200;;
	"2")
		echo "rosservice call /brake true"
		rosservice call /brake true;;
	"3")
		echo "rosservice call /turn true 200"
		rosservice call /turn true 200;;
	"4")
		echo "rosservice call /turn -- true -200"
		rosservice call /turn -- true -200;;
	*)
		echo "[ERROR] Invalid Argument"
		exit 0;
esac
