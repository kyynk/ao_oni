#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "MapData.h"
#include "GameMap.h"
#include "MapNode.h"
#include "MapRouter.h"
#include "config.h"
#include "Entity.h"
#include "Human.h"

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

	Human::Human() :Entity() {
		SetXY(12 * TILE, 11 * TILE + TILE / 2);
		
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
		_premove = none;
		_nowmove = none;
		_pressing = none;
	}
	void Human::init(int step,int offset,Direction dir) {
		_step = step;
		_coroffset = offset;
		_direction = dir;
		_uy = _pos_y - TILE;
		_dy = _pos_y + TILE;
		_lx = _pos_x - TILE;
		_rx = _pos_x + TILE;
	}

	void Human::OnMove(GameMap &map, MapRouter &router, int nowID, vector<vector<int>>&VL, vector<vector<int>>&VR, vector<vector<int>>&TN) {
		if (_isMapChanged && _switchMapCheck) {
			//TRACE("posx:%d posy:%d tilex:%d tiley:%d \n", _pos_x, _pos_y, _pos_x/TILE, _pos_y/TILE);
			if (_pressing == isup) {
				_pos_x = map.GetX() + _nextmapx ;
				_pos_y = map.GetY() + _nextmapy - _coroffset - TILE;
				//TRACE("%d %d %d \n",_pos_x / TILE,_pos_y / TILE,1);
				//TRACE("%d %d %d \n",_pos_x % TILE,_pos_y % TILE,1);
					
			}
			else if (_pressing == isdown) {
				_pos_x = map.GetX() + _nextmapx;
				_pos_y = map.GetY() + _nextmapy - _coroffset + TILE;
				//TRACE("%d %d %d \n",_pos_x/TILE,_pos_y / TILE,2);
				//TRACE("%d %d %d \n",_pos_x%TILE,_pos_y % TILE,2);
			}
			else if (_pressing == isleft) {
				_pos_x = map.GetX() + _nextmapx - TILE;
				_pos_y = map.GetY() + _nextmapy - _coroffset;
				//TRACE("%d %d %d \n",_pos_x / TILE,_pos_y / TILE,3);
				//TRACE("%d %d %d \n",_pos_x % TILE,_pos_y % TILE,3);
			}
			else if (_pressing == isright) {
				_pos_x = map.GetX() + _nextmapx + TILE;
				_pos_y = map.GetY() + _nextmapy - _coroffset;
				//TRACE("%d %d %d \n",_pos_x / TILE,_pos_y / TILE,4);
				//TRACE("%d %d %d \n",_pos_x % TILE,_pos_y % TILE,4);
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
					if (this->GetX() % TILE != 0 || this->GetY() % TILE != 0) {
						int dx = 0;
						int dy = 1;
						if (this->GetX() % TILE >= TILE / 2) {
							dx = 1;

						}
						if (this->GetY() % TILE < TILE / 2) {
							dy = 0;
						}
						_pos_x = (_pos_x / TILE + dx)*TILE;
						_pos_y = ((_pos_y + _coroffset) / TILE + dy)*TILE - _coroffset;
						_uy = _pos_y - TILE;
						_dy = _pos_y + TILE;
						_lx = _pos_x - TILE;
						_rx = _pos_x + TILE;
						bitmap.SetTopLeft(_pos_x, _pos_y);
						TRACE("return to grid\n");
					}
					_walkiter = !_walkiter;
				}
			}
			if (IsTimerStart()) {
				if (TimerGetCount() % 8 == 0) {
					_nowmove = _pressing;
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
				TimerUpdate();

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
	void Human::OnKeyDown(UINT nChar) {
		//TRACE("%d %d \n", _premove, _pressing);
		
		if (nChar == VK_LEFT) {
			_direction = left;
			if (_isMapChanged) {
				_switchMapCheck = true;
			}
			else {
				//_premove = _nowmove;
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
				//_premove = _nowmove;
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
				//_premove = _nowmove;
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
				//_premove = _nowmove;
				_pressing = isdown;
				_isdown = true;
			}
		}
	}
	

	void Human::OnKeyUp(UINT nChar){
		if (nChar == VK_LEFT) {
			_premove = isleft;
			_isleft = false;
		}
		else if (nChar == VK_UP) {
			_premove = isup;
			_isup = false;
		}
		else if (nChar == VK_RIGHT) {
			_premove = isright;
			_isright = false;
		}
		else if (nChar == VK_DOWN) {
			_premove = isdown;
			_isdown = false;
		}
	}

	void Human::OnShow() {
		

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
	void Human::Load(vector<string> filenames, COLORREF color) {
		bitmap.LoadBitmapByString(filenames,color );

	}

}