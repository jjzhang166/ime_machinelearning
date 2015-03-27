add_executable(labirinth ga.cpp)
target_link_libraries(labirinth genalg)
set_target_properties(labirinth PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/challenges)
