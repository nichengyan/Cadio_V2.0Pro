// #include "Draw.h"
// #include "Defines.h"
// #include "Game.h"
// #include "Particle.h"
// #include "FixedMath.h"
// #include "Map.h"

// #include "LUT.h"
#define Main_Fosc 40000000 // 主频：40M 用于计时
#include "_STC8X_.h"
#include "TYPE.h"
#include "Graphic.h"
#include "Key.h"
#include <string.h>
#define WITH_TEXTURES 1
#define PROGMEM
#define PSTR
#define pgm_read_byte(x) (*((uint8_t *)x))
#define pgm_read_word(x) (*((uint16_t *)x))
#define pgm_read_ptr(x) (*((uintptr_t *)x))
#define strlen_P(x) strlen(x)

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define TARGET_FRAMERATE (unsigned char)30

#define INPUT_LEFT (unsigned char)(1 << 0)
#define INPUT_RIGHT (unsigned char)(1 << 1)
#define INPUT_UP (unsigned char)(1 << 2)
#define INPUT_DOWN (unsigned char)(1 << 3)
#define INPUT_A  (unsigned char)(1 << 4）
#define INPUT_B  (unsigned char)(1 << 5）

#define COLOUR_WHITE (unsigned char)1
#define COLOUR_BLACK (unsigned char)0

// constexpr unsigned int FIXED_ANGLE_MAX { 256 };

#define CAMERA_SCALE (int)(1)
#define CLIP_PLANE (int)32
#define CLIP_ANGLE (int)32
#define NEAR_PLANE_MULTIPLIER (int)130
#define NEAR_PLANE (int)(DISPLAY_WIDTH * NEAR_PLANE_MULTIPLIER / 256)
#define HORIZON (int)(DISPLAY_HEIGHT / 2)
#define MAP_SIZE (int)(8)
#define CELL_SIZE (int)256
#define PARTICLES_PER_SYSTEM (int)8
#define BASE_SPRITE_SIZE (int)16
#define MAX_SPRITE_SIZE (int)(DISPLAY_HEIGHT / 2)
#define MIN_TEXTURE_DISTANCE (int)4

#define TURN_SPEED (int)(5)
// texture.h
const uint8_t vectorTexture0[] =
	{
		5,
		0,
		43,
		128,
		43,
		0,
		85,
		128,
		85,
		43,
		0,
		43,
		43,
		85,
		43,
		85,
		85,
		64,
		85,
		64,
		128,
};

const uint8_t vectorTexture1[] =
	{
		6,
		0, 16, 128, 16,
		0, 112, 128, 112,
		0, 16, 0, 112,
		0, 16, 128, 112,
		0, 112, 128, 16,
		128, 16, 128, 112,

		/*	16, 16, 112, 16 ,
		16, 16, 16, 128,
		48, 16, 48, 128,
		80, 16, 80, 128,
		112, 16, 112, 128,*/
};

const uint8_t vectorTexture2[] =
	{
		12,
		38,
		13,
		90,
		13,
		38,
		13,
		64,
		38,
		64,
		38,
		90,
		13,
		13,
		38,
		38,
		64,
		13,
		38,
		13,
		90,
		13,
		90,
		38,
		64,
		38,
		115,
		90,
		115,
		38,
		115,
		64,
		90,
		64,
		90,
		90,
		115,
		90,
		64,
		115,
		38,
		90,
		64,
		115,
		90,
		115,
		38,
		115,
		90,
};

const uint8_t *const textures[] =
	{
		vectorTexture0,
		vectorTexture1,
		vectorTexture2,
};
/*
enum class CellType : uint8_t
{
	Empty = 0,

	// Monster types
	Monster,

	// Non collidable decorations
	Torch,
	Entrance,
	Exit,

	// Items
	Potion,
	Coins,
	Crown,
	Scroll,

	// Collidable decorations
	Urn,
	Chest,
	ChestOpened,
	Sign,

	// Solid cells
	BrickWall,

	FirstCollidableCell = Urn,
	FirstSolidCell = BrickWall
};
*/
// SpriteData.inc.h
//  Generated from Images/enemy.png
const uint8_t skeletonSpriteData_numFrames = 2;

const uint16_t skeletonSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0x3f00, 0x3f00, 0x7f00, 0x4100, 0x7f00, 0x5d00, 0x7f00, 0x5d00, 0x7ffc, 0x417c, 0x7ffe, 0x3f6e, 0x3ffe, 0xe, 0x1ffe, 0x1b0e, 0xfffe, 0xfb6e, 0xfffc, 0xfb7c, 0x1f80, 0x1100, 0x1f80, 0x400, 0x1ffc, 0x15fc, 0xe00, 0x400, 0xe00, 0x0, 0x0, 0x0, 0x3f00, 0x3f00, 0x7f00, 0x4100, 0x7f00, 0x5d00, 0x7f00, 0x5d00, 0xfff8, 0xc1f8, 0xfffc, 0xbfdc, 0x3ffc, 0x1c, 0x3ffc, 0x361c, 0x7ffc, 0x76dc, 0x7ff8, 0x76f8, 0x1f00, 0x200, 0x1f00, 0x800, 0x3ff8, 0x2bf8, 0x1c00, 0x800, 0x1c00, 0x0};
// Generated from Images/mage.png
const uint8_t mageSpriteData_numFrames = 2;
const uint16_t mageSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0xe00, 0xc00, 0xf00, 0x900, 0x780, 0x380, 0x7f8, 0x3f8, 0xfffc, 0x7704, 0x3ffe, 0x2656, 0x3ffe, 0x36f6, 0x3ffe, 0x36f6, 0x3ffe, 0x3656, 0x7ffc, 0x3704, 0x3f8, 0x3f8, 0x180, 0x180, 0x1e1c, 0xc1c, 0x7ff6, 0x6df6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc00, 0xc00, 0xf80, 0xb80, 0x7f0, 0x3f0, 0x7ff8, 0x3608, 0x3ffc, 0x24ac, 0x3ffc, 0x35ec, 0x3ffc, 0x35ec, 0x3ffc, 0x34ac, 0xfff8, 0x7608, 0x3f0, 0x3f0, 0x300, 0x300, 0x3c38, 0x1838, 0xffec, 0xdbec, 0x0, 0x0};
// Generated from Images/torchalt1.png
const uint8_t torchSpriteData1_numFrames = 1;
const uint16_t torchSpriteData1[] PROGMEM =
	{
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0xb0, 0x7ff8, 0x5e98, 0x7ffd, 0x5e9d, 0xf0, 0xb0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
// Generated from Images/torchalt2.png
const uint8_t torchSpriteData2_numFrames = 1;
const uint16_t torchSpriteData2[] PROGMEM =
	{
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0xb0, 0x7ffc, 0x5e9c, 0x7ffa, 0x5e9a, 0xf0, 0xb0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
// Generated from Images/fireball2.png
const uint8_t projectileSpriteData_numFrames = 1;
const uint16_t projectileSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0x0, 0x0, 0xf0, 0xf0, 0x1f8, 0x1f8, 0x3fc, 0x3fc, 0x7fc, 0x7dc, 0x7fc, 0x73c, 0x17fc, 0x163c, 0x7f8, 0x478, 0x2ff0, 0x2cf0, 0xfe0, 0xfe0, 0x1e00, 0x1e00, 0x1880, 0x1880, 0x2200, 0x2200, 0x0, 0x0, 0x0, 0x0};
// Generated from Images/fireball.png
const uint8_t enemyProjectileSpriteData_numFrames = 1;
const uint16_t enemyProjectileSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0x7c0, 0x0, 0xff0, 0x0, 0x1ff8, 0x0, 0x3ffc, 0x200, 0x3ffe, 0x100, 0x7ffe, 0x190, 0x7ffe, 0x3e0, 0x7ffe, 0x7c0, 0x7ffe, 0x980, 0x7ffc, 0x80, 0x3ffc, 0x40, 0x1ff8, 0x0, 0xff0, 0x0, 0x3e0, 0x0, 0x0, 0x0};
// Generated from Images/entrance.png
const uint8_t entranceSpriteData_numFrames = 1;
const uint16_t entranceSpriteData[] PROGMEM =
	{
		0x4, 0x0, 0xe, 0x0, 0xe, 0x0, 0x1f, 0x0, 0x3ff, 0x3fc, 0x9f, 0x90, 0x9f, 0x90, 0x9f, 0x90, 0x9f, 0x90, 0x9f, 0x90, 0x9f, 0x90, 0x3ff, 0x3fc, 0x1f, 0x0, 0xe, 0x0, 0xe, 0x0, 0x4, 0x0};
