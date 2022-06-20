#include <stdint.h>
#include <Gamebuino-Meta.h>

#include "level.h"
#include "commonvars.h"
#include "helperfuncs.h"
#include "sound.h"
#include "printfuncs.h"
#include "cursor.h"
#include "savestate.h"
#include "palettes.h"

uint8_t paused, wasMusicOn, wasSoundOn,redrawLevelDoneBit;

void drawGame()
{
    //background
    if(!paused && !redrawLevelDoneBit)
    {
        //gb.display.clear(INDEX_BLACK);
        gb.display.drawImage(0, 0, gameBackgroundMap);
         //LEVEL:
        printMessage(maxBoardBgWidth, 0, F("LEVEL:"));
        
        //[LEVEL NR] 2 chars
        printNumber(maxBoardBgWidth + 4, 1, selectedLevel, 2);
        
        
        //MOVES:
        printMessage(maxBoardBgWidth, 2, F("MOVES:"));

        printNumber(maxBoardBgWidth + 1, 3, moves, 5); 
        
        //A:XXXXXX (XXXXXX="ROTATE" or XXXXXX="SLIDE " or XXXXXX="ROSLID")
        switch (gameMode)
        {
            case gmRotate:
                printMessage(maxBoardBgWidth, 4, F("a:"));
                printMessage(maxBoardBgWidth, 5, F("ROTATE"));
                break;
            case gmSlide:
                printMessage(maxBoardBgWidth, 4, F("a:"));
                printMessage(maxBoardBgWidth, 5, F("SLIDE"));
                break;
            case gmRotateSlide:
                printMessage(maxBoardBgWidth, 4, F("a:"));
                printMessage(maxBoardBgWidth, 5, F("ROSLID"));
                break;
        }

        //B:BACK
        printMessage(maxBoardBgWidth, 6, F("b:"));
        printMessage(maxBoardBgWidth, 7, F("BACK"));

        //Draw arrows for vertical / horizontal movement
        if(gameMode != gmRotate)
        {
            
            for (uint8_t x = 0; x != boardWidth; x++)
            {
                set_bkg_tile_xy(boardX + x, boardY -1, arrowDown);
                set_bkg_tile_xy(boardX + x, boardY + boardHeight, arrowUp);
            }

            for (uint8_t y = 0; y != boardHeight; y++)
            {
                set_bkg_tile_xy(boardX - 1, boardY + y, arrowRight);
                set_bkg_tile_xy(boardX + boardWidth, boardY + y, arrowLeft);
            }
        }

        //level
        uint16_t i16 = 0; 
        for (uint8_t y = 0; y < boardHeight; y++)
        {
            for(uint8_t x = 0; x <boardWidth; x++)
            {
                set_bkg_tile_xy(boardX +x , boardY + y, level[i16 + x]);
            }
            i16+=boardWidth;
        }          
    }
}

void initGame()
{
    paused = 0;
    SelectMusic(musGame);
    setPaletteGame();
    //set background tiles
    setBlockTilesAsBackground();
    //set sprite for selector / cursor
    initCursors();
    setCursorPos(0, boardX + selectionX, boardY + selectionY);
    showCursors();
    redrawLevelDoneBit = 0;
}

void doPause()
{
    paused = 1;
    wasSoundOn = isSoundOn();
    wasMusicOn = isMusicOn();
    setMusicOn(0);
    setSoundOn(0);
    hideCursors();
    gb.display.setColor(INDEX_BLACK);
    gb.display.fillRect(0, ((maxBoardBgHeight >> 1) - 3) * 8, 16*8, 6*8);
    printMessage(0, (maxBoardBgHeight >> 1) - 3, F("[**************]"));
    printMessage(0, (maxBoardBgHeight >> 1) - 2, F("|PLEASE CONFIRM+")); 
    printMessage(0, (maxBoardBgHeight >> 1) - 1, F("|              +")); 
    printMessage(0, (maxBoardBgHeight >> 1) + 0, F("|   a PLAY     +"));
    printMessage(0, (maxBoardBgHeight >> 1) + 1, F("|   b TO QUIT  +"));
    printMessage(0, (maxBoardBgHeight >> 1) + 2, F("<##############>"));   
}

