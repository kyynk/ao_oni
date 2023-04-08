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
#include "MapData.h"
#include "MapRes.h"
#include "GameMap.h"

namespace game_framework {

	
	void GameMap::ShowMap(int layer)	{
		if (layer >= _layer) {
			throw std::invalid_argument("layer out of range");
		}
		for (int i = 0; i < _height; i++) {
			for (int j = 0; j < _width; j++) {
				int val = _gamemapdata.GetValue(layer , i, j);
				if (val == 0)continue;
				int tmp = selTileset(val);
				//TRACE("i: %d, j:%d ,,, val:%d tmp:%d  %s\n", i,j,val, tmp,_resource_list[tmp].c_str());

				MapRes::GetInstance()->GetData()[_resource_list[tmp]][val - tmp].SetTopLeft(_pos_x+j * TILE,_pos_y+i*TILE);
				MapRes::GetInstance()->GetData()[_resource_list[tmp]][val - tmp].ShowBitmap();
			}
		}
		
	}
	void GameMap::ShowMapAll(Human &humanobj,Oni &oniobj,int overlayindex) {
		int humany = (humanobj.GetY()-_pos_y)/TILE;
		int oniy = (oniobj.GetPosY()- _pos_y)/TILE;
		int humanya = humanobj.GetY()& TILE;
		int oniya = oniobj.GetPosY()&TILE;

		//TRACE("%d %d %d %d \n",humany,oniy, humanya, oniya);
		if (_ID == 0) {
			bool f1 = true;
			bool f2 = true;
			for (int i = 1;i < _layer;i++) {
				ShowMap(i);

				if (i == 2&&(f1 ||f2)) {
					if (humany < 8 && oniy < 8) {
						if (humany < oniy) {
							humanobj.OnShow();
							oniobj.OnShow();
						}
						else {
							oniobj.OnShow();
							humanobj.OnShow();
						}
						f1 = false;
						f2 = false;
					}
					else if (humany < 8) {
						humanobj.OnShow();
						f1 = false;
					}
					else if (oniy < 8) {
						oniobj.OnShow();
						f2 = false;
					}
				}
				else if (i == 3 && (f1 || f2)) {
					if (humany < 13 && oniy < 13) {
						if (humany < oniy) {
							humanobj.OnShow();
							oniobj.OnShow();
						}
						else {
							oniobj.OnShow();
							humanobj.OnShow();
						}
						f1 = false;
						f2 = false;
					}
					else if (humany < 13) {
						humanobj.OnShow();
						f1 = false;

					}
					else if (oniy < 13) {
						oniobj.OnShow();
						f2 = false;

					}
				}
				else  if(i==4&&(f1 || f2)){
					if (humany < oniy) {
						humanobj.OnShow();
						oniobj.OnShow();
					}
					else {
						oniobj.OnShow();
						humanobj.OnShow();
					}
					f1 = false;
					f2 = false;
				}
			}
		}
		else if (_ID == 12) {
			//TRACE("ID == %d", _ID);
			bool f1 = true;
			bool f2 = true;
			for (int i = 1;i < _layer;i++) {
				ShowMap(i);

				if (i == 4 && (f1 || f2)) {
					if (humany < 4 && oniy < 4) {
						if (humany < oniy) {
							humanobj.OnShow();
							oniobj.OnShow();
						}
						else {
							oniobj.OnShow();
							humanobj.OnShow();
						}
						f1 = false;
						f2 = false;
					}
					else if (humany < 4) {
						humanobj.OnShow();
						f1 = false;
					}
					else if (oniy < 4) {
						oniobj.OnShow();
						f2 = false;
					}
				}
				
				else if (i == 5 && (f1 || f2)) {
					if (humany < 8 && oniy < 8) {
						if (humany < oniy) {
							humanobj.OnShow();
							oniobj.OnShow();
						}
						else {
							oniobj.OnShow();
							humanobj.OnShow();
						}
						f1 = false;
						f2 = false;
					}
					else if (humany < 8) {
						humanobj.OnShow();
						f1 = false;

					}
					else if (oniy < 8) {
						oniobj.OnShow();
						f2 = false;

					}
				}
				else if (i == 6 && (f1 || f2)) {
					if (humany < 12 && oniy < 12) {
						if (humany < oniy) {
							humanobj.OnShow();
							oniobj.OnShow();
						}
						else {
							oniobj.OnShow();
							humanobj.OnShow();
						}
						f1 = false;
						f2 = false;
					}
					else if (humany < 12) {
						humanobj.OnShow();
						f1 = false;

					}
					else if (oniy < 12) {
						oniobj.OnShow();
						f2 = false;

					}
				}
				else  if (i == 7 && (f1 || f2)) {
					if (humany < 15 && oniy < 15) {
						if (humany < oniy) {
							humanobj.OnShow();
							oniobj.OnShow();
						}
						else {
							oniobj.OnShow();
							humanobj.OnShow();
						}
						f1 = false;
						f2 = false;
					}
					else if (humany < 15) {
						humanobj.OnShow();
						f1 = false;

					}
					else if (oniy < 15) {
						oniobj.OnShow();
						f2 = false;
					}
				}
				else  if (i == 8 && (f1 || f2)) {
					if (humany < oniy) {
						humanobj.OnShow();
						oniobj.OnShow();
					}
					else {
						oniobj.OnShow();
						humanobj.OnShow();
					}
					f1 = false;
					f2 = false;
				}
			}
		}
		else {
			for (int i = 1; i < _layer; i++) {
				ShowMap(i);
				if (i == overlayindex) {
					if (oniobj.GetPosD() > humanobj.GetD()) {

						oniobj.OnShow();
						humanobj.OnShow();
					}
					else {
						humanobj.OnShow();
						oniobj.OnShow();

					}
				}
			}
		}
		if (isshowtileindex) {
			CDC *cdc = CDDraw::GetBackCDC();
			CTextDraw::ChangeFontLog(cdc, 10, "Noto Sans TC", RGB(255, 255, 255));
			for (int i = 0; i < _height; i++) {
				for (int j = 0; j < _width; j++) {
					int val = _gamemapdata.GetValue(indexlayer, i, j);
					CTextDraw::Print(cdc, _pos_x + j * TILE, _pos_y + i * TILE, to_string(val));
				}
			}
			CDDraw::ReleaseBackCDC();
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
					TRACE("%d ", _gamemapdata.GetValue(i,j,k));
				}
				TRACE("\n");
			}
			TRACE("\n");
		}

	}
	void GameMap::Load(string files)
	{
		std::ifstream in(files);
		in >> _mapname >> _ID;
		//TRACE("%s\n", _mapname.c_str());
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
		_gamemapdata.SetDimension(_layer,_height,_width);
		for (int i = 0; i < _layer; i++) {
			for (int j = 0; j < _height; j++) {
				for (int k = 0; k < _width; k++) {
					int tmp;
					in >> tmp;
					_gamemapdata.AssignValue(i,j,k,tmp);
				}
			}
		}
	}
	void GameMap::ShowTileIndexLayer()
	{
		if (isshowtileindex) {
			CDC *cdc = CDDraw::GetBackCDC();
			CTextDraw::ChangeFontLog(cdc, 20, "Noto Sans TC", RGB(255, 0, 0), 700);
			CTextDraw::Print(cdc, 24 * TILE, 3 * TILE, to_string(indexlayer));
			CDDraw::ReleaseBackCDC();
		}
	}
	int GameMap::GetMapData(int layer, int bx, int by)
	{
		
		if (layer < 0 || layer >= _layer || bx < 0 || bx >= _width || by < 0 || by >= _height) {

			// TRACE("%d %d %d out of range u fucking dunky\n", layer, bx, by);
			// TRACE("this map %d %d %d\n", _layer, _width, _height);
			return -87;
		}
		else {
			return _gamemapdata.GetValue(layer, by, bx);
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