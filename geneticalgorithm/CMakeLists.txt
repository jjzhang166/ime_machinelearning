set (SRCS
     binarygenome.cpp)

add_library(genalg ${SRCS})
target_link_libraries(genalg rng)
