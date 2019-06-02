#!/usr/bin/env bash

index=(2 3 4)

for i in "${index[@]}"
    do
        echo $i
       ./PolicyChecker /Users/redwannewaz/Projects/research@quadrotor/PolicyChecker/Data/p_$i 1
    done