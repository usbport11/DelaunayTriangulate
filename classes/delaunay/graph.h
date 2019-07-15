#ifndef graphH
#define graphH

#include "Edge.h"

struct stGPoint {
	stGPoint() {
		p = glm::vec2(0, 0);
	}
	stGPoint(glm::vec2 inp) {
		p = inp;
	}
	std::vector<stGPoint*> NearPoints;
	glm::vec2 p;
};

struct stGPointEqual {
	stGPoint GPoint;
	stGPointEqual(glm::vec2 inp) {
		GPoint = stGPoint(inp);
	}
	bool operator() (const stGPoint& gp) const {
		return GPoint.p.x == gp.p.x && GPoint.p.y == gp.p.y;
	}
	bool operator() (const stGPoint* gp) const {
		return GPoint.p.x == gp->p.x && GPoint.p.y == gp->p.y;
	}
};

class MGraph {
public:
	std::vector<stGPoint*> GPoints;
	std::vector<MEdge> MinimalEdges;
	void Create(std::vector<MEdge>* pEdges, std::vector<glm::vec2>* AllPoints);
	void CreateMinimalEdges(std::vector<MEdge>* pEdges);
	void Clear();
};

#endif

