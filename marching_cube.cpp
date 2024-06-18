// marching cube.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "marching_cube.h"



int MarchingCube::cal_index(GridCell& Cell, float isovalue)
{
    int cubeindex = 0;
    for (int i = 0; i < 8; i++) {
        // 如果顶点的值小于等于等值面的值，那么对应的位设为1，表示在等值面下方
        if(Cell.value[i] < isovalue) cubeindex |= 1 << i;
    }
    return cubeindex;
}

Point MarchingCube::interpolate(Point p1, Point p2, float v1, float v2, float isovalue)
{
    Point p;

    if (abs(isovalue - v1) < 0.00001) {
        return p1;
    }
    if (abs(isovalue - v2) < 0.00001) {
		return p2;
	}
    if (abs(v1 - v2) < 0.00001) {
		return p1;
	}

    float alpha = (isovalue - v1) / (v2 - v1);
    p.x = p1.x + alpha * (p2.x - p1.x);
    p.y = p1.y + alpha * (p2.y - p1.y);
    p.z = p1.z + alpha * (p2.z - p1.z);
    return p;

}

std::vector<Point> MarchingCube::get_intersection_coordinates(GridCell& cell, float isovalue)
{
    std::vector<Point> intersections(12);

    int cubeIndex = cal_index(cell, isovalue);
    int intersectionKey = edgeTable[cubeIndex];

    int idx = 0;
    while (intersectionKey) {
        // 如果当前边被选中（根据查表得到）
        if (intersectionKey & 1) {
            int v1 = edgeToVertices[idx].first, v2 = edgeToVertices[idx].second;
            // 插值得到三角面片的顶点的坐标
            Point intersectionPoint = interpolate(cell.p[v1], cell.p[v2], cell.value[v1], cell.value[v2], isovalue);
            intersections[idx] = intersectionPoint;
        }
        idx++;
        intersectionKey >>= 1; // 右移一位, 以检查下一条边
    }
    return intersections;
}

std::vector<Triangle> MarchingCube::Triangulation(std::vector<Point>& intersections, int cubeindex)
{
    std::vector<Triangle> triangles;
    for (int i = 0; triangleTable[cubeindex][i] != -1; i += 3) {
		Triangle triangle;
        for (int j = 0; j < 3; j++) {
            // 查表得到三角面片的三个顶点的所在边
			int edge = triangleTable[cubeindex][i + j];
            // 读取intersections中插值的三个顶点的坐标作为三角面片的顶点
			triangle.p[j] = intersections[edge];
		}
		triangles.push_back(triangle);

	}

    return triangles;
}

std::vector<Triangle> MarchingCube::triangulate_cell(GridCell& cell, float isovalue)
{
    int cubeindex = cal_index(cell, isovalue);
    std::vector<Point> intersections = get_intersection_coordinates(cell, isovalue);
    std::vector<Triangle> triangles = Triangulation(intersections, cubeindex);
    return triangles;
}

std::vector<Triangle> MarchingCube::triangulate_field(std::vector<std::vector<std::vector<float>>>& scalarField, float isovalue)
{
    int size = static_cast<int>(scalarField.size());
    std::vector<Triangle> triangles;

    for (int i = 0; i + 1 < size; i++)
    {
        for (int j = 0; j + 1 < size; j++)
        {
            for (int k = 0; k + 1 < size; k++)
            {
                int x = i, y = j, z = k;
                // cell ordered according to convention in referenced website
                GridCell cell =
                {
                    {
                        {x, y, z}, {x + 1, y, z},
                        {x + 1, y, z + 1}, {x, y, z + 1},
                        {x, y + 1, z}, {x + 1, y + 1, z},
                        {x + 1, y + 1, z + 1}, {x, y + 1, z + 1}
                    },
                    {
                        scalarField[i][j][k], scalarField[i + 1][j][k],
                        scalarField[i + 1][j][k + 1], scalarField[i][j][k + 1],
                        scalarField[i][j + 1][k], scalarField[i + 1][j + 1][k],
                        scalarField[i + 1][j + 1][k + 1], scalarField[i][j + 1][k + 1]
                    }
                };
                std::vector<Triangle> cellTriangles = triangulate_cell(cell, isovalue);
                for (int i = 0; i < static_cast<int>(cellTriangles.size()); i++)
                    triangles.push_back(cellTriangles[i]);
            }
        }
    }
    return triangles;
}

//const int MarchingCube::SearchingEdgeTable(int cubeindex)
//{
//    return edgeTable[cubeindex];
//}
//
//const int* MarchingCube::SearchingTriTable(int index)
//{
//    return triangleTable[index];
//}



