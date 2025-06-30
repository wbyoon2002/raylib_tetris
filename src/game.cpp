#include "game.h"
#include <ctime>
#include <random>

int Game::keyPressed = 0;

Game::Game(float x, float y, int keyboard)
    : gameOver(false), isHoldEmpty(true), hasSwappedBlock(false), fontSize(38), totalRowsCleared(0), lockDelayResetCount(0), tSpinType(0), lockBlockTrigger(false),
        combo(-1), actionType(-1), hasTouchedGround(false), isCombo(false), isBacktoBack(false), dropPeriod(1.0), rowsCleared(0), keyBoardLayout(keyboard),
        previousDifficult(false), currentDifficult(false), isLastManeuverRotation(false), isDifficultWallKick(false), actionTrigger(false), offsetX(x), offsetY(y),
        rng(rd()) {
    std::string assetsBasePath = ASSETS_PATH;
    lastDropTime = GetTime();
    lastActionTime = GetTime();
    grid = new Grid(offsetX, offsetY);
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    for (int i = 0; i < 3; i++) {
        nextBlock[i] = GetRandomBlock();
    }
    PlaceGhostBlock();
    std::string soundPath = assetsBasePath + "/Sounds/action.mp3";
    actionSound = LoadSound(soundPath.c_str());
    soundPath = assetsBasePath + "/Sounds/difficultAction.mp3";
    difficultActionSound = LoadSound(soundPath.c_str());
    soundPath = assetsBasePath + "/Sounds/hardDrop.mp3";
    hardDropSound = LoadSound(soundPath.c_str());
    soundPath = assetsBasePath + "/Sounds/gameOver.mp3";
    gameOverSound = LoadSound(soundPath.c_str());
    soundPath = assetsBasePath + "/Sounds/move.mp3";
    moveSound = LoadSound(soundPath.c_str());
    soundPath = assetsBasePath + "/Sounds/hold.mp3";
    holdSound = LoadSound(soundPath.c_str());
    soundPath = assetsBasePath + "/Sounds/rotate.mp3";
    rotateSound = LoadSound(soundPath.c_str());
    actionNames = {"Single", "Double", "Triple", "Tetris", "T-spin no line(s)", "T-Spin Single", "T-Spin Double",
        "T-Spin Triple", "Mini T-spin no line(s)", "Mini T-Spin Single", "Mini T-Spin Double"};
    isDifficultAction = {false, false, false, true, false, true, true, true, false, true, true};
    if (keyBoardLayout < 0 || keyBoardLayout > 2) {
        keyBoardLayout = 0;
    }
}

