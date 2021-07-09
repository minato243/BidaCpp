#include "Resource.h"

using namespace std;

std::string ingameUI(string name) {
	return "res/ui/ingame/" + name;
};

std::string lobbyUI(string name) {
	return "res/ui/lobby/" + name;
};

std::string ballnum(string name) {
	return "res/ui/ballnum/" + name;
}

std::string payment(string name) {
	return "res/ui/payment/" + name;
}

//font
const std::string Res::FONT_BITMAP_NUMBER_1 = "fonts/number_1.fnt";

const std::string Res::FONT_BITMAP_DICE_NUMBER = "fonts/diceNumber.fnt";
const std::string Res::font_facebook = "GUI/Font/facebook.TTF";
const std::string Res::font_viet = "fonts/font_viet.ttf";

//zcsd
//screen
const std::string Res::ZCSD_SCREEN_MENU = "zcsd/screen_menu.json";
const std::string Res::ZCSD_SCREEN_NETWORK = "zcsd/screen_network.json";
const std::string Res::ZCSD_SCREEN_LOCALIZATION = "zcsd/screen_localize.json";
const std::string Res::ZCSD_SCREEN_DRAGON_BONES = "zcsd/screen_dragon_bones.json";
const std::string Res::ZCSD_SCREEN_DECRYPTION = "zcsd/screen_decryption.json";
const std::string Res::ZCSD_SCREEN_ZALO = "zcsd/screen_zalo.json";
//popup
const std::string Res::ZCSD_POPUP_MINI_GAME = "zcsd/game/mini_game/PopupMiniGame.json";

//images
const std::string Res::Slot1_png = "zcsd/slot1.png";
const std::string Res::placeholder_bg = "res/ui/bg.jpg";

// 3D Models
const std::string Res::BALL_HI_POLY = "res/3d/ball/Ball_Hi.c3b";
const std::string Res::BALL_HI_POLY_2 = "res/3d/ball/Ball_Hi.c3t";
const std::string Res::BALL_3D = "res/3d/ball/Ball_Number.c3b";
const std::string Res::BALL_SHADOW = "res/3d/ball/shadow.png";
const std::string Res::TABLE_3D = "res/3d/table/Table_Bida3D.c3b";
const std::string Res::CUE_3D = "res/3d/cue/Cue.c3b";
const std::string Res::LOBBY_3D = "res/3d/lobby_new/Lobby_Room.c3b";
const std::string Res::AUDIENCE_BOARD = "res/3d/lobby_new/Audience.c3b";
const std::string Res::NPC_3D = "res/3d/npc/body.c3b";
const std::string Res::NPC_3D_TEXTURE_CLOTHES = "res/3d/npc/BidaNPC_Clothes copy.png";
const std::string Res::NPC_3D_TEXTURE_SKIN = "res/3d/npc/BidaNPC_Skin copy.png";
const std::string Res::NPC_3D_TEXTURE_HAIR = "res/3d/npc/BidaNPC_Hair copy.png";
const std::string Res::NPC_3D_OPTIMIZE = "res/3d/npc/rig_2.c3b";
const std::string Res::NPC_3D_CLOTHES = "res/3d/npc/clothes.c3b";
const std::string Res::NPC_IDLE = "res/3d/npc/main_idle.c3b";
const std::string Res::NPC_POSE_1 = "res/3d/npc/pose_1.c3b";
const std::string Res::NPC_POSE_2 = "res/3d/npc/pose_2.c3b";

const std::string Res::CUE_CHALLENGE_3D_YELLOW = "res/3d/cue_challenge/Icon_Yellow.c3b";
const std::string Res::CUE_CHALLENGE_3D_RED = "res/3d/cue_challenge/Icon_Red.c3b";
const std::string Res::CUE_CHALLENGE_3D_PINK = "res/3d/cue_challenge/Icon_Pink.c3b";
const std::string Res::CUE_CHALLENGE_3D_BLUE = "res/3d/cue_challenge/Icon_Blue.c3b";
const std::string Res::CUE_CHALLENGE_3D_TEX = "res/3d/cue_challenge/Icon.png";

const std::string Res::CUE_VANG_1 = "res/3d/cue/Cue_New/Gay_vang_1.c3b";
const std::string Res::CUE_VANG_2 = "res/3d/cue/Cue_New/Gay_vang_2.c3b";
const std::string Res::CUE_DO_1 = "res/3d/cue/Cue_New/Gay_do_1.c3b";
const std::string Res::CUE_DO_2 = "res/3d/cue/Cue_New/Gay_do_2.c3b";
const std::string Res::CUE_XANH_1 = "res/3d/cue/Cue_New/Gay_xanh_1.c3b";
const std::string Res::CUE_XANH_2 = "res/3d/cue/Cue_New/Gay_xanh_2.c3b";
const std::string Res::CUE_TIM_1 = "res/3d/cue/Cue_New/Gay_tim_1.c3b";
const std::string Res::CUE_TIM_2 = "res/3d/cue/Cue_New/Gay_tim_2.c3b";
const std::string Res::CUE_TEXTURE_SET_1 = "res/3d/cue/Cue_New/Gay_Set1.png";
const std::string Res::CUE_TEXTURE_SET_1_MRAO = "res/3d/cue/Cue_New/Gay_Set1_MRAO.png";
const std::string Res::CUE_TEXTURE_SET_2 = "res/3d/cue/Cue_New/Gay_Set2.png";
const std::string Res::CUE_TEXTURE_SET_2_MRAO = "res/3d/cue/Cue_New/Gay_Set2_MRAO.png";

