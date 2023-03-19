#include "tournament.h"
#include <getopt.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <sstream>

/*
 * ./pool -t 64 -e 10 -b 1/3 -r 0xabcdef0123456789 -p 0x0000000000007fff [-s SCORER]
 *
 * Then picks are read one per line as
 * 0xabcdef0123456789 0x0000000000007fff Name
 *
 * Pick status are output as json:
 * {
 *   picks: [
 *     {maxRank: N, minRank: M, maxScore: S, champs: { "TEAM1": C1, "TEAM2": C2, ... } },
 *     {...},
 *     {...},
 *     {...}
 *   ]
 * }
 */

uint64_t ILLEGAL = 0xffffffffffffffff;
struct Pool {
  Pool() : num_teams(-1), num_entries(-1), batch(-1),
           num_batches(-1), results(ILLEGAL), played(ILLEGAL),
           debug(false), scorer("Basic"), teams(Tournament::sampleNcaaTeams) {};
  int num_teams;
  int num_entries;
  int batch;
  int num_batches;
  std::string scorer;
  uint64_t results;
  uint64_t played;
  bool debug;
  std::vector<Tournament::Team> teams;
};

void usage(char * program) {
  std::cerr << program;
  std::cerr << " -t NUM_TEAMS";
  std::cerr << " -e NUM_ENTRIES";
  std::cerr << " -b BATCH";
  std::cerr << " -r RESULT";
  std::cerr << " -p PLAYED";
  std::cerr << " [-s SCORER]";
  std::cerr << " < entries" << std::endl;
  std::cerr << "     BATCH is in the form n/m, like 1/3 or 1/1" << std::endl;
  std::cerr << "     RESULT and PLAYED are 64 bit integers in hex format, like 0x0123456789abcdef" << std::endl;
  std::cerr << "     SCORER is a string specifying a scorer class identifier and optional constructor args of the form" << std::endl;
  std::cerr << "         Default is 'Basic'" << std::endl;
  std::cerr << "         Available scorers are:" << std::endl;
  std::cerr << "         Basic([r1,r2,r3,r4,r5,r6]), default args: [1,2,4,8,16,32]" << std::endl;
  std::cerr << "            - correct picks worth a constant amount per round" << std::endl;
  std::cerr << "         Constant(int), default args: 1" << std::endl;
  std::cerr << "            - correct picks worth a constant amount" << std::endl;
  std::cerr << "         SeedAdder([r1,r2,r3,r4,r5,r6]), default args: [3,5,11,19,30,40]" << std::endl;
  std::cerr << "            - correct picks worth winner's seed plus per round amount" << std::endl;
  std::cerr << "         SeedMultiplier([r1,r2,r3,r4,r5,r6]), default args: [1,2,4,8,16,32]" << std::endl;
  std::cerr << "            - correct picks worth winner's seed times per round amount" << std::endl;
  std::cerr << "         SeedDifference([r1,r2,r3,r4,r5,r6]), default args: [1,2,4,8,16,32]" << std::endl;
  std::cerr << "            - correct picks worth max[winner seed - loser seed,1] times per round amount" << std::endl;
  std::cerr << "entries are read from stdin, one per line, in the form:" << std::endl;
  std::cerr << "RESULT PLAYED" << std::endl;
}

