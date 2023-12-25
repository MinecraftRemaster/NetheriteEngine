cmake_minimum_required(VERSION 3.20)

#
project(NetheriteTexGen)

# Create the executable
add_executable(${PROJECT_NAME} ./Main.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 23)

#
target_sources(${PROJECT_NAME} PUBLIC ${NthSrc} ./Main.cpp)

# 
target_include_directories(${PROJECT_NAME} PUBLIC 
    "../../include"
    "../../include/*"
)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC Netherite)