// pick a random tetromino from a bag of tetrominoes
Block Game::GetRandomBlock() {
    if (blocks.empty()) {
        blocks = GetAllBlocks();
    }
    std::uniform_int_distribution<int> dist(0, blocks.size() - 1);
    int randomIndex = dist(rng);
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks() {
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

void Game::Draw() {
    ClearBackground(darkBlue);
    
    if (ActionTriggered()) {
        Color color = isDifficultAction[actionType] ? YELLOW : WHITE;
        float posY = offsetY + 620;
        // Display Combo
        if (isCombo) {
            char comboText[15];
            sprintf(comboText, "Combo x%d", combo);
            DrawTextEx(font, comboText, {offsetX + 355 - MeasureTextEx(font, comboText, fontSize, 2).x / 2, posY}, fontSize, 2, color);
            posY += fontSize + 5;
        }
        // Display back-to-back difficult line clears
        if (isBacktoBack) {
            DrawTextEx(font, "Back-to-Back", {offsetX + 355 - MeasureTextEx(font, "Back-to-Back", fontSize, 2).x / 2, posY}, fontSize, 2, color);
            posY += fontSize + 5;
        }
        // Display Action
        DrawTextEx(font, actionNames[actionType].c_str(), {offsetX + 355 - MeasureTextEx(font, actionNames[actionType].c_str(), fontSize, 2).x / 2, posY}, fontSize, 2, color);
    }

    // Display boxes for the next block and the held block
    DrawTextBox("Next", offsetX + 500, offsetY + 10, 170, 280);
    DrawTextBox("Hold", offsetX + 10, offsetY + 10, 170, 110);
    // Draw the grid
    grid->Draw();
    // draw holdBlock, currentBlock, ghostBlock, and nextBlock
    if (!isHoldEmpty) {
        switch (holdBlock.id)
        {
        case 3:     // I Block
            holdBlock.Draw(offsetX - 55 + 1, offsetY + 60 + 1);
            break;
        case 4:     // O Block
            holdBlock.Draw(offsetX - 55 + 1, offsetY + 75 + 1);
            break;
        default:
            holdBlock.Draw(offsetX - 40 + 1, offsetY + 75 + 1);
            break;
        }
    }
    currentBlock.Draw(offsetX + 190 + 1, offsetY + 11);
    if (hardDropDistance > 0) {
        ghostBlock.DrawEdge(offsetX + 190 + 1, offsetY + 11);
    }
    for (int i = 0; i < 3; i++) {
        switch (nextBlock[i].id)
        {
        case 3:
            nextBlock[i].Draw(offsetX + 435 + 1, offsetY + 60 + i * 85 + 1);
            break;
        case 4:
            nextBlock[i].Draw(offsetX + 435 + 1, offsetY + 75 + i * 85 + 1);
            break;
        default:
            nextBlock[i].Draw(offsetX + 450 + 1, offsetY + 75 + i * 85 + 1);
            break;
        }
    }
}

void Game::HandleInput() {
    actionTrigger = false;
    resetTrigger = false;
    gameOverTrigger = false;
    lockBlockTrigger = false;
    softDropTrigger = false;
    isKeyDown = IsKeyDown(softDropKey[keyBoardLayout]);
    if (!gameOver) {
        if (keyPressed == leftKey[keyBoardLayout]) {
            MoveBlockLeft();
        }
        else if (keyPressed == rightKey[keyBoardLayout]) {
            MoveBlockRight();
        }
        else if (keyPressed == cwKey[keyBoardLayout * 2] || keyPressed == cwKey[keyBoardLayout * 2 + 1]) {
            RotateBlockClockwise();
        }
        else if (keyPressed == ccwKey[keyBoardLayout]) {
            RotateBlockCounterclockwise();
        }
        else if (keyPressed == softDropKey[keyBoardLayout]) {
            // update the gravity trigger time
            lastDropTime = GetTime();
            MoveBlockDown();
        }
        else if (keyPressed == hardDropKey[keyBoardLayout]) {
            HardDropBlock();
        }
        else if (keyPressed == holdKey[keyBoardLayout]) {
            SwapBlock();
        }
        else if (keyPressed == KEY_ESCAPE) {
            exitMode = true;
        }
        double gravityPeriod = isKeyDown ? (0.05 < dropPeriod ? 0.05 : dropPeriod) : dropPeriod;
        // gravity implementation
        if (DropTriggered(gravityPeriod) && hardDropDistance > 0) {
            softDropDistance = (GetTime() - lastDropTime) / gravityPeriod;
            lastDropTime += gravityPeriod * softDropDistance;
            if (softDropDistance > hardDropDistance) {
                softDropDistance = hardDropDistance;
            }
            for (int i = 0; i < softDropDistance; i++) {
                MoveBlockDown();
            }
            if (isKeyDown && dropPeriod > 0.05 && softDropDistance > 0) {
                softDropTrigger = true;
            }
        }
        // start the lock delay timer when the currentBlock has touched the ground
        if (hardDropDistance == 0 && hasTouchedGround == false) {
            hasTouchedGround = true;
            lastLockDelayTime = GetTime();
        }
        // call LockBlock() 0.5s after the lock delay timer was triggered
        if ((LockDelayTriggered() || lockDelayResetCount == 15) && hasTouchedGround && hardDropDistance == 0) {
            LockBlock();
        }
    }
}

void Game::ReadInput() {
    keyPressed = GetKeyPressed();
}

void Game::MoveBlockLeft() {
    PlaySound(moveSound);
    if (!gameOver) {
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || BlockFits() == false) {
            currentBlock.Move(0, 1);
            return;
        }
        // reset the lock delay timer if currentBlock has moved after touching the ground (maximum 15 times)
        if (lockDelayResetCount < 15 && hasTouchedGround) {
            lockDelayResetCount++;
            lastLockDelayTime = GetTime();
            lastDropTime = GetTime();
        }
        PlaceGhostBlock();
        isLastManeuverRotation = false;
    }
}

void Game::MoveBlockRight() {
    PlaySound(moveSound);
    if (!gameOver) {
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || BlockFits() == false) {
            currentBlock.Move(0, -1);
            return;
        }
        if (lockDelayResetCount < 15 && hasTouchedGround) {
            lockDelayResetCount++;
            lastLockDelayTime = GetTime();
            lastDropTime = GetTime();
        }
        PlaceGhostBlock();
        isLastManeuverRotation = false;
    }
}

