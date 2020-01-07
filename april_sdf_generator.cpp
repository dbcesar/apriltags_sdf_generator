#include <opencv2/opencv.hpp>
#include <cstdlib>
#include "xml_gen.hpp"

void Display(std::string name, cv::Mat img)
{
    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
    cv::imshow(name, img);
}

int main(int argc, char** argv)
{   
    if (argc < 3)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "apriltag_id, size_in_meters, image_address, " <<
                "(opt)folder_address, (opt)author, (opt)email" << std::endl;
        return 0;
    }

        
    int id = std::atoi(argv[1]);
    float size_in_meters = std::atof(argv[2]);
    std::string address(argv[3]);

    std::string folder_address;
    (argv[4] == NULL) ? folder_address = "./" : folder_address = argv[4];
    std::string author;
    (argv[5] == NULL) ? author = "" : author = argv[5];
    std::string email;
    (argv[6] == NULL) ? email = "" : email  = argv[6];

    ///// STEP 1 /////
    //address of the image
    std::cout << "id: " << id << std::endl;
    std::cout << "size_in_meters: " << size_in_meters << std::endl;
    std::cout << "address: " << address << std::endl;
    std::cout << "folder_address: " << folder_address << std::endl;
    std::cout << "author: " << author << std::endl;
    std::cout << "email: " << email << std::endl;


    cv::Mat temp = cv::imread(address, 0);
    std::cout << "read the image sucessfully" << std::endl;

    //resize image by a factor
    cv::Mat img_bin;
    cv::threshold(temp, img_bin, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    //sum of rows
    int first_row = 0;
    int first_col = 0;
    int last_row = 0;
    int last_col = 0;
    
    //count all black elements per column
    for (int i = 0; i < img_bin.cols; i++)
    {
        int sum = 0;
        for (int j = 0; j < img_bin.rows; j++)
        {
            int value = img_bin.at<uint8_t>(j,i)/255;
            sum += value;
        }
        if ((float)-(sum-img_bin.rows)/img_bin.rows > 0.6)
        {
            first_col = i;
            break;
        }
    }

    for (int i = 0; i < img_bin.rows; i++)
    {
        int sum = 0;
        for (int j = 0; j < img_bin.cols; j++)
        {
            int value = img_bin.at<uint8_t>(i,j)/255;
            sum += value;
        }
        if ((float)-(sum-img_bin.cols)/img_bin.cols > 0.8)
        {
            first_row = i;
            break;
        }
    }

    for (int i = img_bin.cols-1; i > 0; i--)
    {
        int sum = 0;
        for (int j = img_bin.rows-1; j > 0; j--)
        {
            int value = img_bin.at<uint8_t>(j,i)/255;
            sum += value;
        }
        if ((float)-(sum-img_bin.rows)/img_bin.rows > 0.6)
        {
            last_col = i;
            break;
        }
    }

    for (int i = img_bin.rows-1; i > 0; i--)
    {
        int sum = 0;
        for (int j = img_bin.cols-1; j > 0; j--)
        {
            int value = img_bin.at<uint8_t>(i,j)/255;
            sum += value;
        }
        if ((float)-(sum-img_bin.cols)/img_bin.cols > 0.8)
        {
            last_row = i;
            break;
        }
    }

    cv::Point corner1(first_col, first_row);
    cv::Point corner2(last_col, last_row);

    //region of interest
    cv::Mat roi(img_bin, cv::Rect(corner1, corner2));
    Display("roi", roi);

    int block_w = roi.cols/8;
    int block_h = roi.rows/8;

    //look for black block and overide in in bin_matrix
    cv::Mat test = cv::Mat::ones(10,10, CV_8U) * 255;
    for (int i = 0; i <= roi.rows; i += (block_h+1))
    {
        for (int j = 0; j <= roi.cols; j += (block_w+1))
        {
            cv::Mat roi_temp(roi, cv::Rect(j,i, block_h, block_w));
            cv::Scalar tempValue = cv::mean(roi_temp);

            if (block_h == 0 || block_w == 0)
                tempValue[0] = roi.at<uchar>(i,j);

            if( tempValue[0] < 256/2) 
            {
                test.at<uchar>(i+1, j+1) = 0;
            }

        }
    }

    std::vector<int> temp_vector(10, 255);
    std::vector<std::vector<int> > data_mat(10, temp_vector);

    cv::transpose(test, test);
    cv::flip(test, test, 1);
        
    for(int i=0; i < 10; ++i)
    {
        for(int j=0; j < 10; ++j)
        {
            data_mat[i][j] = (int)test.at<uchar>(i,j);
        }
    }

    sdf_generator::SdfGenerator gen(id, size_in_meters, data_mat, folder_address, author, email);
    gen.generate();
    
    return 0;
}

