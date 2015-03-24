add_executable(neuralnet_example neuralnet.cpp)
target_link_libraries(neuralnet_example neuralnet)
set_target_properties(neuralnet_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/examples)

add_executable(genalg_example genalg.cpp)
target_link_libraries(genalg_example genalg)
set_target_properties(genalg_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/examples)
