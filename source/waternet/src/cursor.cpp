#include <stdint.h>

#include "cursor.h"
#include "commonvars.h"
#include "helperfuncs.h"
#include "../res/selectortiles.h"

constexpr uint8_t maxCursors = 4;
constexpr uint8_t maxCursorFrameCount = (uint8_t)(10 * frameRate / 60);
constexpr uint8_t cursorAnimCount = 2; //blink on & off
constexpr uint8_t cursorNumTiles = maxCursors * 8; //for the max 4 cursors shown at once (on help screens) 

uint8_t cursorFrameCount, cursorFrame, showCursor;
uint8_t spritePos[cursorNumTiles][2];

void move_sprite(uint8_t sprite, uint8_t x, uint8_t y)
{
    spritePos[sprite][0] = x;
    spritePos[sprite][1] = y;
}

void drawCursors()
{
    if((showCursor == 0) || (cursorFrame & 1)) // 2nd or to add blink effect, it will skip drawing if bit 1 is set
        return;
    
    for (uint8_t i=0; i<cursorNumTiles; i++)
        if (spritePos[i][1] < gb.display.height())
        {
            selectorTiles.setFrame(i%8);
            gb.display.drawImage(spritePos[i][0], spritePos[i][1], selectorTiles);
        }
}

//returns 1 if cursor has changed / needs redraw
uint8_t updateCursorFrame()
{
    cursorFrameCount++;
    if (cursorFrameCount >= maxCursorFrameCount)
    {        
        cursorFrame++;
        cursorFrameCount = 0;
        if (cursorFrame >= cursorAnimCount)
            cursorFrame = 0;
        return 1; 
    }
    return 0;
}

void hideCursors()
{
    //HIDE CURSOR SPRITES
    for (uint8_t i = 0; i < maxCursors; i++)
        setCursorPos(i, 0, (gb.display.height() / 8) + 1);
    
    showCursor = 0;
}

void showCursors()
{
    showCursor = 1;
}

void setCursorPos(uint8_t cursorNr, uint8_t xPos, uint8_t yPos)
{
    if (cursorNr >= maxCursors)
        return;

    move_sprite((cursorNr<<3) + 0, ((xPos) << 3),  ((yPos - 1) << 3));
    move_sprite((cursorNr<<3) + 1,  ((xPos + 1) << 3),  ((yPos) << 3));
    move_sprite((cursorNr<<3) + 2,  ((xPos) << 3),  ((yPos + 1) << 3));
    move_sprite((cursorNr<<3) + 3,  ((xPos - 1) << 3),  ((yPos) << 3)); 
    //corners
    move_sprite((cursorNr<<3) + 4, ((xPos + 1) << 3),  ((yPos - 1) << 3));
    move_sprite((cursorNr<<3) + 5, ((xPos + 1) << 3),  ((yPos + 1) << 3));
    move_sprite((cursorNr<<3) + 6, ((xPos - 1) << 3),  ((yPos - 1) << 3));
    move_sprite((cursorNr<<3) + 7, ((xPos - 1) << 3),  ((yPos + 1) << 3)); 
}

void initCursors()
{
    hideCursors();

    cursorFrameCount = 0;
    cursorFrame = 0;
}
