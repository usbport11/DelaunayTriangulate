#include "../../stdafx.h"
#include "Dungeon.h"

int MinimalDistance(int* inDistances, bool* inIncludePoints, int Size) {
	int MinIndex, Min = INT_MAX;
	for(int i=0; i<Size; i++) {
		if(inIncludePoints[i] == false && inDistances[i] <= Min) {
			Min = inDistances[i];
			MinIndex = i;
		}
	}
	return MinIndex;
}

void MDungeon::CreatePathsMap(int** AdjencyArray, int Size) {
	int* Distances = new int [Size];
	bool* IncludePoints = new bool [Size];
	int* Parents = new int [Size];
	
	for(int k=0; k<Size; k++) {
		memset(Distances, 0, Size);
		memset(IncludePoints, 0, Size);
		memset(Parents, 0, Size);
		
		for(int i=0; i<Size; i++) {
			Parents[i] = -1;
			Distances[i] = INT_MAX;
			IncludePoints[i] = false;
		}
		
		Distances[k] = 0;
		
		for(int i=0; i<Size - 1; i++) {
			int MinIndex = MinimalDistance(Distances, IncludePoints, Size);
			IncludePoints[MinIndex] = true;
			for(int j=0; j<Size; j++) {
				if(!IncludePoints[j] && AdjencyArray[MinIndex][j] && 
					Distances[MinIndex] + AdjencyArray[MinIndex][j] < Distances[j]) {
					Parents[j] = MinIndex;
					Distances[j] = Distances[MinIndex] + AdjencyArray[MinIndex][j];
				}
			}
		}
		
		map<int, vector<int> > SubMap;
		for(int i = 0; i<Size; i++) {
			if(k == i) {
				continue;
			}
			 
			int z = i;
			vector<int> NewPath;
			while(Parents[z] != -1) {
				NewPath.insert(NewPath.begin(), z);
				z = Parents[z];
			}
			SubMap.insert(pair<int, vector<int> >(i, NewPath));
		}
		
		PathsMap.insert(pair<int, map<int, vector<int> > >(k, SubMap));
	}
	
	delete [] Distances;
	delete [] IncludePoints;
	delete [] Parents;
}

NRectangle2* MDungeon::GetRoomByCenter(glm::vec2 Point) {
	vector<glm::vec2>::iterator it;
	it = find(RoomsCenters.begin(), RoomsCenters.end(), Point);
	if(it != RoomsCenters.end()) {
		map<glm::vec2*, NRectangle2*>::iterator mit;
		mit = CentersMap.find(&(*it));
		if(mit != CentersMap.end()) return mit->second;
	}
	return NULL;
}

NRectangle2* MDungeon::GetRoomByNumber(int Number) {
	if(Number < 0) return NULL;
	map<glm::vec2*, NRectangle2*>::iterator it;
	it = CentersMap.find(&RoomsCenters.at(Number));
	if(it != CentersMap.end()) return it->second;
	return NULL;
}

void MDungeon::CreateWalls() {
	//physic//
	//create double unsigned char array
	//fill 0 and 1
	//crate walls
}

