#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "config.h"
#include "GameMap.h"
#include "MapRouter.h"
#include "Entity.h"
#include "Human.h"
#include "MainHuman.h"
#include "InterfaceData.h"
#include "Interface.h"
namespace game_framework{

	MainHuman::MainHuman() :Human() {
		_nowmove = none;
		_pressing = none;
	}
	
	void MainHuman::init(int step,int offset,Direction dir) {
		_step = step;
		_offsetY = offset;
		_direction = dir;
		_machine_count = 0;
		_walkiter = true;
		_bstate = s1;
		_isup = false;
		_isdown = false;
		_isleft = false;
		_isright = false;
		_isMapChanged = false;
		_machine_done = true;
		_nextmapx = 0;
		_nextmapy = 0;
		_nextMapID = 0;
		_on_chair = false;
		_door_lock = false;
		_door_open = false;
		_piano_open = false;
	}
	void MainHuman::ResetToGrid() {
		if (this->GetX() % TILE != 0 || this->GetY() % TILE != 0) {
			int dx = 0;
			int dy = 1;
			if (this->GetX() % TILE >= TILE / 2) {
				dx = 1;
			}
			if (this->GetY() % TILE < TILE / 2) {
				dy = 0;
			}
			SetPos((_pos_x / TILE + dx) * TILE, ((_pos_y + _offsetY) / TILE + dy)* TILE);
			bitmap.SetTopLeft(_pos_x, _pos_y);
		}
	}
	void MainHuman::SetNextMapPos(GameMap& map) {
		if (_nowmove == up) {
			SetPos(map.GetX() + _nextmapx, map.GetY() + _nextmapy - TILE);
		}
		else if (_nowmove == down) {
			SetPos(map.GetX() + _nextmapx, map.GetY() + _nextmapy + TILE);
		}
		else if (_nowmove == left) {
			SetPos(map.GetX() + _nextmapx - TILE, map.GetY() + _nextmapy);
		}
		else if (_nowmove == right) {
			SetPos(map.GetX() + _nextmapx + TILE, map.GetY() + _nextmapy);
		}
		else if (_nowmove == machinetransmap) {
			SetPos(map.GetX() + _nextmapx, map.GetY() + _nextmapy);
		}
		bitmap.SetTopLeft(_pos_x, _pos_y);
		_isMapChanged = false;
	}
	void MainHuman::OnMove(GameMap &map, MapRouter &router, const int nowID, const vector<vector<int>>&VL,const vector<vector<int>>&VR) {
		
		bool upmovable = true;
		bool downmovable = true;
		bool leftmovable = true;
		bool rightmovable = true;
		if ((map.GetMapData(0, (this->GetX() - map.GetX()) / TILE, (this->GetU() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetX() - map.GetX()) / TILE, (this->GetU() - map.GetY()) / TILE) == -87) &&
			(this->GetX() - map.GetX()) % TILE == 0 &&
			(this->GetU() - map.GetY()) % TILE == 0) {
			upmovable = false;
		}
		if ((map.GetMapData(0, (this->GetX() - map.GetX()) / TILE, (this->GetD() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetX() - map.GetX()) / TILE, (this->GetD() - map.GetY()) / TILE) == -87) &&
			(this->GetX() - map.GetX()) % TILE == 0 &&
			(this->GetD() - map.GetY()) % TILE == 0) {
			downmovable = false;
		}
		for (auto &f : VL) {
			if ((this->GetX() - map.GetX()) / TILE == f[0] &&
				(this->GetX() - map.GetX()) % TILE == 0 &&
				(this->GetY() - map.GetY()) / TILE == f[1] &&
				(this->GetY() - map.GetY()) % TILE == 0 &&
				nowID == f[2]) {
				leftmovable = false;
				break;
			}
		}
		if (leftmovable && 
			(this->GetL() - map.GetX()) % TILE == 0&&
			(this->GetY() - map.GetY()) % TILE == 0&&
			(map.GetMapData(0, (this->GetL() - map.GetX()) / TILE, (this->GetY() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetL() - map.GetX()) / TILE, (this->GetY() - map.GetY()) / TILE) == -87))
			{
			leftmovable = false;
		}

		for (auto &f : VR) {
			if ((this->GetX() - map.GetX()) / TILE == f[0] &&
				(this->GetX() - map.GetX()) % TILE == 0 &&
				(this->GetY() - map.GetY()) / TILE == f[1] &&
				(this->GetY() - map.GetY()) % TILE == 0 &&
				nowID == f[2]) {
				rightmovable = false;
				break;
			}
		}
		if (rightmovable&&
			(this->GetR() - map.GetX()) % TILE == 0&&
			(this->GetY() - map.GetY()) % TILE == 0&&
			(map.GetMapData(0, (this->GetR() - map.GetX()) / TILE, (this->GetY() - map.GetY()) / TILE) == 0 ||
			map.GetMapData(0, (this->GetR() - map.GetX()) / TILE, (this->GetY() - map.GetY()) / TILE) == -87))
				{
			rightmovable = false;
		}
		if (_isup || _isdown || _isleft || _isright) {
			TimerStart();
		}
		else {
			if (TimerGetCount() == 8) {
				TimerStop();
				ResetToGrid();
				_walkiter = !_walkiter;
			}
		}
		if (IsTimerStart()) {
			if (TimerGetCount() % 8 == 0) {
				_nowmove = _pressing;
				ResetToGrid();

			}
			if (TimerGetCount() == 8) {
				TimerReset();
				_walkiter = !_walkiter;
			}
			if (TimerGetCount() < 4) {
				_bstate = s1;
			}
			else {
				_bstate = s2;
			}
			if (_nowmove == up && upmovable) {
				_pos_y -= _step;
			}
			else if (_nowmove == down && downmovable) {
				_pos_y += _step;
			}
			else if (_nowmove == left && leftmovable) {
				_pos_x -= _step;
			}
			else if (_nowmove == right && rightmovable) {
				_pos_x += _step;
			}
			TimerUpdate();
		}
		bitmap.SetTopLeft(_pos_x, _pos_y);
	}
	void MainHuman::RouterCheckChangeMap(GameMap& map, MapRouter& router, int nowID,Interface& gif) {
		for (int i = 0; i < router.GetRecord(nowID); i++) {
			
			for (int j = 0; j < router.GetNowMapPortal(nowID)[i].GetSize(); j++) {
				if (
				(router.GetNowMapPortal(nowID)[i].GetPointByIndex(j) ==
					NodeData(this->GetL() - map.GetX(), this->GetY() - map.GetY()) &&
					_direction == left &&
					_nowmove == left) ||
				(router.GetNowMapPortal(nowID)[i].GetPointByIndex(j) ==
					NodeData(this->GetR() - map.GetX(), this->GetY() - map.GetY()) &&
					_direction == right &&
					_nowmove == right) ||
				(router.GetNowMapPortal(nowID)[i].GetPointByIndex(j) ==
					NodeData(this->GetX() - map.GetX(), this->GetU() - map.GetY()) &&
					_direction == up &&
					_nowmove == up) ||
				(router.GetNowMapPortal(nowID)[i].GetPointByIndex(j) ==
					NodeData(this->GetX() - map.GetX(), this->GetD() - map.GetY()) &&
					_direction == down &&
					_nowmove == down) 
				){
					if (router.IsPathBlocked(nowID, router.GetNowMapPortal(nowID)[i].GetID())) {
						if (nowID == 5 && gif.FindItem("library key")) {
							router.UnblockPath(5, 12);
							gif.DeleteItem("library key");
							_door_open = true;
						}
						else if (nowID == 8 && gif.FindItem("bedroom key")) {
							router.UnblockPath(8, 16);
							gif.DeleteItem("bedroom key");
							_door_open = true;
						}
						else if (nowID == 17 && !_piano_open) {
							router.UnblockPath(17, 7);
							_piano_open = true;
							_door_open = true;
						}
						else if (nowID == 7 && gif.FindItem("babyroom key")) {
							router.UnblockPath(7, 20);
							gif.DeleteItem("babyroom key");
							_door_open = true;
						}
						else {
							_door_lock = true;

						}
					}
					else {
						_nextmapx = router.GetNowMapPortal(nowID)[i].GetPointByIndex(j).GetY();
						_nextmapy = router.GetNowMapPortal(nowID)[i].GetPointByIndex(j).GetZ();
						_nextMapID = router.GetNowMapPortal(nowID)[i].GetID();
						_isMapChanged = true;
					}
					break;
				}
			}
			if (_isMapChanged) {
				break;
			}
		}
	}
	void MainHuman::OnMove() {
		if (_direction == up) {
			bitmap.SetFrameIndexOfBitmap(BITMAP_UP);
		}
		else if (_direction == down) {
			bitmap.SetFrameIndexOfBitmap(BITMAP_DOWN);
		}
		else if (_direction == left) {
			bitmap.SetFrameIndexOfBitmap(BITMAP_LEFT);
		}
		else if (_direction == right) {
			bitmap.SetFrameIndexOfBitmap(BITMAP_RIGHT);
		}
		bitmap.SetTopLeft(_pos_x, _pos_y);

	}
	void MainHuman::OnMoveBySettings(int countblock){
		if (_isup || _isdown || _isleft || _isright) {
			TimerStart();
			_machine_done = false;
		}
		if (IsTimerStart()) {
			if (TimerGetCount() % 8 == 0) {
				_nowmove = _pressing;
			}
			if (TimerGetCount() == 8) {
				TimerReset();
				_machine_count += 1;
				if (_machine_count == countblock) {
					_machine_done = true;
					TimerStop();
					SetAllMoveFalse();
					_machine_count = 0;
				}
				_walkiter = !_walkiter;
			}
			if (TimerGetCount() < 4) {
				_bstate = s1;
			}
			else {
				_bstate = s2;
			}
			if (_nowmove == up) {
				_pos_y -= _step;
			}
			else if (_nowmove == down) {
				_pos_y += _step;
			}
			else if (_nowmove == left) {
				_pos_x -= _step;
			}
			else if (_nowmove == right) {
				_pos_x += _step;
			}
			TimerUpdate();
		}
		bitmap.SetTopLeft(_pos_x, _pos_y);

	}
	
