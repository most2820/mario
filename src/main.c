#include <genesis.h>
#include "resources.h"
#define SFX_JUMP 64
#define MAX_X 640
#define MAX_Y 224
u8 level[14][40] = {
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

typedef struct
{
	float x;
	float y;
} Point;

typedef struct
{
	Sprite *sprite;
	Point position;
	Point speed;
	bool onGround;
} Player;

typedef struct
{
	Point position;
} Camera;

Map *bga;
Player player;
Camera camera;

void handleInput()
{
	u16 value = JOY_readJoypad(JOY_1);

	if (value)
	{
		if (value & BUTTON_LEFT)
		{
			player.speed.x = -2;
		}
		else if (value & BUTTON_RIGHT)
		{
			player.speed.x = 2;
		}
		if (value & BUTTON_C && player.onGround)
		{
			player.speed.y = -8;
			player.onGround = FALSE;
			XGM_startPlayPCM(SFX_JUMP, 1, SOUND_PCM_CH2);
		}
	}
}

collision(int dir)
{
	for (int i = player.position.y / 16; i < (player.position.y + marioSprite.h) / 16; i++)
	{
		for (int j = player.position.x / 16; j < (player.position.x + marioSprite.w) / 16; j++)
		{
			if (dir == 0)
			{
				if (player.speed.x > 0 && level[i][j] == 0)
				{
					player.position.x = j * 16 - marioSprite.w;
				}
				if (player.speed.x < 0 && level[i][j] == 0)
				{
					player.position.x = j * 16 + marioSprite.w;
				}
			}
			if (dir == 1)
			{
				if (player.speed.y > 0 && level[i][j] == 0)
				{
					player.position.y = i * 16 - marioSprite.h;
					player.onGround = TRUE;
					player.speed.y = 0;
				}
				if (player.speed.y < 0 && level[i][j] == 0)
				{
					player.position.y = i * 16 + marioSprite.h;
					player.speed.y = 0;
				}
			}
		}
	}
}

void setAnimation()
{
	if (player.speed.x > 0 && player.onGround)
	{
		SPR_setAnim(player.sprite, 1);
		SPR_setHFlip(player.sprite, FALSE);
	}
	else if (player.speed.x < 0 && player.onGround)
	{
		SPR_setAnim(player.sprite, 1);
		SPR_setHFlip(player.sprite, TRUE);
	}
	else if (player.onGround)
	{
		SPR_setAnim(player.sprite, 0);
	}
	else if (player.speed.y != 0)
	{
		SPR_setAnim(player.sprite, 2);
	}
}

void setCameraPosition(int x, int y)
{
	if (x - 160 != camera.position.x)
	{
		camera.position.x = x - 160;

		if (camera.position.x < 0)
		{
			camera.position.x = 0;
		}
		else if (camera.position.x > MAX_X - 320)
		{
			camera.position.x = MAX_X - 320;
		}
		MAP_scrollTo(bga, camera.position.x, camera.position.y);
	}
}

void update()
{
	player.position.x += player.speed.x;

	collision(0);

	if (!player.onGround)
	{
		player.speed.y = player.speed.y + 0.4;
	}

	player.position.y += player.speed.y;

	player.onGround = FALSE;

	collision(1);

	setAnimation();

	SPR_setPosition(player.sprite, player.position.x - camera.position.x, player.position.y);

	setCameraPosition(player.position.x, player.position.y);

	player.speed.x = 0;
}

int main(u16 ind)
{
	ind = TILE_USERINDEX;

	camera.position.x = 0;
	camera.position.y = 0;

	XGM_setPCM(SFX_JUMP, sfx_jump, sizeof(sfx_jump));

	VDP_loadTileSet(&bga_tileset, ind, DMA);
	bga = MAP_create(&bga_map, BG_B, TILE_ATTR_FULL(0, FALSE, FALSE, FALSE, TILE_USERINDEX));

	VDP_setPaletteColors(0, palette_all.data, 16);
	SPR_init();
	VDP_setPalette(PAL3, marioSprite.palette->data);

	player.position.x = 64;
	player.position.y = 176;
	player.speed.x = 0;
	player.speed.y = 0;
	player.onGround = FALSE;
	player.sprite = SPR_addSprite(&marioSprite, player.position.x, player.position.y, TILE_ATTR(PAL3, 0, FALSE, FALSE));

	while (1)
	{
		SPR_update();

		handleInput();

		update();

		VDP_showFPS(FALSE);

		SYS_doVBlankProcess();
	}

	return (0);
}