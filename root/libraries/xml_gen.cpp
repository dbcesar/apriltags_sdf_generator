#include "xml_gen.hpp"
#include <cstdlib>

namespace sdf_generator {

std::string SdfGenerator::createFolder(std::string folder_address,
                                       std::string folder_name) {
  std::string sdf_folder_address = (folder_address + folder_name + "/");
  std::system(("mkdir -p " + (sdf_folder_address)).c_str());
  return sdf_folder_address;
}

void SdfGenerator::generateModelConfig(std::string sdf_folder_address,
                                       std::string model_name,
                                       std::string author, std::string email,
                                       std::string description) {
  std::string model_config_address = (sdf_folder_address + "model.config");
  //create and open file
  std::ofstream fs(model_config_address.c_str(), std::ios::trunc);

  if (fs.is_open()) {
    fs << "<?xml version=\"1.0\"?>\n" << "\n" << "<model>\n" << "    <name>"
       << model_name << "</name>\n" << "    <version>1.0</version>\n"
       << "    <sdf version=\"1.4\">model.sdf</sdf>\n" << "    \n"
       << "    <author>\n" << "        <name>" << author << "</name>\n"
       << "        <email>" << email << "</email>\n" << "    </author>\n"
       << "    \n" << "    <description>" << description << "</description>\n"
       << "</model>";
    fs.close();
  } else {
    std::cout << "address: " << model_config_address << std::endl;
    std::cout << "Error opening file";
    EXIT_FAILURE;
  }
}

void SdfGenerator::generate(std::string folder, std::string model_name,
                            float box_size,
                            std::vector<std::vector<bool> > mat) {

  std::string model_sdf_address = (folder + "model.sdf");

  //create and open file
  std::ofstream fs(model_sdf_address.c_str(), std::ios::trunc);

  if (fs.is_open()) {
    //write_header
    fs << "<sdf version=\"1.4\">\n" << "  <model name='" << model_name << "'>\n"
       << "  <static>true</static>\n" << "  <pose>0 0 0 0 0 0</pose>\n"
       << "     <link name='" << model_name << "_body'>\n";

    float total_cols_size = box_size * mat.size();
    //rows
    for (int i = 0; i < mat.size(); ++i) {
      //colums
      for (int j = 0; j < mat[i].size(); ++j) {
        //set boxes
        if (mat[i][j]) {
          fs << "      <visual name='block" << i * mat.size() + j << "'>\n"
             << "        <pose>" << box_size * i << " " << box_size * j
             << " 0.001 0 0 0</pose>\n" << "          <geometry>\n"
             << "            <box>\n" << "            <size>" << box_size << " "
             << box_size << " " << "0.003</size>\n" << "            </box>\n"
             << "          </geometry>\n" << "        <material>\n"
             << "          <ambient>1 1 1 1</ambient>\n"
             << "          <diffuse>1 1 1 1</diffuse>\n"
             << "          <specular>1 1 1 1</specular>\n"
             << "        </material>\n" << "      </visual>\n";
        } else {
          fs << "      <visual name='block" << i * mat.size() + j << "'>\n"
             << "        <pose>" << box_size * i << " " << box_size * j
             << " 0.001 0 0 0</pose>\n" << "          <geometry>\n"
             << "            <box>\n" << "            <size>" << box_size << " "
             << box_size << " " << "0.003</size>\n" << "            </box>\n"
             << "          </geometry>\n" << "        <material>\n"
             << "          <ambient>0 0 0 1</ambient>\n"
             << "          <diffuse>0 0 0 1</diffuse>\n"
             << "          <specular>0 0 0 1</specular>\n"
             << "        </material>\n" << "      </visual>\n";
        }
      }
    }
    //insert a thin white layer at the bottom

    fs << "      <visual name='thin_layer'>\n" << "        <pose>"
       << (total_cols_size - box_size) / 2 << " "
       << (total_cols_size - box_size) / 2 << " 0 0 0 0</pose>\n"
       << "          <geometry>\n" << "            <box>\n"
       << "            <size>" << total_cols_size << " " << total_cols_size
       << " " << "0.001</size>\n" << "            </box>\n"
       << "          </geometry>\n" << "        <material>\n"
       << "          <ambient>0 0 0 1</ambient>\n"
       << "          <diffuse>0 0 0 1</diffuse>\n"
       << "          <specular>0 0 0 1</specular>\n" << "        </material>\n"
       << "      </visual>\n" << "    </link>\n" << "  </model>\n" << "</sdf>";
    fs.close();
  } else {
    std::cout << "address: " << model_sdf_address << std::endl;
    std::cout << "Error opening file";
    EXIT_FAILURE;
  }

}

void SdfGenerator::aprilTagGenerator(uint8_t id, float size_in_meters,
                                     std::vector<std::vector<bool> > data_mat,
                                     std::string folder_address,
                                     std::string author, std::string email) {
  std::ostringstream ss;
  ss << (int) id;
  std::string model_name = "apriltag_" + ss.str();
  std::string description =
      "Fiducial marker developed by the April Laboratories (https://april.eecs.umich.edu/wiki/index.php/AprilTags)";

  if (author.size() < 1) {
    author = "Diego Cesar";
    email = "diego.cesar@fieb.org.br";
  }

  std::string sdf_folder_address = createFolder(folder_address, model_name);
  generateModelConfig(sdf_folder_address, model_name, author, email,
                      description);
  float box_size = size_in_meters / 10;

  generate(sdf_folder_address, model_name, box_size, data_mat);
}

void SdfGenerator::genericGenerator(std::string model_name, float box_size,
                                    std::vector<std::vector<bool> > data_mat,
                                    std::string folder_address,
                                    std::string author, std::string email,
                                    std::string description) {

  std::string sdf_folder_address = createFolder(folder_address, model_name);
  generateModelConfig(sdf_folder_address, model_name, author, email,
                      description);

  generate(sdf_folder_address, model_name, box_size, data_mat);
}

}

