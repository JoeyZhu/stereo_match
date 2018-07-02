/*
 * make_dataset.cpp
 *
 *  Created on: Aug 26, 2017
 *      Author: joey
 */

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include "boost/filesystem.hpp"
#include <string.h>
#include <sys/time.h>


using namespace cv;


static char g_format[12] = "bmp";

bool isLegalPath(const char *imgs_path) {
    if (NULL == imgs_path) {
        return false;
    }
    return (0 == access(imgs_path, F_OK));
}

int fileFilter(const dirent *file) {
    if (DT_REG != file->d_type) { // Only regular file is used
        return 0;
    }
    if (NULL == strstr(file->d_name, g_format)) { // Only .BIN file is used
        return 0;
    }
    return 1;
}

int fileSort(const dirent **file_a, const dirent **file_b) {
    int num_a = atoi(file_a[0]->d_name);
    int num_b = atoi(file_b[0]->d_name);
    if (num_a > num_b) {
        return 0;
    }
    return 1;
}

int main(int argc, char** argv)
{
    std::string sbs_img_file = "";

    std::string img2_filename = "";
    std::string intrinsic_filename = "";
    std::string extrinsic_filename = "";
    std::string disparity_filename = "";
    std::string point_cloud_filename = "";

    char cam0_path[256] = {0};
    char cam1_path[256] = {0};
    char sbs_img_path[256] = {0};

    const char *imgs_path = argv[1];
    dirent **file_list = NULL;

    if (!isLegalPath(imgs_path)) {
        printf("Image folder incorrect. full path should be provided\n");
        return -1;
    }

    int img_num;
    img_num = scandir(imgs_path, &file_list, fileFilter, fileSort);
    if (img_num <= 0) {
        printf("No %s format file in dir %s\n", g_format, imgs_path);
        return -1;
    }

    //  make output dir
    sprintf(cam0_path, "%s%s", imgs_path, "cam0/");
    sprintf(cam1_path, "%s%s", imgs_path, "cam1/");

    if (!boost::filesystem::exists(cam0_path)) {
    	std::string mkdir_cmd = "mkdir ";
        system(mkdir_cmd.append(cam0_path).c_str());
        mkdir_cmd = "mkdir ";
        system(mkdir_cmd.append(cam1_path).c_str());
    }

    long long  file_no = 1000000000;

    std::string image_list_path(imgs_path);
    image_list_path.append("image_list.xml");
    FileStorage fs(image_list_path, FileStorage::WRITE);

    fs << "imagelist" << "[" ;

    while (img_num--) {
        sprintf(sbs_img_path, "%s/%s", imgs_path, file_list[img_num]->d_name);
        std::cout << sbs_img_path << std::endl;
        Mat sidebyside_img = imread(sbs_img_path);
        Mat left_img = sidebyside_img(Rect(0,0, sidebyside_img.cols / 2
        		, sidebyside_img.rows));
        Mat right_img = sidebyside_img(Rect(sidebyside_img.cols / 2
        		, 0, sidebyside_img.cols / 2, sidebyside_img.rows));

        std::string cam0_file(cam0_path);
        std::string cam1_file(cam1_path);
        std::stringstream ss;
        ss << file_no;
        cam0_file.append(ss.str());
        cam1_file.append(ss.str());
        file_no += 10;
        imwrite(cam0_file.append(".bmp"), left_img);
        imwrite(cam1_file.append(".bmp"), right_img);

        fs << cam0_file;
        fs << cam1_file;

        imshow("input", left_img);

        if(waitKey() == 'q'){
        	std::cout << "quit\n";
        	while(img_num){
        		free(file_list[img_num]);
        		img_num--;
        	}
        	free(file_list);
        	file_list = NULL;
        	break;
        }
    }
    fs << "]";
    fs.release();
    destroyAllWindows();

    return 0;


}
