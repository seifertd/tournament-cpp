#include "tournament.h"

Tournament::Scorer::Score Tournament::Scorer::score(const Tournament::Bracket& bracket, const Tournament::Bracket& picks) const {
  Tournament::Scorer::Score score;
  for (int round = 0; round < bracket.rounds(); ++round) {
    for (int game = 0; game < bracket.gamesInRound(round); ++game) {
      Tournament::Bracket::Result& result = bracket.finalResult(round, game);
      Tournament::Bracket::Result& pick = picks.finalResult(round,game);
      if (result.result != -1 && result.result == pick.result) {
        score.score += scoreOf(round, result.winner, result.loser);
      }
      Tournament::Bracket::Result& maxResult = result.result != -1 ? result : pick;
      score.max += scoreOf(round, maxResult.winner, maxResult.loser);
    }
  }
  return score;
}
