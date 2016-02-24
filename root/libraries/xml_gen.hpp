#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <sstream>

namespace sdf_generator
{
    class SdfGenerator
    {
        private:
            std::string author;
            std::string email;
            uint8_t id;
            float size_in_meters;
            std::vector<std::vector<int> > mat;
            int rows, cols;
            std::string folder_address; 
            std::string sdf_folder_address; 
            void createFolder();
        public:
            SdfGenerator();
            SdfGenerator(uint8_t id, float size_in_meters, std::vector<std::vector<int> > data_mat, std::string folder_address="./", std::string author="", std::string email="");
            //TOOD setRows; setCols, setMat;
            void setAuthor(std::string author);
            void setEmail(std::string email);
            void setId(uint8_t id);
            void setSize(float size);
            void setAddress(std::string address);
            void generate();
            void generateModelConfig();
    };
}