bool MDungeon::CreateLevel() {
	Clear();
	
	//split tree
	LeafRoot = new MLeaf(NVector2(0,0), NVector2(TilesCount[0], TilesCount[1]));
	Leafs.push_back(LeafRoot);
	bool SplitSuccess = true;
	int LeafsCount = 0;
	while(SplitSuccess) {
		SplitSuccess = false;
		LeafsCount = Leafs.size();
		for(int i=0; i<LeafsCount; i++) {
			if(!Leafs[i]->LeafLeft && !Leafs[i]->LeafRight) {
				if (Leafs[i]->Size.x > MAX_LEAF_SIZE_2 || Leafs[i]->Size.y > MAX_LEAF_SIZE_2 || rand() % 100 > 25) {
					if(Leafs[i]->Split()) {
						Leafs.push_back(Leafs[i]->LeafLeft);
						Leafs.push_back(Leafs[i]->LeafRight);
						SplitSuccess = true;
					}
				}
			}
		}
	}
	//create rooms
	LeafRoot->CreateRooms();
	
	//fill leafs vertexes
	float rnd;
	for(int i=1; i<Leafs.size(); i++) {
		if(Leafs[i]) {
			rnd = rand() % 100 * 0.01;
			sbLeafs.AddVertex(glm::vec2(Leafs[i]->Position.x * TileSize[0], Leafs[i]->Position.y * TileSize[1]), glm::vec3(0, 1 - rnd, rnd));
			sbLeafs.AddVertex(glm::vec2(Leafs[i]->Position.x * TileSize[0] + Leafs[i]->Size.x * TileSize[0], Leafs[i]->Position.y * TileSize[1]), glm::vec3(0, 1 - rnd, rnd));
			sbLeafs.AddVertex(glm::vec2(Leafs[i]->Position.x * TileSize[0] + Leafs[i]->Size.x * TileSize[0], Leafs[i]->Position.y * TileSize[1] + Leafs[i]->Size.y * TileSize[1]), glm::vec3(0, 1 - rnd, rnd));
			sbLeafs.AddVertex(glm::vec2(Leafs[i]->Position.x * TileSize[0], Leafs[i]->Position.y * TileSize[1] + Leafs[i]->Size.y * TileSize[1]), glm::vec3(0, 1 - rnd, rnd));
		}
	}
	
	//fill rooms vertexes
	map<glm::vec2*, NRectangle2*>::iterator it;
	vector<NRectangle2*> Rooms;
	for(int i=0; i<Leafs.size(); i++) {
		if(Leafs[i]->Room) {
			sbRooms.AddVertex(glm::vec2(Leafs[i]->Room->Position.x * TileSize[0], Leafs[i]->Room->Position.y * TileSize[1]), glm::vec3(1, 1, 1));
			sbRooms.AddVertex(glm::vec2(Leafs[i]->Room->Position.x * TileSize[0] + Leafs[i]->Room->Size.x * TileSize[0], Leafs[i]->Room->Position.y * TileSize[1]), glm::vec3(1, 1, 1));
			sbRooms.AddVertex(glm::vec2(Leafs[i]->Room->Position.x * TileSize[0] + Leafs[i]->Room->Size.x * TileSize[0], Leafs[i]->Room->Position.y * TileSize[1] + Leafs[i]->Room->Size.y * TileSize[1]), glm::vec3(1, 1, 1));
			sbRooms.AddVertex(glm::vec2(Leafs[i]->Room->Position.x * TileSize[0], Leafs[i]->Room->Position.y * TileSize[1] + Leafs[i]->Room->Size.y * TileSize[1]), glm::vec3(1, 1, 1));
			//get rooms centers
			RoomsCenters.push_back(glm::vec2(Leafs[i]->Room->Position.x * TileSize[0] + Leafs[i]->Room->Size.x * TileSize[0] * 0.5f, 
				Leafs[i]->Room->Position.y * TileSize[1] + Leafs[i]->Room->Size.y * TileSize[1] * 0.5f));
			Rooms.push_back(Leafs[i]->Room);
		}
	}
	//link centers with rooms
	for(int i=0; i<RoomsCenters.size(); i++) {
		CentersMap.insert(pair<glm::vec2*, NRectangle2*>(&RoomsCenters[i], Rooms[i]));
	}
	Rooms.clear();
	
	//triangulate and create minimal complex graph
	MDelaunay Triangulation;
	MGraph Graph;
	vector<MTriangle> Triangles = Triangulation.Triangulate(RoomsCenters);
	vector<MEdge> Edges = Triangulation.GetEdges();
	Graph.Create(&Edges, &RoomsCenters);
	Graph.CreateMinimalEdges(&Edges);
	
	//create halls and add its to rooms array
	//create new edges array, because hall between rooms sub original edge between rooms by 1 or 2
	//vector<MEdge> NewEdges;
	vector<NRectangle2*> RoomsToConnect;
	vector<NRectangle2> Halls;
	for(int i=0; i<Graph.MinimalEdges.size(); i++) {
		//search
		for(it = CentersMap.begin(); it != CentersMap.end(); it++) {
			if(*it->first == Graph.MinimalEdges[i].p1 || *it->first == Graph.MinimalEdges[i].p2) RoomsToConnect.push_back(it->second);
		}
		//create hall (1 or 2)
		if(RoomsToConnect.size() == 2) {
			Halls = ConnectRooms(RoomsToConnect[0], RoomsToConnect[1]);
			//add in buffer
			for(int j=0; j<Halls.size(); j++) {
				sbHalls.AddVertex(glm::vec2(Halls[j].Position.x * TileSize[0], Halls[j].Position.y * TileSize[1]), glm::vec3(1, 1, 0));
				sbHalls.AddVertex(glm::vec2(Halls[j].Position.x * TileSize[0] + Halls[j].Size.x * TileSize[0], Halls[j].Position.y * TileSize[1]), glm::vec3(1, 1, 0));
				sbHalls.AddVertex(glm::vec2(Halls[j].Position.x * TileSize[0] + Halls[j].Size.x * TileSize[0], Halls[j].Position.y * TileSize[1] + Halls[j].Size.y * TileSize[1]), glm::vec3(1, 1, 0));
				sbHalls.AddVertex(glm::vec2(Halls[j].Position.x * TileSize[0], Halls[j].Position.y * TileSize[1] + Halls[j].Size.y * TileSize[1]), glm::vec3(1, 1, 0));
			}
			//clear
			RoomsToConnect.clear();
		}
		/*
		//recreate edges and add new centers (in centers and link map)
		NewEdges.push_back(MEdge(Graph.MinimalEdges[i].p1, GetRoomCenter(&Halls[0], TileSize)));
		RoomsCenters.push_back(GetRoomCenter(&Halls[0], TileSize));
		CentersMap.insert(pair<glm::vec2*, NRectangle2*>(&RoomsCenters.back(), &Halls[0]));
		for(int j=1; j<Halls.size(); j++) {
			NewEdges.push_back(MEdge(GetRoomCenter(&Halls[j-1], TileSize), GetRoomCenter(&Halls[j], TileSize)));
			RoomsCenters.push_back(GetRoomCenter(&Halls[j], TileSize));
			CentersMap.insert(pair<glm::vec2*, NRectangle2*>(&RoomsCenters.back(), &Halls[j]));
		}
		NewEdges.push_back(MEdge(GetRoomCenter(&Halls.back(), TileSize), Graph.MinimalEdges[i].p2));
		*/
	}
	RoomsToConnect.clear();
	Halls.clear();
	
	//create empty adjency matrix
	int CentersCount = RoomsCenters.size();
	int** AdjencyMatrix = new int* [CentersCount];
	for(int i=0; i<CentersCount; i++) {
		AdjencyMatrix[i] = new int [CentersCount];
		memset(AdjencyMatrix[i], 0, CentersCount * sizeof(int));
	}
	//fill adjency matrix
	vector<glm::vec2>::iterator cit;
	int Number[2];
	for(int i = 0; i<Graph.MinimalEdges.size(); i++) {
		cit = find(RoomsCenters.begin(), RoomsCenters.end(), Graph.MinimalEdges[i].p1);
		Number[0] = cit - RoomsCenters.begin();
		cit = find(RoomsCenters.begin(), RoomsCenters.end(), Graph.MinimalEdges[i].p2);
		Number[1] = cit - RoomsCenters.begin();
		AdjencyMatrix[Number[0]][Number[1]] = glm::distance(Graph.MinimalEdges[i].p1, Graph.MinimalEdges[i].p2);
		AdjencyMatrix[Number[1]][Number[0]] = AdjencyMatrix[Number[0]][Number[1]];
	}
	//create paths map
	CreatePathsMap(AdjencyMatrix, CentersCount);
	//free memory
	for(int i=0; i<CentersCount; i++) {
		delete [] AdjencyMatrix[i];
	}
	delete [] AdjencyMatrix;
	
	//fill lines
	//for(int i = 0; i<Edges.size(); i++) {
	//	sbLines.AddVertex(Edges[i].p1, glm::vec3(0, 0, 0));
	//	sbLines.AddVertex(Edges[i].p2, glm::vec3(0, 0, 0));
	//}
	
	//fill links
	for(int i = 0; i<Graph.MinimalEdges.size(); i++) {
		sbLinks.AddVertex(Graph.MinimalEdges[i].p1, glm::vec3(1, 0, 0));
		sbLinks.AddVertex(Graph.MinimalEdges[i].p2, glm::vec3(1, 0, 0));
	}
	
	//clear arrays
	Triangulation.Clear();
	Triangles.clear();
	Edges.clear();
	Graph.Clear();
	
	//dispose
	if(!sbLeafs.Dispose()) return false;
	if(!sbRooms.Dispose()) return false;
	if(!sbHalls.Dispose()) return false;
	//if(!sbLines.Dispose()) return false;
	if(!sbLinks.Dispose()) return false;
	
	return true;
}

