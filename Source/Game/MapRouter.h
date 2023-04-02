#pragma once
#include <string>
#include <vector>
#include "GameMap.h"
#include "MapNode.h"
namespace game_framework {
	class MapRouter {
	public:
		MapRouter() {
			memset(record, 0, sizeof(record));
		}
		~MapRouter() {}
		void init();
		void Load(string filename);
		void debug();
	
	private:
		MapNode _data[23][5];
		int record[23];
	};
}