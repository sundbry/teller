#!/bin/bash
igclient --receiver-on --sleep 5 > $1/$2.sigin
./inout.pl $1/$2
