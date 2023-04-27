#define GAME_TITLE           "Game"
#define SIZE_X				 816	
#define SIZE_Y				 820
#define OPEN_AS_FULLSCREEN	 false		
#define SHOW_LOAD_PROGRESS   true		
#define DEFAULT_BG_COLOR	 RGB(0, 0, 0)	
#define GAME_CYCLE_TIME		 15		    
#define SHOW_GAME_CYCLE_TIME false		
#define ENABLE_GAME_PAUSE	 false		
#define ENABLE_AUDIO		 true		
#define ENABLE_TOOLBAR       false     
#define ENABLE_MENU          false      
#define ENABLE_STATUSBAR     false      
#define RESOLUTION_X     1920           
#define RESOLUTION_Y     1080   

#define TILE 32

#define KEY_0 0x30
#define KEY_1 0x31
#define KEY_2 0x32
#define KEY_3 0x33
#define KEY_4 0x34
#define KEY_5 0x35
#define KEY_6 0x36
#define KEY_7 0x37
#define KEY_8 0x38
#define KEY_9 0x39
#define KEY_A 0x41
#define KEY_B 0x42
#define KEY_C 0x43
#define KEY_D 0x44
#define KEY_E 0x45
#define KEY_F 0x46
#define KEY_G 0x47
#define KEY_H 0x48
#define KEY_I 0x49
#define KEY_J 0x4a
#define KEY_K 0x4b
#define KEY_L 0x4c
#define KEY_M 0x4d
#define KEY_N 0x4e
#define KEY_O 0x4f
#define KEY_P 0x50
#define KEY_Q 0x51
#define KEY_R 0x52
#define KEY_S 0x53
#define KEY_T 0x54
#define KEY_U 0x55
#define KEY_V 0x56
#define KEY_W 0x57
#define KEY_X 0x58
#define KEY_Y 0x59
#define KEY_Z 0x5a
#define VK_ENTER VK_RETURN

/*
debug mode
press
e : enable map path edit
i : enable show map tile index
9 amd 0 : navigate through indexes of each map layers
j and k : navigate through maps
w : enable input box
space :  when input box is enable, escape input box.
		otherwise, if input is in the range of map index, teleport to to corresponding map

*/
// event id
#define BROKEN_DISH_E 0
#define START_EVENT_E 1
#define START_EVENT2_E 2
#define KEY_LIB_E 3
#define DETERGENT_E 4
#define PHILLIPS_E 5
#define KEY_3F_L_E 6
#define LIGHTER_E 7
#define DUMB_TAKESHI_E 8
#define HANDKERCHIEF_E 9
#define MIKA_SCARE_E 10
#define MIKA_OK_E 11
#define MIKA_NOTOK_E 12
#define MIKA_REPEAT_E 13