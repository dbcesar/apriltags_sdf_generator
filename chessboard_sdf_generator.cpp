#include <cstdlib>
#include <vector>
#include "xml_gen.hpp"

bool Sum_IsPair(int i, int j)
{
    int rest = (i+j) % 2;

    if (rest == 0) {
        return true;
    }
    else
    {
        return false;
    }
}

int main(int argc, char** argv)
{
  
    // Parameters parsings - To do - Make it better

    if (argc < 4)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "apriltag_id, number_of_lines, number_of_columns, " <<
                "size_in_meters, (opt)folder_address, (opt)author, (opt)email" << std::endl;
        return 0;
    }

    int id = std::atoi(argv[1]),
        line = std::atoi(argv[2]),
        column = std::atoi(argv[3]);

    float size_in_meters = std::atof(argv[4]);

    std::string folder_address;
    (argv[5] == NULL) ? folder_address = "./" : folder_address = argv[5];
    std::string author;
    (argv[6] == NULL) ? author = "" : author = argv[6];
    std::string email;
    (argv[7] == NULL) ? email = "" : email  = argv[7];


    // Creation of pattern matrix
    //   - 255 indicates black square
    //   - 0 indicates white square


    std::vector<int> temp_vector(column, 255);
    std::vector<std::vector<int> > data_mat(line,temp_vector);

    // Alternates between 255 and 0 in data_mat
    for(int i = 0; i < data_mat.size(); i++)
    {
        for(int j = 0; j < data_mat[i].size(); j++)
        {
            if ( !Sum_IsPair(i,j) ) {
                data_mat[i][j] = 0;
            }
        }
    }
    
    // Creation of the SDF
    sdf_generator::SdfGenerator gen(id, size_in_meters, data_mat, folder_address, author, email);
    gen.generate();
}