	void MainHuman::SetAllMoveFalse() {
		_isup = false;
		_isdown = false;
		_isleft = false;
		_isright = false;
		_nowmove = none;
		_pressing = none;
	}
	void MainHuman::SetNowmove(Direction m) {
		_nowmove = m;
	}
	void MainHuman::OnKeyDown(UINT nChar) {
		if (nChar == VK_LEFT) {
			_direction = left;
			_pressing = left;
			_isleft = true;
		}
		else if (nChar == VK_UP) {
			_direction = up;
			_pressing = up;
			_isup = true;
		}
		else if (nChar == VK_RIGHT) {
			_direction = right;
			_pressing = right;
			_isright = true;
		}
		else if (nChar == VK_DOWN) {
			_direction = down;
			_pressing = down;
			_isdown = true;
		}
		
	}
	

	void MainHuman::OnKeyUp(UINT nChar){
		if (nChar == VK_LEFT) {
			_isleft = false;
		}
		else if (nChar == VK_UP) {
			_isup = false;
		}
		else if (nChar == VK_RIGHT) {
			_isright = false;
		}
		else if (nChar == VK_DOWN) {
			_isdown = false;
		}
	
	}
	void MainHuman::CheckMapChangeTN(GameMap& map, MapRouter& router, int const nowID, const vector<vector<int>>& TN, Interface& gif) {
		bool _canchangemap = true;

		for (auto& f : TN) {
			if ((this->GetX() - map.GetX()) / TILE == f[0] && (this->GetY() - map.GetY()) / TILE == f[1] && nowID == f[2]) {
				_canchangemap = false;
				break;
			}
		}
		if (_canchangemap) {
			RouterCheckChangeMap(map, router, nowID,gif);
		}

	}
	void MainHuman::SetNextMap(int x,int y,int NextID) {
		_nextmapx = x*TILE;
		_nextmapy = y*TILE;
		_nextMapID = NextID;
		_isMapChanged = true;
	}
	


	void MainHuman::SetMachine(Direction pressing)
	{
		_pressing = pressing;
		if (pressing == up) {
			_direction = up;
			_isup = true;
		}
		else if (pressing == down) {
			_direction = down;
			_isdown = true;
		}
		else if (pressing == left) {
			_direction = left;
			_isleft = true;
		}
		else if (pressing == right) {
			_direction = right;
			_isright = true;
		}
	}

}