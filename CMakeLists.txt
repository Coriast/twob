cmake_minimum_required(VERSION 3.16)
project(twob)

# can choose STATIC | SHARED | MODULE
add_library(twob STATIC)

target_sources(twob 
    PRIVATE
        src/twob_compress.cpp                       
        src/twob_redblacktree.cpp
)
 
target_include_directories(twob 
    PUBLIC
        include

    PRIVATE
        src
)
