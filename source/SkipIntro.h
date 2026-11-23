#pragma once
#ifndef NFSMW_WIDESCREENFIX_SKIPINTRO_H
#define NFSMW_WIDESCREENFIX_SKIPINTRO_H

#include "MemoryTools.h"
#include <windows.h>
#include <cstdint>

namespace SkipIntro {
    // FEngMovieStarter::Callback check at 0x0055969B and global SkipMovies flag at 0x00926144
    constexpr uintptr_t HOOK_SKIP_MOVIES_CHECK = 0x0055969B;
    constexpr uintptr_t ADDR_SKIP_MOVIES_FLAG  = 0x00926144;

    inline uint32_t __cdecl ProcessSkipIntro() {
        static int counter = 0;
        static uint32_t originalFlag = 0;
        uint32_t* pSkipMovies = reinterpret_cast<uint32_t*>(ADDR_SKIP_MOVIES_FLAG);

        if (counter < 3) {
            if (counter == 0) {
                originalFlag = *pSkipMovies;
            }
            *pSkipMovies = 1;
            counter++;
        } else {
            *pSkipMovies = originalFlag;
        }

        return *pSkipMovies;
    }

    inline void Init() {
        // Replace 5-byte instruction 'mov eax, ds:0x00926144' with 'call ProcessSkipIntro'
        // Preserves the original 'test eax, eax' (0x005596A0) and 'jz loc_5596C0' (0x005596A2)
        MemoryTools::MakeCALL(HOOK_SKIP_MOVIES_CHECK, reinterpret_cast<uintptr_t>(ProcessSkipIntro));
    }
}

#endif // NFSMW_WIDESCREENFIX_SKIPINTRO_H
