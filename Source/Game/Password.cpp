#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <string>
#include "Password.h"

namespace game_framework {
	Password::Password() = default;
	Password::~Password() = default;
	
	void Password::SetParam(string pwd) {
		_pwd = pwd;
		int _posX, _posY;
		_posX = 144;
		_posY = 480;
		_cursorX = _posX + 17; // 544 = width of box2 
		_cursorY = _posY + 9; // 192 = height of box2
		
		_boxX = _posX; // box and box2 is different
		_boxY = _posY;

		_txtX = _posX + 16;
		_txtY = _posY + 10;
		_lineSpacing = 21;

		_nowChoose = 0;
		_guess.resize(4);
		_guess[0] = _guess[1] = _guess[2] = _guess[3] = 0;
		
		_isShow = false;
		_open = false;
		_store = "";
		for (int i = 0; i < 4; i++) {
			_store += to_string(_guess[i]);
		}

		_cursor.LoadBitmapByString({ "img/cursor/tri1_1.bmp","img/cursor/tri1_2.bmp",
			"img/cursor/tri1_3.bmp","img/cursor/tri1_2.bmp" }, RGB(0, 0, 0));
		_box.LoadBitmapByString({ "img/password/box.bmp" }, RGB(204, 255, 0));
	}
	void Password::ShowBox() {
		_box.SetTopLeft(_boxX, _boxY);
		_box.ShowBitmap();
	}
	void Password::ShowCursor() {
		_cursor.SetTopLeft(_cursorX + _nowChoose * _lineSpacing, _cursorY);
		_cursor.SetAnimation(200, false);
		_cursor.ShowBitmap();
	}
	void Password::ShowText(CDC* pDC) {
		_store = "";
		for (int i = 0; i < 4; i++) {
			_store += " " + to_string(_guess[i]);
		}
		CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(255, 255, 255));
		CTextDraw::Print(pDC, _txtX, _txtY, _store);
	}
	void Password::ShowTotal() {
		ShowBox();
		ShowCursor();
		CDC* pDC = CDDraw::GetBackCDC();
		ShowText(pDC);
		CDDraw::ReleaseBackCDC();
	}
	void Password::SetShow(bool show) {
		_isShow = show;
		_guess[0] = _guess[1] = _guess[2] = _guess[3] = 0;
		_nowChoose = 0;
	}
	void Password::OnKeyDown(UINT nChar) {
		if (nChar == VK_RETURN) {
			if (IsCorrect()) {
				_open = true;
				SetShow(false);
			}
			else {
				SetShow(false);
			}
		}
		else if (nChar == VK_UP) {
			if (_guess[_nowChoose] == 9) {
				_guess[_nowChoose] = 0;
			}
			else {
				_guess[_nowChoose]++;
			}
		}
		else if (nChar == VK_DOWN) {
			if (_guess[_nowChoose] == 0) {
				_guess[_nowChoose] = 9;
			}
			else {
				_guess[_nowChoose]--;
			}
		}
		else if (nChar == VK_LEFT) {
			if (_nowChoose == 0) {
				_nowChoose = 3;
			}
			else {
				_nowChoose--;
			}
		}
		else if (nChar == VK_RIGHT) {
			if (_nowChoose == 3) {
				_nowChoose = 0;
			}
			else {
				_nowChoose++;
			}
		}
	}
	bool Password::IsShow() {
		return _isShow;
	}
	bool Password::IsCorrect() {
		_store = "";
		for (int i = 0; i < 4; i++) {
			_store += to_string(_guess[i]);
		}
		return _store == _pwd;
	}
	bool Password::IsOpen() {
		return _open;
	}
}