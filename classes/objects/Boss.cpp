#include "../../stdafx.h"
#include "Boss.h"

MBoss::MBoss():Living() {
	BodySensor = NULL;
	pTarget = NULL;
	pArena = NULL;
	Health = 1;
	InRush = false;
	RushVelocity = 1;
	MainVelocity = 0.4;
	ActionsCount = 5; //for tests 5 (none, teleport, rush, bullet, zone) 
	TimerLimit = 0;
}

void MBoss::Create(glm::vec2 Position, glm::vec2 inSize, glm::vec3 Color, float inScale, b2World* inpWorld, b2BodyType Type, uint16 FilterCategory, uint16 FilterMask, bool Sensor) {
	//main fixture block go throght walls
	MPhysicQuad::Create(Position, inSize, Color, inScale, inpWorld, Type, FilterCategory, FilterMask, Sensor);
	
	PrevPosition = Body->GetPosition();
	
	//create body sensor (hero can move throught enemy)
	Box.SetAsBox(inSize.x * 0.5 * inScale, inSize.y * 0.5 * inScale);
	fixtureDef.filter.categoryBits = OT_ENBODY;
	fixtureDef.filter.maskBits = OT_HERO | OT_BULLET;
	fixtureDef.isSensor = true;
	fixtureDef.shape = &Box;
	BodySensor = Body->CreateFixture(&fixtureDef);
}

void MBoss::OnBeginCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {
}

void MBoss::OnEndCollideWithF(MPhysicQuad* pPhysicQuad, uint16 Type) {
}

void MBoss::OnBeginWallCollide() {
	if(InRush) {
		InRush = fasle;
		SetVelocity(b2Vec2(MainVelocity * Direction.x, MainVelocity * Direction.y));
	}
}

void MBoss::OnEndWallCollide() {
}

void MBoss::Decision() {
	if(!DecisionTimer.Time()) return;
	if(ActionsCount <= 1) {
		cout<<"Actions count are too few"<<endl;
		return; 
	}
	unsigned char CurrentAction = rand() % ActionsCount;
	DecisionTimer.Start();
	//apply selected action
	cout<<"Boss make decision: "<<CurrentAction<<endl;
}

void MBoss::Close(){
	if(BodySensor) Body->DestroyFixture(BodySensor);
	MPhysicQuad::Close();
}

void MBoss::Move() {
	if(Status & LS_ACTIVE != LS_ACTIVE) return;
	if(InRush) return;
	if(pTarget) {
		glm::vec2 Direction;
		if(glm::length(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter()) <= 8) {
			SetVelocity(b2Vec2(0, 0));
			return;
		}
		Direction = glm::normalize(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter());
		SetVelocity(b2Vec2(MainVelocity * Direction.x, MainVelocity * Direction.y));
		return;
	}
}

void MBoss::Update() {
	if(Health <= 0) {
		NeedRemove = true;
		return;
	}
	if(Status & LS_ACTIVE == LS_ACTIVE) Decision();
	MPhysicQuad::Update();
}

void MBoss::SetTarget(MPhysicQuad* inpTarget) {
	pTarget = inpTarget;
}

void MBoss::SetArena(MArena* inpArena) {
	pArena = inpArena;
}

void MBoss::SetStatus(char Status) {
	CurentStatus = Status;
}

void MBoss::SetDecisionTimerLimit(DWORD Limit) {
	DecisionTimer.SetLimit(Limit);
}

void MBoss::DecisionsStart() {
	if(DecisionTimer.Start()) cout<<"Start boss decision timer: success"<<endl;
	else cout<<"Start boss decision timer: fail"<<endl;
}

void MBoss::DecisionsStop() {
	DecisionTimer.Stop();
}

void MBoss::Teleport(char Type) {
	switch(Type) {
		case TT_TOHERO:
			if(pTarget) Body->SetTransform(pTarget->GetColorQuad()->GetCenter(), 0);
			break;
		case TT_RANDOM:
			unsigned int RndTile[2] = {0, 0};
			RndTile[0] = rand() % *(pArena->GetTilesCount());
			RndTile[1] = rand() % *(pArena->GetTilesCount() + 1);
			b2Vec2 Position = b2Vec2(pArena->GetPhysicScale() * RndTile[0] * pArena->GetTileSize().x * 0.5, 
				pArena->GetPhysicScale() * RndTile[1] * pArena->GetTileSize().y * 0.5);
			Body->SetTransform(Position, 0);
			break;
	}
}

void MBoss::Rush() {
	if(InRush) return;
	//select direction
	Direction = glm::normalize(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter());
	SetVelocity(b2Vec2(RushVelocity * Direction.x, RushVelocity * Direction.y));
	//start rush
	InRush = true;
}

void MBoss::FireBullet(int Type) {
	glm::vec2 Direction;
	Direction = glm::normalize(pTarget->GetColorQuad()->GetCenter() - GetColorQuad()->GetCenter());
	glm::vec4 AddValue = glm::vec4(GetColorQuad()->GetCenter().x, GetColorQuad()->GetCenter().y, Direction.x, Direction.y);
	BulletsToCreate.push_back(AddValue);
}

vector<glm::vec4>* MBoss::GetBulletsToCreate() {
	return &BulletsToCreate;
}

void MBoss::CreateZone() {
	BulletsToCreate.push_back(pTarget->GetColorQuad()->GetCenter());
}

vector<glm::vec2>* MBoss::GetZonesToCreate() {
	return &ZonesToCreate;
}
