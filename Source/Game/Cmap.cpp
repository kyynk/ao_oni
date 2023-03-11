#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "config.h"
#include "Cmap.h"
namespace game_framework {
	Cmap::Cmap(){
	}
	void Cmap::Init(int w, int h, int px, int py){
		_width = w;
		_height = h;
		_pos_x = px;
		_pos_y = py;
	}
	void Cmap::ShowMap(int layer)	{
		for (int i = 0; i < _width; i++) {
			for (int j = 0; j < _height; j++) {
				_cmap.at(i).at(j).at(layer).ShowBitmap();
			}
		}
	}
}