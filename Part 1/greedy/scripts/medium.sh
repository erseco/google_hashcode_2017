#! /bin/bash

if [ "$1" != "" ]; then
    TIMES=$1
else
    TIMES=1
fi
for (( c=0; c<$TIMES; c++ ))
do
  ../bin/greedy ../cases/medium.in
done