void Game::MoveBlockDown() {
    if (!gameOver) {
        if (hardDropDistance > 0) {
            currentBlock.Move(1, 0);
            isLastManeuverRotation = false;
            if (lockDelayResetCount < 15 && hasTouchedGround) {
                lockDelayResetCount++;
                lastLockDelayTime = GetTime();
            }
        }
        else {
            LockBlock();
        }
        PlaceGhostBlock();
    }
}

bool Game::IsBlockOutside(bool isGhostBlock)
{
    std::vector<Position> tiles;
    if (isGhostBlock) {
        tiles = ghostBlock.GetCellPositions();
    }
    else {
        tiles = currentBlock.GetCellPositions();
    }
    for (Position item : tiles) {
        if (grid->IsCellOutside(item.row, item.column)) {
            return true;
        }
    }
    return false;
}

void Game::RotateBlockClockwise() {
    if (!gameOver) {
        Block temp = currentBlock;
        currentBlock.RotateClockwise();
        if (CheckClockwiseWallKick()) {
            PlaySound(rotateSound);
            isLastManeuverRotation = true;
            if (lockDelayResetCount < 15 && hasTouchedGround) {
                lockDelayResetCount++;
                lastLockDelayTime = GetTime();
                lastDropTime = GetTime();
            }
        }
        else {
            currentBlock = temp;
        }
        PlaceGhostBlock();
    }
}

void Game::RotateBlockCounterclockwise() {
    if (!gameOver) {
        Block temp = currentBlock;
        currentBlock.RotateCounterclockwise();
        if (CheckCounterclockwiseWallKick()) {
            PlaySound(rotateSound);
            isLastManeuverRotation = true;
            if (lockDelayResetCount < 15 && hasTouchedGround) {
                lockDelayResetCount++;
                lastLockDelayTime = GetTime();
                lastDropTime = GetTime();
            }
        }
        else {
            currentBlock = temp;
        }
        PlaceGhostBlock();
    }
}

