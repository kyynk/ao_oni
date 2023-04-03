#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "config.h"
#include <string>
#include <vector>
#include "Entity.h"
#include "Item.h"
namespace game_framework {
	Item::Item() {
		_step, _anidelay, _boxX, _boxY = 0;
		_type = select;
		_move = none;
		_pressing = none;
		_press = false;
		_triggered = false;
		_pick = false;
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

		vector<string> bitmapName;
		if (_name == lib_book) {
			bitmapName.push_back("img/item_animation/lib_book/lib_book.bmp");
		}
		else if (_name == key_lib) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/star/star" + to_string(i) + ".bmp");
			}
		}
		else if (_name == key_3F_L) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/star/star" + to_string(i) + ".bmp");
			}
		}
		else if (_name == key_2F_TL) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/box_key/key" + to_string(i) + ".bmp");
			}
		}
		else if (_name == key_basement) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/star/star" + to_string(i) + ".bmp");
			}
		}
		else if (_name == key_jail) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/jail_key/jail_key" + to_string(i) + ".bmp");
			}
		}
		else if (_name == key_annexe) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/box_key/key" + to_string(i) + ".bmp");
			}
		}
		else if (_name == broken_dish) {
			for (int i = 0; i < 2; i++) {
				bitmapName.push_back("img/item_animation/dish/dish" + to_string(i) + ".bmp");
			}
		}
		else if (_name == tub_once) {
			for (int i = 1; i < 8; i++) {
				bitmapName.push_back("img/item_animation/tub/tub" + to_string(i) + ".bmp");
			}
		}
		else if (_name == phillips) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/tub/tub_star" + to_string(i) + ".bmp");
			}
		}
		else if (_name == tub_fixed) {
			bitmapName.push_back("img/item_animation/tub/tub7.bmp");
		}
		else if (_name == flathead) {
			bitmapName.push_back("img/item_animation/flathead/flathead.bmp");
		}
		else if (_name == lighter) {
			for (int i = 0; i < 3; i++) {
				bitmapName.push_back("img/item_animation/tatami/tatami_lighter" + to_string(i) + ".bmp");
			}
		}
		else if (_name == oil) {
			bitmapName.push_back("img/item/oil.bmp");
		}
		else if (_name == handkerchief) {
			bitmapName.push_back("img/item/hankati.bmp");
		}
		else if (_name == detergent) {
			for (int i = 0; i < 2; i++) {
				bitmapName.push_back("img/item_animation/detergent/detergent" + to_string(i) + ".bmp");
			}
		}
		else if (_name == door_knob) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/doorknob/doorknob" + to_string(i) + ".bmp");
			}
		}
		else if (_name == door_no_knob) {
			bitmapName.push_back("img/item_animation/doorknob/nodoorknob.bmp");
		}
		else if (_name == gate) {
			for (int i = 0; i < 2; i++) {
				bitmapName.push_back("img/item_animation/gate/gate" + to_string(i) + ".bmp");
			}
		}
		else if (_name == toilet) {
			for (int i = 0; i < 2; i++) {
				bitmapName.push_back("img/item_animation/toilet/toilet" + to_string(i) + ".bmp");
			}
		}
		Load(bitmapName, RGB(204, 255, 0));
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
		if (_playerX + 32 == _pos_x && (_playerY >= _pos_y && _playerY <= y) && _pressing == isright)
			_move = isright;
		else if (_playerX - 32 == x && (_playerY >= _pos_y || _playerY <= y) && _pressing == isleft)
			_move = isleft;
		else if ((_playerX >= _pos_x && _playerX <= x) && _playerY + 32 == _pos_y && _pressing == isdown)
			_move = isdown;
		else if ((_playerX >= _pos_x && _playerX <= x) && _playerY - 32 == y && _pressing == isup)
			_move = isup;
		else
			_move = none;
		_pressing = none;
	}
	void Item::OnMove() {
		if (Collide() && _press) {
			// will add more condition in future
			_close = true;
			SetTrigger();
			_press = false;
		}
	}
	void Item::OnKeyDown(UINT nChar) {
		if (nChar == VK_SPACE) {
			_press = true;
		}
		if (nChar == VK_LEFT) {
			_pressing = isleft;
			//TRACE("\n\n press L \n\n");
		}
		else if (nChar == VK_UP) {
			_pressing = isup;
			//TRACE("\n\n press U \n\n");
		}
		else if (nChar == VK_RIGHT) {
			_pressing = isright;
			//TRACE("\n\n press R \n\n");
		}
		else if (nChar == VK_DOWN) {
			_pressing = isdown;
			//TRACE("\n\n press D \n\n");
		}
	}
	void Item::OnKeyUp(UINT nChar) {
		if (nChar == VK_SPACE) {
			_press = false;
		}
	}
	void Item::OnShow() {
		if (!_pick)
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
		else if (_name == key_basement) return "basement key";
		else if (_name == key_jail) return "jail key";
		else if (_name == key_annexe) return "annexe key";
		else if (_name == broken_dish) return "broken dish";
		else if (_name == phillips) return "phillips screwdriver";
		else if (_name == flathead) return "flathead screwdriver";
		else if (_name == lighter) return "lighter";
		else if (_name == oil) return "oil";
		else if (_name == handkerchief) return "handkerchief";
		else if (_name == detergent) return "detergent";
		else if (_name == door_knob) return "door knob";
		return "";
	}
}