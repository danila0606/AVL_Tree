cmake_minimum_required(VERSION 3.16)

project(TreeGtest)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}  ../main.cpp ../gtests.cpp)
add_definitions(-DGTESTS)

target_link_libraries(${PROJECT_NAME} GTest::GTest GTest::Main  Threads::Threads)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/tests $<TARGET_FILE_DIR:${PROJECT_NAME}>/tests)