int main(int argc, char *argv[]) {
  Pool pool;
  int option_char;
  std::istringstream buffer;
  while ((option_char = getopt(argc, argv, "t:e:b:s:r:p:d")) != -1) {
  //while((option_char = opts()) != EOF) {
    switch (option_char) {
      case 't':
        sscanf(optarg, "%d", &pool.num_teams);
        break;
      case 'e':
        sscanf(optarg, "%d", &pool.num_entries);
        break;
      case 'b':
        sscanf(optarg, "%d/%d", &pool.batch, &pool.num_batches);
        pool.batch--;
        break;
      case 's':
        pool.scorer = optarg;
        break;
      case 'd':
        pool.debug = true;
        break;
      case 'r':
        buffer.str(optarg);
        buffer >> std::hex >> pool.results;
        break;
      case 'p':
        buffer.str(optarg);
        buffer.seekg(0);
        buffer >> std::hex >> pool.played;
        break;
      case '?':
        std::cerr << "Usage:" << std::endl;
        usage(argv[0]);
        return 1;
        break;
      case ':':
        std::cerr << "ILLEGAL ARG: " << optarg << std::endl;
        usage(argv[0]);
        return 2;
        break;
    }
  }
  if (pool.num_teams == -1) {
    std::cerr << "Missing required argument -t NUM_TEAMS" << std::endl;
    usage(argv[0]);
    return 3;
  }
  const Tournament::Scorer* scorer = Tournament::scorerFor(pool.scorer, pool.teams);
  if (scorer == NULL) {
    std::cerr << "SCORER argument " << pool.scorer << " is not valid." << std::endl;
    usage(argv[0]);
    return 6;
  }
  if (pool.num_entries == -1) {
    std::cerr << "Missing required argument -e NUM_ENTRIES" << std::endl;
    usage(argv[0]);
    return 3;
  }
  if (pool.batch == -1) {
    std::cerr << "Missing required argument -b n/m" << std::endl;
    usage(argv[0]);
    return 3;
  }
  if (pool.num_batches == -1) {
    std::cerr << "Missing required argument -b n/m" << std::endl;
    usage(argv[0]);
    return 3;
  }
  if (pool.results == ILLEGAL) {
    std::cerr << "Missing required argument -r RESULT" << std::endl;
    usage(argv[0]);
    return 3;
  }
  if (pool.played == ILLEGAL) {
    std::cerr << "Missing required argument -p PLAYED" << std::endl;
    usage(argv[0]);
    return 3;
  }
  if (pool.batch < 0 || pool.batch >= pool.num_batches) {
    std::cerr << "BATCH must be n/m where n < m and n > 0" << std::endl;
    usage(argv[0]);
    return 5;
  }

  Tournament::Bracket b(pool.num_teams, "Tourney", pool.results, pool.played);
  std::vector<Tournament::Bracket> entries;

  if (pool.debug) {
    std::cout << "Tournament " << b << std::endl;
    std::cout << "Scorer: " << scorer->description() << std::endl;
    std::cout << "Reading " << pool.num_entries << " entries..." << std::endl;
  }
  for(int i = 0; i < pool.num_entries; ++i) {
    uint64_t results;
    uint64_t played;
    std::string name;
    std::cin >> std::hex >> results;
    std::cin >> std::hex >> played;
    std::cin >> name;
    entries.push_back(Tournament::Bracket(pool.num_teams, name, results, played));
    if (pool.debug) {
      std::cout << "Pick " << (i+1) << ": " << entries.back() << std::endl;
    }
    if (!entries.back().isComplete()) {
      std::cerr << " -> Entry " << (i+1) << " is not complete: " <<
                entries.back() << std::endl;
      return 4;
    }
  }

  // TODO: Generalize this
  Tournament::Possibilities poss(b, *scorer, pool.batch, pool.num_batches);
  std::vector<Tournament::Stats> stats = poss.scorePicks(entries, pool.debug);

  std::cout << "{" << std::endl;
  std::cout << "  \"pool\": {" << std::endl;
  std::cout << "      \"numTeams\": " << b.numberOfTeams() << "," << std::endl;
  std::cout << "      \"numOutcomes\": " << b.numberOfOutcomes() << "," << std::endl;
  std::cout << "      \"name\": \"" << b.name() << "\"" << std::endl;
  std::cout << "  }," << std::endl;
  std::cout << "  \"picks\": [" << std::endl;
  for(int i = 0; i < pool.num_entries; ++i) {
    if (i > 0) {
      std::cout << "," << std::endl;
    }
    std::cout << stats[i];
  }
  std::cout << std::endl << "  ]" << std::endl;
  std::cout << "}" << std::endl;

  return 0;
}
