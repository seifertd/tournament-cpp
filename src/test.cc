#include "tournament.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>

void test(Tournament::Scorer* scorer, Tournament::Bracket& b, std::vector<Tournament::Bracket>& picks) {
  int num_iterations = 100000;
  std::chrono::high_resolution_clock::time_point t1, t2;
  t1 = std::chrono::high_resolution_clock::now();
  int num_picks = picks.size();
  for (int t = 0; t < num_iterations; t++) {
    for (int p = 0; p < num_picks; p++) {
      Tournament::Scorer::Score s = scorer->score(b, picks[p]);
    }
  }
  t2 = std::chrono::high_resolution_clock::now();
  int64_t duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  int sps = (double) num_iterations * num_picks / (duration / 1e6);
  std::cout << " " << num_iterations << " scores of " << num_picks << " brackets using " << scorer->name() << " scorer: " << duration << "ms sps: " << sps << std::endl;
}

int main(int argc, char *argv[]) {

  std::vector<Tournament::Team> teams = Tournament::sampleNcaaTeams;
  Tournament::Bracket small = Tournament::Bracket::randomBracket(64, 46);
  std::cout << "RANDOM BRACKET: " << small << std::endl;
  const Tournament::Scorer& scorer =  Tournament::BasicScorer(teams);
  std::vector<Tournament::Bracket> small_picks;
  for (int i = 0; i < 10; i++) {
    small_picks.push_back(Tournament::Bracket::randomBracket(64));
  }
  Tournament::Possibilities poss1(small, scorer, 0, 3);
  Tournament::Possibilities poss2(small, scorer, 1, 3);
  Tournament::Possibilities poss3(small, scorer, 2, 3);
  std::chrono::high_resolution_clock::time_point t1, t2;
  t1 = std::chrono::high_resolution_clock::now();
  std::vector<Tournament::Stats> stats1 = poss1.scorePicks(small_picks);
  std::vector<Tournament::Stats> stats2 = poss2.scorePicks(small_picks);
  std::vector<Tournament::Stats> stats3 = poss3.scorePicks(small_picks);
  t2 = std::chrono::high_resolution_clock::now();
  int64_t duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  int sps = (double) small.numberOfOutcomes() * 10 / (duration / 1e6);
  std::cout << " " << small.numberOfOutcomes() <<
            " scores of 10 brackets using " << scorer.name() << " scorer: " <<
            duration << "ms sps: " << sps << std::endl;

  std::cout << "STATS:" << std::endl;
  for(int i = 0; i < 10; ++i) {
    Tournament::Stats& stats = stats1[i];
    stats.merge(stats2[i]);
    stats.merge(stats3[i]);
    std::cout << "Pick " << (i+1) << ": " << small_picks[i] << ": " << stats << std::endl;
  }
  return 0;

  for (uint64_t i = 25; i < 32; ++i) {
    std::cout << "Number of bits in " << i << ": " << Tournament::bitsSetIn(i) << std::endl;
  }
  uint64_t all = 0xefffffffffffffff;
  std::cout << "Number of bits in " << all << ": " << Tournament::bitsSetIn(all) << std::endl;
  Tournament::Bracket unplayed(64);
  std::cout << "UNPLAYED: " << unplayed.numberOfTeams() << " teams has " << unplayed.totalGames() << " games and " << unplayed.rounds() << " rounds." << std::endl;
  std::cout << "Games Played: " << unplayed.gamesPlayed() << std::endl;
  std::cout << "Teams Left: " << unplayed.teamsLeft() << std::endl;
  std::cout << "Number of outcomes: " << unplayed.numberOfOutcomes() << std::endl;
  std::cout << unplayed << std::endl;

  Tournament::Bracket b = Tournament::Bracket::randomBracket(64);
  std::cout << "Bracket with " << b.numberOfTeams() << " teams has " << b.totalGames() << " games and " << b.rounds() << " rounds." << std::endl;
  std::cout << "Games Played: " << b.gamesPlayed() << std::endl;
  std::cout << "Teams Left: " << b.teamsLeft() << std::endl;
  std::cout << "Number of outcomes: " << b.numberOfOutcomes() << std::endl;
  std::cout << b << std::endl;
  std::cout << " PLAYED: " << Tournament::uint64ToBits(b.played()) << std::endl;
  std::cout << "RESULTS: " << Tournament::uint64ToBits(b.results()) << std::endl;
  std::cout << "FLIPPED: " << Tournament::uint64ToBits(Tournament::flipBits(b.results())) << std::endl;
  for (int j = 0; j < b.rounds(); ++j) {
    std::cout << " -> Round: " << (j+1) << ": gamesInRound: " << b.gamesInRound(j) << " played: " << b.gamesPlayedInRound(j) <<
      " offset: " << b.roundOffset(j) << " mask: " << Tournament::uint64ToBits(b.roundMask(j)) << std::endl;
  }
  std::cout << "Results: Winner: " << b.winner() << std::endl;
  for(int game_count = 0; game_count < b.getFinalResults()->size(); ++game_count) {
    std::cout << "Game " << game_count << ": " << (*b.getFinalResults())[game_count] << std::endl;
  }

  std::cout << "BENCHMARK:" << std::endl;
  std::vector<Tournament::Bracket> picks;
  std::vector<Tournament::Scorer*> scorers;
  scorers.push_back(new Tournament::BasicScorer(teams));
  scorers.push_back(new Tournament::SeedMultiplierScorer(teams));
  scorers.push_back(new Tournament::SeedDifferenceMultiplierScorer(teams));
  scorers.push_back(new Tournament::ConstantValueScorer(teams));
  scorers.push_back(new Tournament::SeedAdderScorer(teams));
  for (int i = 0; i < 10; i++) {
    Tournament::Bracket pick = Tournament::Bracket::randomBracket(64);
    for (int s = 0; s < scorers.size(); ++s) {
      Tournament::Scorer* scorer = scorers[s];
      Tournament::Scorer::Score score = scorer->score(b, pick);
      std::cout << scorer->name() << ": Pick: " << pick << " Bracket: " << b << " Score: " << score.score << std::endl;
    }
    picks.push_back(pick);
  }

  for (int s = 0; s < scorers.size(); ++s) {
    test(scorers[s], b, picks);
  }

  return 0;
}
