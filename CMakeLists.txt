# Copyright (c) 2016 Intel Corporation
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.2.0)

project(libstructure)

option(COVERAGE "Build with coverage support" OFF)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Required to include the export header
set(CMAKE_INCLUDE_CURRENT_DIR ON)

list(APPEND CMAKE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/external")

if (UNIX)
    add_compile_options(-Wall -Wextra -Werror)
endif()

if(WIN32)
    # Force include iso646.h to support alternative operator form (and, or, not...)
    #
    # Suppress warning 4251 (needs to have dll-interface to be used by clients)
    # Exporting classes containing attributes from STL trigger the warning C4251
    # Thoses are private attributes so this is not an issue here
    add_compile_options(/W4 /FIiso646.h -wd4251)
endif()

# Since there is no directory-wide property for linker flags, we can't use
# set_property for the link-time coverage flags.
if(COVERAGE)
    list(APPEND CMAKE_CXX_FLAGS "--coverage")
    list(APPEND CMAKE_EXE_LINKER_FLAGS "--coverage")
endif()

# Hide symbols by default, then exposed symbols are the same in linux and windows
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN true)

# Use a common binary output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

include(ctest/CMakeLists.txt)

include(CMakeDependentOption)
option(DOXYGEN "Enable doxygen generation" ON)

add_subdirectory(doxygen)

add_subdirectory(structure)
add_subdirectory(demo)
add_subdirectory(binary-export)
add_subdirectory(test)
