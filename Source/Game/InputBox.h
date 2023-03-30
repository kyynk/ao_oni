#pragma once
namespace game_framework {
	class InputBox: public Entity {
	private:
		const int  buffersize = 20;
		CMovingBitmap _box;
		bool _iswrite;
		vector <char> _buffer;
		string _placeholder;
		int _margin;
		bool _toggleplaceholder;
	public:
		inline bool isInteger()		{
			const string s = string(_buffer.begin(),_buffer.end());
			if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
			char * p;
			strtol(s.c_str(), &p, 10);
			return (*p == 0);
		}
		InputBox(){}
		~InputBox(){}
		void ToggleBox() { _iswrite = !_iswrite; }
		void init(int x, int y, int cursor, int margin);
		void BoxOn(UINT c);
		void OnMove();
		void ClearBuffer();
		void Load(string box, COLORREF color1 = RGB(0,0,0));
		bool IsWrite() { return _iswrite; }
		void Show();
		string GetString(){ return string(_buffer.begin(), _buffer.end()); }
		

	};
}