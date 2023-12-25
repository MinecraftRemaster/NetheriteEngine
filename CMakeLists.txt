cmake_minimum_required(VERSION 3.20)

#
project(NetheriteEngine)

# 
#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/cpp/include)
#target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# Add sub directories
add_subdirectory(src/cpp/library/JNI/Netherite)
add_subdirectory(src/cpp/library/Native/Netherite)
add_subdirectory(src/cpp/test/Cube)
