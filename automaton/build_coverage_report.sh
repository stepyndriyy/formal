./bin/tests
lcov -t "bin/tests" -o tests.info -c -d build/CMakeFiles/tests.dir/tests/practice1/
genhtml -o report tests.info
rm tests.info
