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
#include "MapRes.h"
namespace game_framework {

	void MapRes::Load(const string name, const int count)
	{
		vector<CMovingBitmap> bmps;
		bmps.resize(count);
		for (int j = 0; j < count; j++) {
			bmps.at(j).LoadBitmapByString({ "map_bmp/" + name + "/image" + to_string(j) + ".bmp" },RGB(204,255,0));
		}
		_data.insert({name,bmps});
	}
	
	MapRes::~MapRes()
	{
		_data.clear();
	}
	void MapRes::check() {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 8; j++) {
				_data["room-01-udy"][i*8+j].SetTopLeft(j*TILE,i*TILE);
				_data["room-01-udy"][i*8+j].ShowBitmap();
			}
		}
	}
	void MapRes::Cleanup()
	{
		if (_Instance) {
			delete _Instance;
			_Instance = nullptr;
		}
	}
	MapRes* MapRes::_Instance = nullptr;
}
