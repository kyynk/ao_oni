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
		_uy = _pos_y - TILE;
		_dy = _pos_y + TILE;
		_lx = _pos_x - TILE;
		_rx = _pos_x + TILE;
		_step = 0;
		_walkiter = true;
		_bstate = s1;
		_isup = false ;
		_isdown = false;
		_isleft = false;
		_isright = false;

		_bdown = false;
		_bup = false;
		_bleft = false;
		_bright = false;
		TimerReset();
		_premove = none;
		_nowmove = none;
		_pressing = none;
	}
	void Human::init(int step,int offset) {
		_step = step;
		_coroffset = offset;
	}

	void Human::OnMove(GameMap &map) {
		
		bool upmovable = false;
		bool downmovable = false;
		bool leftmovable = false;
		bool rightmovable = false;
		
		if ((map.GetMapData(0, (this->GetX1() - map.GetX()) / TILE, (this->GetU() - map.GetY()) / TILE)==0||
			map.GetMapData(0, (this->GetX1() - map.GetX()) / TILE, (this->GetU() - map.GetY()) / TILE) == -87) &&
			(this->GetX1() - map.GetX()) % TILE == 0 &&
			(this->GetU() - map.GetY()) % TILE == 0  ) {
			upmovable = false;
		}
		else {
			upmovable = true;
		}
		if ((map.GetMapData(0, (this->GetX1() - map.GetX()) / TILE, (this->GetD() - map.GetY()) / TILE)==0||
			map.GetMapData(0, (this->GetX1() - map.GetX()) / TILE, (this->GetD() - map.GetY()) / TILE) == -87) &&
			(this->GetX1() - map.GetX()) % TILE == 0 &&
			(this->GetD() - map.GetY()) % TILE == 0) {
			downmovable = false;
		}
		else {
			downmovable = true;
		}
		if ((map.GetMapData(0, (this->GetL() - map.GetX()) / TILE, (this->GetY1() - map.GetY()) / TILE)==0||
			map.GetMapData(0, (this->GetL() - map.GetX()) / TILE, (this->GetY1() - map.GetY()) / TILE) == -87) &&
			(this->GetL() - map.GetX()) % TILE == 0 &&
			(this->GetY1() - map.GetY()) % TILE == 0) {
			leftmovable = false;
		}
		else {
			leftmovable = true;
		}
		if ((map.GetMapData(0, (this->GetR() - map.GetX()) / TILE, (this->GetY1() - map.GetY()) / TILE)==0||
			map.GetMapData(0, (this->GetR() - map.GetX()) / TILE, (this->GetY1() - map.GetY()) / TILE) == -87) &&
			(this->GetR() - map.GetX()) % TILE == 0 &&
			(this->GetY1() - map.GetY()) % TILE == 0) {
			rightmovable = false;
		}
		else {
			rightmovable = true;
		}
		if (_isup || _isdown || _isleft || _isright ) {
			TimerStart();
		}
		else {
			if (TimerGetCount() == 8) {
				TimerStop();
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
		bitmap.SetTopLeft(_pos_x, _pos_y);
	
		//TRACE("%d\n", TimerGetCount());
	}
	void Human::OnKeyDown(UINT nChar,GameMap &map) {
		
		if (nChar == VK_LEFT) {
			_premove = _pressing;
			_pressing = isleft;
			_isleft = true;
		}
		else if (nChar == VK_UP) {
			_premove = _pressing;
			_pressing = isup;
			_isup = true;
		}
		else if (nChar == VK_RIGHT) {
			_premove = _pressing;
			_pressing = isright;
			_isright = true;
		}
		else if (nChar == VK_DOWN) {
			_premove = _pressing;
			_pressing = isdown;
			_isdown = true;
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
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_UP_1) : bitmap.SetFrameIndexOfBitmap(HUMAN_UP_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_UP);
			}
		}
		else if (_nowmove == isdown) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_DOWN_1) : bitmap.SetFrameIndexOfBitmap(HUMAN_DOWN_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_DOWN);
			}
		}

		else if (_nowmove == isleft) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_LEFT_1) : bitmap.SetFrameIndexOfBitmap(HUMAN_LEFT_2);
			}
			else {
				bitmap.SetFrameIndexOfBitmap(HUMAN_LEFT);
			}
		}

		else if (_nowmove == isright) {
			if (_bstate == s1) {
				_walkiter ? bitmap.SetFrameIndexOfBitmap(HUMAN_RIGHT_1) : bitmap.SetFrameIndexOfBitmap(HUMAN_RIGHT_2);
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