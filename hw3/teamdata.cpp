#include <iostream>
#include <fstream>

#include "json.hpp"

#include "teamdata.h"


static const char* META = "metadata";
static const char* NUM_PLAYERS = "numPlayers";
static const char* STATS = "teamStats";
static const char* PLAYER1 = "playerOne";
static const char* PLAYER2 = "playerTwo";
static const char* WIN_PERC = "winPercentage";

TeamData::TeamData(std::string filename) {
    std::ifstream file(filename);
    if (!file) {
        return;
    }

    nlohmann::json jsonIn;
    file >> jsonIn;

    numPlayers_ = jsonIn[META][NUM_PLAYERS];

    winPercentages_.resize(numPlayers_, std::vector<double>(numPlayers_));

    nlohmann::json jsonStats = jsonIn[STATS];

    for (auto iter = jsonStats.begin(); iter != jsonStats.end(); ++iter) {
        int p1 = (*iter)[PLAYER1];
        int p2 = (*iter)[PLAYER2];

        double percentage = (*iter)[WIN_PERC];

        winPercentages_[p1][p2] = percentage;
        winPercentages_[p2][p1] = percentage;
    }
}

int TeamData::numPlayers() const
{
    return numPlayers_;
}

double TeamData::winPercentages(int i, int j) const
{
    return winPercentages_[i][j];
}