void Game::LockBlock() {
    lockBlockTrigger = true;
    // update the grid information
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles) {
        grid->grid[item.row][item.column] = currentBlock.id;
    }
    // 0: invalid T-Spin, 1: T-Spin, 2: mini T-Spin
    tSpinType = CheckValidTSpin();
    rowsCleared = grid->ClearFullRows();
    // load the currentBlock from nextBlock and store a new block for nextBlock
    currentBlock = nextBlock[0];
    for (int i = 0; i < 2; i++) {
        nextBlock[i] = nextBlock[i + 1];
    }
    nextBlock[2] = GetRandomBlock();
    // update lock delay logic
    hasTouchedGround = false;
    lockDelayResetCount = 0;
    // place ghostBlock for the new currentBlock
    PlaceGhostBlock();
    // end the game if currentBlock is already preoccupied
    if (BlockFits() == false) {
        PlaySound(gameOverSound);
        gameOver = true;
        gameOverTrigger = true;
    }
    // an action is made if row(s) have been cleared or a T-Spin is made
    if (rowsCleared > 0 || tSpinType != 0) {
        // initialize actionTrigger, isCombo, and isBacktoBack
        actionTrigger = true;
        isCombo = false;
        isBacktoBack = false;
        // 0-3: simple line clear, 4-7: T-Spin, 8-10: mini T-spin (see actionNames)
        actionType = 4 * tSpinType;
        switch (tSpinType)
        {
        case 0:
            actionType += rowsCleared - 1;
            break;
        default:
            actionType += rowsCleared;
            break;
        }
        PlaySound(isDifficultAction[actionType] ? difficultActionSound : actionSound);
        // trigger the display
        lastActionTime = GetTime();
        if (rowsCleared > 0) {
            // update the back-to-back indicator when a line has been cleared
            previousDifficult = currentDifficult;
            totalRowsCleared += rowsCleared;
            currentDifficult = isDifficultAction[actionType];
            if (previousDifficult && currentDifficult) {
                isBacktoBack = true;
            }
            combo++;
            if (combo > 0) {
                isCombo = true;
            }
        }
    }
    if (rowsCleared == 0) {
        combo = -1;
    }
    hasSwappedBlock = false;
    lastDropTime = GetTime();
}

bool Game::BlockFits(bool isGhostBlock) {
    std::vector<Position> tiles;
    // check for ghostBlock if isGhostBlock==true
    if (isGhostBlock) {
        tiles = ghostBlock.GetCellPositions();
    }
    // otherwise, check for currentBlock
    else {
        tiles = currentBlock.GetCellPositions();
    }
    for (Position item : tiles) {
        if (grid->IsCellEmpty(item.row, item.column) == false) {
            return false;
        }
    }
    return true;
}

void Game::Reset() {
    gameOver = false;
    resetTrigger = true;
    // reset the grid
    grid->Initialize();
    // reset currentBlock, ghostBlock, holdBlock, and nextBlock
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    PlaceGhostBlock();
    for (int i = 0; i < 3; i++) {
        nextBlock[i] = GetRandomBlock();
    }
    isHoldEmpty = true;
    hasSwappedBlock = false;
    totalRowsCleared = 0;
    lastDropTime = GetTime();
    combo = -1;
    previousDifficult = false;
    currentDifficult = false;
    isLastManeuverRotation = false;
    isDifficultWallKick = false;
    hasTouchedGround = false;
    lockDelayResetCount = 0;
    isCombo = false;
    isBacktoBack = false;
}

void Game::PlaceGhostBlock() {
    ghostBlock = currentBlock;
    hardDropDistance = 0;
    while (IsBlockOutside(true) == false && BlockFits(true)) {
        hardDropDistance++;
        ghostBlock.Move(1, 0);
    }
    hardDropDistance--;
    ghostBlock.Move(-1, 0);
}

void Game::HardDropBlock() {
    PlaySound(hardDropSound);
    isLastManeuverRotation = false;
    currentBlock = ghostBlock;
    hardDropDistance = 0;
    LockBlock();
}

void Game::SwapBlock() {
    // initiate swap only when the game isn't over and the player hasn't swapped the block yet
    if (!(gameOver || hasSwappedBlock)) {
        lastDropTime = GetTime();
        PlaySound(holdSound);
        hasSwappedBlock = true;
        // place holdBlock as currentBlock and update the nextBlock array if holdBlock is empty
        if (isHoldEmpty) {
            isHoldEmpty = false;
            holdBlock = currentBlock;
            currentBlock = nextBlock[0];
            for (int i = 0; i < 2; i++) {
                nextBlock[i] = nextBlock[i + 1];
            }
            nextBlock[2] = GetRandomBlock();
        }
        // for other cases, swap between holdBlock and currentBlock
        else {
            Block temp = currentBlock;
            currentBlock = holdBlock;
            holdBlock = temp;
        }
        PlaceGhostBlock();
        // reset the holdBlock to its initial position for displaying the block and for future swapping
        holdBlock.Reset();
        // reset the relevant parameters for lock delay
        hasTouchedGround = false;
        lockDelayResetCount = 0;
    }
}

