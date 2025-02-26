// Player
#define INITIAL_X_POSITION 6
#define INITIAL_JUMP_POWER -1.2
#define MIN_Y_SPEED -8
#define MAX_Y_SPEED 15
#define INVULNERABLE_AFTER_HIT_DURATION 1;
#define MAX_INPUT_CHARS 15

#define GRAVITY 0.6


// Map
#define MAX_PATH_SIZE 64
#define MAP_HEIGHT 12
#define MAP_WIDTH 240
#define SECTION_WIDTH 30
#define TOTAL_SECTIONS MAP_WIDTH / SECTION_WIDTH
#define LASER_ACTIVATION_DELAY 3

// Levels
#define AMOUNT_OF_LEVELS 3
#define LEVEL_SPEED_MULTIPLIER 0.08
#define INITIAL_GRAVITY 0.6


// Screen
#define CELL_SIZE 64
#define SCREEN_WIDTH SECTION_WIDTH * CELL_SIZE
#define SCREEN_HEIGHT MAP_HEIGHT * CELL_SIZE


// UI
#define LINE_THICKNESS 5
#define RECTANGLE_WIDTH 500
#define RECTANGLE_HEIGHT 100
#define RECTANGLE_POSITION_X_CENTER (SCREEN_WIDTH - RECTANGLE_WIDTH ) / 2
#define BUTTON_WIDTH 500
#define BUTTON_HEIGHT 100
#define BUTTON_POSITION_X_CENTER (SCREEN_WIDTH - RECTANGLE_WIDTH ) / 2
#define BUTTON_LABEL_SIZE 40
#define ERROR_MESSAGE_LENGTH 128

// LEADERBOARD
#define MAX_SAVES 5
#define LEADERBOARD_FILE_PATH "leaderboard.bin"

// CHEATS
#define INVULNERABLE_CHEAT_CODE "GHOST"
#define SLOW_CHEAT_CODE "SLOW"


