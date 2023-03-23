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
#include "Conment.h"

namespace game_framework {
	Conment::Conment() {
		_now = none;
	}
	Conment::~Conment() {
	}
	void Conment::SetNow(character a) {
		_now = a;
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
			_head.LoadBitmapByString({ "img/conment/takukrou_default.bmp" }, RGB(204, 255, 0));
		}
		else {
			_head.LoadBitmapByString({ "img/conment/none.bmp" }, RGB(204, 255, 0));
		}
		
		_box.LoadBitmapByString({ "img/conment/box.bmp" }, RGB(204, 255, 0));

		_cursor.LoadBitmapByString({ "img/cursor/tri2_1.bmp", "img/cursor/tri2_2.bmp" }, RGB(0, 0, 0));
	}
	void Conment::SetParam(int _posX, int _posY, 
		int linespacing, vector<string>  st) {
		_cursorX = _posX + 320 - 32; // 320 = width of box
		_cursorY = _posY + 128 - 32; // 128 = height of box
		_boxX = _posX;
		_boxY = _posY;
		_txtX = _posX + 142;
		_txtY = _posY + 16;
		_headX = _posX + 16; //Head is 100x100 pixel
		_headY = _posY + 16;
		_lineSpacing = linespacing; //font size
		_store = st;
	}
	void Conment::ShowBox() {
		_box.SetTopLeft(_boxX, _boxY);
		_box.ShowBitmap();
	}
	void Conment::ShowHead() {
		_head.SetTopLeft(_headX, _headY);
		_head.ShowBitmap();
	}
	void Conment::ShowCursor() {
		_cursor.SetTopLeft(_cursorX, _cursorY + _lineSpacing);
		_cursor.SetAnimation(200, false);
		_cursor.ShowBitmap();
	}
	void Conment::ShowText(CDC* pDC) {
		for (int i = 0; i < int(_store.size()); i++) {
			CTextDraw::Print(pDC, _txtX, _txtY + i * _lineSpacing, _store.at(i));
		}
	}
}