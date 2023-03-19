#ifndef _TOURNAMENT_SCORER_H
#define _TOURNAMENT_SCORER_H

#include "tournament.h"
#include <sstream>
#include <string>
#include <vector>
#include <iterator>

namespace Tournament {
  static int def_valMultipliers [] = {1,2,4,8,16,32};
  static int def_valAdders [] = {3,5,11,19,30,40};
  class Scorer {
    public:
      Scorer(const std::vector<Tournament::Team>& t, const std::string& n) : teams(t), name_(n) { }
      struct Score {
        Score() : score(0), max(0) {}
        int score;
        int max;
      };
      Score score(const Tournament::Bracket& bracket, const Tournament::Bracket& picks) const;
      virtual int scoreOf(int r, int w, int l) const = 0;
      virtual std::string description() const { return name() + ": correct picks worth "; }
      virtual std::string name() const { return name_; }
    protected:
      const std::vector<Tournament::Team>& teams;
      std::string name_;
  };

  int scorerArgInt(const std::string& arg_str, int def_val);
  std::vector<int> scorerArgArr(const std::string& arg_str, int * def_val, int def_val_size);
  const Scorer* scorerFor(const std::string& scorer_spec, const std::vector<Tournament::Team>& t);

  class BasicScorer : public Scorer {
    public:
      BasicScorer(const std::vector<Tournament::Team>& t,
                  const std::vector<int> m) : Scorer(t, "Basic"), round_scores(m) {}
      BasicScorer(const std::vector<Tournament::Team>& t,
                  int* m = def_valMultipliers) :
           Scorer(t, "Basic"),
           round_scores(m, m + sizeof(m) / sizeof(int)) {}
      virtual int scoreOf(int r, int w, int l) const {
        return round_scores[r];
      }
      virtual std::string description() const {
        std::stringstream result;
        result << Scorer::description();
        std::copy(round_scores.begin(), round_scores.end(),
                  std::ostream_iterator<int>(result, ","));
        result << " per round.";
        return result.str();
      }
      std::vector<int> round_scores;
  };

  class ConstantValueScorer : public Scorer {
    public:
      ConstantValueScorer(const std::vector<Tournament::Team>& t,
                          int v = 1) : Scorer(t, "Constant"), value(v) {};
      virtual int scoreOf(int r, int w, int l) const {
        return value;
      }
      virtual std::string description() const {
        std::stringstream result;
        result << Scorer::description() << value << " each.";
        return result.str();
      }
      int value;
  };

  class SeedAdderScorer : public Scorer {
    public:
      SeedAdderScorer(const std::vector<Tournament::Team>& t,
                      const std::vector<int>& m) : Scorer(t, "SeedAdder"),
            per_round(m) {};
      SeedAdderScorer(const std::vector<Tournament::Team>& t,
                      int * m = def_valAdders) : Scorer(t, "SeedAdder"),
            per_round(m, m + sizeof(m) / sizeof(int)) {};
      virtual int scoreOf(int r, int w, int l) const {
        const Tournament::Team& winner = teams[w];
        return per_round[r] + winner.seed;
      }
      virtual std::string description() const {
        std::stringstream result;
        result << Scorer::description();
        std::copy(per_round.begin(), per_round.end(),
                  std::ostream_iterator<int>(result, ","));
        result << " plus winner's seed per round.";
        return result.str();
      }
      std::vector<int> per_round;
  };

  class SeedMultiplierScorer : public Scorer {
    public:
      SeedMultiplierScorer(const std::vector<Tournament::Team>& t,
                           const std::vector<int>& m) : Scorer(t, "SeedMultiplier"),
            multipliers(m) {};
      SeedMultiplierScorer(const std::vector<Tournament::Team>& t,
                           int * m = def_valMultipliers) : Scorer(t, "SeedMultiplier"),
          multipliers(m, m + sizeof(m) / sizeof(int)) {};
      virtual int scoreOf(int r, int w, int l) const {
        const Tournament::Team& team = teams[w];
        return multipliers[r] * team.seed;
      }
      virtual std::string description() const {
        std::stringstream result;
        result << Scorer::description();
        std::copy(multipliers.begin(), multipliers.end(),
                  std::ostream_iterator<int>(result, ","));
        result << " times winner's seed per round.";
        return result.str();
      }
      std::vector<int> multipliers;
  };

  class SeedDifferenceMultiplierScorer : public Scorer {
    public:
      SeedDifferenceMultiplierScorer(const std::vector<Tournament::Team>& t,
                           const std::vector<int>& m) : Scorer(t, "SeedDifference"),
            multipliers(m) {};
      SeedDifferenceMultiplierScorer(const std::vector<Tournament::Team>& t,
                  int * m = def_valMultipliers) : Scorer(t, "SeedDifference"),
          multipliers(m, m + sizeof(m) / sizeof(int)) {};
      virtual int scoreOf(int r, int w, int l) const {
        const Tournament::Team& winner = teams[w];
        const Tournament::Team& loser = teams[w];
        int diff = (winner.seed - loser.seed) + 1;
        if (diff <= 0) {
          diff = 1;
        }
        return multipliers[r] * diff;
      }
      virtual std::string description() const {
        std::stringstream result;
        result << Scorer::description();
        std::copy(multipliers.begin(), multipliers.end(),
                  std::ostream_iterator<int>(result, ","));
        result << " times difference between winner's seed and loser's seed per round.";
        return result.str();
      }
      std::vector<int> multipliers;
  };
}

#endif
