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
#include "ChoiceMenu.h"
#include "Dialog.h"

namespace game_framework {
	Dialog::Dialog() {
		_now = none;
		_cursorX, _cursorY, _boxX, _boxY,
		_txtX, _txtY, _headX, _headY,
		_nameX, _nameY, _nBoxX, _nBoxY,
		_lineSpacing = 0;
		_isShow = false;
		_isChoose = false;
		
	}
	Dialog::~Dialog() {
	}
	void Dialog::SetNow(character a) {
		_now = a;
		_name = { "", "Hirosi", "Mika", "Takesi", "Takurou" };
		if (_now == hirosi) {
			_head.LoadBitmapByString({ "img/dialog/hirosi_default.bmp" }, RGB(204, 255, 0));
		}
		else if (_now == mika) {
			_head.LoadBitmapByString({ "img/dialog/mika_default.bmp" }, RGB(204, 255, 0));
		}
		else if (_now == takesi) {
			_head.LoadBitmapByString({ "img/dialog/takesi_default.bmp" }, RGB(204, 255, 0));
		}
		else if (_now == takurou) {
			_head.LoadBitmapByString({ "img/dialog/takurou_default.bmp" }, RGB(204, 255, 0));
		}
		else {
			_head.LoadBitmapByString({ "img/dialog/none.bmp" }, RGB(204, 255, 0));
		}
		if (_now != none)
			_box.LoadBitmapByString({ "img/dialog/box.bmp" }, RGB(204, 255, 0));
		else
			_box.LoadBitmapByString({ "img/dialog/box2.bmp" }, RGB(204, 255, 0));
		_cursor.LoadBitmapByString({ "img/cursor/tri2_1.bmp", "img/cursor/tri2_2.bmp" }, RGB(0, 0, 0));
		_nameBox.LoadBitmapByString({ "img/dialog/name_box.bmp" }, RGB(204, 255, 0));
		_choice.Load({ "img/cursor/tri1_1.bmp","img/cursor/tri1_2.bmp",
			"img/cursor/tri1_3.bmp","img/cursor/tri1_2.bmp" }, { "img/dialog/none.bmp" },
			RGB(0, 0, 0), RGB(204, 255, 0));
	}
	void Dialog::SetParam(vector<string>  st, bool ch) {
		int _posX, _posY;
		_posX, _posY = 0;
		if (_now != none) {
			_posX = 64;
			_posY = 608;
			_cursorX = _posX + 672 - 16; // 672 = width of box 
			_cursorY = _posY + 128 - 48; // 128 = height of box
			_txtX = _posX + 126; // have Head
			_headX = _posX + 10; // Head is 100x100 pixel
			_headY = _posY + 10;
			_nameX = _posX + 10; // boxX + 10
			_nameY = _posY - 42 + 10; // 48 = nameBox height, 42 is overlap to box
			_nBoxX = _posX;
			_nBoxY = _posY - 42; // 48 = nameBox height

		}
		else {
			_posX = 144;
			_posY = 288;
			_cursorX = _posX + 544 - 16; // 544 = width of box2 
			_cursorY = _posY + 192 - 48; // 192 = height of box2
			_txtX = _posX + 16;
		}
		_boxX = _posX; // box and box2 is different
		_boxY = _posY;
		_txtY = _posY + 10;
		_lineSpacing = 35; //font gap
		_store = st;
		_isChoose = ch;
		if (_isChoose)
			_choice.SetParam(_txtX, _txtY + _store.size() * _lineSpacing,
				0, 0, _txtX - 5, _txtY + _store.size() * _lineSpacing,
				45, { "Yes","No" });
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
	void Dialog::ShowText(CDC *pDC) {
		CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(255, 255, 255));
		for (int i = 0; i < int(_store.size()); i++) {
			CTextDraw::Print(pDC, _txtX, _txtY + i * _lineSpacing, _store.at(i));
		}
		if (_now != none) {
			CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(152, 245, 255));
			CTextDraw::Print(pDC, _nameX, _nameY, _name.at(_now));
		}
		if (_isChoose) {
			CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(255, 255, 255));
			_choice.ShowText(pDC);
		}
	}
	void Dialog::ShowNameBox() {
		_nameBox.SetTopLeft(_nBoxX, _nBoxY);
		_nameBox.ShowBitmap();
	}
	void Dialog::ShowTotal() {
		ShowBox();
		if (_now != none) {
			ShowHead();
			ShowNameBox();
		}
		if (!_isChoose) { 
			ShowCursor();
		}
		else {
			_choice.ShowCursor();
		}
		CDC *pDC = CDDraw::GetBackCDC();
		ShowText(pDC);
		if (_isChoose)
			_choice.ShowText(pDC);
		CDDraw::ReleaseBackCDC();
	}
	bool Dialog::isShow() {
		return _isShow;
	}
	void Dialog::SetShow(bool close)
	{
		_isShow = close;
	}
	bool Dialog::isChoose() {
		return _isChoose;
	}
	void Dialog::GetSelect(UINT nChar) {
		_choice.OnMovingCursor(nChar);
		if (nChar == VK_RETURN) {
			switch (_choice.GetSelection()) {
			case 0:
				SetShow(false);
				// will be done in future
				break;
			case 1:
				SetShow(false);
				// will be done in future
				break;
			}
		}
	}
}