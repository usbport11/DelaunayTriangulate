#ifndef bossH
#define bossH

#include "Bullet.h"
#include "../level/Arena.h"
#include "../system/Timer3.h"

#define TT_TOHERO 1
#define TT_RANDOM 2

#define ZT_SELF 1
#define ZT_TOHERO 2
#define ZT_RANDOM 3

#define AC_NONE 0
#define AC_TELEPORT 1
#define AC_RUSH 2
#define AC_FIREBULLET 3
#define AC_CREATEZONE 4

class MBoss: public MLiving {
private:
	unsigned int ZoneTilesCount[2];
	unsigned int BulletTilesCount[2];
	vector<glm::vec4> BulletsToCreate;
	vector<glm::vec2> ZonesToCreate;
	MPhysicQuad* pTarget;
	b2Fixture* BodySensor;
	MArena* pArena;
	bool InRush;
	float RushVelocity;
	float MainVelocity;
	//decision values
	unsigned char ActionsCount;
	MTimer3 DecisionTimer;
	//actions array (mb vector, mb map)
protected:
	void OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type);
	void OnBeginWallCollide();
	void OnEndWallCollide();
	//dicision (decision timeout, rand from decision count, set action based on selected decision)
	void Decision();
	//actions
	void Teleport(char Type);
	void Rush();
	void FireBullet();
	void CreateZone();
public:
	MBoss();
	void Create(glm::vec2 Position, glm::vec2 inSize, glm::vec3 Color, float inScale, b2World* inpWorld, b2BodyType Type, uint16 FilterCategory, uint16 FilterMask, bool Sensor);
	void Close();
	void Move();
	void Update();
	void SetTarget(MPhysicQuad* inpTarget);
	void SetArena(MArena* inpArena);
	void SetStatus(char Status);
	void SetDecisionTimerLimit(DWORD Limit);
	void DecisionsStart();
	void DecisionsStop();
	vector<glm::vec4>* GetBulletsToCreate();
	vector<glm::vec2>* GetZonesToCreate();
};

#endif
