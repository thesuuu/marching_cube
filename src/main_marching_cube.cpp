#include <iostream>
#include <string>
#include "marching_cube.h"
#include "generator.h"
#include "utilities.h"

//using namespace std;

int main(int argc, char* argv[])
{
	std::string file_in = argv[1];
	std::string file_out = argv[2];
	float isovalue = std::stof(argv[3]);

	MarchingCube mycube = MarchingCube();
	int grid_max = 400;
	Generator generator = Generator(grid_max);
	std::vector<std::vector<std::vector<float>>> field;

	if (file_in == "random") {
		std::cout << "Generating random grid..." << std::endl;
		field = generator.generate_random_grid();
	}
	else if (file_in == "sphere") {
		std::cout << "Generating sphere grid..." << std::endl;
		field = generator.generate_sphere(100, 100, 100, 80);
	}
	else if (file_in == "nested") {
		std::cout << "Generating nested sphere grid..." << std::endl;
		field = generator.generate_nested_spheres();
	}
	else {
		// 暂时只支持mri.raw文件的输入
		std::cout << "Reading grid from " << file_in << "..." << std::endl;
		generator.read_grid_from_file(file_in.c_str(), field);
	}
	std::cout << "Done." << std::endl;

	std::cout << "Triangulating using marching cubes... " << std::endl;
	auto mesh = mycube.triangulate_field(field, isovalue);
	std::cout << "Done." << std::endl;

	std::cout << "Writing mesh to file... " << std::endl;
	write_to_ply(mesh, file_out.c_str());
	std::cout << "Done." << std::endl;
}