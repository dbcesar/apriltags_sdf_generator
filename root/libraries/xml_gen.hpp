#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <sstream>

namespace sdf_generator {
class SdfGenerator {
 private:
  static std::string createFolder(std::string folder_address,
                                  std::string folder_name);

  static void generateModelConfig(std::string sdf_folder_address,
                                  std::string model_name, std::string author =
                                      "",
                                  std::string email = "",
                                  std::string description = "");

  static void generate(std::string folder, std::string model_name,
                       float box_size, std::vector<std::vector<bool> > mat);

 public:
  static void aprilTagGenerator(uint8_t id, float size_in_meters,
                                std::vector<std::vector<bool> > data_mat,
                                std::string folder_address = "./",
                                std::string author = "",
                                std::string email = "");

  static void genericGenerator(std::string model_name, float box_size,
                               std::vector<std::vector<bool> > data_mat,
                               std::string folder_address = "./",
                               std::string author = "", std::string email = "",
                               std::string description = "");
};
}
