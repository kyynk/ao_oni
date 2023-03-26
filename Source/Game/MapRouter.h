#pragma once
namespace game_framework {


	class MapRouter {
	public:
		static MapRouter* GetInstance() { return _Instance = (_Instance != nullptr) ? _Instance : new MapRouter(); }
		void SetNowMap(int nowmap) { _nowID = nowmap; }
		int GetNowMap() { return _nowID; }
		~MapRouter() {}
		void Load(string filename);
		void Cleanup();
	private:
		MapRouter() {}
		int _nowID;
		vector<MapNode> _data[23];
		static MapRouter* _Instance;
	};
}