#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include "config.h"
#include "MapData.h"
namespace game_framework {


	void MapData::SetDimension(int x, int y, int z)
	{
		_DimX = x;
		_DimY = y;
		_DimZ = z;
		//if (_DimZ == 0) throw std::invalid_argument("invalid function call");
		_vector.resize(x*y*z);
	}


	int MapData::GetValue(int x, int y, int z)
	{
		//if (_DimZ == 0) throw std::invalid_argument("invalid function call");
		return _vector.at(x * _DimY * _DimZ + y * _DimZ + z);
	}

	void MapData::AssignValue(int x, int y, int z, int value)
	{
		//if (_DimZ == 0) throw std::invalid_argument("invalid function call");
		_vector.at(x * _DimY * _DimZ + y * _DimZ + z) = value;
	}

	void MapData::SetDimension(int x, int y)
	{
		_DimX = x;
		_DimY = y;
		_vector.resize(x*y);
	}
	int MapData::GetValue(int x, int y)
	{
		return _vector.at(x * _DimY + y );
	}
	
	void MapData::AssignValue(int x, int y, int value)
	{
		_vector.at(x * _DimY  + y ) = value;
	}

	int MapData::Msize()
	{
		return _vector.size();
	}
}