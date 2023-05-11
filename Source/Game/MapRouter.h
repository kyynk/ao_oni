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
		void copypath();
		void Restore();
		void SetAllPathFalse();
		void SetAllPathTrue();
		void Load(string filename);
		void debugF();
		void debug();
		MapNode* GetNowMapPortal(int nowID) {
			return _data[nowID];
		}
		int GetRecord(int nowID) {
			return _record[nowID];
		}
		void BlockPath(int x,int y);
		void UnblockPath(int x,int y);
		bool IsPathBlocked(int x,int y);
		bool& Gaming() { return _isgaming; }
	private:
		MapNode _data[65][6];
		bool _pathblocked[65][6];
		bool _pathblockedcopy[65][6];
		int _record[65];
		bool _isgaming;
	};
}