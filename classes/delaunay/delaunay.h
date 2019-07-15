#ifndef delaunayH
#define delaunayH

#include <vector>
#include <algorithm>
#include "triangle.h"

class MDelaunay {
private:
	std::vector<MTriangle> _triangles;
	std::vector<MEdge> _edges;
	std::vector<glm::vec2> _vertices;
public:
	std::vector<MTriangle>& Triangulate(std::vector<glm::vec2> &vertices);
	std::vector<MTriangle>& GetTriangles();
	std::vector<MEdge>& GetEdges();
	std::vector<glm::vec2>& GetVertices();
	void Clear();
};

#endif
