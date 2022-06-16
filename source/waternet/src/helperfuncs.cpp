#include <Gamebuino-Meta.h>
#include <stdint.h>


#include "commonvars.h"
#include "helperfuncs.h"

#include "../res/blocktiles.h"

Image *currentTiles = nullptr;
uint8_t palIndex = 0;

#define MAKECOLOR(r,g,b) (((b >> 3) & 0x1f) |  (((g >> 2) & 0x3f) << 5) | (((r >> 3) & 0x1f) << 11))

const Color BLACK_WHITE_PALETTE_GAME[] = {

    (Color) MAKECOLOR(0,0,0), // 0x0
    (Color) MAKECOLOR(170,170,170), // 0x2
    (Color) MAKECOLOR(255,255,255), // 0x1
    (Color) MAKECOLOR(181,181,181), // 0x3
    (Color) MAKECOLOR(0,0,0), // 0x4
    (Color) MAKECOLOR(255,0,255), // 0x5
    (Color) 0x0000, // 0x6
    (Color) 0x0000, // 0x7
    (Color) 0x0000, // 0x8
    (Color) 0x0000, // 0x9
    (Color) 0x0000, // 0xa
    (Color) 0x0000, // 0xb
    (Color) 0x0000, // 0xc
    (Color) 0x0000, // 0xd
    (Color) 0x0000, // 0xe
    (Color) 0x0000  // 0xf

};

const Color BLACK_WHITE_PALETTE_TITLE[] = {

    (Color) MAKECOLOR(0,0,0), // 0x0
    (Color) MAKECOLOR(10,10,10), // 0x1
    (Color) MAKECOLOR(111,111,111), // 0x2
    (Color) MAKECOLOR(181,181,181), // 0x3
    (Color) MAKECOLOR(0,0,0), // 0x4
    (Color) MAKECOLOR(255,0,255), // 0x5
    (Color) 0x0000, // 0x6
    (Color) 0x0000, // 0x7
    (Color) 0x0000, // 0x8
    (Color) 0x0000, // 0x9
    (Color) 0x0000, // 0xa
    (Color) 0x0000, // 0xb
    (Color) 0x0000, // 0xc
    (Color) 0x0000, // 0xd
    (Color) 0x0000, // 0xe
    (Color) 0x0000  // 0xf

};

const Color GBC_PALETTE_GAME[] = {

    (Color) MAKECOLOR(128,128,128), // 0x0
    (Color) MAKECOLOR(0,0,128), // 0x1
    (Color) MAKECOLOR(0,0,255), // 0x2
    (Color) MAKECOLOR(0,0,64), // 0x3
    (Color) MAKECOLOR(0,0,0), // 0x4
    (Color) MAKECOLOR(255,0,255), // 0x5
    (Color) 0x0000, // 0x6
    (Color) 0x0000, // 0x7
    (Color) 0x0000, // 0x8
    (Color) 0x0000, // 0x9
    (Color) 0x0000, // 0xa
    (Color) 0x0000, // 0xb
    (Color) 0x0000, // 0xc
    (Color) 0x0000, // 0xd
    (Color) 0x0000, // 0xe
    (Color) 0x0000  // 0xf

};

const Color GBC_PALETTE_TITLE[] = {

    (Color) MAKECOLOR(128,128,128), // 0x0
    (Color) MAKECOLOR(0,0,255), // 0x1
    (Color) MAKECOLOR(0,0,128), // 0x2
    (Color) MAKECOLOR(0,0,64), // 0x3
    (Color) MAKECOLOR(0,0,0), // 0x4
    (Color) MAKECOLOR(255,0,255), // 0x5
    (Color) 0x0000, // 0x6
    (Color) 0x0000, // 0x7
    (Color) 0x0000, // 0x8
    (Color) 0x0000, // 0x9
    (Color) 0x0000, // 0xa
    (Color) 0x0000, // 0xb
    (Color) 0x0000, // 0xc
    (Color) 0x0000, // 0xd
    (Color) 0x0000, // 0xe
    (Color) 0x0000  // 0xf

};

