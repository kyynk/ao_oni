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
		for (int i = 0; i < 5;i++) {
			if(!_points[i].IsEmpty())
			TRACE("{%d, %d, %d, %d}\n", _points[i].w, _points[i].x, _points[i].y, _points[i].z );
		}
	}
}

