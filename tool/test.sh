#!/usr/local/bin/bash

failed=0; # number of failed cases

NBL_FILES=$(find test -name '*.nbl');

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
    echo "All test cases passed";
else
    echo "Total failed test cases: $failed";
fi;

exit $failed
