#ifndef dungeonH
#define dungeonH

#include "Leaf.h"
#include "../objects/ContactListener.h"
#include "../objects/Chest.h"
#include "../objects/LevelExit.h"
#include "../objects/Enemy.h"
#include "../objects/Hero.h"
#include "../delaunay/delaunay.h"
#include "../delaunay/graph.h"
#include "../buffers/StaticBuffer.h"

class MDungeon {
private:
	int LevelNumber;
	MHero* pHero;
	
	//tree
	MLeaf* LeafRoot;
	vector<MLeaf*> Leafs;
	vector<NLine2> Lines;//edges
	
	//buffers
	MStaticBuffer sbRooms;
	MStaticBuffer sbLeafs;
	MStaticBuffer sbLines;
	MStaticBuffer sbLinks;
	MStaticBuffer sbHalls;
	
	//rooms data
	vector<glm::vec2> RoomsCenters;
	map<glm::vec2*, NRectangle2*> CentersMap;
	map<int, map<int, vector<int> > > PathsMap;
	void CreatePathsMap(int** AdjencyArray, int Size);
	NRectangle2* GetRoomByCenter(glm::vec2 Point);
	NRectangle2* GetRoomByNumber(int Number);
	
	//tile settings
	glm::vec2 TileSize;
	unsigned int TilesCount[2];
	
	//physic
	b2World* World;
	MContactListener ContactListener;
	b2Vec2 Gravity;
	float timeStep;
	int velocityIterations;
	int positionIterations;
	float PhysicScale;//100 pixel - 1 meter
	b2Body* staticBody;
	vector<b2Fixture*> staticFixtures;
	unsigned char** PrePhysicMap;
	void CreateWalls();
	
	//dungeon create stages
	bool CreateLevel();
	void Clear();
public:
	MDungeon();
	bool Initialize(unsigned int MapSizeX, unsigned int MapSizeY, float TileSizeX, float TileSizeY);
	bool Generate();
	void ProcessEnemies();
	void Draw();
	b2World* GetWorld();
	MLeaf* GetLeafRoot();
	float GetScale();
	void WorldStep();
	void Close();
	unsigned int GetLevelNumber();
};

#endif
