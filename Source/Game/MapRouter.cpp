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

namespace game_framework {

	void MapRouter::init()
	{
		memset(_record, 0, sizeof(_record));
		
	}
	void MapRouter::Load(string filename)
	{
		int mapID1, mapID2;
		int x1, y1, x2, y2;
		int blocksensitive;
		int twoway;
		int n;
		fstream in(filename);
		in >> n;
		bool nadded = true ;
		for (int i = 0; i < n; i++) {
			in >> twoway >>blocksensitive>> mapID1 >> x1 >> y1 >> mapID2 >> x2 >> y2;
			NodeData tmp1(x1, y1, x2, y2);
			for (int j = 0; j < 5 ; j++) {
				if (_data[mapID1][j].GetID() == mapID2) {
					_data[mapID1][j].AddEdge(move(tmp1));
					nadded = false;
					break;
				}
			}
			if (nadded) {
				MapNode newnode(move(tmp1), mapID2);
				_data[mapID1][_record[mapID1]] = newnode;
				_record[mapID1] += 1;
			}
			nadded = true;
			if (twoway == 1) {
				NodeData tmp2(x2, y2, x1, y1);
				for (int j = 0; j < 5; j++) {
					if (_data[mapID2][j].GetID() == mapID1) {
						_data[mapID2][j].AddEdge(move(tmp2));
						nadded = false;
						break;
					}
				}
				if (nadded) {
					MapNode newnode(move(tmp2), mapID1);
					_data[mapID2][_record[mapID2]] = newnode;
					_record[mapID2] += 1;
				}
				nadded = true;
			}
		}
	}

	void MapRouter::debug()
	{
		for (int i = 0; i < 46; i++) {
			for (int j = 0; j < _record[i];j++) {
				TRACE("map1 ID : %d\n", i);
				_data[i][j].debug();
			}
		}
	}


}

