//displays text when a player changes areas, npcs leave comments and shows things like health/ammo in the outskirts etc.
//this differs from menus in that information is only overlaid and movement is never compromised
#include "32blit.hpp"

#include "logic_globals.h"
#include "../engine/render_globals.h"
#include "../engine/chunk_globals.h"

using namespace blit;

static const Font &font = minimal_font; // TODO

#define INFO_TIMER 120 //display any info for around 3 seconds

extern int32_t show_battery;

int32_t player_area = 0;

int32_t info_time_remain = 0;
int32_t info_display = 0;

int32_t dialogue_time_remain = 0;
int32_t dialogue_display = 0;

//check if the area where the player has changed from the previous frame
void logic_player_area() {

    //we get the array position of the center chunk where the player is
    int32_t chunk_x;
    int32_t chunk_y;

    chunk_locate(camera_position_fixed_point[0], camera_position_fixed_point[2], chunk_x, chunk_y);

    int32_t new_area;

    if (chunk_y >= 6) {
        new_area = AREA_OUTSKIRTS;
        if (chunk_y >= 10 && chunk_x < 3) {
            new_area = AREA_OUTSKIRT_STABLES;
        }
    } else if (chunk_x > 7) {
        new_area = AREA_YAKUZA_ALLEY;
    } else if (chunk_x < 5) {
        new_area = AREA_DOWNTOWN;
    } else {
        new_area = AREA_CITY_CENTER;
    }

    if (new_area != player_area) {
        info_time_remain = INFO_TIMER;
        player_area = new_area;
    }
}


