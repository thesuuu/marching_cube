#include "generator.h"
#include "datatype.h"
#include <fstream>
#include <iostream>
#include <vector>

float Generator::get_random_number()
{
	float randomNumber = (float)rand() / RAND_MAX;
	randomNumber -= 0.5;
	randomNumber *= 2.0;

	return randomNumber;
}

Generator::Generator(int max)
{
	GRID_MAX = max;
}

std::vector<std::vector<std::vector<float>>> Generator::generate_random_grid()
{
	std::vector<std::vector<std::vector<float>>> scalarFunction(GRID_MAX, std::vector<std::vector<float>>(GRID_MAX, std::vector<float>(GRID_MAX)));

	for (int i = 0; i < GRID_MAX; i++)
		for (int j = 0; j < GRID_MAX; j++)
			for (int k = 0; k < GRID_MAX; k++)
				scalarFunction[i][j][k] = get_random_number();

	return scalarFunction;
}


std::vector<std::vector<std::vector<float>>> Generator::generate_sphere(
	float centerX, float centerY, float centerZ, float radius)
{
	std::vector<std::vector<std::vector<float>>> scalarFunction(GRID_MAX, std::vector<std::vector<float>>(GRID_MAX, std::vector<float>(GRID_MAX)));

	for (int k = 0; k < GRID_MAX; k++)
		for (int j = 0; j < GRID_MAX; j++)
			for (int i = 0; i < GRID_MAX; i++)
			{
				float val = (centerX - i) * (centerX - i) + (centerY - j) * (centerY - j) + (centerZ - k) * (centerZ - k);

				if (val <= radius * radius)
					scalarFunction[i][j][k] = -1;
				else
					scalarFunction[i][j][k] = 1;
			}

	return scalarFunction;
}

std::vector<std::vector<std::vector<float>>> Generator::generate_nested_spheres()
{
	std::vector<std::vector<std::vector<float>>> scalarFunction(GRID_MAX, std::vector<std::vector<float>>(GRID_MAX, std::vector<float>(GRID_MAX)));

	int r1 = 50, cx1 = 64, cy1 = 64, cz1 = 64;
	int r2 = 5, cx2 = 64, cy2 = 64, cz2 = 64;

	for (int i = 0; i < GRID_MAX; i++)
	{
		for (int j = 0; j < GRID_MAX; j++)
		{
			for (int k = 0; k < GRID_MAX; k++)
			{
				int val1 = (cx1 - i) * (cx1 - i) + (cy1 - j) * (cy1 - j) + (cz1 - k) * (cz1 - k);
				int val2 = (cx2 - i) * (cx2 - i) + (cy2 - j) * (cy2 - j) + (cz2 - k) * (cz2 - k);
				if (val1 <= r1 * r1 && val2 > r2 * r2)
				{
					scalarFunction[i][j][k] = -1;
				}
				else
					scalarFunction[i][j][k] = 1;
			}
		}
	}
	return scalarFunction;
}

bool Generator::read_grid_from_file(const char* path, std::vector<std::vector<std::vector<float>>>& scalarFunction) {
	
	short int themax = 0;
	short int themin = std::numeric_limits<short int>::max();
	std::ifstream file;
	std::cerr << "Reading data ...\n";
	file.open(path, std::ios::binary);
	if (!file) {
		std::cerr << "File open failed\n";
		return 0;
	}

	// NX, NY, NZ 是已定义的网格大小
	int NX = 200; int NY = 160; int NZ = 160;

	scalarFunction.resize(NX, std::vector<std::vector<float>>(NY, std::vector<float>(NZ)));


	//std::ofstream outFile("output.txt");
	//if (!outFile) {
	//	std::cerr << "Output file open failed\n";
	//	return false;
	//}


	// 读取文件内容
	for (int k = 0; k < NZ; k++) {
		for (int j = 0; j < NY; j++) {
			for (int i = 0; i < NX; i++) {
				char c;
				if (!file.get(c)) {
					std::cerr << "Unexpected end of file\n";
					return 0;
				}
				//std::cout << i << " " << j << " " << k << std::endl;

				short int value = static_cast<short int>(c);
				scalarFunction[i][j][k] = value;
				if (value > themax) themax = value;
				if (value < themin) themin = value;

				//outFile << i << " " << j << " " << k << " " << value << '\n';

				//float value;
				////size_t readCount = fread(&value, sizeof(float), 1, inputFile);
				////if (readCount != 1) {
				////	std::cerr << "Error reading file: " << path << std::endl;
				////	fclose(inputFile);
				////	return scalarFunction;
				////}
				//fread(&value, sizeof(float), 1, inputFile);
				//scalarFunction[i][j][k] = value;
			}
		}
	}

	file.close();
	std::cerr << "Volumetric data range: " << themin << " -> " << themax << '\n';
	
	return 1;
}