cmake_minimum_required (VERSION 3.18)
project(CityBuilderGame)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(benchmark CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(imgui REQUIRED)
find_package(ImGui-SFML REQUIRED)
include(GoogleTest)
include(FetchContent)

FetchContent_Declare(

    tracy
    GIT_REPOSITORY https://github.com/wolfpld/tracy.git
    GIT_TAG v0.9.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(tracy)

file(GLOB_RECURSE SRC_FILES src/*.cpp include/*.h)
add_library(CommonLib STATIC ${SRC_FILES})
target_include_directories(CommonLib PUBLIC include/${SFML_INCLUDE_DIR})
target_link_libraries(CommonLib PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
if(MSVC)
    target_compile_definitions(CommonLib PUBLIC "_USE_MATH_DEFINES")
    target_compile_options(CommonLib PUBLIC /arch:AVX2 /Oi /GR- /EHs-c- /FA /Oy- /GL)
    target_link_options(CommonLib PUBLIC /LTCG)
else()
    target_compile_options(CommonLib PUBLIC "-march=haswell" "-masm=intel" -fno-rtti -fno-exceptions -save-temps
            -fno-omit-frame-pointer -flto -ffast-math)
    target_link_options(CommonLib PUBLIC -flto)
endif()

file(GLOB_RECURSE TEST_FILES test/*.cpp)
add_library(CommonTest ${TEST_FILES})
target_link_libraries(CommonTest PRIVATE CommonLib GTest::gtest GTest::gtest_main sfml-system sfml-network sfml-graphics sfml-window)


file(GLOB BENCH_FILES bench/*.cpp)

foreach(BENCH_FILE ${BENCH_FILES})
    get_filename_component(BENCH_NAME ${BENCH_FILE} NAME_WE)
    add_executable(${BENCH_NAME} ${BENCH_FILE})
    target_link_libraries(${BENCH_NAME} PRIVATE CommonLib benchmark::benchmark benchmark::benchmark_main)

    
    set_target_properties (${BENCH_NAME} PROPERTIES FOLDER Bench)
endforeach(BENCH_FILE ${BENCH_FILES})

file(GLOB TEST_FILES test/*.cpp)

foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} PRIVATE CommonLib GTest::gtest GTest::gtest_main)
    #gtest_add_tests(TARGET ${TEST_NAME})
    
    set_target_properties (${TEST_NAME} PROPERTIES FOLDER Test)
endforeach()


file(GLOB MAIN_FILES main/*.cpp)

foreach(MAIN_FILE ${MAIN_FILES})
    get_filename_component(MAIN_NAME ${MAIN_FILE} NAME_WE)
    add_executable(${MAIN_NAME} ${MAIN_FILE})
    target_link_libraries(${MAIN_NAME} PRIVATE CommonLib)
    #gtest_add_tests(TARGET ${TEST_NAME})
    
    set_target_properties (${MAIN_NAME} PROPERTIES FOLDER Main)
endforeach()

file(GLOB_RECURSE DATA_FILE game/Data/)
source_group("Data/" FILES ${DATA_FILE})
foreach(Data ${DATA_FILE})
get_filename_component(FILE_NAME ${DATA} NAME)
    get_filename_component(PATH_NAME ${DATA} DIRECTORY)
    get_filename_component(EXTENSION ${DATA} EXT)
    file(RELATIVE_PATH PATH_NAME "${PROJECT_SOURCE_DIR}" ${PATH_NAME})
    set(DATA_OUTPUT "${PROJECT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}")
    add_custom_command(
            OUTPUT ${DATA_OUTPUT}
            DEPENDS ${DATA}
            DEPENDS
            COMMAND ${CMAKE_COMMAND} -E copy ${DATA} "${PROJECT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}"
    )
    list(APPEND DATA_BINARY_FILES ${DATA_OUTPUT})
endforeach()

add_custom_target(
    DataTarget
    DEPENDS ${DATA_BINARY_FILES} ${DATA_FILE}
)

file(GLOB_RECURSE FILE_INCLUDE_SOURCE game/src/*.cpp game/main/*.cpp game/include/*.h)
add_executable(MAIN_GAME_CITY_BUILDER ${FILE_INCLUDE_SOURCE})
add_dependencies(MAIN_GAME_CITY_BUILDER DataTarget)
target_include_directories(MAIN_GAME_CITY_BUILDER PRIVATE game/include/ ${SFML_INCLUDE_DIR} ${IMGUI_SOURCES})
target_link_libraries(MAIN_GAME_CITY_BUILDER PRIVATE sfml-system sfml-network sfml-graphics sfml-window imgui::imgui ImGui-SFML::ImGui-SFML TracyClient)
#bench thread
file(GLOB_RECURSE FILE_INCLUDE_SOURCE bench/bench_game_thread.cpp)
add_executable(Bench_Game_thread_real ${FILE_INCLUDE_SOURCE})
add_dependencies(Bench_Game_thread_real DataTarget)
target_include_directories(Bench_Game_thread_real PRIVATE game/include/ ${SFML_INCLUDE_DIR} ${IMGUI_SOURCES})
target_link_libraries(Bench_Game_thread_real PRIVATE  benchmark::benchmark benchmark::benchmark_main sfml-system sfml-network sfml-graphics sfml-window imgui::imgui ImGui-SFML::ImGui-SFML TracyClient)

