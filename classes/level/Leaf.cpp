#include "Leaf.h"

std::vector<NRectangle2> VerticalConnect(NRectangle2* pRoom1, NRectangle2* pRoom2, int CenterX) {
	std::vector<NRectangle2> Result;
	glm::vec2 Center1, Center2;
	NRectangle2 Hall;
	Hall.Size.x = 1;
	Hall.Position.x = CenterX;
	Center1 = glm::vec2(pRoom1->Position.x + pRoom1->Size.x * 0.5, pRoom1->Position.y + pRoom1->Size.y * 0.5);
	Center2 = glm::vec2(pRoom2->Position.x + pRoom2->Size.x * 0.5, pRoom2->Position.y + pRoom2->Size.y * 0.5);
	if(Center1.y > Center2.y) {
		Hall.Size.y = pRoom1->Position.y - (pRoom2->Position.y + pRoom2->Size.y);
		Hall.Position.y = pRoom2->Position.y + pRoom2->Size.y;
	}
	else {
		Hall.Size.y = pRoom2->Position.y - (pRoom1->Position.y + pRoom1->Size.y);
		Hall.Position.y = pRoom1->Position.y + pRoom1->Size.y;
	}
	Result.push_back(Hall);
	
	return Result;
}

std::vector<NRectangle2> HorizontalConnect(NRectangle2* pRoom1, NRectangle2* pRoom2, int CenterY) {
	std::vector<NRectangle2> Result;
	glm::vec2 Center1, Center2;
	NRectangle2 Hall;
	Hall.Size.y = 1;
	Hall.Position.y = CenterY;
	Center1 = glm::vec2(pRoom1->Position.x + pRoom1->Size.x * 0.5, pRoom1->Position.y + pRoom1->Size.y * 0.5);
	Center2 = glm::vec2(pRoom2->Position.x + pRoom2->Size.x * 0.5, pRoom2->Position.y + pRoom2->Size.y * 0.5);
	if(Center1.x > Center2.x) {
		Hall.Size.x = pRoom1->Position.x - (pRoom2->Position.x + pRoom2->Size.x);
		Hall.Position.x = pRoom2->Position.x + pRoom2->Size.x;
	}
	else {
		Hall.Size.x = pRoom2->Position.x - (pRoom1->Position.x + pRoom1->Size.x);
		Hall.Position.x = pRoom1->Position.x + pRoom1->Size.x;
	}
	Result.push_back(Hall);
	
	return Result;
}

NRectangle2 CreateHallFromPoint(NRectangle2* pRoom, NVector2 Center) {
	NRectangle2 Hall;
	if(Center.x > pRoom->Position.x && Center.x < pRoom->Position.x + pRoom->Size.x ) {
		Hall.Position.x = Center.x;
		Hall.Size.x = 1;
		//vertical
		if(Center.y > pRoom->Position.y + pRoom->Size.y) {
			//up
			Hall.Position.y = pRoom->Position.y + pRoom->Size.y;
			Hall.Size.y = Center.y - (pRoom->Position.y + pRoom->Size.y) + 1;
		}
		else {
			//down
			Hall.Position.y = Center.y;
			Hall.Size.y = pRoom->Position.y - Center.y;
		}
	}
	else {
		//horizontal
		Hall.Position.y = Center.y;
		Hall.Size.y = 1;
		if(Center.x > pRoom->Position.x + pRoom->Size.x) {
			//right
			Hall.Position.x = pRoom->Position.x + pRoom->Size.x;
			Hall.Size.x = Center.x - (pRoom->Position.x + pRoom->Size.x) + 1;
		}
		else {
			//left
			Hall.Position.x = Center.x;
			Hall.Size.x = pRoom->Position.x - Center.x;
		}
	}
	return Hall;
}

