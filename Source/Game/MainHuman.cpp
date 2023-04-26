#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "config.h"
#include "GameMap.h"
#include "MapRouter.h"
#include "Entity.h"
#include "MainHuman.h"

#define HUMAN_DOWN 0
#define HUMAN_DOWN_1 1
#define HUMAN_DOWN_2 2
#define HUMAN_UP 9
#define HUMAN_UP_1 10
#define HUMAN_UP_2 11
#define HUMAN_LEFT 3
#define HUMAN_LEFT_1 4
#define HUMAN_LEFT_2 5
#define HUMAN_RIGHT 6
#define HUMAN_RIGHT_1 7
#define HUMAN_RIGHT_2 8

namespace game_framework{

	MainHuman::MainHuman() :Entity() {
		
		_step = 0;
		_walkiter = true;
		_bstate = s1;
		_isup = false ;
		_isdown = false;
		_isleft = false;
		_isright = false;
		_isMapChanged = false;
		_switchMapCheck = false;
		_nextmapx = 0;
		_nextmapy = 0;
		_nextMapID = 0;
		TimerReset();
		_nowmove = none;
		_pressing = none;
	}
	void MainHuman::SetXYAndCol(int x,int y) {
		SetXY(x * TILE, y * TILE - TILE / 2);
		_uy = _pos_y - TILE;
		_dy = _pos_y + TILE;
		_lx = _pos_x - TILE;
		_rx = _pos_x + TILE;
	}
	void MainHuman::init(int step,int offset,direction dir) {
		_step = step;
		_coroffset = offset;
		_direction = dir;
		machine_count = 0;
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
			_pos_x = (_pos_x / TILE + dx) * TILE;
			_pos_y = ((_pos_y + _coroffset) / TILE + dy) * TILE - _coroffset;
			_uy = _pos_y - TILE;
			_dy = _pos_y + TILE;
			_lx = _pos_x - TILE;
			_rx = _pos_x + TILE;
			bitmap.SetTopLeft(_pos_x, _pos_y);
			//TRACE("return to grid\n");
		}
	}
	void MainHuman::OnMove(GameMap &map, MapRouter &router, int nowID, vector<vector<int>>&VL, vector<vector<int>>&VR, vector<vector<int>>&TN) {
		if (_isMapChanged && _switchMapCheck) {
			TRACE("posx:%d posy:%d tilex:%d tiley:%d \n", _pos_x, _pos_y, _pos_x/TILE, _pos_y/TILE);
			
			if (_nowmove == isup) {
				_pos_x = map.GetX() + _nextmapx ;
				_pos_y = map.GetY() + _nextmapy - _coroffset - TILE;
			}
			else if (_nowmove == isdown) {
				_pos_x = map.GetX() + _nextmapx;
				_pos_y = map.GetY() + _nextmapy - _coroffset + TILE;
			}
			else if (_nowmove == isleft) {
				_pos_x = map.GetX() + _nextmapx - TILE;
				_pos_y = map.GetY() + _nextmapy - _coroffset;
			}
			else if (_nowmove == isright) {
				_pos_x = map.GetX() + _nextmapx + TILE;
				_pos_y = map.GetY() + _nextmapy - _coroffset;
			}
			else if (_nowmove == machinetransmap) {
				_pos_x = map.GetX() + _nextmapx;
				_pos_y = map.GetY() + _nextmapy - _coroffset;
			}
			if (_nowmove == none) {
				TRACE("In player default on move _nowmove= none\n");
			}
			_uy = _pos_y - TILE;
			_dy = _pos_y + TILE;
			_lx = _pos_x - TILE;
			_rx = _pos_x + TILE;
			bitmap.SetTopLeft(_pos_x , _pos_y);
			
			_isMapChanged = false;
			_switchMapCheck = false;
		}
		else {
			bool itercheck = true;
			bool upmovable = false;
			bool downmovable = false;
			bool leftmovable = false;
			bool rightmovable = false;
			if ((map.GetMapData(0, (this->GetX() - map.GetX()) / TILE, (this->GetU() - map.GetY()) / TILE) == 0 ||
				map.GetMapData(0, (this->GetX() - map.GetX()) / TILE, (this->GetU() - map.GetY()) / TILE) == -87) &&
				(this->GetX() - map.GetX()) % TILE == 0 &&
				(this->GetU() - map.GetY()) % TILE == 0) {
				upmovable = false;
			}
			else {
				upmovable = true;
			}
			if ((map.GetMapData(0, (this->GetX() - map.GetX()) / TILE, (this->GetD() - map.GetY()) / TILE) == 0 ||
				map.GetMapData(0, (this->GetX() - map.GetX()) / TILE, (this->GetD() - map.GetY()) / TILE) == -87) &&
				(this->GetX() - map.GetX()) % TILE == 0 &&
				(this->GetD() - map.GetY()) % TILE == 0) {
				downmovable = false;
			}
			else {
				downmovable = true;
			}
			for (auto f : VL) {
				if ( (this->GetX() - map.GetX()) / TILE == f[0] && 
					 (this->GetX() - map.GetX()) % TILE == 0 && 
					 (this->GetY() - map.GetY()) / TILE == f[1] && 
					 (this->GetY() - map.GetY()) % TILE == 0 && 
					nowID == f[2]) {
					leftmovable = false;
					itercheck = false;
					break;
				}
			}
			
			if (itercheck) {
				if ((map.GetMapData(0, (this->GetL() - map.GetX()) / TILE, (this->GetY() - map.GetY()) / TILE) == 0 ||
					map.GetMapData(0, (this->GetL() - map.GetX()) / TILE, (this->GetY() - map.GetY()) / TILE) == -87) &&
					(this->GetL() - map.GetX()) % TILE == 0 &&
					(this->GetY() - map.GetY()) % TILE == 0) {
					leftmovable = false;
				}

				else {
					leftmovable = true;
				}
			}
			itercheck = true;
			for (auto f : VR) {
				if ((this->GetX() - map.GetX()) / TILE == f[0] &&
					(this->GetX() - map.GetX()) % TILE == 0 &&
					(this->GetY() - map.GetY()) / TILE == f[1] &&
					(this->GetY() - map.GetY()) % TILE == 0 &&
					nowID == f[2]) {
					rightmovable = false;
					itercheck = false;
					break;
				}
			}
			if (itercheck) {
				if ((map.GetMapData(0, (this->GetR() - map.GetX()) / TILE, (this->GetY() - map.GetY()) / TILE) == 0 ||
					map.GetMapData(0, (this->GetR() - map.GetX()) / TILE, (this->GetY() - map.GetY()) / TILE) == -87) &&
					(this->GetR() - map.GetX()) % TILE == 0 &&
					(this->GetY() - map.GetY()) % TILE == 0) {
					rightmovable = false;
				}
				else {
					rightmovable = true;
				}
			}
			itercheck = true;
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
				if (_nowmove == isup && upmovable) {
					_pos_y -= _step;
					_uy -= _step;
					_dy -= _step;
				}
				else if (_nowmove == isdown && downmovable) {
					_pos_y += _step;
					_uy += _step;
					_dy += _step;
				}
				else if (_nowmove == isleft && leftmovable) {
					_pos_x -= _step;
					_lx -= _step;
					_rx -= _step;
				}
				else if (_nowmove == isright && rightmovable) {
					_pos_x += _step;
					_lx += _step;
					_rx += _step;
				}
				//TRACE("%d\n", TimerGetCount());
				TimerUpdate(clock());

			}
			for (auto f : TN) {
				if ((this->GetX() - map.GetX()) / TILE == f[0] && (this->GetY() - map.GetY()) / TILE == f[1] && nowID == f[2]) {
					itercheck = false;
					break;
				}
			}
			if (itercheck) {
				for (int i = 0; i < router.GetRecord(nowID); i++) {
					for (int j = 0; j < router.GetNowMapPortal(nowID)[i].GetSize(); j++) {
						if(router.IsPathBlocked(nowID, j)) continue;
						if (router.GetNowMapPortal(nowID)[i].GetPointByIndex(j) ==
							NodeData(this->GetL() - map.GetX(), this->GetY() - map.GetY()) &&
							_direction == left &&
							_nowmove == isleft) {
							_nextmapx = router.GetNowMapPortal(nowID)[i].GetPointByIndex(j).GetY();
							_nextmapy = router.GetNowMapPortal(nowID)[i].GetPointByIndex(j).GetZ();
							_nextMapID = router.GetNowMapPortal(nowID)[i].GetID();
							_isMapChanged = true;
							break;
						}
						else if (router.GetNowMapPortal(nowID)[i].GetPointByIndex(j) ==
							NodeData(this->GetR() - map.GetX(), this->GetY() - map.GetY()) &&
							_direction == right &&
							_nowmove == isright) {
							_nextmapx = router.GetNowMapPortal(nowID)[i].GetPointByIndex(j).GetY();
							_nextmapy = router.GetNowMapPortal(nowID)[i].GetPointByIndex(j).GetZ();
							_nextMapID = router.GetNowMapPortal(nowID)[i].GetID();
							_isMapChanged = true;
							break;
						}
						else if (router.GetNowMapPortal(nowID)[i].GetPointByIndex(j) ==
							NodeData(this->GetX() - map.GetX(), this->GetU() - map.GetY()) &&
							_direction == up &&
							_nowmove == isup) {
							_nextmapx = router.GetNowMapPortal(nowID)[i].GetPointByIndex(j).GetY();
							_nextmapy = router.GetNowMapPortal(nowID)[i].GetPointByIndex(j).GetZ();
							_nextMapID = router.GetNowMapPortal(nowID)[i].GetID();
							_isMapChanged = true;
							break;
						}
						else if (router.GetNowMapPortal(nowID)[i].GetPointByIndex(j) ==
							NodeData(this->GetX() - map.GetX(), this->GetD() - map.GetY()) &&
							_direction == down &&
							_nowmove == isdown) {
							_nextmapx = router.GetNowMapPortal(nowID)[i].GetPointByIndex(j).GetY();
							_nextmapy = router.GetNowMapPortal(nowID)[i].GetPointByIndex(j).GetZ();
							_nextMapID = router.GetNowMapPortal(nowID)[i].GetID();
							_isMapChanged = true;
							break;
						}
					}
					if (_isMapChanged) {
						break;
					}
				}
			}
			bitmap.SetTopLeft(_pos_x, _pos_y);
		}
		
	}

	void MainHuman::OnMoveBySettings(int countblock){
		TRACE("onmoveby setting running\n");
		if (_isup || _isdown || _isleft || _isright) {
			TimerStart();
		}
		if (IsTimerStart()) {
			if (TimerGetCount() % 8 == 0) {
				_nowmove = _pressing;
			}
			if (TimerGetCount() == 8) {
				TimerReset();
				machine_count += 1;
				if (machine_count == countblock) {
					TimerStop();
					SetAllMoveFalse();
					machine_count = 0;
				}
				_walkiter = !_walkiter;
			}
			if (TimerGetCount() < 4) {
				_bstate = s1;
			}
			else {
				_bstate = s2;
			}
			if (_nowmove == isup) {
				_pos_y -= _step;
				_uy -= _step;
				_dy -= _step;
			}
			else if (_nowmove == isdown) {
				_pos_y += _step;
				_uy += _step;
				_dy += _step;
			}
			else if (_nowmove == isleft) {
				_pos_x -= _step;
				_lx -= _step;
				_rx -= _step;
			}
			else if (_nowmove == isright) {
				_pos_x += _step;
				_lx += _step;
				_rx += _step;
			}
			TimerUpdate(clock());
		}
		bitmap.SetTopLeft(_pos_x, _pos_y);

	}
	void MainHuman::OnMove() {
		if (_direction == up) {
			bitmap.SetFrameIndexOfBitmap(HUMAN_UP);
		}
		else if (_direction == down) {
			bitmap.SetFrameIndexOfBitmap(HUMAN_DOWN);
		}
		else if (_direction == left) {
			bitmap.SetFrameIndexOfBitmap(HUMAN_LEFT);
		}
		else if (_direction == right) {
			bitmap.SetFrameIndexOfBitmap(HUMAN_RIGHT);
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
		
		TRACE("set all false\n");
	}
	void MainHuman::SetNowmove(move m) {
		_nowmove = m;
	}
	void MainHuman::OnKeyDown(UINT nChar) {
		if (nChar == VK_LEFT) {
			_direction = left;
			if (_isMapChanged) {
				_switchMapCheck = true;
			}
			else {
				_pressing = isleft;
				_isleft = true;
			}
		}
		else if (nChar == VK_UP) {
			_direction = up;
			if (_isMapChanged) {
				_switchMapCheck = true;
			}
			else {
				_pressing = isup;
				_isup = true;
			}
		}
		else if (nChar == VK_RIGHT) {
			_direction = right;
			if (_isMapChanged) {
				_switchMapCheck = true;
			}
			else {
				_pressing = isright;
				_isright = true;
			}
			
		}
		else if (nChar == VK_DOWN) {
			_direction = down;
			if (_isMapChanged) {
				_switchMapCheck = true;
			}
			else {
				_pressing = isdown;
				_isdown = true;
			}
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
	void MainHuman::SetNextMap(int x,int y,int NextID) {
		_nextmapx = x*TILE;
		_nextmapy = y*TILE;
		_nextMapID = NextID;
		_isMapChanged = true;
		_switchMapCheck = true;

	}
	void MainHuman::OnShow() {
		

		if (_nowmove == isup) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_UP_1) : 
					bitmap.SetFrameIndexOfBitmap(HUMAN_UP_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_UP);
			}
		}
		else if (_nowmove == isdown) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_DOWN_1) : 
					bitmap.SetFrameIndexOfBitmap(HUMAN_DOWN_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_DOWN);
			}
		}

		else if (_nowmove == isleft) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_LEFT_1) :
					bitmap.SetFrameIndexOfBitmap(HUMAN_LEFT_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_LEFT);
			}
		}

		else if (_nowmove == isright) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_RIGHT_1) :
					bitmap.SetFrameIndexOfBitmap(HUMAN_RIGHT_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_RIGHT);
			}
		}

		bitmap.ShowBitmap();

	}
	void MainHuman::Load(vector<string> filenames, COLORREF color) {
		bitmap.LoadBitmapByString(filenames,color );

	}

	void MainHuman::SetIsMachine(bool isMachine, move pressing)
	{
		_pressing = pressing;
		if (pressing == isup) {
			_direction = up;
			_isup = true;
		}
		else if (pressing == isdown) {
			_direction = down;
			_isdown = true;
		}
		else if (pressing == isleft) {
			_direction = left;
			_isleft = true;
		}
		else if (pressing == isright) {
			_direction = right;
			_isright = true;
		}
		//_isMachine = false;
	}

}