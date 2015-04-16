macro (add_agent agent)
  add_library(${agent} SHARED ${ARGN})
  set_target_properties(${agent} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/challenges/money_saver)
endmacro()

add_agent(saverTemplate template.cpp)
add_agent(thiefTemplate template.cpp)

add_agent(fnnSaver fnnSaver.cpp)
target_link_libraries(fnnSaver neuralnet)

add_executable(money_saver_fnn_trainer fnnSaverTrainer.cpp ../game.cpp)
target_link_libraries(money_saver_fnn_trainer dl neuralnet fnnSaver)
set_target_properties(money_saver_fnn_trainer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/challenges/money_saver)

add_executable(money_saver_fnn_gfx fnnSaverTrainerGfx.cpp ../game.cpp)
target_link_libraries(money_saver_fnn_gfx dl neuralnet fnnSaver gfx)
set_target_properties(money_saver_fnn_gfx PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/challenges/money_saver)