std::vector<NRectangle2> CompexConnect(NRectangle2* pRoom1, NRectangle2* pRoom2, NVector2 Center) {
	//first test
	if(pRoom1->Position.y + pRoom1->Size.y - pRoom2->Position.y >= 1 && pRoom2->Position.y >= pRoom1->Position.y) {
		return HorizontalConnect(pRoom1, pRoom2, pRoom2->Position.y);
	}
	if(pRoom2->Position.y + pRoom2->Size.y - pRoom1->Position.y >= 1 && pRoom1->Position.y >= pRoom2->Position.y) {
		return HorizontalConnect(pRoom1, pRoom2, pRoom1->Position.y);
	}
	if(pRoom1->Position.x + pRoom1->Size.x - pRoom2->Position.x >= 1 && pRoom2->Position.x >= pRoom1->Position.x) {
		return VerticalConnect(pRoom1, pRoom2, pRoom2->Position.x);
	}
	if(pRoom2->Position.x + pRoom2->Size.x - pRoom1->Position.x >= 1 && pRoom1->Position.x >= pRoom2->Position.x) {
		return  VerticalConnect(pRoom1, pRoom2, pRoom1->Position.x);
	}
	//create joint point
	std::vector<NRectangle2> Result;
	NVector2 JointCenter;
	if(Center.x >= pRoom1->Position.x && Center.x <= pRoom1->Position.x + pRoom1->Size.x) {
		JointCenter.x = (rand() % (pRoom1->Size.x - 1) + 1) + pRoom1->Position.x;//Center.x;
		JointCenter.y = (rand() % (pRoom2->Size.y - 1) + 1) + pRoom2->Position.y;
	}
	if(Center.y >= pRoom1->Position.y && Center.y <= pRoom1->Position.y + pRoom1->Size.y) {
		JointCenter.y = (rand() % (pRoom1->Size.y - 1) + 1) + pRoom1->Position.y;//Center.y;
		JointCenter.x = (rand() % (pRoom2->Size.x - 1) + 1) + pRoom2->Position.x;
	}
	if(Center.x >= pRoom2->Position.x && Center.x <= pRoom2->Position.x + pRoom2->Size.x) {
		JointCenter.x = (rand() % (pRoom2->Size.x - 1) + 1) + pRoom2->Position.x;//Center.x;
		JointCenter.y = (rand() % (pRoom1->Size.y - 1) + 1) + pRoom1->Position.y;
	}
	if(Center.y >= pRoom2->Position.y && Center.y <= pRoom2->Position.y + pRoom2->Size.y) {
		JointCenter.y = (rand() % (pRoom2->Size.y - 1) + 1) + pRoom2->Position.y;//Center.y;
		JointCenter.x = (rand() % (pRoom1->Size.x - 1) + 1) + pRoom1->Position.x;
	}
	//create two halls
	NRectangle2 Hall1 = CreateHallFromPoint(pRoom1, JointCenter);
	NRectangle2 Hall2 = CreateHallFromPoint(pRoom2, JointCenter);
	//result
	Result.push_back(Hall1);
	Result.push_back(Hall2);
	
	return Result;
}

std::vector<NRectangle2> ConnectRooms(NRectangle2* pRoom1, NRectangle2* pRoom2) {
	std::vector<NRectangle2> Result;
	std::vector<int> Points;
	std::vector<int>::iterator it;
	NVector2 Min, Max;
	NVector2 Center;
	bool CenterInRoom1[2] = {0,0};
	bool CenterInRoom2[2] = {0,0};
	//x
	Points.push_back(pRoom1->Position.x);
	Points.push_back(pRoom1->Position.x + pRoom1->Size.x);
	Points.push_back(pRoom2->Position.x);
	Points.push_back(pRoom2->Position.x + pRoom2->Size.x);
	Min.x = *min_element(Points.begin(), Points.end());
	Max.x = *max_element(Points.begin(), Points.end());
	Points.clear();
	//y
	Points.push_back(pRoom1->Position.y);
	Points.push_back(pRoom1->Position.y + pRoom1->Size.y);
	Points.push_back(pRoom2->Position.y);
	Points.push_back(pRoom2->Position.y + pRoom2->Size.y);
	Min.y = *min_element(Points.begin(), Points.end());
	Max.y = *max_element(Points.begin(), Points.end());
	Points.clear();
	//center
	Center.x = Min.x + (Max.x - Min.x) * 0.5;
	Center.y = Min.y + (Max.y - Min.y) * 0.5;
	//test
	if(Center.x >= pRoom1->Position.x && Center.x < pRoom1->Position.x + pRoom1->Size.x) CenterInRoom1[0] = true;
	if(Center.y >= pRoom1->Position.y && Center.y < pRoom1->Position.y + pRoom1->Size.y) CenterInRoom1[1] = true;
	if(Center.x >= pRoom2->Position.x && Center.x < pRoom2->Position.x + pRoom2->Size.x) CenterInRoom2[0] = true;
	if(Center.y >= pRoom2->Position.y && Center.y < pRoom2->Position.y + pRoom2->Size.y) CenterInRoom2[1] = true;
	if(CenterInRoom1[0] && CenterInRoom2[0]) {
		Result = VerticalConnect(pRoom1, pRoom2, Center.x);
	}
	else if(CenterInRoom1[1] && CenterInRoom2[1]) {
		Result = HorizontalConnect(pRoom1, pRoom2, Center.y);
	}
	else {
		Result = CompexConnect(pRoom1, pRoom2, Center);
	}
	
	return Result;
}

