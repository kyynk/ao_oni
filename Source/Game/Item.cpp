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
		ResetUtil();
	}
	Item::~Item() = default;
	void Item::SetParam(int delay, int boxX, int boxY, 
		ItemName name) {
		_pressing = none;
		_anidelay = delay;
		_boxX = boxX;
		_boxY = boxY;
		_name = name;
		_aniType = -1;
		_aniFrame = -1;
		vector<string> bitmapName;
		if (_name == lib_book) {
			bitmapName.push_back("img/item_animation/lib_book/lib_book.bmp");
		}
		else if (_name == key_lib || _name == key_3F_L || _name == key_basement) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/star/star" + to_string(i) + ".bmp");
			}
			SetTrigger();
			Animation(3, 0);
		}
		else if (_name == key_2F_TL || _name == key_annexe) {
			for (int i = 0; i < 4; i++) {
				bitmapName.push_back("img/item_animation/password/pwd_key" + to_string(i) + ".bmp");
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
			bitmapName.push_back("img/item_animation/detergent/detergent0.bmp");
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
		else if (_name == bookcase_l || _name == bookcase_r) {
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
		else if (_name == password_not_open) {
			bitmapName.push_back("img/item_animation/password/pwd_default.bmp");
		}
		else if (_name == password_get_key) {
			bitmapName.push_back("img/item_animation/password/pwd_get_key.bmp");
		}
		else if (_name == piano_blood) {
			bitmapName.push_back("img/password/password_piano/piano_blood.bmp");
		}
		else if (_name == piano_pwd) {
			bitmapName.push_back("img/password/password_piano/piano_0.bmp");
		}
		else if (_name == piano_hint) {
			bitmapName.push_back("img/password/password_piano/piano_hint.bmp");
		}
		else if (_name == diff_door) {
			for (int i = 0; i < 3; i++) {
				bitmapName.push_back("img/door_need_knob/door_knob" + to_string(i) + ".bmp");
			}
		}
		else if (_name == kabe_pwd) {
			bitmapName.push_back("img/item_animation/password/kabe_pwd.bmp");
		}
		else if (_name == candle) {
			for (int i = 0; i < 2; i++) {
				bitmapName.push_back("img/item_animation/candle/candle" + to_string(i) + ".bmp");
			}
		}
		else if (_name == mika_to_oni) {
			for (int i = 0; i < 6; i++) {
				bitmapName.push_back("img/mika_oni/to_oni" + to_string(i) + ".bmp");
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
	int Item::GetPosY_offset() {
		return _pos_y + _boxY;
	}
	void Item::StorePlayerPos(int x, int y) {
		_playerX = x;
		_playerY = y;
	}
	void Item::OnMove() { // actually is action function
		if (utiltriggers[press] && Collide()) {
			//TRACE("\n\nitem press human X:%d item X:%d human Y:%d item Y:%d\n\n", _playerX, _pos_x, _playerY, _pos_y);
			// on lib
			if (_name == lib_book && !utiltriggers[fixed]) {
				SetXY(_pos_x, _pos_y - TILE);
				utiltriggers[fixed] = true;
			}
			// chair move to fixed pos
			else if (_name == key_lib && !utiltriggers[pick]) {
				utiltriggers[pick] = true;
			}
			// lib_book move to fixed pos
			else if (_name == key_3F_L && !utiltriggers[pick]) {
				utiltriggers[pick] = true;
			}
			// password box in 2F_D unlock
			else if (_name == key_2F_TL && !utiltriggers[pick]) {
				utiltriggers[pick] = true;
			}
			// in the jail room (near tatami)
			else if (_name == key_basement && !utiltriggers[pick]) {
				utiltriggers[pick] = true;
			}
			// basement onChair can pick
			else if (_name == key_jail && utiltriggers[onCorrectPos] && !utiltriggers[pick]) {
				utiltriggers[pick] = true;
			}
			// password box in basement1 unlock
			else if (_name == key_annexe && !utiltriggers[pick]) {
				utiltriggers[pick] = true;
			}
			// on kitchen
			else if (_name == broken_dish && !utiltriggers[take]) {
				utiltriggers[take] = true;
				SetTrigger();
				Animation(2, 1);
			}
			// on shower
			else if (_name == tub_once && !utiltriggers[fixed]) {
				SetTrigger();
				Animation(0, 0);
				utiltriggers[fixed] = true;
			}
			// in tub
			else if (_name == phillips && !utiltriggers[pick]) {
				utiltriggers[pick] = true;
			}
			// tub fixed only need to show, no interact
			// on basement (which has a chair), onChair can pick
			else if (_name == flathead && utiltriggers[onCorrectPos] && !utiltriggers[pick]) {
				utiltriggers[pick] = true;
			}
			// tatami leftside
			else if (_name == lighter && !utiltriggers[pick]) {
				if (utiltriggers[close]) {
					SetTrigger();
					Animation(0, 0); // open
					utiltriggers[close] = false;
				}
				else {
					if (_playerX == _pos_x + _boxX && _playerY - TILE == _pos_y + _boxY) {
						utiltriggers[pick] = true;
					}
					else {
						SetTrigger();
						Animation(1, 2); // close
						utiltriggers[close] = true;
					}
				}
			}
			// kid room, onchair can pick
			else if (_name == oil && utiltriggers[onCorrectPos] && !utiltriggers[pick]) {
				utiltriggers[pick] = true;
			}
			// 2F_TR, in lib take 3F key
			else if (_name == handkerchief && !utiltriggers[pick]) {
				utiltriggers[pick] = true;
			}
			// in toilet
			else if (_name == detergent && !utiltriggers[pick]) {
				utiltriggers[pick] = true;
			}
			// 3F, need to use screwdriver
			else if (_name == door_knob && !utiltriggers[pick]) {
				if (utiltriggers[useItem])
					utiltriggers[pick] = true;
				else {
					if (utiltriggers[close]) {
						SetTrigger();
						Animation(0, 1); // open
						utiltriggers[close] = false;
					}
					else {
						SetTrigger();
						Animation(1, 3); // close
						utiltriggers[close] = true;
					}
				}
			}
			else if (_name == password_not_open && !utiltriggers[pick]) {
				if (utiltriggers[close]) {
					utiltriggers[close] = false;
				}
				else {
					utiltriggers[close] = true;
				}
			}
			// door_no_knob only need to show, no interact
			// tatami_l is tatami_r reverse(bitmap order)
			else if (_name == tatami_l) {
				if (utiltriggers[close]) {
					SetTrigger();
					Animation(0, 0); // close
					utiltriggers[close] = false;
				}
				else {
					SetTrigger();
					Animation(1, 2); // open
					utiltriggers[close] = true;
				}
			}
			// tatami
			else if (_name == tatami_r) {
				if (utiltriggers[close]) {
					SetTrigger();
					Animation(0, 0); // open
					utiltriggers[close] = false;
				}
				else {
					SetTrigger();
					Animation(1, 2); // close
					utiltriggers[close] = true;
				}
			}
			// jail
			else if (_name == gate) {
				if (utiltriggers[close]) {
					if (_playerX == _pos_x && 
						(_playerY - TILE == _pos_y + _boxY || 
							_playerY + TILE == _pos_y + _boxY)) {
						SetTrigger();
						Animation(2, 1); // open
						utiltriggers[close] = false;
					}
				}
				else {
					if (_playerX == _pos_x + _boxX && 
						(_playerY - TILE == _pos_y + _boxY || 
							_playerY + TILE == _pos_y + _boxY)) {
						SetTrigger();
						Animation(2, 0); // close
						utiltriggers[close] = true;
					}
				}
			}
			// toilet
			else if (_name == toilet) {
				if (utiltriggers[close]) {
					SetTrigger();
					Animation(2, 1); // open
					utiltriggers[close] = false;
				}
				else {
					SetTrigger();
					Animation(2, 0); // close
					utiltriggers[close] = true;
				}
			}
			// house1 3F L bed
			else if (_name == bed && !utiltriggers[fixed]) {
				if (_playerX == 10 * TILE) {
					utiltriggers[fixed] = true;
				}
			}
			// house1 basement1
			else if (_name == bookcase_l && !utiltriggers[fixed]) {
				if (_playerY == 7 * TILE) {
					utiltriggers[fixed] = true;
				}
			}
			// house1 basement1
			else if (_name == bookcase_r && !utiltriggers[fixed]) {
				if (_playerY == 7 * TILE) {
					utiltriggers[fixed] = true;
				}
			}
			// house1 2F DL
			else if (_name == white_bookcase && !utiltriggers[fixed]) {
				utiltriggers[fixed] = true;
			}
			// house1 room8 (map21)
			else if (_name == bookcase_map21 && !utiltriggers[fixed]) {
				utiltriggers[fixed] = true;
			}
			// house1 2F TR (once), event triggered
			else if (_name == closet_shake && !utiltriggers[fixed]) {
				SetTrigger();
				Animation(0, 0);
				utiltriggers[fixed] = true;
			}
			// house1 2F TR, takesi open (once)
			else if (_name == closet_takesi_0 && !utiltriggers[fixed]) {
				SetTrigger();
				Animation(0, 0);
				utiltriggers[fixed] = true;
			}
			else if (_name == closet_takesi_0 && utiltriggers[fixed] && !utiltriggers[pick]) {
				utiltriggers[pick] = true;
			}
			
			// house1 2F TR, takesi shake
			// takesi shake should be use event unshow
			// house1 2F TR
			// house1 basement3 map2
			else if (_name == closet_hirosi_R || _name == closet_hirosi_L) {
				if (utiltriggers[close]) {
					SetTrigger();
					Animation(0, 1); // in
					utiltriggers[close] = false;
				}
				else {
					SetTrigger();
					Animation(1, 3); // out
					utiltriggers[close] = true;
				}
			}
			// house1 shower door
			else if ((_name == door_oni || _name == door_open || _name == door_die) && !utiltriggers[fixed]) {
				SetTrigger();
				Animation(0, 0);
				utiltriggers[fixed] = true;
			}
			// door_half only 1 frame, need event to control to show
			// piano
			else if (_name == password_not_open && !utiltriggers[pick]) {
				utiltriggers[close] = !utiltriggers[close];
			}
		}

		if (_name == bed || _name == white_bookcase) {
			if (utiltriggers[fixed] && !utiltriggers[onCorrectPos]) {
				TimerStart();
			}
			if (TimerGetCount() == 8) {
				TimerStop();
				utiltriggers[onCorrectPos] = true;
			}
			if (IsTimerStart()) {
				_pos_x -= 4;
				TimerUpdate();
			}
		}
		else if (_name == bookcase_l || _name == bookcase_r) {
			if (utiltriggers[fixed] && !utiltriggers[onCorrectPos]) {
				TimerStart();
			}
			if (TimerGetCount() == 24) {
				TimerStop();
				utiltriggers[onCorrectPos] = true;
			}
			if (IsTimerStart()) {
				_pos_x -= 4;
				TimerUpdate();
			}
		}
		else if (_name == bookcase_map21) {
			if (utiltriggers[fixed] && !utiltriggers[onCorrectPos]) {
				TimerStart();
			}
			if (TimerGetCount() == 16) {
				TimerStop();
				utiltriggers[onCorrectPos] = true;
			}
			if (IsTimerStart()) {
				_pos_x += 4;
				TimerUpdate();
			}
		}
	}
	void Item::OnKeyDown(UINT nChar) {
		if (nChar == VK_SPACE) {
			utiltriggers[press] = true;
		}
		if (nChar == VK_LEFT) {
			_pressing = left;
		}
		else if (nChar == VK_UP) {
			_pressing = up;
		}
		else if (nChar == VK_RIGHT) {
			_pressing = right;
		}
		else if (nChar == VK_DOWN) {
			_pressing = down;
		}
	}
	void Item::OnKeyUp(UINT nChar) {
		if (nChar == VK_SPACE) {
			utiltriggers[press] = false;
		}
	}
	void Item::OnShow() {
		bitmap.SetTopLeft(_pos_x, _pos_y);
		if (!utiltriggers[pick] && _aniType != 1)
			bitmap.ShowBitmap();
		else if (!utiltriggers[pick] && _aniType == 1)
			bitmap.ShowBitmap(_aniType);
	}
	bool Item::Collide() {
		int x = _pos_x + _boxX;
		int y = _pos_y + _boxY;
		if (_playerX + TILE == _pos_x && (_playerY >= GetPosY() && _playerY <= y) && _pressing == right)
			_pressing = right;
		else if (_playerX - TILE == x && (_playerY >= GetPosY() && _playerY <= y) && _pressing == left)
			_pressing = left;
		else if ((_playerX >= _pos_x && _playerX <= x) && _playerY + TILE == GetPosY() && _pressing == down)
			_pressing = down;
		else if	((_playerX >= _pos_x && _playerX <= x) && _playerY - TILE == y && _pressing == up) 
			_pressing = up;
		else
			_pressing = none;

		utiltriggers[press] = false;
		return _pressing != none;
	}
	void Item::SetDirection(int d) {
		if (d == up) {
			_pressing = up;
		}
		else if (d == left) {
			_pressing = left;
		}
		else if (d == right) {
			_pressing = right;
		}
		else if (d == down) {
			_pressing = down;
		}
		else {
			_pressing = none;
		}
	}
	void Item::Animation(int n, int frame) {
		if (utiltriggers[triggered]) {
			_aniType = n;
			if (n == 0 || n == 1) { //once
				//TRACE("\n\n%d\n", _anidelay);
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
			utiltriggers[triggered] = false;
		}
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
	void Item::ResetUtil() {
		for (int i = 0;i < 8;i++) {
			utiltriggers[i] = false;
		}
		utiltriggers[close] = true;

		if (bitmap.GetFrameSizeOfBitmap() != 0) {
			bitmap.SetFrameIndexOfBitmap(0);
			if (_name == key_lib || _name == key_3F_L || _name == key_2F_TL || _name == key_basement || _name == key_jail || _name == key_annexe || _name == phillips || _name == closet_takesi_1) {
				SetTrigger();
				Animation(3, 0);
			}
		}
	}
	bool Item::IsAnimationDone() {
		return bitmap.IsAnimationDone();
	}
	int Item::GetBitMapIndex() {
		return bitmap.GetFrameIndexOfBitmap();
	}
	void Item::EventTrigger() {
		// house1 2F TR (once), event triggered
		if (_name == closet_shake && !utiltriggers[fixed]) {
			SetTrigger();
			Animation(0, 0);
			utiltriggers[fixed] = true;
		}
		// house1 basement3 map2 (once), event triggered
		else if (_name == closet_mika_out && !utiltriggers[fixed]) {
			SetTrigger();
			Animation(0, 0);
			utiltriggers[fixed] = true;
		}
		// house1 shower door
		else if ((_name == door_oni || _name == door_open || _name == door_die) && !utiltriggers[fixed]) {
			SetTrigger();
			Animation(0, 0);
			utiltriggers[fixed] = true;
		}
		else if (_name == tatami_r && utiltriggers[close]) {
			SetTrigger();
			Animation(0, 0); // open
			utiltriggers[close] = false;
		}
		else if (_name == diff_door && _aniFrame < 2) {
			if (_aniFrame == -1) {
				_aniFrame = 0;
			}
			SetTrigger();
			Animation(2, _aniFrame + 1);
		}
		else if (_name == kabe_pwd && !utiltriggers[pick]) {
			utiltriggers[pick] = true;
		}
		else if (_name == candle && !utiltriggers[fixed]) {
			utiltriggers[fixed] = true;
			SetTrigger();
			Animation(2, 1);
		}
		else if (_name == closet_hirosi_R || _name == closet_hirosi_L) {
			if (utiltriggers[close]) {
				SetTrigger();
				Animation(0, 0); // in
				utiltriggers[close] = false;
			}
			else {
				SetTrigger();
				Animation(1, 5); // out
				utiltriggers[close] = true;
			}
		}
		else if (_name == mika_to_oni && !utiltriggers[fixed]) {
			utiltriggers[fixed] = true;
			SetTrigger();
			Animation(0, 0);
		}
		// house1 basement3 map2 (once), event triggered
		else if (_name == closet_mika_out && !utiltriggers[fixed]) {
			SetTrigger();
			Animation(0, 0);
			utiltriggers[fixed] = true;
		}
	}
}