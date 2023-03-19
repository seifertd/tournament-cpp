#ifndef _TOURNAMENT_BRACKET_H_
#define _TOURNAMENT_BRACKET_H_

#include <stdint.h>
#include <iostream>
#include <vector>

namespace Tournament {
  class Bracket {
    public:
      struct Result {
        Result() : result(-1), winner(-1), loser(-1) { };
        int result;
        int winner;
        int loser;
      };
      Bracket(int number_of_teams, const std::string& name, uint64_t results = 0ULL, uint64_t played = 0ULL);
      Bracket(const Bracket& b) {
        number_of_teams_ = b.numberOfTeams();
        name_ = b.name();
        rounds_ = b.rounds();
        results_ = b.results();
        played_ = b.played();
        round_ = b.round();
        final_results_ = new std::vector<Tournament::Bracket::Result>(*b.finalResults());
      };
      virtual ~Bracket();
      int totalGames() const { return number_of_teams_ - 1; };
      int numberOfTeams() const { return number_of_teams_; };
      std::string name() const { return name_; };
      void setName(const std::string& newName) { name_ = newName; };
      int teamsLeft() const { return number_of_teams_ - gamesPlayed(); };
      uint64_t numberOfOutcomes() const { return 1ULL << (teamsLeft()-1); };
      int rounds() const { return rounds_; };
      int round() const { return round_; };
      uint64_t results() const { return results_; };
      uint64_t played() const { return played_; };
      const std::vector<Tournament::Bracket::Result>* finalResults() const { return final_results_; }
      int gamesPlayed() const;
      int gamesPlayedInRound(int round) const;
      int gamesInRound(int round) const;
      bool isPlayed(int round, int game) const;
      bool isComplete() const { return gamesPlayed() >= totalGames(); }
      int winner() const;
      int recordWinBy(int team_id);
      Result& finalResult(int round, int game) const;
      void result(int round, int game, Result& result) const;
      static Bracket randomBracket(int number_of_teams, int number_of_played = -1);
      friend std::ostream& operator<<(std::ostream &out, const Bracket &b);
      friend std::ostream& operator<<(std::ostream &out, const Result &r);
      void resetFinalResults();
      std::vector<Result>* getFinalResults() const { return final_results_; };
      uint64_t roundMask(int round) const;
      int roundOffset(int round) const;
    private:
      struct MatchInfo {
        MatchInfo() : alive(true), team_bit(0), result_offset(0), round(0), game(0) { };
        bool alive;
        short team_bit;
        short result_offset;
        int round;
        int game;
      };
      int resultOffset(int round, int game) const;
      void matchInfo(int team_id, MatchInfo& info) const { return matchInfo(team_id, rounds_, info); };
      void matchInfo(int team_id, int round, MatchInfo& info) const;
      int number_of_teams_;
      std::string name_;
      int rounds_;
      int round_;
      uint64_t results_;
      uint64_t played_;
      std::vector<Result> * final_results_;
  };
}
#endif
