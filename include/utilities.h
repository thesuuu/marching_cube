#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "datatype.h"

void write_to_ply(std::vector<Triangle>& triangles, const char* path);
void write_triangles_to_file(std::vector<Triangle> triangles, const char* path);