void doUnPause()
{
    paused = 0;
    setMusicOn(wasMusicOn);
    setSoundOn(wasSoundOn);
    setCursorPos(0, boardX + selectionX, boardY + selectionY);
    showCursors();
    needRedraw = 1;
}

void game()
{
    if(gameState == gsInitGame)
    {
        initGame();
        gameState -= gsInitDiff;
    }

    if(needRedraw)
    {
        drawGame();
        drawCursors();
        needRedraw = 0;
    }
       
    needRedraw = updateCursorFrame();

    if (gb.buttons.released(BUTTON_DOWN))
    {
        if(!levelDone && !paused)
        {
            playGameMoveSound();
            //if not touching border on bottom
            if (selectionY + 1 < boardHeight + posAdd)
            {
                selectionY += 1;
                needRedraw = 1;
            }
            else
            //set to border on top
            {
                selectionY = -posAdd;
                needRedraw = 1;
            }
            setCursorPos(0, boardX + selectionX, boardY + selectionY);
        }
    } 

    if (gb.buttons.released(BUTTON_UP))
    {
        if (!levelDone && !paused)
        {
            //if not touching border on top
            playGameMoveSound();
            if (selectionY -1 >= -posAdd)
            {
                selectionY -= 1;
                needRedraw = 1;
            }
            else
            //set to border on bottom
            {
                selectionY = boardHeight -1 +posAdd;
                needRedraw = 1;
            }
            setCursorPos(0, boardX + selectionX, boardY + selectionY);
        }
    }

    if (gb.buttons.released(BUTTON_RIGHT))
    {
        if (!levelDone && !paused)
        {
            playGameMoveSound();
            //if not touching border on right
            if(selectionX + 1 < boardWidth + posAdd)
            {
                selectionX += 1;
                needRedraw = 1;
            }
            else
            //set to border on left
            {
                selectionX = -posAdd;
                needRedraw = 1;
            }
            setCursorPos(0, boardX + selectionX, boardY + selectionY);
        }
    }

    if (gb.buttons.released(BUTTON_LEFT))
    {
        if(!levelDone && !paused)
        {
            playGameMoveSound();
            //if not touching border on left
            if( selectionX -1 >= -posAdd)
            {
                selectionX -= 1;
                needRedraw = 1;
            }
            //set to border on right
            else
            {
                selectionX = boardWidth -1 + posAdd;
                needRedraw = 1;
            }
            setCursorPos(0, boardX + selectionX, boardY + selectionY);
        }
    }

    if (gb.buttons.released(BUTTON_A))
    {
        if(paused)
        {
            doUnPause();
            playMenuAcknowlege();
            needRedraw = 1;
        }
        else
        {
            if(!levelDone)
            {
                if ((selectionX > -1) && (selectionX < boardWidth) &&
                    (selectionY > -1) && (selectionY < boardHeight))
                {   
                    if (gameMode != gmSlide)
                    {
                        rotateBlock((uint8_t)selectionX + ((uint8_t)selectionY * boardWidth));
                        moves++;
                        playGameAction();
                        needRedraw = 1;
                    }
                    else
                    {
                        playErrorSound();
                    }
                }
                else
                {
                    if ((selectionX > -1) && (selectionX < boardWidth))
                    {
                        if (selectionY == -1)
                        {
                            moveBlockDown((uint8_t)selectionX + ((uint8_t)(selectionY+1) * boardWidth));
                            moves++;
                            playGameAction();
                            needRedraw = 1;
                        }
                        else
                        {
                            if (selectionY == boardHeight)
                            {
                                moveBlockUp((uint8_t)selectionX + ((uint8_t)(selectionY-1) * boardWidth));
                                moves++;
                                playGameAction();
                                needRedraw = 1;
                            }
                        }
                    }
                    else
                    {
                        if ((selectionY > -1) && (selectionY < boardHeight))    
                        {
                            if (selectionX == -1)
                            {
                                moveBlockRight((uint8_t)(selectionX + 1) + ((uint8_t)selectionY * boardWidth));
                                moves++;
                                playGameAction();
                                needRedraw = 1;
                            }
                            else
                            {
                                if (selectionX == boardWidth)
                                {
                                    moveBlockLeft( (uint8_t)(selectionX - 1) + ((uint8_t)selectionY * boardWidth));
                                    moves++;
                                    playGameAction();
                                    needRedraw = 1;
                                }
                            }
                        }
                        else
                        {
                            playErrorSound();
                        }
                    }
                }
                updateConnected();
                levelDone = isLevelDone();
                if(levelDone)
                {
                    //update level one last time so we are at final state
                    //as it won't be updated anymore as long as level done is displayed
                    //1 forces level to be drawn (only) one last time the other call uses levelDone                      
                    drawGame();
                    SelectMusic(musLevelClear);
                    //hide cursor it's only sprite we use
                    hideCursors();
                    gb.display.setColor(INDEX_BLACK);
                    gb.display.fillRect(((16 - 13) >> 1) * 8, ((maxBoardBgHeight >> 1) - 2) * 8, 14*8, 5*8);
                    printMessage(((16 - 13) >> 1), (maxBoardBgHeight >> 1) - 2, F("[************]"));
                    printMessage(((16 - 13) >> 1), (maxBoardBgHeight >> 1) - 1, F("| LEVEL DONE +"));
                    printMessage(((16 - 13) >> 1), (maxBoardBgHeight >> 1)    , F("|            +"));
                    printMessage(((16 - 13) >> 1), (maxBoardBgHeight >> 1) + 1, F("| a CONTINUE +"));
                    printMessage(((16 - 13) >> 1), (maxBoardBgHeight >> 1) + 2, F("<############>"));
                    redrawLevelDoneBit = 1; 
                }
            }
            else
            {
                redrawLevelDoneBit = 0;
                //goto next level
                if (difficulty == diffRandom)
                {
                    //ned new seed based on time
                    randomSeedGame = getRandomSeed();
                    initLevel(randomSeedGame);
                    SelectMusic(musGame);
                    //show cursor again (it's actually to early but i'm not fixing that)
                    setCursorPos(0, boardX + selectionX, boardY + selectionY);
                    showCursors();
                    needRedraw = 1;
                }
                else
                {   
                    //goto next level if any
                    if (selectedLevel < maxLevel)
                    {
                        selectedLevel++;
                        unlockLevel(gameMode, difficulty, selectedLevel-1);
                        initLevel(randomSeedGame);
                        SelectMusic(musGame);
                        //show cursor again (it's actually to early but i'm not fixing that)
                        setCursorPos(0, boardX + selectionX, boardY + selectionY);
                        showCursors();
                        needRedraw = 1;
                    }
                    else //Goto some congrats screen
                    {
                        gameState = gsInitLevelsCleared;                         
                    }
                }
            }
        }
    }

    if(gb.buttons.released(BUTTON_B)) 
    {
        if(!levelDone)
        {
            if(!paused)
            {
                playMenuBackSound();
                doPause();
                needRedraw = 1;
            }
            else
            {
                //need to enable early again to play backsound
                //normally unpause does it but we only unpause
                //after fade
                setSoundOn(wasSoundOn);
                hideCursors();
                playMenuBackSound();
                gameState = gsInitLevelSelect;
                doUnPause();
                //unpause sets cursor visible !
                hideCursors();
                //need to reset the level to initial state when going back to level selector
                //could not find a better way unfortunatly
                //also we do not want to reset the randomseed used for random level generating
                //or a new level would have been created when going back we only want the level
                //with random to change when pressing left and right in the level selector
                //this way it stays consistent with the normal levels
                //and the player can replay the level if he wants to
                initLevel(randomSeedGame);
            }
        }
    }
}
