#include <iostream>
#include <fstream>
#include <set>
#include <utility>
#include <cmath>

#include "json.hpp"

#include "priorityqueue.h"
#include "teamdata.h"

static const char* TEAMS = "teams";

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: createteams file.json" << std::endl;
        return 0;
    }
    TeamData teamData(argv[1]);

    PriorityQueue pq(teamData.numPlayers() * teamData.numPlayers());
    std::set<int> removed;

    for (int i = 0; i < teamData.numPlayers(); ++i) {
        for (int j = i + 1; j < teamData.numPlayers(); ++j) {
            double key = std::fabs(teamData.winPercentages(i, j) - 50);
            pq.insert(std::make_pair(key, std::make_pair(i, j)));
        }
    }

    std::vector<std::pair<int, int>> teams;

    while (!pq.isEmpty()) {
        KeyValuePair kv = pq.removeMin();

        if (removed.count(kv.second.first) == 0 && removed.count(kv.second.second) == 0) {
            removed.insert(kv.second.first);
            removed.insert(kv.second.second);
            teams.push_back(kv.second);
        }
    }

    nlohmann::json jsonOut;

    jsonOut[TEAMS] = teams;

    std::cout << jsonOut.dump(2) << "\n";

    return 0;
}
