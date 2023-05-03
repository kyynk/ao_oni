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
	Interface::Interface() {
		init();
	}
	Interface::~Interface() = default;
	void Interface::init() {

	}
	// game time counter
	bool Interface::isPause() const {
		return _isPause;
	}
	bool Interface::isStop() const {
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
	long Interface::getStartTime() {
		return start_time;
	}
	string Interface::getGameTime() {
		/*long t = time(0) - start_time;
		gotoxy(35, 12);
		cout << setw(2) << setfill('0') << t / 60 / 60 << ":"
			<< setw(2) << setfill('0') << t / 60 << ":"
			<< setw(2) << setfill('0') << t % 60 << endl;*/
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
	void Interface::PlayerStep() {
	}
	void Interface::ShowCursor() {
	}
	void Interface::ShowText(CDC* pDC) {
	}
	void Interface::StoreItem(string item_name) {
		items.push_back(item_name);
	}
}