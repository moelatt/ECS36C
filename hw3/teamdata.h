#ifndef _TEAMDATA_H_
#define _TEAMDATA_H_

#include <vector>
#include <string>

typedef std::vector<std::vector<double>> Table;

class TeamData
{
public:
    TeamData(std::string filename);
    int numPlayers() const;
    double winPercentages(int i, int j) const;

private:
    
    Table winPercentages_;
    int numPlayers_;

};  

#endif  