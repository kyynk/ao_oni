#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

namespace game_framework {
	CGameStateRun::CGameStateRun(CGame* g) : CGameState(g)
	{
	}

	CGameStateRun::~CGameStateRun()
	{
		MapRes::GetInstance()->Cleanup();
	}
	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{

		entities.reserve(3);
		ShowInitProgress(33, "loading game mid");
		// player map x, player map y, map ID bulk of craps
		vector<string> tmpvec = { "blc","brc","btc" };
		for (auto& f : tmpvec) {
			ifstream in("map_data/"+ f +".txt");
			int value;
			if (in) {
				while (in >> value) {
					std::vector<int> innerVec;
					innerVec.push_back(value);
					for (int i = 0; i < 2; ++i) {
						in >> value;
						innerVec.push_back(value);
					}
					if (f == "blc") {
						blockLeftCor.push_back(innerVec);
					}
					else if (f == "brc") {
						blockRightCor.push_back(innerVec);
					}
					else if (f == "btc") {
						blockTeleportCor.push_back(innerVec);
					}
				}
				in.close();
			}
			
		}
		for (int i = 0;i < 4;i++) {
			darkmask[i].load({"img/mapmask0.bmp","img/mapmask1.bmp"}, default_C);
			darkmask[i].SetState(DarkRoomEffect::dark);

		}
		tmpvec.clear();
		// main character
		vector<string> humans = { "hiroshi_move/Hiroshi_","mika_move/Mika_","takeshi_move/Takeshi_","takuro_move/Takuro_" };
		vector<string> playervec;
		for (int k = 0; k < 4; k++) {
			for (int i = 0; i < 12; i++) {
				playervec.push_back("img/" + humans.at(k) + to_string(i) + ".bmp");
			}
			if (k == 0)
				player.Load(playervec, default_C);
			else if (k == 1)
				human_mika.Load(playervec, default_C);
			else if (k == 2)
				human_takeshi.Load(playervec, default_C);
			else if (k == 3)
				human_takuro.Load(playervec, default_C);
			// other players
			playervec.clear();
		}
		// map resources
		std::ifstream mapres_in("map_bmp/mapsize.txt");
		string name;
		int count;
		for (int i = 0; i < 10; i++) {
			mapres_in >> name >> count;
			ShowInitProgress(33 + 5 * i, name);
			MapRes::GetInstance()->Load(name, count);
		}
		// map data
		for (int i = 0; i < 65; i++) {
			GameMap tmp;
			tmp.Load("map_bmp/map" + to_string(i) + ".txt");
			int w = tmp.GetWidth();
			int h = tmp.GetHeight();
			w = w + ((w % 2 == 0) ? 1 : 0);
			h = h + ((h % 2 == 0) ? 1 : 0);
			tmp.SetTopLeftMap((SIZE_X - 16 - w * TILE) / 2, (SIZE_Y - 20 - h * TILE) / 2);
			gamemaps.push_back(tmp);
		}
		ifstream indexlayer("map_bmp/mapindexlayer.txt");
		int i2;
		for (int i = 0; i < 65; i++) {
			indexlayer >> i2;
			mapoverlayindex.push_back(i2);
		}
		// item
		items.resize(48);
		items.at(TOILET).SetParam(-1, 0, 0, Item::toilet);
		items.at(TUB_ONCE).SetParam(100, 0, TILE, Item::tub_once);
		items.at(PHILLIPS).SetParam(100, 0, TILE, Item::phillips);
		items.at(TUB_FIXED).SetParam(-1, 0, TILE, Item::tub_fixed);
		items.at(BROKEN_DISH).SetParam(-1, 0, 0, Item::broken_dish);
		items.at(LIB_BOOK).SetParam(-1, 0, 0, Item::lib_book);
		items.at(KEY_3F_L).SetParam(100, 0, 0, Item::key_3F_L);
		items.at(KEY_LIB).SetParam(100, 0, 0, Item::key_lib);
		items.at(DOOR_KNOB).SetParam(100, 0, TILE, Item::door_knob);
		items.at(DOOR_NO_KNOB).SetParam(-1, 0, TILE, Item::door_no_knob);
		items.at(LIGHTER).SetParam(100, 2 * TILE, TILE, Item::lighter);
		items.at(TATAMI_L).SetParam(100, 2 * TILE, TILE, Item::tatami_l);
		items.at(TATAMI_R).SetParam(100, 2 * TILE, TILE, Item::tatami_r);
		items.at(DETERGENT).SetParam(-1, 0, 0, Item::detergent);
		items.at(KEY_BASEMENT).SetParam(100, 0, 0, Item::key_basement);
		items.at(GATE).SetParam(-1, TILE, TILE, Item::gate);
		items.at(GATE2).SetParam(-1, TILE, TILE, Item::gate);
		items.at(BED).SetParam(-1, TILE, 2 * TILE, Item::bed);
		items.at(BOOKCASE_L).SetParam(-1, 2 * TILE, 2 * TILE + TILE / 2, Item::bookcase_l);
		items.at(BOOKCASE_R).SetParam(-1, 2 * TILE, 2 * TILE + TILE / 2, Item::bookcase_r);
		items.at(WHITE_BOOKCASE).SetParam(-1, 0, TILE / 2, Item::white_bookcase);
		items.at(BOOKCASE_MAP21).SetParam(-1, 2 * TILE, TILE, Item::bookcase_map21);
		items.at(HANDKERCHIEF).SetParam(-1, 0, 0, Item::handkerchief);
		items.at(CLOSET_SHAKE).SetParam(100, TILE / 2, TILE, Item::closet_shake);
		items.at(CLOSET_TAKESI_0).SetParam(100, 0, TILE / 2, Item::closet_takesi_0);
		items.at(CLOSET_TAKESI_1).SetParam(100, 0, TILE / 2, Item::closet_takesi_1);
		items.at(CLOSET_HIROSI_R).SetParam(100, 0, TILE / 2, Item::closet_hirosi_R);
		items.at(CLOSET_HIROSI_L).SetParam(100, TILE / 2, TILE / 2, Item::closet_hirosi_L);
		items.at(CLOSET_MIKA_OUT).SetParam(100, 0, 0, Item::closet_mika_out);
		items.at(KEY_JAIL).SetParam(100, 0, 0, Item::key_jail);
		items.at(FLATHEAD).SetParam(-1, 0, 0, Item::flathead);
		items.at(OIL).SetParam(-1, 0, 0, Item::oil);
		items.at(DOOR_ONI).SetParam(100, 0, TILE, Item::door_oni);
		items.at(DOOR_OPEN).SetParam(300, 0, TILE, Item::door_open);
		items.at(DOOR_DIE).SetParam(100, 0, TILE, Item::door_die);
		items.at(DOOR_HALF).SetParam(-1, 0, TILE, Item::door_half);
		items.at(PIANO_PWD_NOTOPEN).SetParam(-1, 0, 0, Item::password_not_open);
		items.at(PIANO_PWD_TAKE).SetParam(-1, 0, 0, Item::password_get_key);
		items.at(BB_KEY).SetParam(100, 0, 0, Item::key_2F_TL);
		items.at(BASEMENT_PWD).SetParam(-1, 0, 0, Item::password_not_open);
		items.at(BASEMENT_PWD_TAKE).SetParam(-1, 0, 0, Item::password_get_key);
		items.at(BASEMENT_KEY).SetParam(100, 0, 0, Item::key_annexe);
		items.at(DOOR_DIFF).SetParam(-1, 0, TILE, Item::diff_door);
		items.at(KABE_PWD).SetParam(-1, 0, 0, Item::kabe_pwd);
		items.at(CANDLE1).SetParam(-1, 0, TILE / 2, Item::candle);
		items.at(CANDLE2).SetParam(-1, 0, TILE / 2, Item::candle);
		items.at(CLOSET_HIROSI_MAP15).SetParam(100, 0, TILE / 2, Item::closet_hirosi_R);
		items.at(MIKA_TO_ONI).SetParam(100, 0, 0, Item::mika_to_oni);
		//events
		events.resize(41);
		events.at(BROKEN_DISH_E).SetParam({ {5,13} }, 0,2 );
		events.at(START_EVENT_E).SetParam({ {5,11 }	}, 2, 8);
		events.at(START_EVENT2_E).SetParam({ {13,6},{13,7},{7,14},{7,8},{8,15} }, 10, 3);
		events.at(KEY_LIB_E).SetParam({}, 13, 1);
		events.at(DETERGENT_E).SetParam({}, 14, 1);
		events.at(PHILLIPS_E).SetParam({}, 15, 1);
		events.at(KEY_3F_L_E).SetParam({}, 16, 1);
		events.at(LIGHTER_E).SetParam({}, 17, 1);
		events.at(DUMB_TAKESHI_E).SetParam({}, 18, 6);
		events.at(HANDKERCHIEF_E).SetParam({}, 24, 1);
		events.at(MIKA_SCARE_E).SetParam({}, 25, 4);
		events.at(MIKA_NOTOK_E).SetParam({}, 29, 2);
		events.at(MIKA_OK_E).SetParam({}, 31, 1);
		events.at(MIKA_REPEAT_E).SetParam({}, 32, 1);
		events.at(LIB_KEY_CHASE).SetParam({ {9, 13 }, {6, 18 }, {6,19},{9, 10} }, -1, -1);
		events.at(DUMB_TAKESHI_REPEAT_E).SetParam({}, 18, 1);
		events.at(KEY_2F_TL_E).SetParam({}, 33, 1); // piano pwd
		events.at(KEY_BASEMENT_E).SetParam({}, 34, 1);
		events.at(KEY_ANNEXE_E).SetParam({}, 36, 1);
		events.at(DOOR_LOCKED_E).SetParam({}, 37, 1);
		events.at(DOOR_UNLOCKED_E).SetParam({}, 38, 1);
		events.at(OIL_E).SetParam({}, 39, 1);
		events.at(FLATHEAD_E).SetParam({}, 40, 1);
		events.at(KEY_JAIL_E).SetParam({}, 41, 1);
		events.at(DOOR_WIRED_E).SetParam({}, 42, 1);
		events.at(DOOR_WHERE_KNOB_E).SetParam({}, 43, 1);
		events.at(OPEN_BASEMENT_E).SetParam({}, 44, 1);
		events.at(DOOR_DIFF_OPEN_E).SetParam({ {10,21} }, -1, -1);
		events.at(LIGHTUP_ROOM21).SetParam({}, -1, -1);
		events.at(TATAMI_E).SetParam({}, -1, -1);
		events.at(OPEN_FUCKING_HOLE_E).SetParam({ {16,17} }, -1, -1);
		events.at(OPEN_FUCKING_ROOM_E).SetParam({ {21,22} }, -1, -1);
		events.at(ONI_KILL_MIKA_E).SetParam({}, 44, 2);
		events.at(MIKA_TO_ONI_E).SetParam({}, -1, -1);
		events.at(MIKA_IN_CLOSET_E).SetParam({}, -1, -1);
		events.at(USE_JAIL_KEY_E).SetParam({}, -1, -1);
		events.at(MIKA_DEAD_E).SetParam({}, -1, -1);
		events.at(GATE_ONI_APPEAR_E).SetParam({}, -1, -1);
		std::ifstream file("dialog/dialogs.txt");
		std::string line;
		while (std::getline(file, line)) {
			if (line.empty()) {
				continue;
			}
			Dialog dialog;
			dialog.SetFigure(line);
			std::getline(file, line);
			int numStrings = std::stoi(line);
			vector<string> disappointed;
			for (int i = 0; i < numStrings; i++) {
				std::getline(file, line);
				disappointed.push_back(line);
			}
			std::getline(file, line);
			int ischo = std::stoi(line);
			(ischo == 0) ? dialog.SetParam(disappointed, false) : dialog.SetParam(disappointed, true);
			disappointed.clear();
			for (int i = 0; i < ischo; i++) {
				std::getline(file, line);
				disappointed.push_back(line);
			}
			if (ischo != 0) {
				dialog.SetOption(disappointed.at(0), disappointed.at(1));
			}
			dialogs.push_back(dialog);

		}
		file.close();

		// objMove
		objs.resize(3);
		objs.at(obj_move::house1_2F_TR_chair).SetParam(ObjMove::house1_2F_TR_chair,
			8, 4, 0, 0, 15 * TILE, 9 * TILE);
		objs.at(obj_move::house1_2F_TL_chair).SetParam(ObjMove::house1_2F_TL_chair,
			8, 4, 0, TILE / 2, 14 * TILE, 15 * TILE); // 14 15
		objs.at(obj_move::house1_basement2_chair).SetParam(ObjMove::house1_basement2_chair,
			8, 4, 0, TILE / 2, 9 * TILE, 14 * TILE); // 9 14
		// password
		pwds.resize(2);
		pwds.at(piano).SetParam("9761"); // piano 0
		pwds.at(piano).SetShow(false);
		pwds.at(basement).SetParam("5376");
		pwds.at(basement).SetShow(false);
		// interface
		game_interface.init();
		game_interface.StartCount();
		// house1 map
		house1_map.LoadBitmapByString({ "img/map_house1.bmp" }, black_C);
		deadbody.LoadBitmapByString({ "img/oni_eat/deadbody.bmp" }, default_C);
		tmpvec.clear();
		for (int i = 0;i < 8;i++) {
			tmpvec.push_back("img/oni_eat/oni_eat" + std::to_string(i) + ".bmp");
		}
		oni_eat.LoadBitmapByString(tmpvec, default_C);
		
		// blue paint
		blue_paint.LoadBitmapByString({ "img/password/password_jail/jail_password_L.bmp",
			"img/password/password_jail/jail_password_R.bmp",
			"img/password/password_jail/password_L.bmp",
			"img/password/password_jail/password_R.bmp" }, black_C);
		// piano hint
		piano_hint.LoadBitmapByString({ "img/password/password_piano/piano_blood.bmp",
			"img/password/password_piano/piano_0.bmp",
			"img/password/password_piano/piano_hint.bmp" }, default_C);
		// basement0 kabe
		base0_kabe.LoadBitmapByString({ "img/password/password_jail/kabe_pwd0.bmp",
			"img/password/password_jail/kabe_pwd1.bmp",
			"img/password/password_jail/kabe_pwd2.bmp" }, default_C);
		// gate animation
		vector<string> gate_a;
		for (int i = 0; i < 103; i++) {
			gate_a.push_back("img/gate_oni_animation/" + std::to_string(i) + ".bmp");
		}
		gate_animation.LoadBitmapByString(gate_a, default_C);
		// closet animation
		vector<string> closet_a;
		for (int i = 0; i < 103; i++) {
			closet_a.push_back("img/oni_open_closet_animation/" + std::to_string(i) + ".bmp");
		}
		closet_animation.LoadBitmapByString(closet_a, default_C);
		// debug
		grid.LoadBitmapByString({ "img/grid.bmp" }, black_C);
		tileplaceholder.LoadBitmapByString({ "img/placeholder.bmp" });
		inputbox.Load("img/cursor/input_box.bmp");
		inputbox.init(20 * TILE, 0, 0, 10);
		// map link data
		router.init();
		router.Load("map_bmp/maplink.txt");
		// end dialog
		clear_game.SetFigure("none");
		// audio
		audio_control.resize(12);
		CAudio::Instance()->Load(AUDIO_BROKEN_DISH, "Audio/USE/broken_dish.wav");
		CAudio::Instance()->Load(AUDIO_CANDLE_LIGHT, "Audio/USE/candle_light.wav");
		CAudio::Instance()->Load(AUDIO_DOOR_LOCK, "Audio/USE/door_lock.wav");
		CAudio::Instance()->Load(AUDIO_GATE_ONI, "Audio/USE/gate_oni.wav");
		CAudio::Instance()->Load(AUDIO_HOROR, "Audio/USE/horor.wav");
		CAudio::Instance()->Load(AUDIO_LIB_FIRST, "Audio/USE/lib_first.wav");
		CAudio::Instance()->Load(AUDIO_MIKA_SOUND, "Audio/USE/mika_sound.wav");
		CAudio::Instance()->Load(AUDIO_ONI_OPEN_CLOSET, "Audio/USE/oni_open_closet.wav");
		CAudio::Instance()->Load(AUDIO_TAKESI_NOICE, "Audio/USE/takesi_noice.wav");
		CAudio::Instance()->Load(AUDIO_TUB_WATER, "Audio/USE/tub_water.wav");
		CAudio::Instance()->Load(AUDIO_THE_END, "Audio/USE/the_end.wav");
	}
	void CGameStateRun::OnBeginState()
	{
		_substate = OnWalking;
		boolpspace = true;
		mousex_foc = 0;
		mousey_foc = 0;
		isdebugmode = false;
		istwoway = 0;
		isteleportblock = false;
		isedit = false;
		isgrid = false;
		_pwd = false;
		_map_show = false;
		_use_handkerchief = false;
		_use_f_screwdriver = false;
		_blue_paint_show = false;
		_piano_hint_show = false;
		_base0_kabe_show = false;
		_in_interface = false;
		_in_closet = false;
		_is_danger = false;
		_in_gate_map22 = false;
		_bar_animation_show = false;
		_final = false;
		_oni_eat_mika = false;
		_die = false;
		_killtimes = 0;
		once = true;
		_nowID = 13;
		_tempMapID = -1;
		_dialogID = -1;
		_dialogcount = 0;
		_eventID = 0;
		_playerStep = 0;
		player.init(4, 16, Entity::left);
		player.SetPos(14 * TILE, 13 * TILE);
		human_mika.init(-1, 16, Entity::right);
		human_mika.SetPos(10 * TILE, 12 * TILE);
		human_takeshi.init(-1, 16, Entity::up);
		human_takeshi.SetPos(13 * TILE, 14 * TILE);
		human_takuro.init(-1, 16, Entity::down);
		human_takuro.SetPos(12 * TILE, 12 * TILE);
		normal_oni.init(Oni::normal, 4, 8);
		// audio
		for (int i = 0; i < 12; i++) {
			audio_control[i] = false;
		}
		// interface
		game_interface.ResetItem();
		// darkmask
		for (int i = 0; i < 4; i++) {
			darkmask[i].SetShow(true);
		}
		darkmask[2].SetShow(false);
		darkmask[3].SetShow(false);
		//redChair.Reset();
		
		oni_eat.SetAnimation(100, true);
		oni_eat.ToggleAnimation();
		
		router.SetAllPathTrue();
   		objs.at(house1_2F_TR_chair).Reset();
		objs.at(house1_2F_TR_chair).SetPreX(objs.at(house1_2F_TR_chair).GetPosX());
		objs.at(house1_2F_TR_chair).SetPreY(objs.at(house1_2F_TR_chair).GetPosY());
		objs.at(house1_2F_TL_chair).Reset();
		objs.at(house1_2F_TL_chair).SetPreX(objs.at(house1_2F_TL_chair).GetPosX());
		objs.at(house1_2F_TL_chair).SetPreY(objs.at(house1_2F_TL_chair).GetPosY());
		objs.at(house1_basement2_chair).Reset();
		objs.at(house1_basement2_chair).SetPreX(objs.at(house1_basement2_chair).GetPosX());
		objs.at(house1_basement2_chair).SetPreY(objs.at(house1_basement2_chair).GetPosY());
		//items
		items.at(TOILET).SetXY(12 * TILE, 15 * TILE);
		items.at(TUB_ONCE).SetXY(9 * TILE, 12 * TILE);
		items.at(PHILLIPS).SetXY(9 * TILE, 12 * TILE);
		items.at(TUB_FIXED).SetXY(9 * TILE, 12 * TILE);
		items.at(BROKEN_DISH).SetXY(12 * TILE, 8 * TILE);
		items.at(LIB_BOOK).SetXY(7 * TILE, 14 * TILE);
		items.at(KEY_3F_L).SetXY(7 * TILE, 14 * TILE);
		items.at(KEY_LIB).SetXY(15 * TILE, 9 * TILE);
		items.at(DOOR_KNOB).SetXY(12 * TILE, 6 * TILE);
		items.at(DOOR_NO_KNOB).SetXY(12 * TILE, 6 * TILE);
		items.at(LIGHTER).SetXY(6 * TILE, 6 * TILE);
		items.at(TATAMI_L).SetXY(6 * TILE, 6 * TILE);
		items.at(TATAMI_R).SetXY(9 * TILE, 6 * TILE);
		items.at(DETERGENT).SetXY(11 * TILE, 9 * TILE);
		items.at(KEY_BASEMENT).SetXY(12 * TILE, 11 * TILE);
		items.at(GATE).SetXY(10 * TILE, 12 * TILE);
		items.at(GATE2).SetXY(14 * TILE, 11 * TILE);
		items.at(BED).SetXY(8 * TILE, 14 * TILE + TILE / 2);
		items.at(BOOKCASE_L).SetXY(12 * TILE, 3 * TILE + TILE / 2);
		items.at(BOOKCASE_R).SetXY(15 * TILE, 3 * TILE + TILE / 2);
		items.at(WHITE_BOOKCASE).SetXY(16 * TILE, 6 * TILE + TILE / 2);
		items.at(BOOKCASE_MAP21).SetXY(8 * TILE, 7 * TILE);
		items.at(HANDKERCHIEF).SetXY(8 * TILE, 11 * TILE);
		items.at(CLOSET_SHAKE).SetXY(7 * TILE + TILE / 2, 7 * TILE);  // 7 7
		items.at(CLOSET_TAKESI_0).SetXY(8 * TILE, 7 * TILE + TILE / 2);
		items.at(CLOSET_TAKESI_1).SetXY(8 * TILE, 7 * TILE + TILE / 2);
		items.at(CLOSET_HIROSI_R).SetXY(8 * TILE, 7 * TILE + TILE / 2);
		items.at(CLOSET_HIROSI_L).SetXY(15 * TILE + TILE / 2, 7 * TILE + TILE / 2);
		items.at(CLOSET_MIKA_OUT).SetXY(15 * TILE + TILE / 2, 6 * TILE + TILE / 2);
		items.at(KEY_JAIL).SetXY(13 * TILE, 19 * TILE);
		items.at(FLATHEAD).SetXY(9 * TILE, 9 * TILE);
		items.at(OIL).SetXY(13 * TILE, 7 * TILE);
		items.at(DOOR_ONI).SetXY(10 * TILE, 10 * TILE);
		items.at(DOOR_OPEN).SetXY(10 * TILE, 10 * TILE);
		items.at(DOOR_DIE).SetXY(10 * TILE, 10 * TILE);
		items.at(DOOR_HALF).SetXY(10 * TILE, 10 * TILE);
		items.at(PIANO_PWD_NOTOPEN).SetXY(16 * TILE, 7 * TILE);
		items.at(PIANO_PWD_TAKE).SetXY(16 * TILE, 7 * TILE);
		items.at(BB_KEY).SetXY(16 * TILE, 7 * TILE);
		items.at(BASEMENT_PWD).SetXY(16 * TILE, 5 * TILE);
		items.at(BASEMENT_PWD_TAKE).SetXY(16 * TILE, 5 * TILE);
		items.at(BASEMENT_KEY).SetXY(16 * TILE, 5 * TILE);
		items.at(DOOR_DIFF).SetXY(17 * TILE, 15 * TILE);
		items.at(KABE_PWD).SetXY(16 * TILE, 5 * TILE);
		items.at(CANDLE1).SetXY(12 * TILE, 13 * TILE - TILE / 2);
		items.at(CANDLE2).SetXY(13 * TILE, 10 * TILE - TILE / 2);
		items.at(CLOSET_HIROSI_MAP15).SetXY(8 * TILE, 6 * TILE + TILE / 2);
		items.at(MIKA_TO_ONI).SetXY(7 * TILE + TILE / 2, 7 * TILE + TILE / 2);
		for (int i = 0; i< int(events.size()); i++) {
			events.at(i).SetTriggered(false);
		}
		for (int i = 0; i < int(items.size()); i++) {
			items.at(i).ResetUtil();
		}
		// house1 map blue paint
		house1_map.SetTopLeft(2 * TILE + TILE / 2, 5 * TILE);
		blue_paint.SetTopLeft(2 * TILE + TILE / 2, 5 * TILE);
		piano_hint.SetTopLeft(2 * TILE + TILE / 2, 6 * TILE);
		base0_kabe.SetTopLeft(2 * TILE + TILE / 2, 6 * TILE);
		gate_animation.SetTopLeft(2 * TILE + TILE / 2, 5 * TILE);
		closet_animation.SetTopLeft(2 * TILE + TILE / 2, 5 * TILE);
		gate_animation.SetAnimation(97, true);
		gate_animation.ToggleAnimation();
	}

