#include "graphics.h"
#include "oled.h"
#include "adc.h"
#include <util/delay.h>

#define EXT_RAM 0x1400 // External ram base address

static char player_name[4] = "___";   // 3 letters + null terminator
static int name_index = 0;            // 0–2
static int alphabet_index = 0;        // 0–25
static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

volatile uint16_t game_seconds = 0;
volatile uint8_t game_running = 0;   // 1 = active, 0 = ball scored

static position currentPosition = PLAY;

char updateFlag = 0;

// High score
static highscore_t highscores[5] = {
    {"___", 0}, {"___", 0}, {"___", 0}, {"___", 0}, {"___", 0}
};

void main_menu(pos_t *joystick_pos, char button, menu_state *state);
void name_menu(pos_t *joystick_pos, pos_t *slider_pos, char button, menu_state *state);
void game_menu(pos_t *joystick_pos, pos_t *slider_pos, char button, menu_state *state, uint8_t * goal);
void score_menu(pos_t *joystick_pos, pos_t *slider_pos, char button, menu_state *state);



void menu(pos_t *joystick_pos, pos_t *slider_pos, char button, uint8_t * goal,  menu_state * state) {
    switch (*state) {
        case MAINMENU:
            main_menu(joystick_pos, button, state);
            break;
        case NAME:
            name_menu(joystick_pos, slider_pos, button, state);
            break;
        case GAME:
            game_menu(joystick_pos, slider_pos, button, state, goal);
            break;
        case SCORE:
            score_menu(joystick_pos, slider_pos, button, state);
            break;
    }
}


void main_menu(pos_t *joystick_pos, char button, menu_state *state){
    char *menuTitle = "MENU";
    char *playButton;
    char *exitButton;
    int exitPosition;
    int playPosition;
    //printf("Joystick position:  X:%3d\t  Y:%3d\r\n", joystick_pos->x, joystick_pos->y);
    
    switch (currentPosition) {
        case PLAY:
            playButton = "-> PLAY";
            exitButton = "   EXIT";
            playPosition = (50-(6*3));
            exitPosition = 50;
            if ((button % 2) == 1){
                *state = NAME;
                _delay_ms(50); // Debounce delay
            }
            else if (joystick_pos->y < -10) {
                currentPosition = EXIT;
            }
            break;
        case EXIT:
            playButton = "   PLAY";
            exitButton = "-> EXIT";
            playPosition = 50;
            exitPosition = (50-(6*3));
            if (joystick_pos->y > 10) {
                currentPosition = PLAY;
            }
            break;

    }
    //oled_clear();
    oled_printf(menuTitle, 45, 0, LARGE);
    oled_printf(playButton, (50-(6*3)), 2, NORMAL);
    oled_printf(exitButton, (50-(6*3)), 4, NORMAL);
}

void name_menu(pos_t *joystick_pos, pos_t *slider_pos, char button, menu_state *state)
{
    // --- Movement: LEFT / RIGHT ---
    if (joystick_pos->x > 10) {
        alphabet_index++;
        if (alphabet_index > 25) alphabet_index = 25;
    }
    else if (joystick_pos->x < -10) {
        alphabet_index--;
        if (alphabet_index < 0) alphabet_index = 0;
    }

    // --- Select letter ---
    if (button % 2 == 1) {
        if (name_index < 3) {
            player_name[name_index] = alphabet[alphabet_index];
            name_index++;

            if (name_index == 3) {
                // Name finished → next menu
                *state = GAME;
                game_running = 1;
                game_seconds = 0;
                _delay_ms(50); // Debounce delay
            }
        }
    }

    // --- Drawing ---
   frame_clear();

    // Title
    oled_printf("ENTER NAME", 20, 0, NORMAL);

    // Current name (___ or A_B)
    oled_printf(player_name, 40, 2, LARGE);

    // Alphabet row
    // Each character is 6px wide in NORMAL font
    int start_x = 5;

    for (int i = 0; i < 26; i++) {
        int x = start_x + i * 6;

        if (i == alphabet_index) {
            // Highlight: arrow before the selected letter
            oled_printf(">", x - 6, 5, NORMAL);
        }

        char letter[2] = { alphabet[i], '\0' };
        oled_printf(letter, x, 5, NORMAL);
    }
}


void game_menu(pos_t *joystick_pos, pos_t *slider_pos, char button, menu_state *state, uint8_t * goal){
    char time_str[16];
    char buffer[16];
    uint16_t sec = game_seconds;
    uint8_t minutes = sec / 60;
    uint8_t seconds = sec % 60;
    frame_clear();
    oled_printf("GAME RUNNING", 20, 0, NORMAL);
    oled_printf("Time:", 10, 2, NORMAL);
    snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);
    oled_printf(buffer, 45, 3, LARGE);
    //printf("Goal state: %d\r\n", *goal);
    if (*goal == 1) {
        game_running = 0;
        oled_printf("BALL SCORED!", 20, 5, NORMAL);
        oled_printf("Press button", 15, 6, NORMAL);
        oled_printf("to continue", 20, 7, NORMAL);
        if (button % 2 == 1) {
            *state = SCORE;
            updateFlag = 0;
            *goal = 0;
            _delay_ms(50); // Debounce delay
        }
    }
}

void leaderboard_insert(const char *name, uint16_t score)
{
    // Find insert position
    int pos = -1;
    for (int i = 0; i < 5; i++) {
        if (score > highscores[i].score) { // higher score is better
            pos = i;
            break;
        }
    }

    // If not in top 5 → ignore
    if (pos == -1)
        return;

    // Shift lower scores down
    for (int j = 4; j > pos; j--) {
        highscores[j] = highscores[j - 1];
    }

    // Insert new score
    strncpy(highscores[pos].name, name, 3);
    highscores[pos].name[3] = '\0';
    highscores[pos].score = score;
}


void score_menu(pos_t *joystick_pos, pos_t *slider_pos, char button, menu_state *state)
{
    if (updateFlag == 0) {
        uint16_t score = game_seconds;

        // Update high scores using global player_name
        leaderboard_insert(player_name, score);

        frame_clear();
        updateFlag = 1;

        oled_printf("HIGH SCORES", 10, 0, NORMAL);

        for (int i = 0; i < 5; i++) {
            char line[20];
            snprintf(line, sizeof(line), "%d. %s - %d",
                     i + 1, highscores[i].name, highscores[i].score);
            oled_printf(line, 5, 2 + i, NORMAL);
        }
    }

    if (button % 2 == 1) {
        // Return to main menu
        *state = MAINMENU;

        // Reset name entry state for next game
        name_index = 0;
        player_name[0] = player_name[1] = player_name[2] = '_';
        player_name[3] = '\0';

        // Reset score update flag
        updateFlag = 0;
        _delay_ms(50); // Debounce delay
        frame_clear();
    }
}