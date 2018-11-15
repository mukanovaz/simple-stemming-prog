cmake_minimum_required(VERSION 3.12)
project(sp_pc_2018 C)

set(CMAKE_C_STANDARD 99)

add_executable(sp_pc_2018 main.c library.h trie.c trie.h lcs.c lcs.h LinkedList.c LinkedList.h)