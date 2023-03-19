#ifndef _TOURNAMENT_STATS_H
#define _TOURNAMENT_STATS_H

#include "tournament.h"
#include <map>

namespace Tournament {
  struct Stats {
    Stats() : maxRank(1000000), minRank(-1), maxScore(-1), bracketResults(0) {}
    int maxRank;
    int minRank;
    int maxScore;
    uint64_t bracketResults;
    std::string name;
    std::map<int,int> champCounts;
    friend std::ostream& operator<<(std::ostream& out, const Stats &s);
    void merge(const Stats& other) {
      if (other.maxRank < maxRank) { maxRank = other.maxRank; }
      if (other.minRank > minRank) { minRank = other.minRank; }
      if (other.maxScore > maxScore) { maxScore = other.maxScore; }
      for(std::map<int,int>::const_iterator it = other.champCounts.begin();
          it != other.champCounts.end();
          ++it) {
        champCounts[it->first] += it->second;
      }
    }
  };
}
#endif
