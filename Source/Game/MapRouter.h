#pragma once
#include <string>
#include <vector>
#include "GameMap.h"
#include "MapNode.h"
namespace game_framework {
	class MapRouter {
	public:
		MapRouter() {
		}
		~MapRouter() {}
		void init();
		void Load(string filename);
		void debug();
		MapNode* GetNowMapPortal(int nowID) {
			return _data[nowID];
		}
		int GetRecord(int nowID) {
			return _record[nowID];
		}
	private:
		MapNode _data[46][5];
		int _record[46];
	};
}