// Generated from Images/exit.png
const uint8_t exitSpriteData_numFrames = 1;
const uint16_t exitSpriteData[] PROGMEM =
	{
		0x2000, 0x0, 0x7000, 0x0, 0x7000, 0x0, 0xf800, 0x0, 0xffc0, 0x3fc0, 0xf900, 0x900, 0xf900, 0x900, 0xf900, 0x900, 0xf900, 0x900, 0xf900, 0x900, 0xf900, 0x900, 0xffc0, 0x3fc0, 0xf800, 0x0, 0x7000, 0x0, 0x7000, 0x0, 0x2000, 0x0};
// Generated from Images/urn.png
const uint8_t urnSpriteData_numFrames = 1;
const uint16_t urnSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0x60, 0x60, 0xfe0, 0xde0, 0x3ff0, 0x3df0, 0x7ff1, 0x7df1, 0xfffb, 0xfdfb, 0xffff, 0xfdfe, 0xffff, 0xfdfe, 0xffff, 0xfdfe, 0xffff, 0xfdfe, 0xfffb, 0xfdfb, 0x7ff1, 0x7df1, 0x3ff0, 0x3df0, 0xfe0, 0xde0, 0x60, 0x60, 0x0, 0x0};
// Generated from Images/sign.png
const uint8_t signSpriteData_numFrames = 1;
const uint16_t signSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7f0, 0x7f0, 0x47f0, 0x47f0, 0x87f0, 0x86b0, 0x7f0, 0x6b0, 0xfffc, 0xf6b4, 0xfffc, 0xf6b4, 0x7f0, 0x6b0, 0x87f0, 0x86b0, 0x47f0, 0x47f0, 0x7f0, 0x7f0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
// Generated from Images/crown.png
const uint8_t crownSpriteData_numFrames = 1;
const uint16_t crownSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0x0, 0x0, 0x3fc0, 0x2fc0, 0x7e80, 0x5e80, 0x7c00, 0x5c00, 0xfc00, 0xbc00, 0xff20, 0xbf20, 0xfff0, 0xb9f0, 0xfff0, 0xb9f0, 0xff20, 0xbf20, 0xfc00, 0xbc00, 0x7c00, 0x5c00, 0x7e80, 0x5e80, 0x3fc0, 0x2fc0, 0x0, 0x0, 0x0, 0x0};
// Generated from Images/coins.png
const uint8_t coinsSpriteData_numFrames = 1;
const uint16_t coinsSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0x0, 0x0, 0x6000, 0x6000, 0xf000, 0xd000, 0xf000, 0xd000, 0xff80, 0xdf80, 0x7fc0, 0x7540, 0x7fc0, 0x7540, 0x7fc0, 0x7540, 0x7f80, 0x7f80, 0xfc00, 0xd400, 0xfc00, 0xd400, 0xfc00, 0xd400, 0x7800, 0x7800, 0x0, 0x0, 0x0, 0x0};
// Generated from Images/scroll.png
const uint8_t scrollSpriteData_numFrames = 1;
const uint16_t scrollSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0xc, 0xc, 0x9c3c, 0x9c30, 0xfffc, 0x6f84, 0xfffc, 0x8dfc, 0xfffc, 0xef5c, 0xfffc, 0xed5c, 0xfffc, 0xed7c, 0xfffc, 0xed5c, 0xfffc, 0xed5c, 0xfffc, 0xed5c, 0xfffc, 0xeddc, 0xfffc, 0xef5c, 0xe3fc, 0xe3fc, 0x6078, 0x6078, 0x0, 0x0};
// Generated from Images/chest.png
const uint8_t chestSpriteData_numFrames = 1;
const uint16_t chestSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0x7ff0, 0x7ff0, 0xfff8, 0xc218, 0xfff8, 0xbae8, 0xfff8, 0xbaf8, 0xfff8, 0xbaf8, 0xfff8, 0xa0f8, 0xfff8, 0xacf8, 0xfff8, 0xacf8, 0xfff8, 0xa0f8, 0xfff8, 0xbaf8, 0xfff8, 0xbaf8, 0xfff8, 0xbae8, 0xfff8, 0xc218, 0x7ff0, 0x7ff0, 0x0, 0x0};
// Generated from Images/chestopen.png
const uint8_t chestOpenSpriteData_numFrames = 1;
const uint16_t chestOpenSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0x7ff8, 0x7ff8, 0xfffc, 0xc24c, 0xfffc, 0xba54, 0xfffc, 0xba5c, 0xfffc, 0xba5c, 0xfffc, 0xa05c, 0xfffc, 0xac5c, 0xfffc, 0xac5c, 0xfffc, 0xa05c, 0xfffc, 0xba5c, 0xfffc, 0xba5c, 0xfffc, 0xba54, 0xfffc, 0xc24c, 0x7ff8, 0x7ff8, 0x0, 0x0};
// Generated from Images/potion.png
const uint8_t potionSpriteData_numFrames = 1;
const uint16_t potionSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3800, 0x3800, 0x7c00, 0x4400, 0xfe38, 0xba38, 0xfff8, 0xbde8, 0xfffc, 0xbc0c, 0xfffc, 0xbc0c, 0xfff8, 0xbde8, 0xfe38, 0xba38, 0x7c00, 0x4400, 0x3800, 0x3800, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
// Generated from Images/bat.png
const uint8_t batSpriteData_numFrames = 2;
const uint16_t batSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0x780, 0x0, 0x3c0, 0x0, 0x3e0, 0x0, 0x7c0, 0x0, 0x1c0, 0x0, 0xff0, 0x80, 0x7e0, 0x0, 0x7e0, 0x0, 0xff0, 0x80, 0x1c0, 0x0, 0x7c0, 0x0, 0x3e0, 0x0, 0x3c0, 0x0, 0x780, 0x0, 0x0, 0x0, 0x0, 0x0, 0x140, 0x0, 0x3c0, 0x0, 0x7c0, 0x0, 0x780, 0x0, 0x380, 0x0, 0x1fe0, 0x100, 0xfc0, 0x0, 0xfc0, 0x0, 0x1fe0, 0x100, 0x380, 0x0, 0x780, 0x0, 0x7c0, 0x0, 0x3c0, 0x0, 0x140, 0x0, 0x0, 0x0};
// Generated from Images/spider.png
const uint8_t spiderSpriteData_numFrames = 2;
const uint16_t spiderSpriteData[] PROGMEM =
	{
		0x0, 0x0, 0xfdc, 0xfdc, 0x78, 0x48, 0x7ff0, 0x7f00, 0x81f8, 0x8078, 0x1ffc, 0x1efc, 0x27fc, 0x277c, 0xffc, 0xf54, 0xffc, 0xf6c, 0x27fc, 0x277c, 0x1ffc, 0x1efc, 0x81f8, 0x8078, 0x7ff0, 0x7f00, 0x78, 0x48, 0xfdc, 0xfdc, 0x0, 0x0, 0x0, 0x0, 0x7b0, 0x7b0, 0x8f0, 0x890, 0x3fe0, 0x3e00, 0x43f0, 0x40f0, 0x3ff8, 0x3df8, 0x4ff8, 0x4ef8, 0x1ff8, 0x1ea8, 0x1ff8, 0x1ed8, 0x4ff8, 0x4ef8, 0x3ff8, 0x3df8, 0x43f0, 0x40f0, 0x3fe0, 0x3e00, 0x8f0, 0x890, 0x7b0, 0x7b0, 0x0, 0x0};
// Generated from Images/hand1.png
const uint8_t handSpriteData1[] PROGMEM =
	{
		28, 28,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6, 0x6, 0xf, 0xe, 0x1f, 0x1c, 0x7e, 0x78, 0xfc, 0xf4, 0xfe, 0xe8, 0xfc, 0x90, 0xf8, 0x60, 0xf0, 0x80, 0xe0, 0x0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc, 0xc, 0x1e, 0x1c, 0x3e, 0x38, 0x7c, 0x70, 0xf8, 0xe0, 0xf0, 0xc0, 0xe0, 0x80, 0xc0, 0x80, 0xc0, 0x80, 0xc7, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xfc, 0xfe, 0x0, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x3, 0xf, 0xf, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xf0, 0xfe, 0x60, 0xf0, 0x40, 0xe0, 0x0, 0xc0, 0x0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0xf, 0x1, 0xf, 0x1, 0xf, 0x0, 0xf, 0x0, 0xf, 0x0, 0xf, 0x0, 0xf, 0x0, 0xf, 0x0, 0xf, 0x0, 0xf, 0x0, 0xf, 0x0, 0xf, 0x0, 0xf, 0x0, 0xf, 0x0, 0xe, 0x0, 0xc};
