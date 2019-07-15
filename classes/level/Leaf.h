#ifndef leafH
#define leafH

#include <math.h> //floor()
#include <stdio.h> //printf()
#include <stdlib.h> //rand()
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>

#define MIN_LEAF_SIZE_2 6
#define MAX_LEAF_SIZE_2 20
#define MIN_ROOM_SIZE_2 3

struct NVector2 {
	int x;
	int y;
	NVector2() {
		x = y = 0;
	}
	NVector2(int inX, int inY) {
		x = inX;
		y = inY;
	}
};

struct NLine2 {
	NVector2 a;
	NVector2 b;
	NLine2() {
		a = NVector2(0, 0);
		b = NVector2(0, 0);
	}
	NLine2(NVector2 inA, NVector2 inB) {
		a = inA;
		b = inB;
	}
};

struct NRectangle2 {
	NVector2 Position;
	NVector2 Size;
	NRectangle2() {
		Position = NVector2(0, 0);
		Size = NVector2(0, 0);
	}
	NRectangle2(NVector2 inPosition, NVector2 inSize) {
		Position = inPosition;
		Size = inSize;
	}
};

std::vector<NRectangle2> VerticalConnect(NRectangle2* pRoom1, NRectangle2* pRoom2, int CenterX);
std::vector<NRectangle2> HorizontalConnect(NRectangle2* pRoom1, NRectangle2* pRoom2, int CenterY);
NRectangle2 CreateHallFromPoint(NRectangle2* pRoom, NVector2 Center);
std::vector<NRectangle2> CompexConnect(NRectangle2* pRoom1, NRectangle2* pRoom2, NVector2 Center);
std::vector<NRectangle2> ConnectRooms(NRectangle2* pRoom1, NRectangle2* pRoom2);
glm::vec2 GetRoomCenter(NRectangle2* pRoom, glm::vec2 TileSize);

class MLeaf {
public:
	NVector2 Position;
	NVector2 Size;
	MLeaf* LeafLeft;
	MLeaf* LeafRight;
	NRectangle2* Room;
	MLeaf();
	MLeaf(NVector2 inPostition, NVector2 inSize);
	bool Split();
	void CreateRooms();
	NRectangle2* GetRoom();
};

#endif
