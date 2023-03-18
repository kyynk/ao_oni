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
#include "Cmap.h"

namespace game_framework {
	Cmap::Cmap(){
	}
	void Cmap::Init(int w, int h, int px, int py){
		_width = w;
		_height = h;
		_pos_x = px;
		_pos_y = py;
	}
	void Cmap::ShowMap(int layer, vector<CMovingBitmap> a)	{
		for (int i = 0; i < _width; i++) {
			for (int j = 0; j < _height; j++) {
				a.at(_map.at(i).at(j).at(layer)).ShowBitmap();
			}
		}
	}

	void Cmap::Load(string files, COLORREF color)
	{
		std::ifstream in("map_bmp" + files);
		in >> _width >> _height;
		int n;
		in >> n;
		for (int i = 0; i < n; i++) {
			string str;
			int f_g;
			in >> f_g >> str;
			_resource_list.insert({str,f_g});
		}

		in >> n ;
		//_map = vector<vector<vector<int>>>(n, vector<vector<int>>(_height, vector<int>(_width)));
		for (int k = 0; k < n; k++) {
			vector<vector<int >> a;
			_map.push_back(a);
			for (int i = 0; i < _height; i++) {
				vector<int> b;
				_map.at(k).push_back(b);
				for (int j = 0; j < _width; j++) {
					in >> _map[k][i][j];
				}
			}
		}

	}
}