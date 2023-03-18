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
#include "vector3d.h"
#include "GameMap.h"

namespace game_framework {

	void GameMap::Init(int w, int h, int px, int py){
		_width = w;
		_height = h;
		_pos_x = px;
		_pos_y = py;
	}
	void GameMap::ShowMap(int layer, map<string, vector<CMovingBitmap> >& resource)	{
		if (layer-1 >= _layer) {
			throw std::invalid_argument("layer out of range");
		}
		/*for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 8; j++) {
				resource["003-Forest01"][i * 8 + j].SetTopLeft(j*TILE, i*TILE);
				resource["003-Forest01"][i * 8 + j].ShowBitmap();


			}
		}*/
		for (int i = 0; i < _height; i++) {
			for (int j = 0; j < _width; j++) {
		TRACE("noo");
				int val = _gmap.GetValue(layer-1, i, j);
				int tmp = selTileset(val);
				TRACE("val:%d tmp:%d \n", val, tmp);
				resource[_resource_list[tmp]][val - tmp].SetTopLeft(j * TILE,i*TILE);
				resource[_resource_list[tmp]][val - tmp].ShowBitmap();
			}
		}
	}
	void GameMap::debug() {
		TRACE("%d%d%d", _layer,_height,_width);
		for (int i = 0; i < _layer; i++) {
			for (int j = 0; j < _height; j++) {
				for (int k = 0; k < _width; k++) {
					TRACE("%d ", _gmap.GetValue(i,j,k));
				}
				TRACE("\n");
			}
			TRACE("\n");
		}
	}
	void GameMap::Load(string files, COLORREF color)
	{
		std::ifstream in("map_bmp/" + files);
		in >> _width >> _height;
		int n;
		in >> n;
		for (int i = 0; i < n; i++) {
			string str;
			int first_grid_ID;
			in >> first_grid_ID >> str;
			_resource_list.insert({first_grid_ID,str });
		}
		in >> _layer;
		_gmap.SetDimension(_layer,_height,_width);
		for (int i = 0; i < _layer; i++) {
			for (int j = 0; j < _height; j++) {
				for (int k = 0; k < _width; k++) {
					int tmp;
					in >> tmp;
					_gmap.AssignValue(i,j,k,tmp);
				}
			}
		}
	}
	int GameMap::selTileset(int n)
	{
		int pre = 0;
		int tmp = 0;
		for (const auto& res : _resource_list) {
			if (n >= res.first || tmp == 0) {
				pre = res.first;
				tmp = pre;
			}
		}
		return pre;
		
	}
}