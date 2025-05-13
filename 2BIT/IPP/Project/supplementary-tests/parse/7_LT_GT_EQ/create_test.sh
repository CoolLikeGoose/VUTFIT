#!/bin/bash
# $1 - name
# $2 - exit code

touch $1.src
echo -n $2 > $1.rc
touch $1.out
code $1.src $1.out