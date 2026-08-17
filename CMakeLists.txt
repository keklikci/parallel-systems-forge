cmake_minimum_required(VERSION 3.20)

project(operating_systems_examples
    VERSION 1.0.0
    DESCRIPTION "Portable operating-systems programming examples"
    LANGUAGES C CXX
)

include(CTest)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

function(enable_project_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
        if(ENABLE_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
        if(ENABLE_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()

add_executable(count-character-a examples/memory-map/count_character_a.c)
add_executable(count-character-a-cpp examples/memory-map/count_character_a.cpp)
add_executable(count-character-a-mmap examples/memory-map/count_character_a_mmap.c)
add_executable(memory-management-api examples/memory-map/memory_mgmt_api.cpp)
add_executable(seat-reservation examples/memory-map/posix.cpp)

find_package(Threads REQUIRED)
target_link_libraries(memory-management-api PRIVATE Threads::Threads)
target_link_libraries(seat-reservation PRIVATE Threads::Threads)

foreach(target
        count-character-a
        count-character-a-cpp
        count-character-a-mmap
        memory-management-api
        seat-reservation)
    enable_project_warnings(${target})
endforeach()

add_custom_target(java-compile
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/java-classes
    COMMAND javac -d ${CMAKE_BINARY_DIR}/java-classes
            ${CMAKE_SOURCE_DIR}/examples/dining-philosophers/src/DiningSimulation.java
            ${CMAKE_SOURCE_DIR}/examples/dining-philosophers/src/Philosopher.java
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/examples/dining-philosophers/schemas
            ${CMAKE_BINARY_DIR}/java-classes/schemas
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Compiling the Dining Philosophers Java example"
    VERBATIM
)

add_custom_target(java-test
    DEPENDS java-compile
    COMMAND javac -cp ${CMAKE_BINARY_DIR}/java-classes
            -d ${CMAKE_BINARY_DIR}/java-classes
            ${CMAKE_SOURCE_DIR}/tests/java/DiningSimulationTest.java
    COMMAND java -Djava.awt.headless=true
            -cp ${CMAKE_BINARY_DIR}/java-classes DiningSimulationTest
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running headless Dining Philosophers tests"
    VERBATIM
)

if(BUILD_TESTING)
    add_test(NAME count_character_c
        COMMAND count-character-a ${CMAKE_SOURCE_DIR}/examples/memory-map/fixture.txt)
    add_test(NAME count_character_cpp
        COMMAND count-character-a-cpp ${CMAKE_SOURCE_DIR}/examples/memory-map/fixture.txt)
    add_test(NAME count_character_mmap
        COMMAND count-character-a-mmap ${CMAKE_SOURCE_DIR}/examples/memory-map/fixture.txt)
    add_test(NAME count_character_custom
        COMMAND count-character-a ${CMAKE_SOURCE_DIR}/examples/memory-map/fixture.txt e)
    add_test(NAME memory_manager_smoke COMMAND memory-management-api 10 7 50)
    add_test(NAME seat_reservation_smoke COMMAND seat-reservation 7 50)
endif()
