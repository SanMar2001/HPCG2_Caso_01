#!/bin/bash

> timesV1.doc
> timesV2.doc
> timesV3.doc

for j in {1..10}
do
    for i in 10 100 200 400 600 800 1000 2000
    do
        ./mXmV1 $i >> timesV1.doc
    done
done

for j in {1..10}
do
    for i in 10 100 200 400 600 800 1000 2000
    do
        for k in 2 4 8 16 32
        do
            ./mXmV2 $i $k >> timesV2.doc
            ./mXmV3 $i $k >> timesV3.doc
        done
    done
done