void display_info() {

    //we only display ammo and health when the player is in the outskirts
    if (player_area == AREA_OUTSKIRTS) {

        #ifndef GAMESCOM
        if (player_health < 30) {
            screen.pen = {255, 0, 0};
        } else {
            screen.pen = {255, 255, 255};
        }

        screen.text("+" + std::to_string(player_health), font, {0, 0});

        if(player_ammo < 30) {
            screen.pen = {255, 0, 0};
        } else {
            screen.pen = {255, 255, 255};
        }

        //Position text depending on amount of bullets left
        if (player_ammo == 0) {
            screen.text("No Ammo", font, {75, 0});
        } else if (player_ammo > 99) {
            screen.text(std::to_string(player_ammo), font, {100, 0});
        } else if (player_ammo > 9) {
            screen.text(std::to_string(player_ammo), font, {107, 0});
        } else {
            screen.text(std::to_string(player_ammo), font, {114, 0});
        }


        //we only show score when shooting balloons
        #else

        screen.pen = {255, 255, 255};
        screen.text(std::to_string(player_kills), 0, 0);

        #endif


        //show cross hairs (or point) for shooting
        screen.pen = {255, 0, 0};
        screen.pixel({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
        screen.pixel({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 1});
        screen.pixel({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 2});
        screen.pixel({SCREEN_WIDTH / 2 - 1, SCREEN_HEIGHT / 2});
        screen.pixel({SCREEN_WIDTH / 2 - 2, SCREEN_HEIGHT / 2});
        screen.pixel({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 1});
        screen.pixel({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 2});
        screen.pixel({SCREEN_WIDTH / 2 + 1, SCREEN_HEIGHT / 2});
        screen.pixel({SCREEN_WIDTH / 2 + 2, SCREEN_HEIGHT / 2});
    }

    //show area name if needed
    screen.pen = {255, 255, 255};
    if (info_time_remain != 0) {
        if (player_area == AREA_OUTSKIRTS) {
            screen.text("Outskirts", font, {38, 20});
        } else if (player_area == AREA_YAKUZA_ALLEY) {
            screen.text("Back Alley", font, {35, 20});
        } else if (player_area == AREA_DOWNTOWN) {
            screen.text("Downtown", font, {38, 20});
        } else if (player_area == AREA_CITY_CENTER) {
            screen.text("City Center", font, {35, 20});
        } else if (player_area == AREA_OUTSKIRT_STABLES) {
            screen.text("Outskirt", font, {37, 20});
            screen.text("Stable", font, {40, 30});
        }
        info_time_remain--;
    }

    //show talk button if quest/shop npc is close
    if (close_npc != -1) {
        if (close_npc == 1 && (npc_quest_list[close_npc].dialogue == 11 || npc_quest_list[close_npc].dialogue == 12)) {
            screen.text("A: Buy Ammo", font, {0, 110});
        }else {
            screen.text("A: Talk", font, {0, 110});
        }
    }

    if (show_battery == 1) {
        //screen.text("Battery:" + std::to_string(battery()), font {60, 110}); // FIXME: not exposed through blit API
    }

    //show dialogue from npc
    if (dialogue_time_remain != 0) {

        #ifdef GAMESCOM
        switch(dialogue_display) {
            case 0: screen.text("Hey there!", font, {0, 100}); break;
            case 1: screen.text("It's pretty boring", font, {0, 90}); screen.text("being on guard duty", font, {0, 100});break;
            case 2: screen.text("So I set up some", font, {0, 90}); screen.text("balloons outside", font, {0, 100}); break;
            case 3: screen.text("See if you can", font, {0, 90}); screen.text("shoot them all down", font, {0, 100}); break;
            case 4: screen.text("The gates close", font, {0, 90}); screen.text("at night though", font, {0, 100}); break;
            case 5: screen.text("But the balloons", font, {0, 90}); screen.text("glow in the dark!", font, {0, 100}); break;
            case 6: if (player_kills < 100) {
                        screen.text("See if you can shoot", font, {0, 90}); 
                        screen.text("all the balloons down!", font, {0, 100}); break;
                    } else {
                        screen.text("Wow you got them all!", font, {0, 90}); 
                        screen.text("You're a balloon master!", font, {0, 100}); break;
                    }
        }
        #else
        switch(dialogue_display) {
            case 0: screen.text("Hey there! You seem", font, {0, 90}); screen.text("to be new around here.", font, {0, 100});break;
            case 1: screen.text("I am guarding the", font, {0, 90}); screen.text("city from zombies.", font, {0, 100});break;
            case 2: screen.text("Be careful if you", font, {0, 90}); screen.text("go out, they will attack.", font, {0, 100}); break;
            case 3: screen.text("The gates close at", font, {0, 90}); screen.text("night. Be back by then.", font, {0, 100}); break;
            case 4: screen.text("Zombies are more", font, {0, 90}); screen.text("aggressive in the dark.", font, {0, 100}); break;
            case 5: screen.text("Try not to run out", font, {0, 90}); screen.text("of ammo out there.", font, {0, 100}); break;
            case 6: if (player_kills <= 100) {
                        screen.text("See if you can kill", font, {0, 90}); 
                        screen.text("a couple of the zombies.", font, {0, 100}); break;
                    } else {
                        screen.text("Wow you got over a 100!", font, {0, 90}); 
                        screen.text("You're a zombie killer!", font, {0, 100}); break;
                    }

            
            case 10:screen.text("Need Bullets?", font, {0, 90}); screen.text("10$ for 5.", font, {0, 100});break;
            case 11:screen.text("Good hunting.", font, {0, 90}); screen.text(std::to_string(player_money + QUEST_AMMO_COST) + "$ -> " + std::to_string(player_money) + "$", font, {0, 100}); break;
            case 12:screen.text("You don't have", font, {0, 90}); screen.text("enough money.", font, {0, 100}); break;


            case 20: screen.text("Oh, well done for", font, {0, 90}); screen.text("making it here.", font, {0, 100});break;
            case 21: screen.text("The fire keeps the", font, {0, 90}); screen.text("zombies away.", font, {0, 100});break;
            case 22: screen.text("This place used to", font, {0, 90}); screen.text("have horses but...", font, {0, 100}); break;
            case 23: screen.text("I mean look at the", font, {0, 90}); screen.text("surroundings...", font, {0, 100}); break;
            case 24: screen.text("The stable owner is", font, {0, 90}); screen.text("pretty unhappy.", font, {0, 100}); break;
            case 25: screen.text("Sells ammo to anyone", font, {0, 90}); screen.text("hoping it will help.", font, {0, 100}); break;
            case 26: screen.text("Don't get killed and", font, {0, 90}); screen.text("get some rest here.", font, {0, 100}); break;
        }
        #endif

        dialogue_time_remain--;
    }

    

}
