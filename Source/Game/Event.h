#pragma once

namespace game_framework {

	class Event {
	public:
		Event();
		void debug ();
		void SetConditionTrue(string pre);
		void SetEvents(vector<string> evts);
		bool Trigger();
	private:
		map<string, bool> _prereq;
		bool _trigger;
		
	};
}