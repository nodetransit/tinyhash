cmake_minimum_required (VERSION 3.16)
project(tinyhash_test)

add_subdirectory("./../src" thash)

#---------------------------------------------------------------------

option (DEBUG "Compile with debug symbols." OFF)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set (DEBUG ON)
endif ()
if (NOT DEBUG)
    set (RELEASE ON)
    set (NDEBUG ON)
else ()
    set (DEBUG ON)
    set (_DEBUG ON)
    set (RELEASE OFF)
endif ()

option (VERBOSE "Verbose console output." OFF)
if (VERBOSE)
    set (CMAKE_VERBOSE_MAKEFILE ON)
endif ()

#---------------------------------------------------------------------

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (LINUX ON)
endif()

if (WIN32)
    # operating system for the spoiled and the losers
    set (LOSER ON)
endif ()
# no support for the weirdos using APPLE

#---------------------------------------------------------------------

macro(add_example name)
    add_executable(${name} ${name}.c)

    if (NOT MSVC)
        add_definitions("--pedantic")

        target_compile_options (${name} PRIVATE "-Wno-missing-braces"
                                                "-Wall")

        if (DEBUG)
            target_compile_options (${name} PRIVATE #"-Wno-bool-compare"
                                                    #"-Wno-incompatible-pointer-types"
                                                    "-Wno-unused-parameter"
                                                    #"-Wno-unused-variable"
                                                    "-Wno-unused-function"
                                                    "-Wno-format")
        endif ()

    endif ()

    if (DEBUG)
        message (STATUS "Compiling with DEBUG symbols")

        if (NOT MSVC)
            add_definitions (-g
                             #-Wall
                             #-Werror
                             #-Wfatal-errors
                             -fdiagnostics-show-option
                             -fmax-errors=3)
        else ()
            add_definitions ("-ZI")
        endif ()
    else ()
        if (NOT MSVC)
            target_compile_options (${name} PRIVATE "-O3"
                                                    "-c"
                                                    "-s")
        else ()
            target_compile_options (${name} PRIVATE "-Ox"
                                                    "-GF"
                                                    "-c"
                                                    "-W2"
                                                    "-GL")
        endif ()
    endif ()

    target_link_libraries(${name} "thash")
endmacro()

#---------------------------------------------------------------------

#add_example("aes")
add_example("base85")
#add_example("blowfish")
add_example("simhash")
