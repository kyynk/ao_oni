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
#include "MVector.h"
#include "MapRes.h"
#include "GameMap.h"

namespace game_framework {

	
	void GameMap::ShowMap(int layer)	{
		if (layer-1 >= _layer) {
			throw std::invalid_argument("layer out of range");
		}
		for (int i = 0; i < _height; i++) {
			for (int j = 0; j < _width; j++) {
				int val = _gamemap.GetValue(layer-1, i, j);
				if (val == 0)continue;
				int tmp = selTileset(val);
				TRACE("i: %d, j:%d ,,, val:%d tmp:%d  %s\n", i,j,val, tmp,_resource_list[tmp].c_str());

				MapRes::GetInstance()->GetData()[_resource_list[tmp]][val - tmp].SetTopLeft(_pos_x+j * TILE,_pos_y+i*TILE);
				MapRes::GetInstance()->GetData()[_resource_list[tmp]][val - tmp].ShowBitmap();
			}
		}
	}
	void GameMap::ShowMap() {
		for (int i = 1; i < _layer+1; i++) {
			ShowMap(i);
		}
	}
	void GameMap::SetTopLeftMap(int x,int y)
	{
		_pos_x = x;
		_pos_y = y;
	}
	void GameMap::debug() {
		TRACE("%d%d%d", _layer,_height,_width);
		for (int i = 0; i < _layer; i++) {
			for (int j = 0; j < _height; j++) {
				for (int k = 0; k < _width; k++) {
					TRACE("%d ", _gamemap.GetValue(i,j,k));
				}
				TRACE("\n");
			}
			TRACE("\n");
		}

	}
	void GameMap::Load(string files)
	{
		std::ifstream in("map_bmp/" + files);
		in >> _mapname;
		TRACE("%s\n", _mapname.c_str());
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
		_gamemap.SetDimension(_layer,_height,_width);
		for (int i = 0; i < _layer; i++) {
			for (int j = 0; j < _height; j++) {
				for (int k = 0; k < _width; k++) {
					int tmp;
					in >> tmp;
					_gamemap.AssignValue(i,j,k,tmp);
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