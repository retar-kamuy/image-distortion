cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

project(image-distortion)

add_executable(image-distortion
    src/main.cpp
    src/image.cpp
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

find_package(OpenCV REQUIRED)
find_package(PythonLibs REQUIRED)

if(OpenCV_FOUND)
    target_include_directories(image-distortion
        PRIVATE
            ${OpenCV_INCLUDE_DIRS}
    )

    target_link_libraries(image-distortion
        ${OpenCV_LIBS}
    )

endif(OpenCV_FOUND)

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
