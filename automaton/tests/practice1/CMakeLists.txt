set(COVERAGE_NAME operation_coverage)
set(COVERAGE_TARGET operation_coverage_library)
set(TEST_SOURCES operations_test.cpp)
set(TARGET_NAME "operations")

run_coverage(
    ${TEST_SOURCES}
    ${TARGET_NAME}
)
