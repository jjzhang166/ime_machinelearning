set (SRCS
     binarygenome.cpp)
     #valuegenome.cpp)

add_library(libgenalg ${SRCS})
target_link_libraries(libgenalg librng)
