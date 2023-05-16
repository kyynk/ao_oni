#pragma once
namespace game_framework{
	class GameMap;
	class MapRouter;
	class Human;
	class Interfacedata;
	class Interface;
	class MainHuman : public Human {
	public:
		MainHuman();
		~MainHuman() = default;
		void init(int step,int offset,Direction dir);
		void ResetToGrid();
		void SetNextMapPos(GameMap& map);
		void OnMove(GameMap &map, MapRouter &router, const int nowID, const vector<vector<int>>&, const vector<vector<int>>&) ;
		void RouterCheckChangeMap(GameMap& map, MapRouter& router, int nowID,Interface& gif);
		void OnMove();
		void OnMoveBySettings(int block);
		void OnKeyDown(UINT nChar);
		void OnKeyUp(UINT nChar);
		//void OnShow();
		
		int GetOffset() const { return _offsetY; }
		int GetU() const { return _pos_y - TILE + _offsetY; }
		int GetD() const { return _pos_y + TILE + _offsetY; }
		int GetL() const { return _pos_x - TILE; }
		int GetR() const { return _pos_x + TILE; }
		
		int NextX() const { return _nextmapx; }
		int NextY() const { return _nextmapy; }
		int NextMapID() const { return _nextMapID; }
		bool &IsMachineDone() { return _machine_done; }
		bool &IsMapChanged() { return _isMapChanged; }
		bool &IsDoorLock() { return _door_lock; }
		bool &IsDoorOpen() { return _door_open; }
		void SetMachine(Direction pressing);
		void CheckMapChangeTN(GameMap& map, MapRouter& router, int const nowID, const vector<vector<int>>& TN,Interface& gif);
		void SetNextMap(int x, int y, int mapID);
		void SetAllMoveFalse();
		void SetNowmove(Direction m);
	private:
		bool _isup;
		bool _isdown;
		bool _isleft;
		bool _isright;
		int _nextmapx;
		int _nextmapy;
		int _nextMapID;
		bool _isMapChanged;
		bool _door_lock;
		bool _door_open;
		bool _piano_open;
	};
}