MDungeon::MDungeon() {
	TileSize = glm::vec2(0.0f, 0.0f);
	TilesCount[0] = TilesCount[1] = 0;
	
	Gravity = b2Vec2(0, 0);
	timeStep = 1 / 60.0;
	velocityIterations = 8;
	positionIterations = 3;
	PhysicScale = 0.01;
	World = NULL;
	staticBody = NULL;
	
	LevelNumber = -1;
}

bool MDungeon::Initialize(unsigned int MapSizeX, unsigned int MapSizeY, float TileSizeX, float TileSizeY) {
	if(TileSizeX <= 0 || TileSizeY <= 0) return false;
	
	TilesCount[0] = MapSizeX;
	TilesCount[1] = MapSizeY;
	TileSize = glm::vec2(TileSizeX, TileSizeY);
	
	if(!sbLeafs.Initialize()) return false;
	sbLeafs.SetPrimitiveType(GL_QUADS);
	if(!sbRooms.Initialize()) return false;
	sbRooms.SetPrimitiveType(GL_QUADS);
	if(!sbHalls.Initialize()) return false;
	sbHalls.SetPrimitiveType(GL_QUADS);
	if(!sbLines.Initialize()) return false;
	sbLines.SetPrimitiveType(GL_LINES);
	if(!sbLinks.Initialize()) return false;
	sbLinks.SetPrimitiveType(GL_LINES);
	
	World = new b2World(Gravity);
	World->SetContactListener(&ContactListener);
	
	return true;
}

