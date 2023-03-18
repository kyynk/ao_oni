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
			bmps.at(j).LoadBitmapByString({ "map_bmp/" + name + "/image" + to_string(j) + ".bmp" },RGB(0,0,0));
		}
		_data.insert({name,bmps});
	}
	vector<CMovingBitmap> & MapRes::operator[](const string name)
	{
		return _data[name];
	}
	MapRes::~MapRes()
	{
		_data.clear();
	}
	void MapRes::checkres() {
		for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 8; j++) {
				_data["003-Forest01"][i*8+j].SetTopLeft(j*TILE,i*TILE);
				_data["003-Forest01"][i*8+j].ShowBitmap();
				

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
