add_executable(money_saver main.cpp game.cpp)
target_link_libraries(money_saver dl)
set_target_properties(money_saver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/challenges/money_saver)
file(COPY terrain.map DESTINATION ${PROJECT_BINARY_DIR}/bin/challenges/money_saver)
