cmake_minimum_required(VERSION 3.15)

set(PROJECT FromZ)
project(${PROJECT})
# version
set(serial "1.0")
# basically, we use Release
if (NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
endif()

##########################################
##### Include path / cpp files       #####
##########################################
# Include directories for this project
set(INCLUDE_DIR
    src
)

set(CPP_FILES
    main.cpp
)
##########################################
##### Include path / cpp files (end) #####
##########################################


###########################
##### Libraries       #####
###########################
set(LINK_LIBS
)
###########################
##### Libraries (end) #####
###########################


# Target for this build
add_executable(${PROJECT} ${CPP_FILES})

# Add a include files
target_include_directories(${PROJECT} PUBLIC ${INCLUDE_DIR})
# Add external libraries
target_link_libraries(${PROJECT} PRIVATE ${LINK_LIBS})

if(WIN32)
    set(
        MY_DEBUG_OPTIONS
        -bigobj
    )
elseif (APPLE)
    set(
        MY_DEBUG_OPTIONS
        -g
    )
endif ()
set(
    MY_RELEASE_OPTIONS
    -O2
)

target_compile_options(${PROJECT} PUBLIC "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
target_compile_options(${PROJECT} PUBLIC "$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>")
target_compile_features(${PROJECT} PUBLIC cxx_std_17)
target_compile_definitions(${PROJECT} PUBLIC _USE_MATH_DEFINES)
target_compile_definitions(${PROJECT} PUBLIC _CRT_NONSTDC_NO_DEPRECATE)
target_compile_definitions(${PROJECT} PUBLIC _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(${PROJECT} PUBLIC NOMINMAX)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244")
endif ()