const std::string Res::OLD_NPC_3D = "res/3d/npc_old/body.c3b";
const std::string Res::OLD_NPC_IDLE = "res/3d/npc_old/main_idle.c3b";
const std::string Res::OLD_NPC_POSE_1 = "res/3d/npc_old/pose_1.c3b";
const std::string Res::OLD_NPC_POSE_2 = "res/3d/npc_old/pose_2.c3b";

string Res::Ball_Textures[16] = {
		"res/3d/ball/Ball_No0.png",
		"res/3d/ball/Ball_No1.png",
		"res/3d/ball/Ball_No2.png",
		"res/3d/ball/Ball_No3.png",
		"res/3d/ball/Ball_No4.png",
		"res/3d/ball/Ball_No5.png",
		"res/3d/ball/Ball_No6.png",
		"res/3d/ball/Ball_No7.png",
		"res/3d/ball/Ball_No8.png",
		"res/3d/ball/Ball_No9.png",
		"res/3d/ball/Ball_No10.png",
		"res/3d/ball/Ball_No11.png",
		"res/3d/ball/Ball_No12.png",
		"res/3d/ball/Ball_No13.png",
		"res/3d/ball/Ball_No14.png",
		"res/3d/ball/Ball_No15.png"
};

//Lobby
const std::string Res::LOBBY_PLAY = "res/ui/lobby/PlayNow.png";
const std::string Res::LOBBY_RANK = "res/ui/lobby/PlayRank.png";
const std::string Res::LOBBY_AVATAR_FAKE = "res/ui/lobby/AvatarFake.png";
const std::string Res::LOBBY_CUE_BOX = "res/ui/lobby/CueBox.png";
const std::string Res::LOBBY_CUE_FAKE = "res/ui/lobby/CueFake.png";
const std::string Res::LOBBY_BUTTON_GROUP = "res/ui/lobby/ButtonGroup.png";
const std::string Res::LOBBY_CLAN_BUTTON = "res/ui/lobby/ClanButton.png";
const std::string Res::LOBBY_EVENT_BUTTON = "res/ui/lobby/EventButton.png";
const std::string Res::LOBBY_USER_INFO = "res/ui/lobby/InfoBorder.png";
const std::string Res::LOBBY_PLAY_DEFLECT_EFX = "res/ui/lobby/PlayDeflect.png";
const std::string Res::LOBBY_RANKING_BUTTON = "res/ui/lobby/RankingButton.png";
const std::string Res::LOBBY_SETTING_BUTTON = "res/ui/lobby/SettingButton.png";
const std::string Res::LOBBY_SHOP_BUTTON = "res/ui/lobby/ShopButton.png";
const std::string Res::LOBBY_TV = "res/ui/lobby/TV.png";
const std::string Res::LOBBY_ANNOUNCEMENT_BOX = "res/ui/lobby/AnnouncementBox.png";
const std::string Res::QUESTION_MARK = "res/ui/lobby/no_resource.png";

//INGAME

