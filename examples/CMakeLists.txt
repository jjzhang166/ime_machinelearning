add_executable(neuralnet neuralnet.cpp)
target_link_libraries(neuralnet libneuralnet)
set_target_properties(neuralnet PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/examples)
