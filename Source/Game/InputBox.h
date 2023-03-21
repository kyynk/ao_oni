#pragma once
namespace game_framework {
	class InputBox {
	private:
		string _input;
		bool isOpen;
		CMovingBitmap _box;

	public:
		InputBox();
		~InputBox();
		void BoxOpen();
		void BoxClose();
		string GetInput();
		void ShowInput(CDC *pDC, CFont* &fp);
		void ShowPlaceholder();
		void ShowOption();
		

	};
}