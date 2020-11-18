#!/bin/bash

DATA=$(date)
VOL=$( amixer get Master | awk -F '[][]' 'END{ print $2 }' )
MEMUSE=$( free -h | awk '(NR == 2){ print $3 }' )
DISKFREE=$(df -h | awk '{ if ($6 == "/") print $4}')
# Obsoleto após utilização do cbatticon
#BATERIA=$(cat /sys/class/power_supply/BAT0/capacity)

echo " /: $DISKFREE |  : $MEMUSE |  : $VOL |  : $DATA "
