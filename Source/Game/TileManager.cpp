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
#include "TileManager.h""


void game_framework::TileManager::Load()
{
	std::ifstream in("map_bmp/mapsize.txt");
	string name;
	int count;
	vector<string> tmp;
	int total = 0;
	for(int i=0 ;i<12;i++){
		in >> name >> count;
		tmp.push_back(name);
		total += count;
	}
	_tiles = vector<CMovingBitmap>(total);
	int k = 0;
	for (int i = 0; i < 12;i++) {
		for (auto f : tmp) {
			_tiles.at(k).LoadBitmapByString(f);
			k++;

		}
	}
}