// Generated from Images/hand2.png
const uint8_t handSpriteData2[] PROGMEM =
	{
		28, 27,
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x38, 0x38, 0xfc, 0xf8, 0xfc, 0xe0, 0xf8, 0x80, 0xe0, 0x0, 0x80, 0x0, 0xff, 0xfe, 0xff, 0xfc, 0xfe, 0xc0, 0xfc, 0x0, 0xf0, 0xf0, 0xf8, 0xf0, 0xf8, 0x0, 0xf0, 0xc0, 0xe0, 0x0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x3, 0xf, 0xf, 0xff, 0xfe, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0x1f, 0xff, 0x0, 0x1f, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x8, 0x1c, 0x18, 0x3c, 0x18, 0x3c, 0x18, 0x3c, 0x18, 0x3c, 0x18, 0x3c, 0x1c, 0x3e, 0x3e, 0x7f, 0x3f, 0x7f, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0x3f, 0xff, 0x1f, 0xff, 0x1f, 0xff, 0xf, 0xff, 0xe, 0xff, 0x0, 0xfe, 0x0, 0xf0, 0x0, 0xf0, 0x0, 0xe0, 0x0, 0xe0, 0x0, 0xc0, 0x0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x1, 0x0, 0x7, 0x0, 0x7, 0x0, 0x7, 0x0, 0x7, 0x0, 0x7, 0x0, 0x7, 0x0, 0x7, 0x0, 0x7, 0x0, 0x7, 0x0, 0x7, 0x0, 0x7, 0x0, 0x7, 0x0, 0x7};
// Generated from Images/textures.png
const uint8_t wallTextureData_numTextures = 1;
const uint16_t wallTextureData[] PROGMEM =
	{
		0xf3cf, 0xf3cf, 0xf3cf, 0xf3c0, 0xf3c0, 0xf3cf, 0x3cf, 0x3cf, 0xf3cf, 0xf3cf, 0xf00f, 0xf00f, 0xf3cf, 0xf3cf, 0xf3cf, 0xf3cf};
// Generated from Images/font.png
const uint8_t fontPageData[] PROGMEM =
	{
		0xff, 0xff, 0xff, 0xff, 0xff, 0xd1, 0xff, 0xff, 0xf9, 0xff, 0xf9, 0xff, 0xc1, 0xeb, 0xc1, 0xff, 0xd3, 0xc1, 0xe5, 0xff, 0xcd, 0xf7, 0xd9, 0xff, 0xcb, 0xd5, 0xeb, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xe3, 0xdd, 0xff, 0xff, 0xdd, 0xe3, 0xff, 0xeb, 0xf7, 0xeb, 0xff, 0xf7, 0xe3, 0xf7, 0xff, 0xdf, 0xef, 0xff, 0xff, 0xf7, 0xf7, 0xf7, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xcf, 0xf7, 0xf9, 0xff, 0xc3, 0xdd, 0xe1, 0xff, 0xdb, 0xc1, 0xdf, 0xff, 0xcd, 0xd5, 0xd3, 0xff, 0xdd, 0xd5, 0xeb, 0xff, 0xe1, 0xef, 0xc7, 0xff, 0xd1, 0xd5, 0xe5, 0xff, 0xc3, 0xd5, 0xe5, 0xff, 0xcd, 0xf5, 0xf9, 0xff, 0xc3, 0xd5, 0xe1, 0xff, 0xd3, 0xd5, 0xe1, 0xff, 0xff, 0xeb, 0xff, 0xff, 0xdf, 0xeb, 0xff, 0xff, 0xf7, 0xeb, 0xdd, 0xff, 0xeb, 0xeb, 0xeb, 0xff, 0xdd, 0xeb, 0xf7, 0xff, 0xfd, 0xd5, 0xf9, 0xff, 0xe3, 0xdd, 0xd3, 0xff, 0xc3, 0xf5, 0xc1, 0xff, 0xc3, 0xd5, 0xe9, 0xff, 0xe3, 0xdd, 0xdd, 0xff, 0xc1, 0xdd, 0xe3, 0xff, 0xc3, 0xd5, 0xd5, 0xff, 0xc3, 0xf5, 0xf5, 0xff, 0xe3, 0xdd, 0xc5, 0xff, 0xc1, 0xf7, 0xc1, 0xff, 0xdd, 0xc1, 0xdd, 0xff, 0xef, 0xdd, 0xe1, 0xff, 0xc1, 0xf7, 0xc9, 0xff, 0xc1, 0xdf, 0xdf, 0xff, 0xc1, 0xfb, 0xc1, 0xff, 0xc1, 0xfd, 0xc3, 0xff, 0xe3, 0xdd, 0xe3, 0xff, 0xc1, 0xf5, 0xf3, 0xff, 0xe3, 0xcd, 0xc1, 0xff, 0xc3, 0xf5, 0xc9, 0xff, 0xdb, 0xd5, 0xed, 0xff, 0xfd, 0xc1, 0xfd, 0xff, 0xe1, 0xdf, 0xc1, 0xff, 0xe1, 0xdf, 0xe1, 0xff, 0xc1, 0xef, 0xc1, 0xff, 0xc9, 0xf7, 0xc9, 0xff, 0xf9, 0xc7, 0xf9, 0xff, 0xcd, 0xd5, 0xd9, 0xff, 0xff, 0xc1, 0xdd, 0xff, 0xf9, 0xf7, 0xcf, 0xff, 0xff, 0xdd, 0xc1, 0xff, 0xfb, 0xfd, 0xfb, 0xff, 0xdf, 0xdf, 0xdf, 0xff, 0xff, 0xfd, 0xfb, 0xff, 0xe7, 0xdb, 0xc3, 0xff, 0xc1, 0xdb, 0xe7, 0xff, 0xe7, 0xdb, 0xdb, 0xff, 0xe7, 0xdb, 0xc1, 0xff, 0xe7, 0xcb, 0xd3, 0xff, 0xc3, 0xed, 0xfb, 0xff, 0xb7, 0xab, 0xc7, 0xff, 0xc1, 0xf7, 0xcf, 0xff, 0xff, 0xe5, 0xdf, 0xff, 0xbf, 0xcb, 0xff, 0xff, 0xc1, 0xf7, 0xcb, 0xff, 0xff, 0xe1, 0xdf, 0xff, 0xc3, 0xf7, 0xc3, 0xff, 0xc3, 0xfb, 0xc7, 0xff, 0xe7, 0xdb, 0xe7, 0xff, 0x83, 0xdb, 0xe7, 0xff, 0xe7, 0xdb, 0x83, 0xff, 0xc3, 0xf7, 0xfb, 0xff, 0xd7, 0xd3, 0xeb, 0xff, 0xe1, 0xdb, 0xdf, 0xff, 0xe3, 0xdf, 0xc3, 0xff, 0xc3, 0xdf, 0xe3, 0xff, 0xc3, 0xef, 0xc3, 0xff, 0xcb, 0xf7, 0xcb, 0xff, 0xf3, 0xaf, 0xc3, 0xff, 0xdb, 0xcb, 0xd3, 0xff, 0xf7, 0xc1, 0xdd, 0xff, 0xff, 0xc1, 0xff, 0xff, 0xdd, 0xc1, 0xf7, 0xff, 0xfb, 0xfd, 0xfb, 0xff, 0xe3, 0xed, 0xe3, 0xff};
// Generated from Images/heart.png
const uint8_t heartSpriteData[] PROGMEM =
	{
		0x7c, 0xe6, 0xc6, 0x8e, 0xc6, 0xe6, 0x7c, 0x0};
// Generated from Images/mana.png
const uint8_t manaSpriteData[] PROGMEM =
	{
		0x7c, 0xce, 0xb6, 0x9a, 0xb6, 0xce, 0x7c, 0x0};

// Draw.h
typedef struct
{
	int16_t x, y;
	uint8_t angle;
	int16_t rotCos, rotSin;
	int16_t clipCos, clipSin;
	uint8_t cellX, cellY;
	int8_t tilt, bob;
} Camera;
// map.h
const uint8_t Map[] =
	{
		1,
		1,
		1,
		2,
		1,
		1,
		1,
		1,
		1,
		0,
		0,
		0,
		0,
		0,
		0,
		1,
		1,
		0,
		0,
		0,
		0,
		2,
		0,
		1,
		2,
		0,
		0,
		1,
		3,
		1,
		0,
		1,
		1,
		1,
		0,
		0,
		0,
		1,
		0,
		1,
		1,
		0,
		0,
		1,
		0,
		1,
		0,
		2,
		1,
		1,
		0,
		1,
		0,
		0,
		0,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
		1,
};

// const uint8_t map[] PROGMEM =
//{
//	1, 1, 1, 1, 1, 1, 1, 1,
//	1, 0, 0, 0, 0, 0, 0, 1,
//	1, 0, 0, 0, 0, 0, 0, 1,
//	1, 0, 0, 1, 1, 0, 0, 1,
//	1, 0, 0, 1, 1, 0, 0, 1,
//	1, 0, 0, 0, 0, 0, 0, 1,
//	1, 0, 0, 0, 0, 0, 0, 1,
//	1, 1, 1, 1, 1, 1, 1, 1,
// };

