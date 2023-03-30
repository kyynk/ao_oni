#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "config.h"
#include "Entity.h"
#include "Item.h"
namespace game_framework {
	Item::Item() {
		_step, _anidelay, _boxX, _boxY = 0;
		_type = select;
		_move = none;
		_triggered = false;
		_close = false;
	}
	Item::~Item() {
	}
	void Item::SetParam(int step, int delay, itemtype type, 
		int boxX, int boxY) {
		SetXY(0, 0);
		_step = step;
		_anidelay = delay;
		_type = type;
		_boxX = boxX;
		_boxY = boxY;
	}
	void Item::Load(vector<string> filename, COLORREF color) {
		bitmap.LoadBitmapByString(filename, color);
	}
	void Item::GetPlayerPos(int x, int y) {
		_playerX = x;
		_playerY = y;
	}
	void Item::CheckMoveDirection() {
		int x = _pos_x + _boxX;
		int y = _pos_y + _boxY;
		if (_playerX + 32 == _pos_x && (_playerY == _pos_y || _playerY == y))
			_move = isright;
		else if (_playerX - 32 == x && (_playerY == _pos_y || _playerY == y))
			_move = isleft;
		else if ((_playerX == _pos_x || _playerX == x) && _playerY + 32 == _pos_y)
			_move = isdown;
		else if ((_playerX == _pos_x || _playerX == x) && _playerY - 32 == y)
			_move = isup;
		else
			_move = none;
	}
	void Item::OnMove(UINT nChar) {
		if (Collide()) {
			if (nChar == 0x47)
				_close = true;
			if (_move == isright && nChar == VK_RIGHT)
				_pos_x += _step;
			else if (_move == isleft && nChar == VK_LEFT)
				_pos_x -= _step;
			else if (_move == isup && nChar == VK_UP)
				_pos_y -= _step;
			else if (_move == isdown && nChar == VK_DOWN)
				_pos_y += _step;
		}
		bitmap.SetTopLeft(_pos_x, _pos_y);
	}
	void Item::OnShow() {
		if(!_close)
			bitmap.ShowBitmap();
	}
	bool Item::Collide() {
		CheckMoveDirection();
		return _move != none;
	}
	void Item::Animation(int n = 0) {
		if (_triggered) {
			if (_type == once) {
				bitmap.ToggleAnimation();
				bitmap.SetAnimation(_anidelay, true);
			}
			else if (_type == repeat) {
				bitmap.SetAnimation(_anidelay, false);
			}
			else if (_type == select) {
				SelectShowBitmap(n);
			}
			_triggered = false;
		}
	}
	void Item::SetTrigger() {
		_triggered = true;
	}
}