cmake_minimum_required(VERSION 3.12)

project(
        labwork9
        VERSION 1.0
)

set(CMAKE_CXX_STANDARD 23)


add_subdirectory(lib)
add_subdirectory(bin)


enable_testing()
add_subdirectory(tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -gdwarf-3")
