#pragma once
namespace game_framework {


	class MapRouter {
	public:
		static MapRouter* GetInstance() { return _Instance = (_Instance != nullptr) ? _Instance : new MapRouter(); }
		void SetNowMap(int nowmap) { _nowID = nowmap; }
		MapNode* GetNowMap() { return _data[_nowID]; }
		~MapRouter() {}
		void init();
		void Load(string filename);
		void Cleanup();
		void debug();
		inline void ShowMap() { _gamemaps.at(_nowID).ShowMap(); }
		inline void AddMap(GameMap map) { _gamemaps.push_back(map); }
		inline void ToggleShowTileIndex() { _gamemaps.at(_nowID).isshowtileindex = (_gamemaps.at(_nowID).isshowtileindex) ? false :true; }
		
		inline void AddTileIndex() { 
			if(_gamemaps.at(_nowID).indexlayer<_gamemaps.at(_nowID).GetLayer()-1)_gamemaps.at(_nowID).indexlayer++;
		};
		inline void MinusTileIndex() { 
			if(_gamemaps.at(_nowID).indexlayer>0)_gamemaps.at(_nowID).indexlayer--;
		};
		inline int &GSNowID() { return _nowID; }
		
		void ShowIndexLayer();
	private:
		MapRouter() {
			_nowID = 13;
			memset(record, 0, sizeof(record));
		}
		int _nowID;
		MapNode _data[23][5];
		int record[23];
		vector<GameMap> _gamemaps;
		static MapRouter* _Instance;
	};
}