glm::vec2 GetRoomCenter(NRectangle2* pRoom, glm::vec2 TileSize) {
	if(!pRoom || TileSize.x <= 0 || TileSize.y <= 0) return glm::vec2(0, 0);
	return glm::vec2(TileSize.x * (pRoom->Position.x + pRoom->Size.x * 0.5), TileSize.y * (pRoom->Position.y + pRoom->Size.y * 0.5));
}

MLeaf::MLeaf(NVector2 inPosition, NVector2 inSize) {
	Position = inPosition;
	Size = inSize;
	LeafLeft = NULL;
	LeafRight = NULL;
	Room = NULL;
}

MLeaf::MLeaf()
{
	Position = NVector2(0, 0);
	Size = NVector2(0, 0);
	LeafLeft = NULL;
	LeafRight = NULL;
	Room = NULL;
}

bool MLeaf::Split() {
	if(LeafLeft || LeafRight) return false;
	bool SplitByHeight = rand() % 2;
	if(Size.x > Size.y && Size.x / Size.y >= 1.25) SplitByHeight = false;
	else if(Size.y > Size.x && Size.y / Size.x >= 1.25) SplitByHeight = true;
	int MaxLeafSize = (SplitByHeight ? Size.y : Size.x) - MIN_LEAF_SIZE_2;
	if(MaxLeafSize <= MIN_LEAF_SIZE_2) return false;
	int SplitSize = rand() % (MaxLeafSize - MIN_LEAF_SIZE_2) + MIN_LEAF_SIZE_2;
	if(SplitByHeight) {
		LeafLeft = new MLeaf(NVector2(Position.x, Position.y), NVector2(Size.x, SplitSize));
		LeafRight = new MLeaf(NVector2(Position.x, Position.y + SplitSize), NVector2(Size.x, Size.y - SplitSize));
	}
	else {
		LeafLeft = new MLeaf(NVector2(Position.x, Position.y), NVector2(SplitSize, Size.y));
		LeafRight = new MLeaf(NVector2(Position.x + SplitSize, Position.y), NVector2(Size.x - SplitSize, Size.y));
	}
	return true;
}

void MLeaf::CreateRooms() {
	if(LeafLeft || LeafRight) {
		if(LeafLeft) {
			LeafLeft->CreateRooms();
		}
		if(LeafRight) {
			LeafRight->CreateRooms();
		}
	}
	else {
		Room = new NRectangle2;
		Room->Size.x = rand() % (Size.x - (MIN_ROOM_SIZE_2 + 2)) + MIN_ROOM_SIZE_2;
		Room->Size.y = rand() % (Size.y - (MIN_ROOM_SIZE_2 + 2)) + MIN_ROOM_SIZE_2;
		Room->Position.x = Position.x + rand() % (Size.x - Room->Size.x - 2) + 1;
		Room->Position.y = Position.y + rand() % (Size.y - Room->Size.y - 2) + 1;
		//printf("Room(%i): posx: %i posy: %i sizex: %i sizey: %i\n", Room, Room->Position.x, Room->Position.y, Room->Size.x, Room->Size.y);
	}
}

NRectangle2* MLeaf::GetRoom() {
	if(Room) return Room;
	NRectangle2* RoomLeft = NULL;
	NRectangle2* RoomRight = NULL;
	if(LeafLeft) RoomLeft = LeafLeft->GetRoom();
	if(LeafRight) RoomRight = LeafRight->GetRoom();
	if(RoomLeft && RoomRight) { //bad realisation - random
		if(rand() % 2) {
			return RoomLeft;
		}
		else {
			return RoomRight;
		}
	}
	else {
		if(RoomLeft) return RoomLeft;
		else return RoomRight;
	}
	return NULL;
}

