find_path(CATCH_HEADER catch.hpp)
set_property(CACHE CATCH_HEADER PROPERTY ADVANCED TRUE)
if(NOT CATCH_HEADER)
    message(SEND_ERROR
" Catch header `catch.hpp' could not be found.
Catch is a single header unit test framework required to build some tests.
How to install:
If it is not available on your package manager, Catch can be downloaded from:
  https://raw.github.com/philsquared/Catch/master/single_include/catch.hpp
Then either:
  - append the download folder to the `CMAKE_INCLUDE_PATH' or `CMAKE_PREFIX_PATH' variable
  - copy it in a standard location (/usr/include on most linux distribution).")
endif()

add_library(catch STATIC catch.cpp)
target_include_directories(catch PUBLIC "${CATCH_HEADER}")