unsigned char IsBlocked(uint8_t x, uint8_t y)
{
	if (x >= MAP_SIZE || y >= MAP_SIZE)
	{
		return 1;
	}
	return (Map[y * MAP_SIZE + x] != 0);
}
unsigned char IsBlockedAtWorldPosition(int16_t x, int16_t y)
{
	return IsBlocked((uint8_t)(x >> 8), (uint8_t)(y >> 8));
}
uint8_t GetCellType(uint8_t x, uint8_t y)
{
	if (x >= MAP_SIZE || y >= MAP_SIZE)
	{
		return 1;
	}
	return Map[y * MAP_SIZE + x];
}

// Lut.h

const uint8_t scaleLUT[] = {
	15, 0, 8, 15, 0, 4, 8, 12, 15, 0, 2, 5, 8, 10, 13, 15, 0, 2, 4, 6, 8, 10, 12, 14, 15, 0, 1, 3, 4, 6, 8, 9, 11, 12, 14, 15, 0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 15, 0, 0, 1, 2, 3, 4, 4, 5, 6, 7, 8, 8, 9, 10, 11, 12, 12, 13, 14, 15, 15, 0, 0, 1, 2, 2, 3, 4, 5, 5, 6, 7, 8, 8, 9, 10, 10, 11, 12, 13, 13, 14, 15, 15, 0, 0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 10, 11, 12, 12, 13, 14, 14, 15, 15, 0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 9, 10, 11, 11, 12, 12, 13, 14, 14, 15, 15, 0, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6, 7, 8, 8, 9, 9, 10, 10, 11, 12, 12, 13, 13, 14, 14, 15, 15, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 15};

const int16_t sinTable[] = {
	0, 6, 12, 18, 25, 31, 37, 43, 49, 56, 62, 68, 74, 80, 86, 92, 97, 103, 109, 115, 120, 126, 131, 136, 142, 147, 152, 157, 162, 167, 171, 176, 181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 234, 236, 238, 241, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255, 255, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238, 236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185, 181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115, 109, 103, 97, 92, 86, 80, 74, 68, 62, 56, 49, 43, 37, 31, 25, 18, 12, 6, 0, -6, -12, -18, -25, -31, -37, -43, -49, -56, -62, -68, -74, -80, -86, -92, -97, -103, -109, -115, -120, -126, -131, -136, -142, -147, -152, -157, -162, -167, -171, -176, -181, -185, -189, -193, -197, -201, -205, -209, -212, -216, -219, -222, -225, -228, -231, -234, -236, -238, -241, -243, -244, -246, -248, -249, -251, -252, -253, -254, -254, -255, -255, -255, -255, -255, -255, -255, -254, -254, -253, -252, -251, -249, -248, -246, -244, -243, -241, -238, -236, -234, -231, -228, -225, -222, -219, -216, -212, -209, -205, -201, -197, -193, -189, -185, -181, -176, -171, -167, -162, -157, -152, -147, -142, -136, -131, -126, -120, -115, -109, -103, -97, -92, -86, -80, -74, -68, -62, -56, -49, -43, -37, -31, -25, -18, -12, -6};
// Fixedmath.h

#include <math.h>

#define FIXED_SHIFT 8
#define FIXED_ONE (1 << FIXED_SHIFT)
#define INT_TO_FIXED(x) ((x) * FIXED_ONE)
#define FIXED_TO_INT(x) ((x) >> 8)
#define FLOAT_TO_FIXED(x) ((int16_t)((x) * FIXED_ONE))

#define FIXED_ANGLE_MAX 256
#define FIXED_ANGLE_WRAP(x) ((x) & 255)
#define FIXED_ANGLE_90 (FIXED_ANGLE_MAX / 4)
#define FIXED_ANGLE_TO_RADIANS(x) ((x) * (2.0f * 3.141592654f / FIXED_ANGLE_MAX))

int16_t FixedSin(uint8_t angle)
{
	return sinTable[angle];
}

int16_t FixedCos(uint8_t angle)
{
	return sinTable[FIXED_ANGLE_WRAP(FIXED_ANGLE_90 - angle)];
}

// Game.h
void PutPixel(uint8_t x, uint8_t y, uint8_t colour)
{
	Graphic_DrawPoint(x, y, colour);
}
void DrawVLine(uint8_t x, int8_t y1, int8_t y2, uint8_t pattern)
{
	uint8_t y;
	if (y1 > y2)
		while (y1 > y2)
		{
			Graphic_DrawPoint(x, y1, pattern);
			--y1;
		}
	else
		while (y2 > y1)
		{
			Graphic_DrawPoint(x, y2, pattern);
			--y2;
		}
}

// void DrawBitmap(int16_t x, int16_t y, const uint8_t *bitmap);
// void DrawSolidBitmap(int16_t x, int16_t y, const uint8_t *bitmap);
// oid DrawSprite(int16_t x, int16_t y, const uint8_t *bitmap,const uint8_t *mask, uint8_t frame, uint8_t mask_frame);
// void SetLED(uint8_t r, uint8_t g, uint8_t b);
// void FillScreen(uint8_t col);
// void DrawBackground(void);
// void DrawVLine(uint8_t x, int8_t y1, int8_t y2, uint8_t pattern);

//

void Render(void);
int8_t GetHorizon(int16_t x);

Camera camera;

uint8_t wBuffer[DISPLAY_WIDTH];
uint8_t wallIdBuffer[DISPLAY_WIDTH];
int8_t horizonBuffer[DISPLAY_WIDTH];
uint8_t currentWallId = 0;

#if WITH_TEXTURES
void DrawWallLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t col)
{
	//	if(x1 < 0 || y1 < 0 || x2 >= DISPLAY_WIDTH || y2 >= DISPLAY_HEIGHT)
	//		return;
	int16_t displayEnd, dx, dy, x, y, yerror;
	int8_t ystep, drawSlice, horizon;
	if (x1 > x2)
		return;

	if (y1 < 0)
	{
		if (y2 < 0)
			return;

		if (y2 != y1)
			x1 += ((-y1) * (x2 - x1)) / (y2 - y1);

		y1 = 0;
	}

	displayEnd = (DISPLAY_HEIGHT - 1);

	if (y2 > displayEnd)
	{
		if (y1 > displayEnd)
			return;

		if (y2 != y1)
			x2 += (((displayEnd)-y2) * (x1 - x2)) / (y1 - y2);

		y2 = displayEnd;
	}

	if (x1 < 0)
	{
		if (x2 != x1)
			y1 += ((-x1) * (y2 - y1)) / (x2 - x1);

		x1 = 0;
	}

	if (y1 < y2)
	{
		dy = (y2 - y1);
		ystep = 1;
	}
	else
	{
		dy = (y1 - y2);
		ystep = -1;
	}

	dx = (x2 - x1);
	y = y1;
	yerror = (dx / 2);

	for (x = x1; ((x <= x2) && (x < DISPLAY_WIDTH)); ++x)
	{
		// int w = y > HORIZON ? y - HORIZON : HORIZON - y;
		drawSlice = (wallIdBuffer[x] == currentWallId);
		horizon = (horizonBuffer[x] - HORIZON);

		if (drawSlice)
			PutPixel(x, horizon + y, col);
		yerror -= dy;

		while (yerror < 0)
		{
			y += ystep;

			// if(y < 0 || y >= DISPLAY_HEIGHT)
			//	return;

			yerror += dx;

			if (drawSlice && (yerror < 0))
				PutPixel(x, horizon + y, col);

			if ((x == x2) && (y == y2))
				break;
		}
	}
}
#endif

