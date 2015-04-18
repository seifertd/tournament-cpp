#ifndef _TOURNAMENT_POSSIBILITIES_H_
#define _TOURNAMENT_POSSIBILITIES_H_

#include "tournament.h"
#include <map>
#include <vector>

namespace Tournament {
  class Possibilities {
    public:
      Possibilities(const Bracket& b, const Scorer& s, int batch = 0, int num_batches = 1);
      virtual ~Possibilities();
      void reset();
      struct Stats {
        Stats() : maxRank(1000000), minRank(-1), maxScore(-1) {}
        int maxRank;
        int minRank;
        int maxScore;
        std::map<int,int> champCounts;
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
      std::vector<Stats> scorePicks(const std::vector<Bracket>& picks);
    private:
      const Bracket& bracket_;
      const Scorer& scorer_;
      int batch_;
      int number_of_batches_;
      std::vector<int>* shifts_;
      int games_left_;
      uint64_t start_;
      uint64_t end_;
  };
}

#endif
