#pragma once
namespace game_framework {
	class InterfaceData {
	public:
		InterfaceData(string intro, string name);
		~InterfaceData() = default;
		string GetIntro();
		string GetName();
	private:
		string _intro;
		string _shortName;
	};
}