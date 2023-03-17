#pragma once

namespace game_framework {
	class TileManager {
		TileManager* GetInstance() {
			return (_instance == nullptr) ? new TileManager: _instance;
		}
		void Load();
	private:
		TileManager();
		static TileManager* _instance;
		vector<CMovingBitmap> _tiles;
		void Load();
	};

}