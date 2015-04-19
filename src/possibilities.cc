#include "tournament.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <stdint.h>

struct PossibleScore {
  int pickIndex;
  int score;
  int champ;
} _PossibleScore;

bool sortScores(const PossibleScore& first, const PossibleScore& second) {
  return first.score > second.score;
}

Tournament::Possibilities::Possibilities(const Tournament::Bracket& b, const Tournament::Scorer& s, int batch, int num_batches) :
    bracket_(b), scorer_(s), batch_(batch), number_of_batches_(num_batches), shifts_(NULL) {
  reset();
}

Tournament::Possibilities::~Possibilities() {
  delete shifts_;
}

void Tournament::Possibilities::reset() {
  if (shifts_) {
    delete shifts_;
  }
  games_left_ = bracket_.totalGames() - bracket_.gamesPlayed();
  shifts_ = new std::vector<int>(games_left_ + 1);
  uint64_t left = Tournament::flipBits(bracket_.played());
  uint64_t offset = 0ULL;
  uint64_t limit = bracket_.numberOfOutcomes();
  if (number_of_batches_ > 1) {
    limit = (uint64_t) ((double) limit / number_of_batches_ + 0.5);
    offset = limit * batch_;
  }
  int shift_count = 0;
  for (int left_bit = 0; left_bit < bracket_.numberOfTeams(); ++left_bit) {
    if (left & (1ULL << left_bit)) {
      shifts_->at(shift_count++) = left_bit;
    }
  }
  start_ = bracket_.numberOfOutcomes() - 1ULL - offset;
  end_ = batch_ == number_of_batches_ - 1 ?  0ULL : bracket_.numberOfOutcomes() - offset - limit;
}

std::vector<Tournament::Stats>
  Tournament::Possibilities::scorePicks(const std::vector<Tournament::Bracket>& picks) {
  uint64_t possibility = start_;
  uint64_t played = (1ULL << (bracket_.numberOfTeams() - 1)) - 1;
  uint64_t count = 0ULL;
  uint64_t total = start_ - end_ + 1;
  std::cout << "Checking " << total << " outcomes of bracket: " << bracket_ << std::endl;
  std::cout << "Number of entries: " << picks.size() << std::endl;
  std::cout << "Start: " << start_ << " -> End: " << end_ << std::endl;
  std::cout << "Total Outcomes: " << bracket_.numberOfOutcomes() << std::endl;

  std::vector<Stats> allStats(picks.size());
  std::vector<PossibleScore> pickScores(picks.size());
  while (possibility != end_) {
    uint64_t real_poss = 0;
    for(int i = 0; i < games_left_; ++i) {
      real_poss |= (((possibility & (1ULL << i)) ? 1ULL : 0ULL) << shifts_->at(i));
    }
    real_poss = bracket_.results() | real_poss;
    Tournament::Bracket possible_bracket(bracket_.numberOfTeams(), real_poss, played);
    for(int p = 0; p < picks.size(); p++) {
      Tournament::Scorer::Score score = scorer_.score(possible_bracket, picks[p]);
      pickScores[p].pickIndex = p;
      pickScores[p].score = score.score;
      pickScores[p].champ = possible_bracket.winner();
      //std::cout <<  "POSS: " << possibility << ": " << picks[p] << " <-> " << possible_bracket << ": score=" << score.score << " max=" << score.max << std::endl;
    }
    std::sort(pickScores.begin(), pickScores.end(), sortScores);
    int currentScore = pickScores[0].score;
    int actualRank = 1;
    for(int i = 0; i < picks.size(); ++i) {
      PossibleScore& score = pickScores[i];
      Tournament::Stats& stats = allStats[score.pickIndex];
      if (score.score < currentScore) {
        actualRank++;
        currentScore = score.score;
      }
      if (score.score > stats.maxScore) {
        stats.maxScore = score.score;
      }
      if (actualRank < stats.maxRank) {
        stats.maxRank = actualRank;
      }
      if (actualRank > stats.minRank) {
        stats.minRank = actualRank;
      }
      if (actualRank == 1) {
        stats.champCounts[score.champ]++;
      }
    }

    if (possibility == end_) {
      break;
    }
    if ((count % 10000ULL) == 0) {
      std::cout << count << "/" << total << "\r" << std::flush;
    }
    count++;
    possibility--;
  }
  std::cout << std::endl << "Done." << std::endl;
  return allStats;
}
