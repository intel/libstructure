# Ctest configuration variables must be set BEFORE include(Ctest)

# Check process children and give detail for each leak
set(MEMORYCHECK_COMMAND_OPTIONS
    "${MEMORYCHECK_COMMAND_OPTIONS} --trace-children=yes --leak-check=full")

# As dash is not used to submit results, there is no way to see valgrind result.
# Force it to log to stderr and fail in case of leak or error.
set(MEMORYCHECK_COMMAND_OPTIONS
    "${MEMORYCHECK_COMMAND_OPTIONS} --error-exitcode=255 --log-fd=2")

# Enable tests, coverage, memcheck, ...
# See http://www.cmake.org/Wiki/CMake/Testing_With_CTest#Dashboard_Preparation
include(CTest)