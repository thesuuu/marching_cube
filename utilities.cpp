#include "utilities.h"

VertexContainer hash_vertices_to_indices(std::vector<Triangle>& triangles)
{
    VertexContainer container;
    int cnt = 0;
    for (auto& triangle : triangles)
    {
        std::vector<int> indices;
		for (int i = 0; i < 3; i++)
		{
			// 为每个顶点生成一个唯一的索引
			if (container.vertexMap.count(triangle.p[i]) == 0)
			{
				container.vertexMap[triangle.p[i]] = cnt;
				cnt++;
			}
			indices.push_back(container.vertexMap[triangle.p[i]]);
		}
        container.triangles.push_back(indices);
    }

    return container;
}


void write_to_ply(std::vector<Triangle>& triangles, const char* path)
{
    VertexContainer container = hash_vertices_to_indices(triangles);

    std::ofstream outputFile;
    outputFile.open(path);

    outputFile << "ply\n";
    outputFile << "format ascii 1.0\n";
    outputFile << "element vertex " << container.vertexMap.size() << "\n";
    outputFile << "property float32 x\n";
    outputFile << "property float32 y\n";
    outputFile << "property float32 z\n";
    outputFile << "element face " << container.triangles.size() << "\n";
    outputFile << "property list uint8 int32 vertex_indices\n";
    outputFile << "end_header\n";

    // 写入顶点数据
    for (auto& vertexPair : container.vertexMap)
    {
        const Point& vertex = vertexPair.first;
        outputFile << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
    }
    // 写入面数据
    for (auto& triangle : container.triangles)
    {
        outputFile << 3 << " ";
        for (int index : triangle)
            outputFile << index << " ";
        outputFile << "\n";
    }
}
