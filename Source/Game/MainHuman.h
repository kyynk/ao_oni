#pragma once
namespace game_framework{
	class GameMap;
	class MapRouter;
	class MainHuman : public Entity {
	public:
		MainHuman();
		void SetXYAndCol(int x, int y);
		void init(int step,int offset,Direction dir);
		void ResetToGrid();
		void SwitchMap(GameMap& map);
		void OnMove(GameMap &map, MapRouter &router, int nowID, const vector<vector<int>>&, const vector<vector<int>>&, const vector<vector<int>>&) ;
		void RouterCheckChangeMap(GameMap& map, MapRouter& router, int nowID);
		void OnMove();
		void OnMoveBySettings(int block);
		void OnKeyDown(UINT nChar);
		void OnKeyUp(UINT nChar);
		void OnShow();
		void Load(vector<string> filenames,COLORREF color );
		void SetDirection(Direction d) {
			_direction = d;
		}
		int GetDirection() const  {	return _direction; }
		int GetOffset() const { return _coroffset; }
		int GetU() const { return _uy + _coroffset; }
		int GetD() const { return _dy + _coroffset; }
		int GetL() const { return _lx; }
		int GetR() const { return _rx; }
		int GetX() const { return _pos_x; };
		int GetY() const { return _pos_y+_coroffset; }
		int NextX() const { return _nextmapx; }
		int NextY() const { return _nextmapy; }
		int NextMapID() const { return _nextMapID; }
		bool IsSwitchMap() const { return _switchMapCheck; }
		bool &IsMapChanged() { return _isMapChanged; }

		void SetMachine(Direction pressing);
		void SetNextMap(int x, int y, int mapID);
		void SetAllMoveFalse();
		void SetNowmove(Direction m);
		
	private:
		int _machine_count;
		int _coroffset;
		bool _walkiter;
		bstate _bstate;
		Direction _direction;
		Direction _nowmove;
		Direction _pressing;
		int _step;
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