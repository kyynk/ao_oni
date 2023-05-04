#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "ChoiceMenu.h"
#include "Dialog.h"

namespace game_framework {
	Dialog::Dialog() = default;
	Dialog::~Dialog() = default;
	void Dialog::SetFigure(string&& name) {
		_figurename = name;
		_head.LoadBitmapByString({ "dialog/"+name+".bmp" }, RGB(204, 255, 0));
		
		
		if (name != "none")
			_box.LoadBitmapByString({ "dialog/box.bmp" }, RGB(204, 255, 0));
		else
			_box.LoadBitmapByString({ "dialog/box2.bmp" }, RGB(204, 255, 0));
		_cursor.LoadBitmapByString({ "img/cursor/tri2_1.bmp", "img/cursor/tri2_2.bmp" }, RGB(0, 0, 0));
		_nameBox.LoadBitmapByString({ "dialog/name_box.bmp" }, RGB(204, 255, 0));
		_choicemenu.Load({ "img/cursor/tri1_1.bmp","img/cursor/tri1_2.bmp",
			"img/cursor/tri1_3.bmp","img/cursor/tri1_2.bmp" }, { "dialog/none.bmp" },
			RGB(0, 0, 0), RGB(204, 255, 0));
	}
	void Dialog::SetParam(vector<string>  st, bool ch) {
		_isShow = false;
		int _posX, _posY;
		_posX = _posY = 0;
		if (_figurename != "none") {
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
	}
	void Dialog::SetOption(string str1,string str2) {
		if (_isChoose)
			_choicemenu.SetParam(_txtX, _txtY + _store.size() * _lineSpacing,
				0, 0, _txtX - 5, _txtY + _store.size() * _lineSpacing,
				45, { str1,str2 });
		_choice = undefined;
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
		if (_figurename != "") {
			CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(152, 245, 255));
			CTextDraw::Print(pDC, _nameX, _nameY, _figurename);
		}
		if (_isChoose) {
			CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(255, 255, 255));
			_choicemenu.ShowText(pDC);
		}
	}
	void Dialog::ShowNameBox() {
		_nameBox.SetTopLeft(_nBoxX, _nBoxY);
		_nameBox.ShowBitmap();
	}
	void Dialog::ShowTotal() {
		ShowBox();
		if (_figurename != "none") {
			ShowHead();
			ShowNameBox();
		}
		if (!_isChoose) { 
			ShowCursor();
		}
		else {
		_choicemenu.ShowCursor();
		}
		CDC *pDC = CDDraw::GetBackCDC();
		ShowText(pDC);
		if (_isChoose)
			_choicemenu.ShowText(pDC);
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
		_choicemenu.OnMovingCursor(nChar);
		if (nChar == VK_SPACE) {
			if (_choicemenu.GetSelection() == 0) {
				_choice = yes;
			}
			else{
				_choice = no;
			}
		}
	}
}