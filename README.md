# stereo_match
read sidebyside stereo image and make disparity image with sgbm

# make_dataset
read sidebyside image and generate stereo image and xml file for ./cpp-example-stereo_calib to calibrate
found it in opencv_source_dir/build/bin/, source code in opencv_source_dir/samples/cpp/
./stereo_calib -w=<board_width default=9> -h=<board_height default=6> -s=<square_size default=1.0> <image list XML/YML file default=../data/stereo_calib.xml>