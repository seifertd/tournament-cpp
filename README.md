# tournament-cpp
Implementation of tournament in C++

# How to run
```bash
./pool -t64 -e10 -b1/1 -r 0x00002ff72921d750 -p 0x00003fffffffffff -s Basic < picks.txt
./pool -t64 -e10 -b1/1 -r 0x00002ff72921d750 -p 0x00003fffffffffff -s Basic() < picks.txt
./pool -t64 -e10 -b1/1 -r 0x00002ff72921d750 -p 0x00003fffffffffff -s 'Basic(1,3,5,9,11,15)' < picks.txt
./pool -t64 -e10 -b1/1 -r 0x00002ff72921d750 -p 0x00003fffffffffff -s 'Constant' < picks.txt
./pool -t64 -e10 -b1/1 -r 0x00002ff72921d750 -p 0x00003fffffffffff -s 'Constant(3)' < picks.txt
./pool -t64 -e10 -b1/1 -r 0x00002ff72921d750 -p 0x00003fffffffffff -s 'SeedAdder' < picks.txt
./pool -t64 -e10 -b1/1 -r 0x00002ff72921d750 -p 0x00003fffffffffff -s 'SeedAdder(1,1,1,1,1,1)' < picks.txt
./pool -t64 -e10 -b1/1 -r 0x00002ff72921d750 -p 0x00003fffffffffff -s 'SeedMultiplier' < picks.txt
./pool -t64 -e10 -b1/1 -r 0x00002ff72921d750 -p 0x00003fffffffffff -s 'SeedMultiplier(1,2,3,4,5,6)' < picks.txt
./pool -t64 -e10 -b1/1 -r 0x00002ff72921d750 -p 0x00003fffffffffff -s 'SeedDifference' < picks.txt
./pool -t64 -e10 -b1/1 -r 0x00002ff72921d750 -p 0x00003fffffffffff -s 'SeedDifference(1,2,3,4,5,6)' < picks.txt
```
