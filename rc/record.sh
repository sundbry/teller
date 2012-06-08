#!/bin/bash

recordButton() {
	echo "Press the $1 button"
	igclient --receiver-on --sleep 5 > $remote/$1.sigin
	./inout.pl $remote/$1
}

remote=$1

rm -rf $remote
mkdir $remote

recordButton "power"
recordButton "mute"
recordButton "volumeup"
recordButton "volumedown"
recordButton "channelup"
recordButton "channeldown"

