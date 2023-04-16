#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "Event.h"
namespace game_framework {
	Event::Event()	{
		_condition = false;
		_triggered = false;
		_dialogindex = 0;
		_dialogcount = 0;
		_transmap = false;
	}
	void Event::debug()	{
		
	}
	void Event::SetCondition(bool con) {
		_condition = con;
	}

	void Event::SetTriggered(bool triggered) {
		_triggered = triggered;
	}
	bool Event::IsTriggered()	{
		return _triggered;
	}
	vector<vector<int> >& Event::GetBlockPath() {
		return _blockpath;
	}
	void Event::SetParam( vector<vector<int> > blockpath, int firstindex, int count) {
		_blockpath = blockpath;
		_dialogindex = firstindex;
		_dialogcount = count;
	}
	int Event::GetDialogIndex() {
		return _dialogindex;
	}
	int Event::GetDialogCount() {
		return _dialogcount;
	}
}