bool MDungeon::Generate() {
	if(!World) return false;
	
	LevelNumber ++;
	if(!CreateLevel()) return false;
	
	return true;
}

void MDungeon::Draw() {
	sbLeafs.Draw();
	sbRooms.Draw();
	sbLines.Draw();
	sbLinks.Draw();
	sbHalls.Draw();
}

b2World* MDungeon::GetWorld() {
	return World;
}

MLeaf* MDungeon::GetLeafRoot() {
	return LeafRoot;
}

float MDungeon::GetScale() {
	return PhysicScale;
}

void MDungeon::WorldStep() {
	World->Step(timeStep, velocityIterations, positionIterations);
}

void MDungeon::Clear() {
	sbLeafs.Clear();
	sbRooms.Clear();
	sbLines.Clear();
	sbLinks.Clear();
	sbHalls.Clear();
	
	RoomsCenters.clear();
	CentersMap.clear();
	PathsMap.clear();
	
	for(int i=0; i<Leafs.size(); i++) {
		if(Leafs[i]->Room) delete Leafs[i]->Room;
		if(Leafs[i]->LeafLeft) delete Leafs[i]->LeafLeft;
		if(Leafs[i]->LeafRight) delete Leafs[i]->LeafRight;
	}
	if(LeafRoot) delete LeafRoot;
	Leafs.clear();
}

void MDungeon::Close() {
	Clear();
	
	sbLeafs.Close();
	sbRooms.Close();
	sbLines.Close();
	sbLinks.Close();
	sbHalls.Close();
	
	if(World) {
		if(staticBody) {
			for(int i=0; i<staticFixtures.size(); i++) {
				if(staticFixtures[i]) staticBody->DestroyFixture(staticFixtures[i]);
			}
			staticFixtures.clear();
			World->DestroyBody(staticBody);
		}
		delete World;
	} 
}

unsigned int MDungeon::GetLevelNumber() {
	return LevelNumber;
}
