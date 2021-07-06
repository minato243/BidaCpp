#include "Resource.h"

using namespace std;
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
