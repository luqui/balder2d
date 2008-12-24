#!/bin/bash
# This script is for running balder2d if it is installed system wide.
# The prefix below should be changed to where it is installed.

lib_prefix=/usr/share/games
bin_prefix=/usr/lib
if  [ -n "$1" ]
then
CONFIG_FILE=--config $1
else
CONFIG_FILE=
fi

${bin_prefix}/balder2d/balder2d --data $lib_prefix/balder2d $CONFIG_FILE
