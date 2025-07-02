#ifndef GAME_H
#define GAME_H
#include <string>
#include <random>
#include "mode.h"
#include "grid.h"
#include "blocks.h"
#include "interface.h"

class Game: public Mode {
public:
    Game(float = (WINDOW_WIDTH - 710) / 2, float = 90, int = 0);
    virtual void Draw();
    virtual void HandleInput();
    static void ReadInput();
    static int keyPressed;
    virtual void Pause();
    virtual void Resume();
protected:
    // 1. basic implementation
    // get a random block from the member variable blocks
    Block GetRandomBlock();
    // returns all the tetrominoes
    std::vector<Block> GetAllBlocks();
    bool isKeyDown;
    void MoveBlockDown();
    void MoveBlockLeft();
    void MoveBlockRight();
    // checks if currentBlock(or ghostBlock) is outside the grid
    bool IsBlockOutside(bool isGhostBlock = false);
    void RotateBlockClockwise();
    void RotateBlockCounterclockwise();
    // save the currentBlock onto the grid and bring the nextBlock
    void LockBlock();
    // checks if all the cells of currentBlock are unoccupied
    bool BlockFits(bool isGhostBlock = false);
    // restart the game
    void Reset();
    // trigger function for gravity
    bool DropTriggered(double interval);
    // draw a box of size width * height with a title
    void DrawTextBox(const char* text, int x, int y, int width, int height);
    double lastDropTime;
    bool gameOver;
    int totalRowsCleared;
    int rowsCleared;
    Grid* grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock[3];
    // Random number generation members
    std::random_device rd;
    std::mt19937 rng;
    // Sound effects
    Sound actionSound;
    Sound difficultActionSound;
    Sound hardDropSound;
    Sound gameOverSound;
    Sound moveSound;
    Sound holdSound;
    Sound rotateSound;
    // true when an action is made after HandleInput()
    bool actionTrigger;
    // true when the game is finished
    bool gameOverTrigger;
    // 2. ghost block/hard drop implementation
    // updates the position of ghostBlock
    void PlaceGhostBlock();
    void HardDropBlock();
    Block ghostBlock;
    // distance between ghostBlock and currentBlock
    int distance;
    // distance value for score update (marathon)
    int hardDropDistance;
    float fontSize;
    // 3. hold/swap implementation
    // swap(or place) holdBlock with currentBlock
    void SwapBlock();
    Block holdBlock;
    bool isHoldEmpty;
    bool hasSwappedBlock;
    // 4. SRS wall kick implementation
    // checks for any wall kicks using SRS, returns false if clockwise rotation is invalid
    bool CheckClockwiseWallKick();
    bool CheckCounterclockwiseWallKick();
    // how often the currentBlock will drop due to gravity
    double dropPeriod;
    // 6. score UI/scoring implementation
    // trigger function for displaying actions (single, double, t-spin, etc.)
    bool ActionTriggered();
    double lastActionTime;
    // list of possible actions
    enum actions {
        SINGLE,
        DOUBLE,
        TRIPLE,
        TETRIS,
        TSNOLINES,
        TSS,
        TSD,
        TST,
        MTSNOLINES,
        MTSS,
        MTSD
    };
    std::vector<std::string> actionNames;
    // whether each action is difficult or not
    std::vector<bool> isDifficultAction;
    int actionType;
    int combo;
    // true when a combo is performed
    bool isCombo;
    bool previousDifficult;
    bool currentDifficult;
    // true when a back-to-back is performed
    bool isBacktoBack;
    // 7. T-spin implementation
    // checks if a valid T-spin is made (0: invalid, 1: T-Spin, 2: Mini T-Spin)
    int CheckValidTSpin();
    bool isLastManeuverRotation;
    // a mini t-spin is turned into a t-spin if the most difficult SRS wall kick was made
    bool isDifficultWallKick;
    int tSpinType;
    // 8. lock delay implementation
    // trigger function for lock delay after touching the ground
    bool LockDelayTriggered();
    double lastLockDelayTime;
    // true if the current block has ever touched the ground
    bool hasTouchedGround;
    // a maximum of 15 moves can be made to reset the lock delay timer
    int lockDelayResetCount;
    // 9. Multiplayer
    // variables for drawing the playfield
    float offsetX, offsetY;
    // variables for keyboard layout
    int keyBoardLayout;
    const int leftKey[3] = {KEY_LEFT, KEY_A, KEY_LEFT};
    const int rightKey[3] = {KEY_RIGHT, KEY_D, KEY_RIGHT};
    const int softDropKey[3] = {KEY_DOWN, KEY_S, KEY_DOWN};
    const int cwKey[6] = {KEY_UP, KEY_X, KEY_W, KEY_B, KEY_UP, KEY_PERIOD};
    const int ccwKey[3] = {KEY_Z, KEY_V, KEY_COMMA};
    const int hardDropKey[3] = {KEY_SPACE, KEY_SPACE, KEY_RIGHT_SHIFT};
    const int holdKey[3] = {KEY_C, KEY_N, KEY_SLASH};
    // variables for block locking detection
    bool lockBlockTrigger;
    // variable for implementing gravity at high levels
    int softDropDistance;
    // true when a block moves down due to gravity
    bool softDropTrigger;
    // variables related to pausing/resuming the game
    double pauseInterval;
    double lastPauseTime;
};

#endif