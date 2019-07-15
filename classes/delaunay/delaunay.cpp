#include "delaunay.h"

std::vector<MTriangle>& MDelaunay::Triangulate(std::vector<glm::vec2> &vertices) {
	// Store the vertices localy
	_vertices = vertices;

	// Determinate the super triangle
	float minX = vertices[0].x;
	float minY = vertices[0].y;
	float maxX = minX;
	float maxY = minY;

	for(std::size_t i = 0; i < vertices.size(); ++i) {
		if (vertices[i].x < minX) minX = vertices[i].x;
		if (vertices[i].y < minY) minY = vertices[i].y;
		if (vertices[i].x > maxX) maxX = vertices[i].x;
		if (vertices[i].y > maxY) maxY = vertices[i].y;
	}
			
	float dx = maxX - minX;
	float dy = maxY - minY;
	float deltaMax = std::max(dx, dy);
	float midx = (minX + maxX) / 2.0f;
	float midy = (minY + maxY) / 2.0f;

	glm::vec2 p1 = glm::vec2(midx - 20 * deltaMax, midy - deltaMax);
	glm::vec2 p2 = glm::vec2(midx, midy + 20 * deltaMax);
	glm::vec2 p3 = glm::vec2(midx + 20 * deltaMax, midy - deltaMax);
			
	// Create a list of triangles, and add the supertriangle in it
	_triangles.push_back(MTriangle(p1, p2, p3));

	for(std::vector<glm::vec2>::iterator p = vertices.begin(); p != vertices.end(); p++) {
		std::vector<MTriangle> badTriangles;
		std::vector<MEdge> polygon;

		for(std::vector<MTriangle>::iterator t = _triangles.begin(); t != _triangles.end(); t++) {
			if(t->CircumCircleContains(*p)) {
				badTriangles.push_back(*t);
				polygon.push_back(t->e1);
				polygon.push_back(t->e2);
				polygon.push_back(t->e3);
			}
			else{
			}
		}
		
		for(std::vector<MTriangle>::iterator bt = badTriangles.begin(); bt != badTriangles.end(); bt++) {
			_triangles.erase(std::remove(_triangles.begin(), _triangles.end(), *bt), _triangles.end());
		}

		std::vector<MEdge> badEdges;
		for(std::vector<MEdge>::iterator e1 = polygon.begin(); e1 != polygon.end(); e1++) {
			for(std::vector<MEdge>::iterator e2 = polygon.begin(); e2 != polygon.end(); e2++) {
				if(e1 == e2) continue;
				if(*e1 == *e2) {
					badEdges.push_back(*e1);
					badEdges.push_back(*e2);
				}
			}
		}

		for(std::vector<MEdge>::iterator be = badEdges.begin(); be != badEdges.end(); be++) {
			polygon.erase(std::remove(polygon.begin(), polygon.end(), *be), polygon.end());
		}

		for(std::vector<MEdge>::iterator e = polygon.begin(); e != polygon.end(); e++) {
			_triangles.push_back(MTriangle(e->p1, e->p2, *p));
		}
	}

	_triangles.erase(std::remove_if(_triangles.begin(), _triangles.end(), stTriangleContains(p1, p2, p3)), _triangles.end());
	
	for(std::vector<MTriangle>::iterator t = _triangles.begin(); t != _triangles.end(); t++) {
		_edges.push_back(t->e1);
		_edges.push_back(t->e2);
		_edges.push_back(t->e3);
	}
	
	//remove backward edges
	for(std::vector<MEdge>::iterator e = _edges.begin(); e != _edges.end(); e++) {
		_edges.erase(std::remove_if(_edges.begin(), _edges.end(), stIsEdgeBackward(*e)), _edges.end());
	}

	return _triangles;
}
	
std::vector<MTriangle>& MDelaunay::GetTriangles() {
	return _triangles; 
}

std::vector<MEdge>& MDelaunay::GetEdges() {
	return _edges; 
}

std::vector<glm::vec2>& MDelaunay::GetVertices() { 
	return _vertices;
}

void MDelaunay::Clear() {
	_triangles.clear();
	_edges.clear();
	_vertices.clear();
}
