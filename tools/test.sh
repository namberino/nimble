#!/usr/bin/env bash

#------------------------------------#
# Copyright 2024 Nam Nguyen
# Licensed under Apache License v2.0
#------------------------------------#

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

failed=0; # number of failed cases

NBL_FILES=$(find tests -name '*.nbl');

for nbl in $NBL_FILES; do
    # get expected output
    expected=${nbl}.expected;

    # run and get difference between output and expected output
    echo "Running test case $nbl...";
    if ! ./bin/nimble $nbl | diff -u --color "$expected" -; then
        echo "Test case $nbl failed!";
        failed=$((failed + 1)); # count failed cases
    fi;
done;

if [ $failed -eq 0 ]; then
    echo;
    echo -e "${GREEN}All test cases passed${NC}";
else
    echo;
    echo -e "${RED}Total failed test cases: $failed${NC}";
fi;

exit $failed
