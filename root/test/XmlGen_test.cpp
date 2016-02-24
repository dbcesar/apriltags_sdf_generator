/*
 * XmlGen_test.cpp
 *
 *  Created on: Feb 24, 2016
 *      Author: tiagotrocoli
 */

#define BOOST_TEST_DYN_LINK
//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "XmlGen_test"
//VERY IMPORTANT - include this last

#include <vector>
#include <cmath>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <xml_gen.hpp>

#define RESOURCE_PATH "../../../root/test/resource/"

BOOST_AUTO_TEST_CASE(generateAprilTag_test) {

  ///// STEP 1 /////
  //address of the image

  for (int id = 1; id <= 2; ++id) {

    std::ostringstream s_id;
    s_id << (int) id;

    float size_in_meters = 1.0;
    std::string folder_address = "";
    std::string author = "zezinho_" + s_id.str();
    std::string email = author + "@pif.io";
    std::string image_path = std::string(RESOURCE_PATH) + "img" + s_id.str()
        + ".png";

    std::cout << "id: " << id << std::endl;
    std::cout << "size_in_meters: " << size_in_meters << std::endl;
    std::cout << "folder_address: " << folder_address << std::endl;
    std::cout << "author: " << author << std::endl;
    std::cout << "email: " << email << std::endl;
    std::cout << "image_path: " << image_path << std::endl;

    cv::Mat temp = cv::imread(image_path, 0);
//    cv::imshow("temp_image" + s_id.str(), temp);
//    cv::waitKey();

    //resize image by a factor
    cv::Mat img_bin;
    //float factor = 1;
    //cv::resize(temp, img_bin, cv::Size(temp.cols*factor, temp.rows*factor));
    cv::threshold(temp, img_bin, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    //sum of rows
    int first_row = 0;
    int first_col = 0;
    int last_row = 0;
    int last_col = 0;

    //count all black elements per column
    for (int i = 0; i < img_bin.cols; i++) {
      int sum = 0;
      for (int j = 0; j < img_bin.rows; j++) {
        int value = img_bin.at<uint8_t>(j, i) / 255;
        sum += value;
      }
      if ((float) -(sum - img_bin.rows) / img_bin.rows > 0.6) {
        first_col = i;
        break;
      }
    }

    for (int i = 0; i < img_bin.rows; i++) {
      int sum = 0;
      for (int j = 0; j < img_bin.cols; j++) {
        int value = img_bin.at<uint8_t>(i, j) / 255;
        sum += value;
      }
      if ((float) -(sum - img_bin.cols) / img_bin.cols > 0.8) {
        first_row = i;
        break;
      }
    }

    for (int i = img_bin.cols - 1; i > 0; i--) {
      int sum = 0;
      for (int j = img_bin.rows - 1; j > 0; j--) {
        int value = img_bin.at<uint8_t>(j, i) / 255;
        sum += value;
      }
      if ((float) -(sum - img_bin.rows) / img_bin.rows > 0.6) {
        last_col = i;
        break;
      }
    }

    for (int i = img_bin.rows - 1; i > 0; i--) {
      int sum = 0;
      for (int j = img_bin.cols - 1; j > 0; j--) {
        int value = img_bin.at<uint8_t>(i, j) / 255;
        sum += value;
      }
      if ((float) -(sum - img_bin.cols) / img_bin.cols > 0.8) {
        last_row = i;
        break;
      }
    }

    cv::Point corner1(first_col, first_row);
    cv::Point corner2(last_col, last_row);
    //cv::circle(img_bin, corner1, 3, cv::Scalar(255,0,0), -1);
    //cv::circle(img_bin, corner2, 3, cv::Scalar(255,0,0), -1);

    //region of interest
    cv::Mat roi(img_bin, cv::Rect(corner1, corner2));
//    cv::imshow("roi", roi);

    int block_w = roi.cols / 8;
    int block_h = roi.rows / 8;

    //TODO erase this
    //initialize a 8x8 matrix with all elements black
    //cv::Mat bin_matrix(8,8,CV_8UC1, cv::Scalar(0));

    //initialize matrix with white elements
    std::vector<int> temp_vector(10, 255);
    std::vector<std::vector<int> > data_mat(10, temp_vector);
    //for (int i = 0; i < temp_mat.size(); ++i)
    //{
    //temp_mat[i].resize(10);
    //}

    //look for black block and overide in in bin_matrix
    for (int i = 1; i < roi.rows; i += block_h) {
      for (int j = 1; j < roi.cols; j += block_w) {
        cv::Mat roi_temp(roi, cv::Rect(j, i, block_h - 1, block_w - 1));
        cv::Scalar tempValue = cv::mean(roi_temp);
        if (tempValue[0] < 256 / 2) {
          data_mat[i / block_h + 1][j / block_w + 1] = 0;
        }
      }
    }

    std::vector<std::vector<bool> > data_bool;
    for (int k = 0; k < data_mat.size(); ++k) {
      std::vector<bool> temp_vector(data_mat[k].begin(), data_mat[k].end());
      data_bool.push_back(temp_vector);
    }

    sdf_generator::SdfGenerator::aprilTagGenerator(id, size_in_meters,
                                                   data_bool, folder_address,
                                                   author, email);

    std::string model_config = "model.config";
    std::string model_sdf = "model.sdf";
    std::string ground_truth_path = std::string(RESOURCE_PATH) + "apriltag_"
        + s_id.str() + "/";
    std::string result_path = "apriltag_" + s_id.str() + "/";
    std::string line_gt, line_final;

    // check mode.config files
    std::ifstream ground_truth_config(
        (ground_truth_path + model_config).c_str());
    std::ifstream result_config((result_path + model_config).c_str());

    std::cout << ground_truth_path << model_config << std::endl;
    std::cout << result_path << model_config << std::endl;

    while (std::getline(ground_truth_config, line_gt)) {
      std::getline(result_config, line_final);
      BOOST_CHECK_EQUAL(line_gt, line_final);
    }
    ground_truth_config.close();
    result_config.close();

    // check mode.sdf files
    std::ifstream ground_truth_sdf((ground_truth_path + model_sdf).c_str());
    std::ifstream result_sdf((result_path + model_sdf).c_str());

    std::cout << ground_truth_path << model_sdf << std::endl;
    std::cout << result_path << model_sdf << std::endl;

    while (std::getline(ground_truth_sdf, line_gt)) {
      std::getline(result_sdf, line_final);
      BOOST_CHECK_EQUAL(line_gt, line_final);
    }
  }
}

BOOST_AUTO_TEST_CASE(generateGeneric_test) {

  unsigned int rows = 8, cols = 10;

  std::vector<std::vector<bool> > chessboard_pattern;
  for (unsigned int i = 0; i < rows; ++i) {
    std::vector<bool> temp_vector;
    bool color;
    for (unsigned int j = 0; j < cols; ++j) {
      (i + j) % 2 ? color = true : color = false;
      temp_vector.push_back(color);
    }
    chessboard_pattern.push_back(temp_vector);
  }

  std::cout << "\nCHESSBOARD PATTERN" << std::endl;
  for (unsigned int i = 0; i < rows; ++i) {
    std::cout << "row " << i << " : " << chessboard_pattern[i][0];
    for (unsigned int j = 1; j < cols; ++j) {
      std::cout << "," << chessboard_pattern[i][j];
    }
    std::cout << "\n";
  }

  sdf_generator::SdfGenerator::genericGenerator("chessboard", 0.35,
                                                chessboard_pattern);
}

