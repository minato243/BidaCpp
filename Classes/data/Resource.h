#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <cocos2d.h>

class Res {
public:
    //font
	static const std::string     FONT_BITMAP_NUMBER_1;
	static const std::string         FONT_BITMAP_DICE_NUMBER;
	static const std::string         font_facebook;
	static const std::string         font_viet;
        //zcsd
        //screen
	static const std::string     ZCSD_SCREEN_MENU;
	static const std::string         ZCSD_SCREEN_NETWORK;
	static const std::string         ZCSD_SCREEN_LOCALIZATION;
	static const std::string         ZCSD_SCREEN_DRAGON_BONES;
	static const std::string         ZCSD_SCREEN_DECRYPTION;
	static const std::string         ZCSD_SCREEN_ZALO;
        //popup
	static const std::string         ZCSD_POPUP_MINI_GAME;
        //images
	static const std::string         Slot1_png;
	static const std::string         placeholder_bg;
        // 3D Models
	static const std::string         BALL_HI_POLY;
	static const std::string         BALL_HI_POLY_2;
	static const std::string         BALL_3D;
	static const std::string         BALL_SHADOW;
	static const std::string         TABLE_3D;
	static const std::string         CUE_3D;
	static const std::string         LOBBY_3D;
	static const std::string         AUDIENCE_BOARD;
	static const std::string         NPC_3D;
	static const std::string         NPC_3D_TEXTURE_CLOTHES;
	static const std::string         NPC_3D_TEXTURE_SKIN;
	static const std::string         NPC_3D_TEXTURE_HAIR;
	static const std::string         NPC_3D_OPTIMIZE;
	static const std::string         NPC_3D_CLOTHES;
	static const std::string         NPC_IDLE;
	static const std::string         NPC_POSE_1;
	static const std::string         NPC_POSE_2;
	static const std::string         CUE_CHALLENGE_3D_YELLOW;
	static const std::string         CUE_CHALLENGE_3D_RED;
	static const std::string         CUE_CHALLENGE_3D_PINK;
	static const std::string         CUE_CHALLENGE_3D_BLUE;
	static const std::string         CUE_CHALLENGE_3D_TEX;
	static const std::string         CUE_VANG_1;
	static const std::string         CUE_VANG_2;
	static const std::string         CUE_DO_1;
	static const std::string         CUE_DO_2;
	static const std::string         CUE_XANH_1;
	static const std::string         CUE_XANH_2;
	static const std::string         CUE_TIM_1;
	static const std::string         CUE_TIM_2;
	static const std::string         CUE_TEXTURE_SET_1;
	static const std::string         CUE_TEXTURE_SET_1_MRAO;
	static const std::string         CUE_TEXTURE_SET_2;
	static const std::string         CUE_TEXTURE_SET_2_MRAO;
	static const std::string         OLD_NPC_3D;
	static const std::string         OLD_NPC_IDLE;
	static const std::string         OLD_NPC_POSE_1;
	static const std::string         OLD_NPC_POSE_2;
	static std::string Ball_Textures[16];

	//Lobby;
	static const std::string 	LOBBY_PLAY;
	static const std::string 	LOBBY_RANK;
	static const std::string 	LOBBY_AVATAR_FAKE;
	static const std::string 	LOBBY_CUE_BOX;
	static const std::string 	LOBBY_CUE_FAKE;
	static const std::string 	LOBBY_BUTTON_GROUP;
	static const std::string 	LOBBY_CLAN_BUTTON;
	static const std::string 	LOBBY_EVENT_BUTTON;
	static const std::string 	LOBBY_USER_INFO;
	static const std::string 	LOBBY_PLAY_DEFLECT_EFX;
	static const std::string 	LOBBY_RANKING_BUTTON;
	static const std::string 	LOBBY_SETTING_BUTTON;
	static const std::string 	LOBBY_SHOP_BUTTON;
	static const std::string 	LOBBY_TV;
	static const std::string 	LOBBY_ANNOUNCEMENT_BOX;
	static const std::string 	QUESTION_MARK;

	//INGAME;

	static const std::string 	INGAME_ARROW;
	static const std::string 	INGAME_ARROW_RED;
	static const std::string 	INGAME_AVATAR_1;
	static const std::string 	INGAME_AVATAR_2;
	static const std::string 	INGAME_AVATAR_3;
	static const std::string 	INGAME_AVATAR_FAKE_1;
	static const std::string 	INGAME_AVATAR_FAKE_2;
	static const std::string 	INGAME_AVATAR_FAKE_5;
	static const std::string 	INGAME_AVATAR_FAKE_6;
	static const std::string 	INGAME_AVATAR_FAKE_3;
	static const std::string 	INGAME_AVATAR_FAKE_4;
	static const std::string 	INGAME_BALL_BUTTON;
	static const std::string 	INGAME_BALL_TOUCH_INDICATOR;
	static const std::string 	INGAME_BLUE_BUTTON;
	static const std::string 	INGAME_PURPLE_BUTTON;
	static const std::string 	INGAME_CAMERA_2D_ICON;
	static const std::string 	INGAME_CAMERA_3D_ICON;
	static const std::string 	INGAME_CHAT;
	static const std::string 	INGAME_ANGLE_BOX;
	static const std::string 	INGAME_EXTENSION;
	static const std::string 	INGAME_EXTENSION_GRAY;
	static const std::string 	INGAME_LEVEL_BOX_01;
	static const std::string 	INGAME_LEVEL_BOX_02;
	static const std::string 	INGAME_MENU_ICON;
	static const std::string 	INGAME_MONEY;
	static const std::string 	INGAME_POWER_BAR;
	static const std::string 	INGAME_POWER_BAR_1;
	static const std::string 	INGAME_POWER_BAR_2;
	static const std::string 	INGAME_REWARD_BOX;
	static const std::string 	INGAME_RULER_BAR;
	static const std::string 	INGAME_RULER_BOX;
	static const std::string 	INGAME_RULER_BOX_STENCIL;
	static const std::string 	INGAME_SIDE_BAR;
	static const std::string 	INGAME_SIDE_BAR_SOLID;
	static const std::string 	INGAME_CUE;
	static const std::string 	INGAME_CUE_BOX_ICON;
	static const std::string 	INGAME_TIMER_1;
	static const std::string 	INGAME_TIMER_2;
	static const std::string 	INGAME_USER_INFO_BAR;
	static const std::string 	INGAME_GOLD_COIN;
	static const std::string 	INGAME_POCKET_INDI;
	static const std::string 	INGAME_ANGLE_ICON;

	static const std::string 	BALLNUM_1;
	static const std::string 	BALLNUM_2;
	static const std::string 	BALLNUM_3;
	static const std::string 	BALLNUM_4;
	static const std::string 	BALLNUM_5;
	static const std::string 	BALLNUM_6;
	static const std::string 	BALLNUM_7;
	static const std::string 	BALLNUM_8;
	static const std::string 	BALLNUM_9;
	static const std::string 	BALLNUM_10;
	static const std::string 	BALLNUM_11;
	static const std::string 	BALLNUM_12;
	static const std::string 	BALLNUM_13;
	static const std::string 	BALLNUM_14;
	static const std::string 	BALLNUM_15;

	static const std::string 	network_operator_viettel_icon;
	static const std::string 	network_operator_mobi_icon;
	static const std::string 	network_operator_vina_icon;
};

#endif // !RESOURCE_H_
