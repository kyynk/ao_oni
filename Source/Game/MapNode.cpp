#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "config.h"
#include <fstream>
#include "GameMap.h"
#include "MapNode.h"
namespace game_framework {
	void MapNode::debug()
	{
		TRACE("mapID : %d\n ",_ID);

		for (auto f : _points) {
			TRACE("{%d, %d, %d, %d}\n", f.w, f.x, f.y, f.z );
		}
	}
}