#if WITH_TEXTURES
void DrawWallSegment(const uint8_t *texture, int16_t x1, int16_t w1, int16_t x2, int16_t w2, uint8_t u1clip, uint8_t u2clip, int8_t edgeLeft, int8_t edgeRight, int8_t shadeEdge)
#else
void DrawWallSegment(int16_t x1, int16_t w1, int16_t x2, int16_t w2, int8_t edgeLeft, int8_t edgeRight, int8_t shadeEdge)
#endif
{
	int16_t x, dx, w, dw, wstep, werror, halfDisplayHeight;
	uint8_t wallColour, edgeColour;
	int8_t drawSlice, sliceMask, sliceColour, shadeSlice, horizon, extent;
	int16_t outU1, outU2, interpw1, interpw2, outV1, outV2, u1, v1, u2, v2;
	int16_t index, numLines;
	if (x1 < 0)
	{
#if WITH_TEXTURES
		u1clip += ((int32_t)(0 - x1) * (int32_t)(u2clip - u1clip)) / (x2 - x1);
#endif
		w1 += ((int32_t)(0 - x1) * (int32_t)(w2 - w1)) / (x2 - x1);
		x1 = 0;
		edgeLeft = 0;
	}

	dx = (x2 - x1);

	currentWallId++;

	if (w1 < w2)
	{
		dw = w2 - w1;
		wstep = 1;
	}
	else
	{
		dw = w1 - w2;
		wstep = -1;
	}

	werror = (dx / 2);
	w = w1;
	wallColour = COLOUR_WHITE;
	edgeColour = COLOUR_BLACK;

	for (x = x1; x < DISPLAY_WIDTH; ++x)
	{
		horizon = horizonBuffer[x];

		drawSlice = ((x >= 0) && (wBuffer[x] < w));
		if (drawSlice)
		{
			sliceMask = 0xff;
			sliceColour = wallColour;
			(void)(sliceColour);

			shadeSlice = (shadeEdge && ((x & 1) == 0));
			if ((edgeLeft && (x == x1)) || (edgeRight && (x == x2)))
			{
				sliceMask = 0x00;
				sliceColour = edgeColour;
			}
			else if (shadeSlice)
			{
				sliceMask = 0x55;
			}

			extent = (w > 64) ? 64 : w;
			DrawVLine(x, horizon - extent, horizon + extent, sliceMask);
			PutPixel(x, horizon + extent, edgeColour);
			PutPixel(x, horizon - extent, edgeColour);

			wallIdBuffer[x] = currentWallId;
			wBuffer[x] = (w > 255) ? 255 : w;
		}

		if (x == x2)
			break;

		halfDisplayHeight = (DISPLAY_HEIGHT / 2);

		werror -= dw;

		while (werror < 0)
		{
			w += wstep;
			werror += dx;

			if (drawSlice && (werror < 0) && (w <= halfDisplayHeight))
			{
				PutPixel(x, horizon + (w - 1), edgeColour);
				PutPixel(x, horizon - w, edgeColour);
			}
		}
	}

#if WITH_TEXTURES
	if (w1 < MIN_TEXTURE_DISTANCE || w2 < MIN_TEXTURE_DISTANCE || !texture)
		return;

	if (u1clip == u2clip)
		return;

	index = 0;
	numLines = texture[index];
	++index;

	while (numLines > 0)
	{
		--numLines;

		u1 = texture[index + 0];
		v1 = texture[index + 1];
		u2 = texture[index + 2];
		v2 = texture[index + 3];

		index += 4;

		// if(u1clip != 0 || u2clip != 128)
		//	continue;

		if (u2 < u1clip || u1 > u2clip)
			continue;

		if (u1 < u1clip)
		{
			if (u2 != u1)
				v1 += (u1clip - u1) * (v2 - v1) / (u2 - u1);

			u1 = u1clip;
		}

		if (u2 > u2clip)
		{
			if (u2 != u1)
				v2 += (u2clip - u2) * (v1 - v2) / (u1 - u2);

			u2 = u2clip;
		}

		u1 = (128 * (u1 - u1clip)) / (u2clip - u1clip);
		u2 = (128 * (u2 - u1clip)) / (u2clip - u1clip);

		outU1 = ((((int32_t)(u1)*dx) >> 7) + x1);
		outU2 = ((((int32_t)(u2)*dx) >> 7) + x1);

		interpw1 = (((u1 * (w2 - w1)) >> 7) + w1);
		interpw2 = (((u2 * (w2 - w1)) >> 7) + w1);

		outV1 = ((interpw1 * v1) >> 6);
		outV2 = ((interpw2 * v2) >> 6);

		// uint8_t horizon = horizonBuffer[x]
		// DrawLine(ScreenSurface, outU1, HORIZON - interpw1 + outV1, outU2, HORIZON - interpw2 + outV2, edgeColour, edgeColour, edgeColour);
		DrawWallLine(outU1, HORIZON - interpw1 + outV1, outU2, HORIZON - interpw2 + outV2, edgeColour);
		// DrawWallLine(outU1, -interpw1 + outV1, outU2, -interpw2 + outV2, edgeColour);
	}
#endif
}

int8_t isFrustrumClipped(int16_t x, int16_t y)
{
	if ((camera.clipCos * (x - camera.cellX) - camera.clipSin * (y - camera.cellY)) < -512)
		return 1;
	if ((camera.clipSin * (x - camera.cellX) + camera.clipCos * (y - camera.cellY)) < -512)
		return 1;

	return 0;
}

void TransformToViewSpace(int16_t x, int16_t y, int16_t *outX, int16_t *outY)
{
	int32_t relX = x - camera.x;
	int32_t relY = y - camera.y;
	*outY = (int16_t)((camera.rotCos * relX) >> 8) - (int16_t)((camera.rotSin * relY) >> 8);
	*outX = (int16_t)((camera.rotSin * relX) >> 8) + (int16_t)((camera.rotCos * relY) >> 8);
}

void TransformToScreenSpace(int16_t viewX, int16_t viewZ, int16_t *outX, int16_t *outW)
{
	// apply perspective projection
	*outX = (int16_t)((int32_t)viewX * NEAR_PLANE * CAMERA_SCALE / viewZ);
	*outW = (int16_t)((CELL_SIZE / 2 * NEAR_PLANE * CAMERA_SCALE) / viewZ);

	// transform into screen space
	*outX = (int16_t)((DISPLAY_WIDTH / 2) + *outX);
}

#if WITH_TEXTURES
void DrawWall(const uint8_t *texture, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t edgeLeft, int8_t edgeRight, int8_t shadeEdge)
#else
void DrawWall(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int8_t edgeLeft, int8_t edgeRight, int8_t shadeEdge)
#endif
{
	int16_t viewX1, viewZ1, viewX2, viewZ2;
	int16_t vx1, vx2, sx1, sx2, w1, w2;
#if WITH_TEXTURES
	uint8_t u1 = 0, u2 = 128;
#endif

	TransformToViewSpace(x1, y1, &viewX1, &viewZ1);
	TransformToViewSpace(x2, y2, &viewX2, &viewZ2);

	// Frustum cull
	if (viewX2 < 0 && -2 * viewZ2 > viewX2)
		return;
	if (viewX1 > 0 && 2 * viewZ1 < viewX1)
		return;

	// clip to the front plane
	if ((viewZ1 < CLIP_PLANE) && (viewZ2 < CLIP_PLANE))
		return;

	if (viewZ1 < CLIP_PLANE)
	{
#if WITH_TEXTURES
		u1 += (CLIP_PLANE - viewZ1) * (u2 - u1) / (viewZ2 - viewZ1);
#endif
		viewX1 += (CLIP_PLANE - viewZ1) * (viewX2 - viewX1) / (viewZ2 - viewZ1);
		viewZ1 = CLIP_PLANE;
		edgeLeft = 0;
	}
	else if (viewZ2 < CLIP_PLANE)
	{
#if WITH_TEXTURES
		u2 += (CLIP_PLANE - viewZ2) * (u1 - u2) / (viewZ1 - viewZ2);
#endif
		viewX2 += (CLIP_PLANE - viewZ2) * (viewX1 - viewX2) / (viewZ1 - viewZ2);
		viewZ2 = CLIP_PLANE;
		edgeRight = 0;
	}

	// apply perspective projection
	vx1 = (int16_t)((int32_t)viewX1 * NEAR_PLANE * CAMERA_SCALE / viewZ1);
	vx2 = (int16_t)((int32_t)viewX2 * NEAR_PLANE * CAMERA_SCALE / viewZ2);

	// transform the end points into screen space
	sx1 = (int16_t)((DISPLAY_WIDTH / 2) + vx1);
	sx2 = (int16_t)((DISPLAY_WIDTH / 2) + vx2) - 1;

	if (sx1 >= sx2 || sx2 < 0 || sx1 >= DISPLAY_WIDTH)
		return;

	w1 = (int16_t)((CELL_SIZE / 2 * NEAR_PLANE * CAMERA_SCALE) / viewZ1);
	w2 = (int16_t)((CELL_SIZE / 2 * NEAR_PLANE * CAMERA_SCALE) / viewZ2);

#if WITH_TEXTURES
	DrawWallSegment(texture, sx1, w1, sx2, w2, u1, u2, edgeLeft, edgeRight, shadeEdge);
#else
	DrawWallSegment(sx1, w1, sx2, w2, edgeLeft, edgeRight, shadeEdge);
#endif
}
/*
const uint8_t testSprite[]  =
{
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
	0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
};
*/
const uint8_t testSprite[] =
	{

		0,
		1,
		1,
		1,
		0,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		1,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		0,
		1,
		0,
		1,
		0,
		0,
		0,
		0,
		0,
		0,
		1,
		0,
		1,
		0,
		0,
		1,
		0,
		0,
		1,
		1,
		0,
		0,
		0,
		0,
		0,
		0,
		1,
		0,
		1,
		0,
		0,
		1,
		0,
		0,
		1,
		1,
		0,
		0,
		0,
		0,
		0,
		0,
		1,
		1,
		1,
		0,
		0,
		1,
		0,
		0,
		1,
		1,
		0,
		0,
		0,
		1,
		0,
		1,
		0,
		0,
		0,
		1,
		0,
		1,
		0,
		1,
		0,
		0,
		1,
		1,
		1,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		0,
		1,
		1,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		1,
		1,
		1,
		0,
		0,
		0,
		1,
		1,
		0,
		0,
		0,
		0,
		1,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		1,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		1,
		0,
		0,
		1,
		1,
		1,
		0,
		0,
		0,
		1,
		1,
		0,
		0,
		0,
		0,
		1,
		1,
		0,
};
void DrawScaled(const uint8_t *Data, int x, int y, uint8_t halfSize)
{
	int8_t Size, isX2, isBlack;
	int16_t quarterSize, lutSize, outX, outY, lutIndex, lutIndexI, lutIndexJ;
	uint8_t *lut;
	uint8_t u0, u1, i, j, u2, v1, v2, up, down, px, left, right, colour;
	if (halfSize < 2)
		return;

	if (halfSize > MAX_SPRITE_SIZE)
		return;

	Size = (halfSize * 2);
	isX2 = (Size > MAX_SPRITE_SIZE);
	quarterSize = (halfSize / 2);
	lutSize = (isX2) ? (quarterSize * quarterSize) : (halfSize * halfSize);

	lut = &scaleLUT[lutSize];

	outX = x;

	for (i = 0; ((i < Size) && (outX < DISPLAY_WIDTH)); ++i)
	{
		lutIndexI = (isX2) ? ((i + 1) / 2) : (i + 1);
		u2 = lut[lutIndexI];

		// uint8_t v0 = 0;
		// static_cast<void>(v0);

		v1 = 0;
		up = 0;
		px = Data[0];

		if (outX >= 0 && wBuffer[outX] < halfSize)
		{
			outY = y;

			for (j = 0; ((j < Size) && (outY < DISPLAY_HEIGHT)); j++)
			{
				lutIndexJ = (isX2) ? ((j + 1) / 2) : (j + 1);
				v2 = lut[lutIndexJ];
				down = Data[v2 * BASE_SPRITE_SIZE + u1];

				if ((outX >= 0) && (wBuffer[outX] < halfSize))
				{
					if (px == 0)
					{
						left = Data[v1 * BASE_SPRITE_SIZE + u0];
						right = Data[v1 * BASE_SPRITE_SIZE + u2];

						if ((up | down | left | right) != 0)
							PutPixel(outX, outY, COLOUR_BLACK);
					}
					else
					{
						isBlack = ((i == 0) || (j == 0) || (i == (Size - 1)) || (j == (Size - 1)) || (px == 2));

						colour = (isBlack) ? COLOUR_BLACK : COLOUR_WHITE;

						PutPixel(outX, outY, colour);
					}
				}

				// v0 = v1;
				v1 = v2;

				up = px;
				px = down;
				++outY;
			}
		}

		u0 = u1;
		u1 = u2;
		++outX;
	}
}

