#include "tournament.h"
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <vector>
#include <iostream>
#include <iomanip>

<<<<<<< HEAD
Tournament::Bracket::Bracket(int number_of_teams, uint64_t results, uint64_t played) : number_of_teams_(number_of_teams),
    results_(results), played_(played) {
=======
Tournament::Bracket::Bracket(int number_of_teams, const std::string& name, uint64_t results, uint64_t played) : number_of_teams_(number_of_teams),
    name_(name), results_(results), played_(played) {
>>>>>>> d719525 (minor bug fixes, allow tracking entry by name)
  rounds_ = (int) (log2(number_of_teams) + 0.5);
  round_ = 0;
  final_results_ = new std::vector<Tournament::Bracket::Result>(totalGames());
  if (played_ > 0) {
    resetFinalResults();
  }
}

Tournament::Bracket::~Bracket() {
  if (final_results_) {
    delete final_results_;
  }
}

int Tournament::Bracket::gamesPlayed() const {
  return Tournament::bitsSetIn(played_);
}

int Tournament::Bracket::gamesPlayedInRound(int round) const {
  return Tournament::bitsSetIn((played_ >> roundOffset(round)) & roundMask(round));
}

int Tournament::Bracket::gamesInRound(int round) const {
  return number_of_teams_ / 2 / (1ULL << round);
}

bool Tournament::Bracket::isPlayed(int round, int game) const {
  return ((played_ >> resultOffset(round, game)) & 1) == 1;
}

int Tournament::Bracket::winner() const {
  if (isPlayed(rounds_ - 1, 0)) {
    Tournament::Bracket::Result& result = finalResult(rounds_ - 1, 0);
    return result.winner;
  } else {
    return -1;
  }
}

int Tournament::Bracket::recordWinBy(int team_id) {
  static Tournament::Bracket::MatchInfo info;
  if (!isComplete()) {
    matchInfo(team_id, info);
    if (!info.alive) {
      return 2;
    }
    results_ = results_ | ((uint64_t) info.team_bit << info.result_offset);
    played_ = played_ | (1ULL << info.result_offset);
    if (gamesPlayedInRound(round_) >= gamesInRound(round_)) {
      round_++;
    }
    //std::cout << "RECORD WIN BY " << team_id << " round: " << info.round << " game: " << info.game << " offset: " << info.result_offset << std::endl;
    result(info.round, info.game, (*final_results_)[info.result_offset]);
    return 0;
  } else {
    return 1;
  }
}

Tournament::Bracket::Result& Tournament::Bracket::finalResult(int round, int game) const {
  int result_index = resultOffset(round, game);
  return (*getFinalResults())[result_index];
}

void Tournament::Bracket::result(int round, int game, Tournament::Bracket::Result& my_result) const {
  if (isPlayed(round, game)) {
    my_result.result = 0;
    int r = round;
    int g = game;
    short result_bit = ((results_ >> resultOffset(r,g)) & 1);
    while (r >= 0) {
      short b = ((results_ >> resultOffset(r,g)) & 1);
      my_result.result = my_result.result | (b << r);
      r--;
      g = g * 2 + b;
    }
    if (round == 0) {
      my_result.loser = g + 1 - 2 * result_bit;
    } else {
      Tournament::Bracket::Result previous_result;
      int previous_game = game * 2 + result_bit + 1 - 2 * result_bit;
      result(round-1, previous_game, previous_result);
      my_result.loser = previous_result.winner;
    }
    my_result.winner = g;
  }
}

std::ostream& Tournament::operator<<(std::ostream &out, const Tournament::Bracket &b) {
  using namespace std;

  // cache flags
  ios_base::fmtflags oldFlags = out.flags();
  streamsize         oldPrec  = out.precision();
  char               oldFill  = out.fill();

<<<<<<< HEAD
  out << "Bracket(" << b.numberOfTeams() << "):";
=======
  out << "Bracket(" << b.numberOfTeams() << "): " << b.name() << ": ";
>>>>>>> d719525 (minor bug fixes, allow tracking entry by name)
  out << setprecision(16) << showbase << internal << setfill('0');
  out << " R: " << hex << setw(18) << b.results();
  out << " P: " << hex << setw(18) << b.played();

  // restore flags
  out.flags(oldFlags);
  out.precision(oldPrec);
  out.fill(oldFill);
  return out;
}

std::ostream& Tournament::operator<<(std::ostream &out, const Tournament::Bracket::Result &r) {
  using namespace std;
  out << "[" << r.result << "," << r.winner << "," << r.loser << "]";
  return out;
}

void Tournament::Bracket::resetFinalResults() {
  for (int r = 0; r < rounds_; ++r) {
    int total_games = gamesInRound(r);
    for (int g = 0; g < total_games; ++g) {
      int result_index = resultOffset(r,g);
      result(r, g, (*final_results_)[result_index]);
    }
  }
}

Tournament::Bracket Tournament::Bracket::randomBracket(int number_of_teams, int number_of_played) {
  timeval t1;
  gettimeofday(&t1, NULL);
  srand(t1.tv_usec * t1.tv_sec);
  using namespace Tournament;
<<<<<<< HEAD
  Bracket b(number_of_teams);
=======
  Bracket b(number_of_teams, "random");
>>>>>>> d719525 (minor bug fixes, allow tracking entry by name)
  std::vector<int> teams;
  for (int t = 0; t < number_of_teams; ++t) {
    teams.push_back(t);
  }
  if (number_of_played == -1) {
    number_of_played = b.totalGames();
  }
  //std::cout << "GEN RANDOM BRACKET FOR #TEAMS " << number_of_teams << ", playing " << number_of_played << std::endl;
  while (number_of_played > 0) {
    std::vector<int> new_teams;
    for(int t = 0; t < teams.size() && number_of_played > 0; t += 2) {
      int flip = rand() % 2 + 1;
      if (flip == 1) {
        new_teams.push_back(teams[t]);
      } else {
        new_teams.push_back(teams[t+1]);
      }
      b.recordWinBy(new_teams.back());
      --number_of_played;
      //std::cout << "WIN RECORDED: " << b << std::endl;
    }
    teams = new_teams;
  }
  //b.resetFinalResults();
  //std::cout << "RETURNING: " << b << std::endl;
  return b;
}

uint64_t Tournament::Bracket::roundMask(int round) const {
  return (1ULL << gamesInRound(round)) - 1;
}

int Tournament::Bracket::roundOffset(int round) const {
  return round == 0 ? 0 : (number_of_teams_ - gamesInRound(round-1));
}

int Tournament::Bracket::resultOffset(int round, int game) const {
  return roundOffset(round) + game;
}

void Tournament::Bracket::matchInfo(int team_id, int round, MatchInfo& info) const {
  int win_result = 0;
  int r = 0;
  int game = team_id / 2;
  Tournament::Bracket::Result match_result;
  info.team_bit = team_id % 2;
  while (info.alive && isPlayed(r,game) && r <= round) {
    win_result = win_result | (info.team_bit << r);
    result(r, game, match_result);
    info.alive = (win_result == match_result.result);
    if (!info.alive || r >= round) {
      break;
    }
    r++;
    info.team_bit = game % 2;
    game = game / 2;
  }
  info.round = r;
  info.game = game;
  info.result_offset = resultOffset(r, game);
}
