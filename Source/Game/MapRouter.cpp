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
		_ban_list.push_back(12);
		_ban_list.push_back(28);

	}
	void MapRouter::Load(string filename)
	{
		int mapID1, mapID2;
		int x1, y1,x2,y2;
		int n;
		fstream in(filename);
		in >> n;
		bool nadded = true ;
		for (int i = 0; i < n; i++) {
			in >> mapID1 >> x1 >> y1 >> mapID2 >> x2 >> y2;
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
				_data[mapID1][record[mapID1]] = newnode;
				record[mapID1] += 1;
			}
			nadded = true;
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
				_data[mapID2][record[mapID2]] = newnode;
				record[mapID2] += 1;
			}
			nadded = true;
		}
	}
	void MapRouter::Cleanup()
	{
		if (_Instance) {
			delete _Instance;
			_Instance = nullptr;
		}
	}

	void MapRouter::debug()
	{
		for (int i = 0; i < 23; i++) {
			for (int j = 0; j < record[i];j++) {

				
				TRACE("%d\n", i);
				_data[i][j].debug();
			}
		}
	}

	bool MapRouter::IsInBanlist(int bx, int by)
	{
		for (int i = 0; i < _gamemaps.at(_nowID).GetLayer(); i++) {
			for (int j = 0; j < 2;j++ ) {
				if (_gamemaps.at(_nowID).GetMapData(i, bx-_gamemaps.at(_nowID).GetX()/TILE, by - _gamemaps.at(_nowID).GetY()/TILE) == _ban_list[j]) {
					return true;
				}
			}
		}
		return false;
	}

	

	void MapRouter::ShowIndexLayer()
	{
		_gamemaps.at(_nowID).ShowTileIndexLayer();
	}
	MapRouter* MapRouter::_Instance = nullptr;

}