const std::string Res::INGAME_ARROW = "res/ui/ingame/Arrow.png";
const std::string Res::INGAME_ARROW_RED = "#Arrow_red.v3.png";
const std::string Res::INGAME_AVATAR_1 = "res/ui/ingame/Avatar_01.png";
const std::string Res::INGAME_AVATAR_2 = "res/ui/ingame/Avatar_02.png";
const std::string Res::INGAME_AVATAR_3 = "res/ui/ingame/Avatar_03.png";
const std::string Res::INGAME_AVATAR_FAKE_1 = "res/ui/ingame/AvatarFake5.jpg";
const std::string Res::INGAME_AVATAR_FAKE_2 = "res/ui/ingame/AvatarFake6.jpeg";
const std::string Res::INGAME_AVATAR_FAKE_5 = "res/ui/ingame/AvatarFake1.png";
const std::string Res::INGAME_AVATAR_FAKE_6 = "res/ui/ingame/AvatarFake2.png";
const std::string Res::INGAME_AVATAR_FAKE_3 = "res/ui/ingame/AvatarFake3.jpeg";
const std::string Res::INGAME_AVATAR_FAKE_4 = "res/ui/ingame/AvatarFake4.png";
const std::string Res::INGAME_BALL_BUTTON = "res/ui/ingame/Ball_Button.png";
const std::string Res::INGAME_BALL_TOUCH_INDICATOR = "res/ui/ingame/Ball_Touch_Indicator.png";
const std::string Res::INGAME_BLUE_BUTTON = "res/ui/ingame/Blue_Button_Normal.png";
const std::string Res::INGAME_PURPLE_BUTTON = "res/ui/ingame/Purple_Button_Normal.png";
const std::string Res::INGAME_CAMERA_2D_ICON = "res/ui/ingame/Camera_2D_Icon.png";
const std::string Res::INGAME_CAMERA_3D_ICON = "res/ui/ingame/Camera_3D_Icon.png";
const std::string Res::INGAME_CHAT = "res/ui/ingame/Chat_Icon.png";
const std::string Res::INGAME_ANGLE_BOX = "res/ui/ingame/Degree_Box.png";
const std::string Res::INGAME_EXTENSION = "res/ui/ingame/Extension_Button.png";
const std::string Res::INGAME_EXTENSION_GRAY = "res/ui/ingame/Extension_Button_Gray.png";
const std::string Res::INGAME_LEVEL_BOX_01 = "res/ui/ingame/Level_Box_01.png";
const std::string Res::INGAME_LEVEL_BOX_02 = "res/ui/ingame/Level_Box_02.png";
const std::string Res::INGAME_MENU_ICON = "res/ui/ingame/Menu_Icon.png";
const std::string Res::INGAME_MONEY = "res/ui/ingame/Money_Icon.png";
const std::string Res::INGAME_POWER_BAR = "res/ui/ingame/Power_Bar.png";
const std::string Res::INGAME_POWER_BAR_1 = "res/ui/ingame/Power_Bar_01.png";
const std::string Res::INGAME_POWER_BAR_2 = "res/ui/ingame/Power_Bar_02.png";
const std::string Res::INGAME_REWARD_BOX = "res/ui/ingame/Reward_Box.png";
const std::string Res::INGAME_RULER_BAR = "#Ruler_Box_01.v3.png";
const std::string Res::INGAME_RULER_BOX = "#Ruler_Box_02.v3.png";
const std::string Res::INGAME_RULER_BOX_STENCIL = "res/ui/ingame/Ruler_Box_Stencil.png";
const std::string Res::INGAME_SIDE_BAR = "#Ruler_Box_03.v3.png";
const std::string Res::INGAME_SIDE_BAR_SOLID = "#Ruler_Box_04.v3.png";
const std::string Res::INGAME_CUE = "res/ui/ingame/Stick.png";
const std::string Res::INGAME_CUE_BOX_ICON = "res/ui/ingame/Stick_Icon.png";
const std::string Res::INGAME_TIMER_1 = "res/ui/ingame/Timer_Bar_01.png";
const std::string Res::INGAME_TIMER_2 = "res/ui/ingame/Timer_Bar_02.png";
const std::string Res::INGAME_USER_INFO_BAR = "res/ui/ingame/Userinfo_Bar.png";
const std::string Res::INGAME_GOLD_COIN = "#Gold.png";
const std::string Res::INGAME_POCKET_INDI = "res/ui/ingame/pocket_indi.png";
const std::string Res::INGAME_ANGLE_ICON = "#Angle.v3.png";

const std::string Res::BALLNUM_1 = "res/ui/ballnum/Ball_01.png";
const std::string Res::BALLNUM_2 = "res/ui/ballnum/Ball_02.png";
const std::string Res::BALLNUM_3 = "res/ui/ballnum/Ball_03.png";
const std::string Res::BALLNUM_4 = "res/ui/ballnum/Ball_04.png";
const std::string Res::BALLNUM_5 = "res/ui/ballnum/Ball_05.png";
const std::string Res::BALLNUM_6 = "res/ui/ballnum/Ball_06.png";
const std::string Res::BALLNUM_7 = "res/ui/ballnum/Ball_07.png";
const std::string Res::BALLNUM_8 = "res/ui/ballnum/Ball_01.png";
const std::string Res::BALLNUM_9 = "res/ui/ballnum/Ball_09.png";
const std::string Res::BALLNUM_10 = "res/ui/ballnum/Ball_10.png";
const std::string Res::BALLNUM_11 = "res/ui/ballnum/Ball_11.png";
const std::string Res::BALLNUM_12 = "res/ui/ballnum/Ball_12.png";
const std::string Res::BALLNUM_13 = "res/ui/ballnum/Ball_13.png";
const std::string Res::BALLNUM_14 = "res/ui/ballnum/Ball_14.png";
const std::string Res::BALLNUM_15 = "res/ui/ballnum/Ball_15.png";

const std::string Res::network_operator_viettel_icon = "res/ui/payment/store_popup_tab_viettel_i5.png";
const std::string Res::network_operator_mobi_icon = "res/ui/payment/store_popup_tab_mobi_i5.png";
const std::string Res::network_operator_vina_icon = "res/ui/payment/store_popup_tab_vina_i5.png";

