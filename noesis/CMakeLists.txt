project(noesis)

add_library(fmt_replicant SHARED fmt_replicant.cpp
        support/noesisplugin.cpp support/pluginsupport.cpp
        XAPFile.cpp)
target_link_libraries(fmt_replicant PUBLIC kaine)
install(TARGETS fmt_replicant ARCHIVE DESTINATION ${NOESIS_DIR})
install(TARGETS fmt_replicant RUNTIME DESTINATION ${NOESIS_DIR})
