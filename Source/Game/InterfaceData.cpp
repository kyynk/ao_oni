#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <string>
#include "InterfaceData.h"
namespace game_framework {
	InterfaceData::InterfaceData(string intro, string name) {
		_intro = intro;
		_shortName = name;
	}
	string InterfaceData::GetIntro() {
		return _intro;
	}
	string InterfaceData::GetName() {
		return _shortName;
	}
}