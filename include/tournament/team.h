#ifndef _TOURNAMENT_TEAM_H
#define _TOURNAMENT_TEAM_H

#include <string>
#include <vector>

namespace Tournament {
  struct Team {
    Team(const std::string& n, const std::string& sn, int s, int idx) : name(n), short_name(sn), seed(s), index(idx) {};
    std::string name;
    std::string short_name;
    short seed;
    short index;
  };
  static int ncaaSeeds[16] = {1,16,8,9,5,12,4,13,6,11,3,14,7,10,2,15};
  static Team _sampleNcaaTeams[64] = {
    Team("Team0", "t0", Tournament::ncaaSeeds[0 % 16], 0),
    Team("Team1", "t1", Tournament::ncaaSeeds[1 % 16], 1),
    Team("Team2", "t2", Tournament::ncaaSeeds[2 % 16], 2),
    Team("Team3", "t3", Tournament::ncaaSeeds[3 % 16], 3),
    Team("Team4", "t4", Tournament::ncaaSeeds[4 % 16], 4),
    Team("Team5", "t5", Tournament::ncaaSeeds[5 % 16], 5),
    Team("Team6", "t6", Tournament::ncaaSeeds[6 % 16], 6),
    Team("Team7", "t7", Tournament::ncaaSeeds[7 % 16], 7),
    Team("Team8", "t8", Tournament::ncaaSeeds[8 % 16], 8),
    Team("Team9", "t9", Tournament::ncaaSeeds[9 % 16], 9),
    Team("Team10", "t10", Tournament::ncaaSeeds[10 % 16], 10),
    Team("Team11", "t11", Tournament::ncaaSeeds[11 % 16], 11),
    Team("Team12", "t12", Tournament::ncaaSeeds[12 % 16], 12),
    Team("Team13", "t13", Tournament::ncaaSeeds[13 % 16], 13),
    Team("Team14", "t14", Tournament::ncaaSeeds[14 % 16], 14),
    Team("Team15", "t15", Tournament::ncaaSeeds[15 % 16], 15),
    Team("Team16", "t16", Tournament::ncaaSeeds[16 % 16], 16),
    Team("Team17", "t17", Tournament::ncaaSeeds[17 % 16], 17),
    Team("Team18", "t18", Tournament::ncaaSeeds[18 % 16], 18),
    Team("Team19", "t19", Tournament::ncaaSeeds[19 % 16], 19),
    Team("Team20", "t20", Tournament::ncaaSeeds[20 % 16], 20),
    Team("Team21", "t21", Tournament::ncaaSeeds[21 % 16], 21),
    Team("Team22", "t22", Tournament::ncaaSeeds[22 % 16], 22),
    Team("Team23", "t23", Tournament::ncaaSeeds[23 % 16], 23),
    Team("Team24", "t24", Tournament::ncaaSeeds[24 % 16], 24),
    Team("Team25", "t25", Tournament::ncaaSeeds[25 % 16], 25),
    Team("Team26", "t26", Tournament::ncaaSeeds[26 % 16], 26),
    Team("Team27", "t27", Tournament::ncaaSeeds[27 % 16], 27),
    Team("Team28", "t28", Tournament::ncaaSeeds[28 % 16], 28),
    Team("Team29", "t29", Tournament::ncaaSeeds[29 % 16], 29),
    Team("Team30", "t30", Tournament::ncaaSeeds[30 % 16], 30),
    Team("Team31", "t31", Tournament::ncaaSeeds[31 % 16], 31),
    Team("Team32", "t32", Tournament::ncaaSeeds[32 % 16], 32),
    Team("Team33", "t33", Tournament::ncaaSeeds[33 % 16], 33),
    Team("Team34", "t34", Tournament::ncaaSeeds[34 % 16], 34),
    Team("Team35", "t35", Tournament::ncaaSeeds[35 % 16], 35),
    Team("Team36", "t36", Tournament::ncaaSeeds[36 % 16], 36),
    Team("Team37", "t37", Tournament::ncaaSeeds[37 % 16], 37),
    Team("Team38", "t38", Tournament::ncaaSeeds[38 % 16], 38),
    Team("Team39", "t39", Tournament::ncaaSeeds[39 % 16], 39),
    Team("Team40", "t40", Tournament::ncaaSeeds[40 % 16], 40),
    Team("Team41", "t41", Tournament::ncaaSeeds[41 % 16], 41),
    Team("Team42", "t42", Tournament::ncaaSeeds[42 % 16], 42),
    Team("Team43", "t43", Tournament::ncaaSeeds[43 % 16], 43),
    Team("Team44", "t44", Tournament::ncaaSeeds[44 % 16], 44),
    Team("Team45", "t45", Tournament::ncaaSeeds[45 % 16], 45),
    Team("Team46", "t46", Tournament::ncaaSeeds[46 % 16], 46),
    Team("Team47", "t47", Tournament::ncaaSeeds[47 % 16], 47),
    Team("Team48", "t48", Tournament::ncaaSeeds[48 % 16], 48),
    Team("Team49", "t49", Tournament::ncaaSeeds[49 % 16], 49),
    Team("Team50", "t50", Tournament::ncaaSeeds[50 % 16], 50),
    Team("Team51", "t51", Tournament::ncaaSeeds[51 % 16], 51),
    Team("Team52", "t52", Tournament::ncaaSeeds[52 % 16], 52),
    Team("Team53", "t53", Tournament::ncaaSeeds[53 % 16], 53),
    Team("Team54", "t54", Tournament::ncaaSeeds[54 % 16], 54),
    Team("Team55", "t55", Tournament::ncaaSeeds[55 % 16], 55),
    Team("Team56", "t56", Tournament::ncaaSeeds[56 % 16], 56),
    Team("Team57", "t57", Tournament::ncaaSeeds[57 % 16], 57),
    Team("Team58", "t58", Tournament::ncaaSeeds[58 % 16], 58),
    Team("Team59", "t59", Tournament::ncaaSeeds[59 % 16], 59),
    Team("Team60", "t60", Tournament::ncaaSeeds[60 % 16], 60),
    Team("Team61", "t61", Tournament::ncaaSeeds[61 % 16], 61),
    Team("Team62", "t62", Tournament::ncaaSeeds[62 % 16], 62),
    Team("Team63", "t63", Tournament::ncaaSeeds[63 % 16], 63)
  };
  static std::vector<Team> sampleNcaaTeams(_sampleNcaaTeams, _sampleNcaaTeams + 64);
}
#endif
