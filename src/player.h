#ifndef PLAYER_H
#define PLAYER_H
#include "game.h"

class Player: public Game {
public:
    Player(float, float, int, std::string);
    void Draw() override;
    void HandleInput() override;
    bool HasLost();
    void Stop();
    void Restart();
    std::string getName();
    void HandleAttack(const Player&);
    void Resume() override;
private:
    std::string name;
    // trigger for sending an attack
    bool attackTrigger;
    // number of garbage lines sent
    int attackLines;
    // timestamp for receiving an attack
    double incomeTime;
    // trigger for receiving an attack - true when there is an unresolved garbage income
    double incomeTrigger;
    // number of garbage lines received
    int incomeLines;
    std::vector<int> garbageLines;
};

#endif