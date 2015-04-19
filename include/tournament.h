#ifndef _TOURNAMENT_H_
#define _TOURNAMENT_H_

#include <stdint.h>
#include "tournament/bracket.h"
#include "tournament/team.h"
#include "tournament/scorer.h"
#include "tournament/stats.h"
#include "tournament/possibilities.h"
namespace Tournament {
  int bitsSetIn(uint64_t number);
  uint64_t flipBits(uint64_t number);
  std::string uint64ToBits(const uint64_t& number);
}
#endif
