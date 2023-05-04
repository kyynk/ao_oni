#pragma once
#include "MapData.h"
#include "Entity.h"
#include "Human.h"
#include "MainHuman.h"
#include "Oni.h"
#include "Item.h"
#include "ObjMove.h"

namespace game_framework {
	
	class GameMap {
	public:
		
		GameMap(){
			indexlayer = 0;
			isshowtileindex = false;
		}
		~GameMap(){}
		void ShowMap(int layer);
		void ShowMapAll(MainHuman& humanobj, int overlayindex);
		void ShowMapAll(MainHuman &,Oni &, int overlayindex);
		void ShowMapAll();
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
		void SetMapData(int layer ,int x, int y, int value);
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