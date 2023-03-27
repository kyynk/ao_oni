#pragma once
namespace game_framework {
	class Item :public Entity {
	public:
		enum itemtype {
			once,
			oncedone,
			multi,
			multidone
		};
		Item();

		void init(bool willMove, bool isonce, bool triggered,
			bool blocking, 
			itemtype type, int anidelay);

		void OnMove();

		void OnShow();
		void SetTriggered(bool a) {
			_triggered = a;
		}
		void Load(vector<string> file, COLORREF color);
		bool collide(int playerX, int playerY);
	
	private:
		int _posX, _posY, //Top Left
			_w, _h;  // Width Height
		itemtype _type;
;		bool _triggered;
		bool _isonce;
		bool _isMoving;
		bool _isBlock;
		int _anidelay;
	};

}