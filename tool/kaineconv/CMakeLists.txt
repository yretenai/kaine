project(TOOL_kaineconv)

add_executable(kaineconv kaineconv.cpp)
target_link_libraries(kaineconv kaine)
target_link_libraries(kaineconv standard_dragon)

include(GNUInstallDirs)
install(TARGETS kaineconv RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
