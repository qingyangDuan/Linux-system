#!/bin/bash
while (( 1==1)) 
do
	echo "sourceName destName ip"
	read s d ip
	if [ $s -eq 0 ]
	then
		sudo ip link set $d down
		sudo ifconfig $d $ip netmask 255.255.255.0 up
	else	
		sudo ip link set $s down
		sudo ip link set $s name $d
		sudo ifconfig $d $ip netmask 255.255.255.0 up
	fi
done