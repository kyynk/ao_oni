#pragma once
#include "MapData.h"

#include "entity.h"
#include "Human.h"
#include "Oni.h"

namespace game_framework {
	
	class GameMap {
	public:
		
		GameMap(){
			indexlayer = 0;
			isshowtileindex = false;
		}
		~GameMap(){}
		void ShowMap(int layer);
		void ShowMapAll(Human &,Oni &, int overlayindex);
		void SetTopLeftMap(int x, int y);
		void debug();
		void Load(string files);
		void ShowTileIndexLayer();
		string GetName() { return _mapname; }
		int GetX() { return _pos_x; }
		int GetY() { return _pos_y; }
		int GetWidth() { return _width; }
		int GetHeight() { return _height; }
		int GetLayer() { return _layer; }
		int GetMapData(int layer, int bx, int by);
		bool isshowtileindex;
		int indexlayer;
	private:
		int _layer, _width, _height;
		int _pos_x, _pos_y;
		int selTileset(int n);
		map <int,string> _resource_list;
		MapData _gamemapdata;
		string _mapname;
		int _ID;
		int _overlayindex;
	};

}