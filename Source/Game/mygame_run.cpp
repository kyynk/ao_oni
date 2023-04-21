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
		// thank god
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
		human_Takuruo.Load(playervec, RGB(204, 255, 0));
		// map resources
		std::ifstream mapres_in("map_bmp/mapsize.txt");
		string name;
		int count;
		for (int i = 0; i < 12; i++) {
			mapres_in >> name >> count;
			ShowInitProgress(33 + 2*i, name);
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
		items.resize(50);
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
		//events
		events.resize(30);
		events.at(BROKEN_DISH_E).SetParam({ {5,12},{5,13} }, 0,2 );
		events.at(START_EVENT).SetParam({ },2,8);
		events.at(START_EVENT2).SetParam({}, 10,3);
		events.at(KEY_LIB_E).SetParam({}, 13, 1);
		events.at(DETERGENT_E).SetParam({}, 14, 1);
		events.at(PHILLIPS_E).SetParam({}, 15, 1);
		events.at(KEY_3F_L_E).SetParam({}, 16, 1);
		events.at(LIGHTER_E).SetParam({}, 17, 1);
		//dialogs
		dialogs.resize(30);

		dialogs.at(0).SetFigure(Dialog::hirosi);
		dialogs.at(0).SetParam({ "A broken plate... " }, false);
		dialogs.at(1).SetFigure(Dialog::hirosi);
		dialogs.at(1).SetParam({"Gain the broken plate"},false);
		dialogs.at(2).SetFigure(Dialog::takurou);
		dialogs.at(2).SetParam({ "beautiful interiors!!! " }, false);
		dialogs.at(3).SetFigure(Dialog::mika);
		dialogs.at(3).SetParam({ "I feel a bit cold. " }, false);
		dialogs.at(4).SetFigure(Dialog::takesi);
		dialogs.at(4).SetParam({ "h..","hey","I think we should go home. "}, false);
		dialogs.at(5).SetFigure(Dialog::takurou);
		dialogs.at(5).SetParam({ "What is  wrong with you takesi,"," are you afraid ? "}, false);
		dialogs.at(6).SetFigure(Dialog::takesi);
		dialogs.at(6).SetParam({ "!!!!!" }, false);
		dialogs.at(7).SetFigure(Dialog::takesi);
		dialogs.at(7).SetParam({ "h...","hey","we should go home. "}, false);
		dialogs.at(8).SetFigure(Dialog::hirosi);
		dialogs.at(8).SetParam({ "what a fool!","there are no ghosts in this world."}, false);
		dialogs.at(9).SetFigure(Dialog::takesi);
		dialogs.at(9).SetParam({ "hey","careful Hirosi" }, false);
		dialogs.at(10).SetFigure(Dialog::hirosi);
		dialogs.at(10).SetParam({ "!" },false);
		dialogs.at(11).SetFigure(Dialog::hirosi);
		dialogs.at(11).SetParam({ "!" },false);
		dialogs.at(12).SetFigure(Dialog::hirosi);
		dialogs.at(12).SetParam({ "where is everyone" }, false);
		dialogs.at(13).SetFigure(Dialog::hirosi);
		dialogs.at(13).SetParam({ "Gain the library key " }, false);
		dialogs.at(14).SetFigure(Dialog::hirosi);
		dialogs.at(14).SetParam({ "Gain the detergent " }, false);
		dialogs.at(15).SetFigure(Dialog::hirosi);
		dialogs.at(15).SetParam({ "Gain the phillips screwdriver " }, false);
		dialogs.at(16).SetFigure(Dialog::hirosi);
		dialogs.at(16).SetParam({ "Gain the bedroom key" }, false);
		dialogs.at(17).SetFigure(Dialog::hirosi);
		dialogs.at(17).SetParam({ "Gain the lighter " }, false);

		// objMove
		house1_2F_TR_chair.SetParam(ObjMove::ObjType::house1_2F_TR_chair,
			8, 4, 0, 0, 15 * TILE, 9 * TILE);
		house1_2F_TL_chair.SetParam(ObjMove::ObjType::house1_2F_TL_chair,
			8, 4, 0, TILE / 2, 14 * TILE, 15 * TILE); // 14 15
		house1_basement2_chair.SetParam(ObjMove::ObjType::house1_basement2_chair,
			8, 4, 0, TILE / 2, 9 * TILE, 14 * TILE); // 9 14
		// debug
		grid.LoadBitmapByString({ "img/grid.bmp" }, RGB(0, 0, 0));
		tileplaceholder.LoadBitmapByString({ "img/placeholder.bmp" });
		inputbox.Load("img/cursor/input_box.bmp");
		inputbox.init(20 * TILE, 0, 0, 10);
		// map link data
		router.init();
		router.Load("map_bmp/maplink.txt");
		//router.debug();
		darkroom.resize(65);
		for (int i = 0;i < 65;i++) {
			if(i ==21){
				darkroom.at(i) = true;
			}
			else
			darkroom.at(i) = false;
		}
	}
	void CGameStateRun::OnBeginState()
	{
		_substate = OnWalking;
		oni1.SetXY(10 * TILE, 11 * TILE + 80);
		mousex_foc = 0;
		mousey_foc = 0;
		isdebugmode = false;
		//isbs = 0;
		istwoway = 0;
		isteleportblock = false;
		isedit = false;
		isgrid = false;
		_nowID = 13;
		_dialogID = -1;
		_dialogcount = 0;
		_eventID = 0;
		player.init(4, 16, Human::left);
		player.SetXYAndCol(14, 13);
		human_mika.init(-1, 16,Human::right);
		human_mika.SetXYAndCol(10,12);
		human_takeshi.init(-1, 16, Human::up);
		human_takeshi.SetXYAndCol(13,14);
		human_Takuruo.init(-1, 16, Human::down);
		human_Takuruo.SetXYAndCol(12, 12);
		oni1.SetParam(Oni::OniType::normal, 4, 8);
		//redChair.Reset();
		oni1.SetPos(11 * TILE, 13 * TILE);
		house1_2F_TR_chair.Reset();
		house1_2F_TR_chair._preX = house1_2F_TR_chair.GetPosX();
		house1_2F_TR_chair._preY = house1_2F_TR_chair.GetPosY();
		house1_2F_TL_chair.Reset();
		house1_2F_TL_chair._preX = house1_2F_TL_chair.GetPosX();
		house1_2F_TL_chair._preY = house1_2F_TL_chair.GetPosY();
		house1_basement2_chair.Reset();
		house1_basement2_chair._preX = house1_basement2_chair.GetPosX();
		house1_basement2_chair._preY = house1_basement2_chair.GetPosY();
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
		//items end
		//event
		

	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		mapmask.SetTopLeft(player.GetX() - TILE * 15, player.GetY() - TILE * 16);

		inputbox.OnMove();
		if (events.at(START_EVENT).IsTransMap()) {
			_nowID = player.NextMapID();
			events.at(START_EVENT).IsTransMap() = false;
		}
		if ((player.IsMapChanged() && player.IsSwitchMap())) {
			_nowID = player.NextMapID();
		}
		if (_substate != OnDialogs) {
			player.OnMove(gamemaps.at(_nowID), router, _nowID, blockLeftCor, blockRightCor, blockTeleportCor);
		}
		if (_nowID == 0) {
			items.at(KEY_JAIL).GetPlayerPos(player.GetX(), player.GetY());
			items.at(KEY_JAIL).OnMove();
			items.at(BOOKCASE_L).GetPlayerPos(player.GetX(), player.GetY());
			items.at(BOOKCASE_L).OnMove();
			if (items.at(BOOKCASE_L).IsFixed()) {
				items.at(BOOKCASE_R).GetPlayerPos(player.GetX(), player.GetY());
				items.at(BOOKCASE_R).OnMove();

				gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BOOKCASE_L).GetPosX() + 3 * TILE- gamemaps.at(_nowID).GetX()) / TILE, 312);				//DL
				gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BOOKCASE_L).GetPosX() + TILE + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);		//DM
				gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BOOKCASE_L).GetPosX() + 2 * TILE + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);	//DR

				gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BOOKCASE_L).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);				//DL
				gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BOOKCASE_L).GetPosX() + TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);		//DM
				gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BOOKCASE_L).GetPosX() + 2 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);	//DR
			}
			else {
				gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BOOKCASE_L).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);				//DL
				gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BOOKCASE_L).GetPosX() + TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);		//DM
				gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BOOKCASE_L).GetPosX() + 2 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);	//DR
			}
			if (!items.at(BOOKCASE_R).IsFixed()) {
				gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BOOKCASE_R).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);				//DL
				gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BOOKCASE_R).GetPosX() + TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);		//DM
				gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BOOKCASE_R).GetPosX() + 2 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);	//DR
			}
			else {
				if (items.at(BOOKCASE_R).IsFixed()) {		//if use "Y" and "I" will see next line first posX = 0
					if ((items.at(BOOKCASE_R).GetPosX() + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE != 14) {
						gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
							(items.at(BOOKCASE_R).GetPosX() + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);				//DL (reset)
					}
					if ((items.at(BOOKCASE_R).GetPosX() + TILE + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE != 14 &&
						(items.at(BOOKCASE_R).GetPosX() + TILE + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE != 13) {
						gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
							(items.at(BOOKCASE_R).GetPosX() + TILE + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);		//DM (reset)
					}
					if ((items.at(BOOKCASE_R).GetPosX() + 2 * TILE + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE != 14 &&
						(items.at(BOOKCASE_R).GetPosX() + 2 * TILE + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE != 13) {
						gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
							(items.at(BOOKCASE_R).GetPosX() + 2 * TILE + 3 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);	//DR (reset)
					}

					gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
						(items.at(BOOKCASE_R).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);				//DL
					gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
						(items.at(BOOKCASE_R).GetPosX() + TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);		//DM
					gamemaps.at(_nowID).SetMapData(0, (6 * TILE - gamemaps.at(_nowID).GetY()) / TILE,
						(items.at(BOOKCASE_R).GetPosX() + 2 * TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);	//DR
				}
			}
			if (house1_basement2_chair.isChangeMap()) {
				house1_basement2_chair.GetPlayerPos(player.GetX(), player.GetY());
				house1_basement2_chair.OnMove(gamemaps.at(_nowID));
				gamemaps.at(_nowID).SetMapData(0, (house1_basement2_chair._preY - gamemaps.at(_nowID).GetY()) / TILE,
					(house1_basement2_chair._preX - gamemaps.at(_nowID).GetX()) / TILE, 312);
				house1_basement2_chair._preX = house1_basement2_chair.GetPosX();
				house1_basement2_chair._preY = house1_basement2_chair.GetPosY();
				if (!(house1_basement2_chair._preX == 18 * TILE && house1_basement2_chair._preY == 8 * TILE) && 
					!(house1_basement2_chair._preX == 5 * TILE && house1_basement2_chair._preY == 8 * TILE)) {
					gamemaps.at(_nowID).SetMapData(0, (house1_basement2_chair._preY - gamemaps.at(_nowID).GetY()) / TILE,
						(house1_basement2_chair._preX - gamemaps.at(_nowID).GetX()) / TILE, 0);
				}
			}
		}
		else if (_nowID == 1) {
			items.at(FLATHEAD).GetPlayerPos(player.GetX(), player.GetY());
			items.at(FLATHEAD).OnMove();
			house1_basement2_chair.GetPlayerPos(player.GetX(), player.GetY());
			house1_basement2_chair.OnMove(gamemaps.at(_nowID));
			if (!house1_basement2_chair.isChangeMap()) {
				gamemaps.at(_nowID).SetMapData(0, (house1_basement2_chair._preY - gamemaps.at(_nowID).GetY()) / TILE,
					(house1_basement2_chair._preX - gamemaps.at(_nowID).GetX()) / TILE, 312);
				house1_basement2_chair._preX = house1_basement2_chair.GetPosX();
				house1_basement2_chair._preY = house1_basement2_chair.GetPosY();
				if (!(house1_basement2_chair._preX == 15 * TILE && house1_basement2_chair._preY == 12 * TILE)) {
					gamemaps.at(_nowID).SetMapData(0, (house1_basement2_chair._preY - gamemaps.at(_nowID).GetY()) / TILE,
						(house1_basement2_chair._preX - gamemaps.at(_nowID).GetX()) / TILE, 0);
				}
			}
		}
		else if (_nowID == 2) {
			items.at(CLOSET_HIROSI_L).GetPlayerPos(player.GetX(), player.GetY());
			items.at(CLOSET_HIROSI_L).OnMove();
		}
		else if (_nowID == 3) {
			items.at(GATE2).GetPlayerPos(player.GetX(), player.GetY());
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
		}
		else if (_nowID == 10) {
			items.at(TATAMI_R).GetPlayerPos(player.GetX(), player.GetY());
			items.at(TATAMI_R).OnMove();
			items.at(LIGHTER).GetPlayerPos(player.GetX(), player.GetY());
			items.at(LIGHTER).OnMove();
			if (items.at(LIGHTER).IsPick()) {
				items.at(TATAMI_L).GetPlayerPos(player.GetX(), player.GetY());
				items.at(TATAMI_L).OnMove();
			}
		}
		else if (_nowID == 11) {
			items.at(BROKEN_DISH).GetPlayerPos(player.GetX(), player.GetY());
			items.at(BROKEN_DISH).OnMove();

		}
		else if (_nowID == 12) {
			items.at(LIB_BOOK).GetPlayerPos(player.GetX(), player.GetY());
			items.at(LIB_BOOK).OnMove();
			if (items.at(LIB_BOOK).IsFixed()) {
				items.at(KEY_3F_L).GetPlayerPos(player.GetX(), player.GetY());
				items.at(KEY_3F_L).OnMove();
			}
		}
		else if (_nowID == 13) {
			if (_dialogID >= 2 && _dialogID <= 9) {
				player.OnMove();
				human_takeshi.OnMove();
				human_Takuruo.OnMove();
				human_mika.OnMove();
			}
			if (_dialogID == 8) {
				player.OnMoveBySettings(4);
			}
			if (_dialogID >= 10 && _dialogID <= 12) {
				player.OnMove();
			}
			if (_dialogID == 11) {
				player.OnMoveBySettings(6);
			}
		}
		else if (_nowID == 14) {
			house1_2F_TR_chair.GetPlayerPos(player.GetX(), player.GetY());
			house1_2F_TR_chair.OnMove(gamemaps.at(_nowID));
			
			gamemaps.at(_nowID).SetMapData(0, (house1_2F_TR_chair._preY - gamemaps.at(_nowID).GetY()) / TILE,
				(house1_2F_TR_chair._preX - gamemaps.at(_nowID).GetX()) / TILE, 312);

			house1_2F_TR_chair._preX = house1_2F_TR_chair.GetPosX();
			house1_2F_TR_chair._preY = house1_2F_TR_chair.GetPosY();
			gamemaps.at(_nowID).SetMapData(0, (house1_2F_TR_chair._preY - gamemaps.at(_nowID).GetY()) / TILE,
				(house1_2F_TR_chair._preX - gamemaps.at(_nowID).GetX()) / TILE, 0);
			
			if (house1_2F_TR_chair.IsFixed()) {
				items.at(KEY_LIB).GetPlayerPos(player.GetX(), player.GetY());
				items.at(KEY_LIB).OnMove();
				if (!items.at(KEY_LIB).IsPick()){
					gamemaps.at(_nowID).SetMapData(0, (items.at(KEY_LIB).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
						(items.at(KEY_LIB).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);
				}
				else {
					gamemaps.at(_nowID).SetMapData(0, (items.at(KEY_LIB).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
						(items.at(KEY_LIB).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 312);
				}
			}
			items.at(HANDKERCHIEF).GetPlayerPos(player.GetX(), player.GetY());
			items.at(HANDKERCHIEF).OnMove();
			if (!items.at(HANDKERCHIEF).IsPick()) {
				gamemaps.at(_nowID).SetMapData(0, (items.at(HANDKERCHIEF).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(HANDKERCHIEF).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);
			}
			else {
				gamemaps.at(_nowID).SetMapData(0, (items.at(HANDKERCHIEF).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(HANDKERCHIEF).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 312);
			}
			/*items.at(CLOSET_SHAKE).GetPlayerPos(player.GetX(), player.GetY());
			items.at(CLOSET_SHAKE).OnMove();
			items.at(CLOSET_TAKESI_0).GetPlayerPos(player.GetX(), player.GetY());
			items.at(CLOSET_TAKESI_0).OnMove();
			// CLOSET_TAKESI_1 not have on move*/
			items.at(CLOSET_HIROSI_R).GetPlayerPos(player.GetX(), player.GetY());
			items.at(CLOSET_HIROSI_R).OnMove();
		}
		else if (_nowID == 15) {
			if (!items.at(DOOR_KNOB).IsPick()) {
				items.at(DOOR_KNOB).GetPlayerPos(player.GetX(), player.GetY());
				items.at(DOOR_KNOB).OnMove();
			}
			if (items.at(DOOR_KNOB).IsPick()) {
				items.at(DOOR_NO_KNOB).GetPlayerPos(player.GetX(), player.GetY());
				items.at(DOOR_NO_KNOB).OnMove();
			}
		}
		else if (_nowID == 16) {
			items.at(BED).GetPlayerPos(player.GetX(), player.GetY());
			items.at(BED).OnMove();
			if (!items.at(BED).IsFixed()) {
				gamemaps.at(_nowID).SetMapData(0, (items.at(BED).GetPosY() + TILE / 2 - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BED).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);			//TL
				gamemaps.at(_nowID).SetMapData(0, (items.at(BED).GetPosY() + TILE / 2 + TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BED).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);			//DL
				gamemaps.at(_nowID).SetMapData(0, (items.at(BED).GetPosY() + TILE / 2 - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BED).GetPosX() + TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);	//TR
				gamemaps.at(_nowID).SetMapData(0, (items.at(BED).GetPosY() + TILE / 2 + TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BED).GetPosX() + TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);	//DR
			}
			else {
				gamemaps.at(_nowID).SetMapData(0, (items.at(BED).GetPosY() + TILE / 2 - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BED).GetPosX() + TILE + TILE - gamemaps.at(_nowID).GetX()) / TILE, 312);	//TR (reset)

				gamemaps.at(_nowID).SetMapData(0, (items.at(BED).GetPosY() + TILE / 2 - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BED).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);			//TL
				gamemaps.at(_nowID).SetMapData(0, (items.at(BED).GetPosY() + TILE / 2 + TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BED).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);			//DL
				gamemaps.at(_nowID).SetMapData(0, (items.at(BED).GetPosY() + TILE / 2 - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BED).GetPosX() + TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);	//TR
				gamemaps.at(_nowID).SetMapData(0, (items.at(BED).GetPosY() + TILE / 2 + TILE - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(BED).GetPosX() + TILE - gamemaps.at(_nowID).GetX()) / TILE, 0);	//DR
			}
		}
		else if (_nowID == 17) {
			items.at(WHITE_BOOKCASE).GetPlayerPos(player.GetX(), player.GetY());
			items.at(WHITE_BOOKCASE).OnMove();
		}
		else if (_nowID == 18) {
			if (!items.at(TUB_ONCE).IsFixed() || !items.at(TUB_ONCE).IsAnimationDone()) {
				items.at(TUB_ONCE).GetPlayerPos(player.GetX(), player.GetY());
				items.at(TUB_ONCE).OnMove();
			}
			else if (items.at(TUB_ONCE).IsFixed() && items.at(TUB_ONCE).IsAnimationDone()) {
				items.at(PHILLIPS).GetPlayerPos(player.GetX(), player.GetY());
				items.at(PHILLIPS).OnMove();
			}
			else if (items.at(PHILLIPS).IsPick()) {
				items.at(TUB_FIXED).GetPlayerPos(player.GetX(), player.GetY());
				items.at(TUB_FIXED).OnMove();
			}
		}
		else if (_nowID == 19) {
			items.at(TOILET).GetPlayerPos(player.GetX(), player.GetY());
			items.at(TOILET).OnMove();

			items.at(DETERGENT).GetPlayerPos(player.GetX(), player.GetY());
			items.at(DETERGENT).OnMove();
		}
		else if (_nowID == 20) {
			items.at(OIL).GetPlayerPos(player.GetX(), player.GetY());
			items.at(OIL).OnMove();
			
			human_mika.SetDirection(Human::up);
			human_mika.SetXYAndCol(8,16);
			human_mika.OnMove();
			house1_2F_TL_chair.GetPlayerPos(player.GetX(), player.GetY());
			house1_2F_TL_chair.OnMove(gamemaps.at(_nowID));

			gamemaps.at(_nowID).SetMapData(0, (house1_2F_TL_chair._preY - gamemaps.at(_nowID).GetY()) / TILE,
				(house1_2F_TL_chair._preX - gamemaps.at(_nowID).GetX()) / TILE, 312);
			house1_2F_TL_chair._preX = house1_2F_TL_chair.GetPosX();
			house1_2F_TL_chair._preY = house1_2F_TL_chair.GetPosY();
			if (!(house1_2F_TL_chair._preX == 12 * TILE && house1_2F_TL_chair._preY == 17 * TILE)) {
				gamemaps.at(_nowID).SetMapData(0, (house1_2F_TL_chair._preY - gamemaps.at(_nowID).GetY()) / TILE,
					(house1_2F_TL_chair._preX - gamemaps.at(_nowID).GetX()) / TILE, 0);
			}
		}
		else if (_nowID == 21) {
			items.at(BOOKCASE_MAP21).GetPlayerPos(player.GetX(), player.GetY());
			items.at(BOOKCASE_MAP21).OnMove();
		}
		else if (_nowID == 22) {
			items.at(KEY_BASEMENT).GetPlayerPos(player.GetX(), player.GetY());
			items.at(KEY_BASEMENT).OnMove();
			if (!items.at(KEY_BASEMENT).IsPick()) {
				gamemaps.at(_nowID).SetMapData(0, (items.at(KEY_BASEMENT).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(KEY_BASEMENT).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 0);
			}
			else {
				gamemaps.at(_nowID).SetMapData(0, (items.at(KEY_BASEMENT).GetPosY() - gamemaps.at(_nowID).GetY()) / TILE,
					(items.at(KEY_BASEMENT).GetPosX() - gamemaps.at(_nowID).GetX()) / TILE, 312);
			}
			items.at(GATE).GetPlayerPos(player.GetX(), player.GetY());
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
		}
		// Item end
		oni1.GetPlayerPos(player.GetX(), player.GetY());
		if (oni1.isCatch()) {
			//GotoGameState(GAME_STATE_OVER);
		}
		else {
			oni1.OnMove(gamemaps.at(_nowID));
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

		else if (_substate == OnWalking) {
			if (isdebugmode) {
				if (nChar == KEY_A) {
					//TRACE("%d %d \n", player.GetX(), player.GetY());
					isteleportblock = !isteleportblock;
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

			player.OnKeyDown(nChar);
			if (_nowID == 0) {
				items.at(KEY_JAIL).OnKeyDown(nChar);
				items.at(BOOKCASE_L).OnKeyDown(nChar);
				if (nChar != VK_SPACE) {
					items.at(BOOKCASE_R).OnKeyDown(nChar);
				}
				if (items.at(BOOKCASE_L).IsFixed()) {
					items.at(BOOKCASE_R).OnKeyDown(nChar);
				}
				if (house1_basement2_chair.isChangeMap())
					house1_basement2_chair.OnKeyDown(nChar);
			}
			else if (_nowID == 1) {
				items.at(FLATHEAD).OnKeyDown(nChar);
				house1_basement2_chair.OnKeyDown(nChar);
			}
			else if (_nowID == 2) {
				items.at(CLOSET_HIROSI_L).OnKeyDown(nChar);
			}
			else if (_nowID == 3) {
				items.at(GATE2).OnKeyDown(nChar);
			}
			else if (_nowID == 10) {
				items.at(LIGHTER).OnKeyDown(nChar);
				if (nChar != VK_SPACE) {
					items.at(TATAMI_L).OnKeyDown(nChar);
				}
				if (items.at(LIGHTER).IsPick()) {
					items.at(TATAMI_L).OnKeyDown(nChar);
				}
				items.at(TATAMI_R).OnKeyDown(nChar);

			}
			else if (_nowID == 11) {
				items.at(BROKEN_DISH).OnKeyDown(nChar);
			}
			else if (_nowID == 12) {
				items.at(LIB_BOOK).OnKeyDown(nChar);
				// 3F L
				if (nChar != VK_SPACE)
					items.at(KEY_3F_L).OnKeyDown(nChar);
				if (items.at(LIB_BOOK).IsFixed())
					items.at(KEY_3F_L).OnKeyDown(nChar);
			}
			else if (_nowID == 14) {
				house1_2F_TR_chair.OnKeyDown(nChar);
				if (nChar != VK_SPACE)
					items.at(KEY_LIB).OnKeyDown(nChar);
				if (house1_2F_TR_chair.IsFixed())
					items.at(KEY_LIB).OnKeyDown(nChar);
				items.at(HANDKERCHIEF).OnKeyDown(nChar);
				/*items.at(CLOSET_SHAKE).OnKeyDown(nChar);
				if (!items.at(CLOSET_TAKESI_0).IsFixed()) {
					items.at(CLOSET_TAKESI_0).OnKeyDown(nChar);
				}
				// CLOSET_TAKESI_1 not have on key down*/
				items.at(CLOSET_HIROSI_R).OnKeyDown(nChar);
			}
			else if (_nowID == 15) {
				if (!items.at(DOOR_KNOB).IsPick()) {
					items.at(DOOR_KNOB).OnKeyDown(nChar);
					if (nChar != VK_SPACE)
						items.at(DOOR_NO_KNOB).OnKeyDown(nChar);
				}
				if (items.at(DOOR_KNOB).IsPick())
					items.at(DOOR_NO_KNOB).OnKeyDown(nChar);
			}
			else if (_nowID == 16) {
				items.at(BED).OnKeyDown(nChar);
			}
			else if (_nowID == 17) {
				items.at(WHITE_BOOKCASE).OnKeyDown(nChar);
			}
			else if (_nowID == 18) {
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
			}
			else if (_nowID == 19) {
				items.at(TOILET).OnKeyDown(nChar);
				items.at(DETERGENT).OnKeyDown(nChar);
			}
			else if (_nowID == 20) {
				items.at(OIL).OnKeyDown(nChar);
				house1_2F_TL_chair.OnKeyDown(nChar);
			}
			else if (_nowID == 21) {
				items.at(BOOKCASE_MAP21).OnKeyDown(nChar);
			}
			else if (_nowID == 22) {
				items.at(KEY_BASEMENT).OnKeyDown(nChar);
				items.at(GATE).OnKeyDown(nChar);
			}
		}
		else if (_substate == OnDialogs) {
			if (nChar == VK_SPACE) {
				if (_dialogID == 12 && player.IsTimerStart()) {

				}
				else {
					dialogs.at(_dialogID).SetShow(false);
					_dialogID += 1;
					_dialogcount += 1;
					if (events.at(_eventID).GetDialogCount() != _dialogcount) {
						dialogs.at(_dialogID).SetShow(true);
					}
					else {
						dialogs.at(_dialogID).SetShow(false);
						if (_eventID == START_EVENT) {
							events.at(START_EVENT).IsTransMap() = true;
							player.SetNextMap(0, 3, 5);
						}
						_dialogcount = 0;
						_dialogID = -1;
						_eventID = -1;
						_substate = OnWalking;
					}
				}
			}
		}
	}


	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (_substate == OnWalking) {
			player.OnKeyUp(nChar);
			if (_nowID == 0) {
				if (house1_basement2_chair.isChangeMap())
					house1_basement2_chair.OnKeyUp(nChar);
			}
			else if (_nowID == 1) {
				house1_basement2_chair.OnKeyUp(nChar);
			}
			else if (_nowID == 14) {
				house1_2F_TR_chair.OnKeyUp(nChar);
			}
			else if (_nowID == 20) {
				house1_2F_TL_chair.OnKeyUp(nChar);
			}
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

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	
	{
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

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	
	{

	}
	void CGameStateRun::OnShow()
	{
		//TRACE("%d\n", _dialogID);
		if (!(_dialogID>=2&& _dialogID<=11)) {
			gamemaps.at(_nowID).ShowMapAll(player, oni1, mapoverlayindex.at(_nowID));
		}
		if (_nowID == 0) {
			items.at(KEY_JAIL).OnShow();
			items.at(BOOKCASE_L).OnShow();
			items.at(BOOKCASE_R).OnShow();
			if (house1_basement2_chair.isChangeMap()) {
				house1_basement2_chair.OnShow();
			}
			else {
				house1_basement2_chair.ChangeMap();
			}
		}
		else if (_nowID == 1) {
			items.at(FLATHEAD).OnShow();
			if (!house1_basement2_chair.isChangeMap()) {
				house1_basement2_chair.OnShow();
			}
			else {
				house1_basement2_chair.ChangeMap();
			}
		}
		else if (_nowID == 2) {
			if (house1_basement2_chair.isChangeMap()) {
				house1_basement2_chair.ChangeMap();
			}
			items.at(CLOSET_HIROSI_L).OnShow();
		}
		else if (_nowID == 3) {
			items.at(GATE2).OnShow();
		}
		else if (_nowID == 7) {
			house1_2F_TR_chair.ChangeMap();
			house1_2F_TL_chair.ChangeMap();
		}
		else if (_nowID == 9) {
			if (house1_basement2_chair.isChangeMap()) {
				house1_basement2_chair.ChangeMap();
			}
		}
		else if (_nowID == 10) {
			items.at(LIGHTER).OnShow();
			items.at(TATAMI_R).OnShow();
			if (items.at(LIGHTER).IsPick()) {
				items.at(TATAMI_L).OnShow();

			}
			if (items.at(LIGHTER).IsPick() && !events.at(LIGHTER_E).IsTriggered()) {
				SetEventTriggeredDialog(LIGHTER_E);
			}
		}
		else if (_nowID == 11) {
			items.at(BROKEN_DISH).OnShow();
			if (items.at(BROKEN_DISH).IsTake() && !events.at(BROKEN_DISH_E).IsTriggered()) {
				_eventID = BROKEN_DISH_E;
				events.at(_eventID).SetTriggered(true);
				_dialogID = events.at(_eventID).GetDialogIndex();
				dialogs.at(_dialogID).SetShow(true);
				_substate = OnDialogs;
			}
		}
		else if (_nowID == 12) {
			items.at(LIB_BOOK).OnShow();
			if (items.at(LIB_BOOK).IsFixed()) {
				items.at(KEY_3F_L).OnShow();
			}
			if (items.at(KEY_3F_L).IsPick()&&!events.at(KEY_3F_L_E).IsTriggered()) {
				SetEventTriggeredDialog(KEY_3F_L_E);
			}
		}
		else if (_nowID == 13) {

			if (!events.at(START_EVENT).IsTriggered()) {
				SetEventTriggeredDialog(START_EVENT);

			}
			if (!events.at(START_EVENT2).IsTriggered() && events.at(BROKEN_DISH_E).IsTriggered()) {
				SetEventTriggeredDialog(START_EVENT2);
			}
			if (_dialogID == 6) {
				player.SetDirection(Human::right);
				human_takeshi.SetDirection(Human::right);
				human_Takuruo.SetDirection(Human::right);
			}
			else if (_dialogID == 7) {
				player.SetIsMachine(true, Human::isright);
			}
			else if (_dialogID == 9) {
				player.SetAllMoveFalse();
				player.SetNowmove(Human::machinetransmap);
			}
			else if (_dialogID == 10) {
				player.SetAllMoveFalse();
				player.SetIsMachine(true, Human::isleft);
			}
			else if (_dialogID == 11) {
				player.TimerStart();
			}
			else if (_dialogID == 12) {
				player.SetAllMoveFalse();
				if (player.IsTimerStart()) {
					if (player.TimerGetCount() < 20) {
						player.SetDirection(Human::up);
					}
					else if (player.TimerGetCount() < 40) {
						player.SetDirection(Human::down);
					}
					else if (player.TimerGetCount() < 60) {
						player.SetDirection(Human::left);
						player.TimerStop();
					}
					player.TimerUpdate();
					TRACE("%d\n", player.TimerGetCount());
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
				human_Takuruo.OnShow();
			}
			if (_dialogID >= 10 && _dialogID <= 12) {
				gamemaps.at(_nowID).ShowMapAll();
				player.OnShow();
			}

		}
		else if (_nowID == 14) {
			house1_2F_TR_chair.OnShow();
			if (house1_2F_TR_chair.IsFixed()) {
				items.at(KEY_LIB).OnShow();
				if (items.at(KEY_LIB).IsPick() && !events.at(KEY_LIB_E).IsTriggered()) {
					SetEventTriggeredDialog(KEY_LIB_E);
				}
			}
			items.at(HANDKERCHIEF).OnShow();
			/*if (!items.at(CLOSET_SHAKE).IsFixed() || !items.at(CLOSET_SHAKE).IsAnimationDone()) {
				items.at(CLOSET_SHAKE).OnShow();
			}
			if (!items.at(CLOSET_TAKESI_0).IsFixed() || !items.at(CLOSET_TAKESI_0).IsAnimationDone()) {
				items.at(CLOSET_TAKESI_0).OnShow();
			}
			if (items.at(CLOSET_TAKESI_0).IsFixed() && items.at(CLOSET_TAKESI_0).IsAnimationDone()) {
				items.at(CLOSET_TAKESI_1).OnShow();
			}*/
			items.at(CLOSET_HIROSI_R).OnShow();
			TRACE("\n\nindex %d\n\n", items.at(CLOSET_HIROSI_R).GetBitMapIndex());
		}
		else if (_nowID == 15) {
			if (!items.at(DOOR_KNOB).IsPick())
				items.at(DOOR_KNOB).OnShow();
			if (items.at(DOOR_KNOB).IsPick())
				items.at(DOOR_NO_KNOB).OnShow();
		}
		else if (_nowID == 16) {
			items.at(BED).OnShow();
		}
		else if (_nowID == 17) {
			items.at(WHITE_BOOKCASE).OnShow();
		}
		else if (_nowID == 18) {
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
				
			}
		}
		else if (_nowID == 19) {
			items.at(TOILET).OnShow();
			items.at(DETERGENT).OnShow();
			if (items.at(DETERGENT).IsPick() && !events.at(DETERGENT_E).IsTriggered()) {
				SetEventTriggeredDialog(DETERGENT_E);
			}
		}
		else if (_nowID == 20) {
			items.at(OIL).OnShow();
			human_mika.OnShow();
			house1_2F_TL_chair.OnShow();
		}
		else if (_nowID == 21) {
			items.at(BOOKCASE_MAP21).OnShow();
		}
		else if (_nowID == 22) {
			items.at(KEY_BASEMENT).OnShow();
			items.at(GATE).OnShow();
		}
		for (int i = 0;i < 30;i++) {
			if (dialogs.at(i).isShow()) {
				dialogs.at(i).ShowTotal();
			}
		}
		DeBugRecursive();
	}
	
	void CGameStateRun::SetEventTriggeredDialog(int eventid)
	{
		_eventID = eventid;
		events.at(_eventID).SetTriggered(true);
		_dialogID = events.at(_eventID).GetDialogIndex();
		dialogs.at(_dialogID).SetShow(true);
		_substate = OnDialogs;
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

		inputbox.Show();
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
		CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(255, 255, 255));
		if (!isdebugmode) {
			CTextDraw::Print(pDC, 0, 0, "use KEY_Y to activate debug mode");
		}
		else {
			CTextDraw::Print(pDC, 0, 0, "map index:" + to_string(_nowID) + "  " + gamemaps.at(_nowID).GetName());
			(isedit) ? CTextDraw::Print(pDC, 0, TILE, "edit mode: true") : CTextDraw::Print(pDC, 0, TILE, "edit mode: false");

			CTextDraw::Print(pDC, 0, TILE * 2, "mouse window tile coordinate : " + to_string(mousex) + "  " + to_string(mousey));
			CTextDraw::Print(pDC, 0, TILE * 3, "mouse map tile coordinate : " + to_string(mousex - gamemaps.at(_nowID).GetX() / TILE) + "  " + to_string(mousey - gamemaps.at(_nowID).GetY() / TILE));

			CTextDraw::Print(pDC, 0, TILE * 4, "player tile coordinate on map: " + to_string((player.GetX() - gamemaps.at(_nowID).GetX()) / TILE) + " " + to_string((player.GetY() - gamemaps.at(_nowID).GetY()) / TILE));
			CTextDraw::Print(pDC, 0, TILE * 5, "player tile coordinate on window: " + to_string(player.GetX() / TILE) + " " + to_string(player.GetY() / TILE));
			CTextDraw::Print(pDC, 0, TILE * 6, "(check for out of grid) player cor point x : " + to_string((player.GetX() - gamemaps.at(_nowID).GetX()) % TILE) + " y : " + to_string((player.GetY() - gamemaps.at(_nowID).GetY()) % TILE));
			CTextDraw::Print(pDC, 0, TILE * 17, "     up            :     " + to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetX() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetU() - gamemaps.at(_nowID).GetY()) / TILE)));
			CTextDraw::Print(pDC, 0, TILE * 18, "left    right      : " +
				to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetL() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetY() - gamemaps.at(_nowID).GetY()) / TILE)) +
				"    " + to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetR() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetY() - gamemaps.at(_nowID).GetY()) / TILE)
				));
			CTextDraw::Print(pDC, 0, TILE * 19, "    down           :     " + to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetX() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetD() - gamemaps.at(_nowID).GetY()) / TILE)));
			(istwoway != 0) ? ((istwoway == 1) ? CTextDraw::Print(pDC, 0, TILE * 9, "is twoway : yes") : CTextDraw::Print(pDC, 0, TILE * 9, "is twoway : no")) : CTextDraw::Print(pDC, 0, TILE * 9, "is twoway : none");
			//(isbs != 0) ? ((isbs == 1) ? CTextDraw::Print(pDC, 0, TILE * 10, "is block sensitive : yes" ): CTextDraw::Print(pDC, 0, TILE * 10, "is block sensitive : no")): CTextDraw::Print(pDC,0, TILE * 10, "is block sensitive : none");

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