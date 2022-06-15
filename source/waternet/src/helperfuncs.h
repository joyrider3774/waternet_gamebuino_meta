#ifndef helperfuncs_h
#define helperfuncs_h

#include <Gamebuino-Meta.h>
#include <stdint.h>

uint32_t getRandomSeed();
uint8_t getPalIndex();
void setPalIndex(uint8_t index);
void setPaletteGame();
void setPaletteTitle();
void setBlockTilesAsBackground();
void set_bkg_tile_xy(uint8_t x, uint8_t y, uint8_t tile);
void set_bkg_data(Image *tiles);
Image* get_bkg_data();
void set_bkg_tiles(uint8_t x, uint8_t y, Image *map);
#endif