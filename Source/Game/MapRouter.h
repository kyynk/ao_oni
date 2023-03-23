#pragma once
namespace game_framework {


	class MapRouter {
	public:
		static MapRouter* GetInstance() { return _Instance = (_Instance != nullptr) ? _Instance : new MapRouter(); }
		void SetNowMap(string nowmap) { _nowmap = nowmap; }
		void Load(string filename);
		string GetNowMap() { return _nowmap; }
		~MapRouter() {}
		void Cleanup();
	private:
		MapRouter() {}
		string _nowmap;
		map<string, map<CPoint, string> > _data;
		static MapRouter* _Instance;



	};
}