find_package(OpenCV REQUIRED)

add_executable(stereo_match stereo_match.cpp)

target_link_libraries(stereo_match ${OpenCV_LIBS})

