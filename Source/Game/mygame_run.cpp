#include "stdafx.h"
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

namespace game_framework {
	CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
	{
	}

	CGameStateRun::~CGameStateRun()
	{
		MapRes::GetInstance()->Cleanup();
	}
	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{
		values.reserve(3); // reserve memory to avoid frequent reallocation
		entities.reserve(3);
		ShowInitProgress(33, "loading game mid");
		// player map x, player map y, map ID bulk of craps
		blockLeftCor.push_back({ 8,5,13 });
		blockLeftCor.push_back({ 5,4,7 });
		blockLeftCor.push_back({ 8,11,7 });
		blockLeftCor.push_back({ 6,10,33 });
		blockLeftCor.push_back({ 2,3,34 });
		blockLeftCor.push_back({ 10,4,42 });
		blockLeftCor.push_back({ 10,6,23 });
		blockLeftCor.push_back({ 2,13,54 });
		blockLeftCor.push_back({ 2,5,56 });

		blockRightCor.push_back({ 4,4,7 });
		blockRightCor.push_back({ 7,11,7 });
		blockRightCor.push_back({ 7,5,13 });
		blockRightCor.push_back({ 5,10,33 });
		blockRightCor.push_back({ 1,3 ,34});
		blockRightCor.push_back({ 9,4,42 });
		blockRightCor.push_back({ 9,6,23 });
		blockRightCor.push_back({ 1,13,54 });
		blockRightCor.push_back({ 1,5,56 });

		blockTeleportCor.push_back({ 4,5,7 });
		blockTeleportCor.push_back({ 6,9,33 });
		blockTeleportCor.push_back({ 2,4,34 });
		blockTeleportCor.push_back({ 9,5,42 });
		blockTeleportCor.push_back({ 9,5,23 });
		blockTeleportCor.push_back({ 2,12,54 });
		blockTeleportCor.push_back({ 2,6,56 });
		blockTeleportCor.push_back({ 18,15,54 });
		blockTeleportCor.push_back({ 19,16,54 });
		blockTeleportCor.push_back({ 4,4,55 });
		blockTeleportCor.push_back({ 4,3,46 });
		blockTeleportCor.push_back({ 3,2,46 });
		mapmask.LoadBitmapByString({ "img/mapmask0.bmp","img/mapmask1.bmp"}, RGB(204, 255, 0));
		// main character
		vector<string> playervec;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				playervec.push_back("img/hiroshi_move/Hiroshi_" + to_string(i) + to_string(j) + ".bmp");
			}
		}
		player.Load(playervec, RGB(204, 255, 0));
		// other players
		playervec.clear();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				playervec.push_back("img/mika_move/Mika_" + to_string(i) + to_string(j) + ".bmp");
			}
		}
		human_mika.Load(playervec, RGB(204, 255, 0));
		playervec.clear();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				playervec.push_back("img/takeshi_move/Takeshi_" + to_string(i) + to_string(j) + ".bmp");
			}
		}
		human_takeshi.Load(playervec, RGB(204, 255, 0));
		playervec.clear();
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 3; j++) {
				playervec.push_back("img/takuro_move/Takuro_" + to_string(i) + to_string(j) + ".bmp");
			}
		}
		human_takuro.Load(playervec, RGB(204, 255, 0));
		// map resources
		std::ifstream mapres_in("map_bmp/mapsize.txt");
		string name;
		int count;
		for (int i = 0; i < 10; i++) {
			mapres_in >> name >> count;
			ShowInitProgress(33 + 5*i, name);
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
		items.resize(42);
		items.at(TOILET).SetParam(-1, 0, 0, Item::toilet);
		items.at(TUB_ONCE).SetParam(100, 0, TILE, Item::tub_once);
		items.at(PHILLIPS).SetParam(100, 0, TILE, Item::phillips);
		items.at(TUB_FIXED).SetParam(-1, 0, TILE, Item::tub_fixed);
		items.at(BROKEN_DISH).SetParam(-1, 0, 0, Item::broken_dish);
		items.at(LIB_BOOK).SetParam(-1, 0, 0, Item::lib_book);
		items.at(KEY_3F_L).SetParam(100, 0, 0, Item::key_3F_L);
		items.at(KEY_LIB).SetParam(100, 0, 0, Item::key_lib);
		items.at(DOOR_KNOB).SetParam(100, 0, TILE, Item::door_knob);
		items.at(DOOR_NO_KNOB).SetParam(100, 0, TILE, Item::door_no_knob);
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
		items.at(DOOR_OPEN).SetParam(100, 0, TILE, Item::door_open);
		items.at(DOOR_DIE).SetParam(100, 0, TILE, Item::door_die);
		items.at(DOOR_HALF).SetParam(-1, 0, TILE, Item::door_half);
		items.at(PIANO_PWD_NOTOPEN).SetParam(-1, 0, 0, Item::password_not_open);
		items.at(PIANO_PWD_TAKE).SetParam(-1, 0, 0, Item::password_get_key);
		items.at(BB_KEY).SetParam(100, 0, 0, Item::key_2F_TL);
		items.at(BASEMENT_PWD).SetParam(-1, 0, 0, Item::password_not_open);
		items.at(BASEMENT_PWD_TAKE).SetParam(-1, 0, 0, Item::password_get_key);
		items.at(BASEMENT_KEY).SetParam(100, 0, 0, Item::key_annexe);
		//events
		events.resize(30);
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
		events.at(LIB_KEY_CHASE).SetParam({}, -1, -1);
		events.at(DUMB_TAKESHI_REPEAT_E).SetParam({}, 18, 1);
		events.at(KEY_2F_TL_E).SetParam({}, 33, 1); // piano pwd
		events.at(KEY_BASEMENT_E).SetParam({}, 34, 1);
		events.at(KEY_ANNEXE_E).SetParam({}, 36, 1);
		events.at(DOOR_LOCKED_E).SetParam({}, 37, 1);
		events.at(DOOR_UNLOCKED_E).SetParam({}, 38, 1);
		
		//dialogs
		dialogs.resize(40);
		dialogs.at(0).SetFigure("hirosi");
		dialogs.at(0).SetParam({ "A broken plate... " }, false);
		dialogs.at(1).SetFigure("hirosi");
		dialogs.at(1).SetParam({"Gain the broken plate"},false);
		dialogs.at(2).SetFigure("takurou");
		dialogs.at(2).SetParam({ "beautiful interiors!!! " }, false);
		dialogs.at(3).SetFigure("mika");
		dialogs.at(3).SetParam({ "I feel a bit cold. " }, false);
		dialogs.at(4).SetFigure("takesi");
		dialogs.at(4).SetParam({ "h...","hey","I think we should go home. "}, false);
		dialogs.at(5).SetFigure("takurou");
		dialogs.at(5).SetParam({ "What is wrong with you takesi,"," are you afraid ? "}, false);
		dialogs.at(6).SetFigure("takesi");
		dialogs.at(6).SetParam({ "!!!!!" }, false);
		dialogs.at(7).SetFigure("takesi");
		dialogs.at(7).SetParam({ "h...","hey","we should go home. "}, false);
		dialogs.at(8).SetFigure("hirosi");
		dialogs.at(8).SetParam({ "what a fool!","there are no ghosts in this world."}, false);
		dialogs.at(9).SetFigure("takesi");
		dialogs.at(9).SetParam({ "hey","careful Hirosi" }, false);
		dialogs.at(10).SetFigure("hirosi");
		dialogs.at(10).SetParam({ "!" },false);
		dialogs.at(11).SetFigure("hirosi");
		dialogs.at(11).SetParam({ "!" },false);
		dialogs.at(12).SetFigure("hirosi");
		dialogs.at(12).SetParam({ "where is everyone" }, false);
		dialogs.at(13).SetFigure("hirosi");
		dialogs.at(13).SetParam({ "Gain the library key " }, false);
		dialogs.at(14).SetFigure("hirosi");
		dialogs.at(14).SetParam({ "Gain the detergent " }, false);
		dialogs.at(15).SetFigure("hirosi");
		dialogs.at(15).SetParam({ "Gain the phillips screwdriver " }, false);
		dialogs.at(16).SetFigure("hirosi");
		dialogs.at(16).SetParam({ "Gain the bedroom key" }, false);
		dialogs.at(17).SetFigure("hirosi");
		dialogs.at(17).SetParam({ "Gain the lighter " }, false);
		dialogs.at(18).SetFigure("takesi");
		dialogs.at(18).SetParam({ "Trembling........................." }, false);
		dialogs.at(19).SetFigure("hirosi");
		dialogs.at(19).SetParam({ "Takashi?" }, false);
		dialogs.at(20).SetFigure("takesi");
		dialogs.at(20).SetParam({ "Trembling........................." }, false);
		dialogs.at(21).SetFigure("hirosi");
		dialogs.at(21).SetParam({ "where is everyone?" }, false);
		dialogs.at(22).SetFigure("takesi");
		dialogs.at(22).SetParam({ "Trembling........................." }, false);
		dialogs.at(23).SetFigure("hirosi");
		dialogs.at(23).SetParam({ "......................................................." }, false);
		dialogs.at(24).SetFigure("hirosi");
		dialogs.at(24).SetParam({ "Gain the handkerchief" }, false);
		
		dialogs.at(25).SetFigure("mika");
		dialogs.at(25).SetParam({ "Hiroshi you..." }, false);
		dialogs.at(26).SetFigure("hirosi");
		dialogs.at(26).SetParam({ "Are you ok?","where are the others ?" }, false);
		dialogs.at(27).SetFigure("mika");
		dialogs.at(27).SetParam({ "i don't know. ","mabye hinding somewhere" }, false);
		dialogs.at(28).SetFigure("hirosi");
		dialogs.at(28).SetParam({ "Okay." }, true);
		dialogs.at(28).SetOption("come with me ", "stay here for now");
		
		dialogs.at(29).SetFigure("mika");
		dialogs.at(29).SetParam({ "WhAt?","there's a MONSTER wondering in this house." }, false);
		dialogs.at(30).SetFigure("hirosi");
		dialogs.at(30).SetParam({ "Alright..."}, false);
		dialogs.at(31).SetFigure("mika");
		dialogs.at(31).SetParam({ "ok." }, false);
		dialogs.at(32).SetFigure("mika");
		dialogs.at(32).SetParam({ "takuro................" }, false);

		dialogs.at(33).SetFigure("hirosi");
		dialogs.at(33).SetParam({ "Gain the babyroom key" }, false);
		dialogs.at(34).SetFigure("hirosi");
		dialogs.at(34).SetParam({ "Gain the basement key" }, false);
		dialogs.at(35).SetFigure("hirosi");
		dialogs.at(35).SetParam({ "Gain the jail key" }, false);
		dialogs.at(36).SetFigure("hirosi");
		dialogs.at(36).SetParam({ "Gain the ??? key" }, false);
		dialogs.at(37).SetFigure("hirosi");
		dialogs.at(37).SetParam({ "The door is locked" }, false);
		dialogs.at(38).SetFigure("hirosi");
		dialogs.at(38).SetParam({ "The door is unlocked" }, false);

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
		// debug
		grid.LoadBitmapByString({ "img/grid.bmp" }, RGB(0, 0, 0));
		tileplaceholder.LoadBitmapByString({ "img/placeholder.bmp" });
		inputbox.Load("img/cursor/input_box.bmp");
		inputbox.init(20 * TILE, 0, 0, 10);
		// map link data
		router.init();
		router.Load("map_bmp/maplink.txt");
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
		_nowID = 13;
		_dialogID = -1;
		_dialogcount = 0;
		_eventID = 0;
		_playerStep = 0;
		player.init(4, 16, Entity::left);
		player.SetPos(14*TILE, 13 * TILE);
		human_mika.init(-1, 16,Entity::right);
		human_mika.SetPos(10 * TILE,12 * TILE);
		human_takeshi.init(-1, 16, Entity::up);
		human_takeshi.SetPos(13 * TILE,14 * TILE);
		human_takuro.init(-1, 16, Entity::down);
		human_takuro.SetPos(12 * TILE, 12 * TILE);
		normal_oni.init(Oni::normal, 4, 8);
		//redChair.Reset();
		//normal_oni.SetPos(11 * TILE, 10 * TILE);
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
		
		for (int i = 0; i < int(items.size()); i++) {
			items.at(i).ResetUtil();
		}
	}

	void CGameStateRun::OnMove()
	{
		mapmask.SetTopLeft(player.GetX() - TILE * 15, player.GetY() - TILE * 16);
		if ((player.GetX() - gamemaps.at(_nowID).GetX()) % TILE == 16 || (player.GetY() - gamemaps.at(_nowID).GetY()) % TILE == 16) {
			_playerStep++;
			game_interface.StorePlayerStep(_playerStep);
		}
		inputbox.OnMove();
		if (events.at(START_EVENT_E).IsTransMap()) {
			_nowID = player.NextMapID();
			events.at(START_EVENT_E).IsTransMap() = false;
		}
		
		if (_substate != OnDialogs) {
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
			items.at(CLOSET_HIROSI_L).StorePlayerPos(player.GetX(), player.GetY());
			items.at(CLOSET_HIROSI_L).OnMove();
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
		case 10:
			items.at(TATAMI_R).StorePlayerPos(player.GetX(), player.GetY());
			items.at(TATAMI_R).OnMove();
			items.at(LIGHTER).StorePlayerPos(player.GetX(), player.GetY());
			items.at(LIGHTER).OnMove();
			if (items.at(LIGHTER).IsPick()) {
				items.at(TATAMI_L).StorePlayerPos(player.GetX(), player.GetY());
				items.at(TATAMI_L).OnMove();
			}

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
			items.at(HANDKERCHIEF).StorePlayerPos(player.GetX(), player.GetY());
			items.at(HANDKERCHIEF).OnMove();
			if (!items.at(HANDKERCHIEF).IsPick()) {
				gamemaps.at(_nowID).SetMapData(0, (items.at(HANDKERCHIEF).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(HANDKERCHIEF).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);
			}
			else {
				gamemaps.at(_nowID).SetMapData(0, (items.at(HANDKERCHIEF).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(HANDKERCHIEF).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 312);
			}
			items.at(CLOSET_SHAKE).StorePlayerPos(player.GetX(), player.GetY());
			items.at(CLOSET_SHAKE).OnMove();
			items.at(CLOSET_TAKESI_0).StorePlayerPos(player.GetX(), player.GetY());
			items.at(CLOSET_TAKESI_0).OnMove();
			// CLOSET_TAKESI_1 not have on move*/
			items.at(CLOSET_HIROSI_R).StorePlayerPos(player.GetX(), player.GetY());
			items.at(CLOSET_HIROSI_R).OnMove();
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
			gamemaps.at(_nowID).SetMapData(0, (human_mika.GetY() - gamemaps.at(_nowID).GetY())/TILE, (human_mika.GetX() - gamemaps.at(_nowID).GetX())/TILE, 0);
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
				normal_oni.ResetOni();
				GotoGameState(GAME_STATE_OVER);
			}
			else {
				normal_oni.OnMove(gamemaps.at(_nowID));
			}
		}
		if (normal_oni.IsWait()) {
			normal_oni.OnMove(gamemaps.at(_nowID));

		}

	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		
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
		else if (nChar == VK_ESCAPE) {
			//TRACE("\n\n%s\n\n", game_interface.GetRealTime().c_str());
			game_interface.OnKeyDown(nChar);
		}
		else if (game_interface.IsShow()) {
			game_interface.OnKeyDown(nChar);
		}
		else if (_substate == OnWalking) {
			
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
				GotoGameState(GAME_STATE_OVER);
			}
			switch (_nowID) {
			case 0:
				if (!items.at(BASEMENT_KEY).IsPick() && pwds.at(basement).IsOpen()) {
					items.at(BASEMENT_KEY).SetDirection(player.GetDirection());
					items.at(BASEMENT_KEY).OnKeyDown(nChar);
				}
				if (!items.at(BASEMENT_PWD).IsClose() && pwds.at(basement).IsShow()) {
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
				if (items.at(BOOKCASE_R).IsFixed() && !items.at(BASEMENT_PWD).IsPick()) {
					items.at(BASEMENT_PWD).OnKeyDown(nChar);
				}

				if (objs.at(obj_move::house1_basement2_chair).isChangeMap())
					objs.at(obj_move::house1_basement2_chair).OnKeyDown(nChar);
				
				break;
			case 1:
				items.at(FLATHEAD).OnKeyDown(nChar);
				objs.at(obj_move::house1_basement2_chair).OnKeyDown(nChar);
				break;
			case 2:
				items.at(CLOSET_HIROSI_L).OnKeyDown(nChar);
				break;
			case 3:
				items.at(GATE2).OnKeyDown(nChar);
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
					TRACE("14 CLOSET_SHAKE IsFixed true\n");
				}
				else{
					TRACE("14 CLOSET_SHAKE IsFixed false\n");
				}
				//CLOSET_TAKESI_1 not have on key down*/
				//items.at(CLOSET_HIROSI_R).OnKeyDown(nChar);
				break;
			case 15:
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
				break;
			case 17:
				if (!items.at(BB_KEY).IsPick() && pwds.at(piano).IsOpen()) {
					items.at(BB_KEY).SetDirection(player.GetDirection());
					items.at(BB_KEY).OnKeyDown(nChar);
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
				items.at(OIL).OnKeyDown(nChar);
				objs.at(obj_move::house1_2F_TL_chair).OnKeyDown(nChar);
				human_mika.StorePlayerPos(player.GetX(), player.GetY());
				human_mika.OnKeyDown(nChar);
				break;
			case 21:
				items.at(BOOKCASE_MAP21).OnKeyDown(nChar);
				break;
			case 22:
				items.at(KEY_BASEMENT).OnKeyDown(nChar);
				items.at(GATE).OnKeyDown(nChar);
				break;
			}
			if (!_pwd) {
				player.OnKeyDown(nChar);
			}
		}
		else if (_substate == OnDialogs) {
			if (_dialogID == 28) {
				dialogs.at(_dialogID).GetSelect(nChar);
			}
			if (nChar == VK_SPACE) {
				if (!((_dialogID == 12 || _dialogID == 11 || _dialogID == 8)&& !player.IsMachineDone())) {
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
							events.at(LIB_KEY_CHASE).SetTriggered(true);
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
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		player.OnKeyUp(nChar);
		player.CheckMapChangeTN(gamemaps.at(_nowID), router, _nowID, blockTeleportCor, game_interface);
		if (player.IsDoorLock() && events.at(DOOR_LOCKED_E).IsTriggered() && nChar == VK_SPACE) {
			player.IsDoorLock() = false;
			events.at(DOOR_LOCKED_E).SetTriggered(false);
		}
		if (player.IsDoorOpen() && events.at(DOOR_UNLOCKED_E).IsTriggered() && nChar == VK_SPACE) {
			player.IsDoorOpen() = false;
			events.at(DOOR_UNLOCKED_E).SetTriggered(false);
		}
		if ((player.IsMapChanged())) {
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
		if (!(_dialogID >= 2 && _dialogID <= 11) && _nowID!=0 && _nowID != 6 && _nowID != 10 && _nowID != 12 && _nowID != 14 && _nowID != 15 && _nowID !=17 && _nowID != 19 && _nowID != 21) {
			gamemaps.at(_nowID).ShowMapAll(player, normal_oni, mapoverlayindex.at(_nowID));
		}
		
		if (player.IsDoorLock() && !events.at(DOOR_LOCKED_E).IsTriggered()) {
			SetEventTriggeredDialog(DOOR_LOCKED_E);
		}
		if (player.IsDoorOpen() && !events.at(DOOR_UNLOCKED_E).IsTriggered()) {
			SetEventTriggeredDialog(DOOR_UNLOCKED_E);
		}
	
		if (_nowID == 0) {
			player.CMPY() = (player.GetY() - gamemaps.at(_nowID).GetY()) ;
			normal_oni.CMPY() = normal_oni.GetPosY() + normal_oni.GetOffsetY() - gamemaps.at(_nowID).GetY() ;
			objs.at(obj_move::house1_basement2_chair).CMPY() = (objs.at(obj_move::house1_basement2_chair).GetPosY() - gamemaps.at(_nowID).GetY()) ;
			entities = { &player, &objs.at(obj_move::house1_basement2_chair), &normal_oni };
			std::sort(entities.begin(), entities.end(), [&](Entity * a, Entity * b) {
				return a->CMPY() < b->CMPY();
			});
			bool trishow[3] = { true,true,true };
			for (int i = 1;i < gamemaps.at(_nowID).GetLayer();i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == 2) {
					if (!items.at(BASEMENT_PWD).IsPick() && !pwds.at(basement).IsOpen()) {
						items.at(BASEMENT_PWD).OnShow();
						//TRACE("\n\nppppppassssssswordddddd\n\n");
					}
					else if (!items.at(BASEMENT_KEY).IsPick() && items.at(BASEMENT_PWD).IsPick()) {
						items.at(BASEMENT_KEY).OnShow();
						//TRACE("\n\nkeyyyyyyyyyyyyy\n\n");
					}
					else {
						items.at(BASEMENT_PWD_TAKE).OnShow();
						//TRACE("\n\ntttttakkkkkkeeee\n\n");
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
				//TRACE("name : %s %s %s\n",entities[0]->ObjClass().c_str(), entities[1]->ObjClass().c_str(), entities[2]->ObjClass().c_str());
				//TRACE("CMPY : %d %d %d\n", entities[0]->CMPY(), entities[1]->CMPY(), entities[2]->CMPY());
				for (int j = 0; j < 3; j++) {
					if (((i == 2 && entities[j]->CMPY() < 8 * TILE) || (i == 3 && entities[j]->CMPY() < 14*TILE) || i == 4) && trishow[j]) {
						if (obj = dynamic_cast<ObjMove*>(entities[j])) {
							obj->OnShowConditional();
							TRACE("%s\n",obj->ObjClass().c_str());
						}
						else {
							entities[j]->OnShow();
							TRACE("%s\n", entities[j]->ObjClass().c_str());
						}
						trishow[j] = false;
					}
				}
			}
			values.clear();
			entities.clear();
			items.at(KEY_JAIL).OnShow();			
			gamemaps.at(_nowID).ShowMapTile();
			if (!items.at(BASEMENT_PWD).IsClose() && !pwds.at(basement).IsOpen()) {
				if (!pwds.at(basement).IsShow()) {
					pwds.at(basement).SetShow(true);
				}
				pwds.at(basement).ShowTotal();
			}
		}
		else if( _nowID == 1) {
			items.at(FLATHEAD).OnShow();
			if (!objs.at(obj_move::house1_basement2_chair).isChangeMap()) {
				objs.at(obj_move::house1_basement2_chair).OnShow();
			}
			else {
				objs.at(obj_move::house1_basement2_chair).ChangeMap();
			}

		}
		else if( _nowID == 2) {
			if (objs.at(obj_move::house1_basement2_chair).isChangeMap()) {
				objs.at(obj_move::house1_basement2_chair).ChangeMap();
			}
			items.at(CLOSET_HIROSI_L).OnShow();
		}
		else if( _nowID == 3) {
			items.at(GATE2).OnShow();
		}
		else if( _nowID == 6) {
			items.at(DOOR_ONI).EventTrigger();
			for (int i = 0;i < gamemaps.at(_nowID).GetLayer();i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					items.at(DOOR_ONI).OnShow();
					ShowOniAndPlayer();
				}
			}

		}
		else if( _nowID == 7) {
			objs.at(obj_move::house1_2F_TR_chair).ChangeMap();
			objs.at(obj_move::house1_2F_TL_chair).ChangeMap();
		}
		else if( _nowID == 9) {
			if (objs.at(obj_move::house1_basement2_chair).isChangeMap()) {
				objs.at(obj_move::house1_basement2_chair).ChangeMap();
			}
		}
		else if( _nowID == 10) {
			for (int i = 0;i < gamemaps.at(_nowID).GetLayer();i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					items.at(LIGHTER).OnShow();
					items.at(TATAMI_R).OnShow();
					if (items.at(LIGHTER).IsPick()) {
						items.at(TATAMI_L).OnShow();
					}
					ShowOniAndPlayer();
				}
			}
			if (items.at(LIGHTER).IsPick() && !events.at(LIGHTER_E).IsTriggered()) {
				SetEventTriggeredDialog(LIGHTER_E);
				game_interface.StoreItem("(need oil) lighter", "lighter", Interface::Items::lighter);
			}
		}
		else if( _nowID == 11) {
			items.at(BROKEN_DISH).OnShow();
			if (items.at(BROKEN_DISH).IsTake() && !events.at(BROKEN_DISH_E).IsTriggered()) {
				SetEventTriggeredDialog(BROKEN_DISH_E);
				
				game_interface.StoreItem("broken dish", "plate shards", Interface::Items::broken_dish);
			}
		}
		else if( _nowID == 12) {
			player.CMPY() = (player.GetY() - gamemaps.at(_nowID).GetY());
			normal_oni.CMPY() = normal_oni.GetPosY() + normal_oni.GetOffsetY() - gamemaps.at(_nowID).GetY();
			entities = { &player, &normal_oni };
			std::sort(entities.begin(), entities.end(), [&](Entity* a, Entity* b) {
				return a->CMPY() < b->CMPY();
				});
			bool bishow[2] = { true,true};

			for (int i = 1;i < gamemaps.at(_nowID).GetLayer();i++) {
				gamemaps.at(_nowID).ShowMap(i);
				for (int j = 0;j < 2; j++) {
					if (((i == 4 && entities[j]->CMPY() < 4*TILE) || (i == 5 && entities[j]->CMPY() < 8 * TILE) || (i == 6 && entities[j]->CMPY() < 12 * TILE) || (i == 7 && entities[j]->CMPY() < 15 * TILE) || i == 8) && bishow[j]) {
						entities[j]->OnShow();
						bishow[j] = false;
					}
				}
			}
			items.at(LIB_BOOK).OnShow();
			if (items.at(LIB_BOOK).IsFixed()) {
				items.at(KEY_3F_L).OnShow();
			}
			if (items.at(KEY_3F_L).IsPick() && !events.at(KEY_3F_L_E).IsTriggered()) {
				SetEventTriggeredDialog(KEY_3F_L_E);
				game_interface.StoreItem("3F left", "bedroom key", Interface::Items::key_3F_L);
			}
		}
		else if( _nowID == 13) {

			if (!events.at(START_EVENT_E).IsTriggered()) {
				SetEventTriggeredDialog(START_EVENT_E);
			}
			if (!events.at(START_EVENT2_E).IsTriggered() && events.at(BROKEN_DISH_E).IsTriggered()) {
				SetEventTriggeredDialog(START_EVENT2_E);
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
		}
		else if( _nowID == 14) {
			for (int i = 0;i < gamemaps.at(_nowID).GetLayer();i++) {
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
					items.at(HANDKERCHIEF).OnShow();
					if (items.at(HANDKERCHIEF).IsPick() && !events.at(HANDKERCHIEF_E).IsTriggered()) {
						SetEventTriggeredDialog(HANDKERCHIEF_E);
						game_interface.StoreItem("(dirty) handkerchief", "handkerchief", Interface::Items::handkerchief);
					}
					if ((player.GetY() - gamemaps.at(_nowID).GetY())/TILE <= 3 && (player.GetX() - gamemaps.at(_nowID).GetX()) / TILE >= 5) {
						items.at(CLOSET_SHAKE).EventTrigger();
					}
					if (!items.at(CLOSET_SHAKE).IsFixed() || !items.at(CLOSET_SHAKE).IsAnimationDone()) {
						items.at(CLOSET_SHAKE).OnShow();
					}
					if (items.at(CLOSET_SHAKE).IsFixed() && items.at(CLOSET_SHAKE).IsAnimationDone()) {
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
					ShowOniAndPlayer();
				}
			}
		}
		else if( _nowID == 15) {
			for (int i = 0;i < gamemaps.at(_nowID).GetLayer();i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					if (!items.at(DOOR_KNOB).IsPick()) {
						items.at(DOOR_KNOB).OnShow();
					}
					if (items.at(DOOR_KNOB).IsPick()) {
						items.at(DOOR_NO_KNOB).OnShow();
					}
					ShowOniAndPlayer();
				}
			}
		}
		else if( _nowID == 16) {
			items.at(BED).OnShow();
			if (items.at(BED).IsOnCorPos() && !events.at(OPEN_FUCKING_HOLE_E).IsTriggered()) {
				router.UnblockPath(16, 17);
			}
		}
		else if( _nowID == 17) {
			for (int i = 0;i < gamemaps.at(_nowID).GetLayer();i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					if (!items.at(PIANO_PWD_NOTOPEN).IsPick() && !pwds.at(piano).IsOpen()) {
						items.at(PIANO_PWD_NOTOPEN).OnShow();
					}
					else if(!items.at(BB_KEY).IsPick() && items.at(PIANO_PWD_NOTOPEN).IsPick()) {
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
			if (!items.at(PIANO_PWD_NOTOPEN).IsClose() && !pwds.at(piano).IsOpen()) {
				if (!pwds.at(piano).IsShow()) {
					pwds.at(piano).SetShow(true);
				}
				pwds.at(piano).ShowTotal();
			}
		}
		else if( _nowID == 18) {
			if (!items.at(TUB_ONCE).IsFixed() || !items.at(TUB_ONCE).IsAnimationDone()) {
				items.at(TUB_ONCE).OnShow();
			}
			if (items.at(TUB_ONCE).IsFixed() && items.at(TUB_ONCE).IsAnimationDone()) {
				items.at(PHILLIPS).OnShow();
			}
			if (items.at(PHILLIPS).IsPick()) {
				items.at(TUB_FIXED).OnShow();
			}
			if (items.at(PHILLIPS).IsPick() && !events.at(PHILLIPS_E).IsTriggered()) {
				SetEventTriggeredDialog(PHILLIPS_E);
				game_interface.StoreItem("phillips screwdriver", "phillips scredriver", Interface::Items::phillips);
			}
		}
		else if( _nowID == 19) {
			for (int i = 0;i < gamemaps.at(_nowID).GetLayer();i++) {
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
		}
		else if( _nowID == 20) {
			items.at(OIL).OnShow();
			human_mika.OnShow();
			if (_dialogID>=25 && _dialogID<=32) {
				human_mika.SetDirection(Entity::up);
			}
			else {
				human_mika.SetDirection(Entity::down);

			}
			objs.at(obj_move::house1_2F_TL_chair).OnShow();
			if (human_mika.Trigger()&&!events.at(MIKA_SCARE_E).IsTriggered()) {
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
			if ((events.at(MIKA_NOTOK_E).IsTriggered()|| events.at(MIKA_OK_E).IsTriggered()) && 
				!events.at(MIKA_REPEAT_E).IsTriggered() && human_mika.Trigger()) {
				SetEventTriggeredDialog(MIKA_REPEAT_E);
				human_mika.Trigger() = false;
				events.at(MIKA_REPEAT_E).SetTriggered(false);
			}
		}
		else if( _nowID == 21) {
			for (int i = 0;i < gamemaps.at(_nowID).GetLayer();i++) {
				gamemaps.at(_nowID).ShowMap(i);
				if (i == mapoverlayindex.at(_nowID)) {
					items.at(BOOKCASE_MAP21).OnShow();
					ShowOniAndPlayer();
				}
			}
		}
		else if( _nowID == 22) {
			items.at(KEY_BASEMENT).OnShow();
			if (items.at(KEY_BASEMENT).IsPick() && !events.at(KEY_BASEMENT_E).IsTriggered()) {
				SetEventTriggeredDialog(KEY_BASEMENT_E);
				game_interface.StoreItem("basement", "basement key", Interface::Items::key_basement);
			}
			items.at(GATE).OnShow();
		}
		for (int i = 0; i < 39; i++) {
			if (dialogs.at(i).isShow()) {
				dialogs.at(i).ShowTotal();
			}
		}
		DeBugRecursive();

		if (game_interface.IsShow()) {
			game_interface.ShowTotal();
		}
	}
	void CGameStateRun::map0func1(int chairy, int humany, int oniy)
	{
		if (chairy < humany && humany < oniy) {
			objs.at(obj_move::house1_basement2_chair).OnShowConditional();
			player.OnShow();
			normal_oni.OnShow();
		}
		else if (humany < chairy && chairy < oniy) {
			player.OnShow();
			objs.at(obj_move::house1_basement2_chair).OnShowConditional();
			normal_oni.OnShow();
		}
		else if (humany < oniy && oniy < chairy) {
			player.OnShow();
			normal_oni.OnShow();
			objs.at(obj_move::house1_basement2_chair).OnShowConditional();
		}
		else if (chairy < oniy && oniy < humany) {
			objs.at(obj_move::house1_basement2_chair).OnShowConditional();
			normal_oni.OnShow();
			player.OnShow();
		}
		else if (oniy < chairy && chairy < humany) {
			normal_oni.OnShow();
			objs.at(obj_move::house1_basement2_chair).OnShowConditional();
			player.OnShow();
		}
		else if (oniy < humany && humany < chairy) {
			normal_oni.OnShow();
			player.OnShow();
			objs.at(obj_move::house1_basement2_chair).OnShowConditional();
		}
	}
	void CGameStateRun::ShowOniAndPlayer() {
		if (normal_oni.GetPosD() > player.GetD()) {
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
		_dialogID = events.at(_eventID).GetDialogIndex();
		dialogs.at(_dialogID).SetShow(true);
		_substate = OnDialogs;
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
			player.IsTimerStart()? CTextDraw::Print(pDC, 0, TILE * 8, "timer start") : CTextDraw::Print(pDC, 0, TILE * 8, "timer stop");
			(istwoway != 0) ? ((istwoway == 1) ? CTextDraw::Print(pDC, 0, TILE * 9, "is twoway : yes") : CTextDraw::Print(pDC, 0, TILE * 9, "is twoway : no")) : CTextDraw::Print(pDC, 0, TILE * 9, "is twoway : none");
			
			//player.IsMachineDone() ? CTextDraw::Print(pDC, 0, TILE * 10, "machine done") : CTextDraw::Print(pDC, 0, TILE * 10, "machine not done");
			router.Gaming() ? CTextDraw::Print(pDC, 0, TILE * 10, "All path state : Gaming") : CTextDraw::Print(pDC, 0, TILE * 10, "All path state : Unlock all");
			CTextDraw::Print(pDC, 0, TILE * 11, "Oni overtime : " + to_string(normal_oni.GetOverTimer()));
			player.IsMapChanged() ? CTextDraw::Print(pDC, 0, TILE * 12, "player.IsMapChanged(): T") : CTextDraw::Print(pDC, 0, TILE * 12, "player.IsMapChanged(): F");
			//player.IsSwitchMap() ? CTextDraw::Print(pDC, 0, TILE * 13, "All path state : Gaming") : CTextDraw::Print(pDC, 0, TILE * 13, "All path state : Unlock all");
			CTextDraw::Print(pDC, 0, TILE * 13, to_string(human_mika.GetY()));
			CTextDraw::Print(pDC, 0, TILE * 14, to_string(human_mika.GetX()));
			
			
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