const Color RED_PALETTE_GAME[] = {

    (Color) MAKECOLOR(255,128,128), // 0x0
    (Color) MAKECOLOR(128,0,0), // 0x1
    (Color) MAKECOLOR(255,0,0), // 0x2
    (Color) MAKECOLOR(64,0,0), // 0x3
    (Color) MAKECOLOR(0,0,0), // 0x4
    (Color) MAKECOLOR(255,0,255), // 0x5
    (Color) 0x0000, // 0x6
    (Color) 0x0000, // 0x7
    (Color) 0x0000, // 0x8
    (Color) 0x0000, // 0x9
    (Color) 0x0000, // 0xa
    (Color) 0x0000, // 0xb
    (Color) 0x0000, // 0xc
    (Color) 0x0000, // 0xd
    (Color) 0x0000, // 0xe
    (Color) 0x0000  // 0xf

};

const Color RED_PALETTE_TITLE[] = {

    (Color) MAKECOLOR(255,128,128), // 0x0
    (Color) MAKECOLOR(255,0,0), // 0x1
    (Color) MAKECOLOR(128,0,0), // 0x2
    (Color) MAKECOLOR(64,0,0), // 0x3
    (Color) MAKECOLOR(0,0,0), // 0x4
    (Color) MAKECOLOR(255,0,255), // 0x5
    (Color) 0x0000, // 0x6
    (Color) 0x0000, // 0x7
    (Color) 0x0000, // 0x8
    (Color) 0x0000, // 0x9
    (Color) 0x0000, // 0xa
    (Color) 0x0000, // 0xb
    (Color) 0x0000, // 0xc
    (Color) 0x0000, // 0xd
    (Color) 0x0000, // 0xe
    (Color) 0x0000  // 0xf

};


const Color GB_PALETTE[] = {

    (Color) 0xe765, // 0x0
    (Color) 0x3dc8, // 0x1
    (Color) 0x23a6, // 0x2
    (Color) 0x01c5, // 0x3
    (Color) 0x0000, // 0x4
    (Color) 0xf81f, // 0x5
    (Color) 0x0000, // 0x6
    (Color) 0x0000, // 0x7
    (Color) 0x0000, // 0x8
    (Color) 0x0000, // 0x9
    (Color) 0x0000, // 0xa
    (Color) 0x0000, // 0xb
    (Color) 0x0000, // 0xc
    (Color) 0x0000, // 0xd
    (Color) 0x0000, // 0xe
    (Color) 0x0000  // 0xf

};

uint32_t getRandomSeed()
{
  return micros() * micros() ^ analogRead(1)*analogRead(2);
}

void setPalIndex(uint8_t index)
{
  palIndex = index;
}

uint8_t getPalIndex()
{
  return palIndex;
}

void setPaletteTitle()
{
  switch(palIndex)
  {
    case 0: gb.display.setPalette(GB_PALETTE); break;
    case 1: gb.display.setPalette(BLACK_WHITE_PALETTE_TITLE); break;
    case 2: gb.display.setPalette(GBC_PALETTE_TITLE); break;
    case 3: gb.display.setPalette(RED_PALETTE_TITLE); break;
  }
}

void setPaletteGame()
{
  switch(palIndex)
  {
    case 0: gb.display.setPalette(GB_PALETTE); break;
    case 1: gb.display.setPalette(BLACK_WHITE_PALETTE_GAME); break;
    case 2: gb.display.setPalette(GBC_PALETTE_GAME); break;
    case 3: gb.display.setPalette(RED_PALETTE_GAME); break;
  }
}

void set_bkg_tile_xy(uint8_t x, uint8_t y, uint8_t tile)
{
  if(currentTiles != nullptr)
  {
    currentTiles->setFrame(tile);
    gb.display.drawImage(x * 8, y * 8, *currentTiles);
  }
}

void set_bkg_data(Image *tiles)
{
  currentTiles = tiles;
}

Image* get_bkg_data()
{
  return currentTiles;
}

void set_bkg_tiles(uint8_t x, uint8_t y, Image *map)
{
  gb.display.drawImage(x, y,  *map);
}

void setBlockTilesAsBackground()
{
  set_bkg_data(&blockTiles);
}