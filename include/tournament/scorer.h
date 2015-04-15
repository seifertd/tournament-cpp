#ifndef _TOURNAMENT_SCORER_H
#define _TOURNAMENT_SCORER_H

#include "tournament.h"
#include <string>
#include <vector>

namespace Tournament {
  static int defaultMultipliers [] = {1,2,4,8,16,32};
  static int defaultAdders [] = {3,5,11,19,30,40};
  class Scorer {
    public:
      Scorer(std::vector<Tournament::Team>& t, std::string n) : teams(t), name_(n) { };
      struct Score {
        Score() : score(0), max(0) {};
        int score;
        int max;
      };
      Score score(const Tournament::Bracket& bracket, const Tournament::Bracket& picks) const;
      virtual int scoreOf(int r, int w, int l) const = 0;
      virtual std::string name() { return name_; };
    protected:
      std::vector<Tournament::Team>& teams;
      std::string name_;
  };

  class BasicScorer : public Scorer {
    public:
      BasicScorer(std::vector<Tournament::Team>& t) : Scorer(t, "Basic"),
        round_scores(defaultMultipliers, defaultMultipliers + sizeof(defaultMultipliers) / sizeof(int)) {};
      virtual int scoreOf(int r, int w, int l) const {
        return round_scores[r];
      };
      std::vector<int> round_scores;
  };

  class ConstantValueScorer : public Scorer {
    public:
      ConstantValueScorer(std::vector<Tournament::Team>& t) : Scorer(t, "Constant"), value(1) {};
      virtual int scoreOf(int r, int w, int l) const {
        return value;
      };
      int value;
  };

  class SeedAdderScorer : public Scorer {
    public:
      SeedAdderScorer(std::vector<Tournament::Team>& t) : Scorer(t, "SeedAdder"),
        per_round(defaultMultipliers, defaultMultipliers + sizeof(defaultMultipliers) / sizeof(int)) {};
      virtual int scoreOf(int r, int w, int l) const {
        const Tournament::Team& winner = teams[w];
        return per_round[r] + winner.seed;
      };
      std::vector<int> per_round;
  };

  class SeedMultiplierScorer : public Scorer {
    public:
      SeedMultiplierScorer(std::vector<Tournament::Team>& t) : Scorer(t, "SeedMultiplier"),
          multipliers(defaultMultipliers, defaultMultipliers + sizeof(defaultMultipliers) / sizeof(int)) {};
      virtual int scoreOf(int r, int w, int l) const {
        const Tournament::Team& team = teams[w];
        return multipliers[r] * team.seed;
      };
      std::vector<int> multipliers;
  };

  class SeedDifferenceMultiplierScorer : public Scorer {
    public:
      SeedDifferenceMultiplierScorer(std::vector<Tournament::Team>& t) : Scorer(t, "SeedDifference"),
          multipliers(defaultMultipliers, defaultMultipliers + sizeof(defaultMultipliers) / sizeof(int)) {};
      virtual int scoreOf(int r, int w, int l) const {
        const Tournament::Team& winner = teams[w];
        const Tournament::Team& loser = teams[w];
        int diff = (winner.seed - loser.seed) + 1;
        if (diff <= 0) {
          diff = 1;
        }
        return multipliers[r] * diff;
      };
      std::vector<int> multipliers;
  };
}

#endif
