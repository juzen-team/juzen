cmake_minimum_required(VERSION 3.5)
project(juzen)

include(CheckCXXCompilerFlag)
function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
    endif()
endfunction()

enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-Wextra")
enable_cxx_compiler_flag_if_supported("-pedantic")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

find_package(Jellow REQUIRED)
if (NOT JELLOW_FOUND)
    message(FATAL_ERROR "Jellow not found!")
endif()

include_directories("src/" ${JELLOW_INCLUDE_DIRS})

set(SOURCE_FILES
    src/main.cpp)

add_executable(juzen ${SOURCE_FILES})
set_target_properties(juzen PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)
target_link_libraries(juzen Qt5::Core Qt5::Gui Qt5::Widgets ${JELLOW_LIBRARIES})

