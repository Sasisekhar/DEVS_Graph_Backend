cmake_minimum_required(VERSION 3.1)
project(cadmium)

add_compile_definitions(SIMULATION)

# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_library(cadmium INTERFACE)
target_include_directories(cadmium INTERFACE include/ cadmium_v2/include/ json/include)

execute_process(COMMAND git submodule update --init)

FILE(GLOB Examples RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/main.cpp)
foreach(exampleSrc ${Examples})
    get_filename_component(exampleName ${exampleSrc} NAME_WE)
    get_filename_component(dirname ${exampleSrc} DIRECTORY)
    file(GLOB directorySrc CONFIGURE_DEPENDS "${dirname}/src/*.cpp")
    add_executable(${exampleName} ${exampleSrc} ${directorySrc})
    target_include_directories(${exampleName} PUBLIC "${dirname}/include/")
    target_link_libraries(${exampleName} cadmium)
endforeach(exampleSrc)

FILE(GLOB Tests RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)
foreach(testSrc ${Tests})
    get_filename_component(testName ${testSrc} NAME_WE)
    add_executable(${testName} ${testSrc} )
    target_include_directories(${testName} PUBLIC "src/include")
    target_link_libraries(${testName} cadmium)
endforeach(testSrc)

add_custom_target(run
    COMMAND ./bin/main $(genPeriod) $(procTime) $(obsTime)
    DEPENDS ./bin/main
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

add_custom_target(run_test
    COMMAND ./bin/test_generator input_data/generator_test_input.txt
    COMMAND ./bin/test_server input_data/server_test_input.txt
    COMMAND ./bin/test_transducer input_data/transducer_test_input_generator.txt input_data transducer_test_input_processed.txt > /dev/null
    COMMAND ./bin/test_balancer input_data/balancer_test_input.txt
    COMMAND echo "Done. Visit the log files."
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
