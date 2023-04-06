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
		SetXY(14 * TILE, 15 * TILE);
		_anidelay, _boxX, _boxY = 0;
		_move = none;
		_pressing = none;
		_aniType = -1;
		_aniFrame = -1;
		_press = false;
		_triggered = false;
		_pick = false; // will disappear
		_close = true; // door, toilet, ...
		_fixed = false; // will not do anything if _fixed == true
		_take = false; // will leave
		_useItem = false; // will use screwdriver or lighter ...
	}
	Item::~Item() {
	}
	void Item::SetParam(int delay, int boxX, int boxY, 
		itemName name) {
		_anidelay = delay;
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
			SetTrigger();
			Animation(3, 0);
		}
		else if (_name == key_3F_L) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/star/star" + to_string(i) + ".bmp");
			}
			SetTrigger();
			Animation(3, 0);
		}
		else if (_name == key_2F_TL) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/box_key/key" + to_string(i) + ".bmp");
			}
			SetTrigger();
			Animation(3, 0);
		}
		else if (_name == key_basement) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/star/star" + to_string(i) + ".bmp");
			}
			SetTrigger();
			Animation(3, 0);
		}
		else if (_name == key_jail) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/jail_key/jail_key" + to_string(i) + ".bmp");
			}
			SetTrigger();
			Animation(3, 0);
		}
		else if (_name == key_annexe) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/box_key/key" + to_string(i) + ".bmp");
			}
			SetTrigger();
			Animation(3, 0);
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
			SetTrigger();
			Animation(3, 0);
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
		else if (_name == tatami) {
			for (int i = 0; i < 3; i++) {
				bitmapName.push_back("img/item_animation/tatami/tatami" + to_string(i) + ".bmp");
			}
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
		/*if (_move == isright) TRACE("\n\nmove is RRR\n\n");
		if (_move == isleft) TRACE("\n\nmove is LLL\n\n");
		if (_move == isdown) TRACE("\n\nmove is DDD\n\n");
		if (_move == isup) TRACE("\n\nmove is UUU\n\n");
		if (_move == none) TRACE("\n\nmove is NNN\n\n");*/
		_press = false;
	}
	void Item::OnMove() { // actually is action function
		if (_press) {
			/*TRACE("\n\npress\n\n");*/
			if (Collide()) {
				// on lib
				if (_name == lib_book && !_fixed) {
					SetXY(_pos_x, _pos_y + TILE);
					_fixed = true;
				}
				// chair move to fixed pos
				else if (_name == key_lib && !_pick) {
					_pick = true;
				}
				// lib_book move to fixed pos
				else if (_name == key_3F_L && !_pick) {
					_pick = true;
				}
				// password box in 2F_D unlock
				else if (_name == key_2F_TL && !_pick) {
					_pick = true;
				}
				// in the jail room (near tatami)
				else if (_name == key_basement && !_pick) {
					_pick = true;
				}
				// basement onChair can pick
				else if (_name == key_jail && _onCorrectPos && !_pick) {
					_pick = true;
				}
				// password box in basement1 unlock
				else if (_name == key_annexe && !_pick) {
					_pick = true;
				}
				// on kitchen
				else if (_name == broken_dish && !_take) {
					_take = true;
					SetTrigger();
					Animation(2, 1);
				}
				// on shower
				else if (_name == tub_once && !_fixed) {
					SetTrigger();
					Animation(0, 0);
					_fixed = true;
				}
				// in tub
				else if (_name == phillips && !_pick) {
					_pick = true;
				}
				// tub fixed only need to show, no interact
				// on basement (which has a chair), onChair can pick
				else if (_name == flathead && _onCorrectPos && !_pick) {
					_pick = true;
				}
				// tatami leftside
				else if (_name == lighter && _onCorrectPos && !_pick) {
					_pick = true;
				}
				// kid room, onchair can pick
				else if (_name == oil && _onCorrectPos && !_pick) {
					_pick = true;
				}
				// 2F_TR, in lib take 3F key
				else if (_name == handkerchief && !_pick) {
					_pick = true;
				}
				// in toilet
				else if (_name == detergent && !_pick) {
					_pick = true;
				}
				// 3F, need to use screwdriver
				else if (_name == door_knob && !_take) {
					if (_useItem)
						_take = true;
					else {
						if (_close) {
							SetTrigger();
							// will optimize Animation
							Animation(0, 1); // open
							_close = false;
						}
						else {
							SetTrigger();
							// will optimize Animation
							Animation(1, 3); // close
							_close = true;
						}
					}
				}
				// door_no_knob only need to show, no interact
				//tatami
				else if (_name == tatami) {
					if (_close) {
						SetTrigger();
						// will optimize Animation
						Animation(0, 0); // open
						_close = false;
					}
					else {
						SetTrigger();
						// will optimize Animation
						Animation(1, 2); // close
						_close = true;
					}
				}
				// jail
				else if (_name == gate) {
					if (_close) {
						SetTrigger();
						// will optimize Animation
						Animation(2, 1); // open
						_close = false;
					}
					else {
						SetTrigger();
						// will optimize Animation
						Animation(2, 0); // close
						_close = true;
					}
				}
				// toilet
				else if (_name == toilet) {
					if (_close) {
						SetTrigger();
						Animation(2, 1); // open
						_close = false;
					}
					else {
						SetTrigger();
						Animation(2, 0); // close
						_close = true;
					}
				}
			}
		}
		/*else
			TRACE("\n\nno press\n\n");*/
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
		bitmap.SetTopLeft(_pos_x, _pos_y);
		if (!_pick && _aniType != 1)
			bitmap.ShowBitmap();
		else if (!_pick && _aniType == 1)
			bitmap.ShowBitmap(_aniType);
	}
	bool Item::Collide() {
		CheckMoveDirection();
		return _move != none;
	}
	void Item::Animation(int n, int frame) {
		if (_triggered) {
			_aniType = n;
			if (n == 0 || n == 1) { //once
				bitmap.SetAnimation(_anidelay, true);
				_aniFrame = frame;
				if (n == 0) bitmap.ToggleAnimation();
				else bitmap.ToggleAnimationReverse();
			}
			else if (n == 2) { //select
				_aniFrame = frame;
				bitmap.SetFrameIndexOfBitmap(_aniFrame);
			}
			else if (n == 3) { //repeat
				bitmap.SetAnimation(_anidelay, false);
				_aniFrame = frame;
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
	bool Item::IsFixed() {
		return _fixed;
	}
	void Item::IsOnTriPos(bool a) {
		_onCorrectPos = a;
	}
	void Item::Reset() {
		_press = false;
		_triggered = false;
		_pick = false; // will disappear
		_close = true; // door, toilet, ...
		_fixed = false; // will not do anything if _fixed == true
		_take = false; // will leave
		_useItem = false; // will use screwdriver or lighter ...
	}
}