#pragma once
namespace game_framework{
	class GameMap;
	class MapRouter;
	class Human;
	class MainHuman : public Human {
	public:
		MainHuman();
		~MainHuman() = default;
		//void SetXYAndCol(int x, int y);
		void init(int step,int offset,Direction dir);
		void ResetToGrid();
		void SwitchMap(GameMap& map);
		void OnMove(GameMap &map, MapRouter &router, const int nowID, const vector<vector<int>>&, const vector<vector<int>>&, const vector<vector<int>>&) ;
		void RouterCheckChangeMap(GameMap& map, MapRouter& router, int nowID);
		void OnMove();
		//void OnMove();
		void OnMoveBySettings(int block);
		void OnKeyDown(UINT nChar);
		void OnKeyUp(UINT nChar);
		//void OnShow();
		//void Load(vector<string> filenames,COLORREF color );
		
		int GetOffset() const { return _offsetY; }
		int GetU() const { return _pos_y - TILE + _offsetY; }
		int GetD() const { return _pos_y + TILE + _offsetY; }
		int GetL() const { return _pos_x - TILE; }
		int GetR() const { return _pos_x + TILE; }
		int GetX() const { return _pos_x; };
		int GetY() const { return _pos_y+_offsetY; }
		int NextX() const { return _nextmapx; }
		int NextY() const { return _nextmapy; }
		int NextMapID() const { return _nextMapID; }
		bool IsSwitchMap() const { return _switchMapCheck; }
		bool  IsMachineDone() const { return _machine_done; }
		bool &IsMapChanged() { return _isMapChanged; }

		void SetMachine(Direction pressing);
		void SetNextMap(int x, int y, int mapID);
		void SetAllMoveFalse();
		void SetNowmove(Direction m);
		
	private:
		
		
		Direction _pressing;
		bool _isup;
		bool _isdown;
		bool _isleft;
		bool _isright;
		int _nextmapx;
		int _nextmapy;
		int _nextMapID;
		bool _isMapChanged;
		bool _switchMapCheck;

	};
}