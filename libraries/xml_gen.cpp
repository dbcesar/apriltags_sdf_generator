#include "xml_gen.hpp"
#include <cstdlib>


namespace sdf_generator
{
    SdfGenerator::SdfGenerator()
    {
    }

    SdfGenerator::SdfGenerator(uint8_t id, float size_in_meters, std::vector<std::vector<int> > data_mat, std::string folder_address, std::string author, std::string email)
    {
        this->id = id;
        this->size_in_meters = size_in_meters;
        this->folder_address = folder_address;
        this->author = author;
        this->email = email;
        this->mat = data_mat;


    }

    void SdfGenerator::setAuthor(std::string author)
    {
        this->author = author;
    }

    void SdfGenerator::setEmail(std::string email)
    {
        this->email = email;
    }

    void SdfGenerator::setId(uint8_t id)
    {
        this->id = id;
    }

    void SdfGenerator::setSize(float size)
    {
        this->size_in_meters = size;
    }

    void SdfGenerator::setAddress(std::string address)
    {
        folder_address = address;
    }

    void SdfGenerator::createFolder()
    {
        std::ostringstream ss;
        ss << (int)this->id;

        this->sdf_folder_address = (folder_address+"apriltag_"+ss.str()+"/");

        std::system(("mkdir -p "+(this->sdf_folder_address)).c_str()); 
    }

    void SdfGenerator::generateModelConfig()
    {
        std::string model_config_address = (this->sdf_folder_address+"model.config");
        //create and open file
        std::ofstream fs(model_config_address.c_str(), std::ios::trunc);
        //fs.close();
        //fs.open(model_config_address.c_str());

        if (fs.is_open())
        {
           fs << 
           "<?xml version=\"1.0\"?>\n" <<
           "\n" <<
           "<model>\n" <<
           "    <name>apriltags_" << (int)id << "</name>\n" <<
           "    <version>1.0</version>\n" <<
           "    <sdf version=\"1.4\">model.sdf</sdf>\n" <<
           "    \n" <<
           "    <author>\n" <<
           "        <name>Diego Cesar</name>\n" <<
           "        <email>diego.cesar@fieb.org.br</email>\n" <<
           "    </author>\n" <<
           "    \n" <<
           "    <description>\n" <<
           "        Fiducial marker developed by the April Laboratories (https://april.eecs.umich.edu/wiki/index.php/AprilTags)\n" <<
           "    </description>\n" <<
           "</model>";
        }
        else
        {
            std::cout << "address: " << model_config_address << std::endl;
            std::cout << "Error opening file";
        }

        fs.close();
    }

    void SdfGenerator::generate()
    {
        this->createFolder();
        this->generateModelConfig();

        std::string model_sdf_address = (this->sdf_folder_address+"model.sdf");
        //create and open file
        std::ofstream fs(model_sdf_address.c_str(), std::ios::trunc);

        if (fs.is_open())
        {
            //write_header
            fs << 
               "<sdf version=\"1.4\">\n" <<
               "  <model name='apriltags_" << (int)id << "'>\n" <<
               "  <static>true</static>\n" <<
               "  <pose>0 0 0 0 0 0</pose>\n" <<
               "     <link name='apriltag_body'>\n";

            float box_size = size_in_meters/10;
            //rows
            for (int i=0; i<mat.size(); ++i)
            {
                //colums
                for (int j=0; j<mat[i].size(); ++j)
                {   
                    //set boxes
                    if(mat[i][j] > 0)
                    {
                        fs <<
                            "      <visual name='block" << i*mat.size()+j << "'>\n" <<
                            "        <pose>" << box_size*i << " " << box_size*j << " 0.001 0 0 0</pose>\n" <<
                            "          <geometry>\n" <<
                            "            <box>\n" <<
                            "            <size>" << box_size << " " << box_size << " " << "0.003</size>\n" <<
                            "            </box>\n" <<
                            "          </geometry>\n" <<
                            "        <material>\n" <<
                            "          <ambient>1 1 1 1</ambient>\n" <<
                            "          <diffuse>1 1 1 1</diffuse>\n" <<
                            "          <specular>1 1 1 1</specular>\n" <<
                            "        </material>\n" <<
                            "      </visual>\n";
                    }
                    else
                    {
                        fs <<
                            "      <visual name='block" << i*mat.size()+j << "'>\n" <<
                            "        <pose>" << box_size*i << " " << box_size*j << " 0.001 0 0 0</pose>\n" <<
                            "          <geometry>\n" <<
                            "            <box>\n" <<
                            "            <size>" << box_size << " " << box_size << " " << "0.003</size>\n" <<
                            "            </box>\n" <<
                            "          </geometry>\n" <<
                            "        <material>\n" <<
                            "          <ambient>0 0 0 1</ambient>\n" <<
                            "          <diffuse>0 0 0 1</diffuse>\n" <<
                            "          <specular>0 0 0 1</specular>\n" <<
                            "        </material>\n" <<
                            "      </visual>\n";
                    }
                }
            }
            //insert a thin white layer at the bottom

            fs <<
                "      <visual name='thin_layer'>\n" <<
                "        <pose>" << (size_in_meters-box_size)/2 << " " << (size_in_meters-box_size)/2 << " 0 0 0 0</pose>\n" <<
                "          <geometry>\n" <<
                "            <box>\n" <<
                "            <size>" << size_in_meters << " " << size_in_meters << " " << "0.001</size>\n" <<
                "            </box>\n" <<
                "          </geometry>\n" <<
                "        <material>\n" <<
                "          <ambient>0 0 0 1</ambient>\n" <<
                "          <diffuse>0 0 0 1</diffuse>\n" <<
                "          <specular>0 0 0 1</specular>\n" <<
                "        </material>\n" <<
                "      </visual>\n" << 
                "    </link>\n" << 
                "  </model>\n" << 
                "</sdf>"; 
        }
        else
        {
            std::cout << "address: " << model_sdf_address << std::endl;
            std::cout << "Error opening file";
        }

        fs.close();
    }

}

