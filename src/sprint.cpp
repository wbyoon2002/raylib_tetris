#include "sprint.h"
#include "interface.h"
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip> // Required for std::setfill and std::setw

// Helper function to format duration
std::string formatDuration(double totalSeconds_double) {
    if (totalSeconds_double < 0) {
        // Or handle negative duration as an error, or return "00:00:00.000"
        totalSeconds_double = 0;
    }

    using namespace std::chrono;
    
    // Convert double seconds to a chrono duration
    auto total_duration_ms = duration_cast<milliseconds>(duration<double>(totalSeconds_double));

    // Extract components
    auto ms = total_duration_ms % 1000;
    auto total_seconds_chrono = duration_cast<seconds>(total_duration_ms);
    auto s = total_seconds_chrono % 60;
    auto total_minutes_chrono = duration_cast<minutes>(total_seconds_chrono);
    auto m = total_minutes_chrono % 60;
    auto h = duration_cast<hours>(total_minutes_chrono);

    std::ostringstream oss;
    if (m.count() != 0 || h.count() != 0) {
        if (h.count() != 0) {
            oss << h.count() << ":";
        }
        oss << m.count() << ":";
    }
    oss << s.count() << ":"
        << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

Pages::Sprint::Sprint(int totalLines): totalLines(totalLines), isFinished(false) {
    std::string assetsBasePath = ASSETS_PATH;
    std::string musicPath = assetsBasePath + "/Sounds/music.mp3";
    music = LoadMusicStream(musicPath.c_str());
    PlayMusicStream(music);
    std::string soundPath = assetsBasePath + "/Sounds/victory.mp3";
    victorySound = LoadSound(soundPath.c_str());
    startTime = GetTime();
}

Pages::Sprint::~Sprint() {
    UnloadMusicStream(music);
}

void Pages::Sprint::Draw() {
    UpdateMusicStream(music);
    Game::Draw();
    double elapsedTime = (hasPaused ? lastPauseTime : GetTime()) - startTime;
    if (gameOver) {
        elapsedTime = finalTime;
    }
    
    // Format the elapsed time
    std::string formattedTime = formatDuration(elapsedTime);

    // Display best time
    double best_time = 180.0;
    std::string formattedBestTime = formatDuration(best_time);
    DrawTextEx(font, "Best", {offsetX + 500, offsetY + 620 - fontSize * 2 - 15}, fontSize, 2, WHITE);
    
    // Display the formatted elapsed time
    DrawTextEx(font, formattedBestTime.c_str(), {offsetX + 500, offsetY + 620 - fontSize - 10}, fontSize, 2, WHITE);

    // Display current time
    DrawTextEx(font, "Time", {offsetX + 500, offsetY + 620 - fontSize * 4 - 25}, fontSize, 2, WHITE);
    DrawTextEx(font, formattedTime.c_str(), {offsetX + 500, offsetY + 620 - fontSize * 3 - 20}, fontSize, 2, WHITE);

    // Display "GAME OVER" under the "Next" box
    if (gameOver) {
        if (isFinished) {
            DrawTextEx(font, "FINISHED!", {offsetX + 500, offsetY + 340}, fontSize, 2, WHITE);
        }
        else {
            DrawTextEx(font, "GAME OVER", {offsetX + 500, offsetY + 340}, fontSize, 2, WHITE);
        }
    }

    // Display the number of cleared lines
    DrawTextEx(font, "Lines", {offsetX + 180 - MeasureTextEx(font, "Lines", fontSize, 2).x, offsetY + 620 - fontSize * 2 - 15}, fontSize, 2, WHITE);
    char lineText[20];
    sprintf(lineText, "%d/%d", totalRowsCleared, totalLines);
    DrawTextEx(font, lineText, {offsetX + 180 - MeasureTextEx(font, lineText, fontSize, 2).x, offsetY + 620 - fontSize - 10}, fontSize, 2, WHITE);
}

void Pages::Sprint::HandleInput() {
    ReadInput();
    Game::HandleInput();
    if (gameOver && keyPressed != 0) {      // when a key is pressed after the game is over
        // exit the game when the esc key is pressed
        if (keyPressed == KEY_ESCAPE) {
            exitMode = true;
            return;
        }
        // reset the game when the enter key is pressed
        else if (keyPressed == KEY_ENTER) {
            Game::Reset();
            PlayMusicStream(music);
        }
    }
    else if (!(gameOver || hasPaused) && keyPressed == KEY_ESCAPE) {
        Pause();
        PauseMusicStream(music);
    }
    else if (hasPaused && keyPressed != 0) {
        // exit the game when the esc key is pressed
        if (keyPressed == KEY_ESCAPE) {
            exitMode = true;
            return;
        }
        // reset the game when the enter key is pressed
        else if (keyPressed == KEY_ENTER) {
            Resume();
            startTime += pauseInterval;
            ResumeMusicStream(music);
        }
    }
    if (gameOverTrigger) {
        StopMusicStream(music);
        finalTime = GetTime() - startTime;
        isFinished = false;
    }
    if (totalRowsCleared >= totalLines && !gameOver) {
        StopMusicStream(music);
        PlaySound(victorySound);
        gameOver = true;
        finalTime = GetTime() - startTime;
        isFinished = true;
    }
    if (resetTrigger) {
        Reset();
    }
}

void Pages::Sprint::Reset() {
    startTime = GetTime();
    isFinished = false;
}