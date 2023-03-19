#pragma once
namespace game_framework {

	class MapRes {
	public:
		static MapRes* GetInstance() { return _Instance = (_Instance != nullptr) ? _Instance : new MapRes(); }
		void Load(const string ,const int);
		map<string, vector<CMovingBitmap> >& GetData() { return _data; };
		~MapRes();

		void check();
		void Cleanup();
	private:
		MapRes() {}
		map<string, vector<CMovingBitmap> > _data;
		static MapRes* _Instance;

	};
}