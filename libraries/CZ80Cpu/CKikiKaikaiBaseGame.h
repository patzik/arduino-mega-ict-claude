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
#ifndef CKikiKaikaiBaseGame_h
#define CKikiKaikaiBaseGame_h

#include "CGame.h"


class CKikiKaikaiBaseGame : public CGame
{
    public:

        //
        // CKikiKaikaiBaseGame
        //

        virtual PERROR interruptCheck(
        );

        //
        // Custom function to write the (suspected) watchdog continuously.
        //
        static PERROR watchdogKick(
            void *cKikiKaikaiBaseGame
        );

        //
        // Hold the sound CPU & MCU in reset so they don't access the shared RAM.
        //
        static PERROR onBankSwitchHoldResets(
            void *cKikiKaikaiBaseGame
        );

        //
        // Select the 16KB ROM bank mapped at 0x8000.
        //
        static PERROR onBankSwitchRomBank0(
            void *cKikiKaikaiBaseGame
        );

        static PERROR onBankSwitchRomBank1(
            void *cKikiKaikaiBaseGame
        );

        static PERROR onBankSwitchRomBank2(
            void *cKikiKaikaiBaseGame
        );

        static PERROR onBankSwitchRomBank3(
            void *cKikiKaikaiBaseGame
        );

        static PERROR onBankSwitchRomBank4(
            void *cKikiKaikaiBaseGame
        );

        static PERROR onBankSwitchRomBank5(
            void *cKikiKaikaiBaseGame
        );

    protected:

        CKikiKaikaiBaseGame(
            const ROM_DATA2N *romData2n,
            const ROM_REGION *romRegion
        );

        ~CKikiKaikaiBaseGame(
        );

    private:

        static PERROR romBankSwitch(
            void *cKikiKaikaiBaseGame,
            UINT8 bank
        );

};

#endif

