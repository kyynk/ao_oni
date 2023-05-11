#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "Entity.h"
namespace game_framework {
	Entity::Entity() {
		TimerStop();
		_objclass = "";
	}
	void Entity::SetXY(int x, int y) {
		_pos_x = x;
		_pos_y = y;
	}
	void Entity::SelectShowBitmap(int index) {
		bitmap.SetFrameIndexOfBitmap(index);
	}
}