bool Game::DropTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastDropTime >= interval) {
        return true;
    }
    return false;
}

bool Game::ActionTriggered() {
    double currentTime = GetTime();
    if (currentTime - lastActionTime <= 1.0 && actionType != -1) {
        return true;
    }
    return false;
}

bool Game::LockDelayTriggered() {
    double currentTime = GetTime();
    if (currentTime - lastLockDelayTime > 0.5) {
        return true;
    }
    return false;
}

void Game::DrawTextBox(const char *text, int x, int y, int width, int height) {
    DrawTextEx(font, text, {(float)x + (width - MeasureTextEx(font, text, fontSize, 2).x) / 2, (float)y}, fontSize, 2, WHITE);
    DrawRectangleRounded({(float)x, (float)y + 40, (float)width, (float)height}, 0.3, 6, lightBlue);
}

bool Game::CheckClockwiseWallKick() {
    isDifficultWallKick = false;
    std::vector<Position> shifts = currentBlock.clockwiseWallKick[currentBlock.rotationState];
    for (Position item : shifts) {
        currentBlock.Move(item.row, item.column);
        if (!IsBlockOutside() && BlockFits()) {
            if (item == shifts[4]) {
                isDifficultWallKick = true;
            }
            return true;
        }
        currentBlock.Move(-item.row, -item.column);
    }
    return false;
}

bool Game::CheckCounterclockwiseWallKick() {
    isDifficultWallKick = false;
    std::vector<Position> shifts = currentBlock.counterclockwiseWallKick[currentBlock.rotationState];
    for (Position item : shifts) {
        currentBlock.Move(item.row, item.column);
        if (!IsBlockOutside() && BlockFits()) {
            if (item == shifts[4]) {
                isDifficultWallKick = true;
            }
            return true;
        }
        currentBlock.Move(-item.row, -item.column);
    }
    return false;
}

int Game::CheckValidTSpin() {
    // variables to store the number of minoes in the front and back corner
    int frontCorner = 0, backCorner = 0;
    // check for T-spin only if currentBlock is a T tetromino and the last maneuver is a rotation
    if (currentBlock.id == 6 && isLastManeuverRotation) {
        std::vector<Position> tiles = currentBlock.GetCellPositions();
        // count the numbers of front and back corners for each rotationState
        if (currentBlock.rotationState % 2 == 0) {
            for (int i = 0; i < 2; i++) {
                if (grid->IsCellOutside(tiles[1].row, tiles[2 + i].column) || !grid->IsCellEmpty(tiles[1].row, tiles[2 + i].column)) {
                    frontCorner++;
                }
                if (grid->IsCellOutside(2 * tiles[0].row - tiles[1].row, tiles[2 + i].column) || !grid->IsCellEmpty(2 * tiles[0].row - tiles[1].row, tiles[2 + i].column)) {
                    backCorner++;
                }
            }
        }
        else {
            for (int i = 0; i < 2; i++) {
                if (grid->IsCellOutside(tiles[2 + i].row, tiles[1].column) || !grid->IsCellEmpty(tiles[2 + i].row, tiles[1].column)) {
                    frontCorner++;
                }
                if (grid->IsCellOutside(tiles[2 + i].row, 2 * tiles[0].column - tiles[1].column) || !grid->IsCellEmpty(tiles[2 + i].row, 2 * tiles[0].column - tiles[1].column)) {
                    backCorner++;
                }
            }
        }
        // an invalid T-spin when the number of occupied corners is less than 3
        if (frontCorner + backCorner < 3) {
            return 0;
        }
        // change a mini T-spin to a regular T-spin if it's a difficult wall kick
        else if (isDifficultWallKick) {
            return 1;
        }
        // mini T-spin: one occupied frontCorner, T-spin: two occupied frontCorners
        else {
            return (3 - frontCorner);
        }
    }
    else {
        return 0;
    }
}
