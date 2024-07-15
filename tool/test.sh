#!/bin/sh
NBL_FILES=$(find test -name '*.nbl')

failed=0;
for nbl in $NBL_FILES; do
    expected=${nbl}.expected;
    echo "Running test case $nbl...";
    if ! ./bin/nimble $nbl | diff -u --color "$expected" -; then
        echo "Test case $nbl failed!";
        failed=$((failed + 1));
    fi;
done;

if [ $failed -eq 0 ]; then
    echo;
    echo "All test cases passed";
else
    echo "Total failed test cases: $failed";
fi;

exit $failed
