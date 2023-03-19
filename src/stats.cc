#include "tournament.h"

std::ostream& Tournament::operator<<(std::ostream& out, const Tournament::Stats& s) {
  using namespace std;
    // cache flags
  ios_base::fmtflags oldFlags = out.flags();
  streamsize         oldPrec  = out.precision();
  char               oldFill  = out.fill();

  out << "{" << std::endl;
  out << "  \"maxRank\": " << s.maxRank << ", " << std::endl;
  out << "  \"minRank\": " << s.minRank << ", " << std::endl;
  out << "  \"maxScore\": " << s.maxScore << ", " << std::endl;
  out << "  \"name\": \"" << s.name << "\", " << std::endl;
  out << "  \"champs\": {" << std::endl;
  for(std::map<int,int>::const_iterator it = s.champCounts.begin();
      it != s.champCounts.end(); ++it) {
    if (it != s.champCounts.begin()) {
      std::cout << "," << std::endl;
    }
    std::cout << "    \"" << it->first << "\": " << it->second;
  }
  out << std::endl << "  }" << std::endl;
  out << "}";

  // restore flags
  out.flags(oldFlags);
  out.precision(oldPrec);
  out.fill(oldFill);
  return out;
}
