#pragma once

#if EXTERNAL_GAME_ACTIVITY_CODE
    #include "game-activity/GameActivity.h"

    #define GAMEACTIVITY_CALCULATE_VERSION(Major, Minor, BugFix) ((Major << 16) | (Minor << 8) | (BugFix))

    #if defined(GAMEACTIVITY_MAJOR_VERSION) && defined(GAMEACTIVITY_MINOR_VERSION)
        #define GAMEACTIVITY_VERSION_CURRENT GAMEACTIVITY_CALCULATE_VERSION(GAMEACTIVITY_MAJOR_VERSION, GAMEACTIVITY_MINOR_VERSION, GAMEACTIVITY_BUGFIX_VERSION)
    #else
        #define GAMEACTIVITY_VERSION_CURRENT GAMEACTIVITY_CALCULATE_VERSION(1, 1, 0)
    #endif

    // Note: In GameActivity.h GAMEACTIVITY_BUGFIX_VERSION for version 1.2.2 is 0
    //       Change (1, 2, 0) -> (1, 2, 2) once Google fixes it
    #define GAMEACTIVITY_VERSION_1_2_2 GAMEACTIVITY_CALCULATE_VERSION(1, 2, 0)
#endif
