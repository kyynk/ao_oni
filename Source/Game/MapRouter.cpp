#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "config.h"
#include <fstream>
#include <string>
#include <vector>
#include "GameMap.h"
#include "MapNode.h"
#include "MapRouter.h"
#define HOUSE1_BASEMENT1 0
#define HOUSE1_BASEMENT2 1
#define HOUSE1_BASEMENT3 2
#define HOUSE1_BASEMENT4 3
#define HOUSE1_BASEMENT5 4
#define HOUSE1_HALLWAY1 5
#define HOUSE1_HALLWAY2 6
#define HOUSE1_HALLWAY3 7
#define HOUSE1_HALLWAY4 8
#define HOUSE1_HALLWAY5 9
#define HOUSE1_HALLWAY6 10
#define HOUSE1_KITCHEN 11
#define HOUSE1_LIBRARY 12
#define HOUSE1_LOBBY 13
#define HOUSE1_ROOM1 14
#define HOUSE1_ROOM2 15
#define HOUSE1_ROOM3 16
#define HOUSE1_ROOM4 17
#define HOUSE1_ROOM5 18
#define HOUSE1_ROOM6 19
#define HOUSE1_ROOM7 20
#define HOUSE1_ROOM8 21
#define HOUSE1_ROOM9 22

namespace game_framework {
	void MapRouter::Load(string filename)
	{
		int mapID1, mapID2;
		int x1, y1,x2,y2;
		int n;
		fstream in(filename);
		in >> n;
		for (int i = 0; i < n; i++) {
			in >> mapID1 >> mapID2 >> x1 >> y1 >> x2 >> y2;
			MapNode node1(x1,y1,x2,y2,mapID2);
			MapNode node2(x2,y2,x1,y1,mapID1);
			_data[mapID1].push_back(node1);
			_data[mapID2].push_back(node2);
		}
		
	}
	void MapRouter::Cleanup()
	{
		if (_Instance) {
			delete _Instance;
			_Instance = nullptr;
		}
	}

	MapRouter* MapRouter::_Instance = nullptr;
}

