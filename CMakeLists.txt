cmake_minimum_required(VERSION 3.16)
project(kaine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    set(WIN32_CLANG ON)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_definitions(_WIN32_CLANG)
endif ()

if (MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_options("/wd4068")
    add_compile_options("/std:c++latest")
else ()
    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        add_compile_options("-fpermissive")
    endif ()
endif ()

include(CheckIncludeFileCXX)

CHECK_INCLUDE_FILE_CXX("__pstl_execution" HAS_PARALLEL_ALGORITHMS)

if (HAS_PARALLEL_ALGORITHMS)
    add_compile_definitions("_LIBCPP_HAS_PARALLEL_ALGORITHMS")
endif ()

include(GenerateExportHeader)

add_subdirectory("vendor")

find_package(zstd CONFIG REQUIRED)

add_subdirectory("kaine")
add_subdirectory("tool")

include(GNUInstallDirs)
install(DIRECTORY licenses DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DOCDIR}/licenses RENAME kaine)
