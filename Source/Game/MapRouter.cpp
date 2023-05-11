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
		SetAllPathTrue();
	}
	void MapRouter::copypath() {
		for (int i = 0;i < 65;i++) {
			for (int j = 0;j < 6;j++) {
				_pathblockedcopy[i][j] = _pathblocked[i][j];
			}
		}
	}
	void MapRouter::Restore() {

		for (int i = 0;i < 65;i++) {
			for (int j = 0;j < 6;j++) {
				_pathblocked[i][j] = _pathblockedcopy[i][j];
			}
		}
	}
	void MapRouter::SetAllPathFalse() {

		for (int i = 0;i < 65;i++) {
			for (int j = 0;j < 6;j++) {
				_pathblocked[i][j] = false;
			}
		}
	}
	void MapRouter::SetAllPathTrue() {

		for (int i = 0;i < 65;i++) {
			for (int j = 0;j < 6;j++) {
				_pathblocked[i][j] = true;
			}
		}
	}
	void MapRouter::Load(string filename)
	{
		int mapID1, mapID2;
		int x1, y1, x2, y2;
		//int blocksensitive;
		int twoway;
		int n;
		fstream in(filename);
		in >> n;
		bool nadded = true ;
		for (int i = 0; i < n; i++) {
			in >> twoway >> mapID1 >> x1 >> y1 >> mapID2 >> x2 >> y2;
			NodeData tmp1(x1, y1, x2, y2);
			for (int j = 0; j < 6 ; j++) {
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
				for (int j = 0; j < 6; j++) {
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
	void MapRouter::debugF() {
		for (int i = 0;i < 65;i++) {
			TRACE("%d \n", i);
			for (int j = 0; j < _record[i];j++) {
				if (_pathblocked[i][j])TRACE("num %d ok\n",_data[i][j].GetID());
				else TRACE("num %d FuckYou\n",_data[i][j].GetID());

			}
		}
	}
	void MapRouter::debug()
	{
		for (int i = 0; i < 65; i++) {
			for (int j = 0; j < _record[i];j++) {
				TRACE("map1 ID : %d\n", i);
				_data[i][j].debug();
			}
		}
	}
	void MapRouter::BlockPath(int x, int y)
	{
		for (int i = 0;i < _record[x];i++) {
			if (_data[x][i].GetID() == y) {
				_pathblocked[x][i] = true;
				break;
			}
		}
	}
	void MapRouter::UnblockPath(int x, int y)
	{
		for (int i = 0;i < _record[x];i++) {
			if (_data[x][i].GetID() == y) {
				_pathblocked[x][i] = false;
				break;
			}
		}
		for (int i = 0;i < _record[y];i++) {
			if (_data[y][i].GetID() == x) {
				_pathblocked[y][i] = false;
				break;
			}
		}
	}
	bool MapRouter::IsPathBlocked(int x, int y)
	{
		for (int i = 0;i < _record[x];i++) {
			if (_data[x][i].GetID() == y) {
				return _pathblocked[x][i];
			}
		}
		return true;
	}

}

