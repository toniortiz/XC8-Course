#!/bin/bash

delay=0.05s
port=/dev/ttyS0
A0=$1
A1=$2

sudo chmod o+rw $port

printf "@" > $port
sleep $delay
printf $A0 > $port
sleep $delay
printf $A1 > $port
sleep $delay
printf "#" > $port

