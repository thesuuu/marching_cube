#pragma once
#include <map>
#include <vector>

struct Point
{	
	int x;
	int y;
	int z;

	bool operator<(const Point& rhs) const
	{
		if (x != rhs.x)
			return x < rhs.x;
		if (y != rhs.y)
			return y < rhs.y;
		return z < rhs.z;
	}
};

struct GridCell
{
	Point p[8];
	float value[8];
};

struct Triangle
{
	Point p[3];
};

struct VertexContainer
{
	std::map<Point, int> vertexMap;
	std::vector<std::vector<int>> triangles;
};