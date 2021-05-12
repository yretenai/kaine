project(kaine)

add_library(kaine)
set_target_properties(kaine PROPERTIES POSITION_INDEPENDENT_CODE 1)
set_target_properties(kaine PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
target_compile_definitions(kaine PRIVATE DRAGON_LIBRARY_NAME="kaine")

if (BUILD_SHARED_LIBS)
    if (MSVC)
        generate_export_header(kaine BASE_NAME kaine DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/public/kaine/generated/kaine_export_win.a.h)
    elseif (WIN32_CLANG)
        generate_export_header(kaine BASE_NAME kaine DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/public/kaine/generated/kaine_export_clang.a.h)
    else ()
        generate_export_header(kaine BASE_NAME kaine DEFINE_NO_DEPRECATED EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/public/kaine/generated/kaine_export_linux.a.h)
    endif ()
else ()
    target_compile_definitions(kaine PUBLIC KAINE_STATIC)
endif ()

target_include_directories(kaine PUBLIC ${PROJECT_SOURCE_DIR}/public)

add_subdirectory("private")

target_link_libraries(kaine PUBLIC standard_dragon)

if (MSVC OR WIN32_CLANG)
    target_link_libraries(kaine PUBLIC zstd::libzstd_shared)
else ()
    target_link_libraries(kaine PUBLIC zstd::libzstd_static)
endif ()

file(GLOB_RECURSE kaine_HEADERS "public/*.h" "public/*.hpp")

include(GNUInstallDirs)
if (NOESIS_BUILD)
    install(TARGETS kaine RUNTIME DESTINATION ${NOESIS_DIR})
endif ()
install(TARGETS kaine LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY public/kaine DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h" PATTERN "*.fbs")
