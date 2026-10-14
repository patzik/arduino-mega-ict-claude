//
// Copyright (c) 2026, patzik
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include "CKikiKaikaiBaseGame.h"
#include "CZ80ACpu.h"
#include <DFR_Key.h>

//
// Probe Head GND:
//   Z80 GND Pin 29
//
// Watchdog Disable:
//   TBD.
//
// Board Designations:
//   This is the main CPU on the single main board (K1100195A). It is the
//   Z80 next to the A85_16.H18 & A85_17.H16 program ROMs.
//
// Z80 Compatibility Notes
//   * IO space is not used.
//   * M1 (1st machine cycle) is not used.
//
// Notes:
//   The memory map is taken from MAME (src/mame/taito/kikikai.cpp,
//   base_state::main_map) and the IC locations from the PCB layout in
//   the same file. MAME doesn't say which RAM IC is at which address so the
//   RAM regions are tested byte wide with no IC location given ("-").
//
//   0000-7FFF ROM H16
//   8000-BFFF ROM H18, banked 16KB at a time by F000 D0-D2
//   C000-E7FF RAM shared with the sound CPU (sound CPU 8000-A7FF)
//   E800-E8FF RAM shared with the MCU (MC6801U4 A85-01 at H8)
//   E900-EFFF RAM
//   F000      W  D0-D2 ROM bank, D5 character bank
//   F008      W  D0 flip (?), D1 MCU reset (0=reset), D2 sound CPU reset (0=reset)
//   F010      R  IN3 - service, tilt, start 1 & 2
//   F018      W  watchdog (?)
//   F800-FFFF RAM for the 4 player sub board link (not tested).
//
//   The joystick & coin inputs are read by the MCU and the DIP switches
//   by the YM2203 on the sound CPU, so they are not in the input list here.
//
//   The sound CPU & MCU are held in reset (F008 = 0) before the shared RAM
//   is tested so they don't interfere with it.
//
// void base_state::main_map(address_map &map)
//

static const UINT32 s_KikiKaikaiBankAddress  = 0xF000L;
static const UINT32 s_KikiKaikaiResetAddress = 0xF008L;
static const UINT32 s_KikiKaikaiMcuRamBase   = 0xE800L;

//
// RAM region is the same for all games on this board set.
//
static const RAM_REGION s_ramRegion[] PROGMEM = { //                                                                         "012", "012345"
                                                  {CKikiKaikaiBaseGame::onBankSwitchHoldResets, 0xC000, 0xE7FF, 1, 0xFF, " - ", "Shared"}, // RAM shared with the sound CPU
                                                  {CKikiKaikaiBaseGame::onBankSwitchHoldResets, 0xE800, 0xE8FF, 1, 0xFF, " - ", "McuRam"}, // RAM shared with the MCU
                                                  {CKikiKaikaiBaseGame::onBankSwitchHoldResets, 0xE900, 0xEFFF, 1, 0xFF, " - ", "Prog. "}, // RAM
                                                  {0}
                                                }; // end of list

//
// No write-only RAM on this platform.
//
static const RAM_REGION s_ramRegionWriteOnly[] PROGMEM = { {0} }; // end of list

//
// Input region is the same for all games on this board set.
//
static const INPUT_REGION s_inputRegion[] PROGMEM = { //                             "012", "012345"
                                                      {NO_BANK_SWITCH, 0xF010L, 0xFF, " - ", "IN3   "}, // Service, tilt, start
                                                      {0}
                                                    }; // end of list

//
// Output region is the same for all versions on this board set.
//
static const OUTPUT_REGION s_outputRegion[] PROGMEM = { //                                  "012", "012345"
                                                        {NO_BANK_SWITCH, 0xF000L, 0x27, 0x00, " - ", "RomBnk"}, // ROM bank (D0-D2) & char bank (D5)
                                                        {NO_BANK_SWITCH, 0xF008L, 0xFF, 0x00, " - ", "Resets"}, // D1 MCU reset, D2 sound CPU reset
                                                        {NO_BANK_SWITCH, 0xF018L, 0xFF, 0x00, " - ", "WD Res"}, // Watchdog (?)
                                                        {0}
                                                      }; // end of list

//
// No custom functions implemented for this game.
//
static const CUSTOM_FUNCTION s_customFunction[] PROGMEM = {{NO_CUSTOM_FUNCTION}}; // end of list


CKikiKaikaiBaseGame::CKikiKaikaiBaseGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CGame( romData2n,
           romRegion,
           s_ramRegion,
           s_ramRegion,
           s_ramRegionWriteOnly,
           s_inputRegion,
           s_outputRegion,
           s_customFunction)
{
    m_cpu = new CZ80ACpu();
    m_cpu->idle();

    // The VBLANK interrupt is on the INT pin.
    m_interrupt = ICpu::IRQ0;

    // The vector is supplied from the MCU shared RAM, see interruptCheck.
    m_interruptAutoVector = false;
    m_interruptResponse = 0x5A;
}


CKikiKaikaiBaseGame::~CKikiKaikaiBaseGame(
)
{
    delete m_cpu;
    m_cpu = (ICpu *) NULL;
}


//
// MAME supplies the interrupt vector from the first byte of the MCU shared
// RAM (mcram_vect_r) so a known vector is placed there first with the MCU
// held in reset. MAME also notes it isn't sure whether the MCU is involved in
// generating the VBLANK interrupt, so a failure here with the MCU held in
// reset isn't conclusive.
//
PERROR
CKikiKaikaiBaseGame::interruptCheck(
)
{
    PERROR error = onBankSwitchHoldResets(this);

    if (SUCCESS(error))
    {
        error = m_cpu->memoryWrite(s_KikiKaikaiMcuRamBase, m_interruptResponse);
    }

    if (SUCCESS(error))
    {
        error = CGame::interruptCheck();
    }

    return error;
}


PERROR
CKikiKaikaiBaseGame::onBankSwitchHoldResets(
    void *cKikiKaikaiBaseGame
)
{
    CKikiKaikaiBaseGame *thisGame = (CKikiKaikaiBaseGame *) cKikiKaikaiBaseGame;
    ICpu                *cpu      = thisGame->m_cpu;

    return cpu->memoryWrite(s_KikiKaikaiResetAddress, 0x00);
}


PERROR
CKikiKaikaiBaseGame::romBankSwitch(
    void *cKikiKaikaiBaseGame,
    UINT8 bank
)
{
    CKikiKaikaiBaseGame *thisGame = (CKikiKaikaiBaseGame *) cKikiKaikaiBaseGame;
    ICpu                *cpu      = thisGame->m_cpu;

    return cpu->memoryWrite(s_KikiKaikaiBankAddress, bank);
}


PERROR
CKikiKaikaiBaseGame::onBankSwitchRomBank0(
    void *cKikiKaikaiBaseGame
)
{
    return romBankSwitch(cKikiKaikaiBaseGame, 0);
}


PERROR
CKikiKaikaiBaseGame::onBankSwitchRomBank1(
    void *cKikiKaikaiBaseGame
)
{
    return romBankSwitch(cKikiKaikaiBaseGame, 1);
}


PERROR
CKikiKaikaiBaseGame::onBankSwitchRomBank2(
    void *cKikiKaikaiBaseGame
)
{
    return romBankSwitch(cKikiKaikaiBaseGame, 2);
}


PERROR
CKikiKaikaiBaseGame::onBankSwitchRomBank3(
    void *cKikiKaikaiBaseGame
)
{
    return romBankSwitch(cKikiKaikaiBaseGame, 3);
}

