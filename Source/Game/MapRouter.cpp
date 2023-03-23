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
#include <string>
#include "MapRouter.h"
//namespace game_framework {
//	void MapRouter::Load(string filename)
//	{
//		string s1, s2;
//		int xx, yy;
//		int n;
//		fstream in(filename);
//		for (int i = 0; i < n; i++) {
//			in >> s1 >> s2 >> xx >> yy;
//			if (!_data.empty()) {
//				if (_data.find(s1) == _data.end()) {
//					map<CPoint, string> tmp;
//					tmp.insert({ CPoint(xx,yy),s2 });
//					_data.insert({ s1,tmp });
//				}
//				else {
//					_data[s1].insert({ CPoint(xx,yy),s2 });
//				}
//			}
//		}
//	}
//	void MapRouter::Cleanup()
//	{
//		if (_Instance) {
//			delete _Instance;
//			_Instance = nullptr;
//		}
//	}
//
//	MapRouter* MapRouter::_Instance = nullptr;
//};
//
