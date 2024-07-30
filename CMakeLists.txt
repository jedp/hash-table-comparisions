cmake_minimum_required(VERSION 3.28)
project(hash_table_comparison C)

set(CMAKE_C_STANDARD 11)

set(VAL_OBJS
        ${CMAKE_SOURCE_DIR}/val.c
)

set(HASH_TABLE_OBJS
        ${CMAKE_SOURCE_DIR}/hash.c
        ${CMAKE_SOURCE_DIR}/linked-lists.c
)

add_executable(hash_table_comparison
        ${VAL_OBJS}
        ${HASH_TABLE_OBJS}
        main.c
)
