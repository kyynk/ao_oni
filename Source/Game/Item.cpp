#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "config.h"
#include <string.h>
#include "Entity.h"
#include "Item.h"
namespace game_framework {
	Item::Item() {
		_step, _anidelay, _boxX, _boxY = 0;
		_type = select;
		_move = none;
		_press = false;
		_triggered = false;
		_close = false;
	}
	Item::~Item() {
	}
	void Item::SetParam(int step, int delay, itemtype type,
		int boxX, int boxY, itemName name) {
		SetXY(0, 0);
		_step = step;
		_anidelay = delay;
		_type = type;
		_boxX = boxX;
		_boxY = boxY;
		_name = name;
	}
	void Item::Load(vector<string> filename, COLORREF color) {
		bitmap.LoadBitmapByString(filename, color);
	}
	void Item::SetPos(int x, int y) {
		SetXY(x, y);
	}
	int Item::GetPosX() {
		return _pos_x;
	}
	int Item::GetPosY() {
		return _pos_y;
	}
	int Item::GetPosL() {
		return _pos_x - TILE;
	}
	int Item::GetPosU() {
		return _pos_y - TILE;
	}
	int Item::GetPosR() {
		return _pos_x + _boxX + TILE;
	}
	int Item::GetPosD() {
		return _pos_y + _boxY + TILE;
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
	void Item::OnMove() {
		if (Collide() && _press) {
			// will add more condition in future
			_close = true;
			SetTrigger();
		}
	}
	void Item::OnKeyDown(UINT nChar) {
		if (nChar == VK_SPACE) {
			_press = true;
		}
	}
	void Item::OnKeyUp(UINT nChar) {
		if (nChar == VK_SPACE) {
			_press = false;
		}
	}
	void Item::OnShow() {
		if (!_close)
			bitmap.ShowBitmap();
		else
			bitmap.UnshowBitmap();
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
	string Item::GetName() {
		if (_name == key_lib) return "library key";
		else if (_name == key_3F_L) return "3F bedroom key";
		else if (_name == key_2F_TL) return "2F kid room key";
		else if (_name == broken_dash) return "broken dash";
		else if (_name == flathead) return "flathead_screwdriver";
		else if (_name == phillips) return "phillips_screwdriver";
		else if (_name == lighter) return "lighter";
		else if (_name == oil) return "oil";
		else if (_name == handkerchief) return "handkerchief";
		else if (_name == detergent) return "detergent";
		else if (_name == door_knob) return "door_knob";
		return "";
	}
}