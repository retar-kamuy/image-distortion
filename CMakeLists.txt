cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CPP_COMPILER clang++)

project(image-distortion)

add_executable(image-distortion
    src/main.cpp
)

target_compile_features(image-distortion
    PRIVATE cxx_std_17
)

# target_compile_definitions(image-distortion
# )

target_include_directories(image-distortion
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

find_package(PythonLibs REQUIRED)

if(PythonLibs_FOUND)
    target_include_directories(image-distortion
    PRIVATE
        /usr/include/python3.6m
        /usr/local/lib64/python3.6/site-packages/numpy/core/include
    )

    target_link_libraries(image-distortion
        python3.6m
    )
endif(PythonLibs_FOUND)
