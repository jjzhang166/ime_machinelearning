set (SRCS
     binarygenome.cpp)

add_library(genalg SHARED ${SRCS})
target_link_libraries(genalg rng)
