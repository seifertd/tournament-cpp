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
      std::vector<Stats> scorePicks(const std::vector<Bracket>& picks, bool debug = false);
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
