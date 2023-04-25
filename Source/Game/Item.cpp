#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "config.h"
#include <string>
#include "Entity.h"
#include "Item.h"
namespace game_framework {
	Item::Item() {
		SetXY(14 * TILE, 15 * TILE);
		_anidelay = 0;
		_boxX = 0;
		_boxY = 0;
		_move = none;
		_pressing = none;
		_aniType = -1;
		_aniFrame = -1;
		utiltriggers[2] = false;
		utiltriggers[0] = false;
		utiltriggers[3] = false; // will disappear
		utiltriggers[1] = true; // door, toilet, ...
		utiltriggers[4] = false; // will not do anything if _fixed == true
		utiltriggers[5] = false; // will leave
		utiltriggers[7] = false; // will use screwdriver or lighter ...
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
			bitmapName.push_back("img/item_animation/oil/oil.bmp");
		}
		else if (_name == handkerchief) {
			bitmapName.push_back("img/item_animation/handkerchief/handkerchief.bmp");
		}
		else if (_name == detergent) {
			for (int i = 0; i < 2; i++) { // actually only need detergent0.bmp
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
		else if (_name == tatami_l) {
			for (int i = 2; i > -1; i--) {
				bitmapName.push_back("img/item_animation/tatami/tatami" + to_string(i) + ".bmp");
			}
		}
		else if (_name == tatami_r) {
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
		else if (_name == bed) {	
			bitmapName.push_back("img/item_animation/bed/bed.bmp");
		}
		else if (_name == bookcase_l) {
			bitmapName.push_back("img/item_animation/bookcase/bookcase.bmp");
		}
		else if (_name == bookcase_r) {
			bitmapName.push_back("img/item_animation/bookcase/bookcase.bmp");
		}
		else if (_name == white_bookcase) {
			bitmapName.push_back("img/item_animation/bookcase/white_bookcase.bmp");
		}
		else if (_name == bookcase_map21) {
			bitmapName.push_back("img/item_animation/bookcase/bookcase_map21.bmp");
		}
		else if (_name == closet_shake) {
			for (int i = 0; i < 3; i++) {
				bitmapName.push_back("img/item_animation/closet/closet_shake_" + to_string(i) + ".bmp");
			}
		}
		else if (_name == closet_takesi_0) {
			for (int i = 0; i < 2; i++) {
				bitmapName.push_back("img/item_animation/closet/closet_takesi_open_" + to_string(i) + ".bmp");
			}
		}
		else if (_name == closet_takesi_1) {
			for (int i = 0; i < 2; i++) {
				bitmapName.push_back("img/item_animation/closet/closet_takesi_shake_" + to_string(i) + ".bmp");
			}
			SetTrigger();
			Animation(3, 0);
		}
		else if (_name == closet_hirosi_R) {
			for (int i = 0; i < 6; i++) {
				bitmapName.push_back("img/item_animation/closet/closet_hirosi_R_" + to_string(i) + ".bmp");
			}
		}
		else if (_name == closet_hirosi_L) {
			for (int i = 0; i < 6; i++) {
				bitmapName.push_back("img/item_animation/closet/closet_hirosi_L_" + to_string(i) + ".bmp");
			}
		}
		else if (_name == closet_mika_out) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/closet/closet_mika_" + to_string(i) + ".bmp");
			}
		}
		else if (_name == door_oni) {
			for (int i = 0; i < 16; i++) {
				bitmapName.push_back("img/item_animation/door_wash/door_oni_" + to_string(i) + ".bmp");
			}
		}
		else if (_name == door_open) {
			for (int i = 0; i < 3; i++) {
				bitmapName.push_back("img/item_animation/door_wash/door_open_" + to_string(i) + ".bmp");
			}
		}
		else if (_name == door_die) {
			for (int i = 0; i < 2; i++) {
				bitmapName.push_back("img/item_animation/door_wash/door_die_" + to_string(i) + ".bmp");
			}
		}
		else if (_name == door_half) {
			bitmapName.push_back("img/item_animation/door_wash/door_half.bmp");
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
		if (_name == gate) return _pos_y + _boxY;
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
		if (_playerX + TILE == _pos_x && (_playerY >= GetPosY() && _playerY <= y) && _pressing == isright)
			_move = isright;
		else if (_playerX - TILE == x && (_playerY >= GetPosY() && _playerY <= y) && _pressing == isleft)
			_move = isleft;
		else if ((_playerX >= _pos_x && _playerX <= x) && _playerY + TILE == GetPosY() && _pressing == isdown)
			_move = isdown;
		else if ((_playerX >= _pos_x && _playerX <= x) && _playerY - TILE == y && _pressing == isup)
			_move = isup;
		else
			_move = none;
		utiltriggers[2] = false;
	}
	void Item::OnMove() { // actually is action function
		if (utiltriggers[2]) {
			TRACE("\n\nitem press human X:%d item X:%d human Y:%d item Y:%d\n\n", _playerX, _pos_x, _playerY, _pos_y);
			if (Collide()) {
				TRACE("collide\n\n");
				//TRACE("\n\nitem press human X:%d item X:%d human Y:%d item Y:%d\n\n", _playerX, _pos_x, _playerY, _pos_y);
				// on lib
				if (_name == lib_book && !utiltriggers[4]) {
					SetXY(_pos_x, _pos_y - TILE);
					utiltriggers[4] = true;
				}
				// chair move to fixed pos
				else if (_name == key_lib && !utiltriggers[3]) {
					utiltriggers[3] = true;
				}
				// lib_book move to fixed pos
				else if (_name == key_3F_L && !utiltriggers[3]) {
					utiltriggers[3] = true;
				}
				// password box in 2F_D unlock
				else if (_name == key_2F_TL && !utiltriggers[3]) {
					utiltriggers[3] = true;
				}
				// in the jail room (near tatami)
				else if (_name == key_basement && !utiltriggers[3]) {
					utiltriggers[3] = true;
				}
				// basement onChair can pick
				else if (_name == key_jail && utiltriggers[6] && !utiltriggers[3]) {
					utiltriggers[3] = true;
				}
				// password box in basement1 unlock
				else if (_name == key_annexe && !utiltriggers[3]) {
					utiltriggers[3] = true;
				}
				// on kitchen
				else if (_name == broken_dish && !utiltriggers[5]) {
					utiltriggers[5] = true;
					SetTrigger();
					Animation(2, 1);
				}
				// on shower
				else if (_name == tub_once && !utiltriggers[4]) {
					SetTrigger();
					Animation(0, 0);
					utiltriggers[4] = true;
				}
				// in tub
				else if (_name == phillips && !utiltriggers[3]) {
					utiltriggers[3] = true;
				}
				// tub fixed only need to show, no interact
				// on basement (which has a chair), onChair can pick
				else if (_name == flathead && utiltriggers[6] && !utiltriggers[3]) {
					utiltriggers[3] = true;
				}
				// tatami leftside
				else if (_name == lighter && !utiltriggers[3]) {
					if (utiltriggers[1]) {
						SetTrigger();
						Animation(0, 0); // open
						utiltriggers[1] = false;
					}
					else {
						if (_playerX == _pos_x + _boxX && _playerY - TILE == _pos_y + _boxY) {
							utiltriggers[3] = true;
						}
						else {
							SetTrigger();
							Animation(1, 2); // close
							utiltriggers[1] = true;
						}
					}
				}
				// kid room, onchair can pick
				else if (_name == oil && utiltriggers[6] && !utiltriggers[3]) {
					utiltriggers[3] = true;
				}
				// 2F_TR, in lib take 3F key
				else if (_name == handkerchief && !utiltriggers[3]) {
					utiltriggers[3] = true;
				}
				// in toilet
				else if (_name == detergent && !utiltriggers[3]) {
					utiltriggers[3] = true;
				}
				// 3F, need to use screwdriver
				else if (_name == door_knob && !utiltriggers[3]) {
					if (utiltriggers[7])
						utiltriggers[3] = true;
					else {
						if (utiltriggers[1]) {
							SetTrigger();
							Animation(0, 1); // open
							utiltriggers[1] = false;
						}
						else {
							SetTrigger();
							Animation(1, 3); // close
							utiltriggers[1] = true;
						}
					}
				}
				// door_no_knob only need to show, no interact
				// tatami_l is tatami_r reverse(bitmap order)
				else if (_name == tatami_l) {
					if (utiltriggers[1]) {
						SetTrigger();
						Animation(0, 0); // close
						utiltriggers[1] = false;
					}
					else {
						SetTrigger();
						Animation(1, 2); // open
						utiltriggers[1] = true;
					}
				}
				// tatami
				else if (_name == tatami_r) {
					if (utiltriggers[1]) {
						SetTrigger();
						Animation(0, 0); // open
						utiltriggers[1] = false;
					}
					else {
						SetTrigger();
						Animation(1, 2); // close
						utiltriggers[1] = true;
					}
				}
				// jail
				else if (_name == gate) {
					if (utiltriggers[1]) {
						if (_playerX == _pos_x && 
							(_playerY - TILE == _pos_y + _boxY || 
								_playerY + TILE == _pos_y + _boxY)) {
							SetTrigger();
							Animation(2, 1); // open
							utiltriggers[1] = false;
						}
					}
					else {
						if (_playerX == _pos_x + _boxX && 
							(_playerY - TILE == _pos_y + _boxY || 
								_playerY + TILE == _pos_y + _boxY)) {
							SetTrigger();
							Animation(2, 0); // close
							utiltriggers[1] = true;
						}
					}
				}
				// toilet
				else if (_name == toilet) {
					if (utiltriggers[1]) {
						SetTrigger();
						Animation(2, 1); // open
						utiltriggers[1] = false;
					}
					else {
						SetTrigger();
						Animation(2, 0); // close
						utiltriggers[1] = true;
					}
				}
				// house1 3F L bed
				else if (_name == bed && !utiltriggers[4]) {
					if (_playerX == 10 * TILE) {
						utiltriggers[4] = true;
					}
				}
				// house1 basement1
				else if (_name == bookcase_l && !utiltriggers[4]) {
					if (_playerY == 7 * TILE) {
						utiltriggers[4] = true;
					}
				}
				// house1 basement1
				else if (_name == bookcase_r && !utiltriggers[4]) {
					if (_playerY == 7 * TILE) {
						utiltriggers[4] = true;
					}
				}
				// house1 2F DL
				else if (_name == white_bookcase && !utiltriggers[4]) {
					utiltriggers[4] = true;
				}
				// house1 room8 (map21)
				else if (_name == bookcase_map21 && !utiltriggers[4]) {
					utiltriggers[4] = true;
				}
				// house1 2F TR (once), event triggered
				else if (_name == closet_shake && !utiltriggers[4]) {
					SetTrigger();
					Animation(0, 0);
					utiltriggers[4] = true;
				}
				// house1 2F TR, takesi open (once)
				else if (_name == closet_takesi_0 && !utiltriggers[4]) {
					SetTrigger();
					Animation(0, 0);
					utiltriggers[4] = true;
				}
				// house1 2F TR, takesi shake
				// takesi shake should be use event unshow
				// house1 2F TR
				// house1 basement3 map2
				else if (_name == closet_hirosi_R || _name == closet_hirosi_L) {
					if (utiltriggers[1]) {
						SetTrigger();
						Animation(0, 1); // in
						utiltriggers[1] = false;
					}
					else {
						SetTrigger();
						Animation(1, 3); // out
						utiltriggers[1] = true;
					}
				}
				// house1 basement3 map2 (once), event triggered
				else if (_name == closet_mika_out) {
					SetTrigger();
					Animation(0, 0);
					utiltriggers[4] = true;
				}
				// house1 shower door
				else if (_name == door_oni || _name == door_open || _name == door_die && !utiltriggers[4]) {
					SetTrigger();
					Animation(0, 0);
					utiltriggers[4] = true;
				}
				// door_half only 1 frame, need event to control to show
			}
		}

		if (_name == bed || _name == white_bookcase) {
			if (utiltriggers[4] && !utiltriggers[6]) {
				TimerStart();
			}
			if (TimerGetCount() == 8) {
				TimerStop();
				utiltriggers[6] = true;
			}
			if (IsTimerStart()) {
				_pos_x -= 4;
				TimerUpdate(clock());
			}
		}
		else if (_name == bookcase_l || _name == bookcase_r) {
			if (utiltriggers[4] && !utiltriggers[6]) {
				TimerStart();
			}
			if (TimerGetCount() == 24) {
				TimerStop();
				utiltriggers[6] = true;
			}
			if (IsTimerStart()) {
				_pos_x -= 4;
				TimerUpdate(clock());
			}
		}
		else if (_name == bookcase_map21) {
			if (utiltriggers[4] && !utiltriggers[6]) {
				TimerStart();
			}
			if (TimerGetCount() == 16) {
				TimerStop();
				utiltriggers[6] = true;
			}
			if (IsTimerStart()) {
				_pos_x += 4;
				TimerUpdate(clock());
			}
		}
	}
	void Item::OnKeyDown(UINT nChar) {
		//TRACE("\n\niiiiiiiiii\n\n");
		if (nChar == VK_SPACE) {
			utiltriggers[2] = true;
			//TRACE("\n\npresssssss\n\n");
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
			utiltriggers[2] = false;
		}
	}
	void Item::OnShow() {
		bitmap.SetTopLeft(_pos_x, _pos_y);
		if (!utiltriggers[3] && _aniType != 1)
			bitmap.ShowBitmap();
		else if (!utiltriggers[3] && _aniType == 1)
			bitmap.ShowBitmap(_aniType);
	}
	bool Item::Collide() {
		CheckMoveDirection();
		return _move != none;
	}
	void Item::Animation(int n, int frame) {
		if (utiltriggers[0]) {
			_aniType = n;
			if (n == 0 || n == 1) { //once
				TRACE("\n\n%d\n", _anidelay);
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
			utiltriggers[0] = false;
		}
	}
	void Item::SetTrigger() {
		utiltriggers[0] = true;
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
	void Item::IsOnTriPos(bool a) {
		utiltriggers[6] = a;
	}
	void Item::Reset() {
		utiltriggers[2] = false;
		utiltriggers[0] = false;
		utiltriggers[3] = false; // will disappear
		utiltriggers[1] = true; // door, toilet, ...
		utiltriggers[4] = false; // will not do anything if _fixed == true
		utiltriggers[5] = false; // will leave
		utiltriggers[7] = false; // will use screwdriver or lighter ...
	}
	bool Item::IsPick() {
		return utiltriggers[3];
	}
	bool Item::IsFixed() {
		return utiltriggers[4];
	}
	bool Item::IsTake() {
		return utiltriggers[5];
	}
	bool Item::IsAnimationDone() {
		return bitmap.IsAnimationDone();
	}
	bool Item::IsClose() {
		return utiltriggers[1];
	}
	int Item::GetBitMapIndex() {
		return bitmap.GetFrameIndexOfBitmap();
	}
	void Item::EventTrigger() {
		// house1 2F TR (once), event triggered
		if (_name == closet_shake && !utiltriggers[4]) {
			SetTrigger();
			Animation(0, 0);
			utiltriggers[4] = true;
		}
		// house1 basement3 map2 (once), event triggered
		else if (_name == closet_mika_out && !utiltriggers[4]) {
			SetTrigger();
			Animation(0, 0);
			utiltriggers[4] = true;
		}
		// house1 shower door
		else if (_name == door_oni || _name == door_open || _name == door_die && !utiltriggers[4]) {
			SetTrigger();
			Animation(0, 0);
			utiltriggers[4] = true;
		}
	}
}