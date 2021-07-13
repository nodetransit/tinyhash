cmake_minimum_required (VERSION 3.16)
project (tinyhash VERSION 0.1.0 DESCRIPTION "Tiny collection of hashing algorithms.")

set (TINYHASH_MAJOR_VERSION 0)

include (CheckIncludeFiles)

#---------------------------------------------------------------------

option (DEBUG "Compile with debug symbols." OFF)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set (DEBUG ON)
endif ()

if (NOT DEBUG)
    set (RELEASE ON)
    set (DEBUG OFF)
    set (_DEBUG OFF)
    set (NDEBUG ON)
else ()
    set (RELEASE OFF)
    set (DEBUG ON)
    set (_DEBUG ON)
    set (NDEBUG OFF)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set (DEBUGINFO ON)
endif ()

option (VERBOSE "Verbose console output." OFF)
if (VERBOSE)
    set (CMAKE_VERBOSE_MAKEFILE ON)
endif ()

option (TINYHASH_EXPORT_LIB "Build Shared Libraries." ON)
if (TINYHASH_EXPORT_LIB)
    set (TINYHASH_LIB_TYPE SHARED)
else ()
    set (TINYHASH_LIB_TYPE STATIC)
endif ()

# BASE85 | Z85 | ASCII85
option (TINYHASH_BASE85_VARIANT "Code table used for base85." "BASE85")

#---------------------------------------------------------------------

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (LINUX ON)
endif ()

if (WIN32)
    # operating system for the spoiled and the losers
    set (LOSER ON)
endif ()
# no support for the weirdos using APPLE

#---------------------------------------------------------------------

#file (GLOB_RECURSE SOURCE_FILES "." "*.c")
set (SOURCE_FILES "base85.c"
                  )

set (BINARY_NAME "thash")
add_library (${BINARY_NAME} ${TINYHASH_LIB_TYPE} ${SOURCE_FILES})

#---------------------------------------------------------------------

set (COMMON_COMPILE_OPTIONS "")
set (COMMON_DEBUG_COMPILE_OPTTIONS "")

if("${TINYHASH_BASE85_VARIANT}" STREQUAL "Z85")
    set(TINYHASH_BASE85_VARIANT "BASE85_VARIANT_Z85")
elseif ("${TINYHASH_BASE85_VARIANT}" STREQUAL "ASCII85")
    set(TINYHASH_BASE85_VARIANT "BASE85_VARIANT_ASCII85")
else ()
    set(TINYHASH_BASE85_VARIANT "BASE85_VARIANT_BASE85")
endif ()

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/base85.in.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/base85.h"
                @ONLY)

#---------------------------------------------------------------------

if (NOT MSVC)
    list (APPEND COMMON_COMPILE_OPTIONS "-Wno-missing-braces"
                                        "-Wall"
                                        "-Wextra"
                                        #-Wfatal-errors
                                        "-fno-strict-aliasing")
    #add_definitions ("--pedantic"
    #                 "-ansi")

    if (DEBUG)
        list (APPEND COMMON_DEBUG_COMPILE_OPTTIONS #"-Wno-bool-compare"
                                                   #"-Wno-incompatible-pointer-types"
                                                   "-Wno-unused-parameter"
                                                   #"-Wno-unused-variable"
                                                   "-Wno-unused-function"
                                                   "-Wno-format")
    endif ()

endif ()

if (DEBUG OR DEBUGINFO)
    message (STATUS "Compiling with DEBUG symbols")

    if (NOT MSVC)
        add_definitions (-g3
                         -gz=zlib
                         -fdiagnostics-show-option
                         -femit-class-debug-always
                         -fvar-tracking
                         -fmax-errors=3)
    else ()
        add_definitions ("-ZI")
    endif ()
endif ()

if (NOT DEBUG)
    message (STATUS "Compiling with optimizations")

    if (NOT MSVC)
        list (APPEND COMMON_COMPILE_OPTIONS "-O3"
                                            "-s"
                                            "-c"
                                            "-D_FORTIFY_SOURCE=2")
    else ()
        list (APPEND COMMON_COMPILE_OPTIONS "-Ox"
                                            "-GF"
                                            "-c"
                                            "-W2"
                                            "-GL")
    endif ()
endif ()

#---------------------------------------------------------------------

if (${TINYHASH_LIB_TYPE} STREQUAL "STATIC")
    list (APPEND COMMON_DEBUG_COMPILE_OPTTIONS "-fPIC"
          )
elseif (${TINYHASH_LIB_TYPE} STREQUAL "SHARED")
    if (LOSER)
        set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

        if (MSVC)
            set_target_properties (${BINARY_NAME} PROPERTIES OUTPUT_NAME "${BINARY_NAME}"
                                                  ARCHIVE_OUTPUT_NAME "${BINARY_NAME}"
                                                  LINK_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_INIT} \/DYNAMICBASE")
        endif ()

        if (DLLTOOL)
            set_target_properties (${BINARY_NAME} PROPERTIES OUTPUT_NAME "${BINARY_NAME}"
                                                  ARCHIVE_OUTPUT_NAME "${BINARY_NAME}"
                                                  LINK_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_INIT} -Wl,--output-def=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${BINARY_NAME}.def")

            add_custom_command(TARGET ${BINARY_NAME} POST_BUILD
                               WORKING_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
                               COMMAND echo "Generating import library"
                               COMMAND "${DLLTOOL}" --dllname "lib${BINARY_NAME}.dll"
                                                    --input-def "lib${BINARY_NAME}.def"
                                                    --output-lib "lib${BINARY_NAME}.lib"
                                                    #todo add --as-flags=--64
                                                    )

        endif ()
    endif ()

    #if (LINUX)
    #    set_target_properties (${BINARY_NAME} PROPERTIES OUTPUT_NAME "${BINARY_NAME}"
    #                           ARCHIVE_OUTPUT_NAME "${BINARY_NAME}"
    #                           LINK_FLAGS ${CMAKE_SHARED_LINKER_FLAGS_INIT}
    #                                      "-Wl,-rpath=./")
    #
    #endif ()

    set_target_properties(${BINARY_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
    set_target_properties(${BINARY_NAME} PROPERTIES SOVERSION ${TINYHASH_MAJOR_VERSION})
endif ()

target_compile_options (${BINARY_NAME} PRIVATE ${COMMON_COMPILE_OPTIONS}
                        ${COMMON_DEBUG_COMPILE_OPTTIONS})
set_property (TARGET ${BINARY_NAME} PROPERTY CXX_STANDARD 11)

target_include_directories(${BINARY_NAME}
                           PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                                  "$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")

#---------------------------------------------------------------------
