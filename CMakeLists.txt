cmake_minimum_required (VERSION 2.6)

project (AllocatorShowcase)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")


file(GLOB AllocatorShowcase_SRC
    "showcase.cpp"
)
add_executable(AllocatorShowcase ${AllocatorShowcase_SRC})
