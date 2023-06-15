#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "ChoiceMenu.h"
#include "Dialog.h"
#include "InterfaceData.h"
#include "Interface.h"
namespace game_framework {
	Interface::Interface() = default;
	Interface::~Interface() = default;
	void Interface::init() {
		_boxX = 64;
		_boxY = 160;
		_lineSpacing = 32;
		_statusChoose = _itemChoose = _saveChoose = _endChoose = 0;
		_step = 0;
		_show = none;
		_useItemIndex = -1;
		_isShow = false;
		_IsGoTitle = false;
		_IsEndGame = false;
		_isPause = false;
		_isStop = true;
		_IsUseItem = false;
		_IsShowMapAndDialog = false;
		_itemsImg.resize(14);
		for (int i = 0; i < 6; i++) { //key
			_itemsImg.at(i).LoadBitmapByString({ "img/interface/item/key.bmp" }, RGB(204, 255, 0));
		}
		_itemsImg.at(broken_dish).LoadBitmapByString({ "img/interface/item/broken_dish.bmp" }, RGB(204, 255, 0));
		_itemsImg.at(phillips).LoadBitmapByString({ "img/interface/item/driver.bmp", "img/interface/item/sin.bmp" }, RGB(204, 255, 0));
		_itemsImg.at(flathead).LoadBitmapByString({ "img/interface/item/sin.bmp", "img/interface/item/driver.bmp" }, RGB(204, 255, 0));
		_itemsImg.at(lighter).LoadBitmapByString({ "img/interface/item/lighter.bmp" }, RGB(204, 255, 0));
		_itemsImg.at(oil).LoadBitmapByString({ "img/interface/item/oil.bmp" }, RGB(204, 255, 0));
		_itemsImg.at(handkerchief).LoadBitmapByString({ "img/interface/item/handkerchief.bmp", "img/interface/item/clean_handkerchief.bmp" }, RGB(204, 255, 0));
		_itemsImg.at(detergent).LoadBitmapByString({ "img/interface/item/detergent.bmp" }, RGB(204, 255, 0));
		_itemsImg.at(door_knob).LoadBitmapByString({ "img/interface/item/doorknob.bmp" }, RGB(204, 255, 0));

		_cursor.LoadBitmapByString({ "img/cursor/tri1_1.bmp","img/cursor/tri1_2.bmp",
			"img/cursor/tri1_3.bmp","img/cursor/tri1_2.bmp" }, RGB(0, 0, 0));
		_status.LoadBitmapByString({ "img/interface/status.bmp" }, RGB(204, 255, 0));
		_item.LoadBitmapByString({ "img/interface/item.bmp" }, RGB(204, 255, 0));
		_save.LoadBitmapByString({ "img/interface/save.bmp" }, RGB(204, 255, 0));
		_end.LoadBitmapByString({ "img/interface/end.bmp" }, RGB(204, 255, 0));
		_useItemDialog.SetFigure("hirosi");
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
	time_t Interface::GetStartTime() const {
		return start_time;
	}
	string Interface::GetGameTime() {
		// if pause time
		// time_t t = time(0) - start_time - (time(0) - pause_time);
		// no pause time
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
	void Interface::StoreItem(string intro, string name, Items item) {
		_itemsIntro.push_back(intro);
		_itemsName.push_back(name);
		_itemsImgShowIndex.push_back(item);
	}
	InterfaceData Interface::UseItem() {
		_IsUseItem = false;
		
		return InterfaceData(_itemsIntro.at(_useItemIndex), _itemsName.at(_useItemIndex));
	}
	void Interface::ChangeItemStatusByName(string originalName, string intro, string name, int frame_index) {
		int change_index = -1;
		for (int i = 0; i < int(_itemsName.size()); i++) {
			if (_itemsName.at(i) == originalName) {
				change_index = i;
				break;
			}
		}
		if (change_index != -1) {
			_itemsIntro.at(change_index) = intro;
			_itemsName.at(change_index) = name;
			_itemsImg.at(_itemsImgShowIndex.at(change_index)).SetFrameIndexOfBitmap(frame_index);
		}
	}
	void Interface::ChangeItemStatusByIntro(string originalIntro, string intro, string name, int frame_index) {
		int change_index = -1;
		for (int i = 0; i < int(_itemsIntro.size()); i++) {
			if (_itemsIntro.at(i) == originalIntro) {
				change_index = i;
				break;
			}
		}
		if (change_index != -1) {
			_itemsIntro.at(change_index) = intro;
			_itemsName.at(change_index) = name;
			_itemsImg.at(_itemsImgShowIndex.at(change_index)).SetFrameIndexOfBitmap(frame_index);
		}
	}
	void Interface::DeleteItem(string name) {
		int del_index = -1;
		for (int i = 0; i < int(_itemsName.size()); i++) {
			if (_itemsName.at(i) == name) {
				del_index = i;
				break;
			}
		}
		if (del_index != -1) {
			_itemsIntro.erase(_itemsIntro.begin() + del_index);
			_itemsName.erase(_itemsName.begin() + del_index);
			_itemsImgShowIndex.erase(_itemsImgShowIndex.begin() + del_index);
		}
	}
	bool Interface::FindItem(string name) {
		auto find_it = std::find(_itemsName.begin(), _itemsName.end(), name);
		if (find_it != _itemsName.end()) {
			return true;
		}
		else {
			return false;
		}
	}
	void Interface::ResetItem() {
		_itemsName.clear();
		_itemsIntro.clear();
		_itemsImgShowIndex.clear();
	}
	void Interface::ShowCursorStatus() {
		_cursorX = _boxX + 16;
		_cursorY = _boxY + 16;
		_cursor.SetTopLeft(_cursorX, _cursorY + _statusChoose * _lineSpacing);
		_cursor.SetAnimation(200, false);
		_cursor.ShowBitmap();
	}
	void Interface::ShowCursorItem() {
		_cursorX = _boxX + 16; // item img => 24x24
		_cursorY = _boxY + 3 * 32 - 4;
		_cursor.SetTopLeft(_cursorX + (_itemChoose % 2) * 11 * 32, _cursorY + (_itemChoose / 2) * _lineSpacing);
		_cursor.SetAnimation(200, false);
		_cursor.ShowBitmap();
	}
	void Interface::ShowCursorSave() {
		// set topleft will change, now is try
		/*_cursor.SetTopLeft(_cursorX, _cursorY);
		_cursor.SetAnimation(200, false);
		_cursor.ShowBitmap();*/
	}
	void Interface::ShowCursorEnd() {
		_cursorX = _boxX + 8 * 32 + 16;
		_cursorY = _boxY + 6 * 32 - 4;
		_cursor.SetTopLeft(_cursorX, _cursorY + _endChoose * _lineSpacing);
		_cursor.SetAnimation(200, false);
		_cursor.ShowBitmap();
	}
	void Interface::ShowTextStatus(CDC* pDC) {
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
		CTextDraw::Print(pDC, 88, 404 + _lineSpacing, GetGameTime().c_str());
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
		if (int(_itemsIntro.size()) != 0) {
			CTextDraw::ChangeFontLog(pDC, 16, "Consolas", RGB(255, 255, 255));
			CTextDraw::Print(pDC, _boxX + 16, _boxY + 20, _itemsIntro.at(_itemChoose));
			for (int i = 0; i < int(_itemsName.size()); i++) {
				CTextDraw::ChangeFontLog(pDC, 16, "Consolas", RGB(255, 255, 255));
				CTextDraw::Print(pDC, _boxX + 16 + 2 * 32 + (i % 2) * 11 * 32, _boxY + 3 * 32 + (i / 2) * _lineSpacing, _itemsName.at(i));
			}
		}
	}
	void Interface::ShowTextSave(CDC* pDC) {
		CTextDraw::ChangeFontLog(pDC, 16, "Consolas", RGB(255, 255, 255));
		CTextDraw::Print(pDC, _boxX + 16, _boxY + 20, "Now we don't have 'save' action...  :(");
	}
	void Interface::ShowTextEnd(CDC* pDC) {
		vector<string> vec_str;
		vec_str = { "Title", "End", "Cancel" };
		for (int i = 0; i < 3; i++) {
			CTextDraw::ChangeFontLog(pDC, 16, "Consolas", RGB(255, 255, 255));
			CTextDraw::Print(pDC, _boxX + 9 * 32, _boxY + 6 * 32 + i * _lineSpacing, vec_str.at(i));
		}
	}
	void Interface::ShowItemBitmap() {
		for (int i = 0; i < int(_itemsImgShowIndex.size()); i++) {
			_itemsImg.at(_itemsImgShowIndex.at(i)).SetTopLeft(_boxX + 16 + 32 + (i % 2) * 11 * 32, _boxY + 3 * 32 + (i / 2) * _lineSpacing);
			_itemsImg.at(_itemsImgShowIndex.at(i)).ShowBitmap();
		}
	}
	void Interface::ShowTotal() {
		if (_show == status) {
			_status.SetTopLeft(_boxX, _boxY);
			_status.ShowBitmap();
			ShowCursorStatus();
			CDC* pDC = CDDraw::GetBackCDC();
			ShowTextStatus(pDC);
			CDDraw::ReleaseBackCDC();
		}
		else if (_show == item) {
			_item.SetTopLeft(_boxX, _boxY);
			_item.ShowBitmap();
			if (int(_itemsIntro.size()) != 0) {
				ShowCursorItem();
				ShowItemBitmap();
				CDC* pDC = CDDraw::GetBackCDC();
				ShowTextItem(pDC);
				CDDraw::ReleaseBackCDC();
			}
		}
		else if (_show == save) {
			_save.SetTopLeft(_boxX, _boxY);
			_save.ShowBitmap();
			ShowCursorSave();
			CDC* pDC = CDDraw::GetBackCDC();
			ShowTextSave(pDC);
			CDDraw::ReleaseBackCDC();
		}
		else if (_show == end) {
			_end.SetTopLeft(_boxX, _boxY);
			_end.ShowBitmap();
			ShowCursorEnd();
			CDC* pDC = CDDraw::GetBackCDC();
			ShowTextEnd(pDC);
			CDDraw::ReleaseBackCDC();
		}
		else if (_show == use_item) {
			_useItemDialog.ShowTotal();
		}
	}
	void Interface::OnKeyDown(UINT nChar) {
		if (_show == use_item) {
			_useItemDialog.GetSelect(nChar);
		}
		if (nChar == VK_ESCAPE) {
			if (!IsShow()) {
				SetShow(true);
				_show = status;
				ResetChoose();
			}
			else if (_show == status) {
				SetShow(false);
				_show = none;
				ResetChoose();
			}
			else if (_show == item) {
				_show = status;
				ResetChoose();
			}
			else if (_show == save) {
				_show = status;
				ResetChoose();
			}
			else if (_show == end) {
				_show = status;
				ResetChoose();
			}
		}
		else if (nChar == VK_SPACE) {
			if (_show == status) {
				if (_statusChoose == 0) {
					_show = item;
					ResetChoose();
				}
				else if (_statusChoose == 1) {
					_show = save;
					ResetChoose();
				}
				else if (_statusChoose == 2) {
					_show = end;
					ResetChoose();
				}
			}
			else if (_show == item) {
				if (_itemsIntro.size() != 0) {
					_useItemIndex = _itemChoose;
					if (IsChangeStatus()) {
						_show = use_item;
						_useItemDialog.SetParam({ "Do u want to use the " + _itemsName.at(_useItemIndex) + "?" }, true);
						_useItemDialog.SetOption("Yes", "No");
						_IsShowMapAndDialog = true;
					}
					else {
						_IsUseItem = true;
						_show = none;
						SetShow(false);
						ResetChoose();
					}
				}
			}
			else if (_show == end) {
				if (_endChoose == 0) {
					_IsGoTitle = true;
					_show = none;
					SetShow(false);
					ResetChoose();
				}
				else if (_endChoose == 1) {
					_IsEndGame = true;
					_show = none;
					SetShow(false);
					ResetChoose();
				}
				else if (_endChoose == 2) {
					_show = status;
					ResetChoose();
				}
			}
			else if (_show == use_item) {
				if (_useItemDialog.Choice() == Dialog::option::yes) {
					if (_itemsIntro.at(_useItemIndex) == "phillips screwdriver core") {
						ChangeItemStatusByIntro("flathead screwdriver", "flathead screwdriver core", "screwdriver core", 0);
						ChangeItemStatusByIntro("phillips screwdriver core", "phillips screwdriver", "screwdriver", 0);
					}
					else if (_itemsIntro.at(_useItemIndex) == "flathead screwdriver core") {
						ChangeItemStatusByIntro("phillips screwdriver", "phillips screwdriver core", "screwdriver core", 1);
						ChangeItemStatusByIntro("flathead screwdriver core", "flathead screwdriver", "screwdriver", 1);
					}
					else if (_itemsName.at(_useItemIndex) == "detergent") {
						ChangeItemStatusByName("handkerchief", "handkerchief (clean)", "handkerchief", 1);
						DeleteItem("detergent");
					}
					else if (_itemsName.at(_useItemIndex) == "oil") {
						ChangeItemStatusByName("lighter", "lighter (full of oil)", "lighter", 0);
						DeleteItem("oil");
					}
				}
				_show = none;
				SetShow(false);
				ResetChoose();
				_IsShowMapAndDialog = false;
			}
		}
		else if (nChar == VK_UP) {
			if (_show == status) {
				if (_statusChoose == 0) {
					_statusChoose = 2;
				}
				else {
					_statusChoose--;
				}
			}
			else if (_show == item) {
				if (!(_itemChoose - 2 < 0)) {
					_itemChoose -= 2;
				}
			}
			else if (_show == end) {
				if (_endChoose == 0) {
					_endChoose = 2;
				}
				else {
					_endChoose--;
				}
			}
		}
		else if (nChar == VK_DOWN) {
			if (_show == status) {
				if (_statusChoose == 2) {
					_statusChoose = 0;
				}
				else {
					_statusChoose++;
				}
			}
			else if (!(_itemChoose + 2 >= int(_itemsName.size()))) {
				_itemChoose += 2;
			}
			else if (_show == end) {
				if (_endChoose == 2) {
					_endChoose = 0;
				}
				else {
					_endChoose++;
				}
			}
		}
		else if (nChar == VK_LEFT) {
			if (_show == item) {
				if (!(_itemChoose - 1 < 0)) {
					_itemChoose --;
				}
			}
		}
		else if (nChar == VK_RIGHT) {
			if (_show == item) {
				if (!(_itemChoose + 1 >= int(_itemsName.size()))) {
					_itemChoose++;
				}
			}
		}
	}
	void Interface::ResetChoose() {
		_statusChoose = _itemChoose = _saveChoose = _endChoose = 0;
	}
	void Interface::SetShow(bool show) {
		_isShow = show;
	}
	bool Interface::IsShow() const {
		return _isShow;
	}
	bool Interface::IsTitle() const {
		return _IsGoTitle;
	}
	bool Interface::IsEnd() const {
		return _IsEndGame;
	}
	bool Interface::IsChangeStatus() {
		return (_itemsName.at(_useItemIndex) == "screwdriver core" && FindItem("screwdriver"))
			|| (_itemsName.at(_useItemIndex) == "detergent" && FindItem("handkerchief"))
			|| (_itemsName.at(_useItemIndex) == "oil" && FindItem("lighter"));
	}
	bool Interface::IsUseItem() const{
		return _IsUseItem;
	}
	bool Interface::IsShowMapAndDialog() const{
		return _IsShowMapAndDialog;
	}
}