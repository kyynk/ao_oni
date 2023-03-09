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
	}
	void Event::debug()	{
		for (auto iter : _prereq) {
			TRACE("%s ,%d\n", iter.first, (iter.second) ? 1 : 0);
		}
	}
	void Event::SetConditionTrue(string pre) {
		_prereq[pre] = true;
	}
	void Event::SetEvents(vector<string> evts) {
		for (auto iter : evts) {
			_prereq.emplace(iter, false);
		}
	}
	bool Event::Trigger()	{
		bool trigger = true;
		for (auto iter : _prereq) {
			trigger = trigger && iter.second;
		}
		return trigger;
	}
}