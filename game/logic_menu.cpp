//display and handle different menus (main, shop and death etc.)
//menus usually remove control in some way from the player by deactivating normal movement
#include "32blit.hpp"

#include "logic_globals.h"

#include "assets.hpp"

using namespace blit;

static const Font &font = picosystem_font;

int32_t menu = MENU_START;

#ifdef BENCHMARK
extern int32_t benchmark_complete;
#endif

#ifdef FRAME_COUNTER
extern uint32_t perf_25_below;
extern uint32_t perf_50_below;
extern uint32_t perf_75_below;
extern uint32_t perf_75_above;
#endif

void display_menu() {

    if (menu == MENU_MAIN) {
        screen.pen = {255, 255, 255};

        screen.text("MENU:", font, {0, 0});

        screen.text("Health: " + std::to_string(player_health), font, {0, 20});
        screen.text("Ammo: " + std::to_string(player_ammo), font, {0, 30});
        screen.text("Kills: " + std::to_string(player_kills), font, {0, 40});
        screen.text("Money: " + std::to_string(player_money) + "$", font, {0, 50});
        //screen.text("Battery:" + std::to_string(battery()), font, {0, 60});

        //additional debug info if needed
        #ifdef FRAME_COUNTER
        screen.text("40:" + std::to_string(perf_25_below), font, {60, 0});
        screen.text("20:" + std::to_string(perf_50_below), font, {60, 10});
        screen.text("13:" + std::to_string(perf_75_below), font, {60, 20});
        screen.text("<13:" + std::to_string(perf_75_above), font, {60, 30});
        #endif

        //additional debug info if needed
        #ifdef DEBUG_INFO
        //amount of triangles passed on to Core1 for rasterization
        screen.text("#R: " + std::to_string(rendered_triangles), font, {0, 70});

        //amount of triangles stored in the chunk cache
        screen.text("#C: " + std::to_string(cached_triangles), font, {60, 70});

        //Core 0 logic (update) time
        screen.text("C0U:" + std::to_string(logic_time), font, {0, 80});
        #endif


        //Info Text to exit menu/change certain settings
        screen.text("UP/DOWN: Display(" + std::to_string(brightness) + ")", font, {0, 100});
        screen.text("Y: Exit", font, {0, 110});
        //screen.text("Y: Exit   A: Sound ON", font, {0, 110});

    //Start screen/splash screen
    } else if (menu == MENU_START) {
        screen.pen = {255, 255, 255};

        #ifdef BENCHMARK
        if (benchmark_complete == 0) {
            screen.text("BENCHMARKING", font, {0, 0});
        }
        #else
        if (demo_progress < 2500) {
            screen.text("Pico3D Engine", font, {28, 20});
            screen.text("by: Bernhard Strobl", font, {10, 30});
        }

        if ((demo_progress / 32) % 2 == 0) {
            screen.text("Press any button", font, {20, 90});
        }
        #endif

    } else if (menu == MENU_DEATH) {
        screen.pen = {255, 0, 0};
        screen.text("YOU DIED!", font, {38, 20});
    }

}