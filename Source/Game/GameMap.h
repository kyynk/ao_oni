#pragma once
#include "vector3d.h"
namespace game_framework {
	
	class GameMap {
	public:
		GameMap(){}
		void Init(int w,int h,int px,int py);
		void ShowMap(int layer);
		void ShowMap();
		void debug();
		void Load(string files, COLORREF color);
	private:
		int _layer, _width, _height;
		int _pos_x, _pos_y;
		int selTileset(int n);
		map <int,string> _resource_list;
		vector3d _gamemap;
	};

}