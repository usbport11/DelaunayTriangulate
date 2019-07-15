#include "../../stdafx.h"
#include "graph.h"

void MGraph::Create(std::vector<MEdge>* pEdges, std::vector<glm::vec2>* pAllPoints) {
	if(!pEdges) return;
	if(pEdges->empty()) return;
	if(!pAllPoints) return;
	if(pAllPoints->empty()) return;
	
	//copy data
	for(int i = 0; i < pAllPoints->size(); i++) {
		GPoints.push_back(new stGPoint(pAllPoints->at(i)));
	}
	
	//cycle on points
	char PointNumber;
	std::vector<stGPoint*>::iterator it;
	for(int i = 0; i < GPoints.size(); i++) {
		//cycle by edges
		for(int j = 0; j < pEdges->size(); j++) {
			//search point on edge
			PointNumber = pEdges->at(j).SecondPointByPoint(GPoints[i]->p);
			if(PointNumber == -1) continue;
			//check duplicates in near points of current point
			it = find_if(GPoints[i]->NearPoints.begin(), GPoints[i]->NearPoints.end(), stGPointEqual(pEdges->at(j).GetPointByNumber(PointNumber)));
			if(it == GPoints[i]->NearPoints.end()) {
				//find point in main points array
				it = find_if(GPoints.begin(), GPoints.end(), stGPointEqual(pEdges->at(j).GetPointByNumber(PointNumber)));
				//add near point in main point
				GPoints[i]->NearPoints.push_back((stGPoint*)*it);
			}
		}
	}
}

void MGraph::CreateMinimalEdges(std::vector<MEdge>* pEdges) {
	if(GPoints.empty()) return;
	if(!pEdges) return;
	if(pEdges->empty()) return;
	
	int ReadyPointCount = 1;
	float MinDistance, Distance;
	stGPoint* pNearestGPoint[2];
	std::vector<stGPoint*> UsedGPoints;
	std::vector<stGPoint*>::iterator it;
	
	//add fisrt point
	UsedGPoints.push_back(GPoints[0]);
	LogFile<<"Graph: start create minimal graph"<<endl;
	LogFile<<"================================="<<endl;
	LogFile<<"Start point: "<<GPoints[0]->p[0]<<" "<<GPoints[0]->p[1]<<endl;
	
	//main cycle
	while(UsedGPoints.size() < GPoints.size() && MinDistance > 0) {
		MinDistance = 0;
		
		//get start minimal distance and points
		LogFile<<"Used points: "<<UsedGPoints.size()<<endl;
		for(int j=0; j<UsedGPoints.size() && !MinDistance; j++) {
			for(int i=0; i<UsedGPoints[j]->NearPoints.size(); i++) {
				it = find(UsedGPoints.begin(), UsedGPoints.end(), UsedGPoints[j]->NearPoints[i]);
				if(it == UsedGPoints.end()) {
					MinDistance = glm::distance(UsedGPoints[j]->p, UsedGPoints[j]->NearPoints[i]->p);
					pNearestGPoint[0] = UsedGPoints[j];
					pNearestGPoint[1] = UsedGPoints[j]->NearPoints[i];
					break;
				}
			}
		}
		if(!MinDistance) break;
		LogFile<<"Start mini dist points: ("<<pNearestGPoint[0]->p[0]<<", "<<pNearestGPoint[0]->p[1]<<") ("<<pNearestGPoint[1]->p[0]<<", "<<pNearestGPoint[1]->p[1]<<")"<<endl;
		
		//main points cycle
		for(int j=0; j<UsedGPoints.size(); j++) {
			//near points cycle
			LogFile<<" | ";
			for(int i=0; i<UsedGPoints[j]->NearPoints.size(); i++) {
				//skip point that alredy used and used points count more than 1
				if(UsedGPoints.size() > 1) {
					it = find(UsedGPoints.begin(), UsedGPoints.end(), UsedGPoints[j]->NearPoints[i]);
					if(it != UsedGPoints.end()) continue;
				}
				LogFile<<"("<<UsedGPoints[j]->NearPoints[i]->p[0]<<" "<<UsedGPoints[j]->NearPoints[i]->p[1]<<") ";
				//get min distance and points
				Distance = glm::distance(UsedGPoints[j]->p, UsedGPoints[j]->NearPoints[i]->p);
				if(MinDistance > Distance) {
					MinDistance = Distance;
					pNearestGPoint[0] = UsedGPoints[j];
					pNearestGPoint[1] = UsedGPoints[j]->NearPoints[i];
				}
			}
		}
		LogFile<<endl;
		
		//erase
		it = find(pNearestGPoint[1]->NearPoints.begin(), pNearestGPoint[1]->NearPoints.end(), pNearestGPoint[0]);
		if(it != pNearestGPoint[1]->NearPoints.end()) pNearestGPoint[1]->NearPoints.erase(it);
		it = find(pNearestGPoint[0]->NearPoints.begin(), pNearestGPoint[0]->NearPoints.end(), pNearestGPoint[1]);
		if(it != pNearestGPoint[0]->NearPoints.end()) pNearestGPoint[0]->NearPoints.erase(it);
		//push new point
		UsedGPoints.push_back(pNearestGPoint[1]);
		//create edge
		MinimalEdges.push_back(MEdge(pNearestGPoint[0]->p, pNearestGPoint[1]->p));
		LogFile<<"("<<MinimalEdges.back().p1[0]<<", "<<MinimalEdges.back().p1[1]<< ") ("<<MinimalEdges.back().p2[0]<<", "<<MinimalEdges.back().p2[1]<<")"<<endl;
	}
	
	UsedGPoints.clear();
}

void MGraph::Clear() {
	for(int i=0; i<GPoints.size(); i++) {
		if(GPoints[i]) delete GPoints[i];
	}
	GPoints.clear();
	MinimalEdges.clear();
}
