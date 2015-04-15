#include "tournament.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <stdint.h>

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

void Tournament::Possibilities::scorePicks(const std::vector<Tournament::Bracket>& picks) {
  uint64_t possibility = start_;
  uint64_t played = (1ULL << (bracket_.numberOfTeams() - 1)) - 1;
  uint64_t count = 0ULL;
  uint64_t total = start_ - end_ + 1;
  std::cout << "Checking " << total << " outcomes: " << bracket_ << std::endl;
  std::cout << "Start: " << start_ << " -> End: " << end_ << std::endl;
  std::cout << "Total Outcomes: " << bracket_.numberOfOutcomes() << std::endl;
  while (possibility != end_) {
    uint64_t real_poss = 0;
    for(int i = 0; i < games_left_; ++i) {
      real_poss |= (((possibility & (1ULL << i)) ? 1ULL : 0ULL) << shifts_->at(i));
    }
    real_poss = bracket_.results() | real_poss;
    Tournament::Bracket possible_bracket(bracket_.numberOfTeams(), real_poss, played);
    for(int p = 0; p < picks.size(); p++) {
      Tournament::Scorer::Score score = scorer_.score(possible_bracket, picks[p]);
      //std::cout <<  "POSS: " << possibility << ": " << picks[p] << " <-> " << possible_bracket << ": score=" << score.score << " max=" << score.max << std::endl;
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
}
