#include <string.h>
#include <stdint.h>
#include <Gamebuino-Meta.h>

#include "commonvars.h"
#include "palettes.h"

constexpr uint8_t soundOptionBit = 0U;
constexpr uint8_t musicOptionBit = 1U; 
constexpr uint8_t colorInverseBit = 2U;

uint32_t levelLocksPacked[3];
uint8_t options = 0; //bit 0 sound on/off, bit 1 music on/off
uint8_t activeColor = 0; 

int addrLevelLocksPacked, addrOptions;

uint8_t clearBit8(uint8_t val, uint8_t bit)
{
    if(bit > 7)
        return val;
    return val & (~(1U << bit));
}

uint8_t setBit8(uint8_t val, uint8_t bit)
{
    if (bit > 7)
        return val;
    return val | (1U << bit);
}

uint8_t checkBit8(uint8_t val, uint8_t bit)
 {
    if (bit > 7)
        return 0; 
    return ((val >> bit) & 1U);
 }

uint32_t clearBit32(uint32_t val, uint8_t bit)
{
    if(bit > 31)
        return val;
    return val & (~(1UL << bit));
}

uint32_t setBit32(uint32_t val, uint8_t bit)
{
    if (bit > 31)
        return val;
    return val | (1UL << bit);
}

uint8_t checkBit32(uint32_t val, uint8_t bit)
 {
    if (bit > 31)
        return 0; 
    return ((val >> bit) & 1UL);
 }

void packLevelLock(uint8_t mode, uint8_t diff, uint8_t level)
{
    uint8_t levelIndex = (mode * diffCount) + diff;
    uint8_t pack = levelIndex / 6; // 6 x 5 bit nr in a pack so have to divide levelindex by 6 to get packnr
    uint8_t bit = (levelIndex * 5) - (pack * 30); //6 x 5 bit nr in a pack so level index * 5 = bit nr acrross all packs - pack * 30 to know bit for current pack 
    for (uint8_t i = 0; i<5; i++)
    {
        if (checkBit8(level, i))
            levelLocksPacked[pack] = setBit32(levelLocksPacked[pack], bit + i);
        else
            levelLocksPacked[pack] = clearBit32(levelLocksPacked[pack], bit + i);
    }
}

uint8_t unPackLevelLock(uint8_t mode, uint8_t diff)
{
    uint8_t levelIndex = (mode * diffCount) + diff;
    uint8_t pack = levelIndex / 6; // 6 x 5 bit nr in a pack so have to divide levelindex by 6 to get packnr
    uint8_t bit = (levelIndex * 5) - (pack * 30); //6 x 5 bit nr in a pack so level index * 5 = bit nr acrross all packs - pack * 30 to know bit for current pack 
    uint8_t result = 0;
    for (uint8_t i = 0; i<5; i++)
    {
        if (checkBit32(levelLocksPacked[pack], bit+i))
            result = setBit8(result, i);
    }
    return result;
}
            
uint8_t validateSaveState()
{
    uint8_t levelsUnlocked;
    for (uint8_t j=0; j<gmCount; j++)
    {
        for (uint8_t i=0; i<diffCount; i++)
        {
            levelsUnlocked = unPackLevelLock(j, i);
            if ((levelsUnlocked == 0) || (levelsUnlocked > levelCount))
                return 0;
        }
    }
    if (options > 7) //bit 0 & 1 & 2 set = 7
        return 0;
    if (activeColor >  getMaxPalettes())
        return 0;
    return 1;
}


void initSaveState()
{
    //read eeprom  
    gb.save.get(0, levelLocksPacked , sizeof(levelLocksPacked) );
    gb.save.get(1, &options, sizeof(options));
    gb.save.get(2, &activeColor, sizeof(activeColor));

    if(!validateSaveState())
    { 
        levelLocksPacked[0] = 0UL;
        levelLocksPacked[1] = 0UL;
        levelLocksPacked[2] = 0UL;
        for (uint8_t j=0; j<gmCount; j++)
            for (uint8_t i=0; i<diffCount; i++)
                packLevelLock(j, i, 1U); //1st level unlocked
        options = 3; //bit 0 & 1 set = music & sound on
        activeColor = 4;
    }
}

void saveSaveState()
{
    gb.save.set(0, levelLocksPacked, sizeof(levelLocksPacked));
    gb.save.set(1, &options, sizeof(options));
    gb.save.set(2, &activeColor, sizeof(activeColor));
}

void setMusicOnSaveState(uint8_t value)
{
    if (value)
    {
        options = setBit8(options, musicOptionBit);
    }
    else
    {
        options = clearBit8(options, musicOptionBit);
    }
    saveSaveState();  
}

uint8_t isMusicOnSaveState()
{
    return checkBit8(options, musicOptionBit);
}

void setActiveColorSaveState(uint8_t value)
{
    activeColor = value;
    saveSaveState();
}

uint8_t getActiveColorSaveState()
{
    return activeColor;
}

void setSoundOnSaveState(uint8_t value)
{
    if (value)
        options = setBit8(options, soundOptionBit);
    else
        options = clearBit8(options, soundOptionBit);
    saveSaveState();
}

uint8_t isSoundOnSaveState()
{
    return checkBit8(options, soundOptionBit);
}

void setInverseColorSaveState(uint8_t value)
{
    if (value)
        options = setBit8(options, colorInverseBit);
    else
        options = clearBit8(options, colorInverseBit);
    saveSaveState();
}

uint8_t getInverseColorSaveState()
{
    return checkBit8(options, colorInverseBit);
}

uint8_t levelUnlocked(uint8_t mode, uint8_t diff, uint8_t level)
{
    return (unPackLevelLock(mode, diff) > level);
}

uint8_t lastUnlockedLevel(uint8_t mode, uint8_t diff)
{
    return unPackLevelLock(mode, diff);
}

void unlockLevel(uint8_t mode, uint8_t diff, uint8_t level)
{
    if (level + 1> lastUnlockedLevel(mode, diff))
    {
        packLevelLock(mode, diff, level + 1);
        saveSaveState();
    }
}