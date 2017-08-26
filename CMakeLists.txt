cmake_minimum_required(VERSION 2.8)

SET("OpenCV_DIR" "/usr/local/share/OpenCV")


find_package(OpenCV REQUIRED PATHS "/usr/local/share")
find_package(Boost COMPONENTS system filesystem REQUIRED)

add_executable(stereo_match stereo_match.cpp)
add_executable(make_dataset make_dataset.cpp)

target_link_libraries(stereo_match ${OpenCV_LIBS})
target_link_libraries(make_dataset ${OpenCV_LIBS} ${Boost_LIBRARIES})

