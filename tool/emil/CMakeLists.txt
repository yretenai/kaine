project(TOOL_emil)

add_executable(emil emil.cpp)
target_link_libraries(emil kaine)
target_link_libraries(emil standard_dragon)

include(GNUInstallDirs)
install(TARGETS emil RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
