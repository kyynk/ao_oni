#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "Interface.h"
namespace game_framework {
	Interface::Interface() = default;
	Interface::~Interface() = default;
	void Interface::init() {
		_boxX = 64;
		_boxY = 160;
		_lineSpacing = 32;
		_initChoose = _itemChoose = _saveChoose = _endChoose = 0;
		_step = 0;
		_isPause = false;
		_isStop = true;
		_cursor.LoadBitmapByString({ "img/cursor/tri1_1.bmp","img/cursor/tri1_2.bmp",
			"img/cursor/tri1_3.bmp","img/cursor/tri1_2.bmp" }, RGB(0, 0, 0));
		_init.LoadBitmapByString({ "img/interface/status.bmp" }, RGB(204, 255, 0));
		_item.LoadBitmapByString({ "img/interface/item.bmp" }, RGB(204, 255, 0));
		_save.LoadBitmapByString({ "img/interface/save.bmp" }, RGB(204, 255, 0));
		_end.LoadBitmapByString({ "img/interface/end.bmp" }, RGB(204, 255, 0));
	}
	// game time counter
	bool Interface::IsPause() const {
		return _isPause;
	}
	bool Interface::IsStop() const {
		return _isStop;
	}
	void Interface::StartCount() {
		if (_isStop) {
			start_time = time(0);
			_isStop = false;
		}
		else if (_isPause) {
			_isPause = false;
			start_time += time(0) - pause_time;
		}
	}
	void Interface::PauseCount() {
		if (_isStop || _isPause) 
			return;
		else {
			_isPause = true;
			pause_time = time(0);
		}
	}
	void Interface::StopCount() {
		if (_isStop)
			return;
		else if (_isPause) {
			_isPause = false;
			_isStop = true;
		}
		else if (!_isStop) {
			_isStop = true;
		}
	}
	time_t Interface::GetStartTime() {
		return start_time;
	}
	string Interface::GetGameTime() {
		time_t t = time(0) - start_time;
		stringstream hour, min, sec;
		sec << setw(2) << setfill('0') << t % 60;
		t = t / 60;
		min << setw(2) << setfill('0') << t % 60;
		t = t / 60;
		hour << setw(2) << setfill('0') << t;
		string str_time = "";
		str_time = hour.str() + ":" + min.str() + ":" + sec.str();
		return str_time;
	}

	// game time counter end
	string Interface::GetRealTime() {
		time_t now = time(0);
		tm* ltm = localtime(&now);
		string real_time = "";
		real_time = std::to_string(1900 + ltm->tm_year) + "/" +
			std::to_string(1 + ltm->tm_mon) + "/" +
			std::to_string(ltm->tm_mday) + " " +
			std::to_string(ltm->tm_hour) + ":" +
			std::to_string(ltm->tm_min);
		return real_time;
	}
	void Interface::StorePlayerStep(int step) {
		_step = step;
	}
	void Interface::StoreItem(string item_name) {
		items.push_back(item_name);
	}
	void Interface::ShowCursorInit() {
		// set topleft will change, now is try
		_cursorX = _boxX + 16;
		_cursorY = _boxY + 16;
		_cursor.SetTopLeft(_cursorX, _cursorY + _initChoose * _lineSpacing);
		_cursor.SetAnimation(200, false);
		_cursor.ShowBitmap();
	}
	void Interface::ShowCursorItem() {
		// set topleft will change, now is try
		/*_cursor.SetTopLeft(_cursorX, _cursorY);
		_cursor.SetAnimation(200, false);
		_cursor.ShowBitmap();*/
	}
	void Interface::ShowCursorSave() {
		// set topleft will change, now is try
		/*_cursor.SetTopLeft(_cursorX, _cursorY);
		_cursor.SetAnimation(200, false);
		_cursor.ShowBitmap();*/
	}
	void Interface::ShowCursorEnd() {
		// set topleft will change, now is try
		_cursorX = _boxX + 240;
		_cursorY = _boxY + 176;
		_cursor.SetTopLeft(_cursorX, _cursorY + _initChoose * _lineSpacing);
		_cursor.SetAnimation(200, false);
		_cursor.ShowBitmap();
	}
	void Interface::ShowTextInit(CDC* pDC) {
		vector<string> vec_str;
		vec_str = { "Item", "Save", "End", "Game time", "Step", "$", "    0", 
			"Hirosi          Student",
			"LV   1  [normal]       HP    1/1",
			"E           0/26       SP    1/1" };
		CTextDraw::ChangeFontLog(pDC, 16, "Consolas", RGB(255, 255, 255));
		for (int i = 0; i < 3; i++) {
			CTextDraw::Print(pDC, 88, 180 + i * _lineSpacing, vec_str.at(i));
		}
		CTextDraw::ChangeFontLog(pDC, 16, "Consolas", RGB(102, 255, 230));
		CTextDraw::Print(pDC, 88, 404, vec_str.at(3));
		CTextDraw::ChangeFontLog(pDC, 16, "Consolas", RGB(255, 255, 255));
		CTextDraw::Print(pDC, 88, 404 + _lineSpacing, GetRealTime().c_str());
		CTextDraw::ChangeFontLog(pDC, 16, "Consolas", RGB(102, 255, 230));
		CTextDraw::Print(pDC, 88, 500, vec_str.at(4));
		CTextDraw::ChangeFontLog(pDC, 16, "Consolas", RGB(255, 255, 255));
		CTextDraw::Print(pDC, 88, 500 + _lineSpacing, to_string(_step));
		CTextDraw::ChangeFontLog(pDC, 16, "Consolas", RGB(102, 255, 230));
		CTextDraw::Print(pDC, 88, 600, vec_str.at(5));
		CTextDraw::ChangeFontLog(pDC, 16, "Consolas", RGB(255, 255, 255));
		CTextDraw::Print(pDC, 88, 600, vec_str.at(6));

		CTextDraw::Print(pDC, 344, 180 + 0 * _lineSpacing, vec_str.at(7));
		CTextDraw::Print(pDC, 344, 180 + 1 * _lineSpacing, vec_str.at(8));
		CTextDraw::Print(pDC, 344, 180 + 2 * _lineSpacing, vec_str.at(9));
	}
	void Interface::ShowTextItem(CDC* pDC) {

	}
	void Interface::ShowTextSave(CDC* pDC) {

	}
	void Interface::ShowTextEnd(CDC* pDC) {

	}
	void Interface::ShowTotal() {
		_init.SetTopLeft(_boxX, _boxY);
		_init.ShowBitmap();
		ShowCursorInit();
		CDC* pDC = CDDraw::GetBackCDC();
		ShowTextInit(pDC);
		CDDraw::ReleaseBackCDC();
	}
	void Interface::OnKeyDown(UINT nChar) {
		if (nChar == VK_UP) {
			if (_initChoose == 0) {
				_initChoose = 2;
			}
			else {
				_initChoose++;
			}
			if (_endChoose == 0) {
				_endChoose = 2;
			}
			else {
				_endChoose++;
			}
		}
		else if (nChar == VK_DOWN) {
			if (_initChoose == 2) {
				_initChoose = 0;
			}
			else {
				_initChoose--;
			}
			if (_endChoose == 2) {
				_endChoose = 0;
			}
			else {
				_endChoose--;
			}
		}
		//esc
	}
}