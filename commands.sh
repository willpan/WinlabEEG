#! /bin/bash

source ~/.bashrc
export ROS_MASTER_URI=http://192.168.207.2:11311
export ROS_HOSTNAME=william-Q430-Q530
export ROS_IP=localhost

case $1 in
    "3")
	echo "Drive Forward"
	rosservice call tank true 350 350 &
	;;
    "2")
	echo "Turn Right"
	rosservice call turn true 150 &
	;;
    "1")
	echo "Turn Left"
	rosservice call turn -- true -150 &
	;;
    "4")
	echo "Brake True"
	rosservice call brake true &
	;;
esac

