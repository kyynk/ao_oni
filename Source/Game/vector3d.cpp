#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "config.h"
#include "vector3d.h"
namespace game_framework {


	void vector3d::SetDimension(int x, int y, int z)
	{
		_DimX = x;
		_DimY = y;
		_DimZ = z;
		_vector.resize(x*y*z);
	}


	int vector3d::GetValue(int x, int y, int z)
	{
		return _vector.at(x * _DimY * _DimZ + y * _DimZ + z);
	}

	void vector3d::AssignValue(int x, int y, int z, int value)
	{
		_vector.at(x * _DimY * _DimZ + y * _DimZ + z) = value;
	}

	

	int vector3d::V3dSize()
	{
		return _vector.size();
	}
}