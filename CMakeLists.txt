cmake_minimum_required(VERSION 3.15)

project(Tree)
set(CMAKE_CXX_STANDARD 20)


add_executable(${PROJECT_NAME} main.cpp CompareTest.cpp)


#############################################################################


project(TreeGtest)
set(CMAKE_CXX_STANDARD 20)

#add_definitions(-DGTESTS)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}  main.cpp gtests.cpp )
target_compile_definitions(${PROJECT_NAME} PRIVATE GTESTS)



target_link_libraries(${PROJECT_NAME} GTest::GTest GTest::Main  Threads::Threads)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/tests $<TARGET_FILE_DIR:${PROJECT_NAME}>/tests)