int8_t GetHorizon(int16_t x)
{
	const int index = (x < 0) ? 0 : (x >= DISPLAY_WIDTH) ? (DISPLAY_WIDTH - 1)
														 : x;
	return horizonBuffer[index];
}

int8_t canCullWithFrustum(int16_t relX, int16_t relZ)
{
	if (relZ < CLIP_PLANE)
		return 1;

	if ((relX < 0) && (2 * -relZ) > relX)
		return 1;

	if ((relX > 0) && (2 * relZ) < relX)
		return 1;

	return 0;
}

void DrawObject(int16_t x, int16_t y)
{
	int16_t relX, screenX;
	int16_t relZ, screenW;

	TransformToViewSpace(x, y, &relX, &relZ);

	// Frustum cull
	if (canCullWithFrustum(relX, relZ))
		return;

	TransformToScreenSpace(relX, relZ, &screenX, &screenW);

	DrawScaled(testSprite, screenX - screenW, GetHorizon(screenX) - screenW, (uint8_t)screenW);
}

/*
template< size_t particleCount >
void DrawParticleSystem(const ParticleSystem<particleCount> & system, int16_t x, int16_t y)
{
	int16_t relX;
	int16_t relZ;

	TransformToViewSpace(x, y, relX, relZ);

	// Frustum cull
	if(canCullWithFrustum(relX, relZ))
		return;

	int16_t screenX;
	int16_t screenW;

	TransformToScreenSpace(relX, relZ, screenX, screenW);

	system.Draw(screenX, screenW);
}

ParticleSystem<PARTICLES_PER_SYSTEM> testParticles;
*/

void DrawBackground()
{
	uint8_t *ptr = Graphic_GetBuffer();
	uint8_t counter = 64;

	while (counter--)
	{
		*ptr++ = 0x55;
		*ptr++ = 0;
		*ptr++ = 0x55;
		*ptr++ = 0;
		*ptr++ = 0x55;
		*ptr++ = 0;
		*ptr++ = 0x55;
		*ptr++ = 0;
	}

	counter = 64;
	while (counter--)
	{
		*ptr++ = 0x55;
		*ptr++ = 0xAA;
		*ptr++ = 0x55;
		*ptr++ = 0xAA;
		*ptr++ = 0x55;
		*ptr++ = 0xAA;
		*ptr++ = 0x55;
		*ptr++ = 0xAA;
	}
}

void DrawCell(uint8_t x, uint8_t y)
{
	int16_t x1, x2, y1, y2;
	int8_t blockedLeft;
	int8_t blockedRight;
	int8_t blockedUp;
	int8_t blockedDown;
	uint8_t cellType;
	uint8_t *texture;
	if (!IsBlocked(x, y))
	{
		return;
	}

	if (isFrustrumClipped(x, y))
	{
		return;
	}

	x1 = x * CELL_SIZE;
	y1 = y * CELL_SIZE;
	x2 = x1 + CELL_SIZE;
	y2 = y1 + CELL_SIZE;

	blockedLeft = IsBlocked(x - 1, y);
	blockedRight = IsBlocked(x + 1, y);
	blockedUp = IsBlocked(x, y - 1);
	blockedDown = IsBlocked(x, y + 1);

#if WITH_TEXTURES
	cellType = GetCellType(x, y);
	texture = (const uint8_t *)textures[cellType - 1];
//	texture = nullptr;
#endif

	if (!blockedLeft && camera.x < x1)
	{
#if WITH_TEXTURES
		DrawWall(texture, x1, y1, x1, y2, !blockedUp && camera.y > y1, !blockedDown && camera.y < y2, 1);
#else
		DrawWall(x1, y1, x1, y2, !blockedUp && camera.y > y1, !blockedDown && camera.y < y2, 1);
#endif
	}

	if (!blockedDown && camera.y > y2)
	{
#if WITH_TEXTURES
		DrawWall(texture, x1, y2, x2, y2, !blockedLeft && camera.x > x1, !blockedRight && camera.x < x2, 0);
#else
		DrawWall(x1, y2, x2, y2, !blockedLeft && camera.x > x1, !blockedRight && camera.x < x2, 0);
#endif
	}

	if (!blockedRight && camera.x > x2)
	{
#if WITH_TEXTURES
		DrawWall(texture, x2, y2, x2, y1, !blockedDown && camera.y < y2, !blockedUp && camera.y > y1, 1);
#else
		DrawWall(x2, y2, x2, y1, !blockedDown && camera.y < y2, !blockedUp && camera.y > y1, 1);
#endif
	}

	if (!blockedUp && camera.y < y1)
	{
#if WITH_TEXTURES
		DrawWall(texture, x2, y1, x1, y1, !blockedRight && camera.x < x2, !blockedLeft && camera.x > x1, 0);
#else
		DrawWall(x2, y1, x1, y1, !blockedRight && camera.x < x2, !blockedLeft && camera.x > x1, 0);
#endif
	}
}

void DrawCells()
{
	uint8_t x, y;
	for (y = 0; y < MAP_SIZE; y++)
	{
		for (x = 0; x < MAP_SIZE; x++)
		{
			DrawCell(x, y);
		}
	}
}
/*
uint GetCellSafe(uint8_t x,uint8_t y){
	if(x >= MAP_SIZE || y >= MAP_SIZE){
		return 1;
	}else{
		return Map[y * MAP_SIZE + x];
	}
}
voidDrawCell(uint8_t x, uint8_t y)
{
	uint cellType = GetCellSafe(x, y);
	uint8_t torchScale;
	uint16_t* torchSpriteData;
	if (isFrustrumClipped(x, y))
	{
		return;
	}

	switch (cellType)
	{
	case 2
	{
		torchSpriteData = torchSpriteData2;//Game::globalTickFrame & 4 ? torchSpriteData1 : torchSpriteData2;//用系统时钟来控制动画
		torchScale = 75;

		if (Map::IsSolid(x - 1, y))
		{
			DrawObject(torchSpriteData, x * CELL_SIZE + CELL_SIZE / 7, y * CELL_SIZE + CELL_SIZE / 2, torchScale, AnchorType::Center);
		}
		else if (Map::IsSolid(x + 1, y))
		{
			DrawObject(torchSpriteData, x * CELL_SIZE + 6 * CELL_SIZE / 7, y * CELL_SIZE + CELL_SIZE / 2, torchScale, AnchorType::Center);
		}
		else if (Map::IsSolid(x, y - 1))
		{
			DrawObject(torchSpriteData, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 7, torchScale, AnchorType::Center);
		}
		else if (Map::IsSolid(x, y + 1))
		{
			DrawObject(torchSpriteData, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + 6 * CELL_SIZE / 7, torchScale, AnchorType::Center);
		}
	}
	return;
	/*
	case CellType::Entrance:
		DrawObject(entranceSpriteData, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2, 96, AnchorType::Ceiling);
		return;
	case CellType::Exit:
		DrawObject(exitSpriteData, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2, 96);
		return;
	case CellType::Urn:
		DrawObject(urnSpriteData, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2, 80);
		return;
	case CellType::Potion:
		DrawObject(potionSpriteData, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2, 64);
		return;
	case CellType::Scroll:
		DrawObject(scrollSpriteData, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2, 64);
		return;
	case CellType::Coins:
		DrawObject(coinsSpriteData, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2, 64);
		return;
	case CellType::Crown:
		DrawObject(crownSpriteData, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2, 64);
		return;
	case CellType::Sign:
		DrawObject(signSpriteData, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2, 80);
		return;
	case CellType::Chest:
		DrawObject(chestSpriteData, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2, 75);
		return;
	case CellType::ChestOpened:
		DrawObject(chestOpenSpriteData, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2, 75);
		return;
	*/
