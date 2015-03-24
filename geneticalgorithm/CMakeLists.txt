set (SRCS
     binarygenome.cpp
     genalg.cpp)

add_library(genalg ${SRCS})
target_link_libraries(genalg rng)
