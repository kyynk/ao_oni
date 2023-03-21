#pragma once
namespace game_framework {
	class Item :public Entity {
	public:
		enum itemtype {
			once,
			multi,
			oncedone,
			multidone
		};
		Item();

		void init(bool isonce, bool triggered, itemtype type, int anidelay);

		void OnMove();

		void OnShow();
		void SetTriggered(bool a) {
			_triggered = a;
		}
		void Load(vector<string> file, COLORREF color);
	
	private:
		itemtype _type;
;		bool _triggered;
		bool _isonce;
		int _anidelay;
	};

}