/*
	default:
		break;
	}

	if(numBufferSlicesFilled >= DISPLAY_WIDTH)
	{
		return;
	}

	if (!Map::IsSolid(x, y))
	{
		return;
	}

	int16_t x1 = x * CELL_SIZE;
	int16_t y1 = y * CELL_SIZE;
	int16_t x2 = x1 + CELL_SIZE;
	int16_t y2 = y1 + CELL_SIZE;

	bool blockedLeft = Map::IsSolid(x - 1, y);
	bool blockedRight = Map::IsSolid(x + 1, y);
	bool blockedUp = Map::IsSolid(x, y - 1);
	bool blockedDown = Map::IsSolid(x, y + 1);

#if WITH_IMAGE_TEXTURES
	const uint16_t* texture = wallTextureData + (16 * (cellType - 1));
#elif WITH_VECTOR_TEXTURES
	const uint8_t* texture = vectorTexture0; // (const uint8_t*) pgm_read_ptr(&textures[(uint8_t)cellType - (uint8_t)CellType::FirstSolidCell]);
#endif

	if (!blockedLeft && camera.x < x1)
	{
#if WITH_TEXTURES
		DrawWall(texture, x1, y1, x1, y2, !blockedUp && camera.y > y1, !blockedDown && camera.y < y2, true);
#else
		DrawWall(x1, y1, x1, y2, !blockedUp && camera.y > y1, !blockedDown && camera.y < y2, true);
#endif
	}

	if (!blockedDown && camera.y > y2)
	{
#if WITH_TEXTURES
		DrawWall(texture, x1, y2, x2, y2, !blockedLeft && camera.x > x1, !blockedRight && camera.x < x2, false);
#else
		DrawWall(x1, y2, x2, y2, !blockedLeft && camera.x > x1, !blockedRight && camera.x < x2, false);
#endif
	}

	if (!blockedRight && camera.x > x2)
	{
#if WITH_TEXTURES
		DrawWall(texture, x2, y2, x2, y1, !blockedDown && camera.y < y2, !blockedUp && camera.y > y1, true);
#else
		DrawWall(x2, y2, x2, y1, !blockedDown && camera.y < y2, !blockedUp && camera.y > y1, true);
#endif
	}

	if (!blockedUp && camera.y < y1)
	{
#if WITH_TEXTURES
		DrawWall(texture, x2, y1, x1, y1, !blockedRight && camera.x < x2, !blockedLeft && camera.x > x1, false);
#else
		DrawWall(x2, y1, x1, y1, !blockedRight && camera.x < x2, !blockedLeft && camera.x > x1, false);
#endif
	}
}

void Renderer::DrawCells()
{
	constexpr int8_t MAP_BUFFER_WIDTH = 16;
	constexpr int8_t MAP_BUFFER_HEIGHT = 16;

	int16_t cosAngle = FixedCos(camera.angle);
	int16_t sinAngle = FixedSin(camera.angle);

	int8_t bufferX = (int8_t)((camera.x + cosAngle * 7) >> 8) - MAP_BUFFER_WIDTH / 2;
	int8_t bufferY = (int8_t)((camera.y + sinAngle * 7) >> 8) - MAP_BUFFER_WIDTH / 2;;

// This should make cells draw front to back
	int8_t xd, yd;
	int8_t x1, y1, x2, y2;

	if(bufferX < 0)
		bufferX = 0;
	if(bufferY < 0)
		bufferY = 0;
	if(bufferX > Map::width - MAP_BUFFER_WIDTH)
		bufferX = Map::width - MAP_BUFFER_WIDTH;
	if(bufferY > Map::height - MAP_BUFFER_HEIGHT)
		bufferY = Map::height - MAP_BUFFER_HEIGHT;



	if(camera.rotCos > 0)
	{
		x1 = bufferX;
		x2 = x1 + MAP_BUFFER_WIDTH;
		xd = 1;
	}
	else
	{
		x2 = bufferX - 1;
		x1 = x2 + MAP_BUFFER_WIDTH;
		xd = -1;
	}
	if(camera.rotSin < 0)
	{
		y1 = bufferY;
		y2 = y1 + MAP_BUFFER_HEIGHT;
		yd = 1;
	}
	else
	{
		y2 = bufferY - 1;
		y1 = y2 + MAP_BUFFER_HEIGHT;
		yd = -1;
	}

	if(ABS(camera.rotCos) < ABS(camera.rotSin))
	{
		for(int8_t y = y1; y != y2; y += yd)
		{
			for(int8_t x = x1; x != x2; x+= xd)
			{
				DrawCell(x, y);
			}
		}
	}
	else
	{
		for(int8_t x = x1; x != x2; x+= xd)
		{
			for(int8_t y = y1; y != y2; y += yd)
			{
				DrawCell(x, y);
			}
		}
	}
}
*/
void Render()
{

	uint8_t n, doorX, doorY;
	currentWallId = 0;
	DrawBackground();
	for (n = 0; n < DISPLAY_WIDTH; n++)
	{
		wallIdBuffer[n] = 0;
		wBuffer[n] = 0;
		horizonBuffer[n] = HORIZON + (((DISPLAY_WIDTH / 2 - n) * camera.tilt) >> 8);
	}

	camera.cellX = camera.x / CELL_SIZE;
	camera.cellY = camera.y / CELL_SIZE;

	camera.rotCos = FixedCos(-camera.angle);
	camera.rotSin = FixedSin(-camera.angle);
	camera.clipCos = FixedCos(-camera.angle + CLIP_ANGLE);
	camera.clipSin = FixedSin(-camera.angle + CLIP_ANGLE);

#if !WITH_TEXTURES
	doorX = 3;
	doorY = 4;
	DrawWall(doorX * CELL_SIZE + CELL_SIZE / 2, doorY * CELL_SIZE,
			 doorX * CELL_SIZE + CELL_SIZE / 2, doorY * CELL_SIZE + CELL_SIZE / 2, 1, 1, 0);
	DrawWall(doorX * CELL_SIZE + CELL_SIZE / 2, doorY * CELL_SIZE + CELL_SIZE / 2,
			 doorX * CELL_SIZE + CELL_SIZE / 2, doorY * CELL_SIZE + CELL_SIZE, 1, 1, 0);
	DrawWall(doorX * CELL_SIZE + CELL_SIZE / 2, doorY * CELL_SIZE + CELL_SIZE / 2,
			 doorX * CELL_SIZE + CELL_SIZE / 2, doorY * CELL_SIZE, 1, 1, 0);
	DrawWall(doorX * CELL_SIZE + CELL_SIZE / 2, doorY * CELL_SIZE + CELL_SIZE,
			 doorX * CELL_SIZE + CELL_SIZE / 2, doorY * CELL_SIZE + CELL_SIZE / 2, 1, 1, 0);
#endif

	DrawCells();

	DrawObject(3 * CELL_SIZE + CELL_SIZE / 2, 1 * CELL_SIZE + CELL_SIZE / 2);
	DrawObject(1 * CELL_SIZE + CELL_SIZE / 2, 5 * CELL_SIZE + CELL_SIZE / 2);

	// testParticles.Step();
	// DrawParticleSystem(testParticles, 1 * CELL_SIZE + CELL_SIZE / 2, 1 * CELL_SIZE + CELL_SIZE / 2);
}

// Game.c
int16_t cameraVelocityX;
int16_t cameraVelocityY;

void InitGame()
{
	camera.x = CELL_SIZE * 1 + CELL_SIZE / 2;
	camera.y = CELL_SIZE * 1 + CELL_SIZE / 2;
	camera.tilt = 0;
}

