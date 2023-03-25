#pragma once
#include "MVector.h"
namespace game_framework {
	
	class GameMap {
	public:
		GameMap(){}
		~GameMap(){}
		void ShowMap(int layer);
		void ShowMap();
		void SetTopLeftMap(int x, int y);
		void debug();
		void Load(string files);
		string GetName() { return _mapname; }
		int GetX() { return _pos_x; }
		int GetY() { return _pos_y; }
		int GetWidth() { return _width; }
		int GetHeight() { return _height; }
		int GetLayer() { return _layer; }
		bool istileindex;
		
	private:
		int _layer, _width, _height;
		int _pos_x, _pos_y;
		int selTileset(int n);
		map <int,string> _resource_list;
		MVector _gamemap;
		string _mapname;
		int _ID;
	};

}