#pragma once
namespace game_framework {


	class MapRouter {
	public:
		static MapRouter* GetInstance() { return _Instance = (_Instance != nullptr) ? _Instance : new MapRouter(); }
		void SetNowMap(int nowmap) { _nowID = nowmap; }
		void Load(string filename);
		int GetNowMap() { return _nowID; }
		~MapRouter() {}
		void Cleanup();
	private:
		MapRouter() {}
		int _nowID;
		vector<MapNode> _data[23];
		static MapRouter* _Instance;
	};
}