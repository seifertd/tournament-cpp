#include "tournament.h"
#include <algorithm>
#include <map>
#include <sstream>

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

int Tournament::scorerArgInt(const std::string& arg_str, int def_val) {
  if (arg_str.length() == 0) {
    return def_val;
  }
  int argValue = 0;
  std::istringstream buffer(arg_str);
  buffer >> argValue;
  return argValue;
}

std::vector<int> Tournament::scorerArgArr(const std::string& arg_str, int * def_val, int def_val_size) {
  if (arg_str.length() == 0) {
    return std::vector<int>(def_val, def_val + def_val_size);
  }
  std::string list_num;
  if (arg_str[0] == '[') {
    // Chop off the beginning [ and trailing ]
    list_num = arg_str.substr(1, arg_str.length() - 2);
  } else {
    list_num = arg_str;
  }
  int size = std::count(list_num.begin(), list_num.end(), ',') + 1;
  std::vector<int> argValue(size);
  std::istringstream buffer(list_num);
  std::string token;
  int counter = 0;
  while (std::getline(buffer, token, ',')) {
    std::istringstream buf2(token);
    buf2 >> argValue[counter];
    counter++;
  }
  return argValue;
}

const Tournament::Scorer* Tournament::scorerFor(const std::string& scorer_spec,
    const std::vector<Tournament::Team>& t) {
  // Allow scorer_spec to be something like Basic([1,2,3,4,5,6])
  //  or Constant(3), SeedAdder([1,2,3,4,5,6]), SeedMultiplier([1,2,3,4,5,6]),
  //     SeedDifference([1,2,3,4,5,6])
  std::string arg_str;
  std::string scorer_name;
  std::size_t paren = scorer_spec.find("(");
  if (paren != std::string::npos) {
    std::size_t len = scorer_spec.length() - paren - 2;
    arg_str = scorer_spec.substr(paren + 1, len);
    scorer_name = scorer_spec.substr(0, paren);
  } else {
    scorer_name = scorer_spec;
  }

  if (scorer_name == "Basic") {
    std::vector<int> arg_arr = scorerArgArr(arg_str, Tournament::def_valMultipliers, 6);
    return new Tournament::BasicScorer(t, arg_arr);
  } else if (scorer_name == "Constant") {
    int value = scorerArgInt(arg_str, 1);
    return new Tournament::ConstantValueScorer(t, value);
  } else if (scorer_name == "SeedAdder") {
    std::vector<int> arg_arr = scorerArgArr(arg_str, Tournament::def_valAdders, 6);
    return new Tournament::SeedAdderScorer(t, arg_arr);
  } else if (scorer_name == "SeedMultiplier") {
    std::vector<int> arg_arr = scorerArgArr(arg_str, Tournament::def_valMultipliers, 6);
    return new Tournament::SeedMultiplierScorer(t, arg_arr);
  } else if (scorer_name == "SeedDifference") {
    std::vector<int> arg_arr = scorerArgArr(arg_str, Tournament::def_valMultipliers, 6);
    return new Tournament::SeedDifferenceMultiplierScorer(t, arg_arr);
  }
  return NULL;
}
