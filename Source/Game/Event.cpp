#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "Event.h"
namespace game_framework {
	Event::Event()	{
		_condition = false;
		_prereq = "";
	}
	void Event::debug()	{
		
	}
	void Event::SetConditionTrue() {
		_condition = true;;
	}
	void Event::SetEvents(string evts) {
		_condition = false;
		_prereq = evts;
	}
	void Event::SetTriggered(bool triggered) {
		_triggered = triggered;
	}
	bool Event::IsTriggered()	{
		return _triggered;
	}
	vector<vector<int> >& Event::GetBlockPath() {
		return blockpath;
	}
}