#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "config.h"
#include "Oni.h"

#define ONI_DOWN 0
#define ONI_DOWN_1 1
#define ONI_DOWN_2 2
#define ONI_UP 9
#define ONI_UP_1 10
#define ONI_UP_2 11
#define ONI_LEFT 3
#define ONI_LEFT_1 4
#define ONI_LEFT_2 5
#define ONI_RIGHT 6
#define ONI_RIGHT_1 7
#define ONI_RIGHT_2 8

namespace game_framework {
	Oni::Oni() {
		_type = normal;
	}
	Oni::~Oni() {
	}
}