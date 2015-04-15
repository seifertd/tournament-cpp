#include "tournament.h"
#include <bitset>

static const uint64_t M1  = 0x5555555555555555;
static const uint64_t M2  = 0x3333333333333333;
static const uint64_t M4  = 0x0f0f0f0f0f0f0f0f;
static const uint64_t M8  = 0x00ff00ff00ff00ff;
static const uint64_t RM8  = 0xff00ff00ff00ff00;
static const uint64_t M16 = 0x0000ffff0000ffff;
static const uint64_t RM16 = 0xffff0000ffff0000;
static const uint64_t M32 = 0x00000000ffffffff;
static const uint64_t RM32 = 0xffffffff00000000;

int Tournament::bitsSetIn(uint64_t i) {
  i = i - ((i >> 1) & M1);
  i = (i & M2) + ((i >> 2) & M2);
  i = (i + (i >> 4)) & M4;
  i = i + (i >> 8);
  i = i + (i >> 16);
  i = i + (i >> 32);
  return (int) (i & 0x7f);
}

uint64_t Tournament::flipBits(uint64_t i) {
  return i ^ 0xffffffffffffffff;
}

std::string Tournament::uint64ToBits(const uint64_t& num) {
  std::bitset<64> bs(num);
  return bs.to_string();
}
