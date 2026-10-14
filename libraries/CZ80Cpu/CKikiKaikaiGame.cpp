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
#include "CKikiKaikaiGame.h"

//
// ROM set, addresses & lengths are from MAME (src/mame/taito/kikikai.cpp).
//
// The 64KB H18 ROM is mapped in 16KB banks at 0x8000 so it is checked as
// four regions, each with its own CRC.
//
// TODO: The data2n samples and bank CRCs have not been generated (all 0) so
//       the ROM check will fail. Generate them from the ROM images.
//
static const ROM_DATA2N s_romData2nSet1[] PROGMEM  = { // 01   02   04   08   10   20   40   80  100  200  400  800 1000 2000 4000
                                                       {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // a85-17.h16
                                                       {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00     }, // a85-16.h18 bank 0
                                                       {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00     }, // a85-16.h18 bank 1
                                                       {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00     }, // a85-16.h18 bank 2
                                                       {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00     }, // a85-16.h18 bank 3
                                                       {0} }; // end of list

static const ROM_REGION s_romRegionSet1[] PROGMEM = { //
                                                      {NO_BANK_SWITCH,                            0x0000, 0x8000, s_romData2nSet1[0].data2n, 0x00000000, "H16"}, // a85-17.h16
                                                      {CKikiKaikaiBaseGame::onBankSwitchRomBank0, 0x8000, 0x4000, s_romData2nSet1[1].data2n, 0x00000000, "H18"}, // a85-16.h18 0x0000-0x3FFF
                                                      {CKikiKaikaiBaseGame::onBankSwitchRomBank1, 0x8000, 0x4000, s_romData2nSet1[2].data2n, 0x00000000, "H18"}, // a85-16.h18 0x4000-0x7FFF
                                                      {CKikiKaikaiBaseGame::onBankSwitchRomBank2, 0x8000, 0x4000, s_romData2nSet1[3].data2n, 0x00000000, "H18"}, // a85-16.h18 0x8000-0xBFFF
                                                      {CKikiKaikaiBaseGame::onBankSwitchRomBank3, 0x8000, 0x4000, s_romData2nSet1[4].data2n, 0x00000000, "H18"}, // a85-16.h18 0xC000-0xFFFF
                                                      {0} }; // end of list


IGame*
CKikiKaikaiGame::createInstanceSet1(
)
{
    return (new CKikiKaikaiGame(s_romData2nSet1, s_romRegionSet1));
}


CKikiKaikaiGame::CKikiKaikaiGame(
    const ROM_DATA2N *romData2n,
    const ROM_REGION *romRegion
) : CKikiKaikaiBaseGame( romData2n, romRegion )
{
}