int8_t cameraAngularVelocity = 0;

#define COLLISION_SIZE 48

int8_t IsObjectColliding(Camera *obj)
{
	return IsBlockedAtWorldPosition(obj->x - COLLISION_SIZE, obj->y - COLLISION_SIZE) || IsBlockedAtWorldPosition(obj->x + COLLISION_SIZE, obj->y - COLLISION_SIZE) || IsBlockedAtWorldPosition(obj->x + COLLISION_SIZE, obj->y + COLLISION_SIZE) || IsBlockedAtWorldPosition(obj->x - COLLISION_SIZE, obj->y + COLLISION_SIZE);
}

void MoveCamera(Camera *entity, int16_t deltaX, int16_t deltaY)
{
	entity->x += deltaX;
	entity->y += deltaY;

	if (IsObjectColliding(entity))
	{
		entity->y -= deltaY;
		if (IsObjectColliding(entity))
		{
			entity->x -= deltaX;
			entity->y += deltaY;

			if (IsObjectColliding(entity))
			{
				entity->y -= deltaY;
			}
		}
	}
	/*int16_t newX = entity->x + deltaX;
	int16_t newY = entity->y + deltaY;

	if (deltaX < 0)
	{
		if (IsBlockedAtWorldPosition(newX - COLLISION_SIZE, entity->y)
			|| IsBlockedAtWorldPosition(newX - COLLISION_SIZE, entity->y - COLLISION_SIZE)
			|| IsBlockedAtWorldPosition(newX - COLLISION_SIZE, entity->y + COLLISION_SIZE))
		{
			entity->x = (entity->x & 0xff00) + COLLISION_SIZE;
		}
		else
		{
			entity->x = newX;
		}
	}
	else if(deltaX > 0)
	{
		if (IsBlockedAtWorldPosition(newX + COLLISION_SIZE, entity->y)
			|| IsBlockedAtWorldPosition(newX + COLLISION_SIZE, entity->y - COLLISION_SIZE)
			|| IsBlockedAtWorldPosition(newX + COLLISION_SIZE, entity->y + COLLISION_SIZE))
		{
			entity->x = (entity->x & 0xff00) + CELL_SIZE - COLLISION_SIZE;
		}
		else
		{
			entity->x = newX;
		}
	}

	if (deltaY < 0)
	{
		if (IsBlockedAtWorldPosition(entity->x, newY - COLLISION_SIZE)
			|| IsBlockedAtWorldPosition(entity->x + COLLISION_SIZE, newY - COLLISION_SIZE)
			|| IsBlockedAtWorldPosition(entity->x - COLLISION_SIZE, newY - COLLISION_SIZE))
		{
			entity->y = (entity->y & 0xff00) + COLLISION_SIZE;
		}
		else
		{
			entity->y = newY;
		}
	}
	else if (deltaY > 0)
	{
		if (IsBlockedAtWorldPosition(entity->x, newY + COLLISION_SIZE)
			|| IsBlockedAtWorldPosition(entity->x + COLLISION_SIZE, newY + COLLISION_SIZE)
			|| IsBlockedAtWorldPosition(entity->x - COLLISION_SIZE, newY + COLLISION_SIZE))
		{
			entity->y = (entity->y & 0xff00) + CELL_SIZE - COLLISION_SIZE;
		}
		else
		{
			entity->y = newY;
		}
	}*/
}
uint16_t xs = 0;
uint16_t Random()
{
	xs ^= xs << 7;
	xs ^= xs >> 9;
	xs ^= xs << 8;
	return xs;
}
void TickGame()
{
	uint8_t kl = 0, kr = 0;
	int8_t turnDelta = 0;
	int8_t targetTilt = 0;
	int8_t moveDelta = 0;
	int8_t strafeDelta = 0;
	int8_t targetBob = 0;
	int tiltTimer = 0;
	int tiltRate, bobRate;
	int16_t cosAngle;
	int16_t sinAngle;
	int16_t cos90Angle;
	int16_t sin90Angle;
	// uint8_t shakeTime=0;

	kl = Key_Scan() >> 8;
	kr = Key_Scan();

	if (kl != 0xff && kr != 0xff)
	{
		if (kr == 0x48)
		{
			strafeDelta--;
		}
		if (kr == 0x4a)
		{
			strafeDelta++;
		}
	}
	if (kl == 0xff && kr != 0xff)
	{
		if (kr == 0x48)
		{
			turnDelta -= (TURN_SPEED) * 2;
		}
		if (kr == 0x4a)
		{
			turnDelta += (TURN_SPEED) * 2;
		}
	}

	if (cameraAngularVelocity < turnDelta)
	{
		cameraAngularVelocity++;
	}
	else if (cameraAngularVelocity > turnDelta)
	{
		cameraAngularVelocity--;
	}

	camera.angle += cameraAngularVelocity >> 1;

	if (kr == 0x44)
	{
		moveDelta++;
	}
	if (kr == 0x49)
	{
		moveDelta--;
	}
	tiltTimer = 0;
	tiltTimer++;
	if (moveDelta && 0)
	{
		targetTilt = (int8_t)(FixedSin(tiltTimer * 10) / 32);
	}
	else
	{
		targetTilt = 0;
	}

	targetTilt += cameraAngularVelocity * 3;
	targetTilt += strafeDelta * 14;
	targetBob = moveDelta || strafeDelta ? FixedSin(tiltTimer * 10) / 128 : 0;

	/*
	tiltRate=6;
	if (camera.tilt < targetTilt)
	{
		camera.tilt += tiltRate;
		if (camera.tilt > targetTilt)
		{
			camera.tilt = targetTilt;
		}
	}
	else if (camera.tilt > targetTilt)
	{
		camera.tilt -= tiltRate;
		if (camera.tilt < targetTilt)
		{
			camera.tilt = targetTilt;
		}
	}
	*/
	bobRate = 3;

	if (camera.bob < targetBob)
	{
		camera.bob += bobRate;
		if (camera.bob > targetBob)
		{
			camera.bob = targetBob;
		}
	}
	else if (camera.bob > targetBob)
	{
		camera.bob -= bobRate;
		if (camera.bob < targetBob)
		{
			camera.bob = targetBob;
		}
	}

	cosAngle = FixedCos(camera.angle);
	sinAngle = FixedSin(camera.angle);

	cos90Angle = FixedCos(camera.angle + FIXED_ANGLE_90);
	sin90Angle = FixedSin(camera.angle + FIXED_ANGLE_90);
	// camera.x += (moveDelta * cosAngle) >> 4;
	// camera.y += (moveDelta * sinAngle) >> 4;
	cameraVelocityX += (moveDelta * cosAngle) / 32;
	cameraVelocityY += (moveDelta * sinAngle) / 32;

	cameraVelocityX += (strafeDelta * cos90Angle) / 32;
	cameraVelocityY += (strafeDelta * sin90Angle) / 32;

	MoveCamera(&camera, cameraVelocityX / 4, cameraVelocityY / 4);

	cameraVelocityX = (cameraVelocityX * 7) / 8;
	cameraVelocityY = (cameraVelocityY * 7) / 8;

	Render();
}
uint32_t tick = 0;
uint8_t fps = 0, fcnt = 0;
void Timer1_Isr(void) interrupt 3
{
	tick++;
}

void Timer1_Init(void) // 1毫秒@40.000MHz
{
	AUXR |= 0x40; // 定时器时钟1T模式
	TMOD &= 0x0F; // 设置定时器模式
	TL1 = 0xC0;	  // 设置定时初始值
	TH1 = 0x63;	  // 设置定时初始值
	TF1 = 0;	  // 清除TF1标志
	TR1 = 1;	  // 定时器1开始计时
	ET1 = 1;	  // 使能定时器1中断
}
sbit Audio = P2 ^ 1;
sbit Wifi = P2 ^ 0;
sbit Laser = P1 ^ 3;
sbit light = P1 ^ 7;
void Sys_Init()
{
	P_SW2 |= 0x80; // 扩展寄存器(XFR)访问使能
	P0M0 = 0x00;
	P0M1 = 0x00;
	P1M1 = 0x00;
	P1M0 = 0x00; // 设置为高阻抗
	P2M0 = 0x00;
	P2M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
	Audio = 0;
	Wifi = 0;
	Laser = 0;
	light = 0;
	OLED_Init();
	OLED_Display_On();
	OLED_ColorTurn(0);
	OLED_Clear();
	Graphic_ClearBufferBit(1);
	Timer1_Init();
	EA = 1;
}

void main()
{
	Sys_Init();
	InitGame();
	while (1)
	{

		TickGame();
		Graphic_Flush();
		Graphic_ClearBufferBit(1);
		fcnt++;
		// OLED_Printf(0, 0, 16, "%bd",fps);
		if (tick && tick % 1000 == 0)
		{
			// Graphic_Printf(0, 0,
			fps = fcnt;
			// fcnt = 0;
		}
	}
	return;
}