	void CGameStateRun::OnMove()
	{
		// update mapmask
		for (int i = 0; i < 3; i++) {
			darkmask[i].SetXY(player.GetX() - TILE * 15, player.GetY() - TILE * 16);
		}
		if ((player.GetX() - gamemaps.at(_nowID).GetX()) % TILE == 16 || (player.GetY() - gamemaps.at(_nowID).GetY()) % TILE == 16) {
			_playerStep++;
			game_interface.StorePlayerStep(_playerStep);
		}
		inputbox.OnMove();
		if (events.at(START_EVENT_E).IsTransMap()) {
			_nowID = player.NextMapID();
			events.at(START_EVENT_E).IsTransMap() = false;
		}

		if (_substate != OnDialogs && !player.IsOnChair()) {
			player.OnMove(gamemaps.at(_nowID), router, _nowID, blockLeftCor, blockRightCor);
		}
		switch (_nowID) {
		case 0:
			items.at(KEY_JAIL).StorePlayerPos(player.GetX(), player.GetY());
			items.at(KEY_JAIL).OnMove();
			items.at(BOOKCASE_L).StorePlayerPos(player.GetX(), player.GetY());
			items.at(BOOKCASE_L).OnMove();
			if (items.at(BOOKCASE_L).IsFixed()) {
				items.at(BOOKCASE_R).StorePlayerPos(player.GetX(), player.GetY());
				items.at(BOOKCASE_R).OnMove();
				for (int i = 0; i < 3; i++) { // DL, DM, DR
					gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
						(items.at(BOOKCASE_L).GetPosX() + i * TILE + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);
					gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
						(items.at(BOOKCASE_L).GetPosX() + i * TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);
				}
			}
			else {
				for (int i = 0; i < 3; i++) { // DL, DM, DR
					gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
						(items.at(BOOKCASE_L).GetPosX() + i * TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);
					// (reset)
					gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
						(items.at(BOOKCASE_L).GetPosX() - (i + 1) * TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);
				}
			}
			if (!items.at(BOOKCASE_R).IsFixed()) {
				for (int i = 0; i < 3; i++) { // DL, DM, DR
					gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
						(items.at(BOOKCASE_R).GetPosX() + i * TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);
				}
			}
			else {
				if (items.at(BOOKCASE_R).IsFixed()) {		//if use "Y" and "I" will see next line first posX = 0
					if ((items.at(BOOKCASE_R).GetPosX() + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE != 14) {
						gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
							(items.at(BOOKCASE_R).GetPosX() + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);				//DL (reset)
					}
					for (int i = 1; i < 3; i++) { // DM, DR (reset)
						if ((items.at(BOOKCASE_R).GetPosX() + i * TILE + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE != 14 &&
							(items.at(BOOKCASE_R).GetPosX() + i * TILE + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE != 13) {
							gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
								(items.at(BOOKCASE_R).GetPosX() + i * TILE + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);
						}
					}
					for (int i = 0; i < 3; i++) { // DL, DM, DR
						gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
							(items.at(BOOKCASE_R).GetPosX() + i * TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);
					}
				}
			}
			if (objs.at(obj_move::house1_basement2_chair).isChangeMap()) {
				objs.at(obj_move::house1_basement2_chair).StorePlayerPos(player.GetX(), player.GetY());
				objs.at(obj_move::house1_basement2_chair).OnMove(gamemaps.at(_nowID));
				gamemaps.at(_nowID).SetMapData(0, (objs.at(obj_move::house1_basement2_chair).GetPreY() - gamemaps.at(_nowID).GetY()) / TILE,
					(objs.at(obj_move::house1_basement2_chair).GetPreX() - gamemaps.at(_nowID).GetX()) / TILE, 312);

				objs.at(obj_move::house1_basement2_chair).SetPrePos();

				if (!(objs.at(obj_move::house1_basement2_chair).GetPreX() == 18 * TILE && objs.at(obj_move::house1_basement2_chair).GetPreY() == 8 * TILE) &&
					!(objs.at(obj_move::house1_basement2_chair).GetPreX() == 5 * TILE && objs.at(obj_move::house1_basement2_chair).GetPreY() == 8 * TILE)) {
					gamemaps.at(_nowID).SetMapData(0, (objs.at(obj_move::house1_basement2_chair).GetPreY() - gamemaps.at(_nowID).GetY()) / TILE,
						(objs.at(obj_move::house1_basement2_chair).GetPreX() - gamemaps.at(_nowID).GetX()) / TILE, 0);
				}
			}
			if (!items.at(BASEMENT_PWD).IsPick() && items.at(BOOKCASE_R).IsFixed()) {
				items.at(BASEMENT_PWD).StorePlayerPos(player.GetX(), player.GetY());
				items.at(BASEMENT_PWD).SetDirection(player.GetDirection());
				items.at(BASEMENT_PWD).OnMove();
			}
			if (!items.at(BASEMENT_KEY).IsPick() && items.at(BASEMENT_PWD).IsPick()) {
				items.at(BASEMENT_KEY).StorePlayerPos(player.GetX(), player.GetY());
				items.at(BASEMENT_KEY).SetDirection(player.GetDirection());
				items.at(BASEMENT_KEY).OnMove();
			}
			break;
		case 1:
			items.at(FLATHEAD).StorePlayerPos(player.GetX(), player.GetY());
			items.at(FLATHEAD).OnMove();
			objs.at(obj_move::house1_basement2_chair).StorePlayerPos(player.GetX(), player.GetY());
			objs.at(obj_move::house1_basement2_chair).OnMove(gamemaps.at(_nowID));
			if (!objs.at(obj_move::house1_basement2_chair).isChangeMap()) {
				gamemaps.at(_nowID).SetMapData(0, (objs.at(obj_move::house1_basement2_chair).GetPreY() - gamemaps.at(_nowID).GetY()) / TILE,
					(objs.at(obj_move::house1_basement2_chair).GetPreX() - gamemaps.at(_nowID).GetX()) / TILE, 312);

				objs.at(obj_move::house1_basement2_chair).SetPrePos();

				if (!(objs.at(obj_move::house1_basement2_chair).GetPreX() == 15 * TILE && objs.at(obj_move::house1_basement2_chair).GetPreY() == 12 * TILE)) {
					gamemaps.at(_nowID).SetMapData(0, (objs.at(obj_move::house1_basement2_chair).GetPreY() - gamemaps.at(_nowID).GetY()) / TILE,
						(objs.at(obj_move::house1_basement2_chair).GetPreX() - gamemaps.at(_nowID).GetX()) / TILE, 0);
				}
			}
			break;
		case 2:
			break;
		case 3:
			items.at(GATE2).StorePlayerPos(player.GetX(), player.GetY());
			items.at(GATE2).OnMove();
			if (items.at(GATE2).IsClose()) {
				gamemaps.at(_nowID).SetMapData(0, (items.at(GATE2).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(GATE2).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);
				gamemaps.at(_nowID).SetMapData(0, (items.at(GATE2).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(GATE2).GetPosX() + TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);
			}
			else {
				gamemaps.at(_nowID).SetMapData(0, (items.at(GATE2).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(GATE2).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 312);
				gamemaps.at(_nowID).SetMapData(0, (items.at(GATE2).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(GATE2).GetPosX() + TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);
			}
			break;
		case 9:
			deadbody.SetTopLeft(TILE * 11, TILE * 11);
			oni_eat.SetTopLeft(TILE * 11, TILE * 11);
		case 10:
			items.at(TATAMI_R).StorePlayerPos(player.GetX(), player.GetY());
			items.at(TATAMI_R).OnMove();
			items.at(LIGHTER).StorePlayerPos(player.GetX(), player.GetY());
			items.at(LIGHTER).OnMove();
			if (items.at(LIGHTER).IsPick()) {
				items.at(TATAMI_L).StorePlayerPos(player.GetX(), player.GetY());
				items.at(TATAMI_L).OnMove();
			}
			items.at(DOOR_DIFF).StorePlayerPos(player.GetX(), player.GetY());

			break;
		case 11:
			items.at(BROKEN_DISH).StorePlayerPos(player.GetX(), player.GetY());
			items.at(BROKEN_DISH).OnMove();
			break;
		case 12:
			items.at(LIB_BOOK).StorePlayerPos(player.GetX(), player.GetY());
			items.at(LIB_BOOK).OnMove();
			if (items.at(LIB_BOOK).IsFixed()) {
				items.at(KEY_3F_L).StorePlayerPos(player.GetX(), player.GetY());
				items.at(KEY_3F_L).OnMove();
			}
			break;
		case 13:
			if (_dialogID >= 2 && _dialogID <= 12) {
				player.OnMove();
				human_takeshi.OnMove();
				human_takuro.OnMove();
				human_mika.OnMove();
			}
			if (_dialogID == 8) {
				player.OnMoveBySettings(4);
			}

			if (_dialogID == 11) {
				player.OnMoveBySettings(6);
			}
			break;
		case 14:
			objs.at(obj_move::house1_2F_TR_chair).StorePlayerPos(player.GetX(), player.GetY());
			objs.at(obj_move::house1_2F_TR_chair).OnMove(gamemaps.at(_nowID));

			gamemaps.at(_nowID).SetMapData(0, (objs.at(obj_move::house1_2F_TR_chair).GetPreY() - gamemaps.at(_nowID).GetY()) / TILE,
				(objs.at(obj_move::house1_2F_TR_chair).GetPreX() - gamemaps.at(_nowID).GetX()) / TILE, 312);

			objs.at(obj_move::house1_2F_TR_chair).SetPrePos();

			gamemaps.at(_nowID).SetMapData(0, (objs.at(obj_move::house1_2F_TR_chair).GetPreY() - gamemaps.at(_nowID).GetY()) / TILE,
				(objs.at(obj_move::house1_2F_TR_chair).GetPreX() - gamemaps.at(_nowID).GetX()) / TILE, 0);

			if (objs.at(obj_move::house1_2F_TR_chair).IsFixed()) {
				items.at(KEY_LIB).StorePlayerPos(player.GetX(), player.GetY());
				items.at(KEY_LIB).OnMove();
				if (!items.at(KEY_LIB).IsPick()) {
					gamemaps.at(_nowID).SetMapData(0, (items.at(KEY_LIB).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
						(items.at(KEY_LIB).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);
				}
				else {
					gamemaps.at(_nowID).SetMapData(0, (items.at(KEY_LIB).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
						(items.at(KEY_LIB).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 312);
				}
			}
			if (events.at(KEY_3F_L_E).IsTriggered()) {
				items.at(HANDKERCHIEF).StorePlayerPos(player.GetX(), player.GetY());
				items.at(HANDKERCHIEF).OnMove();
			}
			if (items.at(HANDKERCHIEF).IsPick() || !events.at(KEY_3F_L_E).IsTriggered()) {
				gamemaps.at(_nowID).SetMapData(0, (items.at(HANDKERCHIEF).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(HANDKERCHIEF).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 312);
			}
			else {
				gamemaps.at(_nowID).SetMapData(0, (items.at(HANDKERCHIEF).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(HANDKERCHIEF).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);
			}
			items.at(CLOSET_SHAKE).StorePlayerPos(player.GetX(), player.GetY());
			items.at(CLOSET_SHAKE).OnMove();
			items.at(CLOSET_TAKESI_0).StorePlayerPos(player.GetX(), player.GetY());
			items.at(CLOSET_TAKESI_0).OnMove();
			// CLOSET_TAKESI_1 not have on move

			break;
		case 15:
			if (!items.at(DOOR_KNOB).IsPick()) {
				items.at(DOOR_KNOB).StorePlayerPos(player.GetX(), player.GetY());
				items.at(DOOR_KNOB).OnMove();
			}
			if (items.at(DOOR_KNOB).IsPick()) {
				items.at(DOOR_NO_KNOB).StorePlayerPos(player.GetX(), player.GetY());
				items.at(DOOR_NO_KNOB).OnMove();
			}
			break;
		case 16:
			items.at(BED).StorePlayerPos(player.GetX(), player.GetY());
			items.at(BED).OnMove();
			if (!items.at(BED).IsFixed()) {
				for (int i = 0; i < 2; i++) { // left, right
					for (int j = 0; j < 2; j++) { // top, down
						gamemaps.at(_nowID).SetMapData(0, (items.at(BED).GetPosY() + TILE / 2 + j * TILE - gamemaps.at(_nowID).GetY()) / TILE,
							(items.at(BED).GetPosX() + i * TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);
					}
				}
				for (int j = 0; j < 2; j++) { // top, down (reset)
					gamemaps.at(_nowID).SetMapData(0, (items.at(BED).GetPosY() + TILE / 2 + j * TILE - gamemaps.at(_nowID).GetY()) / TILE,
						(items.at(BED).GetPosX() - TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);
				}
			}
			else {
				gamemaps.at(_nowID).SetMapData(0, (items.at(BED).GetPosY() + TILE / 2 - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BED).GetPosX() + TILE + TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);	//TR (reset)

				for (int i = 0; i < 2; i++) { // left, right
					for (int j = 0; j < 2; j++) { // top, down
						gamemaps.at(_nowID).SetMapData(0, (items.at(BED).GetPosY() + TILE / 2 + j * TILE - gamemaps.at(_nowID).GetY()) / TILE,
							(items.at(BED).GetPosX() + i * TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);
					}
				}
			}
			break;
		case 17:
			items.at(WHITE_BOOKCASE).StorePlayerPos(player.GetX(), player.GetY());
			items.at(WHITE_BOOKCASE).OnMove();
			if (!items.at(PIANO_PWD_NOTOPEN).IsPick() && items.at(WHITE_BOOKCASE).IsFixed()) {
				items.at(PIANO_PWD_NOTOPEN).StorePlayerPos(player.GetX(), player.GetY());
				items.at(PIANO_PWD_NOTOPEN).SetDirection(player.GetDirection());
				items.at(PIANO_PWD_NOTOPEN).OnMove();
			}
			if (!items.at(BB_KEY).IsPick() && items.at(PIANO_PWD_NOTOPEN).IsPick()) {
				items.at(BB_KEY).StorePlayerPos(player.GetX(), player.GetY());
				items.at(BB_KEY).SetDirection(player.GetDirection());
				items.at(BB_KEY).OnMove();
			}
			break;
		case 18:
			if (!items.at(TUB_ONCE).IsFixed() || !items.at(TUB_ONCE).IsAnimationDone()) {
				items.at(TUB_ONCE).StorePlayerPos(player.GetX(), player.GetY());
				items.at(TUB_ONCE).OnMove();
			}
			else if (items.at(TUB_ONCE).IsFixed() && items.at(TUB_ONCE).IsAnimationDone()) {
				items.at(PHILLIPS).StorePlayerPos(player.GetX(), player.GetY());
				items.at(PHILLIPS).OnMove();
			}
			else if (items.at(PHILLIPS).IsPick()) {
				items.at(TUB_FIXED).StorePlayerPos(player.GetX(), player.GetY());
				items.at(TUB_FIXED).OnMove();
			}
			break;
		case 19:
			items.at(TOILET).StorePlayerPos(player.GetX(), player.GetY());
			items.at(TOILET).OnMove();

			items.at(DETERGENT).StorePlayerPos(player.GetX(), player.GetY());
			items.at(DETERGENT).OnMove();
			break;
		case 20:
			items.at(OIL).StorePlayerPos(player.GetX(), player.GetY());
			items.at(OIL).OnMove();
			human_mika.SetPos(8 * TILE, 16 * TILE);
			human_mika.OnMove();
			if (!events.at(OPEN_BASEMENT_E).IsTriggered()) {
				gamemaps.at(_nowID).SetMapData(0, (human_mika.GetY() - gamemaps.at(_nowID).GetY()) / TILE, (human_mika.GetX() - gamemaps.at(_nowID).GetX()) / TILE, 0);
			}
			else if (!events.at(MIKA_TO_ONI_E).IsTriggered()) {
				gamemaps.at(_nowID).SetMapData(0, (human_mika.GetY() - gamemaps.at(_nowID).GetY()) / TILE, (human_mika.GetX() - gamemaps.at(_nowID).GetX()) / TILE, 312);
				gamemaps.at(_nowID).SetMapData(0, (11 * TILE - gamemaps.at(_nowID).GetY()) / TILE, (11 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);
				gamemaps.at(_nowID).SetMapData(0, (11 * TILE - gamemaps.at(_nowID).GetY()) / TILE, (12 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);
			}
			else {
				gamemaps.at(_nowID).SetMapData(0, (11 * TILE - gamemaps.at(_nowID).GetY()) / TILE, (11 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);
				gamemaps.at(_nowID).SetMapData(0, (11 * TILE - gamemaps.at(_nowID).GetY()) / TILE, (12 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);
			}
			objs.at(house1_2F_TL_chair).StorePlayerPos(player.GetX(), player.GetY());
			objs.at(house1_2F_TL_chair).OnMove(gamemaps.at(_nowID));

			gamemaps.at(_nowID).SetMapData(0, (objs.at(house1_2F_TL_chair).GetPreY() - gamemaps.at(_nowID).GetY()) / TILE,
				(objs.at(house1_2F_TL_chair).GetPreX() - gamemaps.at(_nowID).GetX()) / TILE, 312);

			objs.at(house1_2F_TL_chair).SetPrePos();

			if (!(objs.at(house1_2F_TL_chair).GetPreX() == 12 * TILE && objs.at(house1_2F_TL_chair).GetPreY() == 17 * TILE)) {
				gamemaps.at(_nowID).SetMapData(0, (objs.at(house1_2F_TL_chair).GetPreY() - gamemaps.at(_nowID).GetY()) / TILE,
					(objs.at(house1_2F_TL_chair).GetPreX() - gamemaps.at(_nowID).GetX()) / TILE, 0);
			}
			break;
		case 21:
			items.at(BOOKCASE_MAP21).StorePlayerPos(player.GetX(), player.GetY());
			items.at(BOOKCASE_MAP21).OnMove();
			break;
		case 22:
			items.at(KEY_BASEMENT).StorePlayerPos(player.GetX(), player.GetY());
			items.at(KEY_BASEMENT).OnMove();
			if (!items.at(KEY_BASEMENT).IsPick()) {
				gamemaps.at(_nowID).SetMapData(0, (items.at(KEY_BASEMENT).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(KEY_BASEMENT).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);
			}
			else {
				gamemaps.at(_nowID).SetMapData(0, (items.at(KEY_BASEMENT).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(KEY_BASEMENT).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 312);
			}
			items.at(GATE).StorePlayerPos(player.GetX(), player.GetY());
			items.at(GATE).OnMove();
			if (items.at(GATE).IsClose()) {
				gamemaps.at(_nowID).SetMapData(0, (items.at(GATE).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(GATE).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);
				gamemaps.at(_nowID).SetMapData(0, (items.at(GATE).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(GATE).GetPosX() + TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);
			}
			else {
				gamemaps.at(_nowID).SetMapData(0, (items.at(GATE).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(GATE).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 312);
				gamemaps.at(_nowID).SetMapData(0, (items.at(GATE).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(GATE).GetPosX() + TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);
			}
			break;
		}
		// Item end
		if (normal_oni.IsShow() && !normal_oni.IsWait()) {
			normal_oni.SetPlayerPos(player.GetX(), player.GetY());
			if (normal_oni.isCatch() && !isdebugmode) {
				CAudio::Instance()->Stop(AUDIO_HOROR);
				normal_oni.ResetOni();
				GotoGameState(GAME_STATE_OVER);
			}
			else {
				normal_oni.OnMove(gamemaps.at(_nowID), _nowID);
			}
		}
		if (normal_oni.IsWait()) {
			normal_oni.OnMove(gamemaps.at(_nowID), _nowID);
		}
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (_substate == OnWalking && !_in_closet) {
			game_interface.OnKeyDown(nChar);
		}
		if (game_interface.IsShow() && !_in_interface) {
			_in_interface = true;
		}
		if (game_interface.IsUseItem()) {
			InterfaceData nowItem = game_interface.UseItem();
			if (_nowID == 15 && !items.at(DOOR_KNOB).IsPick() && items.at(DOOR_KNOB).IsClose()
				&& nowItem.GetIntro() == "phillips screwdriver" && player.GetDirection() == Entity::up
				&& player.GetX() == 12 * TILE && player.GetY() == 8 * TILE) {
				items.at(DOOR_KNOB).SetIsPick(true);
				game_interface.StoreItem("door knob", "door knob", Interface::Items::door_knob);
			}
			else if (_nowID == 10 && items.at(DOOR_DIFF).GetBitMapIndex() == 0
				&& nowItem.GetName() == "plate shards" && player.GetDirection() == Entity::up
				&& player.GetX() == 17 * TILE && player.GetY() == 17 * TILE) {
				items.at(DOOR_DIFF).EventTrigger();
			}
			else if (_nowID == 10 && items.at(DOOR_DIFF).GetBitMapIndex() == 1
				&& nowItem.GetName() == "door knob" && player.GetDirection() == Entity::up
				&& player.GetX() == 17 * TILE && player.GetY() == 17 * TILE) {
				items.at(DOOR_DIFF).EventTrigger();
				game_interface.DeleteItem("door knob");
			}
			else if (_nowID == 17 && !_piano_hint_show
				&& nowItem.GetIntro() == "handkerchief (clean)" && player.GetDirection() == Entity::right
				&& player.GetX() == 11 * TILE && player.GetY() == 12 * TILE) {
				_use_handkerchief = true;
				game_interface.DeleteItem("handkerchief");
				normal_oni.SetPos(9 * TILE, 9 * TILE);
				normal_oni.IsShow() = true;
				normal_oni.Once() = false;
			}
			else if (_nowID == 0 && !_base0_kabe_show && base0_kabe.GetFrameIndexOfBitmap() == 0
				&& nowItem.GetIntro() == "flathead screwdriver" && player.GetDirection() == Entity::up
				&& player.GetX() == 16 * TILE && player.GetY() == 6 * TILE) {
				_use_f_screwdriver = true;
				_base0_kabe_show = true;
			}
			else if (_nowID == 21 && nowItem.GetIntro() == "lighter (full of oil)" && !events.at(LIGHTUP_ROOM21).IsTriggered()) {
				events.at(LIGHTUP_ROOM21).SetTriggered(true);
				darkmask[1].SetState(DarkRoomEffect::dim);
			}
			else if (_nowID == 3 && nowItem.GetIntro() == "lighter (full of oil)" && !events.at(LIGHTUP_ROOM3).IsTriggered()) {
				events.at(LIGHTUP_ROOM3).SetTriggered(true);
				darkmask[0].SetState(DarkRoomEffect::dim);
			}
		}
		else if (!game_interface.IsShow() && !game_interface.IsUseItem()) {
			_in_interface = false;
		}
		if (_substate == OnInputBox) {
			inputbox.BoxOn(nChar);
			if (nChar == VK_SPACE) { // press "space" close dialog
				if (inputbox.isInteger()) {
					int index = stoi(string(inputbox.GetString()));
					if (index >= 0 && index <= 64) {
						_nowID = index;
					}
				}
				_substate = OnWalking;
			}
		}
		else if (_substate == OnWalking && !_in_interface) {

			if (isdebugmode) {
				if (nChar == KEY_A) {
					isteleportblock = !isteleportblock;
				}
				if (nChar == KEY_O) {
					if (router.Gaming()) {
						router.copypath();
						router.SetAllPathFalse();
					}
					else {
						router.Restore();
					}
					router.Gaming() = !router.Gaming();
				}
				if (nChar == KEY_I) {
					gamemaps.at(_nowID).isshowtileindex = (gamemaps.at(_nowID).isshowtileindex) ? false : true;
				}
				if (nChar == KEY_9) {
					if (gamemaps.at(_nowID).indexlayer > 0)gamemaps.at(_nowID).indexlayer--;
				}
				if (nChar == KEY_0) {
					if (gamemaps.at(_nowID).indexlayer < gamemaps.at(_nowID).GetLayer() - 1)gamemaps.at(_nowID).indexlayer++;
				}
				if (nChar == KEY_J) {
					if (_nowID > 0)
						_nowID -= 1;
				}
				if (nChar == KEY_K) {
					if (_nowID < 64)
						_nowID++;
				}

				if (nChar == KEY_G) {
					isgrid = !isgrid;
				}

				if (nChar == KEY_E) {
					if (pointvec.size() % 6 == 3) {
						TRACE("still one point in buffer, pop out or add a new point.\n");
					}
					else {
						isedit = !isedit;
					}
				}
				if (isedit && !inputbox.IsWrite()) {
					if (nChar == KEY_7) {
						istwoway = 1;
					}
					else if (nChar == KEY_8) {
						istwoway = 2;
					}

				}
				if (nChar == KEY_W) {
					_substate = OnInputBox;
					inputbox.ClearBuffer();
					inputbox.TimerStart();
					inputbox.ToggleBox();
				}
			}
			if (nChar == KEY_Y) {
				isdebugmode = !isdebugmode;
			}
			if (nChar == KEY_Q) {
				for (int i = 0; i < 12; i++) {
					CAudio::Instance()->Stop(i);
				}
				GotoGameState(GAME_STATE_OVER);
			}
			switch (_nowID) {
			case 0:
				if (!items.at(BASEMENT_KEY).IsPick() && pwds.at(basement).IsOpen() && _use_f_screwdriver && !_base0_kabe_show) {
					items.at(BASEMENT_KEY).SetDirection(player.GetDirection());
					items.at(BASEMENT_KEY).OnKeyDown(nChar);
				}
				if (!items.at(BASEMENT_PWD).IsClose() && pwds.at(basement).IsShow() && _use_f_screwdriver && !_base0_kabe_show) {
					_pwd = true;
					pwds.at(basement).OnKeyDown(nChar);
					if (pwds.at(basement).IsOpen()) {
						items.at(BASEMENT_PWD).SetIsPick(true);
					}
				}
				else {
					_pwd = false;
				}
				items.at(KEY_JAIL).OnKeyDown(nChar);
				items.at(BOOKCASE_L).OnKeyDown(nChar);
				if (nChar != VK_SPACE) {
					items.at(BOOKCASE_R).OnKeyDown(nChar);
					items.at(BASEMENT_PWD).OnKeyDown(nChar);
				}
				if (items.at(BOOKCASE_L).IsFixed()) {
					items.at(BOOKCASE_R).OnKeyDown(nChar);
				}
				if (items.at(BOOKCASE_R).IsFixed() && !items.at(BASEMENT_PWD).IsPick() && _use_f_screwdriver && !_base0_kabe_show) {
					items.at(BASEMENT_PWD).OnKeyDown(nChar);
				}
				if (objs.at(obj_move::house1_basement2_chair).IsFixed() && objs.at(obj_move::house1_basement2_chair).isCollide() && player.GetDirection() == Entity::down && nChar == VK_SPACE) {
					player.SetOnChair(-TILE / 2);
					player.OnMove();
				}
				else if (objs.at(obj_move::house1_basement2_chair).IsFixed() && player.IsOnChair() && nChar == VK_SPACE && !items.at(KEY_JAIL).IsPick()) {
					items.at(KEY_JAIL).SetIsPick(true);
				}
				else if (objs.at(obj_move::house1_basement2_chair).IsFixed() && player.IsOnChair() && nChar == VK_SPACE) {
					player.SetOffChair(-TILE / 2);
					player.OnMove();
				}
				if (objs.at(obj_move::house1_basement2_chair).isChangeMap())
					objs.at(obj_move::house1_basement2_chair).OnKeyDown(nChar);


				if (!items.at(KABE_PWD).IsPick() && nChar == VK_SPACE && !_use_f_screwdriver
					&& player.GetDirection() == Entity::up
					&& player.GetX() == 16 * TILE && player.GetY() == 6 * TILE
					&& base0_kabe.GetFrameIndexOfBitmap() == 0) {
					_base0_kabe_show = !_base0_kabe_show;
				}
				else if (!items.at(KABE_PWD).IsPick() && nChar == VK_SPACE && _use_f_screwdriver
					&& base0_kabe.GetFrameIndexOfBitmap() == 0) {
					base0_kabe.SetFrameIndexOfBitmap(1);
				}
				else if (!items.at(KABE_PWD).IsPick() && nChar == VK_SPACE && _use_f_screwdriver
					&& base0_kabe.GetFrameIndexOfBitmap() == 1) {
					base0_kabe.SetFrameIndexOfBitmap(2);
				}
				else if (!items.at(KABE_PWD).IsPick() && nChar == VK_SPACE && _use_f_screwdriver
					&& base0_kabe.GetFrameIndexOfBitmap() == 2) {
					_base0_kabe_show = false;
					items.at(KABE_PWD).SetIsPick(true);
				}
				break;
			case 1:
				items.at(FLATHEAD).OnKeyDown(nChar);
				objs.at(obj_move::house1_basement2_chair).OnKeyDown(nChar);
				if (objs.at(obj_move::house1_basement2_chair).IsFixed2() && objs.at(obj_move::house1_basement2_chair).isCollide() && player.GetDirection() == Entity::up && nChar == VK_SPACE && !items.at(FLATHEAD).IsPick()) {
					player.SetOnChair(3 * TILE / 2);
					player.OnMove();
				}
				else if (objs.at(obj_move::house1_basement2_chair).IsFixed2() && player.IsOnChair() && nChar == VK_SPACE && !items.at(FLATHEAD).IsPick()) {
					items.at(FLATHEAD).SetIsPick(true);
				}
				else if (objs.at(obj_move::house1_basement2_chair).IsFixed2() && player.IsOnChair() && nChar == VK_SPACE) {
					player.SetOffChair(3 * TILE / 2);
					player.OnMove();
				}
				break;
			case 2:
				if (nChar == VK_SPACE && player.GetDirection() == Entity::up
					&& (items.at(CLOSET_HIROSI_L).GetBitMapIndex() == 0
						|| items.at(CLOSET_HIROSI_L).GetBitMapIndex() == 5)
					&& player.GetX() == 16 * TILE && player.GetY() == 9 * TILE
					&& !(game_interface.FindItem("???") && !events.at(MIKA_IN_CLOSET_E).IsTriggered())) {
					items.at(CLOSET_HIROSI_L).EventTrigger();
					if (darkmask[2].IsShow()) {
						darkmask[2].SetShow(false);
					}
					if ((normal_oni.IsShow() && !normal_oni.IsWait())) {
						_is_danger = true;
					}
					else if ((normal_oni.IsShow() && normal_oni.IsWait() && !darkmask[2].IsShow())) {
						audio_control.at(AUDIO_HOROR) = false;
						CAudio::Instance()->Stop(AUDIO_HOROR);
						darkmask[2].SetShow(true);
						normal_oni.ResetOni();
					}
				}
				if (_final && nChar == VK_SPACE) {
					clear_game.SetShow(false);
					CAudio::Instance()->Stop(AUDIO_THE_END);
					GotoGameState(GAME_STATE_INIT);
				}
				break;
			case 3:
				if (nChar == VK_SPACE && player.GetDirection() == Entity::up
					&& player.GetX() == 14 * TILE && player.GetY() == 13 * TILE
					&& !events.at(USE_JAIL_KEY_E).IsTriggered() && game_interface.FindItem("jail key")) {
					events.at(USE_JAIL_KEY_E).SetTriggered(true);
					game_interface.DeleteItem("jail key");
				}
				if (events.at(USE_JAIL_KEY_E).IsTriggered()) {
					items.at(GATE2).OnKeyDown(nChar);
				}
				else if (nChar != VK_SPACE) {
					items.at(GATE2).OnKeyDown(nChar);
				}
				if (nChar == VK_SPACE && player.GetDirection() == Entity::up && !darkmask[0].IsShow()) {
					_blue_paint_show = !_blue_paint_show;
					if (player.GetX() == 9 * TILE && player.GetY() == 13 * TILE) {
						blue_paint.SetFrameIndexOfBitmap(0);
					}
					else if (player.GetX() == 10 * TILE && player.GetY() == 13 * TILE) {
						blue_paint.SetFrameIndexOfBitmap(1);
					}
					else if (player.GetX() == 9 * TILE && player.GetY() == 9 * TILE) {
						blue_paint.SetFrameIndexOfBitmap(2);
					}
					else if (player.GetX() == 10 * TILE && player.GetY() == 9 * TILE) {
						blue_paint.SetFrameIndexOfBitmap(3);
					}
					else {
						_blue_paint_show = !_blue_paint_show;
					}
				}
				if (nChar == VK_SPACE && events.at(LIGHTUP_ROOM3).IsTriggered() && items.at(CANDLE2).GetBitMapIndex() == 0
					&& ((player.GetDirection() == Entity::up && player.GetX() == 13 * TILE && player.GetY() == 11 * TILE)
						|| (player.GetDirection() == Entity::right && player.GetX() == 12 * TILE && player.GetY() == 10 * TILE)
						|| (player.GetDirection() == Entity::down && player.GetX() == 13 * TILE && player.GetY() == 9 * TILE)
						|| (player.GetDirection() == Entity::left && player.GetX() == 14 * TILE && player.GetY() == 10 * TILE))) {
					// trigger dialog
					CAudio::Instance()->Play(AUDIO_CANDLE_LIGHT, false);
					items.at(CANDLE2).EventTrigger();
					darkmask[0].SetState(DarkRoomEffect::bright);
				}
				break;
			case 6:
				if (nChar == VK_SPACE && _killtimes == 7) {
					_killtimes++;
				}
				else if (nChar == VK_SPACE && _killtimes == 8 && items.at(DOOR_OPEN).IsAnimationDone()) {
					_killtimes++;
				}
				else if (nChar == VK_SPACE && _killtimes == 9 && items.at(DOOR_DIE).IsAnimationDone()) {
					_killtimes++;
				}
				if (_killtimes == 10) {
					GotoGameState(GAME_STATE_OVER);
				}
				break;
			case 7:
				if (nChar == VK_SPACE && player.GetDirection() == Entity::up && player.GetX() == 11 * TILE && player.GetY() == 7 * TILE) {
					_map_show = !_map_show;
				}
				break;
			case 9:

				break;
			case 10:
				items.at(LIGHTER).OnKeyDown(nChar);
				if (nChar != VK_SPACE) {
					items.at(TATAMI_L).OnKeyDown(nChar);
				}
				if (items.at(LIGHTER).IsPick()) {
					items.at(TATAMI_L).OnKeyDown(nChar);
				}
				items.at(TATAMI_R).OnKeyDown(nChar);
				items.at(DOOR_DIFF).OnKeyDown(nChar);
				if (nChar == VK_SPACE && items.at(DOOR_DIFF).GetBitMapIndex() == 0
					&& items.at(DOOR_DIFF).Collide() && !events.at(DOOR_WIRED_E).IsTriggered()) {
					SetEventTriggeredDialog(DOOR_WIRED_E);
					events.at(DOOR_WIRED_E).SetTriggered(false);
				}
				else if (nChar == VK_SPACE && items.at(DOOR_DIFF).GetBitMapIndex() == 1
					&& items.at(DOOR_DIFF).Collide() && !events.at(DOOR_WHERE_KNOB_E).IsTriggered()) {
					SetEventTriggeredDialog(DOOR_WHERE_KNOB_E);
					events.at(DOOR_WHERE_KNOB_E).SetTriggered(false);
				}
				else if (nChar == VK_SPACE && items.at(DOOR_DIFF).GetBitMapIndex() == 2
					&& items.at(DOOR_DIFF).Collide() && !events.at(DOOR_DIFF_OPEN_E).IsTriggered()) {
					SetEventTriggeredDialog(DOOR_DIFF_OPEN_E);
				}
				break;
			case 11:
				items.at(BROKEN_DISH).OnKeyDown(nChar);
				break;
			case 12:
				items.at(LIB_BOOK).OnKeyDown(nChar);
				// 3F L
				if (nChar != VK_SPACE)
					items.at(KEY_3F_L).OnKeyDown(nChar);
				if (items.at(LIB_BOOK).IsFixed())
					items.at(KEY_3F_L).OnKeyDown(nChar);
				break;
			case 14:
				objs.at(obj_move::house1_2F_TR_chair).OnKeyDown(nChar);
				if (nChar != VK_SPACE)
					items.at(KEY_LIB).OnKeyDown(nChar);
				if (objs.at(obj_move::house1_2F_TR_chair).IsFixed())
					items.at(KEY_LIB).OnKeyDown(nChar);
				items.at(HANDKERCHIEF).OnKeyDown(nChar);
				if (items.at(CLOSET_SHAKE).IsFixed()) {
					items.at(CLOSET_TAKESI_0).OnKeyDown(nChar);
				}
				
				//CLOSET_TAKESI_1 not have on key down*/
				if (events.at(KEY_3F_L_E).IsTriggered() && nChar == VK_SPACE
					&& (items.at(CLOSET_HIROSI_R).GetBitMapIndex() == 0
						|| items.at(CLOSET_HIROSI_R).GetBitMapIndex() == 5)
					&& player.GetDirection() == Entity::up
					&& player.GetX() == 8 * TILE && player.GetY() == 9 * TILE) {
					items.at(CLOSET_HIROSI_R).EventTrigger();
					if (darkmask[2].IsShow()) {
						darkmask[2].SetShow(false);
					}
					if ((normal_oni.IsShow() && !normal_oni.IsWait())) {
						_is_danger = true;
					}
					else if ((normal_oni.IsShow() && normal_oni.IsWait() && !darkmask[2].IsShow())) {
						audio_control.at(AUDIO_HOROR) = false;
						CAudio::Instance()->Stop(AUDIO_HOROR);
						darkmask[2].SetShow(true);
						normal_oni.ResetOni();
					}
				}
				break;
			case 15:
				if (nChar == VK_SPACE && player.GetDirection() == Entity::up
					&& (items.at(CLOSET_HIROSI_MAP15).GetBitMapIndex() == 0
						|| items.at(CLOSET_HIROSI_MAP15).GetBitMapIndex() == 5)
					&& player.GetX() == 8 * TILE && player.GetY() == 8 * TILE) {
					items.at(CLOSET_HIROSI_MAP15).EventTrigger();
					if (darkmask[2].IsShow()) {
						darkmask[2].SetShow(false);
					}
					if ((normal_oni.IsShow() && !normal_oni.IsWait())) {
						_is_danger = true;
					}
					else if ((normal_oni.IsShow() && normal_oni.IsWait() && !darkmask[2].IsShow())) {
						audio_control.at(AUDIO_HOROR) = false;
						CAudio::Instance()->Stop(AUDIO_HOROR);
						darkmask[2].SetShow(true);
						normal_oni.ResetOni();
					}
				}
				if (!items.at(DOOR_KNOB).IsPick()) {
					items.at(DOOR_KNOB).OnKeyDown(nChar);
					if (nChar != VK_SPACE)
						items.at(DOOR_NO_KNOB).OnKeyDown(nChar);
				}
				if (items.at(DOOR_KNOB).IsPick())
					items.at(DOOR_NO_KNOB).OnKeyDown(nChar);
				break;
			case 16:
				items.at(BED).OnKeyDown(nChar);
				if (items.at(BED).IsOnCorPos() && !events.at(OPEN_FUCKING_HOLE_E).IsTriggered()) {
					SetEventTriggeredDialog(OPEN_FUCKING_HOLE_E);
				}
				break;
			case 17:
				if (!items.at(BB_KEY).IsPick() && pwds.at(piano).IsOpen()) {
					items.at(BB_KEY).SetDirection(player.GetDirection());
					items.at(BB_KEY).OnKeyDown(nChar);
				}
				if (!items.at(PIANO_PWD_NOTOPEN).IsPick() && nChar == VK_SPACE) {
					_piano_hint_show = !_piano_hint_show;
					if (player.GetX() == 11 * TILE && player.GetY() == 12 * TILE && player.GetDirection() == Entity::right) {
						if (_use_handkerchief) {
							piano_hint.SetFrameIndexOfBitmap(1);
						}
						else {
							piano_hint.SetFrameIndexOfBitmap(0);
						}
					}
					else if (items.at(WHITE_BOOKCASE).IsFixed() && player.GetX() == 16 * TILE && player.GetY() == 8 * TILE && player.GetDirection() == Entity::up) {
						piano_hint.SetFrameIndexOfBitmap(2);
					}
					else {
						_piano_hint_show = !_piano_hint_show;
					}
				}
				if (!items.at(PIANO_PWD_NOTOPEN).IsClose() && pwds.at(piano).IsShow()) {
					_pwd = true;
					pwds.at(piano).OnKeyDown(nChar);
					if (pwds.at(piano).IsOpen()) {
						items.at(PIANO_PWD_NOTOPEN).SetIsPick(true);
					}
				}
				else {
					_pwd = false;
				}
				items.at(WHITE_BOOKCASE).OnKeyDown(nChar);
				if (nChar != VK_SPACE) {
					items.at(PIANO_PWD_NOTOPEN).OnKeyDown(nChar);
				}
				if (items.at(WHITE_BOOKCASE).IsFixed() && !items.at(PIANO_PWD_NOTOPEN).IsPick()) {
					items.at(PIANO_PWD_NOTOPEN).OnKeyDown(nChar);
				}
				break;
			case 18:
				if (!items.at(TUB_ONCE).IsFixed()) {
					items.at(TUB_ONCE).OnKeyDown(nChar);
					if (nChar != VK_SPACE)
						items.at(PHILLIPS).OnKeyDown(nChar);
				}
				if (items.at(TUB_ONCE).IsFixed()) {
					items.at(PHILLIPS).OnKeyDown(nChar);
				}
				if (items.at(PHILLIPS).IsPick()) {
					items.at(TUB_FIXED).OnKeyDown(nChar);
				}
				break;
			case 19:
				items.at(TOILET).OnKeyDown(nChar);
				items.at(DETERGENT).OnKeyDown(nChar);
				break;
			case 20:
			{
				items.at(OIL).OnKeyDown(nChar);
				objs.at(obj_move::house1_2F_TL_chair).OnKeyDown(nChar);
				human_mika.StorePlayerPos(player.GetX(), player.GetY());
				human_mika.OnKeyDown(nChar);
				if (objs.at(obj_move::house1_2F_TL_chair).IsFixed() && objs.at(obj_move::house1_2F_TL_chair).isCollide() && player.GetDirection() == Entity::up && nChar == VK_SPACE) {
					player.SetOnChair(3 * TILE / 2);
					player.OnMove();
				}
				else if (objs.at(obj_move::house1_2F_TL_chair).IsFixed() && player.IsOnChair() && nChar == VK_SPACE && !items.at(OIL).IsPick()) {
					items.at(OIL).SetIsPick(true);
				}
				else if (objs.at(obj_move::house1_2F_TL_chair).IsFixed() && player.IsOnChair() && nChar == VK_SPACE) {
					player.SetOffChair(3 * TILE / 2);
					player.OnMove();
				}
				break;
			}
			case 21:
				items.at(BOOKCASE_MAP21).OnKeyDown(nChar);
				if (items.at(BOOKCASE_MAP21).IsOnCorPos() && !events.at(OPEN_FUCKING_ROOM_E).IsTriggered()) {
					SetEventTriggeredDialog(OPEN_FUCKING_ROOM_E);
				}
				if (nChar == VK_SPACE && events.at(LIGHTUP_ROOM21).IsTriggered() && items.at(CANDLE1).GetBitMapIndex() == 0
					&& ((player.GetDirection() == Entity::up && player.GetX() == 12 * TILE && player.GetY() == 14 * TILE)
						|| (player.GetDirection() == Entity::right && player.GetX() == 11 * TILE && player.GetY() == 13 * TILE)
						|| (player.GetDirection() == Entity::down && player.GetX() == 12 * TILE && player.GetY() == 12 * TILE)
						|| (player.GetDirection() == Entity::left && player.GetX() == 13 * TILE && player.GetY() == 13 * TILE))) {
					// trigger dialog
					CAudio::Instance()->Play(AUDIO_CANDLE_LIGHT, false);
					items.at(CANDLE1).EventTrigger();
					darkmask[1].SetState(DarkRoomEffect::bright);
				}
				break;
			case 22:
				if (!items.at(KEY_BASEMENT).IsPick()) {
					items.at(KEY_BASEMENT).OnKeyDown(nChar);
				}
				if (events.at(GATE_ONI_APPEAR_E).IsTriggered() && normal_oni.IsShow()) {
					if (nChar != VK_SPACE) {
						items.at(GATE).OnKeyDown(nChar);
					}
				}
				else {
					items.at(GATE).OnKeyDown(nChar);
				}
				break;
			}
			if (!items.at(CLOSET_HIROSI_R).IsClose() || !items.at(CLOSET_HIROSI_MAP15).IsClose() || !items.at(CLOSET_HIROSI_L).IsClose()) {
				_in_closet = true;
			}
			else if (items.at(CLOSET_HIROSI_R).IsClose() && items.at(CLOSET_HIROSI_MAP15).IsClose() && items.at(CLOSET_HIROSI_L).IsClose()) {
				_in_closet = false;
			}
			if (!game_interface.IsShow() && !_pwd && !_map_show && !_blue_paint_show && !_piano_hint_show && !_base0_kabe_show
				&& !_in_closet && _killtimes < 7 && items.at(DOOR_ONI).IsAnimationDone()
				&& (!items.at(MIKA_TO_ONI).IsFixed() || (items.at(MIKA_TO_ONI).IsFixed() && items.at(MIKA_TO_ONI).GetBitMapIndex() == 5))
				&& !_bar_animation_show && !_is_danger && !_final) {
				player.OnKeyDown(nChar);
			}
			if (_is_danger && !_die && !audio_control.at(AUDIO_ONI_OPEN_CLOSET)) {
				/* call oni open closet animation */
				audio_control.at(AUDIO_ONI_OPEN_CLOSET) = true;
				closet_animation.ToggleAnimation();
				closet_animation.SetAnimation(29, true);
				CAudio::Instance()->Stop(AUDIO_HOROR);
				CAudio::Instance()->Play(AUDIO_ONI_OPEN_CLOSET, false);
			}
		}
		else if (_substate == OnDialogs) {
			if (_dialogID == 28) {
				dialogs.at(_dialogID).GetSelect(nChar);
			}
			if (nChar == VK_SPACE) {
				if (!((_dialogID == 12 || _dialogID == 11 || _dialogID == 8) && !player.IsMachineDone())) {
					dialogs.at(_dialogID).SetShow(false);
					_dialogID += 1;
					_dialogcount += 1;
					if (events.at(_eventID).GetDialogCount() != _dialogcount) {
						dialogs.at(_dialogID).SetShow(true);
					}
					else {
						dialogs.at(_dialogID).SetShow(false);
						if (_eventID == START_EVENT_E) {
							events.at(START_EVENT_E).IsTransMap() = true;
							player.SetNextMap(0, 3, 5);
						}
						if (_dialogID == 17) {
							SetEventTriggeredDialog(LIB_KEY_CHASE);
							//events.at(LIB_KEY_CHASE).SetTriggered(true);
							normal_oni.SetPos(11 * TILE, 10 * TILE);
							normal_oni.IsShow() = true;
						}
					
						_substate = OnWalking;
						_dialogcount = 0;
						_dialogID = -1;
						_eventID = -1;
					}
				}
			}
		}
		if (!game_interface.IsShow() && !game_interface.IsUseItem()) {
			_in_interface = false;
		}
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		player.OnKeyUp(nChar);
		if (!_final && !_in_interface && _killtimes < 7 && (nChar == VK_SPACE || nChar == VK_UP || nChar == VK_RIGHT || nChar == VK_LEFT || nChar == VK_DOWN)) {
			player.CheckMapChangeTN(gamemaps.at(_nowID), router, _nowID, blockTeleportCor, game_interface);
		}
		if (player.IsDoorLock() && events.at(DOOR_LOCKED_E).IsTriggered() && nChar == VK_SPACE) {
			if (_nowID == 6 && player.GetX() == 10 * TILE && player.GetY() == 12 * TILE && !events.at(KEY_3F_L_E).IsTriggered()) {
				_killtimes++;
			}
			player.IsDoorLock() = false;
			events.at(DOOR_LOCKED_E).SetTriggered(false);
			CAudio::Instance()->Play(AUDIO_DOOR_LOCK, false);
		}
		if (player.IsDoorOpen() && events.at(DOOR_UNLOCKED_E).IsTriggered() && nChar == VK_SPACE) {
			player.IsDoorOpen() = false;
			events.at(DOOR_UNLOCKED_E).SetTriggered(false);
		}
		if ((player.IsMapChanged())) {
			if (_nowID == 6 && player.GetDirection() == Entity::up
				&& player.GetX() == 10 * TILE && player.GetY() == 12 * TILE
				&& !items.at(DOOR_HALF).IsPick()) {
				items.at(DOOR_HALF).SetIsPick(true);
			}
			_nowID = player.NextMapID();
			player.SetNextMapPos(gamemaps.at(_nowID));

			if (normal_oni.IsShow()) {
				normal_oni.Once() = true;
				normal_oni.SetChangeMap(player.NextX(), player.NextY(), _nowID);
			}
		}
		if (_nowID == 0) {
			if (objs.at(obj_move::house1_basement2_chair).isChangeMap())
				objs.at(obj_move::house1_basement2_chair).OnKeyUp(nChar);
		}
		else if (_nowID == 1) {
			objs.at(obj_move::house1_basement2_chair).OnKeyUp(nChar);
		}
		else if (_nowID == 14) {
			objs.at(obj_move::house1_2F_TR_chair).OnKeyUp(nChar);
		}
		else if (_nowID == 20) {
			objs.at(obj_move::house1_2F_TL_chair).OnKeyUp(nChar);
		}

	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (isdebugmode) {
			if (isedit && ((istwoway != 0) || (pointvec.size() % 6 == 3))) {

				mousex_foc = mousex;
				mousey_foc = mousey;
				tileplaceholder.SetTopLeft(mousex_foc * TILE, mousey_foc * TILE);
				if (istwoway != 0) {
					twowayvec.push_back(istwoway);
					(istwoway == 1) ? TRACE("push twoway {%d} true\n", istwoway) : TRACE("push twoway {%d} false\n", istwoway);
					istwoway = 0;
				}
				pointvec.push_back(_nowID);
				pointvec.push_back(mousex_foc * TILE - gamemaps.at(_nowID).GetX());
				pointvec.push_back(mousey_foc * TILE - gamemaps.at(_nowID).GetY());
				TRACE("push {%d, %d ,%d }\n", _nowID, mousex_foc * TILE - gamemaps.at(_nowID).GetX(), mousey_foc * TILE - gamemaps.at(_nowID).GetY());
			}
			else if (isedit) {
				TRACE("\n please specify istwoway?(oneway/twoway)(7/8)\n");
			}
		}
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)
	{
		mousex = point.x / TILE;
		mousey = point.y / TILE;
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)
	{
		if (isdebugmode) {
			if (isedit) {
				if (pointvec.empty()) {
					TRACE("nothing to be popped\n");
				}
				else {
					int len = int(pointvec.size());
					int len2 = int(twowayvec.size());
					//int len3 = int(bsvec.size());
					if (int(pointvec.size() % 6 == 3)) {
						TRACE("twoway popped{%d}\n", twowayvec[len2 - 1]);
						//TRACE("bs popped{%d}\n",bsvec[len3-1]);
						twowayvec.pop_back();
						//bsvec.pop_back();
					}
					TRACE("element {%d,%d,%d} popped\n", pointvec[len - 3], pointvec[len - 2], pointvec[len - 1]);
					pointvec.pop_back();
					pointvec.pop_back();
					pointvec.pop_back();
				}
			}
		}
	}
	void CGameStateRun::OnShow()
	{
		if (player.IsDoorLock() && !events.at(DOOR_LOCKED_E).IsTriggered()) {
			SetEventTriggeredDialog(DOOR_LOCKED_E);
		}
		if (player.IsDoorOpen() && !events.at(DOOR_UNLOCKED_E).IsTriggered()) {
			SetEventTriggeredDialog(DOOR_UNLOCKED_E);
		}
		if (!(!game_interface.IsShow() || game_interface.IsShowMapAndDialog())) {
			_tempMapID = _nowID;
			_nowID = -1;
		}

		if (_nowID != 3 && _nowID != -1 && events.at(LIGHTUP_ROOM3).IsTriggered() && darkmask[0].IsShow()) {
			events.at(LIGHTUP_ROOM3).SetTriggered(false);
			darkmask[0].SetState(DarkRoomEffect::dark);
		}
		if (_nowID != 21 && _nowID != -1 && events.at(LIGHTUP_ROOM21).IsTriggered() && darkmask[1].IsShow()) {
			events.at(LIGHTUP_ROOM21).SetTriggered(false);
			darkmask[1].SetState(DarkRoomEffect::dark);
		}

		if (normal_oni.IsShow() && !audio_control.at(AUDIO_HOROR)) {
			audio_control.at(AUDIO_HOROR) = true;
			CAudio::Instance()->Play(AUDIO_HOROR, true);
		}
		else if (!normal_oni.IsShow() && audio_control.at(AUDIO_HOROR)) {
			audio_control.at(AUDIO_HOROR) = false;
			CAudio::Instance()->Stop(AUDIO_HOROR);
		}
		switch (_nowID) {
		case -1:
			break;
		case 0: {
			if (!_base0_kabe_show) {
				if (player.IsOnChair()) {
					player.SetCMPY(objs.at(obj_move::house1_basement2_chair).GetPosY() - gamemaps.at(_nowID).GetY());
					normal_oni.SetCMPY(normal_oni.GetPosY() + normal_oni.GetOffsetY() - gamemaps.at(_nowID).GetY());
					objs.at(obj_move::house1_basement2_chair).SetCMPY(player.GetY() - gamemaps.at(_nowID).GetY());
				}
				else {
					player.SetCMPY(player.GetY() - gamemaps.at(_nowID).GetY());
					normal_oni.SetCMPY(normal_oni.GetPosY() + normal_oni.GetOffsetY() - gamemaps.at(_nowID).GetY());
					objs.at(obj_move::house1_basement2_chair).SetCMPY(objs.at(obj_move::house1_basement2_chair).GetPosY() - gamemaps.at(_nowID).GetY());
				}
				entities = { &player, &objs.at(obj_move::house1_basement2_chair), &normal_oni };
				std::sort(entities.begin(), entities.end(), [&](Entity* a, Entity* b) {
					return a->CMPY() < b->CMPY();
					});

				bool trishow[3] = { true,true,true };
				for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
					gamemaps.at(_nowID).ShowMap(i);
					if (i == 2) {
						if (!items.at(KABE_PWD).IsPick()) {
							items.at(KABE_PWD).OnShow();
						}
						else if (!items.at(BASEMENT_PWD).IsPick() && !pwds.at(basement).IsOpen()) {
							items.at(BASEMENT_PWD).OnShow();
						}
						else if (!items.at(BASEMENT_KEY).IsPick() && items.at(BASEMENT_PWD).IsPick()) {
							items.at(BASEMENT_KEY).OnShow();
						}
						else {
							items.at(BASEMENT_PWD_TAKE).OnShow();
							if (!events.at(KEY_ANNEXE_E).IsTriggered()) {
								SetEventTriggeredDialog(KEY_ANNEXE_E);
								game_interface.StoreItem("get out of here", "???", Interface::Items::key_annexe);
							}
						}
						if (!items.at(BASEMENT_PWD).IsClose() && !pwds.at(basement).IsOpen()) {
							if (!pwds.at(basement).IsShow()) {
								pwds.at(basement).SetShow(true);
							}
							pwds.at(basement).ShowTotal();
						}
						items.at(BOOKCASE_L).OnShow();
						items.at(BOOKCASE_R).OnShow();
					}
					for (int j = 0; j < 3; j++) {
						if (((i == 2 && entities[j]->CMPY() < 8 * TILE) || (i == 3 && entities[j]->CMPY() < 14 * TILE) || i == 4) && trishow[j]) {
							if (obj = dynamic_cast<ObjMove*>(entities[j])) {
								obj->OnShowConditional(true);
							}
							else {
								entities[j]->OnShow();
							}
							trishow[j] = false;
						}
					}
				}
				entities.clear();
				items.at(KEY_JAIL).OnShow();
				// gamemaps.at(_nowID).ShowMapTile();
				if (items.at(KEY_JAIL).IsPick() && player.IsOnChair() && !events.at(KEY_JAIL_E).IsTriggered()) {
					SetEventTriggeredDialog(KEY_JAIL_E);
					game_interface.StoreItem("basement jail", "jail key", Interface::Items::key_jail);
				}
				if (events.at(KEY_JAIL_E).IsTriggered() && !events.at(MIKA_TO_ONI_E).IsTriggered() && player.GetY() <= 15 * TILE) {
					events.at(MIKA_TO_ONI_E).SetTriggered(true);
					items.at(MIKA_TO_ONI).EventTrigger();
				}
				if (items.at(MIKA_TO_ONI).IsFixed() && items.at(MIKA_TO_ONI).GetBitMapIndex() <= 5) {
					items.at(MIKA_TO_ONI).OnShow();
				}
				if (!items.at(MIKA_TO_ONI).IsPick() && items.at(MIKA_TO_ONI).IsFixed() && items.at(MIKA_TO_ONI).GetBitMapIndex() == 5) {
					items.at(MIKA_TO_ONI).SetIsPick(true);
					normal_oni.SetPos(8 * TILE, 10 * TILE);
					normal_oni.SetType(Oni::mika);
					normal_oni.IsShow() = true;
					normal_oni.Once() = false;
				}

				if (!items.at(BASEMENT_PWD).IsClose() && !pwds.at(basement).IsOpen()) {
					if (!pwds.at(basement).IsShow()) {
						pwds.at(basement).SetShow(true);
					}
					pwds.at(basement).ShowTotal();
				}
			}
			if (_base0_kabe_show) {
				base0_kabe.ShowBitmap();
			}
			break;
		}
		case 1: {
			if (player.IsOnChair()) {
				player.SetCMPY(objs.at(obj_move::house1_basement2_chair).GetPosY() - gamemaps.at(_nowID).GetY());
				normal_oni.SetCMPY(normal_oni.GetPosY() + normal_oni.GetOffsetY() - gamemaps.at(_nowID).GetY());
				objs.at(obj_move::house1_basement2_chair).SetCMPY(player.GetY() - gamemaps.at(_nowID).GetY());
			}
			else {
				player.SetCMPY(player.GetY() - gamemaps.at(_nowID).GetY());
				normal_oni.SetCMPY(normal_oni.GetPosY() + normal_oni.GetOffsetY() - gamemaps.at(_nowID).GetY());
				objs.at(obj_move::house1_basement2_chair).SetCMPY(objs.at(obj_move::house1_basement2_chair).GetPosY() - gamemaps.at(_nowID).GetY());

			}
			entities = { &player, &objs.at(obj_move::house1_basement2_chair), &normal_oni };
			std::sort(entities.begin(), entities.end(), [&](Entity* a, Entity* b) {
				return a->CMPY() < b->CMPY();
				});
			bool trishow[3] = { true,true,true };

			for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					items.at(FLATHEAD).OnShow();
					for (int j = 0; j < 3; j++) {
						if (obj = dynamic_cast<ObjMove*>(entities[j])) {
							obj->OnShowConditional(false);
						}
						else {
							entities[j]->OnShow();
						}
						trishow[j] = false;
					}

				}
			}
			entities.clear();
			if (items.at(FLATHEAD).IsPick() && player.IsOnChair() && !events.at(FLATHEAD_E).IsTriggered()) {
				SetEventTriggeredDialog(FLATHEAD_E);
				game_interface.StoreItem("flathead screwdriver core", "screwdriver core", Interface::Items::flathead);
			}
			break;
		}
		case 2:
			if (objs.at(obj_move::house1_basement2_chair).isChangeMap()) {
				objs.at(obj_move::house1_basement2_chair).ChangeMap();
			}
			for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					if (game_interface.FindItem("???")
						&& !events.at(MIKA_IN_CLOSET_E).IsTriggered()
						&& player.GetDirection() == Entity::up
						&& player.GetX() == 12 * TILE && player.GetY() == 9 * TILE) {
						events.at(MIKA_IN_CLOSET_E).SetTriggered(true);
					}
					items.at(CLOSET_HIROSI_L).OnShow();
					if (events.at(MIKA_IN_CLOSET_E).IsTriggered() && !items.at(CLOSET_MIKA_OUT).IsPick()) {
						items.at(CLOSET_MIKA_OUT).EventTrigger();
						items.at(CLOSET_MIKA_OUT).OnShow();
					}
					if (!items.at(CLOSET_MIKA_OUT).IsPick() && items.at(CLOSET_MIKA_OUT).GetBitMapIndex() == 3) {
						items.at(CLOSET_MIKA_OUT).SetIsPick(true);
						normal_oni.SetPos(16 * TILE, 9 * TILE);
						normal_oni.SetType(Oni::mika);
						normal_oni.IsShow() = true;
						normal_oni.Once() = false;
					}
					if (player.GetDirection() == Entity::up
						&& player.GetX() == 12 * TILE && player.GetY() == 9 * TILE
						&& items.at(CLOSET_MIKA_OUT).IsPick() && !normal_oni.IsShow() && !_final && !audio_control.at(AUDIO_THE_END)) {
						game_interface.DeleteItem("???");
						audio_control.at(AUDIO_THE_END) = true;
						clear_game.SetParam({ "finally, i get out of here",
							"but mika was become oni...",
							"i hope takesi, takuro can escape here",
							"U clear in game time : " + game_interface.GetGameTime(),
							"Now : " + game_interface.GetRealTime() }, false);
						darkmask[3].SetShow(true);
						clear_game.SetShow(true);
						_final = true;
						CAudio::Instance()->Play(AUDIO_THE_END, true);
					}
					ShowOniAndPlayer();
				}
				darkmask[2].OnShow();
				if (_final) {
					darkmask[3].OnShow();
					clear_game.ShowTotal();
				}
			}
			break;
		case 3: {
			player.SetCMPY(player.GetY() - gamemaps.at(_nowID).GetY());
			normal_oni.SetCMPY(normal_oni.GetPosY() + normal_oni.GetOffsetY() - gamemaps.at(_nowID).GetY());
			items.at(GATE2).SetCMPY(items.at(GATE2).GetPosY() - gamemaps.at(_nowID).GetY());
			items.at(CANDLE2).SetCMPY(items.at(CANDLE2).GetPosY_offset() - gamemaps.at(_nowID).GetY());
			entities = { &player, &normal_oni, &items.at(GATE2), &items.at(CANDLE2) };
			std::sort(entities.begin(), entities.end(), [&](Entity* a, Entity* b) {
				return a->CMPY() < b->CMPY();
				});
			bool tribool[4] = { true,true,true,true };
			for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
				gamemaps.at(_nowID).ShowMap(i);
				for (int j = 0; j < 4; j++) {
					if (((i == 3 && entities.at(j)->CMPY() < 6 * TILE) || (i == 4 && entities.at(j)->CMPY() > 5 * TILE)) && tribool[j]) {
						entities.at(j)->OnShow();
					}
				}
			}
			if (_blue_paint_show) {
				blue_paint.ShowBitmap();
			}
			darkmask[0].OnShow();
			break;
		}
		case 4:
			gamemaps.at(_nowID).ShowMapAll(player, normal_oni, mapoverlayindex.at(_nowID));
			break;
		case 5:
			gamemaps.at(_nowID).ShowMapAll(player, normal_oni, mapoverlayindex.at(_nowID));
			audio_control.at(AUDIO_LIB_FIRST) = false;
			if (!items.at(KEY_3F_L).IsPick()) {
				audio_control.at(AUDIO_LIB_FIRST) = false;
				CAudio::Instance()->Stop(AUDIO_LIB_FIRST);
			}
			break;
		case 6:
			for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					if (!events.at(KEY_3F_L_E).IsTriggered()) {
						items.at(DOOR_ONI).EventTrigger();
						items.at(DOOR_ONI).OnShow();
						if (items.at(DOOR_ONI).IsAnimationDone() && items.at(DOOR_ONI).IsFixed()
							&& _killtimes == 8) {
							items.at(DOOR_OPEN).EventTrigger();
							items.at(DOOR_OPEN).OnShow();
						}
						else if (items.at(DOOR_OPEN).IsAnimationDone() && items.at(DOOR_OPEN).IsFixed()
							&& _killtimes == 9) {
							items.at(DOOR_DIE).EventTrigger();
							items.at(DOOR_DIE).OnShow();
						}
					}
					else {
						items.at(DOOR_ONI).OnShow(); // just show
						if (!items.at(DOOR_HALF).IsPick()) {
							items.at(DOOR_HALF).OnShow();
						}
					}

					ShowOniAndPlayer();
				}
			}
			break;
		case 7:
			gamemaps.at(_nowID).ShowMapAll(player, normal_oni, mapoverlayindex.at(_nowID));
			objs.at(obj_move::house1_2F_TR_chair).ChangeMap();
			objs.at(obj_move::house1_2F_TL_chair).ChangeMap();
			if (_map_show) {
				house1_map.ShowBitmap();
			}
			break;
		case 8:
			gamemaps.at(_nowID).ShowMapAll(player, normal_oni, mapoverlayindex.at(_nowID));
			break;
		case 9:
			gamemaps.at(_nowID).ShowMapAll(player, normal_oni, mapoverlayindex.at(_nowID));
			if (objs.at(obj_move::house1_basement2_chair).isChangeMap()) {
				objs.at(obj_move::house1_basement2_chair).ChangeMap();
			}
			if (player.IsBasementTrigger() && !events.at(OPEN_BASEMENT_E).IsTriggered() && !audio_control.at(AUDIO_MIKA_SOUND)) {
				audio_control.at(AUDIO_MIKA_SOUND) = true;
				SetEventTriggeredDialog(OPEN_BASEMENT_E);
				CAudio::Instance()->Play(AUDIO_MIKA_SOUND, false);
			}
			if (_oni_eat_mika && !normal_oni.IsShow()) {
				player.SetBasementUnlock(true);
			}
			break;
		case 10:
			for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					items.at(LIGHTER).OnShow();
					items.at(TATAMI_R).OnShow();
					if (items.at(LIGHTER).IsPick()) {
						items.at(TATAMI_L).OnShow();
					}
					items.at(DOOR_DIFF).OnShow();

					if (!events.at(TATAMI_E).IsTriggered() && player.GetY() <= 11 * TILE) {
						normal_oni.SetPos(11 * TILE, 8 * TILE);
						normal_oni.SetType(Oni::normal);
						items.at(TATAMI_R).EventTrigger();
						events.at(TATAMI_E).SetTriggered(true);
						normal_oni.IsShow() = true;
						normal_oni.Once() = false;
					}
					ShowOniAndPlayer();
				}
			}
			if (items.at(LIGHTER).IsPick() && !events.at(LIGHTER_E).IsTriggered()) {
				SetEventTriggeredDialog(LIGHTER_E);
				game_interface.StoreItem("(need oil) lighter", "lighter", Interface::Items::lighter);
			}
			break;
		case 11:
			gamemaps.at(_nowID).ShowMapAll(player, normal_oni, mapoverlayindex.at(_nowID));
			items.at(BROKEN_DISH).OnShow();
			if (items.at(BROKEN_DISH).IsTake() && !events.at(BROKEN_DISH_E).IsTriggered()) {
				SetEventTriggeredDialog(BROKEN_DISH_E);
				game_interface.StoreItem("broken dish", "plate shards", Interface::Items::broken_dish);
			}
			break;
		case 12: {
			player.SetCMPY(player.GetY() - gamemaps.at(_nowID).GetY());
			normal_oni.SetCMPY(normal_oni.GetPosY() + normal_oni.GetOffsetY() - gamemaps.at(_nowID).GetY());
			entities = { &player, &normal_oni };
			std::sort(entities.begin(), entities.end(), [&](Entity* a, Entity* b) {
				return a->CMPY() < b->CMPY();
				});
			bool bishow[2] = { true,true };
			for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
				gamemaps.at(_nowID).ShowMap(i);
				for (int j = 0; j < 2; j++) {
					if (((i == 4 && entities[j]->CMPY() < 4 * TILE) || (i == 5 && entities[j]->CMPY() < 8 * TILE) || (i == 6 && entities[j]->CMPY() < 12 * TILE) || (i == 7 && entities[j]->CMPY() < 15 * TILE) || i == 8) && bishow[j]) {
						entities[j]->OnShow();
						bishow[j] = false;
					}
				}
			}

			entities.clear();
			items.at(LIB_BOOK).OnShow();
			if (items.at(LIB_BOOK).IsFixed()) {
				items.at(KEY_3F_L).OnShow();
			}
			if (!items.at(KEY_3F_L).IsPick() && !audio_control.at(AUDIO_LIB_FIRST)) {
				audio_control.at(AUDIO_LIB_FIRST) = true;
				CAudio::Instance()->Play(AUDIO_LIB_FIRST, true);
			}
			if (items.at(KEY_3F_L).IsPick() && !events.at(KEY_3F_L_E).IsTriggered()) {
				SetEventTriggeredDialog(KEY_3F_L_E);
				game_interface.StoreItem("3F left", "bedroom key", Interface::Items::key_3F_L);
				CAudio::Instance()->Stop(AUDIO_LIB_FIRST);
			}
			break;
		}
		case 13:
			if (!(_dialogID >= 2 && _dialogID <= 11)) {
				gamemaps.at(_nowID).ShowMapAll(player, normal_oni, mapoverlayindex.at(_nowID));
			}
			if (!events.at(START_EVENT_E).IsTriggered()) {
				SetEventTriggeredDialog(START_EVENT_E);
			}
			if (!events.at(START_EVENT2_E).IsTriggered() && events.at(BROKEN_DISH_E).IsTriggered()) {
				SetEventTriggeredDialog(START_EVENT2_E);
			}
			if (_dialogID == 6 && !audio_control.at(AUDIO_BROKEN_DISH)) {
				audio_control.at(AUDIO_BROKEN_DISH) = true;
				CAudio::Instance()->Play(AUDIO_BROKEN_DISH, false);
			}
			if (_dialogID == 6) {
				player.SetDirection(Entity::right);
				human_takeshi.SetDirection(Entity::right);
				human_takuro.SetDirection(Entity::right);
			}
			else if (_dialogID == 7) {
				player.SetMachine(Entity::right);
			}
			else if (_dialogID == 9) {
				player.SetNowmove(Entity::machinetransmap);
			}
			else if (_dialogID == 10) {
				player.SetAllMoveFalse();
				player.SetMachine(Entity::left);
			}
			else if (_dialogID == 11) {
				if (player.IsMachineDone()) {
					player.TimerStart();
				}
			}
			else if (_dialogID == 12) {
				if (player.IsTimerStart()) {
					player.IsMachineDone() = false;
					if (player.TimerGetCount() < 20) {
						player.SetDirection(Entity::up);
					}
					else if (player.TimerGetCount() < 40) {
						player.SetDirection(Entity::down);
					}
					else if (player.TimerGetCount() < 60) {
						player.SetDirection(Entity::left);
						player.TimerStop();
					}
					player.TimerUpdate();
				}
				else {
					player.IsMachineDone() = true;
				}
			}
			// in event situation show function is different from normal situation
			if (_dialogID >= 2 && _dialogID <= 9) {
				gamemaps.at(_nowID).ShowMapAll();
				if (_dialogID != 9) {
					player.OnShow();
				}
				human_mika.OnShow();
				human_takeshi.OnShow();
				human_takuro.OnShow();
			}
			if (_dialogID >= 10 && _dialogID <= 12) {
				gamemaps.at(_nowID).ShowMapAll();
				player.OnShow();
			}
			break;
		case 14:
			for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					objs.at(obj_move::house1_2F_TR_chair).OnShow();
					if (objs.at(obj_move::house1_2F_TR_chair).IsFixed()) {
						items.at(KEY_LIB).OnShow();
						if (items.at(KEY_LIB).IsPick() && !events.at(KEY_LIB_E).IsTriggered()) {
							SetEventTriggeredDialog(KEY_LIB_E);
							game_interface.StoreItem("library", "library key", Interface::Items::key_lib);
						}
					}
					if (events.at(KEY_3F_L_E).IsTriggered()) {
						items.at(HANDKERCHIEF).OnShow();
					}
					if (events.at(KEY_3F_L_E).IsTriggered() && items.at(HANDKERCHIEF).IsPick() && !events.at(HANDKERCHIEF_E).IsTriggered()) {
						SetEventTriggeredDialog(HANDKERCHIEF_E);
						game_interface.StoreItem("(dirty) handkerchief", "handkerchief", Interface::Items::handkerchief);
					}
					if ((player.GetY() - gamemaps.at(_nowID).GetY()) / TILE <= 3 && (player.GetX() - gamemaps.at(_nowID).GetX()) / TILE >= 5) {
						items.at(CLOSET_SHAKE).EventTrigger();
					}
					if (items.at(CLOSET_SHAKE).IsFixed() && !audio_control.at(AUDIO_TAKESI_NOICE)) {
						audio_control.at(AUDIO_TAKESI_NOICE) = true;
						CAudio::Instance()->Play(AUDIO_TAKESI_NOICE, false);
					}
					if ((!items.at(CLOSET_SHAKE).IsFixed() || !items.at(CLOSET_SHAKE).IsAnimationDone()) && !events.at(KEY_3F_L_E).IsTriggered()) {
						items.at(CLOSET_SHAKE).OnShow();
					}
					if (items.at(CLOSET_SHAKE).IsFixed() && items.at(CLOSET_SHAKE).IsAnimationDone() && !events.at(KEY_3F_L_E).IsTriggered()) {
						if (!items.at(CLOSET_TAKESI_0).IsFixed() || !items.at(CLOSET_TAKESI_0).IsAnimationDone()) {
							items.at(CLOSET_TAKESI_0).OnShow();
						}
						if (items.at(CLOSET_TAKESI_0).IsFixed() && items.at(CLOSET_TAKESI_0).IsAnimationDone()) {
							if (!events.at(DUMB_TAKESHI_E).IsTriggered()) {
								SetEventTriggeredDialog(DUMB_TAKESHI_E);
							}
							if (!events.at(DUMB_TAKESHI_REPEAT_E).IsTriggered() && items.at(CLOSET_TAKESI_0).IsPick()) {
								items.at(CLOSET_TAKESI_0).SetIsPick(false);
								SetEventTriggeredDialog(DUMB_TAKESHI_REPEAT_E);
								events.at(DUMB_TAKESHI_REPEAT_E).SetTriggered(false);
							}
							items.at(CLOSET_TAKESI_1).OnShow();
						}
					}
					if (events.at(KEY_3F_L_E).IsTriggered()) {
						items.at(CLOSET_HIROSI_R).OnShow();
					}

					ShowOniAndPlayer();
				}
			}
			darkmask[2].OnShow();
			break;
		case 15:
			for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					if (!items.at(DOOR_KNOB).IsPick()) {
						items.at(DOOR_KNOB).OnShow();
					}
					if (items.at(DOOR_KNOB).IsPick()) {
						items.at(DOOR_NO_KNOB).OnShow();
					}
					items.at(CLOSET_HIROSI_MAP15).OnShow();
					ShowOniAndPlayer();
				}
			}
			darkmask[2].OnShow();
			break;
		case 16:
			gamemaps.at(_nowID).ShowMapAll(player, normal_oni, mapoverlayindex.at(_nowID));
			items.at(BED).OnShow();
			if (items.at(BED).IsOnCorPos() && !events.at(OPEN_FUCKING_HOLE_E).IsTriggered()) {
				router.UnblockPath(16, 17);
			}
			break;
		case 17:
			for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					if (!items.at(PIANO_PWD_NOTOPEN).IsPick() && !pwds.at(piano).IsOpen()) {
						items.at(PIANO_PWD_NOTOPEN).OnShow();
					}
					else if (!items.at(BB_KEY).IsPick() && items.at(PIANO_PWD_NOTOPEN).IsPick()) {
						items.at(BB_KEY).OnShow();
					}
					else {
						items.at(PIANO_PWD_TAKE).OnShow();
						if (!events.at(KEY_2F_TL_E).IsTriggered()) {
							SetEventTriggeredDialog(KEY_2F_TL_E);
							game_interface.StoreItem("2F top left", "babyroom key", Interface::Items::key_2F_TL);
						}
					}
					items.at(WHITE_BOOKCASE).OnShow();
					ShowOniAndPlayer();
				}
			}
			if (_piano_hint_show) {
				piano_hint.ShowBitmap();
				if (player.GetDirection() == Entity::up) {
					if (!pwds.at(piano).IsShow()) {
						pwds.at(piano).SetShow(true);
					}
					pwds.at(piano).ShowTotal();
				}
			}
			break;
		case 18:
			gamemaps.at(_nowID).ShowMapAll(player, normal_oni, mapoverlayindex.at(_nowID));
			if (!items.at(TUB_ONCE).IsFixed() || !items.at(TUB_ONCE).IsAnimationDone()) {
				items.at(TUB_ONCE).OnShow();
			}
			if (items.at(TUB_ONCE).IsFixed() && !audio_control.at(AUDIO_TUB_WATER)) {
				audio_control.at(AUDIO_TUB_WATER) = true;
				CAudio::Instance()->Play(AUDIO_TUB_WATER, false);
			}
			if (items.at(TUB_ONCE).IsFixed() && items.at(TUB_ONCE).IsAnimationDone()) {
				items.at(PHILLIPS).OnShow();
			}
			if (items.at(PHILLIPS).IsPick()) {
				items.at(TUB_FIXED).OnShow();
			}
			if (items.at(PHILLIPS).IsPick() && !events.at(PHILLIPS_E).IsTriggered()) {
				SetEventTriggeredDialog(PHILLIPS_E);
				game_interface.StoreItem("phillips screwdriver", "screwdriver", Interface::Items::phillips);
			}
			break;
		case 19:
			for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					items.at(DETERGENT).OnShow();
					items.at(TOILET).OnShow();
					ShowOniAndPlayer();
				}
			}
			if (items.at(DETERGENT).IsPick() && !events.at(DETERGENT_E).IsTriggered()) {
				SetEventTriggeredDialog(DETERGENT_E);
				game_interface.StoreItem("detergent", "detergent", Interface::Items::detergent);
			}
			break;
		case 20:
			if (player.IsOnChair()) {
				player.SetCMPY(objs.at(obj_move::house1_2F_TL_chair).GetPosY() - gamemaps.at(_nowID).GetY());
				normal_oni.SetCMPY(normal_oni.GetPosY() + normal_oni.GetOffsetY() - gamemaps.at(_nowID).GetY());
				objs.at(obj_move::house1_2F_TL_chair).SetCMPY(player.GetY() - gamemaps.at(_nowID).GetY());
			}
			else {
				player.SetCMPY(player.GetY() - gamemaps.at(_nowID).GetY());
				normal_oni.SetCMPY(normal_oni.GetPosY() + normal_oni.GetOffsetY() - gamemaps.at(_nowID).GetY());
				objs.at(obj_move::house1_2F_TL_chair).SetCMPY(objs.at(obj_move::house1_2F_TL_chair).GetPosY() - gamemaps.at(_nowID).GetY());
			}
			entities = { &player, &objs.at(obj_move::house1_2F_TL_chair), &normal_oni };
			std::sort(entities.begin(), entities.end(), [&](Entity* a, Entity* b) {
				return a->CMPY() < b->CMPY();
				});
			for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(i)) {
					if (!events.at(MIKA_TO_ONI_E).IsTriggered() && !events.at(MIKA_DEAD_E).IsTriggered() && events.at(OPEN_BASEMENT_E).IsTriggered()) {
						deadbody.ShowBitmap();
					}
					for (int j = 0; j < 3; j++) {
						if (!events.at(MIKA_TO_ONI_E).IsTriggered() && !events.at(MIKA_DEAD_E).IsTriggered() && events.at(OPEN_BASEMENT_E).IsTriggered()) {
							if (!oni_eat.IsAnimationDone()) {
								oni_eat.ShowBitmap();
								normal_oni.SetPos(12 * TILE, 13 * TILE);
								normal_oni.SetType(Oni::normal);
							}
							else if (once) {
								normal_oni.IsShow() = true;
								once = false;
								_oni_eat_mika = true;
							}
						}
						else if (!events.at(MIKA_TO_ONI_E).IsTriggered()) {
							human_mika.OnShow();
						}
						entities.at(j)->OnShow();
					}
				}
			}
			entities.clear();
			items.at(OIL).OnShow();

			if (_dialogID >= 25 && _dialogID <= 32) {
				human_mika.SetDirection(Entity::up);
			}
			else {
				human_mika.SetDirection(Entity::down);

			}
			if (items.at(OIL).IsPick() && player.IsOnChair() && !events.at(OIL_E).IsTriggered()) {
				SetEventTriggeredDialog(OIL_E);
				game_interface.StoreItem("oil", "oil", Interface::Items::oil);
			}
			if (human_mika.Trigger() && !events.at(MIKA_SCARE_E).IsTriggered()) {
				SetEventTriggeredDialog(MIKA_SCARE_E);
				human_mika.SetDirection(Entity::up);
				human_mika.Trigger() = false;
			}
			if (events.at(MIKA_SCARE_E).IsTriggered()) {
				if (dialogs.at(28).Choice() == Dialog::yes && !events.at(MIKA_NOTOK_E).IsTriggered()) {
					SetEventTriggeredDialog(MIKA_NOTOK_E);
					human_mika.Trigger() = false;
				}
				if (dialogs.at(28).Choice() == Dialog::no && !events.at(MIKA_OK_E).IsTriggered()) {
					SetEventTriggeredDialog(MIKA_OK_E);
					human_mika.Trigger() = false;
				}
			}
			if ((events.at(MIKA_NOTOK_E).IsTriggered() || events.at(MIKA_OK_E).IsTriggered()) &&
				!events.at(MIKA_REPEAT_E).IsTriggered() && human_mika.Trigger()) {
				SetEventTriggeredDialog(MIKA_REPEAT_E);
				human_mika.Trigger() = false;
				events.at(MIKA_REPEAT_E).SetTriggered(false);
			}

			break;
		case 21: {
			player.SetCMPY(player.GetY() - gamemaps.at(_nowID).GetY());
			normal_oni.SetCMPY(normal_oni.GetPosY() + normal_oni.GetOffsetY() - gamemaps.at(_nowID).GetY());
			items.at(CANDLE1).SetCMPY(items.at(CANDLE1).GetPosY_offset() - gamemaps.at(_nowID).GetY());
			entities = { &player, &normal_oni, &items.at(CANDLE1) };
			std::sort(entities.begin(), entities.end(), [&](Entity* a, Entity* b) {
				return a->CMPY() < b->CMPY();
				});
			bool tribool[3] = { true,true,true };
			for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					items.at(BOOKCASE_MAP21).OnShow();
				}
				for (int j = 0; j < 3; j++) {
					if (((i == 3 && entities.at(j)->CMPY() < 6 * TILE) || (i == 4 && entities.at(j)->CMPY() > 5 * TILE)) && tribool[j]) {
						entities.at(j)->OnShow();
					}
				}
			}
			darkmask[1].OnShow();
			break;
		}
		case 22: {
			player.SetCMPY(player.GetY() - gamemaps.at(_nowID).GetY());
			normal_oni.SetCMPY(normal_oni.GetPosY() + normal_oni.GetOffsetY() - gamemaps.at(_nowID).GetY());
			items.at(GATE).SetCMPY(items.at(GATE).GetPosY() - gamemaps.at(_nowID).GetY());
			entities = { &player,&normal_oni ,&items.at(GATE) };
			std::sort(entities.begin(), entities.end(), [&](Entity* a, Entity* b) {
				return a->CMPY() < b->CMPY();
				});
			bool tribool[3] = { true,true,true };
			for (int i = 1; i < gamemaps.at(_nowID).GetLayer(); i++) {
				gamemaps.at(_nowID).ShowMap(i);
				for (int j = 0; j < 3; j++) {
					if (((i == 3 && entities.at(j)->CMPY() < 6 * TILE) || (i == 4 && entities.at(j)->CMPY() > 5 * TILE)) && tribool[j]) {
						entities.at(j)->OnShow();
					}
				}
			}

			if (events.at(KEY_BASEMENT_E).IsTriggered() && !events.at(GATE_ONI_APPEAR_E).IsTriggered()) {
				events.at(GATE_ONI_APPEAR_E).SetTriggered(true);
				normal_oni.SetPos(10 * TILE, 16 * TILE);
				normal_oni.SetType(Oni::normal);
				normal_oni.IsShow() = true;
				normal_oni.Once() = false;
				if (items.at(GATE).IsClose()) {
					_in_gate_map22 = true;
				}
			}

			items.at(KEY_BASEMENT).OnShow();
			if (items.at(KEY_BASEMENT).IsPick() && !events.at(KEY_BASEMENT_E).IsTriggered()) {
				SetEventTriggeredDialog(KEY_BASEMENT_E);
				game_interface.StoreItem("basement", "basement key", Interface::Items::key_basement);
				
			}

			if (_in_gate_map22 && normal_oni.GetOverTimer() == 0 && !gate_animation.IsAnimationDone()) {
				if (!audio_control.at(AUDIO_GATE_ONI)) {
					audio_control.at(AUDIO_GATE_ONI) = true;
					CAudio::Instance()->Stop(AUDIO_HOROR);
					CAudio::Instance()->Play(AUDIO_GATE_ONI, false);
				}
				gate_animation.ShowBitmap();
				_bar_animation_show = true;
			}
			else if (_in_gate_map22 && normal_oni.GetOverTimer() == 0 && gate_animation.IsAnimationDone()) {
				_in_gate_map22 = false;
				normal_oni.ResetOni();
				_bar_animation_show = false;
				CAudio::Instance()->Stop(AUDIO_GATE_ONI);
			}
			break;
		}
		default:
			if (_nowID > 22) {
				gamemaps.at(_nowID).ShowMapAll(player, normal_oni, mapoverlayindex.at(_nowID));
			}
			break;
		}
		if (_nowID != -1) {
			for (int i = 0; i < int(dialogs.size()) - 1; i++) {
				if (dialogs.at(i).isShow()) {
					dialogs.at(i).ShowTotal();
				}
			}
			gamemaps.at(_nowID).ShowMapTile();
			DeBugRecursive();
		}
		if (_nowID == -1) {
			_nowID = _tempMapID;
			_tempMapID = -1;
		}
		if (game_interface.IsShow()) {
			game_interface.ShowTotal();
		}
		if (_is_danger && closet_animation.IsAnimationDone() && _die) {
			GotoGameState(GAME_STATE_OVER);
		}
		else if (_is_danger && !closet_animation.IsAnimationDone()) {
			closet_animation.ShowBitmap();
			_die = true;
		}
	}

	void CGameStateRun::ShowOniAndPlayer() {
		if (items.at(CLOSET_HIROSI_R).GetBitMapIndex() >= 3
			|| items.at(CLOSET_HIROSI_MAP15).GetBitMapIndex() >= 3
			|| items.at(CLOSET_HIROSI_L).GetBitMapIndex() >= 3
			|| items.at(DOOR_OPEN).GetBitMapIndex() >= 1) {
			normal_oni.OnShow();
		}
		else if (normal_oni.GetPosD() > player.GetD()) {
			player.OnShow();
			normal_oni.OnShow();
		}
		else {
			normal_oni.OnShow();
			player.OnShow();
		}
	}
	void CGameStateRun::SetEventTriggeredDialog(int eventid)
	{
		_eventID = eventid;
		events.at(_eventID).SetTriggered(true);
		if (events.at(_eventID).GetDialogIndex() != -1) {
			_dialogID = events.at(_eventID).GetDialogIndex();
			dialogs.at(_dialogID).SetShow(true);
			_substate = OnDialogs;
		}
		for (auto& f : events.at(_eventID).GetBlockPath()) {
			router.UnblockPath(f.at(0), f.at(1));
		}
	}
	void CGameStateRun::DeBugRecursive() {
		string maplink = "map_bmp/maplink.txt";
		if (isedit && !ofs.is_open()) {
			ofs.open(maplink, std::ios::app);
			if (!ofs.is_open()) {
				TRACE("Failed to open file.\n");
				throw std::invalid_argument("open failed");
			}
			TRACE("open maplink.txt\n");
		}
		if (ofs.is_open() && !isedit) {

			int i = 1;
			int j = 0;
			int k = 0;
			for (auto f : pointvec) {
				if (i % 6 == 1) {
					ofs << twowayvec[j++];
					ofs << " ";
				}
				ofs << f;
				if (i % 6 == 0) {
					ofs << "\n";
				}
				else {
					ofs << " ";
				}
				i++;
			}
			ofs.close();
			twowayvec.clear();
			pointvec.clear();
			//bsvec.clear();
			TRACE("twowayvec & pointtmp & bsvec cleared \n");
			ifstream file(maplink);
			string content;
			string line;
			int lineNumber = 0;
			while (getline(file, line)) {
				lineNumber++;
				if (lineNumber != 1)content += line + "\n";
			}
			file.close();
			TRACE("link number : %d\n", lineNumber - 1);
			ofstream outputFile(maplink, ios::trunc);
			outputFile << lineNumber - 1 << "\n";
			outputFile << content;
			outputFile.close();
			TRACE("close\n");
		}

		inputbox.OnShow();
		if (isteleportblock) {
			for (int i = 0; i < router.GetRecord(_nowID); i++) {
				for (int j = 0; j < router.GetNowMapPortal(_nowID)[i].GetSize(); j++) {
					tileplaceholder.SetTopLeft(router.GetNowMapPortal(_nowID)[i].GetPointByIndex(j).GetW() + gamemaps.at(_nowID).GetX(), router.GetNowMapPortal(_nowID)[i].GetPointByIndex(j).GetX() + gamemaps.at(_nowID).GetY());
					tileplaceholder.ShowBitmap();
				}
			}
		}
		gamemaps.at(_nowID).ShowTileIndexLayer();
		if (isgrid) {
			grid.ShowBitmap();
		}
		if (isedit) {
			tileplaceholder.ShowBitmap();
		}
		CDC* pDC = CDDraw::GetBackCDC();
		CTextDraw::ChangeFontLog(pDC, 20, "Consolas", RGB(255, 255, 255));
		if (!isdebugmode) {
			CTextDraw::Print(pDC, 0, 0, "use KEY_Y to activate GOD mode");
		}
		else {
			CTextDraw::Print(pDC, 0, 0, "map index:" + to_string(_nowID) + "  " + gamemaps.at(_nowID).GetName());
			(isedit) ? CTextDraw::Print(pDC, 0, TILE, "edit mode: true") : CTextDraw::Print(pDC, 0, TILE, "edit mode: false");

			CTextDraw::Print(pDC, 0, TILE * 2, "mouse window tile coordinate : " + to_string(mousex) + "  " + to_string(mousey));
			CTextDraw::Print(pDC, 0, TILE * 3, "mouse map tile coordinate : " + to_string(mousex - gamemaps.at(_nowID).GetX() / TILE) + "  " + to_string(mousey - gamemaps.at(_nowID).GetY() / TILE));

			CTextDraw::Print(pDC, 0, TILE * 4, "player tile coordinate on map: " + to_string((player.GetX() - gamemaps.at(_nowID).GetX()) / TILE) + " " + to_string((player.GetY() - gamemaps.at(_nowID).GetY()) / TILE));
			CTextDraw::Print(pDC, 0, TILE * 5, "player tile coordinate on window: " + to_string(player.GetX() / TILE) + " " + to_string(player.GetY() / TILE));
			CTextDraw::Print(pDC, 0, TILE * 6, "(check for out of grid) player cor point x : " + to_string((player.GetX() - gamemaps.at(_nowID).GetX()) % TILE) + " y : " + to_string((player.GetY() - gamemaps.at(_nowID).GetY()) % TILE));
			CTextDraw::Print(pDC, 0, TILE * 7, "_dialogID : " + to_string(_dialogID));
			player.IsTimerStart() ? CTextDraw::Print(pDC, 0, TILE * 8, "timer start") : CTextDraw::Print(pDC, 0, TILE * 8, "timer stop");
			(istwoway != 0) ? ((istwoway == 1) ? CTextDraw::Print(pDC, 0, TILE * 9, "is twoway : yes") : CTextDraw::Print(pDC, 0, TILE * 9, "is twoway : no")) : CTextDraw::Print(pDC, 0, TILE * 9, "is twoway : none");

			//player.IsMachineDone() ? CTextDraw::Print(pDC, 0, TILE * 10, "machine done") : CTextDraw::Print(pDC, 0, TILE * 10, "machine not done");
			router.Gaming() ? CTextDraw::Print(pDC, 0, TILE * 10, "All path state : Gaming") : CTextDraw::Print(pDC, 0, TILE * 10, "All path state : Unlock all");
			CTextDraw::Print(pDC, 0, TILE * 11, "Oni overtime : " + to_string(normal_oni.GetOverTimer()));
			player.IsMapChanged() ? CTextDraw::Print(pDC, 0, TILE * 12, "player.IsMapChanged(): T") : CTextDraw::Print(pDC, 0, TILE * 12, "player.IsMapChanged(): F");
			//player.IsSwitchMap() ? CTextDraw::Print(pDC, 0, TILE * 13, "All path state : Gaming") : CTextDraw::Print(pDC, 0, TILE * 13, "All path state : Unlock all");
			CTextDraw::Print(pDC, 0, TILE * 13, to_string(gamemaps.at(_nowID).GetX()));
			CTextDraw::Print(pDC, 0, TILE * 14, to_string(gamemaps.at(_nowID).GetY()));


			//(human_mika.Trigger()) ? CTextDraw::Print(pDC, 0, TILE * 12, "mkia ahhhh") : CTextDraw::Print(pDC, 0, TILE * 12, "mika nnnnnnahhh");
			CTextDraw::Print(pDC, 0, TILE * 17, "     up            :     " + to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetX() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetU() - gamemaps.at(_nowID).GetY()) / TILE)));
			CTextDraw::Print(pDC, 0, TILE * 18, "left    right      : " +
				to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetL() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetY() - gamemaps.at(_nowID).GetY()) / TILE)) +
				"    " + to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetR() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetY() - gamemaps.at(_nowID).GetY()) / TILE)
				));
			CTextDraw::Print(pDC, 0, TILE * 19, "    down           :     " + to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetX() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetD() - gamemaps.at(_nowID).GetY()) / TILE)));

			int len = int(pointvec.size());
			if (len % 6 == 0 && len != 0) {
				CTextDraw::Print(pDC, 0, TILE * 20, "point1  " + to_string(pointvec[len - 6]) + "  " + to_string(pointvec[len - 5]) + "  " + to_string(pointvec[len - 4]) + "  tile x:  " + to_string(pointvec[len - 5] / TILE) + "  tile y:  " + to_string(pointvec[len - 4] / TILE));
				CTextDraw::Print(pDC, 0, TILE * 21, "point2  " + to_string(pointvec[len - 3]) + "  " + to_string(pointvec[len - 2]) + "  " + to_string(pointvec[len - 1]) + "  tile x:  " + to_string(pointvec[len - 2] / TILE) + "  tile y:  " + to_string(pointvec[len - 1] / TILE));
			}
			else if (len % 3 == 0 && len != 0) {
				CTextDraw::Print(pDC, 0, TILE * 20, "point1  " + to_string(pointvec[len - 3]) + "  " + to_string(pointvec[len - 2]) + "  " + to_string(pointvec[len - 1]) + "  tile x:  " + to_string(pointvec[len - 2] / TILE) + "  tile y:  " + to_string(pointvec[len - 1] / TILE));
			}
		}
		CDDraw::ReleaseBackCDC();
	}
};