#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "config.h"
#include <bitset>
#include <string.h>
#include <vector>
#include "Dialog.h"

namespace game_framework {
	Dialog::Dialog() {
		_now = none;
		_isClose = true;
	}
	Dialog::~Dialog() {
	}
	void Dialog::SetNow(character a) {
		_now = a;
		_name = { "", "Hirosi", "Mika", "Takesi", "Takurou" };
		if (_now == hirosi) {
			_head.LoadBitmapByString({ "img/conment/hirosi_default.bmp" }, RGB(204, 255, 0));
		}
		else if (_now == mika) {
			_head.LoadBitmapByString({ "img/conment/mika_default.bmp" }, RGB(204, 255, 0));
		}
		else if (_now == takesi) {
			_head.LoadBitmapByString({ "img/conment/takesi_default.bmp" }, RGB(204, 255, 0));
		}
		else if (_now == takurou) {
			_head.LoadBitmapByString({ "img/conment/takurou_default.bmp" }, RGB(204, 255, 0));
		}
		else {
			_head.LoadBitmapByString({ "img/conment/none.bmp" }, RGB(204, 255, 0));
		}
		
		_box.LoadBitmapByString({ "img/conment/box.bmp" }, RGB(204, 255, 0));
		_cursor.LoadBitmapByString({ "img/cursor/tri2_1.bmp", "img/cursor/tri2_2.bmp" }, RGB(0, 0, 0));
		_nameBox.LoadBitmapByString({ "img/conment/name_box.bmp" }, RGB(204, 255, 0));
	}
	void Dialog::SetParam(int _posX, int _posY,
		int linespacing, vector<string>  st) {
		_cursorX = _posX + 448 - 16; // 320 = width of box
		_cursorY = _posY + 128 - 40; // 128 = height of box
		_boxX = _posX;
		_boxY = _posY;
		if (_now != none)
			_txtX = _posX + 126;
		else
			_txtX = _posX + 16;
		_txtY = _posY + 10;
		_headX = _posX + 10; //Head is 100x100 pixel
		_headY = _posY + 10;
		_nameX = _posX +10;
		_nameY = _posY - 42 + 10; // 48 = nameBox height
		_nBoxX = _posX;
		_nBoxY = _posY - 42; // 48 = nameBox height
		_lineSpacing = linespacing; //font size
		_store = st;
	}
	void Dialog::ShowBox() {
		_box.SetTopLeft(_boxX, _boxY);
		_box.ShowBitmap();
	}
	void Dialog::ShowHead() {
		_head.SetTopLeft(_headX, _headY);
		_head.ShowBitmap();
	}
	void Dialog::ShowCursor() {
		_cursor.SetTopLeft(_cursorX, _cursorY + _lineSpacing);
		_cursor.SetAnimation(200, false);
		_cursor.ShowBitmap();
	}
	void Dialog::ShowText() {
		CDC* pDC = CDDraw::GetBackCDC();
		CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(255, 255, 255));
		for (int i = 0; i < int(_store.size()); i++) {
			CTextDraw::Print(pDC, _txtX, _txtY + i * _lineSpacing, _store.at(i));
		}
		CDDraw::ReleaseBackCDC();
	}
	void Dialog::ShowNameBox() {
		_nameBox.SetTopLeft(_nBoxX, _nBoxY);
		_nameBox.ShowBitmap();
	}
	void Dialog::ShowName() {
		CDC* pDC = CDDraw::GetBackCDC();
		CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(152, 245, 255));
		CTextDraw::Print(pDC, _nameX, _nameY, _name.at(_now));
		CDDraw::ReleaseBackCDC();
	}
	void Dialog::ShowTotal() {
		ShowBox();
		ShowHead();
		ShowText();
		if (_now != none) {
			ShowNameBox();
			ShowName();
		}
		ShowCursor();
	}
	bool Dialog::isClose() {
		return _isClose;
	}
	void Dialog::Show() {
		_isClose = false;
	}
	void Dialog::Close() {
		_isClose = true;
	}
}