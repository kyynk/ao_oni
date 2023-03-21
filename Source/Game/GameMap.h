#pragma once
#include "vector3d.h"
namespace game_framework {
	
	class GameMap {
	public:
		GameMap(){}
		~GameMap(){}
		void Init(int px, int py);
		void ShowMap(int layer);
		void ShowMap();
		void debug();
		void Load(string files);
		string GetName() { return _mapname; }
	private:
		int _layer, _width, _height;
		int _pos_x, _pos_y;
		int selTileset(int n);
		map <int,string> _resource_list;
		vector3d _gamemap;
		string _mapname;
	};

}