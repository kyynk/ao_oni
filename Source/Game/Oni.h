#pragma once
namespace game_framework {
	class Oni : public Entity{
	public:
		enum OniName {
			normal,
			mika,
			takesi,
			takurou,
			flat,
			muddy,
			muscle,
			strange
		};
		Oni() {
			_objclass = "oni";
		}
		~Oni() = default;
		void init(OniName tp, int step, int moveTime);
		void SetPos(int x, int y);
		int GetPosX() const;
		int GetPosY() const;
		int GetPosL() const;
		int GetPosU() const;
		int GetPosR() const;
		int GetPosD() const;
		int GetOffsetY() const { return _offsetY; }
		void Load(string filename, COLORREF color);
		void SetPlayerPos(int playerX, int playerY);
		void Track(GameMap &map);
		void OnMove(GameMap &map);	// every time oni move, will track first
		void OnShow();
		// every time oni appear, default time is 10 sec
		// 10 -> 0 sec, if is 0 sec, 
		// change stage will make Oni disappear
		void Countdown();
		void SetChangeMap(int x, int y, int id);
		bool &IsWait() { return _wait; };
		bool &IsShow() { return _isShow; };
		bool &Once() { return _changemaponceprocess ;};
		int& GetOverTimer() { return _overTime; }
		bool isCatch();  // collide
		void ResetOni();
	private:
		
		int _humanX, _humanY, 
			_step, _moveTime, _overTime, 
			_offsetX, _offsetY;
		int _nextx;
		int _nexty; 
		int _mapID;
		bool _isShow;
		bool _walkiter;
		bool _wait;
		bool _changemaponceprocess;
		bool GonnaGiveUpSoSadUntilTheNextMap;
		OniName _type;
		bstate _bstate;
		Direction _nowmove;
		Direction _tracking;
		const int EXIST_TIME = 500;
	};
}