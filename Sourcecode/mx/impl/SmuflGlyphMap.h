// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#pragma once

#include <string>
#include <map>

namespace mx
{
    namespace impl
    {
        inline void createSmuflMap(std::map<const std::string, const char16_t>& outMap)
        {
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "4stringTabClef", 0xE06E )); // glyphnumber: 0, 4-string tab clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "6stringTabClef", 0xE06D )); // glyphnumber: 1, 6-string tab clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal11LargeDiesisDown", 0xE30D )); // glyphnumber: 2, 11 large diesis down, 3° down [46 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal11LargeDiesisUp", 0xE30C )); // glyphnumber: 3, 11 large diesis up, (11L), (sharp less 11M), 3° up [46 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal11MediumDiesisDown", 0xE30B )); // glyphnumber: 4, 11 medium diesis down, 1°[17 31] 2°46 down, 1/4-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal11MediumDiesisUp", 0xE30A )); // glyphnumber: 5, 11 medium diesis up, (11M), 1°[17 31] 2°46 up, 1/4-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal11v19LargeDiesisDown", 0xE3AB )); // glyphnumber: 6, 11:19 large diesis down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal11v19LargeDiesisUp", 0xE3AA )); // glyphnumber: 7, 11:19 large diesis up, (11:19L, apotome less 11:19M)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal11v19MediumDiesisDown", 0xE3A3 )); // glyphnumber: 8, 11:19 medium diesis down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal11v19MediumDiesisUp", 0xE3A2 )); // glyphnumber: 9, 11:19 medium diesis up, (11:19M, 11M plus 19s)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal11v49CommaDown", 0xE397 )); // glyphnumber: 10, 11:49 comma down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal11v49CommaUp", 0xE396 )); // glyphnumber: 11, 11:49 comma up, (11:49C, 11M less 49C)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal143CommaDown", 0xE395 )); // glyphnumber: 12, 143 comma down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal143CommaUp", 0xE394 )); // glyphnumber: 13, 143 comma up, (143C, 13L less 11M)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal17CommaDown", 0xE343 )); // glyphnumber: 14, 17 comma down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal17CommaUp", 0xE342 )); // glyphnumber: 15, 17 comma up, (17C)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal17KleismaDown", 0xE393 )); // glyphnumber: 16, 17 kleisma down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal17KleismaUp", 0xE392 )); // glyphnumber: 17, 17 kleisma up, (17k)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal19CommaDown", 0xE399 )); // glyphnumber: 18, 19 comma down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal19CommaUp", 0xE398 )); // glyphnumber: 19, 19 comma up, (19C)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal19SchismaDown", 0xE391 )); // glyphnumber: 20, 19 schisma down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal19SchismaUp", 0xE390 )); // glyphnumber: 21, 19 schisma up, (19s)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal23CommaDown", 0xE371 )); // glyphnumber: 22, 23 comma down, 2° down [96 EDO], 1/8-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal23CommaUp", 0xE370 )); // glyphnumber: 23, 23 comma up, (23C), 2° up [96 EDO], 1/8-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal23SmallDiesisDown", 0xE39F )); // glyphnumber: 24, 23 small diesis down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal23SmallDiesisUp", 0xE39E )); // glyphnumber: 25, 23 small diesis up, (23S)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal25SmallDiesisDown", 0xE307 )); // glyphnumber: 26, 25 small diesis down, 2° down [53 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal25SmallDiesisUp", 0xE306 )); // glyphnumber: 27, 25 small diesis up, (25S, ~5:13S, ~37S, 5C plus 5C), 2° up [53 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal35LargeDiesisDown", 0xE30F )); // glyphnumber: 28, 35 large diesis down, 2° down [50 EDO], 5/18-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal35LargeDiesisUp", 0xE30E )); // glyphnumber: 29, 35 large diesis up, (35L, ~13L, ~125L,  sharp less 35M), 2°50 up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal35MediumDiesisDown", 0xE309 )); // glyphnumber: 30, 35 medium diesis down, 1°[50] 2°[27] down, 2/9-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal35MediumDiesisUp", 0xE308 )); // glyphnumber: 31, 35 medium diesis up, (35M, ~13M, ~125M, 5C plus 7C), 2/9-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal49LargeDiesisDown", 0xE3A9 )); // glyphnumber: 32, 49 large diesis down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal49LargeDiesisUp", 0xE3A8 )); // glyphnumber: 33, 49 large diesis up, (49L, ~31L, apotome less 49M)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal49MediumDiesisDown", 0xE3A5 )); // glyphnumber: 34, 49 medium diesis down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal49MediumDiesisUp", 0xE3A4 )); // glyphnumber: 35, 49 medium diesis up, (49M, ~31M, 7C plus 7C)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal49SmallDiesisDown", 0xE39D )); // glyphnumber: 36, 49 small diesis down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal49SmallDiesisUp", 0xE39C )); // glyphnumber: 37, 49 small diesis up, (49S, ~31S)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal55CommaDown", 0xE345 )); // glyphnumber: 38, 55 comma down, 3° down [96 EDO], 3/16-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal55CommaUp", 0xE344 )); // glyphnumber: 39, 55 comma up, (55C, 11M less 5C), 3°up [96 EDO], 3/16-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5CommaDown", 0xE303 )); // glyphnumber: 40, 5 comma down, 1° down [22 27 29 34 41 46 53 96 EDOs], 1/12-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5CommaUp", 0xE302 )); // glyphnumber: 41, 5 comma up, (5C), 1° up [22 27 29 34 41 46 53 96 EDOs], 1/12-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v11SmallDiesisDown", 0xE349 )); // glyphnumber: 42, 5:11 small diesis down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v11SmallDiesisUp", 0xE348 )); // glyphnumber: 43, 5:11 small diesis up, (5:11S, ~7:13S, ~11:17S, 5:7k plus 7:11C)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v13LargeDiesisDown", 0xE3AD )); // glyphnumber: 44, 5:13 large diesis down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v13LargeDiesisUp", 0xE3AC )); // glyphnumber: 45, 5:13 large diesis up, (5:13L, ~37L, apotome less 5:13M)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v13MediumDiesisDown", 0xE3A1 )); // glyphnumber: 46, 5:13 medium diesis down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v13MediumDiesisUp", 0xE3A0 )); // glyphnumber: 47, 5:13 medium diesis up, (5:13M, ~37M, 5C plus 13C)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v19CommaDown", 0xE373 )); // glyphnumber: 48, 5:19 comma down, 1/20-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v19CommaUp", 0xE372 )); // glyphnumber: 49, 5:19 comma up, (5:19C, 5C plus 19s), 1/20-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v23SmallDiesisDown", 0xE375 )); // glyphnumber: 50, 5:23 small diesis down, 2° down [60 EDO], 1/5-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v23SmallDiesisUp", 0xE374 )); // glyphnumber: 51, 5:23 small diesis up, (5:23S, 5C plus 23C), 2° up [60 EDO], 1/5-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v49MediumDiesisDown", 0xE3A7 )); // glyphnumber: 52, 5:49 medium diesis down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v49MediumDiesisUp", 0xE3A6 )); // glyphnumber: 53, 5:49 medium diesis up, (5:49M, half apotome)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v7KleismaDown", 0xE301 )); // glyphnumber: 54, 5:7 kleisma down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal5v7KleismaUp", 0xE300 )); // glyphnumber: 55, 5:7 kleisma up, (5:7k, ~11:13k, 7C less 5C)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal7CommaDown", 0xE305 )); // glyphnumber: 56, 7 comma down, 1° down [43 EDO], 2° down [72 EDO], 1/6-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal7CommaUp", 0xE304 )); // glyphnumber: 57, 7 comma up, (7C), 1° up [43 EDO], 2° up [72 EDO], 1/6-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal7v11CommaDown", 0xE347 )); // glyphnumber: 58, 7:11 comma down, 1° down [60 EDO], 1/10-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal7v11CommaUp", 0xE346 )); // glyphnumber: 59, 7:11 comma up, (7:11C, ~13:17S, ~29S, 11L less 7C), 1° up [60 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal7v11KleismaDown", 0xE341 )); // glyphnumber: 60, 7:11 kleisma down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal7v11KleismaUp", 0xE340 )); // glyphnumber: 61, 7:11 kleisma up, (7:11k, ~29k)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal7v19CommaDown", 0xE39B )); // glyphnumber: 62, 7:19 comma down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittal7v19CommaUp", 0xE39A )); // glyphnumber: 63, 7:19 comma up, (7:19C, 7C less 19s)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalAcute", 0xE3F2 )); // glyphnumber: 64, Acute, 5 schisma up (5s), 2 cents up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat", 0xE335 )); // glyphnumber: 65, Double flat, (2 apotomes down)[almost all EDOs], whole-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat11v49CUp", 0xE3E9 )); // glyphnumber: 66, Double flat 11:49C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat143CUp", 0xE3EB )); // glyphnumber: 67, Double flat 143C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat17CUp", 0xE365 )); // glyphnumber: 68, Double flat 17C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat17kUp", 0xE3ED )); // glyphnumber: 69, Double flat 17k-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat19CUp", 0xE3E7 )); // glyphnumber: 70, Double flat 19C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat19sUp", 0xE3EF )); // glyphnumber: 71, Double flat 19s-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat23CUp", 0xE387 )); // glyphnumber: 72, Double flat 23C-up, 14° down [96 EDO], 7/8-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat23SUp", 0xE3E1 )); // glyphnumber: 73, Double flat 23S-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat25SUp", 0xE32D )); // glyphnumber: 74, Double flat 25S-up, 8°down [53 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat49SUp", 0xE3E3 )); // glyphnumber: 75, Double flat 49S-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat55CUp", 0xE363 )); // glyphnumber: 76, Double flat 55C-up, 13° down [96 EDO], 13/16-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat5CUp", 0xE331 )); // glyphnumber: 77, Double flat 5C-up, 5°[22 29] 7°[34 41] 9°53 down, 11/12 tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat5v11SUp", 0xE35F )); // glyphnumber: 78, Double flat 5:11S-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat5v19CUp", 0xE385 )); // glyphnumber: 79, Double flat 5:19C-up, 19/20-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat5v23SUp", 0xE383 )); // glyphnumber: 80, Double flat 5:23S-up, 8° down  [60 EDO], 4/5-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat5v7kUp", 0xE333 )); // glyphnumber: 81, Double flat 5:7k-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat7CUp", 0xE32F )); // glyphnumber: 82, Double flat 7C-up, 5° down [43 EDO], 10° down [72 EDO], 5/6-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat7v11CUp", 0xE361 )); // glyphnumber: 83, Double flat 7:11C-up, 9° down [60 EDO], 9/10-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat7v11kUp", 0xE367 )); // glyphnumber: 84, Double flat 7:11k-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleFlat7v19CUp", 0xE3E5 )); // glyphnumber: 85, Double flat 7:19C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp", 0xE334 )); // glyphnumber: 86, Double sharp, (2 apotomes up)[almost all EDOs], whole-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp11v49CDown", 0xE3E8 )); // glyphnumber: 87, Double sharp 11:49C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp143CDown", 0xE3EA )); // glyphnumber: 88, Double sharp 143C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp17CDown", 0xE364 )); // glyphnumber: 89, Double sharp 17C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp17kDown", 0xE3EC )); // glyphnumber: 90, Double sharp 17k-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp19CDown", 0xE3E6 )); // glyphnumber: 91, Double sharp 19C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp19sDown", 0xE3EE )); // glyphnumber: 92, Double sharp 19s-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp23CDown", 0xE386 )); // glyphnumber: 93, Double sharp 23C-down, 14°up [96 EDO], 7/8-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp23SDown", 0xE3E0 )); // glyphnumber: 94, Double sharp 23S-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp25SDown", 0xE32C )); // glyphnumber: 95, Double sharp 25S-down, 8°up [53 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp49SDown", 0xE3E2 )); // glyphnumber: 96, Double sharp 49S-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp55CDown", 0xE362 )); // glyphnumber: 97, Double sharp 55C-down, 13° up [96 EDO], 13/16-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp5CDown", 0xE330 )); // glyphnumber: 98, Double sharp 5C-down, 5°[22 29] 7°[34 41] 9°53 up, 11/12 tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp5v11SDown", 0xE35E )); // glyphnumber: 99, Double sharp 5:11S-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp5v19CDown", 0xE384 )); // glyphnumber: 100, Double sharp 5:19C-down, 19/20-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp5v23SDown", 0xE382 )); // glyphnumber: 101, Double sharp 5:23S-down, 8° up [60 EDO], 4/5-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp5v7kDown", 0xE332 )); // glyphnumber: 102, Double sharp 5:7k-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp7CDown", 0xE32E )); // glyphnumber: 103, Double sharp 7C-down, 5°[43] 10°[72] up, 5/6-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp7v11CDown", 0xE360 )); // glyphnumber: 104, Double sharp 7:11C-down, 9° up [60 EDO], 9/10-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp7v11kDown", 0xE366 )); // glyphnumber: 105, Double sharp 7:11k-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalDoubleSharp7v19CDown", 0xE3E4 )); // glyphnumber: 106, Double sharp 7:19C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat", 0xE319 )); // glyphnumber: 107, Flat, (apotome down)[almost all EDOs], 1/2-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat11LDown", 0xE329 )); // glyphnumber: 108, Flat 11L-down, 8° up [46 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat11MDown", 0xE327 )); // glyphnumber: 109, Flat 11M-down, 3° down [17 31 EDOs], 7° down [46 EDO], 3/4-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat11v19LDown", 0xE3DB )); // glyphnumber: 110, Flat 11:19L-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat11v19MDown", 0xE3D3 )); // glyphnumber: 111, Flat 11:19M-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat11v49CDown", 0xE3C7 )); // glyphnumber: 112, Flat 11:49C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat11v49CUp", 0xE3B9 )); // glyphnumber: 113, Flat 11:49C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat143CDown", 0xE3C5 )); // glyphnumber: 114, Flat 143C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat143CUp", 0xE3BB )); // glyphnumber: 115, Flat 143C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat17CDown", 0xE357 )); // glyphnumber: 116, Flat 17C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat17CUp", 0xE351 )); // glyphnumber: 117, Flat 17C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat17kDown", 0xE3C3 )); // glyphnumber: 118, Flat 17k-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat17kUp", 0xE3BD )); // glyphnumber: 119, Flat 17k-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat19CDown", 0xE3C9 )); // glyphnumber: 120, Flat 19C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat19CUp", 0xE3B7 )); // glyphnumber: 121, Flat 19C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat19sDown", 0xE3C1 )); // glyphnumber: 122, Flat 19s-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat19sUp", 0xE3BF )); // glyphnumber: 123, Flat 19s-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat23CDown", 0xE37D )); // glyphnumber: 124, Flat 23C-down, 10° down [96 EDO], 5/8-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat23CUp", 0xE37B )); // glyphnumber: 125, Flat 23C-up, 6° down [96 EDO], 3/8-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat23SDown", 0xE3CF )); // glyphnumber: 126, Flat 23S-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat23SUp", 0xE3B1 )); // glyphnumber: 127, Flat 23S-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat25SDown", 0xE323 )); // glyphnumber: 128, Flat 25S-down, 7° down [53 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat25SUp", 0xE311 )); // glyphnumber: 129, Flat 25S-up, 3° down [53 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat35LDown", 0xE32B )); // glyphnumber: 130, Flat 35L-down, 5° down [50 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat35MDown", 0xE325 )); // glyphnumber: 131, Flat 35M-down, 4° down [50 EDO], 6° down [27 EDO], 13/18-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat49LDown", 0xE3D9 )); // glyphnumber: 132, Flat 49L-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat49MDown", 0xE3D5 )); // glyphnumber: 133, Flat 49M-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat49SDown", 0xE3CD )); // glyphnumber: 134, Flat 49S-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat49SUp", 0xE3B3 )); // glyphnumber: 135, Flat 49S-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat55CDown", 0xE359 )); // glyphnumber: 136, Flat 55C-down, 11° down [96 EDO], 11/16-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat55CUp", 0xE34F )); // glyphnumber: 137, Flat 55C-up, 5° down [96 EDO], 5/16-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat5CDown", 0xE31F )); // glyphnumber: 138, Flat 5C-down, 4°[22 29] 5°[27 34 41] 6°[39 46 53] down, 7/12-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat5CUp", 0xE315 )); // glyphnumber: 139, Flat 5C-up, 2°[22,29] 3°[34 41] 4°[46 53 60] down, 5/12-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat5v11SDown", 0xE35D )); // glyphnumber: 140, Flat 5:11S-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat5v11SUp", 0xE34B )); // glyphnumber: 141, Flat 5:11S-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat5v13LDown", 0xE3DD )); // glyphnumber: 142, Flat 5:13L-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat5v13MDown", 0xE3D1 )); // glyphnumber: 143, Flat 5:13M-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat5v19CDown", 0xE37F )); // glyphnumber: 144, Flat 5:19C-down, 11/20-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat5v19CUp", 0xE379 )); // glyphnumber: 145, Flat 5:19C-up, 9/20-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat5v23SDown", 0xE381 )); // glyphnumber: 146, Flat 5:23S-down, 7° down [60 EDO], 7/10-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat5v23SUp", 0xE377 )); // glyphnumber: 147, Flat 5:23S-up, 3° down [60 EDO], 3/10-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat5v49MDown", 0xE3D7 )); // glyphnumber: 148, Flat 5:49M-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat5v7kDown", 0xE31D )); // glyphnumber: 149, Flat 5:7k-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat5v7kUp", 0xE317 )); // glyphnumber: 150, Flat 5:7k-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat7CDown", 0xE321 )); // glyphnumber: 151, Flat 7C-down, 4° down [43 EDO], 8° down [72 EDO], 2/3-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat7CUp", 0xE313 )); // glyphnumber: 152, Flat 7C-up, 2° down [43 EDO], 4° down [72 EDO], 1/3-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat7v11CDown", 0xE35B )); // glyphnumber: 153, Flat 7:11C-down, 6° down [60 EDO], 3/5- tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat7v11CUp", 0xE34D )); // glyphnumber: 154, Flat 7:11C-up, 4° down [60 EDO], 2/5-tone down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat7v11kDown", 0xE355 )); // glyphnumber: 155, Flat 7:11k-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat7v11kUp", 0xE353 )); // glyphnumber: 156, Flat 7:11k-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat7v19CDown", 0xE3CB )); // glyphnumber: 157, Flat 7:19C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalFlat7v19CUp", 0xE3B5 )); // glyphnumber: 158, Flat 7:19C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalGrave", 0xE3F3 )); // glyphnumber: 159, Grave, 5 schisma down, 2 cents down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalShaftDown", 0xE3F1 )); // glyphnumber: 160, Shaft down, (natural for use with only diacritics down)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalShaftUp", 0xE3F0 )); // glyphnumber: 161, Shaft up, (natural for use with only diacritics up)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp", 0xE318 )); // glyphnumber: 162, Sharp, (apotome up)[almost all EDOs], 1/2-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp11LUp", 0xE328 )); // glyphnumber: 163, Sharp 11L-up, 8° up [46 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp11MUp", 0xE326 )); // glyphnumber: 164, Sharp 11M-up, 3° up [17 31 EDOs], 7° up [46 EDO], 3/4-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp11v19LUp", 0xE3DA )); // glyphnumber: 165, Sharp 11:19L-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp11v19MUp", 0xE3D2 )); // glyphnumber: 166, Sharp 11:19M-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp11v49CDown", 0xE3B8 )); // glyphnumber: 167, Sharp 11:49C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp11v49CUp", 0xE3C6 )); // glyphnumber: 168, Sharp 11:49C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp143CDown", 0xE3BA )); // glyphnumber: 169, Sharp 143C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp143CUp", 0xE3C4 )); // glyphnumber: 170, Sharp 143C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp17CDown", 0xE350 )); // glyphnumber: 171, Sharp 17C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp17CUp", 0xE356 )); // glyphnumber: 172, Sharp 17C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp17kDown", 0xE3BC )); // glyphnumber: 173, Sharp 17k-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp17kUp", 0xE3C2 )); // glyphnumber: 174, Sharp 17k-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp19CDown", 0xE3B6 )); // glyphnumber: 175, Sharp 19C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp19CUp", 0xE3C8 )); // glyphnumber: 176, Sharp 19C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp19sDown", 0xE3BE )); // glyphnumber: 177, Sharp 19s-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp19sUp", 0xE3C0 )); // glyphnumber: 178, Sharp 19s-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp23CDown", 0xE37A )); // glyphnumber: 179, Sharp 23C-down, 6° up [96 EDO], 3/8-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp23CUp", 0xE37C )); // glyphnumber: 180, Sharp 23C-up, 10° up [96 EDO], 5/8-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp23SDown", 0xE3B0 )); // glyphnumber: 181, Sharp 23S-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp23SUp", 0xE3CE )); // glyphnumber: 182, Sharp 23S-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp25SDown", 0xE310 )); // glyphnumber: 183, Sharp 25S-down, 3° up [53 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp25SUp", 0xE322 )); // glyphnumber: 184, Sharp 25S-up, 7° up [53 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp35LUp", 0xE32A )); // glyphnumber: 185, Sharp 35L-up, 5° up [50 EDO]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp35MUp", 0xE324 )); // glyphnumber: 186, Sharp 35M-up, 4° up [50 EDO], 6° up [27 EDO], 13/18-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp49LUp", 0xE3D8 )); // glyphnumber: 187, Sharp 49L-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp49MUp", 0xE3D4 )); // glyphnumber: 188, Sharp 49M-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp49SDown", 0xE3B2 )); // glyphnumber: 189, Sharp 49S-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp49SUp", 0xE3CC )); // glyphnumber: 190, Sharp 49S-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp55CDown", 0xE34E )); // glyphnumber: 191, Sharp 55C-down, 5° up [96 EDO], 5/16-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp55CUp", 0xE358 )); // glyphnumber: 192, Sharp 55C-up, 11° up [96 EDO], 11/16-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp5CDown", 0xE314 )); // glyphnumber: 193, Sharp 5C-down, 2°[22 29] 3°[34 41] 4°[46 53 60] up, 5/12-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp5CUp", 0xE31E )); // glyphnumber: 194, Sharp 5C-up, 4°[22 29] 5°[27 34 41] 6°[39 46 53] up, 7/12-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp5v11SDown", 0xE34A )); // glyphnumber: 195, Sharp 5:11S-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp5v11SUp", 0xE35C )); // glyphnumber: 196, Sharp 5:11S-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp5v13LUp", 0xE3DC )); // glyphnumber: 197, Sharp 5:13L-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp5v13MUp", 0xE3D0 )); // glyphnumber: 198, Sharp 5:13M-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp5v19CDown", 0xE378 )); // glyphnumber: 199, Sharp 5:19C-down, 9/20-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp5v19CUp", 0xE37E )); // glyphnumber: 200, Sharp 5:19C-up, 11/20-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp5v23SDown", 0xE376 )); // glyphnumber: 201, Sharp 5:23S-down, 3° up [60 EDO], 3/10-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp5v23SUp", 0xE380 )); // glyphnumber: 202, Sharp 5:23S-up, 7° up [60 EDO], 7/10-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp5v49MUp", 0xE3D6 )); // glyphnumber: 203, Sharp 5:49M-up, (one and a half apotomes)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp5v7kDown", 0xE316 )); // glyphnumber: 204, Sharp 5:7k-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp5v7kUp", 0xE31C )); // glyphnumber: 205, Sharp 5:7k-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp7CDown", 0xE312 )); // glyphnumber: 206, Sharp 7C-down, 2° up [43 EDO], 4° up [72 EDO], 1/3-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp7CUp", 0xE320 )); // glyphnumber: 207, Sharp 7C-up, 4° up [43 EDO], 8° up [72 EDO], 2/3-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp7v11CDown", 0xE34C )); // glyphnumber: 208, Sharp 7:11C-down, 4° up [60 EDO], 2/5-tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp7v11CUp", 0xE35A )); // glyphnumber: 209, Sharp 7:11C-up, 6° up [60 EDO], 3/5- tone up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp7v11kDown", 0xE352 )); // glyphnumber: 210, Sharp 7:11k-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp7v11kUp", 0xE354 )); // glyphnumber: 211, Sharp 7:11k-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp7v19CDown", 0xE3B4 )); // glyphnumber: 212, Sharp 7:19C-down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalSharp7v19CUp", 0xE3CA )); // glyphnumber: 213, Sharp 7:19C-up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalUnused1", 0xE31A )); // glyphnumber: 214, Unused
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalUnused2", 0xE31B )); // glyphnumber: 215, Unused
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalUnused3", 0xE3DE )); // glyphnumber: 216, Unused
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accSagittalUnused4", 0xE3DF )); // glyphnumber: 217, Unused
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnCombDot", 0xE8CA )); // glyphnumber: 218, Combining accordion coupler dot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnCombLH2RanksEmpty", 0xE8C8 )); // glyphnumber: 219, Combining left hand, 2 ranks, empty
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnCombLH3RanksEmptySquare", 0xE8C9 )); // glyphnumber: 220, Combining left hand, 3 ranks, empty (square)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnCombRH3RanksEmpty", 0xE8C6 )); // glyphnumber: 221, Combining right hand, 3 ranks, empty
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnCombRH4RanksEmpty", 0xE8C7 )); // glyphnumber: 222, Combining right hand, 4 ranks, empty
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnDiatonicClef", 0xE079 )); // glyphnumber: 223, Diatonic accordion clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnLH2Ranks16Round", 0xE8BC )); // glyphnumber: 224, Left hand, 2 ranks, 16' stop (round)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnLH2Ranks8Plus16Round", 0xE8BD )); // glyphnumber: 225, Left hand, 2 ranks, 8' stop + 16' stop (round)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnLH2Ranks8Round", 0xE8BB )); // glyphnumber: 226, Left hand, 2 ranks, 8' stop (round)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnLH2RanksFullMasterRound", 0xE8C0 )); // glyphnumber: 227, Left hand, 2 ranks, full master (round)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnLH2RanksMasterPlus16Round", 0xE8BF )); // glyphnumber: 228, Left hand, 2 ranks, master + 16' stop (round)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnLH2RanksMasterRound", 0xE8BE )); // glyphnumber: 229, Left hand, 2 ranks, master (round)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnLH3Ranks2Plus8Square", 0xE8C4 )); // glyphnumber: 230, Left hand, 3 ranks, 2' stop + 8' stop (square)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnLH3Ranks2Square", 0xE8C2 )); // glyphnumber: 231, Left hand, 3 ranks, 2' stop (square)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnLH3Ranks8Square", 0xE8C1 )); // glyphnumber: 232, Left hand, 3 ranks, 8' stop (square)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnLH3RanksDouble8Square", 0xE8C3 )); // glyphnumber: 233, Left hand, 3 ranks, double 8' stop (square)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnLH3RanksTuttiSquare", 0xE8C5 )); // glyphnumber: 234, Left hand, 3 ranks, 2' stop + double 8' stop (tutti) (square)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnPull", 0xE8CC )); // glyphnumber: 235, Pull
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnPush", 0xE8CB )); // glyphnumber: 236, Push
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksAccordion", 0xE8AC )); // glyphnumber: 237, Right hand, 3 ranks, 8' stop + upper tremolo 8' stop + 16' stop (accordion)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksAuthenticMusette", 0xE8A8 )); // glyphnumber: 238, Right hand, 3 ranks, lower tremolo 8' stop + 8' stop + upper tremolo 8' stop (authentic musette)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksBandoneon", 0xE8AB )); // glyphnumber: 239, Right hand, 3 ranks, 8' stop + 16' stop (bandoneón)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksBassoon", 0xE8A4 )); // glyphnumber: 240, Right hand, 3 ranks, 16' stop (bassoon)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksClarinet", 0xE8A1 )); // glyphnumber: 241, Right hand, 3 ranks, 8' stop (clarinet)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksDoubleTremoloLower8ve", 0xE8B1 )); // glyphnumber: 242, Right hand, 3 ranks, lower tremolo 8' stop + 8' stop + upper tremolo 8' stop + 16' stop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksDoubleTremoloUpper8ve", 0xE8B2 )); // glyphnumber: 243, Right hand, 3 ranks, 4' stop + lower tremolo 8' stop + 8' stop + upper tremolo 8' stop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksFullFactory", 0xE8B3 )); // glyphnumber: 244, Right hand, 3 ranks, 4' stop + lower tremolo 8' stop + 8' stop + upper tremolo 8' stop + 16' stop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksHarmonium", 0xE8AA )); // glyphnumber: 245, Right hand, 3 ranks, 4' stop + 8' stop + 16' stop (harmonium)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksImitationMusette", 0xE8A7 )); // glyphnumber: 246, Right hand, 3 ranks, 4' stop + 8' stop + upper tremolo 8' stop (imitation musette)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksLowerTremolo8", 0xE8A3 )); // glyphnumber: 247, Right hand, 3 ranks, lower tremolo 8' stop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksMaster", 0xE8AD )); // glyphnumber: 248, Right hand, 3 ranks, 4' stop + lower tremolo 8' stop + upper tremolo 8' stop + 16' stop (master)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksOboe", 0xE8A5 )); // glyphnumber: 249, Right hand, 3 ranks, 4' stop + 8' stop (oboe)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksOrgan", 0xE8A9 )); // glyphnumber: 250, Right hand, 3 ranks, 4' stop + 16' stop (organ)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksPiccolo", 0xE8A0 )); // glyphnumber: 251, Right hand, 3 ranks, 4' stop (piccolo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksTremoloLower8ve", 0xE8AF )); // glyphnumber: 252, Right hand, 3 ranks, lower tremolo 8' stop + upper tremolo 8' stop + 16' stop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksTremoloUpper8ve", 0xE8B0 )); // glyphnumber: 253, Right hand, 3 ranks, 4' stop + lower tremolo 8' stop + upper tremolo 8' stop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksTwoChoirs", 0xE8AE )); // glyphnumber: 254, Right hand, 3 ranks, lower tremolo 8' stop + upper tremolo 8' stop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksUpperTremolo8", 0xE8A2 )); // glyphnumber: 255, Right hand, 3 ranks, upper tremolo 8' stop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH3RanksViolin", 0xE8A6 )); // glyphnumber: 256, Right hand, 3 ranks, 8' stop + upper tremolo 8' stop (violin)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH4RanksAlto", 0xE8B5 )); // glyphnumber: 257, Right hand, 4 ranks, alto
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH4RanksBassAlto", 0xE8BA )); // glyphnumber: 258, Right hand, 4 ranks, bass/alto
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH4RanksMaster", 0xE8B7 )); // glyphnumber: 259, Right hand, 4 ranks, master
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH4RanksSoftBass", 0xE8B8 )); // glyphnumber: 260, Right hand, 4 ranks, soft bass
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH4RanksSoftTenor", 0xE8B9 )); // glyphnumber: 261, Right hand, 4 ranks, soft tenor
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH4RanksSoprano", 0xE8B4 )); // glyphnumber: 262, Right hand, 4 ranks, soprano
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRH4RanksTenor", 0xE8B6 )); // glyphnumber: 263, Right hand, 4 ranks, tenor
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRicochet2", 0xE8CD )); // glyphnumber: 264, Ricochet (2 tones)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRicochet3", 0xE8CE )); // glyphnumber: 265, Ricochet (3 tones)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRicochet4", 0xE8CF )); // glyphnumber: 266, Ricochet (4 tones)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRicochet5", 0xE8D0 )); // glyphnumber: 267, Ricochet (5 tones)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRicochet6", 0xE8D1 )); // glyphnumber: 268, Ricochet (6 tones)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRicochetStem2", 0xE8D2 )); // glyphnumber: 269, Combining ricochet for stem (2 tones)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRicochetStem3", 0xE8D3 )); // glyphnumber: 270, Combining ricochet for stem (3 tones)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRicochetStem4", 0xE8D4 )); // glyphnumber: 271, Combining ricochet for stem (4 tones)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRicochetStem5", 0xE8D5 )); // glyphnumber: 272, Combining ricochet for stem (5 tones)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accdnRicochetStem6", 0xE8D6 )); // glyphnumber: 273, Combining ricochet for stem (6 tones)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidental1CommaFlat", 0xE454 )); // glyphnumber: 274, 1-comma flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidental1CommaSharp", 0xE450 )); // glyphnumber: 275, 1-comma sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidental2CommaFlat", 0xE455 )); // glyphnumber: 276, 2-comma flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidental2CommaSharp", 0xE451 )); // glyphnumber: 277, 2-comma sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidental3CommaFlat", 0xE456 )); // glyphnumber: 278, 3-comma flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidental3CommaSharp", 0xE452 )); // glyphnumber: 279, 3-comma sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidental4CommaFlat", 0xE457 )); // glyphnumber: 280, 4-comma flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidental5CommaSharp", 0xE453 )); // glyphnumber: 281, 5-comma sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalArrowDown", 0xE27B )); // glyphnumber: 282, Arrow down (lower by one quarter-tone)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalArrowUp", 0xE27A )); // glyphnumber: 283, Arrow up (raise by one quarter-tone)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalBakiyeFlat", 0xE442 )); // glyphnumber: 284, Bakiye (flat)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalBakiyeSharp", 0xE445 )); // glyphnumber: 285, Bakiye (sharp)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalBracketLeft", 0xE26C )); // glyphnumber: 286, Accidental bracket, left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalBracketRight", 0xE26D )); // glyphnumber: 287, Accidental bracket, right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalBuyukMucennebFlat", 0xE440 )); // glyphnumber: 288, Büyük mücenneb (flat)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalBuyukMucennebSharp", 0xE447 )); // glyphnumber: 289, Büyük mücenneb (sharp)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCombiningCloseCurlyBrace", 0xE2EF )); // glyphnumber: 290, Combining close curly brace
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCombiningLower17Schisma", 0xE2E6 )); // glyphnumber: 291, Combining lower by one 17-limit schisma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCombiningLower19Schisma", 0xE2E8 )); // glyphnumber: 292, Combining lower by one 19-limit schisma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCombiningLower23Limit29LimitComma", 0xE2EA )); // glyphnumber: 293, Combining lower by one 23-limit comma or 29-limit comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCombiningLower31Schisma", 0xE2EC )); // glyphnumber: 294, Combining lower by one 31-limit schisma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCombiningLower53LimitComma", 0xE2F7 )); // glyphnumber: 295, Combining lower by one 53-limit comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCombiningOpenCurlyBrace", 0xE2EE )); // glyphnumber: 296, Combining open curly brace
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCombiningRaise17Schisma", 0xE2E7 )); // glyphnumber: 297, Combining raise by one 17-limit schisma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCombiningRaise19Schisma", 0xE2E9 )); // glyphnumber: 298, Combining raise by one 19-limit schisma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCombiningRaise23Limit29LimitComma", 0xE2EB )); // glyphnumber: 299, Combining raise by one 23-limit comma or 29-limit comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCombiningRaise31Schisma", 0xE2ED )); // glyphnumber: 300, Combining raise by one 31-limit schisma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCombiningRaise53LimitComma", 0xE2F8 )); // glyphnumber: 301, Combining raise by one 53-limit comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCommaSlashDown", 0xE47A )); // glyphnumber: 302, Syntonic/Didymus comma (80:81) down (Bosanquet)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalCommaSlashUp", 0xE479 )); // glyphnumber: 303, Syntonic/Didymus comma (80:81) up (Bosanquet)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleFlat", 0xE264 )); // glyphnumber: 304, Double flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleFlatArabic", 0xED30 )); // glyphnumber: 305, Arabic double flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleFlatEqualTempered", 0xE2F0 )); // glyphnumber: 306, Double flat equal tempered semitone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleFlatOneArrowDown", 0xE2C0 )); // glyphnumber: 307, Double flat lowered by one syntonic comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleFlatOneArrowUp", 0xE2C5 )); // glyphnumber: 308, Double flat raised by one syntonic comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleFlatReversed", 0xE483 )); // glyphnumber: 309, Reversed double flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleFlatThreeArrowsDown", 0xE2D4 )); // glyphnumber: 310, Double flat lowered by three syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleFlatThreeArrowsUp", 0xE2D9 )); // glyphnumber: 311, Double flat raised by three syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleFlatTurned", 0xE485 )); // glyphnumber: 312, Turned double flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleFlatTwoArrowsDown", 0xE2CA )); // glyphnumber: 313, Double flat lowered by two syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleFlatTwoArrowsUp", 0xE2CF )); // glyphnumber: 314, Double flat raised by two syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleSharp", 0xE263 )); // glyphnumber: 315, Double sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleSharpArabic", 0xED38 )); // glyphnumber: 316, Arabic double sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleSharpEqualTempered", 0xE2F4 )); // glyphnumber: 317, Double sharp equal tempered semitone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleSharpOneArrowDown", 0xE2C4 )); // glyphnumber: 318, Double sharp lowered by one syntonic comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleSharpOneArrowUp", 0xE2C9 )); // glyphnumber: 319, Double sharp raised by one syntonic comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleSharpThreeArrowsDown", 0xE2D8 )); // glyphnumber: 320, Double sharp lowered by three syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleSharpThreeArrowsUp", 0xE2DD )); // glyphnumber: 321, Double sharp raised by three syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleSharpTwoArrowsDown", 0xE2CE )); // glyphnumber: 322, Double sharp lowered by two syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalDoubleSharpTwoArrowsUp", 0xE2D3 )); // glyphnumber: 323, Double sharp raised by two syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalEnharmonicAlmostEqualTo", 0xE2FA )); // glyphnumber: 324, Enharmonically reinterpret accidental almost equal to
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalEnharmonicEquals", 0xE2FB )); // glyphnumber: 325, Enharmonically reinterpret accidental equals
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalEnharmonicTilde", 0xE2F9 )); // glyphnumber: 326, Enharmonically reinterpret accidental tilde
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFilledReversedFlatAndFlat", 0xE296 )); // glyphnumber: 327, Filled reversed flat and flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFilledReversedFlatAndFlatArrowDown", 0xE298 )); // glyphnumber: 328, Filled reversed flat and flat with arrow down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFilledReversedFlatAndFlatArrowUp", 0xE297 )); // glyphnumber: 329, Filled reversed flat and flat with arrow up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFilledReversedFlatArrowDown", 0xE293 )); // glyphnumber: 330, Filled reversed flat with arrow down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFilledReversedFlatArrowUp", 0xE292 )); // glyphnumber: 331, Filled reversed flat with arrow up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFiveQuarterTonesFlatArrowDown", 0xE279 )); // glyphnumber: 332, Five-quarter-tones flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFiveQuarterTonesSharpArrowUp", 0xE276 )); // glyphnumber: 333, Five-quarter-tones sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlat", 0xE260 )); // glyphnumber: 334, Flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlatArabic", 0xED32 )); // glyphnumber: 335, Arabic half-tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlatEqualTempered", 0xE2F1 )); // glyphnumber: 336, Flat equal tempered semitone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlatLoweredStockhausen", 0xED53 )); // glyphnumber: 337, Lowered flat (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlatOneArrowDown", 0xE2C1 )); // glyphnumber: 338, Flat lowered by one syntonic comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlatOneArrowUp", 0xE2C6 )); // glyphnumber: 339, Flat raised by one syntonic comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlatRaisedStockhausen", 0xED52 )); // glyphnumber: 340, Raised flat (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlatRepeatedLineStockhausen", 0xED5C )); // glyphnumber: 341, Repeated flat, note on line (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlatRepeatedSpaceStockhausen", 0xED5B )); // glyphnumber: 342, Repeated flat, note in space (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlatThreeArrowsDown", 0xE2D5 )); // glyphnumber: 343, Flat lowered by three syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlatThreeArrowsUp", 0xE2DA )); // glyphnumber: 344, Flat raised by three syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlatTurned", 0xE484 )); // glyphnumber: 345, Turned flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlatTwoArrowsDown", 0xE2CB )); // glyphnumber: 346, Flat lowered by two syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalFlatTwoArrowsUp", 0xE2D0 )); // glyphnumber: 347, Flat raised by two syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalHalfSharpArrowDown", 0xE29A )); // glyphnumber: 348, Half sharp with arrow down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalHalfSharpArrowUp", 0xE299 )); // glyphnumber: 349, Half sharp with arrow up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalJohnston13", 0xE2B6 )); // glyphnumber: 350, Thirteen (raise by 65:64)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalJohnston31", 0xE2B7 )); // glyphnumber: 351, Inverted 13 (lower by 65:64)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalJohnstonDown", 0xE2B5 )); // glyphnumber: 352, Down arrow (lower by 33:32)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalJohnstonEl", 0xE2B2 )); // glyphnumber: 353, Inverted seven (raise by 36:35)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalJohnstonMinus", 0xE2B1 )); // glyphnumber: 354, Minus (lower by 81:80)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalJohnstonPlus", 0xE2B0 )); // glyphnumber: 355, Plus (raise by 81:80)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalJohnstonSeven", 0xE2B3 )); // glyphnumber: 356, Seven (lower by 36:35)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalJohnstonUp", 0xE2B4 )); // glyphnumber: 357, Up arrow (raise by 33:32)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalKomaFlat", 0xE443 )); // glyphnumber: 358, Koma (flat)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalKomaSharp", 0xE444 )); // glyphnumber: 359, Koma (sharp)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalKoron", 0xE460 )); // glyphnumber: 360, Koron (quarter tone flat)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalKucukMucennebFlat", 0xE441 )); // glyphnumber: 361, Küçük mücenneb (flat)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalKucukMucennebSharp", 0xE446 )); // glyphnumber: 362, Küçük mücenneb (sharp)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalLargeDoubleSharp", 0xE47D )); // glyphnumber: 363, Large double sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalLowerOneSeptimalComma", 0xE2DE )); // glyphnumber: 364, Lower by one septimal comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalLowerOneTridecimalQuartertone", 0xE2E4 )); // glyphnumber: 365, Lower by one tridecimal quartertone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalLowerOneUndecimalQuartertone", 0xE2E2 )); // glyphnumber: 366, Lower by one undecimal quartertone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalLowerTwoSeptimalCommas", 0xE2E0 )); // glyphnumber: 367, Lower by two septimal commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalLoweredStockhausen", 0xED51 )); // glyphnumber: 368, Lowered (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNarrowReversedFlat", 0xE284 )); // glyphnumber: 369, Narrow reversed flat(quarter-tone flat)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNarrowReversedFlatAndFlat", 0xE285 )); // glyphnumber: 370, Narrow reversed flat and flat(three-quarter-tones flat)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNatural", 0xE261 )); // glyphnumber: 371, Natural
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNaturalArabic", 0xED34 )); // glyphnumber: 372, Arabic natural
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNaturalEqualTempered", 0xE2F2 )); // glyphnumber: 373, Natural equal tempered semitone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNaturalFlat", 0xE267 )); // glyphnumber: 374, Natural flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNaturalLoweredStockhausen", 0xED55 )); // glyphnumber: 375, Lowered natural (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNaturalOneArrowDown", 0xE2C2 )); // glyphnumber: 376, Natural lowered by one syntonic comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNaturalOneArrowUp", 0xE2C7 )); // glyphnumber: 377, Natural raised by one syntonic comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNaturalRaisedStockhausen", 0xED54 )); // glyphnumber: 378, Raised natural (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNaturalReversed", 0xE482 )); // glyphnumber: 379, Reversed natural
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNaturalSharp", 0xE268 )); // glyphnumber: 380, Natural sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNaturalThreeArrowsDown", 0xE2D6 )); // glyphnumber: 381, Natural lowered by three syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNaturalThreeArrowsUp", 0xE2DB )); // glyphnumber: 382, Natural raised by three syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNaturalTwoArrowsDown", 0xE2CC )); // glyphnumber: 383, Natural lowered by two syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalNaturalTwoArrowsUp", 0xE2D1 )); // glyphnumber: 384, Natural raised by two syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalOneAndAHalfSharpsArrowDown", 0xE29C )); // glyphnumber: 385, One and a half sharps with arrow down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalOneAndAHalfSharpsArrowUp", 0xE29B )); // glyphnumber: 386, One and a half sharps with arrow up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalOneQuarterToneFlatFerneyhough", 0xE48F )); // glyphnumber: 387, One-quarter-tone flat (Ferneyhough)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalOneQuarterToneFlatStockhausen", 0xED59 )); // glyphnumber: 388, One-quarter-tone flat (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalOneQuarterToneSharpFerneyhough", 0xE48E )); // glyphnumber: 389, One-quarter-tone sharp (Ferneyhough)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalOneQuarterToneSharpStockhausen", 0xED58 )); // glyphnumber: 390, One-quarter-tone sharp (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalOneThirdToneFlatFerneyhough", 0xE48B )); // glyphnumber: 391, One-third-tone flat (Ferneyhough)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalOneThirdToneSharpFerneyhough", 0xE48A )); // glyphnumber: 392, One-third-tone sharp (Ferneyhough)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalParensLeft", 0xE26A )); // glyphnumber: 393, Accidental parenthesis, left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalParensRight", 0xE26B )); // glyphnumber: 394, Accidental parenthesis, right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterFlatEqualTempered", 0xE2F5 )); // glyphnumber: 395, Lower by one equal tempered quarter-tone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterSharpEqualTempered", 0xE2F6 )); // glyphnumber: 396, Raise by one equal tempered quarter tone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneFlat4", 0xE47F )); // glyphnumber: 397, Quarter-tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneFlatArabic", 0xED33 )); // glyphnumber: 398, Arabic quarter-tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneFlatArrowUp", 0xE270 )); // glyphnumber: 399, Quarter-tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneFlatFilledReversed", 0xE480 )); // glyphnumber: 400, Filled reversed flat (quarter-tone flat)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneFlatNaturalArrowDown", 0xE273 )); // glyphnumber: 401, Quarter-tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneFlatPenderecki", 0xE478 )); // glyphnumber: 402, Quarter tone flat (Penderecki)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneFlatStein", 0xE280 )); // glyphnumber: 403, Reversed flat (quarter-tone flat) (Stein)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneFlatVanBlankenburg", 0xE488 )); // glyphnumber: 404, Quarter-tone flat (van Blankenburg)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneSharp4", 0xE47E )); // glyphnumber: 405, Quarter-tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneSharpArabic", 0xED35 )); // glyphnumber: 406, Arabic quarter-tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneSharpArrowDown", 0xE275 )); // glyphnumber: 407, Quarter-tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneSharpBusotti", 0xE472 )); // glyphnumber: 408, Quarter tone sharp (Bussotti)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneSharpNaturalArrowUp", 0xE272 )); // glyphnumber: 409, Quarter-tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneSharpStein", 0xE282 )); // glyphnumber: 410, Half sharp (quarter-tone sharp) (Stein)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalQuarterToneSharpWiggle", 0xE475 )); // glyphnumber: 411, Quarter tone sharp with wiggly tail
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalRaiseOneSeptimalComma", 0xE2DF )); // glyphnumber: 412, Raise by one septimal comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalRaiseOneTridecimalQuartertone", 0xE2E5 )); // glyphnumber: 413, Raise by one tridecimal quartertone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalRaiseOneUndecimalQuartertone", 0xE2E3 )); // glyphnumber: 414, Raise by one undecimal quartertone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalRaiseTwoSeptimalCommas", 0xE2E1 )); // glyphnumber: 415, Raise by two septimal commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalRaisedStockhausen", 0xED50 )); // glyphnumber: 416, Raised (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalReversedFlatAndFlatArrowDown", 0xE295 )); // glyphnumber: 417, Reversed flat and flat with arrow down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalReversedFlatAndFlatArrowUp", 0xE294 )); // glyphnumber: 418, Reversed flat and flat with arrow up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalReversedFlatArrowDown", 0xE291 )); // glyphnumber: 419, Reversed flat with arrow down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalReversedFlatArrowUp", 0xE290 )); // glyphnumber: 420, Reversed flat with arrow up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharp", 0xE262 )); // glyphnumber: 421, Sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpArabic", 0xED36 )); // glyphnumber: 422, Arabic half-tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpEqualTempered", 0xE2F3 )); // glyphnumber: 423, Sharp equal tempered semitone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpLoweredStockhausen", 0xED57 )); // glyphnumber: 424, Lowered sharp (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpOneArrowDown", 0xE2C3 )); // glyphnumber: 425, Sharp lowered by one syntonic comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpOneArrowUp", 0xE2C8 )); // glyphnumber: 426, Sharp raised by one syntonic comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpOneHorizontalStroke", 0xE473 )); // glyphnumber: 427, One or three quarter tones sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpRaisedStockhausen", 0xED56 )); // glyphnumber: 428, Raised sharp (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpRepeatedLineStockhausen", 0xED5E )); // glyphnumber: 429, Repeated sharp, note on line (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpRepeatedSpaceStockhausen", 0xED5D )); // glyphnumber: 430, Repeated sharp, note in space (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpReversed", 0xE481 )); // glyphnumber: 431, Reversed sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpSharp", 0xE269 )); // glyphnumber: 432, Sharp sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpThreeArrowsDown", 0xE2D7 )); // glyphnumber: 433, Sharp lowered by three syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpThreeArrowsUp", 0xE2DC )); // glyphnumber: 434, Sharp raised by three syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpTwoArrowsDown", 0xE2CD )); // glyphnumber: 435, Sharp lowered by two syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSharpTwoArrowsUp", 0xE2D2 )); // glyphnumber: 436, Sharp raised by two syntonic commas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSims12Down", 0xE2A0 )); // glyphnumber: 437, 1/12 tone low
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSims12Up", 0xE2A3 )); // glyphnumber: 438, 1/12 tone high
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSims4Down", 0xE2A2 )); // glyphnumber: 439, 1/4 tone low
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSims4Up", 0xE2A5 )); // glyphnumber: 440, 1/4 tone high
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSims6Down", 0xE2A1 )); // glyphnumber: 441, 1/6 tone low
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSims6Up", 0xE2A4 )); // glyphnumber: 442, 1/6 tone high
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalSori", 0xE461 )); // glyphnumber: 443, Sori (quarter tone sharp)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalTavenerFlat", 0xE477 )); // glyphnumber: 444, Byzantine-style Bakiye flat (Tavener)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalTavenerSharp", 0xE476 )); // glyphnumber: 445, Byzantine-style Büyük mücenneb sharp (Tavener)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalThreeQuarterTonesFlatArabic", 0xED31 )); // glyphnumber: 446, Arabic three-quarter-tones flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalThreeQuarterTonesFlatArrowDown", 0xE271 )); // glyphnumber: 447, Three-quarter-tones flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalThreeQuarterTonesFlatArrowUp", 0xE278 )); // glyphnumber: 448, Three-quarter-tones flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalThreeQuarterTonesFlatCouper", 0xE489 )); // glyphnumber: 449, Three-quarter-tones flat (Couper)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalThreeQuarterTonesFlatGrisey", 0xE486 )); // glyphnumber: 450, Three-quarter-tones flat (Grisey)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalThreeQuarterTonesFlatTartini", 0xE487 )); // glyphnumber: 451, Three-quarter-tones flat (Tartini)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalThreeQuarterTonesFlatZimmermann", 0xE281 )); // glyphnumber: 452, Reversed flat and flat (three-quarter-tones flat) (Zimmermann)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalThreeQuarterTonesSharpArabic", 0xED37 )); // glyphnumber: 453, Arabic three-quarter-tones sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalThreeQuarterTonesSharpArrowDown", 0xE277 )); // glyphnumber: 454, Three-quarter-tones sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalThreeQuarterTonesSharpArrowUp", 0xE274 )); // glyphnumber: 455, Three-quarter-tones sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalThreeQuarterTonesSharpBusotti", 0xE474 )); // glyphnumber: 456, Three quarter tones sharp (Bussotti)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalThreeQuarterTonesSharpStein", 0xE283 )); // glyphnumber: 457, One and a half sharps (three-quarter-tones sharp) (Stein)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalThreeQuarterTonesSharpStockhausen", 0xED5A )); // glyphnumber: 458, Three-quarter-tones sharp (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalTripleFlat", 0xE266 )); // glyphnumber: 459, Triple flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalTripleSharp", 0xE265 )); // glyphnumber: 460, Triple sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalTwoThirdTonesFlatFerneyhough", 0xE48D )); // glyphnumber: 461, Two-third-tones flat (Ferneyhough)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalTwoThirdTonesSharpFerneyhough", 0xE48C )); // glyphnumber: 462, Two-third-tones sharp (Ferneyhough)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWilsonMinus", 0xE47C )); // glyphnumber: 463, Wilson minus (5 comma down)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWilsonPlus", 0xE47B )); // glyphnumber: 464, Wilson plus (5 comma up)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky10TwelfthsFlat", 0xE434 )); // glyphnumber: 465, 5/6 tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky10TwelfthsSharp", 0xE429 )); // glyphnumber: 466, 5/6 tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky11TwelfthsFlat", 0xE435 )); // glyphnumber: 467, 11/12 tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky11TwelfthsSharp", 0xE42A )); // glyphnumber: 468, 11/12 tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky1TwelfthsFlat", 0xE42B )); // glyphnumber: 469, 1/12 tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky1TwelfthsSharp", 0xE420 )); // glyphnumber: 470, 1/12 tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky2TwelfthsFlat", 0xE42C )); // glyphnumber: 471, 1/6 tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky2TwelfthsSharp", 0xE421 )); // glyphnumber: 472, 1/6 tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky3TwelfthsFlat", 0xE42D )); // glyphnumber: 473, 1/4 tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky3TwelfthsSharp", 0xE422 )); // glyphnumber: 474, 1/4 tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky4TwelfthsFlat", 0xE42E )); // glyphnumber: 475, 1/3 tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky4TwelfthsSharp", 0xE423 )); // glyphnumber: 476, 1/3 tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky5TwelfthsFlat", 0xE42F )); // glyphnumber: 477, 5/12 tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky5TwelfthsSharp", 0xE424 )); // glyphnumber: 478, 5/12 tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky6TwelfthsFlat", 0xE430 )); // glyphnumber: 479, 1/2 tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky6TwelfthsSharp", 0xE425 )); // glyphnumber: 480, 1/2 tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky7TwelfthsFlat", 0xE431 )); // glyphnumber: 481, 7/12 tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky7TwelfthsSharp", 0xE426 )); // glyphnumber: 482, 7/12 tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky8TwelfthsFlat", 0xE432 )); // glyphnumber: 483, 2/3 tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky8TwelfthsSharp", 0xE427 )); // glyphnumber: 484, 2/3 tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky9TwelfthsFlat", 0xE433 )); // glyphnumber: 485, 3/4 tone flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalWyschnegradsky9TwelfthsSharp", 0xE428 )); // glyphnumber: 486, 3/4 tone sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalXenakisOneThirdToneSharp", 0xE470 )); // glyphnumber: 487, One-third-tone sharp (Xenakis)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "accidentalXenakisTwoThirdTonesSharp", 0xE471 )); // glyphnumber: 488, Two-third-tones sharp (Xenakis)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "analyticsChoralmelodie", 0xE86A )); // glyphnumber: 489, Choralmelodie (Berg)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "analyticsEndStimme", 0xE863 )); // glyphnumber: 490, End of stimme
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "analyticsHauptrhythmus", 0xE86B )); // glyphnumber: 491, Hauptrhythmus (Berg)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "analyticsHauptstimme", 0xE860 )); // glyphnumber: 492, Hauptstimme
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "analyticsInversion1", 0xE869 )); // glyphnumber: 493, Inversion 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "analyticsNebenstimme", 0xE861 )); // glyphnumber: 494, Nebenstimme
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "analyticsStartStimme", 0xE862 )); // glyphnumber: 495, Start of stimme
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "analyticsTheme", 0xE864 )); // glyphnumber: 496, Theme
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "analyticsTheme1", 0xE868 )); // glyphnumber: 497, Theme 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "analyticsThemeInversion", 0xE867 )); // glyphnumber: 498, Inversion of theme
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "analyticsThemeRetrograde", 0xE865 )); // glyphnumber: 499, Retrograde of theme
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "analyticsThemeRetrogradeInversion", 0xE866 )); // glyphnumber: 500, Retrograde inversion of theme
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arpeggiatoDown", 0xE635 )); // glyphnumber: 501, Arpeggiato down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arpeggiatoUp", 0xE634 )); // glyphnumber: 502, Arpeggiato up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowBlackDown", 0xEB64 )); // glyphnumber: 503, Black arrow down (S)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowBlackDownLeft", 0xEB65 )); // glyphnumber: 504, Black arrow down-left (SW)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowBlackDownRight", 0xEB63 )); // glyphnumber: 505, Black arrow down-right (SE)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowBlackLeft", 0xEB66 )); // glyphnumber: 506, Black arrow left (W)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowBlackRight", 0xEB62 )); // glyphnumber: 507, Black arrow right (E)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowBlackUp", 0xEB60 )); // glyphnumber: 508, Black arrow up (N)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowBlackUpLeft", 0xEB67 )); // glyphnumber: 509, Black arrow up-left (NW)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowBlackUpRight", 0xEB61 )); // glyphnumber: 510, Black arrow up-right (NE)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowOpenDown", 0xEB74 )); // glyphnumber: 511, Open arrow down (S)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowOpenDownLeft", 0xEB75 )); // glyphnumber: 512, Open arrow down-left (SW)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowOpenDownRight", 0xEB73 )); // glyphnumber: 513, Open arrow down-right (SE)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowOpenLeft", 0xEB76 )); // glyphnumber: 514, Open arrow left (W)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowOpenRight", 0xEB72 )); // glyphnumber: 515, Open arrow right (E)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowOpenUp", 0xEB70 )); // glyphnumber: 516, Open arrow up (N)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowOpenUpLeft", 0xEB77 )); // glyphnumber: 517, Open arrow up-left (NW)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowOpenUpRight", 0xEB71 )); // glyphnumber: 518, Open arrow up-right (NE)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowWhiteDown", 0xEB6C )); // glyphnumber: 519, White arrow down (S)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowWhiteDownLeft", 0xEB6D )); // glyphnumber: 520, White arrow down-left (SW)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowWhiteDownRight", 0xEB6B )); // glyphnumber: 521, White arrow down-right (SE)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowWhiteLeft", 0xEB6E )); // glyphnumber: 522, White arrow left (W)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowWhiteRight", 0xEB6A )); // glyphnumber: 523, White arrow right (E)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowWhiteUp", 0xEB68 )); // glyphnumber: 524, White arrow up (N)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowWhiteUpLeft", 0xEB6F )); // glyphnumber: 525, White arrow up-left (NW)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowWhiteUpRight", 0xEB69 )); // glyphnumber: 526, White arrow up-right (NE)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadBlackDown", 0xEB7C )); // glyphnumber: 527, Black arrowhead down (S)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadBlackDownLeft", 0xEB7D )); // glyphnumber: 528, Black arrowhead down-left (SW)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadBlackDownRight", 0xEB7B )); // glyphnumber: 529, Black arrowhead down-right (SE)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadBlackLeft", 0xEB7E )); // glyphnumber: 530, Black arrowhead left (W)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadBlackRight", 0xEB7A )); // glyphnumber: 531, Black arrowhead right (E)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadBlackUp", 0xEB78 )); // glyphnumber: 532, Black arrowhead up (N)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadBlackUpLeft", 0xEB7F )); // glyphnumber: 533, Black arrowhead up-left (NW)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadBlackUpRight", 0xEB79 )); // glyphnumber: 534, Black arrowhead up-right (NE)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadOpenDown", 0xEB8C )); // glyphnumber: 535, Open arrowhead down (S)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadOpenDownLeft", 0xEB8D )); // glyphnumber: 536, Open arrowhead down-left (SW)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadOpenDownRight", 0xEB8B )); // glyphnumber: 537, Open arrowhead down-right (SE)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadOpenLeft", 0xEB8E )); // glyphnumber: 538, Open arrowhead left (W)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadOpenRight", 0xEB8A )); // glyphnumber: 539, Open arrowhead right (E)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadOpenUp", 0xEB88 )); // glyphnumber: 540, Open arrowhead up (N)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadOpenUpLeft", 0xEB8F )); // glyphnumber: 541, Open arrowhead up-left (NW)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadOpenUpRight", 0xEB89 )); // glyphnumber: 542, Open arrowhead up-right (NE)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadWhiteDown", 0xEB84 )); // glyphnumber: 543, White arrowhead down (S)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadWhiteDownLeft", 0xEB85 )); // glyphnumber: 544, White arrowhead down-left (SW)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadWhiteDownRight", 0xEB83 )); // glyphnumber: 545, White arrowhead down-right (SE)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadWhiteLeft", 0xEB86 )); // glyphnumber: 546, White arrowhead left (W)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadWhiteRight", 0xEB82 )); // glyphnumber: 547, White arrowhead right (E)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadWhiteUp", 0xEB80 )); // glyphnumber: 548, White arrowhead up (N)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadWhiteUpLeft", 0xEB87 )); // glyphnumber: 549, White arrowhead up-left (NW)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "arrowheadWhiteUpRight", 0xEB81 )); // glyphnumber: 550, White arrowhead up-right (NE)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articAccentAbove", 0xE4A0 )); // glyphnumber: 551, Accent above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articAccentBelow", 0xE4A1 )); // glyphnumber: 552, Accent below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articAccentStaccatoAbove", 0xE4B0 )); // glyphnumber: 553, Accent-staccato above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articAccentStaccatoBelow", 0xE4B1 )); // glyphnumber: 554, Accent-staccato below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articLaissezVibrerAbove", 0xE4BA )); // glyphnumber: 555, Laissez vibrer (l.v.) above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articLaissezVibrerBelow", 0xE4BB )); // glyphnumber: 556, Laissez vibrer (l.v.) below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articMarcatoAbove", 0xE4AC )); // glyphnumber: 557, Marcato above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articMarcatoBelow", 0xE4AD )); // glyphnumber: 558, Marcato below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articMarcatoStaccatoAbove", 0xE4AE )); // glyphnumber: 559, Marcato-staccato above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articMarcatoStaccatoBelow", 0xE4AF )); // glyphnumber: 560, Marcato-staccato below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articMarcatoTenutoAbove", 0xE4BC )); // glyphnumber: 561, Marcato-tenuto above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articMarcatoTenutoBelow", 0xE4BD )); // glyphnumber: 562, Marcato-tenuto below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articSoftAccentAbove", 0xED40 )); // glyphnumber: 563, Soft accent above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articSoftAccentBelow", 0xED41 )); // glyphnumber: 564, Soft accent below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articSoftAccentStaccatoAbove", 0xED42 )); // glyphnumber: 565, Soft accent-staccato above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articSoftAccentStaccatoBelow", 0xED43 )); // glyphnumber: 566, Soft accent-staccato below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articSoftAccentTenutoAbove", 0xED44 )); // glyphnumber: 567, Soft accent-tenuto above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articSoftAccentTenutoBelow", 0xED45 )); // glyphnumber: 568, Soft accent-tenuto below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articSoftAccentTenutoStaccatoAbove", 0xED46 )); // glyphnumber: 569, Soft accent-tenuto-staccato above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articSoftAccentTenutoStaccatoBelow", 0xED47 )); // glyphnumber: 570, Soft accent-tenuto-staccato below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articStaccatissimoAbove", 0xE4A6 )); // glyphnumber: 571, Staccatissimo above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articStaccatissimoBelow", 0xE4A7 )); // glyphnumber: 572, Staccatissimo below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articStaccatissimoStrokeAbove", 0xE4AA )); // glyphnumber: 573, Staccatissimo stroke above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articStaccatissimoStrokeBelow", 0xE4AB )); // glyphnumber: 574, Staccatissimo stroke below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articStaccatissimoWedgeAbove", 0xE4A8 )); // glyphnumber: 575, Staccatissimo wedge above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articStaccatissimoWedgeBelow", 0xE4A9 )); // glyphnumber: 576, Staccatissimo wedge below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articStaccatoAbove", 0xE4A2 )); // glyphnumber: 577, Staccato above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articStaccatoBelow", 0xE4A3 )); // glyphnumber: 578, Staccato below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articStressAbove", 0xE4B6 )); // glyphnumber: 579, Stress above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articStressBelow", 0xE4B7 )); // glyphnumber: 580, Stress below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articTenutoAbove", 0xE4A4 )); // glyphnumber: 581, Tenuto above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articTenutoAccentAbove", 0xE4B4 )); // glyphnumber: 582, Tenuto-accent above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articTenutoAccentBelow", 0xE4B5 )); // glyphnumber: 583, Tenuto-accent below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articTenutoBelow", 0xE4A5 )); // glyphnumber: 584, Tenuto below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articTenutoStaccatoAbove", 0xE4B2 )); // glyphnumber: 585, Louré (tenuto-staccato) above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articTenutoStaccatoBelow", 0xE4B3 )); // glyphnumber: 586, Louré (tenuto-staccato) below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articUnstressAbove", 0xE4B8 )); // glyphnumber: 587, Unstress above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "articUnstressBelow", 0xE4B9 )); // glyphnumber: 588, Unstress below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "augmentationDot", 0xE1E7 )); // glyphnumber: 589, Augmentation dot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "barlineDashed", 0xE036 )); // glyphnumber: 590, Dashed barline
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "barlineDotted", 0xE037 )); // glyphnumber: 591, Dotted barline
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "barlineDouble", 0xE031 )); // glyphnumber: 592, Double barline
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "barlineFinal", 0xE032 )); // glyphnumber: 593, Final barline
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "barlineHeavy", 0xE034 )); // glyphnumber: 594, Heavy barline
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "barlineHeavyHeavy", 0xE035 )); // glyphnumber: 595, Heavy double barline
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "barlineReverseFinal", 0xE033 )); // glyphnumber: 596, Reverse final barline
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "barlineShort", 0xE038 )); // glyphnumber: 597, Short barline
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "barlineSingle", 0xE030 )); // glyphnumber: 598, Single barline
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "barlineTick", 0xE039 )); // glyphnumber: 599, Tick barline
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit1", 0xEAF4 )); // glyphnumber: 600, Accel./rit. beam 1 (widest)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit10", 0xEAFD )); // glyphnumber: 601, Accel./rit. beam 10
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit11", 0xEAFE )); // glyphnumber: 602, Accel./rit. beam 11
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit12", 0xEAFF )); // glyphnumber: 603, Accel./rit. beam 12
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit13", 0xEB00 )); // glyphnumber: 604, Accel./rit. beam 13
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit14", 0xEB01 )); // glyphnumber: 605, Accel./rit. beam 14
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit15", 0xEB02 )); // glyphnumber: 606, Accel./rit. beam 15 (narrowest)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit2", 0xEAF5 )); // glyphnumber: 607, Accel./rit. beam 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit3", 0xEAF6 )); // glyphnumber: 608, Accel./rit. beam 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit4", 0xEAF7 )); // glyphnumber: 609, Accel./rit. beam 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit5", 0xEAF8 )); // glyphnumber: 610, Accel./rit. beam 5
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit6", 0xEAF9 )); // glyphnumber: 611, Accel./rit. beam 6
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit7", 0xEAFA )); // glyphnumber: 612, Accel./rit. beam 7
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit8", 0xEAFB )); // glyphnumber: 613, Accel./rit. beam 8
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRit9", 0xEAFC )); // glyphnumber: 614, Accel./rit. beam 9
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "beamAccelRitFinal", 0xEB03 )); // glyphnumber: 615, Accel./rit. beam terminating line
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brace", 0xE000 )); // glyphnumber: 616, Brace
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "bracket", 0xE002 )); // glyphnumber: 617, Bracket
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "bracketBottom", 0xE004 )); // glyphnumber: 618, Bracket bottom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "bracketTop", 0xE003 )); // glyphnumber: 619, Bracket top
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassBend", 0xE5E3 )); // glyphnumber: 620, Bend
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassDoitLong", 0xE5D6 )); // glyphnumber: 621, Doit, long
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassDoitMedium", 0xE5D5 )); // glyphnumber: 622, Doit, medium
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassDoitShort", 0xE5D4 )); // glyphnumber: 623, Doit, short
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassFallLipLong", 0xE5D9 )); // glyphnumber: 624, Lip fall, long
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassFallLipMedium", 0xE5D8 )); // glyphnumber: 625, Lip fall, medium
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassFallLipShort", 0xE5D7 )); // glyphnumber: 626, Lip fall, short
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassFallRoughLong", 0xE5DF )); // glyphnumber: 627, Rough fall, long
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassFallRoughMedium", 0xE5DE )); // glyphnumber: 628, Rough fall, medium
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassFallRoughShort", 0xE5DD )); // glyphnumber: 629, Rough fall, short
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassFallSmoothLong", 0xE5DC )); // glyphnumber: 630, Smooth fall, long
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassFallSmoothMedium", 0xE5DB )); // glyphnumber: 631, Smooth fall, medium
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassFallSmoothShort", 0xE5DA )); // glyphnumber: 632, Smooth fall, short
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassFlip", 0xE5E1 )); // glyphnumber: 633, Flip
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassHarmonMuteClosed", 0xE5E8 )); // glyphnumber: 634, Harmon mute, stem in
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassHarmonMuteStemHalfLeft", 0xE5E9 )); // glyphnumber: 635, Harmon mute, stem extended, left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassHarmonMuteStemHalfRight", 0xE5EA )); // glyphnumber: 636, Harmon mute, stem extended, right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassHarmonMuteStemOpen", 0xE5EB )); // glyphnumber: 637, Harmon mute, stem out
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassJazzTurn", 0xE5E4 )); // glyphnumber: 638, Jazz turn
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassLiftLong", 0xE5D3 )); // glyphnumber: 639, Lift, long
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassLiftMedium", 0xE5D2 )); // glyphnumber: 640, Lift, medium
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassLiftShort", 0xE5D1 )); // glyphnumber: 641, Lift, short
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassLiftSmoothLong", 0xE5EE )); // glyphnumber: 642, Smooth lift, long
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassLiftSmoothMedium", 0xE5ED )); // glyphnumber: 643, Smooth lift, medium
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassLiftSmoothShort", 0xE5EC )); // glyphnumber: 644, Smooth lift, short
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassMuteClosed", 0xE5E5 )); // glyphnumber: 645, Muted (closed)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassMuteHalfClosed", 0xE5E6 )); // glyphnumber: 646, Half-muted (half-closed)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassMuteOpen", 0xE5E7 )); // glyphnumber: 647, Open
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassPlop", 0xE5E0 )); // glyphnumber: 648, Plop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassScoop", 0xE5D0 )); // glyphnumber: 649, Scoop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassSmear", 0xE5E2 )); // glyphnumber: 650, Smear
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "brassValveTrill", 0xE5EF )); // glyphnumber: 651, Valve trill
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "breathMarkComma", 0xE4CE )); // glyphnumber: 652, Breath mark (comma)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "breathMarkSalzedo", 0xE4D5 )); // glyphnumber: 653, Breath mark (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "breathMarkTick", 0xE4CF )); // glyphnumber: 654, Breath mark (tick-like)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "breathMarkUpbow", 0xE4D0 )); // glyphnumber: 655, Breath mark (upbow-like)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "bridgeClef", 0xE078 )); // glyphnumber: 656, Bridge clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "buzzRoll", 0xE22A )); // glyphnumber: 657, Buzz roll
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "cClef", 0xE05C )); // glyphnumber: 658, C clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "cClef8vb", 0xE05D )); // glyphnumber: 659, C clef ottava bassa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "cClefArrowDown", 0xE05F )); // glyphnumber: 660, C clef, arrow down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "cClefArrowUp", 0xE05E )); // glyphnumber: 661, C clef, arrow up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "cClefChange", 0xE07B )); // glyphnumber: 662, C clef change
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "cClefCombining", 0xE061 )); // glyphnumber: 663, Combining C clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "cClefReversed", 0xE075 )); // glyphnumber: 664, Reversed C clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "cClefSquare", 0xE060 )); // glyphnumber: 665, C clef (19th century)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "caesura", 0xE4D1 )); // glyphnumber: 666, Caesura
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "caesuraCurved", 0xE4D4 )); // glyphnumber: 667, Curved caesura
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "caesuraShort", 0xE4D3 )); // glyphnumber: 668, Short caesura
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "caesuraThick", 0xE4D2 )); // glyphnumber: 669, Thick caesura
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantAccentusAbove", 0xE9D6 )); // glyphnumber: 670, Accentus above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantAccentusBelow", 0xE9D7 )); // glyphnumber: 671, Accentus below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantAuctumAsc", 0xE994 )); // glyphnumber: 672, Punctum auctum, ascending
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantAuctumDesc", 0xE995 )); // glyphnumber: 673, Punctum auctum, descending
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantAugmentum", 0xE9D9 )); // glyphnumber: 674, Augmentum (mora)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantCaesura", 0xE8F8 )); // glyphnumber: 675, Caesura
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantCclef", 0xE906 )); // glyphnumber: 676, Plainchant C clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantCirculusAbove", 0xE9D2 )); // glyphnumber: 677, Circulus above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantCirculusBelow", 0xE9D3 )); // glyphnumber: 678, Circulus below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantConnectingLineAsc2nd", 0xE9BD )); // glyphnumber: 679, Connecting line, ascending 2nd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantConnectingLineAsc3rd", 0xE9BE )); // glyphnumber: 680, Connecting line, ascending 3rd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantConnectingLineAsc4th", 0xE9BF )); // glyphnumber: 681, Connecting line, ascending 4th
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantConnectingLineAsc5th", 0xE9C0 )); // glyphnumber: 682, Connecting line, ascending 5th
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantConnectingLineAsc6th", 0xE9C1 )); // glyphnumber: 683, Connecting line, ascending 6th
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantCustosStemDownPosHigh", 0xEA08 )); // glyphnumber: 684, Plainchant custos, stem down, high position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantCustosStemDownPosHighest", 0xEA09 )); // glyphnumber: 685, Plainchant custos, stem down, highest position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantCustosStemDownPosMiddle", 0xEA07 )); // glyphnumber: 686, Plainchant custos, stem down, middle position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantCustosStemUpPosLow", 0xEA05 )); // glyphnumber: 687, Plainchant custos, stem up, low position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantCustosStemUpPosLowest", 0xEA04 )); // glyphnumber: 688, Plainchant custos, stem up, lowest position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantCustosStemUpPosMiddle", 0xEA06 )); // glyphnumber: 689, Plainchant custos, stem up, middle position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantDeminutumLower", 0xE9B3 )); // glyphnumber: 690, Punctum deminutum, lower
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantDeminutumUpper", 0xE9B2 )); // glyphnumber: 691, Punctum deminutum, upper
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantDivisioFinalis", 0xE8F6 )); // glyphnumber: 692, Divisio finalis
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantDivisioMaior", 0xE8F4 )); // glyphnumber: 693, Divisio maior
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantDivisioMaxima", 0xE8F5 )); // glyphnumber: 694, Divisio maxima
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantDivisioMinima", 0xE8F3 )); // glyphnumber: 695, Divisio minima
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantEntryLineAsc2nd", 0xE9B4 )); // glyphnumber: 696, Entry line, ascending 2nd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantEntryLineAsc3rd", 0xE9B5 )); // glyphnumber: 697, Entry line, ascending 3rd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantEntryLineAsc4th", 0xE9B6 )); // glyphnumber: 698, Entry line, ascending 4th
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantEntryLineAsc5th", 0xE9B7 )); // glyphnumber: 699, Entry line, ascending 5th
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantEntryLineAsc6th", 0xE9B8 )); // glyphnumber: 700, Entry line, ascending 6th
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantEpisema", 0xE9D8 )); // glyphnumber: 701, Episema
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantFclef", 0xE902 )); // glyphnumber: 702, Plainchant F clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantIctusAbove", 0xE9D0 )); // glyphnumber: 703, Ictus above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantIctusBelow", 0xE9D1 )); // glyphnumber: 704, Ictus below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantLigaturaDesc2nd", 0xE9B9 )); // glyphnumber: 705, Ligated stroke, descending 2nd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantLigaturaDesc3rd", 0xE9BA )); // glyphnumber: 706, Ligated stroke, descending 3rd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantLigaturaDesc4th", 0xE9BB )); // glyphnumber: 707, Ligated stroke, descending 4th
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantLigaturaDesc5th", 0xE9BC )); // glyphnumber: 708, Ligated stroke, descending 5th
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantOriscusAscending", 0xE99C )); // glyphnumber: 709, Oriscus ascending
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantOriscusDescending", 0xE99D )); // glyphnumber: 710, Oriscus descending
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantOriscusLiquescens", 0xE99E )); // glyphnumber: 711, Oriscus liquescens
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantPodatusLower", 0xE9B0 )); // glyphnumber: 712, Podatus, lower
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantPodatusUpper", 0xE9B1 )); // glyphnumber: 713, Podatus, upper
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantPunctum", 0xE990 )); // glyphnumber: 714, Punctum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantPunctumCavum", 0xE998 )); // glyphnumber: 715, Punctum cavum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantPunctumDeminutum", 0xE9A1 )); // glyphnumber: 716, Punctum deminutum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantPunctumInclinatum", 0xE991 )); // glyphnumber: 717, Punctum inclinatum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantPunctumInclinatumAuctum", 0xE992 )); // glyphnumber: 718, Punctum inclinatum auctum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantPunctumInclinatumDeminutum", 0xE993 )); // glyphnumber: 719, Punctum inclinatum deminutum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantPunctumLinea", 0xE999 )); // glyphnumber: 720, Punctum linea
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantPunctumLineaCavum", 0xE99A )); // glyphnumber: 721, Punctum linea cavum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantPunctumVirga", 0xE996 )); // glyphnumber: 722, Punctum virga
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantPunctumVirgaReversed", 0xE997 )); // glyphnumber: 723, Punctum virga, reversed
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantQuilisma", 0xE99B )); // glyphnumber: 724, Quilisma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantSemicirculusAbove", 0xE9D4 )); // glyphnumber: 725, Semicirculus above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantSemicirculusBelow", 0xE9D5 )); // glyphnumber: 726, Semicirculus below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantStaff", 0xE8F0 )); // glyphnumber: 727, Plainchant staff
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantStaffNarrow", 0xE8F2 )); // glyphnumber: 728, Plainchant staff (narrow)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantStaffWide", 0xE8F1 )); // glyphnumber: 729, Plainchant staff (wide)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantStrophicus", 0xE99F )); // glyphnumber: 730, Strophicus
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantStrophicusAuctus", 0xE9A0 )); // glyphnumber: 731, Strophicus auctus
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantStrophicusLiquescens2nd", 0xE9C2 )); // glyphnumber: 732, Strophicus liquescens, 2nd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantStrophicusLiquescens3rd", 0xE9C3 )); // glyphnumber: 733, Strophicus liquescens, 3rd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantStrophicusLiquescens4th", 0xE9C4 )); // glyphnumber: 734, Strophicus liquescens, 4th
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantStrophicusLiquescens5th", 0xE9C5 )); // glyphnumber: 735, Strophicus liquescens, 5th
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "chantVirgula", 0xE8F7 )); // glyphnumber: 736, Virgula
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "clef15", 0xE07E )); // glyphnumber: 737, 15 for clefs
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "clef8", 0xE07D )); // glyphnumber: 738, 8 for clefs
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "clefChangeCombining", 0xE07F )); // glyphnumber: 739, Combining clef change
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "coda", 0xE048 )); // glyphnumber: 740, Coda
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "codaSquare", 0xE049 )); // glyphnumber: 741, Square coda
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "conductorBeat2Compound", 0xE897 )); // glyphnumber: 742, Beat 2, compound time
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "conductorBeat2Simple", 0xE894 )); // glyphnumber: 743, Beat 2, simple time
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "conductorBeat3Compound", 0xE898 )); // glyphnumber: 744, Beat 3, compound time
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "conductorBeat3Simple", 0xE895 )); // glyphnumber: 745, Beat 3, simple time
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "conductorBeat4Compound", 0xE899 )); // glyphnumber: 746, Beat 4, compound time
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "conductorBeat4Simple", 0xE896 )); // glyphnumber: 747, Beat 4, simple time
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "conductorLeftBeat", 0xE891 )); // glyphnumber: 748, Left-hand beat or cue
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "conductorRightBeat", 0xE892 )); // glyphnumber: 749, Right-hand beat or cue
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "conductorStrongBeat", 0xE890 )); // glyphnumber: 750, Strong beat or cue
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "conductorUnconducted", 0xE89A )); // glyphnumber: 751, Unconducted/free passages
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "conductorWeakBeat", 0xE893 )); // glyphnumber: 752, Weak beat or cue
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "controlBeginBeam", 0xE8E0 )); // glyphnumber: 753, Begin beam
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "controlBeginPhrase", 0xE8E6 )); // glyphnumber: 754, Begin phrase
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "controlBeginSlur", 0xE8E4 )); // glyphnumber: 755, Begin slur
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "controlBeginTie", 0xE8E2 )); // glyphnumber: 756, Begin tie
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "controlEndBeam", 0xE8E1 )); // glyphnumber: 757, End beam
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "controlEndPhrase", 0xE8E7 )); // glyphnumber: 758, End phrase
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "controlEndSlur", 0xE8E5 )); // glyphnumber: 759, End slur
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "controlEndTie", 0xE8E3 )); // glyphnumber: 760, End tie
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "csymAugmented", 0xE872 )); // glyphnumber: 761, Augmented
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "csymBracketLeftTall", 0xE877 )); // glyphnumber: 762, Double-height left bracket
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "csymBracketRightTall", 0xE878 )); // glyphnumber: 763, Double-height right bracket
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "csymDiminished", 0xE870 )); // glyphnumber: 764, Diminished
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "csymHalfDiminished", 0xE871 )); // glyphnumber: 765, Half-diminished
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "csymMajorSeventh", 0xE873 )); // glyphnumber: 766, Major seventh
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "csymMinor", 0xE874 )); // glyphnumber: 767, Minor
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "csymParensLeftTall", 0xE875 )); // glyphnumber: 768, Double-height left parenthesis
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "csymParensRightTall", 0xE876 )); // glyphnumber: 769, Double-height right parenthesis
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "curlewSign", 0xE4D6 )); // glyphnumber: 770, Curlew (Britten)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daCapo", 0xE046 )); // glyphnumber: 771, Da capo
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dalSegno", 0xE045 )); // glyphnumber: 772, Dal segno
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianExcellentes1", 0xEA3C )); // glyphnumber: 773, Daseian excellentes 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianExcellentes2", 0xEA3D )); // glyphnumber: 774, Daseian excellentes 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianExcellentes3", 0xEA3E )); // glyphnumber: 775, Daseian excellentes 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianExcellentes4", 0xEA3F )); // glyphnumber: 776, Daseian excellentes 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianFinales1", 0xEA34 )); // glyphnumber: 777, Daseian finales 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianFinales2", 0xEA35 )); // glyphnumber: 778, Daseian finales 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianFinales3", 0xEA36 )); // glyphnumber: 779, Daseian finales 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianFinales4", 0xEA37 )); // glyphnumber: 780, Daseian finales 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianGraves1", 0xEA30 )); // glyphnumber: 781, Daseian graves 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianGraves2", 0xEA31 )); // glyphnumber: 782, Daseian graves 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianGraves3", 0xEA32 )); // glyphnumber: 783, Daseian graves 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianGraves4", 0xEA33 )); // glyphnumber: 784, Daseian graves 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianResidua1", 0xEA40 )); // glyphnumber: 785, Daseian residua 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianResidua2", 0xEA41 )); // glyphnumber: 786, Daseian residua 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianSuperiores1", 0xEA38 )); // glyphnumber: 787, Daseian superiores 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianSuperiores2", 0xEA39 )); // glyphnumber: 788, Daseian superiores 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianSuperiores3", 0xEA3A )); // glyphnumber: 789, Daseian superiores 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "daseianSuperiores4", 0xEA3B )); // glyphnumber: 790, Daseian superiores 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "doubleTongueAbove", 0xE5F0 )); // glyphnumber: 791, Double-tongue above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "doubleTongueBelow", 0xE5F1 )); // glyphnumber: 792, Double-tongue below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicCombinedSeparatorColon", 0xE546 )); // glyphnumber: 793, Colon separator for combined dynamics
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicCombinedSeparatorHyphen", 0xE547 )); // glyphnumber: 794, Hyphen separator for combined dynamics
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicCombinedSeparatorSpace", 0xE548 )); // glyphnumber: 795, Space separator for combined dynamics
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicCrescendoHairpin", 0xE53E )); // glyphnumber: 796, Crescendo
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicDiminuendoHairpin", 0xE53F )); // glyphnumber: 797, Diminuendo
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicFF", 0xE52F )); // glyphnumber: 798, ff
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicFFF", 0xE530 )); // glyphnumber: 799, fff
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicFFFF", 0xE531 )); // glyphnumber: 800, ffff
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicFFFFF", 0xE532 )); // glyphnumber: 801, fffff
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicFFFFFF", 0xE533 )); // glyphnumber: 802, ffffff
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicForte", 0xE522 )); // glyphnumber: 803, Forte
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicFortePiano", 0xE534 )); // glyphnumber: 804, Forte-piano
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicForzando", 0xE535 )); // glyphnumber: 805, Forzando
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicHairpinBracketLeft", 0xE544 )); // glyphnumber: 806, Left bracket (for hairpins)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicHairpinBracketRight", 0xE545 )); // glyphnumber: 807, Right bracket (for hairpins)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicHairpinParenthesisLeft", 0xE542 )); // glyphnumber: 808, Left parenthesis (for hairpins)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicHairpinParenthesisRight", 0xE543 )); // glyphnumber: 809, Right parenthesis (for hairpins)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicMF", 0xE52D )); // glyphnumber: 810, mf
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicMP", 0xE52C )); // glyphnumber: 811, mp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicMessaDiVoce", 0xE540 )); // glyphnumber: 812, Messa di voce
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicMezzo", 0xE521 )); // glyphnumber: 813, Mezzo
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicNiente", 0xE526 )); // glyphnumber: 814, Niente
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicNienteForHairpin", 0xE541 )); // glyphnumber: 815, Niente (for hairpins)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicPF", 0xE52E )); // glyphnumber: 816, pf
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicPP", 0xE52B )); // glyphnumber: 817, pp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicPPP", 0xE52A )); // glyphnumber: 818, ppp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicPPPP", 0xE529 )); // glyphnumber: 819, pppp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicPPPPP", 0xE528 )); // glyphnumber: 820, ppppp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicPPPPPP", 0xE527 )); // glyphnumber: 821, pppppp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicPiano", 0xE520 )); // glyphnumber: 822, Piano
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicRinforzando", 0xE523 )); // glyphnumber: 823, Rinforzando
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicRinforzando1", 0xE53C )); // glyphnumber: 824, Rinforzando 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicRinforzando2", 0xE53D )); // glyphnumber: 825, Rinforzando 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicSforzando", 0xE524 )); // glyphnumber: 826, Sforzando
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicSforzando1", 0xE536 )); // glyphnumber: 827, Sforzando 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicSforzandoPianissimo", 0xE538 )); // glyphnumber: 828, Sforzando-pianissimo
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicSforzandoPiano", 0xE537 )); // glyphnumber: 829, Sforzando-piano
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicSforzato", 0xE539 )); // glyphnumber: 830, Sforzato
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicSforzatoFF", 0xE53B )); // glyphnumber: 831, Sforzatissimo
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicSforzatoPiano", 0xE53A )); // glyphnumber: 832, Sforzato-piano
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "dynamicZ", 0xE525 )); // glyphnumber: 833, Z
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecAudioChannelsEight", 0xEB46 )); // glyphnumber: 834, Eight channels (7.1 surround)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecAudioChannelsFive", 0xEB43 )); // glyphnumber: 835, Five channels
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecAudioChannelsFour", 0xEB42 )); // glyphnumber: 836, Four channels
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecAudioChannelsOne", 0xEB3E )); // glyphnumber: 837, One channel (mono)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecAudioChannelsSeven", 0xEB45 )); // glyphnumber: 838, Seven channels
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecAudioChannelsSix", 0xEB44 )); // glyphnumber: 839, Six channels (5.1 surround)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecAudioChannelsThreeFrontal", 0xEB40 )); // glyphnumber: 840, Three channels (frontal)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecAudioChannelsThreeSurround", 0xEB41 )); // glyphnumber: 841, Three channels (surround)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecAudioChannelsTwo", 0xEB3F )); // glyphnumber: 842, Two channels (stereo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecAudioIn", 0xEB49 )); // glyphnumber: 843, Audio in
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecAudioMono", 0xEB3C )); // glyphnumber: 844, Mono audio setup
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecAudioOut", 0xEB4A )); // glyphnumber: 845, Audio out
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecAudioStereo", 0xEB3D )); // glyphnumber: 846, Stereo audio setup
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecCamera", 0xEB1B )); // glyphnumber: 847, Camera
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecDataIn", 0xEB4D )); // glyphnumber: 848, Data in
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecDataOut", 0xEB4E )); // glyphnumber: 849, Data out
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecDisc", 0xEB13 )); // glyphnumber: 850, Disc
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecDownload", 0xEB4F )); // glyphnumber: 851, Download
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecEject", 0xEB2B )); // glyphnumber: 852, Eject
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecFastForward", 0xEB1F )); // glyphnumber: 853, Fast-forward
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecHeadphones", 0xEB11 )); // glyphnumber: 854, Headphones
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecHeadset", 0xEB12 )); // glyphnumber: 855, Headset
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecLineIn", 0xEB47 )); // glyphnumber: 856, Line in
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecLineOut", 0xEB48 )); // glyphnumber: 857, Line out
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecLoop", 0xEB23 )); // glyphnumber: 858, Loop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecLoudspeaker", 0xEB1A )); // glyphnumber: 859, Loudspeaker
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMIDIController0", 0xEB36 )); // glyphnumber: 860, MIDI controller 0%
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMIDIController100", 0xEB3B )); // glyphnumber: 861, MIDI controller 100%
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMIDIController20", 0xEB37 )); // glyphnumber: 862, MIDI controller 20%
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMIDIController40", 0xEB38 )); // glyphnumber: 863, MIDI controller 40%
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMIDIController60", 0xEB39 )); // glyphnumber: 864, MIDI controller 60%
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMIDIController80", 0xEB3A )); // glyphnumber: 865, MIDI controller 80%
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMIDIIn", 0xEB34 )); // glyphnumber: 866, MIDI in
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMIDIOut", 0xEB35 )); // glyphnumber: 867, MIDI out
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMicrophone", 0xEB10 )); // glyphnumber: 868, Microphone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMicrophoneMute", 0xEB28 )); // glyphnumber: 869, Mute microphone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMicrophoneUnmute", 0xEB29 )); // glyphnumber: 870, Unmute microphone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMixingConsole", 0xEB15 )); // glyphnumber: 871, Mixing console
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMonitor", 0xEB18 )); // glyphnumber: 872, Monitor
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecMute", 0xEB26 )); // glyphnumber: 873, Mute
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecPause", 0xEB1E )); // glyphnumber: 874, Pause
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecPlay", 0xEB1C )); // glyphnumber: 875, Play
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecPowerOnOff", 0xEB2A )); // glyphnumber: 876, Power on/off
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecProjector", 0xEB19 )); // glyphnumber: 877, Projector
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecReplay", 0xEB24 )); // glyphnumber: 878, Replay
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecRewind", 0xEB20 )); // glyphnumber: 879, Rewind
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecShuffle", 0xEB25 )); // glyphnumber: 880, Shuffle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecSkipBackwards", 0xEB22 )); // glyphnumber: 881, Skip backwards
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecSkipForwards", 0xEB21 )); // glyphnumber: 882, Skip forwards
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecStop", 0xEB1D )); // glyphnumber: 883, Stop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecTape", 0xEB14 )); // glyphnumber: 884, Tape
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecUSB", 0xEB16 )); // glyphnumber: 885, USB connection
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecUnmute", 0xEB27 )); // glyphnumber: 886, Unmute
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecUpload", 0xEB50 )); // glyphnumber: 887, Upload
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecVideoCamera", 0xEB17 )); // glyphnumber: 888, Video camera
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecVideoIn", 0xEB4B )); // glyphnumber: 889, Video in
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecVideoOut", 0xEB4C )); // glyphnumber: 890, Video out
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecVolumeFader", 0xEB2C )); // glyphnumber: 891, Combining volume fader
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecVolumeFaderThumb", 0xEB2D )); // glyphnumber: 892, Combining volume fader thumb
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecVolumeLevel0", 0xEB2E )); // glyphnumber: 893, Volume level 0%
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecVolumeLevel100", 0xEB33 )); // glyphnumber: 894, Volume level 100%
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecVolumeLevel20", 0xEB2F )); // glyphnumber: 895, Volume level 20%
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecVolumeLevel40", 0xEB30 )); // glyphnumber: 896, Volume level 40%
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecVolumeLevel60", 0xEB31 )); // glyphnumber: 897, Volume level 60%
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "elecVolumeLevel80", 0xEB32 )); // glyphnumber: 898, Volume level 80%
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fClef", 0xE062 )); // glyphnumber: 899, F clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fClef15ma", 0xE066 )); // glyphnumber: 900, F clef quindicesima alta
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fClef15mb", 0xE063 )); // glyphnumber: 901, F clef quindicesima bassa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fClef8va", 0xE065 )); // glyphnumber: 902, F clef ottava alta
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fClef8vb", 0xE064 )); // glyphnumber: 903, F clef ottava bassa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fClefArrowDown", 0xE068 )); // glyphnumber: 904, F clef, arrow down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fClefArrowUp", 0xE067 )); // glyphnumber: 905, F clef, arrow up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fClefChange", 0xE07C )); // glyphnumber: 906, F clef change
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fClefReversed", 0xE076 )); // glyphnumber: 907, Reversed F clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fClefTurned", 0xE077 )); // glyphnumber: 908, Turned F clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataAbove", 0xE4C0 )); // glyphnumber: 909, Fermata above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataBelow", 0xE4C1 )); // glyphnumber: 910, Fermata below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataLongAbove", 0xE4C6 )); // glyphnumber: 911, Long fermata above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataLongBelow", 0xE4C7 )); // glyphnumber: 912, Long fermata below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataLongHenzeAbove", 0xE4CA )); // glyphnumber: 913, Long fermata (Henze) above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataLongHenzeBelow", 0xE4CB )); // glyphnumber: 914, Long fermata (Henze) below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataShortAbove", 0xE4C4 )); // glyphnumber: 915, Short fermata above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataShortBelow", 0xE4C5 )); // glyphnumber: 916, Short fermata below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataShortHenzeAbove", 0xE4CC )); // glyphnumber: 917, Short fermata (Henze) above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataShortHenzeBelow", 0xE4CD )); // glyphnumber: 918, Short fermata (Henze) below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataVeryLongAbove", 0xE4C8 )); // glyphnumber: 919, Very long fermata above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataVeryLongBelow", 0xE4C9 )); // glyphnumber: 920, Very long fermata below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataVeryShortAbove", 0xE4C2 )); // glyphnumber: 921, Very short fermata above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fermataVeryShortBelow", 0xE4C3 )); // glyphnumber: 922, Very short fermata below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass0", 0xEA50 )); // glyphnumber: 923, Figured bass 0
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass1", 0xEA51 )); // glyphnumber: 924, Figured bass 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass2", 0xEA52 )); // glyphnumber: 925, Figured bass 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass2Raised", 0xEA53 )); // glyphnumber: 926, Figured bass 2 raised by half-step
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass3", 0xEA54 )); // glyphnumber: 927, Figured bass 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass4", 0xEA55 )); // glyphnumber: 928, Figured bass 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass4Raised", 0xEA56 )); // glyphnumber: 929, Figured bass 4 raised by half-step
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass5", 0xEA57 )); // glyphnumber: 930, Figured bass 5
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass5Raised1", 0xEA58 )); // glyphnumber: 931, Figured bass 5 raised by half-step
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass5Raised2", 0xEA59 )); // glyphnumber: 932, Figured bass 5 raised by half-step 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass5Raised3", 0xEA5A )); // glyphnumber: 933, Figured bass diminished 5
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass6", 0xEA5B )); // glyphnumber: 934, Figured bass 6
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass6Raised", 0xEA5C )); // glyphnumber: 935, Figured bass 6 raised by half-step
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass6Raised2", 0xEA6F )); // glyphnumber: 936, Figured bass 6 raised by half-step 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass7", 0xEA5D )); // glyphnumber: 937, Figured bass 7
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass7Diminished", 0xECC0 )); // glyphnumber: 938, Figured bass 7 diminished
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass7Raised1", 0xEA5E )); // glyphnumber: 939, Figured bass 7 raised by half-step
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass7Raised2", 0xEA5F )); // glyphnumber: 940, Figured bass 7 raised by a half-step 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass8", 0xEA60 )); // glyphnumber: 941, Figured bass 8
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass9", 0xEA61 )); // glyphnumber: 942, Figured bass 9
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbass9Raised", 0xEA62 )); // glyphnumber: 943, Figured bass 9 raised by half-step
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbassBracketLeft", 0xEA68 )); // glyphnumber: 944, Figured bass [
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbassBracketRight", 0xEA69 )); // glyphnumber: 945, Figured bass ]
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbassCombiningLowering", 0xEA6E )); // glyphnumber: 946, Combining lower
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbassCombiningRaising", 0xEA6D )); // glyphnumber: 947, Combining raise
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbassDoubleFlat", 0xEA63 )); // glyphnumber: 948, Figured bass double flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbassDoubleSharp", 0xEA67 )); // glyphnumber: 949, Figured bass double sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbassFlat", 0xEA64 )); // glyphnumber: 950, Figured bass flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbassNatural", 0xEA65 )); // glyphnumber: 951, Figured bass natural
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbassParensLeft", 0xEA6A )); // glyphnumber: 952, Figured bass (
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbassParensRight", 0xEA6B )); // glyphnumber: 953, Figured bass )
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbassPlus", 0xEA6C )); // glyphnumber: 954, Figured bass +
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "figbassSharp", 0xEA66 )); // glyphnumber: 955, Figured bass sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingering0", 0xED10 )); // glyphnumber: 956, Fingering 0 (open string)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingering1", 0xED11 )); // glyphnumber: 957, Fingering 1 (thumb)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingering2", 0xED12 )); // glyphnumber: 958, Fingering 2 (index finger)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingering3", 0xED13 )); // glyphnumber: 959, Fingering 3 (middle finger)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingering4", 0xED14 )); // glyphnumber: 960, Fingering 4 (ring finger)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingering5", 0xED15 )); // glyphnumber: 961, Fingering 5 (little finger)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringALower", 0xED1B )); // glyphnumber: 962, Fingering a (anular; right-hand ring finger for guitar)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringCLower", 0xED1C )); // glyphnumber: 963, Fingering c (right-hand little finger for guitar)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringELower", 0xED1E )); // glyphnumber: 964, Fingering e (right-hand little finger for guitar)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringILower", 0xED19 )); // glyphnumber: 965, Fingering i (indicio; right-hand index finger for guitar)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringMLower", 0xED1A )); // glyphnumber: 966, Fingering m (medio; right-hand middle finger for guitar)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringMultipleNotes", 0xED23 )); // glyphnumber: 967, Multiple notes played by thumb or single finger
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringOLower", 0xED1F )); // glyphnumber: 968, Fingering o (right-hand little finger for guitar)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringPLower", 0xED17 )); // glyphnumber: 969, Fingering p (pulgar; right-hand thumb for guitar)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringSubstitutionAbove", 0xED20 )); // glyphnumber: 970, Finger substitution above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringSubstitutionBelow", 0xED21 )); // glyphnumber: 971, Finger substitution below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringSubstitutionDash", 0xED22 )); // glyphnumber: 972, Finger substitution dash
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringTLower", 0xED18 )); // glyphnumber: 973, Fingering t (right-hand thumb for guitar)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringTUpper", 0xED16 )); // glyphnumber: 974, Fingering T (left-hand thumb for guitar)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fingeringXLower", 0xED1D )); // glyphnumber: 975, Fingering x (right-hand little finger for guitar)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag1024thDown", 0xE24F )); // glyphnumber: 976, Combining flag 8 (1024th) below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag1024thUp", 0xE24E )); // glyphnumber: 977, Combining flag 8 (1024th) above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag128thDown", 0xE249 )); // glyphnumber: 978, Combining flag 5 (128th) below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag128thUp", 0xE248 )); // glyphnumber: 979, Combining flag 5 (128th) above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag16thDown", 0xE243 )); // glyphnumber: 980, Combining flag 2 (16th) below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag16thUp", 0xE242 )); // glyphnumber: 981, Combining flag 2 (16th) above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag256thDown", 0xE24B )); // glyphnumber: 982, Combining flag 6 (256th) below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag256thUp", 0xE24A )); // glyphnumber: 983, Combining flag 6 (256th) above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag32ndDown", 0xE245 )); // glyphnumber: 984, Combining flag 3 (32nd) below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag32ndUp", 0xE244 )); // glyphnumber: 985, Combining flag 3 (32nd) above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag512thDown", 0xE24D )); // glyphnumber: 986, Combining flag 7 (512th) below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag512thUp", 0xE24C )); // glyphnumber: 987, Combining flag 7 (512th) above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag64thDown", 0xE247 )); // glyphnumber: 988, Combining flag 4 (64th) below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag64thUp", 0xE246 )); // glyphnumber: 989, Combining flag 4 (64th) above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag8thDown", 0xE241 )); // glyphnumber: 990, Combining flag 1 (8th) below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flag8thUp", 0xE240 )); // glyphnumber: 991, Combining flag 1 (8th) above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flagInternalDown", 0xE251 )); // glyphnumber: 992, Internal combining flag below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "flagInternalUp", 0xE250 )); // glyphnumber: 993, Internal combining flag above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fretboard3String", 0xE850 )); // glyphnumber: 994, 3-string fretboard
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fretboard3StringNut", 0xE851 )); // glyphnumber: 995, 3-string fretboard at nut
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fretboard4String", 0xE852 )); // glyphnumber: 996, 4-string fretboard
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fretboard4StringNut", 0xE853 )); // glyphnumber: 997, 4-string fretboard at nut
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fretboard5String", 0xE854 )); // glyphnumber: 998, 5-string fretboard
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fretboard5StringNut", 0xE855 )); // glyphnumber: 999, 5-string fretboard at nut
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fretboard6String", 0xE856 )); // glyphnumber: 1000, 6-string fretboard
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fretboard6StringNut", 0xE857 )); // glyphnumber: 1001, 6-string fretboard at nut
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fretboardFilledCircle", 0xE858 )); // glyphnumber: 1002, Fingered fret (filled circle)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fretboardO", 0xE85A )); // glyphnumber: 1003, Open string (O)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "fretboardX", 0xE859 )); // glyphnumber: 1004, String not played (X)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionAngleLeft", 0xEA93 )); // glyphnumber: 1005, Function theory angle bracket left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionAngleRight", 0xEA94 )); // glyphnumber: 1006, Function theory angle bracket right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionBracketLeft", 0xEA8F )); // glyphnumber: 1007, Function theory bracket left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionBracketRight", 0xEA90 )); // glyphnumber: 1008, Function theory bracket right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionDD", 0xEA81 )); // glyphnumber: 1009, Function theory dominant of dominant
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionDLower", 0xEA80 )); // glyphnumber: 1010, Function theory minor dominant
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionDUpper", 0xEA7F )); // glyphnumber: 1011, Function theory major dominant
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionEight", 0xEA78 )); // glyphnumber: 1012, Function theory 8
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionFUpper", 0xEA99 )); // glyphnumber: 1013, Function theory F
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionFive", 0xEA75 )); // glyphnumber: 1014, Function theory 5
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionFour", 0xEA74 )); // glyphnumber: 1015, Function theory 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionGLower", 0xEA84 )); // glyphnumber: 1016, Function theory g
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionGUpper", 0xEA83 )); // glyphnumber: 1017, Function theory G
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionGreaterThan", 0xEA7C )); // glyphnumber: 1018, Function theory greater than
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionILower", 0xEA9B )); // glyphnumber: 1019, Function theory i
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionIUpper", 0xEA9A )); // glyphnumber: 1020, Function theory I
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionKLower", 0xEA9D )); // glyphnumber: 1021, Function theory k
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionKUpper", 0xEA9C )); // glyphnumber: 1022, Function theory K
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionLLower", 0xEA9F )); // glyphnumber: 1023, Function theory l
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionLUpper", 0xEA9E )); // glyphnumber: 1024, Function theory L
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionLessThan", 0xEA7A )); // glyphnumber: 1025, Function theory less than
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionMLower", 0xED01 )); // glyphnumber: 1026, Function theory m
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionMUpper", 0xED00 )); // glyphnumber: 1027, Function theory M
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionMinus", 0xEA7B )); // glyphnumber: 1028, Function theory minus
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionNLower", 0xEA86 )); // glyphnumber: 1029, Function theory n
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionNUpper", 0xEA85 )); // glyphnumber: 1030, Function theory N
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionNUpperSuperscript", 0xED02 )); // glyphnumber: 1031, Function theory superscript N
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionNine", 0xEA79 )); // glyphnumber: 1032, Function theory 9
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionOne", 0xEA71 )); // glyphnumber: 1033, Function theory 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionPLower", 0xEA88 )); // glyphnumber: 1034, Function theory p
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionPUpper", 0xEA87 )); // glyphnumber: 1035, Function theory P
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionParensLeft", 0xEA91 )); // glyphnumber: 1036, Function theory parenthesis left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionParensRight", 0xEA92 )); // glyphnumber: 1037, Function theory parenthesis right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionPlus", 0xEA98 )); // glyphnumber: 1038, Function theory prefix plus
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionRLower", 0xED03 )); // glyphnumber: 1039, Function theory r
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionRepetition1", 0xEA95 )); // glyphnumber: 1040, Function theory repetition 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionRepetition2", 0xEA96 )); // glyphnumber: 1041, Function theory repetition 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionRing", 0xEA97 )); // glyphnumber: 1042, Function theory prefix ring
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionSLower", 0xEA8A )); // glyphnumber: 1043, Function theory minor subdominant
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionSSLower", 0xEA7E )); // glyphnumber: 1044, Function theory minor subdominant of subdominant
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionSSUpper", 0xEA7D )); // glyphnumber: 1045, Function theory major subdominant of subdominant
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionSUpper", 0xEA89 )); // glyphnumber: 1046, Function theory major subdominant
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionSeven", 0xEA77 )); // glyphnumber: 1047, Function theory 7
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionSix", 0xEA76 )); // glyphnumber: 1048, Function theory 6
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionSlashedDD", 0xEA82 )); // glyphnumber: 1049, Function theory double dominant seventh
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionTLower", 0xEA8C )); // glyphnumber: 1050, Function theory minor tonic
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionTUpper", 0xEA8B )); // glyphnumber: 1051, Function theory tonic
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionThree", 0xEA73 )); // glyphnumber: 1052, Function theory 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionTwo", 0xEA72 )); // glyphnumber: 1053, Function theory 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionVLower", 0xEA8E )); // glyphnumber: 1054, Function theory v
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionVUpper", 0xEA8D )); // glyphnumber: 1055, Function theory V
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "functionZero", 0xEA70 )); // glyphnumber: 1056, Function theory 0
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClef", 0xE050 )); // glyphnumber: 1057, G clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClef15ma", 0xE054 )); // glyphnumber: 1058, G clef quindicesima alta
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClef15mb", 0xE051 )); // glyphnumber: 1059, G clef quindicesima bassa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClef8va", 0xE053 )); // glyphnumber: 1060, G clef ottava alta
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClef8vb", 0xE052 )); // glyphnumber: 1061, G clef ottava bassa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClef8vbCClef", 0xE056 )); // glyphnumber: 1062, G clef ottava bassa with C clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClef8vbOld", 0xE055 )); // glyphnumber: 1063, G clef ottava bassa (old style)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClef8vbParens", 0xE057 )); // glyphnumber: 1064, G clef, optionally ottava bassa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClefArrowDown", 0xE05B )); // glyphnumber: 1065, G clef, arrow down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClefArrowUp", 0xE05A )); // glyphnumber: 1066, G clef, arrow up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClefChange", 0xE07A )); // glyphnumber: 1067, G clef change
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClefLigatedNumberAbove", 0xE059 )); // glyphnumber: 1068, Combining G clef, number above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClefLigatedNumberBelow", 0xE058 )); // glyphnumber: 1069, Combining G clef, number below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClefReversed", 0xE073 )); // glyphnumber: 1070, Reversed G clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "gClefTurned", 0xE074 )); // glyphnumber: 1071, Turned G clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "glissandoDown", 0xE586 )); // glyphnumber: 1072, Glissando down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "glissandoUp", 0xE585 )); // glyphnumber: 1073, Glissando up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "graceNoteAcciaccaturaStemDown", 0xE561 )); // glyphnumber: 1074, Slashed grace note stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "graceNoteAcciaccaturaStemUp", 0xE560 )); // glyphnumber: 1075, Slashed grace note stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "graceNoteAppoggiaturaStemDown", 0xE563 )); // glyphnumber: 1076, Grace note stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "graceNoteAppoggiaturaStemUp", 0xE562 )); // glyphnumber: 1077, Grace note stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "graceNoteSlashStemDown", 0xE565 )); // glyphnumber: 1078, Slash for stem down grace note
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "graceNoteSlashStemUp", 0xE564 )); // glyphnumber: 1079, Slash for stem up grace note
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarBarreFull", 0xE848 )); // glyphnumber: 1080, Full barré
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarBarreHalf", 0xE849 )); // glyphnumber: 1081, Half barré
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarClosePedal", 0xE83F )); // glyphnumber: 1082, Closed wah/volume pedal
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarFadeIn", 0xE843 )); // glyphnumber: 1083, Fade in
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarFadeOut", 0xE844 )); // glyphnumber: 1084, Fade out
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarGolpe", 0xE842 )); // glyphnumber: 1085, Golpe (tapping the pick guard)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarHalfOpenPedal", 0xE83E )); // glyphnumber: 1086, Half-open wah/volume pedal
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarLeftHandTapping", 0xE840 )); // glyphnumber: 1087, Left-hand tapping
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarOpenPedal", 0xE83D )); // glyphnumber: 1088, Open wah/volume pedal
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarRightHandTapping", 0xE841 )); // glyphnumber: 1089, Right-hand tapping
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarShake", 0xE832 )); // glyphnumber: 1090, Guitar shake
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarString0", 0xE833 )); // glyphnumber: 1091, String number 0
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarString1", 0xE834 )); // glyphnumber: 1092, String number 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarString2", 0xE835 )); // glyphnumber: 1093, String number 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarString3", 0xE836 )); // glyphnumber: 1094, String number 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarString4", 0xE837 )); // glyphnumber: 1095, String number 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarString5", 0xE838 )); // glyphnumber: 1096, String number 5
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarString6", 0xE839 )); // glyphnumber: 1097, String number 6
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarString7", 0xE83A )); // glyphnumber: 1098, String number 7
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarString8", 0xE83B )); // glyphnumber: 1099, String number 8
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarString9", 0xE83C )); // glyphnumber: 1100, String number 9
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarStrumDown", 0xE847 )); // glyphnumber: 1101, Strum direction down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarStrumUp", 0xE846 )); // glyphnumber: 1102, Strum direction up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarVibratoBarDip", 0xE831 )); // glyphnumber: 1103, Guitar vibrato bar dip
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarVibratoBarScoop", 0xE830 )); // glyphnumber: 1104, Guitar vibrato bar scoop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarVibratoStroke", 0xEAB2 )); // glyphnumber: 1105, Vibrato wiggle segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarVolumeSwell", 0xE845 )); // glyphnumber: 1106, Volume swell
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "guitarWideVibratoStroke", 0xEAB3 )); // glyphnumber: 1107, Wide vibrato wiggle segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsBelltree", 0xE81F )); // glyphnumber: 1108, Belltree
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsDamp3", 0xE81E )); // glyphnumber: 1109, Damp 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsEcho1", 0xE81B )); // glyphnumber: 1110, Echo
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsEcho2", 0xE81C )); // glyphnumber: 1111, Echo 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsGyro", 0xE81D )); // glyphnumber: 1112, Gyro
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsHandMartellato", 0xE812 )); // glyphnumber: 1113, Hand martellato
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsMalletBellOnTable", 0xE815 )); // glyphnumber: 1114, Mallet, bell on table
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsMalletBellSuspended", 0xE814 )); // glyphnumber: 1115, Mallet, bell suspended
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsMalletLft", 0xE816 )); // glyphnumber: 1116, Mallet lift
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsMartellato", 0xE810 )); // glyphnumber: 1117, Martellato
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsMartellatoLift", 0xE811 )); // glyphnumber: 1118, Martellato lift
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsMutedMartellato", 0xE813 )); // glyphnumber: 1119, Muted martellato
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsPluckLift", 0xE817 )); // glyphnumber: 1120, Pluck lift
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsSwing", 0xE81A )); // glyphnumber: 1121, Swing
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsSwingDown", 0xE819 )); // glyphnumber: 1122, Swing down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsSwingUp", 0xE818 )); // glyphnumber: 1123, Swing up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsTablePairBells", 0xE821 )); // glyphnumber: 1124, Table pair of handbells
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "handbellsTableSingleBell", 0xE820 )); // glyphnumber: 1125, Table single handbell
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpMetalRod", 0xE68F )); // glyphnumber: 1126, Metal rod pictogram
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpPedalCentered", 0xE681 )); // glyphnumber: 1127, Harp pedal centered (natural)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpPedalDivider", 0xE683 )); // glyphnumber: 1128, Harp pedal divider
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpPedalLowered", 0xE682 )); // glyphnumber: 1129, Harp pedal lowered (sharp)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpPedalRaised", 0xE680 )); // glyphnumber: 1130, Harp pedal raised (flat)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoAeolianAscending", 0xE695 )); // glyphnumber: 1131, Ascending aeolian chords (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoAeolianDescending", 0xE696 )); // glyphnumber: 1132, Descending aeolian chords (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoDampAbove", 0xE69A )); // glyphnumber: 1133, Damp above (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoDampBelow", 0xE699 )); // glyphnumber: 1134, Damp below (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoDampBothHands", 0xE698 )); // glyphnumber: 1135, Damp with both hands (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoDampLowStrings", 0xE697 )); // glyphnumber: 1136, Damp only low strings (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoFluidicSoundsLeft", 0xE68D )); // glyphnumber: 1137, Fluidic sounds, left hand (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoFluidicSoundsRight", 0xE68E )); // glyphnumber: 1138, Fluidic sounds, right hand (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoIsolatedSounds", 0xE69C )); // glyphnumber: 1139, Isolated sounds (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoMetallicSounds", 0xE688 )); // glyphnumber: 1140, Metallic sounds (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoMetallicSoundsOneString", 0xE69B )); // glyphnumber: 1141, Metallic sounds, one string (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoMuffleTotally", 0xE68C )); // glyphnumber: 1142, Muffle totally (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoOboicFlux", 0xE685 )); // glyphnumber: 1143, Oboic flux (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoPlayUpperEnd", 0xE68A )); // glyphnumber: 1144, Play at upper end of strings (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoSlideWithSuppleness", 0xE684 )); // glyphnumber: 1145, Slide with suppleness (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoSnareDrum", 0xE69D )); // glyphnumber: 1146, Snare drum effect (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoTamTamSounds", 0xE689 )); // glyphnumber: 1147, Tam-tam sounds (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoThunderEffect", 0xE686 )); // glyphnumber: 1148, Thunder effect (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoTimpanicSounds", 0xE68B )); // glyphnumber: 1149, Timpanic sounds (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpSalzedoWhistlingSounds", 0xE687 )); // glyphnumber: 1150, Whistling sounds (Salzedo)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpStringNoiseStem", 0xE694 )); // glyphnumber: 1151, Combining string noise for stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpTuningKey", 0xE690 )); // glyphnumber: 1152, Tuning key pictogram
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpTuningKeyGlissando", 0xE693 )); // glyphnumber: 1153, Retune strings for glissando
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpTuningKeyHandle", 0xE691 )); // glyphnumber: 1154, Use handle of tuning key pictogram
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "harpTuningKeyShank", 0xE692 )); // glyphnumber: 1155, Use shank of tuning key pictogram
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardBebung2DotsAbove", 0xE668 )); // glyphnumber: 1156, Clavichord bebung, 2 finger movements (above)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardBebung2DotsBelow", 0xE669 )); // glyphnumber: 1157, Clavichord bebung, 2 finger movements (below)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardBebung3DotsAbove", 0xE66A )); // glyphnumber: 1158, Clavichord bebung, 3 finger movements (above)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardBebung3DotsBelow", 0xE66B )); // glyphnumber: 1159, Clavichord bebung, 3 finger movements (below)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardBebung4DotsAbove", 0xE66C )); // glyphnumber: 1160, Clavichord bebung, 4 finger movements (above)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardBebung4DotsBelow", 0xE66D )); // glyphnumber: 1161, Clavichord bebung, 4 finger movements (below)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardLeftPedalPictogram", 0xE65E )); // glyphnumber: 1162, Left pedal pictogram
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardMiddlePedalPictogram", 0xE65F )); // glyphnumber: 1163, Middle pedal pictogram
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalD", 0xE653 )); // glyphnumber: 1164, Pedal d
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalDot", 0xE654 )); // glyphnumber: 1165, Pedal dot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalE", 0xE652 )); // glyphnumber: 1166, Pedal e
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalHalf", 0xE656 )); // glyphnumber: 1167, Half-pedal mark
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalHalf2", 0xE65B )); // glyphnumber: 1168, Half pedal mark 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalHalf3", 0xE65C )); // glyphnumber: 1169, Half pedal mark 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalHeel1", 0xE661 )); // glyphnumber: 1170, Pedal heel 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalHeel2", 0xE662 )); // glyphnumber: 1171, Pedal heel 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalHeel3", 0xE663 )); // glyphnumber: 1172, Pedal heel 3 (Davis)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalHeelToToe", 0xE674 )); // glyphnumber: 1173, Pedal heel to toe
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalHeelToe", 0xE666 )); // glyphnumber: 1174, Pedal heel or toe
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalHookEnd", 0xE673 )); // glyphnumber: 1175, Pedal hook end
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalHookStart", 0xE672 )); // glyphnumber: 1176, Pedal hook start
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalHyphen", 0xE658 )); // glyphnumber: 1177, Pedal hyphen
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalP", 0xE651 )); // glyphnumber: 1178, Pedal P
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalPed", 0xE650 )); // glyphnumber: 1179, Pedal mark
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalS", 0xE65A )); // glyphnumber: 1180, Pedal S
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalSost", 0xE659 )); // glyphnumber: 1181, Sostenuto pedal mark
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalToe1", 0xE664 )); // glyphnumber: 1182, Pedal toe 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalToe2", 0xE665 )); // glyphnumber: 1183, Pedal toe 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalToeToHeel", 0xE675 )); // glyphnumber: 1184, Pedal toe to heel
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalUp", 0xE655 )); // glyphnumber: 1185, Pedal up mark
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalUpNotch", 0xE657 )); // glyphnumber: 1186, Pedal up notch
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPedalUpSpecial", 0xE65D )); // glyphnumber: 1187, Pedal up special
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPlayWithLH", 0xE670 )); // glyphnumber: 1188, Play with left hand
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPlayWithLHEnd", 0xE671 )); // glyphnumber: 1189, Play with left hand (end)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPlayWithRH", 0xE66E )); // glyphnumber: 1190, Play with right hand
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPlayWithRHEnd", 0xE66F )); // glyphnumber: 1191, Play with right hand (end)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardPluckInside", 0xE667 )); // glyphnumber: 1192, Pluck strings inside piano (Maderna)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "keyboardRightPedalPictogram", 0xE660 )); // glyphnumber: 1193, Right pedal pictogram
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanAccidentalFlat", 0xEC3E )); // glyphnumber: 1194, Kievan flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanAccidentalSharp", 0xEC3D )); // glyphnumber: 1195, Kievan sharp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanAugmentationDot", 0xEC3C )); // glyphnumber: 1196, Kievan augmentation dot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanCClef", 0xEC30 )); // glyphnumber: 1197, Kievan C clef (tse-fa-ut)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanEndingSymbol", 0xEC31 )); // glyphnumber: 1198, Kievan ending symbol
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanNote8thStemDown", 0xEC3A )); // glyphnumber: 1199, Kievan eighth note, stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanNote8thStemUp", 0xEC39 )); // glyphnumber: 1200, Kievan eighth note, stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanNoteBeam", 0xEC3B )); // glyphnumber: 1201, Kievan beam
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanNoteHalfStaffLine", 0xEC35 )); // glyphnumber: 1202, Kievan half note (on staff line)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanNoteHalfStaffSpace", 0xEC36 )); // glyphnumber: 1203, Kievan half note (in staff space)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanNoteQuarterStemDown", 0xEC38 )); // glyphnumber: 1204, Kievan quarter note, stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanNoteQuarterStemUp", 0xEC37 )); // glyphnumber: 1205, Kievan quarter note, stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanNoteReciting", 0xEC32 )); // glyphnumber: 1206, Kievan reciting note
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanNoteWhole", 0xEC33 )); // glyphnumber: 1207, Kievan whole note
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kievanNoteWholeFinal", 0xEC34 )); // glyphnumber: 1208, Kievan final whole note
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kodalyHandDo", 0xEC40 )); // glyphnumber: 1209, Do hand sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kodalyHandFa", 0xEC43 )); // glyphnumber: 1210, Fa hand sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kodalyHandLa", 0xEC45 )); // glyphnumber: 1211, La hand sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kodalyHandMi", 0xEC42 )); // glyphnumber: 1212, Mi hand sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kodalyHandRe", 0xEC41 )); // glyphnumber: 1213, Re hand sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kodalyHandSo", 0xEC44 )); // glyphnumber: 1214, So hand sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "kodalyHandTi", 0xEC46 )); // glyphnumber: 1215, Ti hand sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "leftRepeatSmall", 0xE04C )); // glyphnumber: 1216, Left repeat sign within bar
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "legerLine", 0xE022 )); // glyphnumber: 1217, Leger line
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "legerLineNarrow", 0xE024 )); // glyphnumber: 1218, Leger line (narrow)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "legerLineWide", 0xE023 )); // glyphnumber: 1219, Leger line (wide)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteBarlineEndRepeat", 0xEBA4 )); // glyphnumber: 1220, Lute tablature end repeat barline
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteBarlineFinal", 0xEBA5 )); // glyphnumber: 1221, Lute tablature final barline
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteBarlineStartRepeat", 0xEBA3 )); // glyphnumber: 1222, Lute tablature start repeat barline
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteDuration16th", 0xEBAB )); // glyphnumber: 1223, 16th note (semiquaver) duration sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteDuration32nd", 0xEBAC )); // glyphnumber: 1224, 32nd note (demisemiquaver) duration sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteDuration8th", 0xEBAA )); // glyphnumber: 1225, Eighth note (quaver) duration sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteDurationDoubleWhole", 0xEBA6 )); // glyphnumber: 1226, Double whole note (breve) duration sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteDurationHalf", 0xEBA8 )); // glyphnumber: 1227, Half note (minim) duration sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteDurationQuarter", 0xEBA9 )); // glyphnumber: 1228, Quarter note (crotchet) duration sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteDurationWhole", 0xEBA7 )); // glyphnumber: 1229, Whole note (semibreve) duration sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFingeringRHFirst", 0xEBAE )); // glyphnumber: 1230, Right-hand fingering, first finger
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFingeringRHSecond", 0xEBAF )); // glyphnumber: 1231, Right-hand fingering, second finger
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFingeringRHThird", 0xEBB0 )); // glyphnumber: 1232, Right-hand fingering, third finger
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFingeringRHThumb", 0xEBAD )); // glyphnumber: 1233, Right-hand fingering, thumb
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrench10thCourse", 0xEBD0 )); // glyphnumber: 1234, 10th course (diapason)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrench7thCourse", 0xEBCD )); // glyphnumber: 1235, Seventh course (diapason)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrench8thCourse", 0xEBCE )); // glyphnumber: 1236, Eighth course (diapason)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrench9thCourse", 0xEBCF )); // glyphnumber: 1237, Ninth course (diapason)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchAppoggiaturaAbove", 0xEBD5 )); // glyphnumber: 1238, Appoggiatura from above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchAppoggiaturaBelow", 0xEBD4 )); // glyphnumber: 1239, Appoggiatura from below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchFretA", 0xEBC0 )); // glyphnumber: 1240, Open string (a)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchFretB", 0xEBC1 )); // glyphnumber: 1241, First fret (b)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchFretC", 0xEBC2 )); // glyphnumber: 1242, Second fret (c)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchFretD", 0xEBC3 )); // glyphnumber: 1243, Third fret (d)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchFretE", 0xEBC4 )); // glyphnumber: 1244, Fourth fret (e)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchFretF", 0xEBC5 )); // glyphnumber: 1245, Fifth fret (f)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchFretG", 0xEBC6 )); // glyphnumber: 1246, Sixth fret (g)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchFretH", 0xEBC7 )); // glyphnumber: 1247, Seventh fret (h)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchFretI", 0xEBC8 )); // glyphnumber: 1248, Eighth fret (i)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchFretK", 0xEBC9 )); // glyphnumber: 1249, Ninth fret (k)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchFretL", 0xEBCA )); // glyphnumber: 1250, 10th fret (l)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchFretM", 0xEBCB )); // glyphnumber: 1251, 11th fret (m)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchFretN", 0xEBCC )); // glyphnumber: 1252, 12th fret (n)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchMordentInverted", 0xEBD3 )); // glyphnumber: 1253, Inverted mordent
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchMordentLower", 0xEBD2 )); // glyphnumber: 1254, Mordent with lower auxiliary
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteFrenchMordentUpper", 0xEBD1 )); // glyphnumber: 1255, Mordent with upper auxiliary
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanALower", 0xEC00 )); // glyphnumber: 1256, 5th course, 1st fret (a)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanAUpper", 0xEC17 )); // glyphnumber: 1257, 6th course, 1st fret (A)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanBLower", 0xEC01 )); // glyphnumber: 1258, 4th course, 1st fret (b)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanBUpper", 0xEC18 )); // glyphnumber: 1259, 6th course, 2nd fret (B)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanCLower", 0xEC02 )); // glyphnumber: 1260, 3rd course, 1st fret (c)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanCUpper", 0xEC19 )); // glyphnumber: 1261, 6th course, 3rd fret (C)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanDLower", 0xEC03 )); // glyphnumber: 1262, 2nd course, 1st fret (d)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanDUpper", 0xEC1A )); // glyphnumber: 1263, 6th course, 4th fret (D)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanELower", 0xEC04 )); // glyphnumber: 1264, 1st course, 1st fret (e)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanEUpper", 0xEC1B )); // glyphnumber: 1265, 6th course, 5th fret (E)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanFLower", 0xEC05 )); // glyphnumber: 1266, 5th course, 2nd fret (f)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanFUpper", 0xEC1C )); // glyphnumber: 1267, 6th course, 6th fret (F)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanGLower", 0xEC06 )); // glyphnumber: 1268, 4th course, 2nd fret (g)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanGUpper", 0xEC1D )); // glyphnumber: 1269, 6th course, 7th fret (G)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanHLower", 0xEC07 )); // glyphnumber: 1270, 3rd course, 2nd fret (h)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanHUpper", 0xEC1E )); // glyphnumber: 1271, 6th course, 8th fret (H)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanILower", 0xEC08 )); // glyphnumber: 1272, 2nd course, 2nd fret (i)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanIUpper", 0xEC1F )); // glyphnumber: 1273, 6th course, 9th fret (I)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanKLower", 0xEC09 )); // glyphnumber: 1274, 1st course, 2nd fret (k)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanKUpper", 0xEC20 )); // glyphnumber: 1275, 6th course, 10th fret (K)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanLLower", 0xEC0A )); // glyphnumber: 1276, 5th course, 3rd fret (l)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanLUpper", 0xEC21 )); // glyphnumber: 1277, 6th course, 11th fret (L)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanMLower", 0xEC0B )); // glyphnumber: 1278, 4th course, 3rd fret (m)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanMUpper", 0xEC22 )); // glyphnumber: 1279, 6th course, 12th fret (M)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanNLower", 0xEC0C )); // glyphnumber: 1280, 3rd course, 3rd fret (n)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanNUpper", 0xEC23 )); // glyphnumber: 1281, 6th course, 13th fret (N)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanOLower", 0xEC0D )); // glyphnumber: 1282, 2nd course, 3rd fret (o)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanPLower", 0xEC0E )); // glyphnumber: 1283, 1st course, 3rd fret (p)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanQLower", 0xEC0F )); // glyphnumber: 1284, 5th course, 4th fret (q)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanRLower", 0xEC10 )); // glyphnumber: 1285, 4th course, 4th fret (r)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanSLower", 0xEC11 )); // glyphnumber: 1286, 3rd course, 4th fret (s)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanTLower", 0xEC12 )); // glyphnumber: 1287, 2nd course, 4th fret (t)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanVLower", 0xEC13 )); // glyphnumber: 1288, 1st course, 4th fret (v)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanXLower", 0xEC14 )); // glyphnumber: 1289, 5th course, 5th fret (x)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanYLower", 0xEC15 )); // glyphnumber: 1290, 4th course, 5th fret (y)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteGermanZLower", 0xEC16 )); // glyphnumber: 1291, 3rd course, 5th fret (z)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianClefCSolFaUt", 0xEBF1 )); // glyphnumber: 1292, C sol fa ut clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianClefFFaUt", 0xEBF0 )); // glyphnumber: 1293, F fa ut clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianFret0", 0xEBE0 )); // glyphnumber: 1294, Open string (0)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianFret1", 0xEBE1 )); // glyphnumber: 1295, First fret (1)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianFret2", 0xEBE2 )); // glyphnumber: 1296, Second fret (2)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianFret3", 0xEBE3 )); // glyphnumber: 1297, Third fret (3)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianFret4", 0xEBE4 )); // glyphnumber: 1298, Fourth fret (4)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianFret5", 0xEBE5 )); // glyphnumber: 1299, Fifth fret (5)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianFret6", 0xEBE6 )); // glyphnumber: 1300, Sixth fret (6)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianFret7", 0xEBE7 )); // glyphnumber: 1301, Seventh fret (7)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianFret8", 0xEBE8 )); // glyphnumber: 1302, Eighth fret (8)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianFret9", 0xEBE9 )); // glyphnumber: 1303, Ninth fret (9)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianHoldFinger", 0xEBF4 )); // glyphnumber: 1304, Hold finger in place
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianHoldNote", 0xEBF3 )); // glyphnumber: 1305, Hold note
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianReleaseFinger", 0xEBF5 )); // glyphnumber: 1306, Release finger
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianTempoFast", 0xEBEA )); // glyphnumber: 1307, Fast tempo indication (de Mudarra)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianTempoNeitherFastNorSlow", 0xEBEC )); // glyphnumber: 1308, Neither fast nor slow tempo indication (de Mudarra)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianTempoSlow", 0xEBED )); // glyphnumber: 1309, Slow tempo indication (de Mudarra)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianTempoSomewhatFast", 0xEBEB )); // glyphnumber: 1310, Somewhat fast tempo indication (de Narvaez)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianTempoVerySlow", 0xEBEE )); // glyphnumber: 1311, Very slow indication (de Narvaez)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianTimeTriple", 0xEBEF )); // glyphnumber: 1312, Triple time indication
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianTremolo", 0xEBF2 )); // glyphnumber: 1313, Single-finger tremolo or mordent
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteItalianVibrato", 0xEBF6 )); // glyphnumber: 1314, Vibrato (verre cassé)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteStaff6Lines", 0xEBA0 )); // glyphnumber: 1315, Lute tablature staff, 6 courses
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteStaff6LinesNarrow", 0xEBA2 )); // glyphnumber: 1316, Lute tablature staff, 6 courses (narrow)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "luteStaff6LinesWide", 0xEBA1 )); // glyphnumber: 1317, Lute tablature staff, 6 courses (wide)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "lyricsElision", 0xE551 )); // glyphnumber: 1318, Elision
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "lyricsElisionNarrow", 0xE550 )); // glyphnumber: 1319, Narrow elision
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "lyricsElisionWide", 0xE552 )); // glyphnumber: 1320, Wide elision
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "lyricsHyphenBaseline", 0xE553 )); // glyphnumber: 1321, Baseline hyphen
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "lyricsHyphenBaselineNonBreaking", 0xE554 )); // glyphnumber: 1322, Non-breaking baseline hyphen
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenFlatHardB", 0xE9E1 )); // glyphnumber: 1323, Flat, hard b (mi)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenFlatSoftB", 0xE9E0 )); // glyphnumber: 1324, Flat, soft b (fa)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenFlatWithDot", 0xE9E4 )); // glyphnumber: 1325, Flat with dot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenGClefCMN", 0xEA24 )); // glyphnumber: 1326, G clef (Corpus Monodicum)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenLiquescenceCMN", 0xEA22 )); // glyphnumber: 1327, Liquescence
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenLiquescentAscCMN", 0xEA26 )); // glyphnumber: 1328, Liquescent ascending (Corpus Monodicum)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenLiquescentDescCMN", 0xEA27 )); // glyphnumber: 1329, Liquescent descending (Corpus Monodicum)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenNatural", 0xE9E2 )); // glyphnumber: 1330, Natural
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenNaturalWithCross", 0xE9E5 )); // glyphnumber: 1331, Natural with interrupted cross
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenOriscusCMN", 0xEA2A )); // glyphnumber: 1332, Oriscus (Corpus Monodicum)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenPlicaCMN", 0xEA23 )); // glyphnumber: 1333, Plica
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenPunctumCMN", 0xEA25 )); // glyphnumber: 1334, Punctum (Corpus Monodicum)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenQuilismaCMN", 0xEA28 )); // glyphnumber: 1335, Quilisma (Corpus Monodicum)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenSharpCroix", 0xE9E3 )); // glyphnumber: 1336, Croix
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "medRenStrophicusCMN", 0xEA29 )); // glyphnumber: 1337, Strophicus (Corpus Monodicum)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralAlterationSign", 0xEA10 )); // glyphnumber: 1338, Alteration sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralBlackBrevis", 0xE952 )); // glyphnumber: 1339, Black mensural brevis
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralBlackBrevisVoid", 0xE956 )); // glyphnumber: 1340, Black mensural void brevis
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralBlackDragma", 0xE95A )); // glyphnumber: 1341, Black mensural dragma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralBlackLonga", 0xE951 )); // glyphnumber: 1342, Black mensural longa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralBlackMaxima", 0xE950 )); // glyphnumber: 1343, Black mensural maxima
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralBlackMinima", 0xE954 )); // glyphnumber: 1344, Black mensural minima
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralBlackMinimaVoid", 0xE958 )); // glyphnumber: 1345, Black mensural void minima
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralBlackSemibrevis", 0xE953 )); // glyphnumber: 1346, Black mensural semibrevis
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralBlackSemibrevisCaudata", 0xE959 )); // glyphnumber: 1347, Black mensural semibrevis caudata
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralBlackSemibrevisOblique", 0xE95B )); // glyphnumber: 1348, Black mensural oblique semibrevis
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralBlackSemibrevisVoid", 0xE957 )); // glyphnumber: 1349, Black mensural void semibrevis
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralBlackSemiminima", 0xE955 )); // glyphnumber: 1350, Black mensural semiminima
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCclef", 0xE905 )); // glyphnumber: 1351, Mensural C clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCclefPetrucciPosHigh", 0xE90A )); // glyphnumber: 1352, Petrucci C clef, high position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCclefPetrucciPosHighest", 0xE90B )); // glyphnumber: 1353, Petrucci C clef, highest position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCclefPetrucciPosLow", 0xE908 )); // glyphnumber: 1354, Petrucci C clef, low position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCclefPetrucciPosLowest", 0xE907 )); // glyphnumber: 1355, Petrucci C clef, lowest position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCclefPetrucciPosMiddle", 0xE909 )); // glyphnumber: 1356, Petrucci C clef, middle position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralColorationEndRound", 0xEA0F )); // glyphnumber: 1357, Coloration end, round
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralColorationEndSquare", 0xEA0D )); // glyphnumber: 1358, Coloration end, square
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralColorationStartRound", 0xEA0E )); // glyphnumber: 1359, Coloration start, round
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralColorationStartSquare", 0xEA0C )); // glyphnumber: 1360, Coloration start, square
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemDiagonal", 0xE940 )); // glyphnumber: 1361, Combining stem diagonal
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemDown", 0xE93F )); // glyphnumber: 1362, Combining stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemDownFlagExtended", 0xE948 )); // glyphnumber: 1363, Combining stem with extended flag down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemDownFlagFlared", 0xE946 )); // glyphnumber: 1364, Combining stem with flared flag down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemDownFlagFusa", 0xE94C )); // glyphnumber: 1365, Combining stem with fusa flag down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemDownFlagLeft", 0xE944 )); // glyphnumber: 1366, Combining stem with flag left down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemDownFlagRight", 0xE942 )); // glyphnumber: 1367, Combining stem with flag right down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemDownFlagSemiminima", 0xE94A )); // glyphnumber: 1368, Combining stem with semiminima flag down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemUp", 0xE93E )); // glyphnumber: 1369, Combining stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemUpFlagExtended", 0xE947 )); // glyphnumber: 1370, Combining stem with extended flag up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemUpFlagFlared", 0xE945 )); // glyphnumber: 1371, Combining stem with flared flag up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemUpFlagFusa", 0xE94B )); // glyphnumber: 1372, Combining stem with fusa flag up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemUpFlagLeft", 0xE943 )); // glyphnumber: 1373, Combining stem with flag left up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemUpFlagRight", 0xE941 )); // glyphnumber: 1374, Combining stem with flag right up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCombStemUpFlagSemiminima", 0xE949 )); // glyphnumber: 1375, Combining stem with semiminima flag up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCustosCheckmark", 0xEA0A )); // glyphnumber: 1376, Checkmark custos
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCustosDown", 0xEA03 )); // glyphnumber: 1377, Mensural custos down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCustosTurn", 0xEA0B )); // glyphnumber: 1378, Turn-like custos
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralCustosUp", 0xEA02 )); // glyphnumber: 1379, Mensural custos up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralFclef", 0xE903 )); // glyphnumber: 1380, Mensural F clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralFclefPetrucci", 0xE904 )); // glyphnumber: 1381, Petrucci F clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralGclef", 0xE900 )); // glyphnumber: 1382, Mensural G clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralGclefPetrucci", 0xE901 )); // glyphnumber: 1383, Petrucci G clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralModusImperfectumVert", 0xE92D )); // glyphnumber: 1384, Modus imperfectum, vertical
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralModusPerfectumVert", 0xE92C )); // glyphnumber: 1385, Modus perfectum, vertical
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadLongaBlack", 0xE934 )); // glyphnumber: 1386, Longa/brevis notehead, black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadLongaBlackVoid", 0xE936 )); // glyphnumber: 1387, Longa/brevis notehead, black and void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadLongaVoid", 0xE935 )); // glyphnumber: 1388, Longa/brevis notehead, void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadLongaWhite", 0xE937 )); // glyphnumber: 1389, Longa/brevis notehead, white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadMaximaBlack", 0xE930 )); // glyphnumber: 1390, Maxima notehead, black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadMaximaBlackVoid", 0xE932 )); // glyphnumber: 1391, Maxima notehead, black and void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadMaximaVoid", 0xE931 )); // glyphnumber: 1392, Maxima notehead, void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadMaximaWhite", 0xE933 )); // glyphnumber: 1393, Maxima notehead, white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadMinimaWhite", 0xE93C )); // glyphnumber: 1394, Minima notehead, white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadSemibrevisBlack", 0xE938 )); // glyphnumber: 1395, Semibrevis notehead, black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadSemibrevisBlackVoid", 0xE93A )); // glyphnumber: 1396, Semibrevis notehead, black and void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadSemibrevisBlackVoidTurned", 0xE93B )); // glyphnumber: 1397, Semibrevis notehead, black and void (turned)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadSemibrevisVoid", 0xE939 )); // glyphnumber: 1398, Semibrevis notehead, void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralNoteheadSemiminimaWhite", 0xE93D )); // glyphnumber: 1399, Semiminima/fusa notehead, white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc2ndBlack", 0xE970 )); // glyphnumber: 1400, Oblique form, ascending 2nd, black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc2ndBlackVoid", 0xE972 )); // glyphnumber: 1401, Oblique form, ascending 2nd, black and void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc2ndVoid", 0xE971 )); // glyphnumber: 1402, Oblique form, ascending 2nd, void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc2ndWhite", 0xE973 )); // glyphnumber: 1403, Oblique form, ascending 2nd, white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc3rdBlack", 0xE974 )); // glyphnumber: 1404, Oblique form, ascending 3rd, black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc3rdBlackVoid", 0xE976 )); // glyphnumber: 1405, Oblique form, ascending 3rd, black and void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc3rdVoid", 0xE975 )); // glyphnumber: 1406, Oblique form, ascending 3rd, void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc3rdWhite", 0xE977 )); // glyphnumber: 1407, Oblique form, ascending 3rd, white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc4thBlack", 0xE978 )); // glyphnumber: 1408, Oblique form, ascending 4th, black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc4thBlackVoid", 0xE97A )); // glyphnumber: 1409, Oblique form, ascending 4th, black and void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc4thVoid", 0xE979 )); // glyphnumber: 1410, Oblique form, ascending 4th, void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc4thWhite", 0xE97B )); // glyphnumber: 1411, Oblique form, ascending 4th, white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc5thBlack", 0xE97C )); // glyphnumber: 1412, Oblique form, ascending 5th, black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc5thBlackVoid", 0xE97E )); // glyphnumber: 1413, Oblique form, ascending 5th, black and void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc5thVoid", 0xE97D )); // glyphnumber: 1414, Oblique form, ascending 5th, void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueAsc5thWhite", 0xE97F )); // glyphnumber: 1415, Oblique form, ascending 5th, white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc2ndBlack", 0xE980 )); // glyphnumber: 1416, Oblique form, descending 2nd, black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc2ndBlackVoid", 0xE982 )); // glyphnumber: 1417, Oblique form, descending 2nd, black and void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc2ndVoid", 0xE981 )); // glyphnumber: 1418, Oblique form, descending 2nd, void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc2ndWhite", 0xE983 )); // glyphnumber: 1419, Oblique form, descending 2nd, white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc3rdBlack", 0xE984 )); // glyphnumber: 1420, Oblique form, descending 3rd, black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc3rdBlackVoid", 0xE986 )); // glyphnumber: 1421, Oblique form, descending 3rd, black and void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc3rdVoid", 0xE985 )); // glyphnumber: 1422, Oblique form, descending 3rd, void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc3rdWhite", 0xE987 )); // glyphnumber: 1423, Oblique form, descending 3rd, white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc4thBlack", 0xE988 )); // glyphnumber: 1424, Oblique form, descending 4th, black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc4thBlackVoid", 0xE98A )); // glyphnumber: 1425, Oblique form, descending 4th, black and void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc4thVoid", 0xE989 )); // glyphnumber: 1426, Oblique form, descending 4th, void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc4thWhite", 0xE98B )); // glyphnumber: 1427, Oblique form, descending 4th, white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc5thBlack", 0xE98C )); // glyphnumber: 1428, Oblique form, descending 5th, black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc5thBlackVoid", 0xE98E )); // glyphnumber: 1429, Oblique form, descending 5th, black and void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc5thVoid", 0xE98D )); // glyphnumber: 1430, Oblique form, descending 5th, void
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralObliqueDesc5thWhite", 0xE98F )); // glyphnumber: 1431, Oblique form, descending 5th, white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlation1", 0xE910 )); // glyphnumber: 1432, Tempus perfectum cum prolatione perfecta (9/8)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlation10", 0xE919 )); // glyphnumber: 1433, Tempus imperfectum cum prolatione imperfecta diminution 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlation11", 0xE91A )); // glyphnumber: 1434, Tempus imperfectum cum prolatione imperfecta diminution 5
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlation2", 0xE911 )); // glyphnumber: 1435, Tempus perfectum cum prolatione imperfecta (3/4)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlation3", 0xE912 )); // glyphnumber: 1436, Tempus perfectum cum prolatione imperfecta diminution 1 (3/8)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlation4", 0xE913 )); // glyphnumber: 1437, Tempus perfectum cum prolatione perfecta diminution 2 (9/16)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlation5", 0xE914 )); // glyphnumber: 1438, Tempus imperfectum cum prolatione perfecta (6/8)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlation6", 0xE915 )); // glyphnumber: 1439, Tempus imperfectum cum prolatione imperfecta (2/4)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlation7", 0xE916 )); // glyphnumber: 1440, Tempus imperfectum cum prolatione imperfecta diminution 1 (2/2)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlation8", 0xE917 )); // glyphnumber: 1441, Tempus imperfectum cum prolatione imperfecta diminution 2 (6/16)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlation9", 0xE918 )); // glyphnumber: 1442, Tempus imperfectum cum prolatione imperfecta diminution 3 (2/2)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlationCombiningDot", 0xE920 )); // glyphnumber: 1443, Combining dot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlationCombiningDotVoid", 0xE924 )); // glyphnumber: 1444, Combining void dot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlationCombiningStroke", 0xE925 )); // glyphnumber: 1445, Combining vertical stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlationCombiningThreeDots", 0xE922 )); // glyphnumber: 1446, Combining three dots horizontal
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlationCombiningThreeDotsTri", 0xE923 )); // glyphnumber: 1447, Combining three dots triangular
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProlationCombiningTwoDots", 0xE921 )); // glyphnumber: 1448, Combining two dots
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProportion1", 0xE926 )); // glyphnumber: 1449, Mensural proportion 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProportion2", 0xE927 )); // glyphnumber: 1450, Mensural proportion 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProportion3", 0xE928 )); // glyphnumber: 1451, Mensural proportion 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProportion4", 0xE929 )); // glyphnumber: 1452, Mensural proportion 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProportionMajor", 0xE92B )); // glyphnumber: 1453, Mensural proportion major
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProportionMinor", 0xE92A )); // glyphnumber: 1454, Mensural proportion minor
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProportionProportioDupla1", 0xE91C )); // glyphnumber: 1455, Proportio dupla 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProportionProportioDupla2", 0xE91D )); // glyphnumber: 1456, Proportio dupla 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProportionProportioQuadrupla", 0xE91F )); // glyphnumber: 1457, Proportio quadrupla
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProportionProportioTripla", 0xE91E )); // glyphnumber: 1458, Proportio tripla
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralProportionTempusPerfectum", 0xE91B )); // glyphnumber: 1459, Tempus perfectum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralRestBrevis", 0xE9F3 )); // glyphnumber: 1460, Brevis rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralRestFusa", 0xE9F7 )); // glyphnumber: 1461, Fusa rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralRestLongaImperfecta", 0xE9F2 )); // glyphnumber: 1462, Longa imperfecta rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralRestLongaPerfecta", 0xE9F1 )); // glyphnumber: 1463, Longa perfecta rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralRestMaxima", 0xE9F0 )); // glyphnumber: 1464, Maxima rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralRestMinima", 0xE9F5 )); // glyphnumber: 1465, Minima rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralRestSemibrevis", 0xE9F4 )); // glyphnumber: 1466, Semibrevis rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralRestSemifusa", 0xE9F8 )); // glyphnumber: 1467, Semifusa rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralRestSemiminima", 0xE9F6 )); // glyphnumber: 1468, Semiminima rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralSignumDown", 0xEA01 )); // glyphnumber: 1469, Signum congruentiae down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralSignumUp", 0xEA00 )); // glyphnumber: 1470, Signum congruentiae up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralTempusImperfectumHoriz", 0xE92F )); // glyphnumber: 1471, Tempus imperfectum, horizontal
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralTempusPerfectumHoriz", 0xE92E )); // glyphnumber: 1472, Tempus perfectum, horizontal
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralWhiteBrevis", 0xE95E )); // glyphnumber: 1473, White mensural brevis
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralWhiteFusa", 0xE961 )); // glyphnumber: 1474, White mensural fusa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralWhiteLonga", 0xE95D )); // glyphnumber: 1475, White mensural longa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralWhiteMaxima", 0xE95C )); // glyphnumber: 1476, White mensural maxima
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralWhiteMinima", 0xE95F )); // glyphnumber: 1477, White mensural minima
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "mensuralWhiteSemiminima", 0xE960 )); // glyphnumber: 1478, White mensural semiminima
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metAugmentationDot", 0xECB7 )); // glyphnumber: 1479, Augmentation dot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote1024thDown", 0xECB6 )); // glyphnumber: 1480, 1024th note (semihemidemisemihemidemisemiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote1024thUp", 0xECB5 )); // glyphnumber: 1481, 1024th note (semihemidemisemihemidemisemiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote128thDown", 0xECB0 )); // glyphnumber: 1482, 128th note (semihemidemisemiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote128thUp", 0xECAF )); // glyphnumber: 1483, 128th note (semihemidemisemiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote16thDown", 0xECAA )); // glyphnumber: 1484, 16th note (semiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote16thUp", 0xECA9 )); // glyphnumber: 1485, 16th note (semiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote256thDown", 0xECB2 )); // glyphnumber: 1486, 256th note (demisemihemidemisemiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote256thUp", 0xECB1 )); // glyphnumber: 1487, 256th note (demisemihemidemisemiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote32ndDown", 0xECAC )); // glyphnumber: 1488, 32nd note (demisemiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote32ndUp", 0xECAB )); // glyphnumber: 1489, 32nd note (demisemiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote512thDown", 0xECB4 )); // glyphnumber: 1490, 512th note (hemidemisemihemidemisemiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote512thUp", 0xECB3 )); // glyphnumber: 1491, 512th note (hemidemisemihemidemisemiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote64thDown", 0xECAE )); // glyphnumber: 1492, 64th note (hemidemisemiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote64thUp", 0xECAD )); // glyphnumber: 1493, 64th note (hemidemisemiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote8thDown", 0xECA8 )); // glyphnumber: 1494, Eighth note (quaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNote8thUp", 0xECA7 )); // glyphnumber: 1495, Eighth note (quaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNoteDoubleWhole", 0xECA0 )); // glyphnumber: 1496, Double whole note (breve)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNoteDoubleWholeSquare", 0xECA1 )); // glyphnumber: 1497, Double whole note (square)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNoteHalfDown", 0xECA4 )); // glyphnumber: 1498, Half note (minim) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNoteHalfUp", 0xECA3 )); // glyphnumber: 1499, Half note (minim) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNoteQuarterDown", 0xECA6 )); // glyphnumber: 1500, Quarter note (crotchet) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNoteQuarterUp", 0xECA5 )); // glyphnumber: 1501, Quarter note (crotchet) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metNoteWhole", 0xECA2 )); // glyphnumber: 1502, Whole note (semibreve)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metricModulationArrowLeft", 0xEC63 )); // glyphnumber: 1503, Left-pointing arrow for metric modulation
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "metricModulationArrowRight", 0xEC64 )); // glyphnumber: 1504, Right-pointing arrow for metric modulation
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "miscDoNotCopy", 0xEC61 )); // glyphnumber: 1505, Do not copy
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "miscDoNotPhotocopy", 0xEC60 )); // glyphnumber: 1506, Do not photocopy
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "miscEyeglasses", 0xEC62 )); // glyphnumber: 1507, Eyeglasses
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note1024thDown", 0xE1E6 )); // glyphnumber: 1508, 1024th note (semihemidemisemihemidemisemiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note1024thUp", 0xE1E5 )); // glyphnumber: 1509, 1024th note (semihemidemisemihemidemisemiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note128thDown", 0xE1E0 )); // glyphnumber: 1510, 128th note (semihemidemisemiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note128thUp", 0xE1DF )); // glyphnumber: 1511, 128th note (semihemidemisemiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note16thDown", 0xE1DA )); // glyphnumber: 1512, 16th note (semiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note16thUp", 0xE1D9 )); // glyphnumber: 1513, 16th note (semiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note256thDown", 0xE1E2 )); // glyphnumber: 1514, 256th note (demisemihemidemisemiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note256thUp", 0xE1E1 )); // glyphnumber: 1515, 256th note (demisemihemidemisemiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note32ndDown", 0xE1DC )); // glyphnumber: 1516, 32nd note (demisemiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note32ndUp", 0xE1DB )); // glyphnumber: 1517, 32nd note (demisemiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note512thDown", 0xE1E4 )); // glyphnumber: 1518, 512th note (hemidemisemihemidemisemiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note512thUp", 0xE1E3 )); // glyphnumber: 1519, 512th note (hemidemisemihemidemisemiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note64thDown", 0xE1DE )); // glyphnumber: 1520, 64th note (hemidemisemiquaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note64thUp", 0xE1DD )); // glyphnumber: 1521, 64th note (hemidemisemiquaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note8thDown", 0xE1D8 )); // glyphnumber: 1522, Eighth note (quaver) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "note8thUp", 0xE1D7 )); // glyphnumber: 1523, Eighth note (quaver) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteABlack", 0xE197 )); // glyphnumber: 1524, A (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteAFlatBlack", 0xE196 )); // glyphnumber: 1525, A flat (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteAFlatHalf", 0xE17F )); // glyphnumber: 1526, A flat (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteAFlatWhole", 0xE168 )); // glyphnumber: 1527, A flat (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteAHalf", 0xE180 )); // glyphnumber: 1528, A (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteASharpBlack", 0xE198 )); // glyphnumber: 1529, A sharp (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteASharpHalf", 0xE181 )); // glyphnumber: 1530, A sharp (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteASharpWhole", 0xE16A )); // glyphnumber: 1531, A sharp (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteAWhole", 0xE169 )); // glyphnumber: 1532, A (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteBBlack", 0xE19A )); // glyphnumber: 1533, B (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteBFlatBlack", 0xE199 )); // glyphnumber: 1534, B flat (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteBFlatHalf", 0xE182 )); // glyphnumber: 1535, B flat (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteBFlatWhole", 0xE16B )); // glyphnumber: 1536, B flat (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteBHalf", 0xE183 )); // glyphnumber: 1537, B (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteBSharpBlack", 0xE19B )); // glyphnumber: 1538, B sharp (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteBSharpHalf", 0xE184 )); // glyphnumber: 1539, B sharp (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteBSharpWhole", 0xE16D )); // glyphnumber: 1540, B sharp (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteBWhole", 0xE16C )); // glyphnumber: 1541, B (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteCBlack", 0xE19D )); // glyphnumber: 1542, C (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteCFlatBlack", 0xE19C )); // glyphnumber: 1543, C flat (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteCFlatHalf", 0xE185 )); // glyphnumber: 1544, C flat (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteCFlatWhole", 0xE16E )); // glyphnumber: 1545, C flat (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteCHalf", 0xE186 )); // glyphnumber: 1546, C (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteCSharpBlack", 0xE19E )); // glyphnumber: 1547, C sharp (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteCSharpHalf", 0xE187 )); // glyphnumber: 1548, C sharp (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteCSharpWhole", 0xE170 )); // glyphnumber: 1549, C sharp (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteCWhole", 0xE16F )); // glyphnumber: 1550, C (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDBlack", 0xE1A0 )); // glyphnumber: 1551, D (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDFlatBlack", 0xE19F )); // glyphnumber: 1552, D flat (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDFlatHalf", 0xE188 )); // glyphnumber: 1553, D flat (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDFlatWhole", 0xE171 )); // glyphnumber: 1554, D flat (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDHalf", 0xE189 )); // glyphnumber: 1555, D (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDSharpBlack", 0xE1A1 )); // glyphnumber: 1556, D sharp (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDSharpHalf", 0xE18A )); // glyphnumber: 1557, D sharp (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDSharpWhole", 0xE173 )); // glyphnumber: 1558, D sharp (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDWhole", 0xE172 )); // glyphnumber: 1559, D (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDoBlack", 0xE160 )); // glyphnumber: 1560, Do (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDoHalf", 0xE158 )); // glyphnumber: 1561, Do (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDoWhole", 0xE150 )); // glyphnumber: 1562, Do (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDoubleWhole", 0xE1D0 )); // glyphnumber: 1563, Double whole note (breve)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteDoubleWholeSquare", 0xE1D1 )); // glyphnumber: 1564, Double whole note (square)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteEBlack", 0xE1A3 )); // glyphnumber: 1565, E (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteEFlatBlack", 0xE1A2 )); // glyphnumber: 1566, E flat (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteEFlatHalf", 0xE18B )); // glyphnumber: 1567, E flat (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteEFlatWhole", 0xE174 )); // glyphnumber: 1568, E flat (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteEHalf", 0xE18C )); // glyphnumber: 1569, E (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteESharpBlack", 0xE1A4 )); // glyphnumber: 1570, E sharp (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteESharpHalf", 0xE18D )); // glyphnumber: 1571, E sharp (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteESharpWhole", 0xE176 )); // glyphnumber: 1572, E sharp (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteEWhole", 0xE175 )); // glyphnumber: 1573, E (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteEmptyBlack", 0xE1AF )); // glyphnumber: 1574, Empty black note
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteEmptyHalf", 0xE1AE )); // glyphnumber: 1575, Empty half note
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteEmptyWhole", 0xE1AD )); // glyphnumber: 1576, Empty whole note
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteFBlack", 0xE1A6 )); // glyphnumber: 1577, F (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteFFlatBlack", 0xE1A5 )); // glyphnumber: 1578, F flat (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteFFlatHalf", 0xE18E )); // glyphnumber: 1579, F flat (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteFFlatWhole", 0xE177 )); // glyphnumber: 1580, F flat (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteFHalf", 0xE18F )); // glyphnumber: 1581, F (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteFSharpBlack", 0xE1A7 )); // glyphnumber: 1582, F sharp (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteFSharpHalf", 0xE190 )); // glyphnumber: 1583, F sharp (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteFSharpWhole", 0xE179 )); // glyphnumber: 1584, F sharp (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteFWhole", 0xE178 )); // glyphnumber: 1585, F (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteFaBlack", 0xE163 )); // glyphnumber: 1586, Fa (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteFaHalf", 0xE15B )); // glyphnumber: 1587, Fa (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteFaWhole", 0xE153 )); // glyphnumber: 1588, Fa (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteGBlack", 0xE1A9 )); // glyphnumber: 1589, G (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteGFlatBlack", 0xE1A8 )); // glyphnumber: 1590, G flat (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteGFlatHalf", 0xE191 )); // glyphnumber: 1591, G flat (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteGFlatWhole", 0xE17A )); // glyphnumber: 1592, G flat (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteGHalf", 0xE192 )); // glyphnumber: 1593, G (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteGSharpBlack", 0xE1AA )); // glyphnumber: 1594, G sharp (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteGSharpHalf", 0xE193 )); // glyphnumber: 1595, G sharp (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteGSharpWhole", 0xE17C )); // glyphnumber: 1596, G sharp (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteGWhole", 0xE17B )); // glyphnumber: 1597, G (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteHBlack", 0xE1AB )); // glyphnumber: 1598, H (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteHHalf", 0xE194 )); // glyphnumber: 1599, H (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteHSharpBlack", 0xE1AC )); // glyphnumber: 1600, H sharp (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteHSharpHalf", 0xE195 )); // glyphnumber: 1601, H sharp (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteHSharpWhole", 0xE17E )); // glyphnumber: 1602, H sharp (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteHWhole", 0xE17D )); // glyphnumber: 1603, H (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteHalfDown", 0xE1D4 )); // glyphnumber: 1604, Half note (minim) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteHalfUp", 0xE1D3 )); // glyphnumber: 1605, Half note (minim) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteLaBlack", 0xE165 )); // glyphnumber: 1606, La (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteLaHalf", 0xE15D )); // glyphnumber: 1607, La (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteLaWhole", 0xE155 )); // glyphnumber: 1608, La (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteMiBlack", 0xE162 )); // glyphnumber: 1609, Mi (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteMiHalf", 0xE15A )); // glyphnumber: 1610, Mi (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteMiWhole", 0xE152 )); // glyphnumber: 1611, Mi (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteQuarterDown", 0xE1D6 )); // glyphnumber: 1612, Quarter note (crotchet) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteQuarterUp", 0xE1D5 )); // glyphnumber: 1613, Quarter note (crotchet) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteReBlack", 0xE161 )); // glyphnumber: 1614, Re (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteReHalf", 0xE159 )); // glyphnumber: 1615, Re (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteReWhole", 0xE151 )); // glyphnumber: 1616, Re (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeArrowheadLeftBlack", 0xE1C9 )); // glyphnumber: 1617, Arrowhead left black (Funk 7-shape re)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeArrowheadLeftDoubleWhole", 0xECDC )); // glyphnumber: 1618, Arrowhead left double whole (Funk 7-shape re)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeArrowheadLeftWhite", 0xE1C8 )); // glyphnumber: 1619, Arrowhead left white (Funk 7-shape re)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeDiamondBlack", 0xE1B9 )); // glyphnumber: 1620, Diamond black (4-shape mi; 7-shape mi)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeDiamondDoubleWhole", 0xECD4 )); // glyphnumber: 1621, Diamond double whole (4-shape mi; 7-shape mi)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeDiamondWhite", 0xE1B8 )); // glyphnumber: 1622, Diamond white (4-shape mi; 7-shape mi)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeIsoscelesTriangleBlack", 0xE1C5 )); // glyphnumber: 1623, Isosceles triangle black (Walker 7-shape ti)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeIsoscelesTriangleDoubleWhole", 0xECDA )); // glyphnumber: 1624, Isosceles triangle double whole (Walker 7-shape ti)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeIsoscelesTriangleWhite", 0xE1C4 )); // glyphnumber: 1625, Isosceles triangle white (Walker 7-shape ti)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeKeystoneBlack", 0xE1C1 )); // glyphnumber: 1626, Inverted keystone black (Walker 7-shape do)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeKeystoneDoubleWhole", 0xECD8 )); // glyphnumber: 1627, Inverted keystone double whole (Walker 7-shape do)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeKeystoneWhite", 0xE1C0 )); // glyphnumber: 1628, Inverted keystone white (Walker 7-shape do)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeMoonBlack", 0xE1BD )); // glyphnumber: 1629, Moon black (Aikin 7-shape re)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeMoonDoubleWhole", 0xECD6 )); // glyphnumber: 1630, Moon double whole (Aikin 7-shape re)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeMoonLeftBlack", 0xE1C7 )); // glyphnumber: 1631, Moon left black (Funk 7-shape do)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeMoonLeftDoubleWhole", 0xECDB )); // glyphnumber: 1632, Moon left double whole (Funk 7-shape do)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeMoonLeftWhite", 0xE1C6 )); // glyphnumber: 1633, Moon left white (Funk 7-shape do)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeMoonWhite", 0xE1BC )); // glyphnumber: 1634, Moon white (Aikin 7-shape re)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeQuarterMoonBlack", 0xE1C3 )); // glyphnumber: 1635, Quarter moon black (Walker 7-shape re)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeQuarterMoonDoubleWhole", 0xECD9 )); // glyphnumber: 1636, Quarter moon double whole (Walker 7-shape re)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeQuarterMoonWhite", 0xE1C2 )); // glyphnumber: 1637, Quarter moon white (Walker 7-shape re)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeRoundBlack", 0xE1B1 )); // glyphnumber: 1638, Round black (4-shape sol; 7-shape so)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeRoundDoubleWhole", 0xECD0 )); // glyphnumber: 1639, Round double whole (4-shape sol; 7-shape so)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeRoundWhite", 0xE1B0 )); // glyphnumber: 1640, Round white (4-shape sol; 7-shape so)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeSquareBlack", 0xE1B3 )); // glyphnumber: 1641, Square black (4-shape la; Aikin 7-shape la)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeSquareDoubleWhole", 0xECD1 )); // glyphnumber: 1642, Square double whole (4-shape la; Aikin 7-shape la)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeSquareWhite", 0xE1B2 )); // glyphnumber: 1643, Square white (4-shape la; Aikin 7-shape la)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleLeftBlack", 0xE1B7 )); // glyphnumber: 1644, Triangle left black (stem up; 4-shape fa; 7-shape fa)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleLeftDoubleWhole", 0xECD3 )); // glyphnumber: 1645, Triangle left double whole (stem up; 4-shape fa; 7-shape fa)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleLeftWhite", 0xE1B6 )); // glyphnumber: 1646, Triangle left white (stem up; 4-shape fa; 7-shape fa)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleRightBlack", 0xE1B5 )); // glyphnumber: 1647, Triangle right black (stem down; 4-shape fa; 7-shape fa)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleRightDoubleWhole", 0xECD2 )); // glyphnumber: 1648, Triangle right double whole (stem down; 4-shape fa; 7-shape fa)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleRightWhite", 0xE1B4 )); // glyphnumber: 1649, Triangle right white (stem down; 4-shape fa; 7-shape fa)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleRoundBlack", 0xE1BF )); // glyphnumber: 1650, Triangle-round black (Aikin 7-shape ti)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleRoundDoubleWhole", 0xECD7 )); // glyphnumber: 1651, Triangle-round white (Aikin 7-shape ti)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleRoundLeftBlack", 0xE1CB )); // glyphnumber: 1652, Triangle-round left black (Funk 7-shape ti)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleRoundLeftDoubleWhole", 0xECDD )); // glyphnumber: 1653, Triangle-round left double whole (Funk 7-shape ti)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleRoundLeftWhite", 0xE1CA )); // glyphnumber: 1654, Triangle-round left white (Funk 7-shape ti)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleRoundWhite", 0xE1BE )); // glyphnumber: 1655, Triangle-round white (Aikin 7-shape ti)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleUpBlack", 0xE1BB )); // glyphnumber: 1656, Triangle up black (Aikin 7-shape do)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleUpDoubleWhole", 0xECD5 )); // glyphnumber: 1657, Triangle up double whole (Aikin 7-shape do)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteShapeTriangleUpWhite", 0xE1BA )); // glyphnumber: 1658, Triangle up white (Aikin 7-shape do)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteSiBlack", 0xE167 )); // glyphnumber: 1659, Si (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteSiHalf", 0xE15F )); // glyphnumber: 1660, Si (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteSiWhole", 0xE157 )); // glyphnumber: 1661, Si (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteSoBlack", 0xE164 )); // glyphnumber: 1662, So (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteSoHalf", 0xE15C )); // glyphnumber: 1663, So (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteSoWhole", 0xE154 )); // glyphnumber: 1664, So (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteTiBlack", 0xE166 )); // glyphnumber: 1665, Ti (black note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteTiHalf", 0xE15E )); // glyphnumber: 1666, Ti (half note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteTiWhole", 0xE156 )); // glyphnumber: 1667, Ti (whole note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteWhole", 0xE1D2 )); // glyphnumber: 1668, Whole note (semibreve)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadBlack", 0xE0A4 )); // glyphnumber: 1669, Black notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircleSlash", 0xE0F7 )); // glyphnumber: 1670, Circle slash notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircleX", 0xE0B3 )); // glyphnumber: 1671, Circle X notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircleXDoubleWhole", 0xE0B0 )); // glyphnumber: 1672, Circle X double whole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircleXHalf", 0xE0B2 )); // glyphnumber: 1673, Circle X half
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircleXWhole", 0xE0B1 )); // glyphnumber: 1674, Circle X whole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircledBlack", 0xE0E4 )); // glyphnumber: 1675, Circled black notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircledBlackLarge", 0xE0E8 )); // glyphnumber: 1676, Black notehead in large circle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircledDoubleWhole", 0xE0E7 )); // glyphnumber: 1677, Circled double whole notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircledDoubleWholeLarge", 0xE0EB )); // glyphnumber: 1678, Double whole notehead in large circle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircledHalf", 0xE0E5 )); // glyphnumber: 1679, Circled half notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircledHalfLarge", 0xE0E9 )); // glyphnumber: 1680, Half notehead in large circle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircledWhole", 0xE0E6 )); // glyphnumber: 1681, Circled whole notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircledWholeLarge", 0xE0EA )); // glyphnumber: 1682, Whole notehead in large circle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadCircledXLarge", 0xE0EC )); // glyphnumber: 1683, Cross notehead in large circle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterDoubleWhole2nd", 0xE124 )); // glyphnumber: 1684, Double whole note cluster, 2nd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterDoubleWhole3rd", 0xE128 )); // glyphnumber: 1685, Double whole note cluster, 3rd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterDoubleWholeBottom", 0xE12E )); // glyphnumber: 1686, Combining double whole note cluster, bottom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterDoubleWholeMiddle", 0xE12D )); // glyphnumber: 1687, Combining double whole note cluster, middle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterDoubleWholeTop", 0xE12C )); // glyphnumber: 1688, Combining double whole note cluster, top
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterHalf2nd", 0xE126 )); // glyphnumber: 1689, Half note cluster, 2nd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterHalf3rd", 0xE12A )); // glyphnumber: 1690, Half note cluster, 3rd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterHalfBottom", 0xE134 )); // glyphnumber: 1691, Combining half note cluster, bottom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterHalfMiddle", 0xE133 )); // glyphnumber: 1692, Combining half note cluster, middle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterHalfTop", 0xE132 )); // glyphnumber: 1693, Combining half note cluster, top
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterQuarter2nd", 0xE127 )); // glyphnumber: 1694, Quarter note cluster, 2nd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterQuarter3rd", 0xE12B )); // glyphnumber: 1695, Quarter note cluster, 3rd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterQuarterBottom", 0xE137 )); // glyphnumber: 1696, Combining quarter note cluster, bottom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterQuarterMiddle", 0xE136 )); // glyphnumber: 1697, Combining quarter note cluster, middle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterQuarterTop", 0xE135 )); // glyphnumber: 1698, Combining quarter note cluster, top
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterRoundBlack", 0xE123 )); // glyphnumber: 1699, Cluster notehead black (round)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterRoundWhite", 0xE122 )); // glyphnumber: 1700, Cluster notehead white (round)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterSquareBlack", 0xE121 )); // glyphnumber: 1701, Cluster notehead black (square)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterSquareWhite", 0xE120 )); // glyphnumber: 1702, Cluster notehead white (square)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterWhole2nd", 0xE125 )); // glyphnumber: 1703, Whole note cluster, 2nd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterWhole3rd", 0xE129 )); // glyphnumber: 1704, Whole note cluster, 3rd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterWholeBottom", 0xE131 )); // glyphnumber: 1705, Combining whole note cluster, bottom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterWholeMiddle", 0xE130 )); // glyphnumber: 1706, Combining whole note cluster, middle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadClusterWholeTop", 0xE12F )); // glyphnumber: 1707, Combining whole note cluster, top
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondBlack", 0xE0DB )); // glyphnumber: 1708, Diamond black notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondBlackOld", 0xE0E2 )); // glyphnumber: 1709, Diamond black notehead (old)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondBlackWide", 0xE0DC )); // glyphnumber: 1710, Diamond black notehead (wide)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondClusterBlack2nd", 0xE139 )); // glyphnumber: 1711, Black diamond cluster, 2nd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondClusterBlack3rd", 0xE13B )); // glyphnumber: 1712, Black diamond cluster, 3rd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondClusterBlackBottom", 0xE141 )); // glyphnumber: 1713, Combining black diamond cluster, bottom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondClusterBlackMiddle", 0xE140 )); // glyphnumber: 1714, Combining black diamond cluster, middle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondClusterBlackTop", 0xE13F )); // glyphnumber: 1715, Combining black diamond cluster, top
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondClusterWhite2nd", 0xE138 )); // glyphnumber: 1716, White diamond cluster, 2nd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondClusterWhite3rd", 0xE13A )); // glyphnumber: 1717, White diamond cluster, 3rd
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondClusterWhiteBottom", 0xE13E )); // glyphnumber: 1718, Combining white diamond cluster, bottom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondClusterWhiteMiddle", 0xE13D )); // glyphnumber: 1719, Combining white diamond cluster, middle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondClusterWhiteTop", 0xE13C )); // glyphnumber: 1720, Combining white diamond cluster, top
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondDoubleWhole", 0xE0D7 )); // glyphnumber: 1721, Diamond double whole notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondDoubleWholeOld", 0xE0DF )); // glyphnumber: 1722, Diamond double whole notehead (old)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondHalf", 0xE0D9 )); // glyphnumber: 1723, Diamond half notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondHalfFilled", 0xE0E3 )); // glyphnumber: 1724, Half-filled diamond notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondHalfOld", 0xE0E1 )); // glyphnumber: 1725, Diamond half notehead (old)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondHalfWide", 0xE0DA )); // glyphnumber: 1726, Diamond half notehead (wide)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondOpen", 0xE0FC )); // glyphnumber: 1727, Open diamond notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondWhite", 0xE0DD )); // glyphnumber: 1728, Diamond white notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondWhiteWide", 0xE0DE )); // glyphnumber: 1729, Diamond white notehead (wide)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondWhole", 0xE0D8 )); // glyphnumber: 1730, Diamond whole notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDiamondWholeOld", 0xE0E0 )); // glyphnumber: 1731, Diamond whole notehead (old)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDoubleWhole", 0xE0A0 )); // glyphnumber: 1732, Double whole (breve) notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDoubleWholeSquare", 0xE0A1 )); // glyphnumber: 1733, Double whole (breve) notehead (square)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadDoubleWholeWithX", 0xE0B4 )); // glyphnumber: 1734, Double whole notehead with X
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadHalf", 0xE0A3 )); // glyphnumber: 1735, Half (minim) notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadHalfFilled", 0xE0FB )); // glyphnumber: 1736, Filled half (minim) notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadHalfWithX", 0xE0B6 )); // glyphnumber: 1737, Half notehead with X
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadHeavyX", 0xE0F8 )); // glyphnumber: 1738, Heavy X notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadHeavyXHat", 0xE0F9 )); // glyphnumber: 1739, Heavy X with hat notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadLargeArrowDownBlack", 0xE0F4 )); // glyphnumber: 1740, Large arrow down (lowest pitch) black notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadLargeArrowDownDoubleWhole", 0xE0F1 )); // glyphnumber: 1741, Large arrow down (lowest pitch) double whole notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadLargeArrowDownHalf", 0xE0F3 )); // glyphnumber: 1742, Large arrow down (lowest pitch) half notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadLargeArrowDownWhole", 0xE0F2 )); // glyphnumber: 1743, Large arrow down (lowest pitch) whole notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadLargeArrowUpBlack", 0xE0F0 )); // glyphnumber: 1744, Large arrow up (highest pitch) black notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadLargeArrowUpDoubleWhole", 0xE0ED )); // glyphnumber: 1745, Large arrow up (highest pitch) double whole notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadLargeArrowUpHalf", 0xE0EF )); // glyphnumber: 1746, Large arrow up (highest pitch) half notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadLargeArrowUpWhole", 0xE0EE )); // glyphnumber: 1747, Large arrow up (highest pitch) whole notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadMoonBlack", 0xE0CB )); // glyphnumber: 1748, Moon notehead black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadMoonWhite", 0xE0CA )); // glyphnumber: 1749, Moon notehead white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadNull", 0xE0A5 )); // glyphnumber: 1750, Null notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadParenthesis", 0xE0CE )); // glyphnumber: 1751, Parenthesis notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadParenthesisLeft", 0xE0F5 )); // glyphnumber: 1752, Opening parenthesis
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadParenthesisRight", 0xE0F6 )); // glyphnumber: 1753, Closing parenthesis
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadPlusBlack", 0xE0AF )); // glyphnumber: 1754, Plus notehead black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadPlusDoubleWhole", 0xE0AC )); // glyphnumber: 1755, Plus notehead double whole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadPlusHalf", 0xE0AE )); // glyphnumber: 1756, Plus notehead half
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadPlusWhole", 0xE0AD )); // glyphnumber: 1757, Plus notehead whole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRectangularClusterBlackBottom", 0xE144 )); // glyphnumber: 1758, Combining black rectangular cluster, bottom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRectangularClusterBlackMiddle", 0xE143 )); // glyphnumber: 1759, Combining black rectangular cluster, middle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRectangularClusterBlackTop", 0xE142 )); // glyphnumber: 1760, Combining black rectangular cluster, top
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRectangularClusterWhiteBottom", 0xE147 )); // glyphnumber: 1761, Combining white rectangular cluster, bottom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRectangularClusterWhiteMiddle", 0xE146 )); // glyphnumber: 1762, Combining white rectangular cluster, middle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRectangularClusterWhiteTop", 0xE145 )); // glyphnumber: 1763, Combining white rectangular cluster, top
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRoundBlack", 0xE113 )); // glyphnumber: 1764, Round black notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRoundBlackDoubleSlashed", 0xE11C )); // glyphnumber: 1765, Round black notehead, double slashed
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRoundBlackLarge", 0xE110 )); // glyphnumber: 1766, Large round black notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRoundBlackSlashed", 0xE118 )); // glyphnumber: 1767, Round black notehead, slashed
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRoundBlackSlashedLarge", 0xE116 )); // glyphnumber: 1768, Large round black notehead, slashed
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRoundWhite", 0xE114 )); // glyphnumber: 1769, Round white notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRoundWhiteDoubleSlashed", 0xE11D )); // glyphnumber: 1770, Round white notehead, double slashed
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRoundWhiteLarge", 0xE111 )); // glyphnumber: 1771, Large round white notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRoundWhiteSlashed", 0xE119 )); // glyphnumber: 1772, Round white notehead, slashed
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRoundWhiteSlashedLarge", 0xE117 )); // glyphnumber: 1773, Large round white notehead, slashed
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRoundWhiteWithDot", 0xE115 )); // glyphnumber: 1774, Round white notehead with dot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadRoundWhiteWithDotLarge", 0xE112 )); // glyphnumber: 1775, Large round white notehead with dot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashDiamondWhite", 0xE104 )); // glyphnumber: 1776, Large white diamond
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashHorizontalEnds", 0xE101 )); // glyphnumber: 1777, Slash with horizontal ends
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashHorizontalEndsMuted", 0xE108 )); // glyphnumber: 1778, Muted slash with horizontal ends
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashVerticalEnds", 0xE100 )); // glyphnumber: 1779, Slash with vertical ends
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashVerticalEndsMuted", 0xE107 )); // glyphnumber: 1780, Muted slash with vertical ends
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashVerticalEndsSmall", 0xE105 )); // glyphnumber: 1781, Small slash with vertical ends
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashWhiteDoubleWhole", 0xE10A )); // glyphnumber: 1782, White slash double whole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashWhiteHalf", 0xE103 )); // glyphnumber: 1783, White slash half
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashWhiteMuted", 0xE109 )); // glyphnumber: 1784, Muted white slash
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashWhiteWhole", 0xE102 )); // glyphnumber: 1785, White slash whole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashX", 0xE106 )); // glyphnumber: 1786, Large X notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashedBlack1", 0xE0CF )); // glyphnumber: 1787, Slashed black notehead (bottom left to top right)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashedBlack2", 0xE0D0 )); // glyphnumber: 1788, Slashed black notehead (top left to bottom right)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashedDoubleWhole1", 0xE0D5 )); // glyphnumber: 1789, Slashed double whole notehead (bottom left to top right)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashedDoubleWhole2", 0xE0D6 )); // glyphnumber: 1790, Slashed double whole notehead (top left to bottom right)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashedHalf1", 0xE0D1 )); // glyphnumber: 1791, Slashed half notehead (bottom left to top right)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashedHalf2", 0xE0D2 )); // glyphnumber: 1792, Slashed half notehead (top left to bottom right)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashedWhole1", 0xE0D3 )); // glyphnumber: 1793, Slashed whole notehead (bottom left to top right)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSlashedWhole2", 0xE0D4 )); // glyphnumber: 1794, Slashed whole notehead (top left to bottom right)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSquareBlack", 0xE0B9 )); // glyphnumber: 1795, Square notehead black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSquareBlackLarge", 0xE11A )); // glyphnumber: 1796, Large square black notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSquareBlackWhite", 0xE11B )); // glyphnumber: 1797, Large square white notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadSquareWhite", 0xE0B8 )); // glyphnumber: 1798, Square notehead white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleDownBlack", 0xE0C7 )); // glyphnumber: 1799, Triangle notehead down black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleDownDoubleWhole", 0xE0C3 )); // glyphnumber: 1800, Triangle notehead down double whole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleDownHalf", 0xE0C5 )); // glyphnumber: 1801, Triangle notehead down half
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleDownWhite", 0xE0C6 )); // glyphnumber: 1802, Triangle notehead down white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleDownWhole", 0xE0C4 )); // glyphnumber: 1803, Triangle notehead down whole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleLeftBlack", 0xE0C0 )); // glyphnumber: 1804, Triangle notehead left black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleLeftWhite", 0xE0BF )); // glyphnumber: 1805, Triangle notehead left white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleRightBlack", 0xE0C2 )); // glyphnumber: 1806, Triangle notehead right black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleRightWhite", 0xE0C1 )); // glyphnumber: 1807, Triangle notehead right white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleRoundDownBlack", 0xE0CD )); // glyphnumber: 1808, Triangle-round notehead down black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleRoundDownWhite", 0xE0CC )); // glyphnumber: 1809, Triangle-round notehead down white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleUpBlack", 0xE0BE )); // glyphnumber: 1810, Triangle notehead up black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleUpDoubleWhole", 0xE0BA )); // glyphnumber: 1811, Triangle notehead up double whole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleUpHalf", 0xE0BC )); // glyphnumber: 1812, Triangle notehead up half
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleUpRightBlack", 0xE0C9 )); // glyphnumber: 1813, Triangle notehead up right black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleUpRightWhite", 0xE0C8 )); // glyphnumber: 1814, Triangle notehead up right white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleUpWhite", 0xE0BD )); // glyphnumber: 1815, Triangle notehead up white
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadTriangleUpWhole", 0xE0BB )); // glyphnumber: 1816, Triangle notehead up whole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadVoidWithX", 0xE0B7 )); // glyphnumber: 1817, Void notehead with X
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadWhole", 0xE0A2 )); // glyphnumber: 1818, Whole (semibreve) notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadWholeFilled", 0xE0FA )); // glyphnumber: 1819, Filled whole (semibreve) notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadWholeWithX", 0xE0B5 )); // glyphnumber: 1820, Whole notehead with X
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadXBlack", 0xE0A9 )); // glyphnumber: 1821, X notehead black
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadXDoubleWhole", 0xE0A6 )); // glyphnumber: 1822, X notehead double whole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadXHalf", 0xE0A8 )); // glyphnumber: 1823, X notehead half
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadXOrnate", 0xE0AA )); // glyphnumber: 1824, Ornate X notehead
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadXOrnateEllipse", 0xE0AB )); // glyphnumber: 1825, Ornate X notehead in ellipse
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "noteheadXWhole", 0xE0A7 )); // glyphnumber: 1826, X notehead whole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "octaveBaselineA", 0xEC91 )); // glyphnumber: 1827, a (baseline)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "octaveBaselineB", 0xEC93 )); // glyphnumber: 1828, b (baseline)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "octaveBaselineM", 0xEC95 )); // glyphnumber: 1829, m (baseline)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "octaveBaselineV", 0xEC97 )); // glyphnumber: 1830, v (baseline)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "octaveBassa", 0xE51F )); // glyphnumber: 1831, Bassa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "octaveLoco", 0xEC90 )); // glyphnumber: 1832, Loco
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "octaveParensLeft", 0xE51A )); // glyphnumber: 1833, Left parenthesis for octave signs
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "octaveParensRight", 0xE51B )); // glyphnumber: 1834, Right parenthesis for octave signs
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "octaveSuperscriptA", 0xEC92 )); // glyphnumber: 1835, a (superscript)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "octaveSuperscriptB", 0xEC94 )); // glyphnumber: 1836, b (superscript)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "octaveSuperscriptM", 0xEC96 )); // glyphnumber: 1837, m (superscript)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "octaveSuperscriptV", 0xEC98 )); // glyphnumber: 1838, v (superscript)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentBottomLeftConcaveStroke", 0xE59A )); // glyphnumber: 1839, Ornament bottom left concave stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentBottomLeftConcaveStrokeLarge", 0xE59B )); // glyphnumber: 1840, Ornament bottom left concave stroke, large
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentBottomLeftConvexStroke", 0xE59C )); // glyphnumber: 1841, Ornament bottom left convex stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentBottomRightConcaveStroke", 0xE5A7 )); // glyphnumber: 1842, Ornament bottom right concave stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentBottomRightConvexStroke", 0xE5A8 )); // glyphnumber: 1843, Ornament bottom right convex stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentComma", 0xE581 )); // glyphnumber: 1844, Comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentDoubleObliqueLinesAfterNote", 0xE57E )); // glyphnumber: 1845, Double oblique straight lines NW-SE
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentDoubleObliqueLinesBeforeNote", 0xE57D )); // glyphnumber: 1846, Double oblique straight lines SW-NE
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentDownCurve", 0xE578 )); // glyphnumber: 1847, Curve below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentHaydn", 0xE56F )); // glyphnumber: 1848, Haydn ornament
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentHighLeftConcaveStroke", 0xE592 )); // glyphnumber: 1849, Ornament high left concave stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentHighLeftConvexStroke", 0xE593 )); // glyphnumber: 1850, Ornament high left convex stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentHighRightConcaveStroke", 0xE5A2 )); // glyphnumber: 1851, Ornament high right concave stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentHighRightConvexStroke", 0xE5A3 )); // glyphnumber: 1852, Ornament high right convex stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentHookAfterNote", 0xE576 )); // glyphnumber: 1853, Hook after note
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentHookBeforeNote", 0xE575 )); // glyphnumber: 1854, Hook before note
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentLeftFacingHalfCircle", 0xE572 )); // glyphnumber: 1855, Left-facing half circle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentLeftFacingHook", 0xE574 )); // glyphnumber: 1856, Left-facing hook
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentLeftPlus", 0xE597 )); // glyphnumber: 1857, Ornament left +
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentLeftShakeT", 0xE596 )); // glyphnumber: 1858, Ornament left shake t
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentLeftVerticalStroke", 0xE594 )); // glyphnumber: 1859, Ornament left vertical stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentLeftVerticalStrokeWithCross", 0xE595 )); // glyphnumber: 1860, Ornament left vertical stroke with cross (+)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentLowLeftConcaveStroke", 0xE598 )); // glyphnumber: 1861, Ornament low left concave stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentLowLeftConvexStroke", 0xE599 )); // glyphnumber: 1862, Ornament low left convex stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentLowRightConcaveStroke", 0xE5A5 )); // glyphnumber: 1863, Ornament low right concave stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentLowRightConvexStroke", 0xE5A6 )); // glyphnumber: 1864, Ornament low right convex stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentMiddleVerticalStroke", 0xE59F )); // glyphnumber: 1865, Ornament middle vertical stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentMordent", 0xE56C )); // glyphnumber: 1866, Mordent
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentMordentInverted", 0xE56D )); // glyphnumber: 1867, Inverted mordent
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentObliqueLineAfterNote", 0xE57C )); // glyphnumber: 1868, Oblique straight line NW-SE
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentObliqueLineBeforeNote", 0xE57B )); // glyphnumber: 1869, Oblique straight line SW-NE
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentObliqueLineHorizAfterNote", 0xE580 )); // glyphnumber: 1870, Oblique straight line tilted NW-SE
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentObliqueLineHorizBeforeNote", 0xE57F )); // glyphnumber: 1871, Oblique straight line tilted SW-NE
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentOriscus", 0xEA21 )); // glyphnumber: 1872, Oriscus
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPinceCouperin", 0xE588 )); // glyphnumber: 1873, Pincé (Couperin)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPortDeVoixV", 0xE570 )); // glyphnumber: 1874, Port de voix
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompAppoggTrill", 0xE5B2 )); // glyphnumber: 1875, Supported appoggiatura trill
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompAppoggTrillSuffix", 0xE5B3 )); // glyphnumber: 1876, Supported appoggiatura trill with two-note suffix
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompCadence", 0xE5BE )); // glyphnumber: 1877, Cadence
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompCadenceUpperPrefix", 0xE5C1 )); // glyphnumber: 1878, Cadence with upper prefix
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompCadenceUpperPrefixTurn", 0xE5C2 )); // glyphnumber: 1879, Cadence with upper prefix and turn
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompCadenceWithTurn", 0xE5BF )); // glyphnumber: 1880, Cadence with turn
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompDescendingSlide", 0xE5B1 )); // glyphnumber: 1881, Descending slide
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompDoubleCadenceLowerPrefix", 0xE5C0 )); // glyphnumber: 1882, Double cadence with lower prefix
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompDoubleCadenceUpperPrefix", 0xE5C3 )); // glyphnumber: 1883, Double cadence with upper prefix
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompDoubleCadenceUpperPrefixTurn", 0xE5C4 )); // glyphnumber: 1884, Double cadence with upper prefix and turn
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompInvertedMordentUpperPrefix", 0xE5C7 )); // glyphnumber: 1885, Inverted mordent with upper prefix
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompMordentRelease", 0xE5C5 )); // glyphnumber: 1886, Mordent with release
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompMordentUpperPrefix", 0xE5C6 )); // glyphnumber: 1887, Mordent with upper prefix
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompPortDeVoixMordent", 0xE5BC )); // glyphnumber: 1888, Pre-beat port de voix follwed by multiple mordent (Dandrieu)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompSlide", 0xE5B0 )); // glyphnumber: 1889, Slide
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompSlideTrillBach", 0xE5B8 )); // glyphnumber: 1890, Slide-trill with two-note suffix (J.S. Bach)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompSlideTrillDAnglebert", 0xE5B5 )); // glyphnumber: 1891, Slide-trill (D'Anglebert)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompSlideTrillMarpurg", 0xE5B6 )); // glyphnumber: 1892, Slide-trill with one-note suffix (Marpurg)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompSlideTrillMuffat", 0xE5B9 )); // glyphnumber: 1893, Slide-trill (Muffat)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompSlideTrillSuffixMuffat", 0xE5BA )); // glyphnumber: 1894, Slide-trill with two-note suffix (Muffat)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompTrillLowerSuffix", 0xE5C8 )); // glyphnumber: 1895, Trill with lower suffix
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompTrillSuffixDandrieu", 0xE5BB )); // glyphnumber: 1896, Trill with two-note suffix (Dandrieu)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompTrillWithMordent", 0xE5BD )); // glyphnumber: 1897, Trill with mordent
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompTurnTrillBach", 0xE5B7 )); // glyphnumber: 1898, Turn-trill with two-note suffix (J.S. Bach)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentPrecompTurnTrillDAnglebert", 0xE5B4 )); // glyphnumber: 1899, Turn-trill (D'Anglebert)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentQuilisma", 0xEA20 )); // glyphnumber: 1900, Quilisma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentRightFacingHalfCircle", 0xE571 )); // glyphnumber: 1901, Right-facing half circle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentRightFacingHook", 0xE573 )); // glyphnumber: 1902, Right-facing hook
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentRightVerticalStroke", 0xE5A4 )); // glyphnumber: 1903, Ornament right vertical stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentSchleifer", 0xE587 )); // glyphnumber: 1904, Schleifer (long mordent)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentShake3", 0xE582 )); // glyphnumber: 1905, Shake
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentShakeMuffat1", 0xE584 )); // glyphnumber: 1906, Shake (Muffat)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentShortObliqueLineAfterNote", 0xE57A )); // glyphnumber: 1907, Short oblique straight line NW-SE
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentShortObliqueLineBeforeNote", 0xE579 )); // glyphnumber: 1908, Short oblique straight line SW-NE
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentTopLeftConcaveStroke", 0xE590 )); // glyphnumber: 1909, Ornament top left concave stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentTopLeftConvexStroke", 0xE591 )); // glyphnumber: 1910, Ornament top left convex stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentTopRightConcaveStroke", 0xE5A0 )); // glyphnumber: 1911, Ornament top right concave stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentTopRightConvexStroke", 0xE5A1 )); // glyphnumber: 1912, Ornament top right convex stroke
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentTremblement", 0xE56E )); // glyphnumber: 1913, Tremblement
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentTremblementCouperin", 0xE589 )); // glyphnumber: 1914, Tremblement appuyé (Couperin)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentTrill", 0xE566 )); // glyphnumber: 1915, Trill
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentTurn", 0xE567 )); // glyphnumber: 1916, Turn
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentTurnInverted", 0xE568 )); // glyphnumber: 1917, Inverted turn
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentTurnSlash", 0xE569 )); // glyphnumber: 1918, Turn with slash
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentTurnUp", 0xE56A )); // glyphnumber: 1919, Turn up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentTurnUpS", 0xE56B )); // glyphnumber: 1920, Inverted turn up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentUpCurve", 0xE577 )); // glyphnumber: 1921, Curve above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentVerticalLine", 0xE583 )); // glyphnumber: 1922, Vertical line
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentZigZagLineNoRightEnd", 0xE59D )); // glyphnumber: 1923, Ornament zig-zag line without right-hand end
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ornamentZigZagLineWithRightEnd", 0xE59E )); // glyphnumber: 1924, Ornament zig-zag line with right-hand end
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ottava", 0xE510 )); // glyphnumber: 1925, Ottava
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ottavaAlta", 0xE511 )); // glyphnumber: 1926, Ottava alta
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ottavaBassa", 0xE512 )); // glyphnumber: 1927, Ottava bassa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ottavaBassaBa", 0xE513 )); // glyphnumber: 1928, Ottava bassa (ba)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ottavaBassaVb", 0xE51C )); // glyphnumber: 1929, Ottava bassa (8vb)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pendereckiTremolo", 0xE22B )); // glyphnumber: 1930, Penderecki unmeasured tremolo
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictAgogo", 0xE717 )); // glyphnumber: 1931, Agogo
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictAlmglocken", 0xE712 )); // glyphnumber: 1932, Almglocken
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictAnvil", 0xE701 )); // glyphnumber: 1933, Anvil
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBambooChimes", 0xE6C3 )); // glyphnumber: 1934, Bamboo tube chimes
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBambooScraper", 0xE6FB )); // glyphnumber: 1935, Bamboo scraper
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBassDrum", 0xE6D4 )); // glyphnumber: 1936, Bass drum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBassDrumOnSide", 0xE6D5 )); // glyphnumber: 1937, Bass drum on side
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterBow", 0xE7DE )); // glyphnumber: 1938, Bow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterBox", 0xE7EB )); // glyphnumber: 1939, Box for percussion beater
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterBrassMalletsDown", 0xE7DA )); // glyphnumber: 1940, Brass mallets down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterBrassMalletsUp", 0xE7D9 )); // glyphnumber: 1941, Brass mallets up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterCombiningDashedCircle", 0xE7EA )); // glyphnumber: 1942, Combining dashed circle for round beaters (plated)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterCombiningParentheses", 0xE7E9 )); // glyphnumber: 1943, Combining parentheses for round beaters (padded)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterDoubleBassDrumDown", 0xE7A1 )); // glyphnumber: 1944, Double bass drum stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterDoubleBassDrumUp", 0xE7A0 )); // glyphnumber: 1945, Double bass drum stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterFinger", 0xE7E4 )); // glyphnumber: 1946, Finger
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterFingernails", 0xE7E6 )); // glyphnumber: 1947, Fingernails
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterFist", 0xE7E5 )); // glyphnumber: 1948, Fist
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterGuiroScraper", 0xE7DD )); // glyphnumber: 1949, Guiro scraper
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHammer", 0xE7E1 )); // glyphnumber: 1950, Hammer
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHammerMetalDown", 0xE7D0 )); // glyphnumber: 1951, Metal hammer, down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHammerMetalUp", 0xE7CF )); // glyphnumber: 1952, Metal hammer, up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHammerPlasticDown", 0xE7CE )); // glyphnumber: 1953, Plastic hammer, down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHammerPlasticUp", 0xE7CD )); // glyphnumber: 1954, Plastic hammer, up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHammerWoodDown", 0xE7CC )); // glyphnumber: 1955, Wooden hammer, down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHammerWoodUp", 0xE7CB )); // glyphnumber: 1956, Wooden hammer, up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHand", 0xE7E3 )); // glyphnumber: 1957, Hand
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardBassDrumDown", 0xE79D )); // glyphnumber: 1958, Hard bass drum stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardBassDrumUp", 0xE79C )); // glyphnumber: 1959, Hard bass drum stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardGlockenspielDown", 0xE785 )); // glyphnumber: 1960, Hard glockenspiel stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardGlockenspielLeft", 0xE787 )); // glyphnumber: 1961, Hard glockenspiel stick left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardGlockenspielRight", 0xE786 )); // glyphnumber: 1962, Hard glockenspiel stick right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardGlockenspielUp", 0xE784 )); // glyphnumber: 1963, Hard glockenspiel stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardTimpaniDown", 0xE791 )); // glyphnumber: 1964, Hard timpani stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardTimpaniLeft", 0xE793 )); // glyphnumber: 1965, Hard timpani stick left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardTimpaniRight", 0xE792 )); // glyphnumber: 1966, Hard timpani stick right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardTimpaniUp", 0xE790 )); // glyphnumber: 1967, Hard timpani stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardXylophoneDown", 0xE779 )); // glyphnumber: 1968, Hard xylophone stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardXylophoneLeft", 0xE77B )); // glyphnumber: 1969, Hard xylophone stick left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardXylophoneRight", 0xE77A )); // glyphnumber: 1970, Hard xylophone stick right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardXylophoneUp", 0xE778 )); // glyphnumber: 1971, Hard xylophone stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardYarnDown", 0xE7AB )); // glyphnumber: 1972, Hard yarn beater down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardYarnLeft", 0xE7AD )); // glyphnumber: 1973, Hard yarn beater left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardYarnRight", 0xE7AC )); // glyphnumber: 1974, Hard yarn beater right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterHardYarnUp", 0xE7AA )); // glyphnumber: 1975, Hard yarn beater up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterJazzSticksDown", 0xE7D4 )); // glyphnumber: 1976, Jazz sticks down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterJazzSticksUp", 0xE7D3 )); // glyphnumber: 1977, Jazz sticks up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterKnittingNeedle", 0xE7E2 )); // glyphnumber: 1978, Knitting needle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMallet", 0xE7DF )); // glyphnumber: 1979, Chime hammer
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumBassDrumDown", 0xE79B )); // glyphnumber: 1980, Medium bass drum stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumBassDrumUp", 0xE79A )); // glyphnumber: 1981, Medium bass drum stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumTimpaniDown", 0xE78D )); // glyphnumber: 1982, Medium timpani stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumTimpaniLeft", 0xE78F )); // glyphnumber: 1983, Medium timpani stick left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumTimpaniRight", 0xE78E )); // glyphnumber: 1984, Medium timpani stick right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumTimpaniUp", 0xE78C )); // glyphnumber: 1985, Medium timpani stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumXylophoneDown", 0xE775 )); // glyphnumber: 1986, Medium xylophone stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumXylophoneLeft", 0xE777 )); // glyphnumber: 1987, Medium xylophone stick left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumXylophoneRight", 0xE776 )); // glyphnumber: 1988, Medium xylophone stick right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumXylophoneUp", 0xE774 )); // glyphnumber: 1989, Medium xylophone stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumYarnDown", 0xE7A7 )); // glyphnumber: 1990, Medium yarn beater down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumYarnLeft", 0xE7A9 )); // glyphnumber: 1991, Medium yarn beater left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumYarnRight", 0xE7A8 )); // glyphnumber: 1992, Medium yarn beater right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMediumYarnUp", 0xE7A6 )); // glyphnumber: 1993, Medium yarn beater up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMetalBassDrumDown", 0xE79F )); // glyphnumber: 1994, Metal bass drum stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMetalBassDrumUp", 0xE79E )); // glyphnumber: 1995, Metal bass drum stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMetalDown", 0xE7C8 )); // glyphnumber: 1996, Metal beater down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMetalHammer", 0xE7E0 )); // glyphnumber: 1997, Metal hammer
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMetalLeft", 0xE7CA )); // glyphnumber: 1998, Metal beater, left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMetalRight", 0xE7C9 )); // glyphnumber: 1999, Metal beater, right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterMetalUp", 0xE7C7 )); // glyphnumber: 2000, Metal beater, up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSnareSticksDown", 0xE7D2 )); // glyphnumber: 2001, Snare sticks down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSnareSticksUp", 0xE7D1 )); // glyphnumber: 2002, Snare sticks up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftBassDrumDown", 0xE799 )); // glyphnumber: 2003, Soft bass drum stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftBassDrumUp", 0xE798 )); // glyphnumber: 2004, Soft bass drum stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftGlockenspielDown", 0xE781 )); // glyphnumber: 2005, Soft glockenspiel stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftGlockenspielLeft", 0xE783 )); // glyphnumber: 2006, Soft glockenspiel stick left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftGlockenspielRight", 0xE782 )); // glyphnumber: 2007, Soft glockenspiel stick right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftGlockenspielUp", 0xE780 )); // glyphnumber: 2008, Soft glockenspiel stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftTimpaniDown", 0xE789 )); // glyphnumber: 2009, Soft timpani stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftTimpaniLeft", 0xE78B )); // glyphnumber: 2010, Soft timpani stick left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftTimpaniRight", 0xE78A )); // glyphnumber: 2011, Soft timpani stick right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftTimpaniUp", 0xE788 )); // glyphnumber: 2012, Soft timpani stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftXylophone", 0xE7DB )); // glyphnumber: 2013, Soft xylophone beaters
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftXylophoneDown", 0xE771 )); // glyphnumber: 2014, Soft xylophone stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftXylophoneLeft", 0xE773 )); // glyphnumber: 2015, Soft xylophone stick left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftXylophoneRight", 0xE772 )); // glyphnumber: 2016, Soft xylophone stick right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftXylophoneUp", 0xE770 )); // glyphnumber: 2017, Soft xylophone stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftYarnDown", 0xE7A3 )); // glyphnumber: 2018, Soft yarn beater down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftYarnLeft", 0xE7A5 )); // glyphnumber: 2019, Soft yarn beater left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftYarnRight", 0xE7A4 )); // glyphnumber: 2020, Soft yarn beater right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSoftYarnUp", 0xE7A2 )); // glyphnumber: 2021, Soft yarn beater up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSpoonWoodenMallet", 0xE7DC )); // glyphnumber: 2022, Spoon-shaped wooden mallet
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSuperballDown", 0xE7AF )); // glyphnumber: 2023, Superball beater down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSuperballLeft", 0xE7B1 )); // glyphnumber: 2024, Superball beater left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSuperballRight", 0xE7B0 )); // glyphnumber: 2025, Superball beater right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterSuperballUp", 0xE7AE )); // glyphnumber: 2026, Superball beater up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterTriangleDown", 0xE7D6 )); // glyphnumber: 2027, Triangle beater down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterTriangleUp", 0xE7D5 )); // glyphnumber: 2028, Triangle beater up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterWireBrushesDown", 0xE7D8 )); // glyphnumber: 2029, Wire brushes down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterWireBrushesUp", 0xE7D7 )); // glyphnumber: 2030, Wire brushes up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterWoodTimpaniDown", 0xE795 )); // glyphnumber: 2031, Wood timpani stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterWoodTimpaniLeft", 0xE797 )); // glyphnumber: 2032, Wood timpani stick left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterWoodTimpaniRight", 0xE796 )); // glyphnumber: 2033, Wood timpani stick right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterWoodTimpaniUp", 0xE794 )); // glyphnumber: 2034, Wood timpani stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterWoodXylophoneDown", 0xE77D )); // glyphnumber: 2035, Wood xylophone stick down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterWoodXylophoneLeft", 0xE77F )); // glyphnumber: 2036, Wood xylophone stick left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterWoodXylophoneRight", 0xE77E )); // glyphnumber: 2037, Wood xylophone stick right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBeaterWoodXylophoneUp", 0xE77C )); // glyphnumber: 2038, Wood xylophone stick up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBell", 0xE714 )); // glyphnumber: 2039, Bell
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBellOfCymbal", 0xE72A )); // glyphnumber: 2040, Bell of cymbal
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBellPlate", 0xE713 )); // glyphnumber: 2041, Bell plate
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBellTree", 0xE71A )); // glyphnumber: 2042, Bell tree
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBirdWhistle", 0xE751 )); // glyphnumber: 2043, Bird whistle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBoardClapper", 0xE6F7 )); // glyphnumber: 2044, Board clapper
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBongos", 0xE6DD )); // glyphnumber: 2045, Bongos
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictBrakeDrum", 0xE6E1 )); // glyphnumber: 2046, Brake drum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCabasa", 0xE743 )); // glyphnumber: 2047, Cabasa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCannon", 0xE761 )); // glyphnumber: 2048, Cannon
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCarHorn", 0xE755 )); // glyphnumber: 2049, Car horn
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCastanets", 0xE6F8 )); // glyphnumber: 2050, Castanets
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCastanetsWithHandle", 0xE6F9 )); // glyphnumber: 2051, Castanets with handle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCelesta", 0xE6B0 )); // glyphnumber: 2052, Celesta
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCencerro", 0xE716 )); // glyphnumber: 2053, Cencerro
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCenter1", 0xE7FE )); // glyphnumber: 2054, Center (Weinberg)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCenter2", 0xE7FF )); // glyphnumber: 2055, Center (Ghent)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCenter3", 0xE800 )); // glyphnumber: 2056, Center (Caltabiano)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictChainRattle", 0xE748 )); // glyphnumber: 2057, Chain rattle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictChimes", 0xE6C2 )); // glyphnumber: 2058, Chimes
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictChineseCymbal", 0xE726 )); // glyphnumber: 2059, Chinese cymbal
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictChokeCymbal", 0xE805 )); // glyphnumber: 2060, Choke (Weinberg)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictClaves", 0xE6F2 )); // glyphnumber: 2061, Claves
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCoins", 0xE7E7 )); // glyphnumber: 2062, Coins
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictConga", 0xE6DE )); // glyphnumber: 2063, Conga
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCowBell", 0xE711 )); // glyphnumber: 2064, Cow bell
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCrashCymbals", 0xE720 )); // glyphnumber: 2065, Crash cymbals
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCrotales", 0xE6AE )); // glyphnumber: 2066, Crotales
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCrushStem", 0xE80C )); // glyphnumber: 2067, Combining crush for stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCuica", 0xE6E4 )); // glyphnumber: 2068, Cuica
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictCymbalTongs", 0xE728 )); // glyphnumber: 2069, Cymbal tongs
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictDamp1", 0xE7F9 )); // glyphnumber: 2070, Damp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictDamp2", 0xE7FA )); // glyphnumber: 2071, Damp 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictDamp3", 0xE7FB )); // glyphnumber: 2072, Damp 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictDamp4", 0xE7FC )); // glyphnumber: 2073, Damp 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictDeadNoteStem", 0xE80D )); // glyphnumber: 2074, Combining X for stem (dead note)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictDrumStick", 0xE7E8 )); // glyphnumber: 2075, Drum stick
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictDuckCall", 0xE757 )); // glyphnumber: 2076, Duck call
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictEdgeOfCymbal", 0xE729 )); // glyphnumber: 2077, Edge of cymbal
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictEmptyTrap", 0xE6A9 )); // glyphnumber: 2078, Empty trapezoid
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictFingerCymbals", 0xE727 )); // glyphnumber: 2079, Finger cymbals
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictFlexatone", 0xE740 )); // glyphnumber: 2080, Flexatone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictFootballRatchet", 0xE6F5 )); // glyphnumber: 2081, Football rattle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGlassHarmonica", 0xE765 )); // glyphnumber: 2082, Glass harmonica
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGlassHarp", 0xE764 )); // glyphnumber: 2083, Glass harp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGlassPlateChimes", 0xE6C6 )); // glyphnumber: 2084, Glass plate chimes
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGlassTubeChimes", 0xE6C5 )); // glyphnumber: 2085, Glass tube chimes
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGlsp", 0xE6A0 )); // glyphnumber: 2086, Glockenspiel
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGlspSmithBrindle", 0xE6AA )); // glyphnumber: 2087, Glockenspiel (Smith Brindle)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGobletDrum", 0xE6E2 )); // glyphnumber: 2088, Goblet drum (djembe, dumbek)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGong", 0xE732 )); // glyphnumber: 2089, Gong
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGongWithButton", 0xE733 )); // glyphnumber: 2090, Gong with button (nipple)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGuiro", 0xE6F3 )); // glyphnumber: 2091, Guiro
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGumHardDown", 0xE7C4 )); // glyphnumber: 2092, Hard gum beater, down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGumHardLeft", 0xE7C6 )); // glyphnumber: 2093, Hard gum beater, left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGumHardRight", 0xE7C5 )); // glyphnumber: 2094, Hard gum beater, right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGumHardUp", 0xE7C3 )); // glyphnumber: 2095, Hard gum beater, up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGumMediumDown", 0xE7C0 )); // glyphnumber: 2096, Medium gum beater, down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGumMediumLeft", 0xE7C2 )); // glyphnumber: 2097, Medium gum beater, left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGumMediumRight", 0xE7C1 )); // glyphnumber: 2098, Medium gum beater, right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGumMediumUp", 0xE7BF )); // glyphnumber: 2099, Medium gum beater, up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGumSoftDown", 0xE7BC )); // glyphnumber: 2100, Soft gum beater, down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGumSoftLeft", 0xE7BE )); // glyphnumber: 2101, Soft gum beater, left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGumSoftRight", 0xE7BD )); // glyphnumber: 2102, Soft gum beater, right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictGumSoftUp", 0xE7BB )); // glyphnumber: 2103, Soft gum beater, up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictHalfOpen1", 0xE7F6 )); // glyphnumber: 2104, Half-open
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictHalfOpen2", 0xE7F7 )); // glyphnumber: 2105, Half-open 2 (Weinberg)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictHandbell", 0xE715 )); // glyphnumber: 2106, Handbell
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictHiHat", 0xE722 )); // glyphnumber: 2107, Hi-hat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictHiHatOnStand", 0xE723 )); // glyphnumber: 2108, Hi-hat cymbals on stand
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictJawHarp", 0xE767 )); // glyphnumber: 2109, Jaw harp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictJingleBells", 0xE719 )); // glyphnumber: 2110, Jingle bells
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictKlaxonHorn", 0xE756 )); // glyphnumber: 2111, Klaxon horn
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictLeftHandCircle", 0xE807 )); // glyphnumber: 2112, Right hand (Agostini)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictLionsRoar", 0xE763 )); // glyphnumber: 2113, Lion's roar
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictLithophone", 0xE6B1 )); // glyphnumber: 2114, Lithophone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictLogDrum", 0xE6DF )); // glyphnumber: 2115, Log drum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictLotusFlute", 0xE75A )); // glyphnumber: 2116, Lotus flute
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictMar", 0xE6A6 )); // glyphnumber: 2117, Marimba
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictMarSmithBrindle", 0xE6AC )); // glyphnumber: 2118, Marimba (Smith Brindle)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictMaraca", 0xE741 )); // glyphnumber: 2119, Maraca
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictMaracas", 0xE742 )); // glyphnumber: 2120, Maracas
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictMegaphone", 0xE759 )); // glyphnumber: 2121, Megaphone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictMetalPlateChimes", 0xE6C8 )); // glyphnumber: 2122, Metal plate chimes
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictMetalTubeChimes", 0xE6C7 )); // glyphnumber: 2123, Metal tube chimes
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictMusicalSaw", 0xE766 )); // glyphnumber: 2124, Musical saw
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictNormalPosition", 0xE804 )); // glyphnumber: 2125, Normal position (Caltabiano)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictOnRim", 0xE7F4 )); // glyphnumber: 2126, On rim
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictOpen", 0xE7F8 )); // glyphnumber: 2127, Open
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictOpenRimShot", 0xE7F5 )); // glyphnumber: 2128, Closed / rim shot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictPistolShot", 0xE760 )); // glyphnumber: 2129, Pistol shot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictPoliceWhistle", 0xE752 )); // glyphnumber: 2130, Police whistle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictQuijada", 0xE6FA )); // glyphnumber: 2131, Quijada (jawbone)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictRainstick", 0xE747 )); // glyphnumber: 2132, Rainstick
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictRatchet", 0xE6F4 )); // glyphnumber: 2133, Ratchet
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictRecoReco", 0xE6FC )); // glyphnumber: 2134, Reco-reco
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictRightHandSquare", 0xE806 )); // glyphnumber: 2135, Left hand (Agostini)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictRim1", 0xE801 )); // glyphnumber: 2136, Rim or edge (Weinberg)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictRim2", 0xE802 )); // glyphnumber: 2137, Rim (Ghent)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictRim3", 0xE803 )); // glyphnumber: 2138, Rim (Caltabiano)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictRimShotOnStem", 0xE7FD )); // glyphnumber: 2139, Rim shot for stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSandpaperBlocks", 0xE762 )); // glyphnumber: 2140, Sandpaper blocks
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictScrapeAroundRim", 0xE7F3 )); // glyphnumber: 2141, Scrape around rim (counter-clockwise)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictScrapeAroundRimClockwise", 0xE80E )); // glyphnumber: 2142, Scrape around rim (clockwise)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictScrapeCenterToEdge", 0xE7F1 )); // glyphnumber: 2143, Scrape from center to edge
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictScrapeEdgeToCenter", 0xE7F2 )); // glyphnumber: 2144, Scrape from edge to center
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictShellBells", 0xE718 )); // glyphnumber: 2145, Shell bells
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictShellChimes", 0xE6C4 )); // glyphnumber: 2146, Shell chimes
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSiren", 0xE753 )); // glyphnumber: 2147, Siren
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSistrum", 0xE746 )); // glyphnumber: 2148, Sistrum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSizzleCymbal", 0xE724 )); // glyphnumber: 2149, Sizzle cymbal
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSleighBell", 0xE710 )); // glyphnumber: 2150, Sleigh bell
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSlideBrushOnGong", 0xE734 )); // glyphnumber: 2151, Slide brush on gong
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSlideWhistle", 0xE750 )); // glyphnumber: 2152, Slide whistle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSlitDrum", 0xE6E0 )); // glyphnumber: 2153, Slit drum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSnareDrum", 0xE6D1 )); // glyphnumber: 2154, Snare drum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSnareDrumMilitary", 0xE6D3 )); // glyphnumber: 2155, Military snare drum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSnareDrumSnaresOff", 0xE6D2 )); // glyphnumber: 2156, Snare drum, snares off
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSteelDrums", 0xE6AF )); // glyphnumber: 2157, Steel drums
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictStickShot", 0xE7F0 )); // glyphnumber: 2158, Stick shot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSuperball", 0xE7B2 )); // glyphnumber: 2159, Superball
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSuspendedCymbal", 0xE721 )); // glyphnumber: 2160, Suspended cymbal
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictSwishStem", 0xE808 )); // glyphnumber: 2161, Combining swish for stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTabla", 0xE6E3 )); // glyphnumber: 2162, Indian tabla
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTamTam", 0xE730 )); // glyphnumber: 2163, Tam-tam
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTamTamWithBeater", 0xE731 )); // glyphnumber: 2164, Tam-tam with beater (Smith Brindle)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTambourine", 0xE6DB )); // glyphnumber: 2165, Tambourine
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTempleBlocks", 0xE6F1 )); // glyphnumber: 2166, Temple blocks
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTenorDrum", 0xE6D6 )); // glyphnumber: 2167, Tenor drum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictThundersheet", 0xE744 )); // glyphnumber: 2168, Thundersheet
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTimbales", 0xE6DC )); // glyphnumber: 2169, Timbales
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTimpani", 0xE6D0 )); // glyphnumber: 2170, Timpani
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTomTom", 0xE6D7 )); // glyphnumber: 2171, Tom-tom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTomTomChinese", 0xE6D8 )); // glyphnumber: 2172, Chinese tom-tom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTomTomIndoAmerican", 0xE6DA )); // glyphnumber: 2173, Indo-American tom tom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTomTomJapanese", 0xE6D9 )); // glyphnumber: 2174, Japanese tom-tom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTriangle", 0xE700 )); // glyphnumber: 2175, Triangle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTubaphone", 0xE6B2 )); // glyphnumber: 2176, Tubaphone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTubularBells", 0xE6C0 )); // glyphnumber: 2177, Tubular bells
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTurnLeftStem", 0xE80A )); // glyphnumber: 2178, Combining turn left for stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTurnRightLeftStem", 0xE80B )); // glyphnumber: 2179, Combining turn left or right for stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictTurnRightStem", 0xE809 )); // glyphnumber: 2180, Combining turn right for stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictVib", 0xE6A7 )); // glyphnumber: 2181, Vibraphone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictVibMotorOff", 0xE6A8 )); // glyphnumber: 2182, Metallophone (vibraphone motor off)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictVibSmithBrindle", 0xE6AD )); // glyphnumber: 2183, Vibraphone (Smith Brindle)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictVibraslap", 0xE745 )); // glyphnumber: 2184, Vibraslap
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictVietnameseHat", 0xE725 )); // glyphnumber: 2185, Vietnamese hat cymbal
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictWhip", 0xE6F6 )); // glyphnumber: 2186, Whip
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictWindChimesGlass", 0xE6C1 )); // glyphnumber: 2187, Wind chimes (glass)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictWindMachine", 0xE754 )); // glyphnumber: 2188, Wind machine
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictWindWhistle", 0xE758 )); // glyphnumber: 2189, Wind whistle (or mouth siren)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictWoodBlock", 0xE6F0 )); // glyphnumber: 2190, Wood block
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictWoundHardDown", 0xE7B4 )); // glyphnumber: 2191, Wound beater, hard core down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictWoundHardLeft", 0xE7B6 )); // glyphnumber: 2192, Wound beater, hard core left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictWoundHardRight", 0xE7B5 )); // glyphnumber: 2193, Wound beater, hard core right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictWoundHardUp", 0xE7B3 )); // glyphnumber: 2194, Wound beater, hard core up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictWoundSoftDown", 0xE7B8 )); // glyphnumber: 2195, Wound beater, soft core down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictWoundSoftLeft", 0xE7BA )); // glyphnumber: 2196, Wound beater, soft core left
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictWoundSoftRight", 0xE7B9 )); // glyphnumber: 2197, Wound beater, soft core right
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictWoundSoftUp", 0xE7B7 )); // glyphnumber: 2198, Wound beater, soft core up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictXyl", 0xE6A1 )); // glyphnumber: 2199, Xylophone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictXylBass", 0xE6A3 )); // glyphnumber: 2200, Bass xylophone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictXylSmithBrindle", 0xE6AB )); // glyphnumber: 2201, Xylophone (Smith Brindle)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictXylTenor", 0xE6A2 )); // glyphnumber: 2202, Tenor xylophone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictXylTenorTrough", 0xE6A5 )); // glyphnumber: 2203, Trough tenor xylophone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pictXylTrough", 0xE6A4 )); // glyphnumber: 2204, Trough xylophone
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pluckedBuzzPizzicato", 0xE632 )); // glyphnumber: 2205, Buzz pizzicato
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pluckedDamp", 0xE638 )); // glyphnumber: 2206, Damp
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pluckedDampAll", 0xE639 )); // glyphnumber: 2207, Damp all
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pluckedDampOnStem", 0xE63B )); // glyphnumber: 2208, Damp for stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pluckedFingernailFlick", 0xE637 )); // glyphnumber: 2209, Fingernail flick
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pluckedLeftHandPizzicato", 0xE633 )); // glyphnumber: 2210, Left-hand pizzicato
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pluckedPlectrum", 0xE63A )); // glyphnumber: 2211, Plectrum
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pluckedSnapPizzicatoAbove", 0xE631 )); // glyphnumber: 2212, Snap pizzicato above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pluckedSnapPizzicatoBelow", 0xE630 )); // glyphnumber: 2213, Snap pizzicato below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "pluckedWithFingernails", 0xE636 )); // glyphnumber: 2214, With fingernails
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "quindicesima", 0xE514 )); // glyphnumber: 2215, Quindicesima
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "quindicesimaAlta", 0xE515 )); // glyphnumber: 2216, Quindicesima alta
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "quindicesimaBassa", 0xE516 )); // glyphnumber: 2217, Quindicesima bassa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "quindicesimaBassaMb", 0xE51D )); // glyphnumber: 2218, Quindicesima bassa (mb)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "repeat1Bar", 0xE500 )); // glyphnumber: 2219, Repeat last bar
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "repeat2Bars", 0xE501 )); // glyphnumber: 2220, Repeat last two bars
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "repeat4Bars", 0xE502 )); // glyphnumber: 2221, Repeat last four bars
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "repeatDot", 0xE044 )); // glyphnumber: 2222, Single repeat dot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "repeatDots", 0xE043 )); // glyphnumber: 2223, Repeat dots
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "repeatLeft", 0xE040 )); // glyphnumber: 2224, Left (start) repeat sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "repeatRight", 0xE041 )); // glyphnumber: 2225, Right (end) repeat sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "repeatRightLeft", 0xE042 )); // glyphnumber: 2226, Right and left repeat sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "rest1024th", 0xE4ED )); // glyphnumber: 2227, 1024th rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "rest128th", 0xE4EA )); // glyphnumber: 2228, 128th (semihemidemisemiquaver) rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "rest16th", 0xE4E7 )); // glyphnumber: 2229, 16th (semiquaver) rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "rest256th", 0xE4EB )); // glyphnumber: 2230, 256th rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "rest32nd", 0xE4E8 )); // glyphnumber: 2231, 32nd (demisemiquaver) rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "rest512th", 0xE4EC )); // glyphnumber: 2232, 512th rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "rest64th", 0xE4E9 )); // glyphnumber: 2233, 64th (hemidemisemiquaver) rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "rest8th", 0xE4E6 )); // glyphnumber: 2234, Eighth (quaver) rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restDoubleWhole", 0xE4E2 )); // glyphnumber: 2235, Double whole (breve) rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restDoubleWholeLegerLine", 0xE4F3 )); // glyphnumber: 2236, Double whole rest on leger lines
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restHBar", 0xE4EE )); // glyphnumber: 2237, Multiple measure rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restHBarLeft", 0xE4EF )); // glyphnumber: 2238, H-bar, left half
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restHBarMiddle", 0xE4F0 )); // glyphnumber: 2239, H-bar, middle
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restHBarRight", 0xE4F1 )); // glyphnumber: 2240, H-bar, right half
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restHalf", 0xE4E4 )); // glyphnumber: 2241, Half (minim) rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restHalfLegerLine", 0xE4F5 )); // glyphnumber: 2242, Half rest on leger line
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restLonga", 0xE4E1 )); // glyphnumber: 2243, Longa rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restMaxima", 0xE4E0 )); // glyphnumber: 2244, Maxima rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restQuarter", 0xE4E5 )); // glyphnumber: 2245, Quarter (crotchet) rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restQuarterOld", 0xE4F2 )); // glyphnumber: 2246, Old-style quarter (crotchet) rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restQuarterZ", 0xE4F6 )); // glyphnumber: 2247, Z-style quarter (crotchet) rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restWhole", 0xE4E3 )); // glyphnumber: 2248, Whole (semibreve) rest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "restWholeLegerLine", 0xE4F4 )); // glyphnumber: 2249, Whole rest on leger line
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "reversedBrace", 0xE001 )); // glyphnumber: 2250, Reversed brace
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "reversedBracketBottom", 0xE006 )); // glyphnumber: 2251, Reversed bracket bottom
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "reversedBracketTop", 0xE005 )); // glyphnumber: 2252, Reversed bracket top
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "rightRepeatSmall", 0xE04D )); // glyphnumber: 2253, Right repeat sign within bar
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "schaefferClef", 0xE06F )); // glyphnumber: 2254, Schäffer clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "schaefferFClefToGClef", 0xE072 )); // glyphnumber: 2255, Schäffer F clef to G clef change
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "schaefferGClefToFClef", 0xE071 )); // glyphnumber: 2256, Schäffer G clef to F clef change
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "schaefferPreviousClef", 0xE070 )); // glyphnumber: 2257, Schäffer previous clef
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "segno", 0xE047 )); // glyphnumber: 2258, Segno
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "segnoSerpent1", 0xE04A )); // glyphnumber: 2259, Segno (serpent)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "segnoSerpent2", 0xE04B )); // glyphnumber: 2260, Segno (serpent with vertical lines)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "semipitchedPercussionClef1", 0xE06B )); // glyphnumber: 2261, Semi-pitched percussion clef 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "semipitchedPercussionClef2", 0xE06C )); // glyphnumber: 2262, Semi-pitched percussion clef 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "smnFlat", 0xEC52 )); // glyphnumber: 2263, Flat
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "smnFlatWhite", 0xEC53 )); // glyphnumber: 2264, Flat (white)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "smnHistoryDoubleFlat", 0xEC57 )); // glyphnumber: 2265, Double flat history sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "smnHistoryDoubleSharp", 0xEC55 )); // glyphnumber: 2266, Double sharp history sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "smnHistoryFlat", 0xEC56 )); // glyphnumber: 2267, Flat history sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "smnHistorySharp", 0xEC54 )); // glyphnumber: 2268, Sharp history sign
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "smnNatural", 0xEC58 )); // glyphnumber: 2269, Natural (N)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "smnSharp", 0xEC50 )); // glyphnumber: 2270, Sharp stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "smnSharpDown", 0xEC59 )); // glyphnumber: 2271, Sharp stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "smnSharpWhite", 0xEC51 )); // glyphnumber: 2272, Sharp (white) stem up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "smnSharpWhiteDown", 0xEC5A )); // glyphnumber: 2273, Sharp (white) stem down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "splitBarDivider", 0xE00A )); // glyphnumber: 2274, Split bar divider (bar spans a system break)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff1Line", 0xE010 )); // glyphnumber: 2275, 1-line staff
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff1LineNarrow", 0xE01C )); // glyphnumber: 2276, 1-line staff (narrow)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff1LineWide", 0xE016 )); // glyphnumber: 2277, 1-line staff (wide)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff2Lines", 0xE011 )); // glyphnumber: 2278, 2-line staff
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff2LinesNarrow", 0xE01D )); // glyphnumber: 2279, 2-line staff (narrow)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff2LinesWide", 0xE017 )); // glyphnumber: 2280, 2-line staff (wide)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff3Lines", 0xE012 )); // glyphnumber: 2281, 3-line staff
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff3LinesNarrow", 0xE01E )); // glyphnumber: 2282, 3-line staff (narrow)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff3LinesWide", 0xE018 )); // glyphnumber: 2283, 3-line staff (wide)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff4Lines", 0xE013 )); // glyphnumber: 2284, 4-line staff
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff4LinesNarrow", 0xE01F )); // glyphnumber: 2285, 4-line staff (narrow)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff4LinesWide", 0xE019 )); // glyphnumber: 2286, 4-line staff (wide)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff5Lines", 0xE014 )); // glyphnumber: 2287, 5-line staff
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff5LinesNarrow", 0xE020 )); // glyphnumber: 2288, 5-line staff (narrow)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff5LinesWide", 0xE01A )); // glyphnumber: 2289, 5-line staff (wide)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff6Lines", 0xE015 )); // glyphnumber: 2290, 6-line staff
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff6LinesNarrow", 0xE021 )); // glyphnumber: 2291, 6-line staff (narrow)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staff6LinesWide", 0xE01B )); // glyphnumber: 2292, 6-line staff (wide)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffDivideArrowDown", 0xE00B )); // glyphnumber: 2293, Staff divide arrow down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffDivideArrowUp", 0xE00C )); // glyphnumber: 2294, Staff divide arrow up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffDivideArrowUpDown", 0xE00D )); // glyphnumber: 2295, Staff divide arrows
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosLower1", 0xEB98 )); // glyphnumber: 2296, Lower 1 staff position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosLower2", 0xEB99 )); // glyphnumber: 2297, Lower 2 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosLower3", 0xEB9A )); // glyphnumber: 2298, Lower 3 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosLower4", 0xEB9B )); // glyphnumber: 2299, Lower 4 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosLower5", 0xEB9C )); // glyphnumber: 2300, Lower 5 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosLower6", 0xEB9D )); // glyphnumber: 2301, Lower 6 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosLower7", 0xEB9E )); // glyphnumber: 2302, Lower 7 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosLower8", 0xEB9F )); // glyphnumber: 2303, Lower 8 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosRaise1", 0xEB90 )); // glyphnumber: 2304, Raise 1 staff position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosRaise2", 0xEB91 )); // glyphnumber: 2305, Raise 2 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosRaise3", 0xEB92 )); // glyphnumber: 2306, Raise 3 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosRaise4", 0xEB93 )); // glyphnumber: 2307, Raise 4 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosRaise5", 0xEB94 )); // glyphnumber: 2308, Raise 5 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosRaise6", 0xEB95 )); // glyphnumber: 2309, Raise 6 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosRaise7", 0xEB96 )); // glyphnumber: 2310, Raise 7 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "staffPosRaise8", 0xEB97 )); // glyphnumber: 2311, Raise 8 staff positions
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stem", 0xE210 )); // glyphnumber: 2312, Combining stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemBowOnBridge", 0xE215 )); // glyphnumber: 2313, Combining bow on bridge stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemBowOnTailpiece", 0xE216 )); // glyphnumber: 2314, Combining bow on tailpiece stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemBuzzRoll", 0xE217 )); // glyphnumber: 2315, Combining buzz roll stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemDamp", 0xE218 )); // glyphnumber: 2316, Combining damp stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemHarpStringNoise", 0xE21F )); // glyphnumber: 2317, Combining harp string noise stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemMultiphonicsBlack", 0xE21A )); // glyphnumber: 2318, Combining multiphonics (black) stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemMultiphonicsBlackWhite", 0xE21C )); // glyphnumber: 2319, Combining multiphonics (black and white) stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemMultiphonicsWhite", 0xE21B )); // glyphnumber: 2320, Combining multiphonics (white) stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemPendereckiTremolo", 0xE213 )); // glyphnumber: 2321, Combining Penderecki unmeasured tremolo stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemRimShot", 0xE21E )); // glyphnumber: 2322, Combining rim shot stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemSprechgesang", 0xE211 )); // glyphnumber: 2323, Combining sprechgesang stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemSulPonticello", 0xE214 )); // glyphnumber: 2324, Combining sul ponticello (bow behind bridge) stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemSussurando", 0xE21D )); // glyphnumber: 2325, Combining sussurando stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemSwished", 0xE212 )); // glyphnumber: 2326, Combining swished stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stemVibratoPulse", 0xE219 )); // glyphnumber: 2327, Combining vibrato pulse accent (Saunders) stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stockhausenTremolo", 0xE232 )); // glyphnumber: 2328, Stockhausen irregular tremolo ("Morsen", like Morse code)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsBowBehindBridge", 0xE618 )); // glyphnumber: 2329, Bow behind bridge (sul ponticello)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsBowBehindBridgeFourStrings", 0xE62A )); // glyphnumber: 2330, Bow behind bridge on four strings
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsBowBehindBridgeOneString", 0xE627 )); // glyphnumber: 2331, Bow behind bridge on one string
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsBowBehindBridgeThreeStrings", 0xE629 )); // glyphnumber: 2332, Bow behind bridge on three strings
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsBowBehindBridgeTwoStrings", 0xE628 )); // glyphnumber: 2333, Bow behind bridge on two strings
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsBowOnBridge", 0xE619 )); // glyphnumber: 2334, Bow on top of bridge
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsBowOnTailpiece", 0xE61A )); // glyphnumber: 2335, Bow on tailpiece
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsChangeBowDirection", 0xE626 )); // glyphnumber: 2336, Change bow direction, indeterminate
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsDownBow", 0xE610 )); // glyphnumber: 2337, Down bow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsDownBowTurned", 0xE611 )); // glyphnumber: 2338, Turned down bow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsFouette", 0xE622 )); // glyphnumber: 2339, Fouetté
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsHalfHarmonic", 0xE615 )); // glyphnumber: 2340, Half-harmonic
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsHarmonic", 0xE614 )); // glyphnumber: 2341, Harmonic
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsJeteAbove", 0xE620 )); // glyphnumber: 2342, Jeté (gettato) above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsJeteBelow", 0xE621 )); // glyphnumber: 2343, Jeté (gettato) below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsMuteOff", 0xE617 )); // glyphnumber: 2344, Mute off
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsMuteOn", 0xE616 )); // glyphnumber: 2345, Mute on
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsOverpressureDownBow", 0xE61B )); // glyphnumber: 2346, Overpressure, down bow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsOverpressureNoDirection", 0xE61F )); // glyphnumber: 2347, Overpressure, no bow direction
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsOverpressurePossibileDownBow", 0xE61D )); // glyphnumber: 2348, Overpressure possibile, down bow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsOverpressurePossibileUpBow", 0xE61E )); // glyphnumber: 2349, Overpressure possibile, up bow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsOverpressureUpBow", 0xE61C )); // glyphnumber: 2350, Overpressure, up bow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsThumbPosition", 0xE624 )); // glyphnumber: 2351, Thumb position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsThumbPositionTurned", 0xE625 )); // glyphnumber: 2352, Turned thumb position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsUpBow", 0xE612 )); // glyphnumber: 2353, Up bow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsUpBowTurned", 0xE613 )); // glyphnumber: 2354, Turned up bow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "stringsVibratoPulse", 0xE623 )); // glyphnumber: 2355, Vibrato pulse accent (Saunders) for stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "systemDivider", 0xE007 )); // glyphnumber: 2356, System divider
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "systemDividerExtraLong", 0xE009 )); // glyphnumber: 2357, Extra long system divider
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "systemDividerLong", 0xE008 )); // glyphnumber: 2358, Long system divider
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textAugmentationDot", 0xE1FC )); // glyphnumber: 2359, Augmentation dot
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textBlackNoteFrac16thLongStem", 0xE1F5 )); // glyphnumber: 2360, Black note, fractional 16th beam, long stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textBlackNoteFrac16thShortStem", 0xE1F4 )); // glyphnumber: 2361, Black note, fractional 16th beam, short stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textBlackNoteFrac32ndLongStem", 0xE1F6 )); // glyphnumber: 2362, Black note, fractional 32nd beam, long stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textBlackNoteFrac8thLongStem", 0xE1F3 )); // glyphnumber: 2363, Black note, fractional 8th beam, long stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textBlackNoteFrac8thShortStem", 0xE1F2 )); // glyphnumber: 2364, Black note, fractional 8th beam, short stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textBlackNoteLongStem", 0xE1F1 )); // glyphnumber: 2365, Black note, long stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textBlackNoteShortStem", 0xE1F0 )); // glyphnumber: 2366, Black note, short stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textCont16thBeamLongStem", 0xE1FA )); // glyphnumber: 2367, Continuing 16th beam for long stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textCont16thBeamShortStem", 0xE1F9 )); // glyphnumber: 2368, Continuing 16th beam for short stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textCont32ndBeamLongStem", 0xE1FB )); // glyphnumber: 2369, Continuing 32nd beam for long stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textCont8thBeamLongStem", 0xE1F8 )); // glyphnumber: 2370, Continuing 8th beam for long stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textCont8thBeamShortStem", 0xE1F7 )); // glyphnumber: 2371, Continuing 8th beam for short stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textTie", 0xE1FD )); // glyphnumber: 2372, Tie
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textTuplet3LongStem", 0xE202 )); // glyphnumber: 2373, Tuplet number 3 for long stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textTuplet3ShortStem", 0xE1FF )); // glyphnumber: 2374, Tuplet number 3 for short stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textTupletBracketEndLongStem", 0xE203 )); // glyphnumber: 2375, Tuplet bracket end for long stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textTupletBracketEndShortStem", 0xE200 )); // glyphnumber: 2376, Tuplet bracket end for short stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textTupletBracketStartLongStem", 0xE201 )); // glyphnumber: 2377, Tuplet bracket start for long stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "textTupletBracketStartShortStem", 0xE1FE )); // glyphnumber: 2378, Tuplet bracket start for short stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig0", 0xE080 )); // glyphnumber: 2379, Time signature 0
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig0Reversed", 0xECF0 )); // glyphnumber: 2380, Reversed time signature 0
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig0Turned", 0xECE0 )); // glyphnumber: 2381, Turned time signature 0
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig1", 0xE081 )); // glyphnumber: 2382, Time signature 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig1Reversed", 0xECF1 )); // glyphnumber: 2383, Reversed time signature 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig1Turned", 0xECE1 )); // glyphnumber: 2384, Turned time signature 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig2", 0xE082 )); // glyphnumber: 2385, Time signature 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig2Reversed", 0xECF2 )); // glyphnumber: 2386, Reversed time signature 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig2Turned", 0xECE2 )); // glyphnumber: 2387, Turned time signature 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig3", 0xE083 )); // glyphnumber: 2388, Time signature 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig3Reversed", 0xECF3 )); // glyphnumber: 2389, Reversed time signature 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig3Turned", 0xECE3 )); // glyphnumber: 2390, Turned time signature 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig4", 0xE084 )); // glyphnumber: 2391, Time signature 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig4Reversed", 0xECF4 )); // glyphnumber: 2392, Reversed time signature 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig4Turned", 0xECE4 )); // glyphnumber: 2393, Turned time signature 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig5", 0xE085 )); // glyphnumber: 2394, Time signature 5
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig5Reversed", 0xECF5 )); // glyphnumber: 2395, Reversed time signature 5
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig5Turned", 0xECE5 )); // glyphnumber: 2396, Turned time signature 5
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig6", 0xE086 )); // glyphnumber: 2397, Time signature 6
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig6Reversed", 0xECF6 )); // glyphnumber: 2398, Reversed time signature 6
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig6Turned", 0xECE6 )); // glyphnumber: 2399, Turned time signature 6
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig7", 0xE087 )); // glyphnumber: 2400, Time signature 7
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig7Reversed", 0xECF7 )); // glyphnumber: 2401, Reversed time signature 7
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig7Turned", 0xECE7 )); // glyphnumber: 2402, Turned time signature 7
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig8", 0xE088 )); // glyphnumber: 2403, Time signature 8
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig8Reversed", 0xECF8 )); // glyphnumber: 2404, Reversed time signature 8
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig8Turned", 0xECE8 )); // glyphnumber: 2405, Turned time signature 8
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig9", 0xE089 )); // glyphnumber: 2406, Time signature 9
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig9Reversed", 0xECF9 )); // glyphnumber: 2407, Reversed time signature 9
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSig9Turned", 0xECE9 )); // glyphnumber: 2408, Turned time signature 9
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigBracketLeft", 0xEC80 )); // glyphnumber: 2409, Left bracket for whole time signature
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigBracketLeftSmall", 0xEC82 )); // glyphnumber: 2410, Left bracket for numerator only
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigBracketRight", 0xEC81 )); // glyphnumber: 2411, Right bracket for whole time signature
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigBracketRightSmall", 0xEC83 )); // glyphnumber: 2412, Right bracket for numerator only
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigCombDenominator", 0xE09F )); // glyphnumber: 2413, Control character for denominator digit
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigCombNumerator", 0xE09E )); // glyphnumber: 2414, Control character for numerator digit
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigComma", 0xE096 )); // glyphnumber: 2415, Time signature comma
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigCommon", 0xE08A )); // glyphnumber: 2416, Common time
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigCommonReversed", 0xECFA )); // glyphnumber: 2417, Reversed common time
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigCommonTurned", 0xECEA )); // glyphnumber: 2418, Turned common time
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigCut2", 0xEC85 )); // glyphnumber: 2419, Cut time (Bach)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigCut3", 0xEC86 )); // glyphnumber: 2420, Cut triple time (9/8)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigCutCommon", 0xE08B )); // glyphnumber: 2421, Cut time
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigCutCommonReversed", 0xECFB )); // glyphnumber: 2422, Reversed cut time
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigCutCommonTurned", 0xECEB )); // glyphnumber: 2423, Turned cut time
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigEquals", 0xE08F )); // glyphnumber: 2424, Time signature equals
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigFractionHalf", 0xE098 )); // glyphnumber: 2425, Time signature fraction ½
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigFractionOneThird", 0xE09A )); // glyphnumber: 2426, Time signature fraction ⅓
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigFractionQuarter", 0xE097 )); // glyphnumber: 2427, Time signature fraction ¼
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigFractionThreeQuarters", 0xE099 )); // glyphnumber: 2428, Time signature fraction ¾
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigFractionTwoThirds", 0xE09B )); // glyphnumber: 2429, Time signature fraction ⅔
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigFractionalSlash", 0xE08E )); // glyphnumber: 2430, Time signature fraction slash
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigMinus", 0xE090 )); // glyphnumber: 2431, Time signature minus
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigMultiply", 0xE091 )); // glyphnumber: 2432, Time signature multiply
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigOpenPenderecki", 0xE09D )); // glyphnumber: 2433, Open time signature (Penderecki)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigParensLeft", 0xE094 )); // glyphnumber: 2434, Left parenthesis for whole time signature
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigParensLeftSmall", 0xE092 )); // glyphnumber: 2435, Left parenthesis for numerator only
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigParensRight", 0xE095 )); // glyphnumber: 2436, Right parenthesis for whole time signature
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigParensRightSmall", 0xE093 )); // glyphnumber: 2437, Right parenthesis for numerator only
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigPlus", 0xE08C )); // glyphnumber: 2438, Time signature +
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigPlusSmall", 0xE08D )); // glyphnumber: 2439, Time signature + (for numerators)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigSlash", 0xEC84 )); // glyphnumber: 2440, Time signature slash separator
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "timeSigX", 0xE09C )); // glyphnumber: 2441, Open time signature
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremolo1", 0xE220 )); // glyphnumber: 2442, Combining tremolo 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremolo2", 0xE221 )); // glyphnumber: 2443, Combining tremolo 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremolo3", 0xE222 )); // glyphnumber: 2444, Combining tremolo 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremolo4", 0xE223 )); // glyphnumber: 2445, Combining tremolo 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremolo5", 0xE224 )); // glyphnumber: 2446, Combining tremolo 5
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremoloDivisiDots2", 0xE22E )); // glyphnumber: 2447, Divide measured tremolo by 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremoloDivisiDots3", 0xE22F )); // glyphnumber: 2448, Divide measured tremolo by 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremoloDivisiDots4", 0xE230 )); // glyphnumber: 2449, Divide measured tremolo by 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremoloDivisiDots6", 0xE231 )); // glyphnumber: 2450, Divide measured tremolo by 6
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremoloFingered1", 0xE225 )); // glyphnumber: 2451, Fingered tremolo 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremoloFingered2", 0xE226 )); // glyphnumber: 2452, Fingered tremolo 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremoloFingered3", 0xE227 )); // glyphnumber: 2453, Fingered tremolo 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremoloFingered4", 0xE228 )); // glyphnumber: 2454, Fingered tremolo 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tremoloFingered5", 0xE229 )); // glyphnumber: 2455, Fingered tremolo 5
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tripleTongueAbove", 0xE5F2 )); // glyphnumber: 2456, Triple-tongue above
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tripleTongueBelow", 0xE5F3 )); // glyphnumber: 2457, Triple-tongue below
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tuplet0", 0xE880 )); // glyphnumber: 2458, Tuplet 0
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tuplet1", 0xE881 )); // glyphnumber: 2459, Tuplet 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tuplet2", 0xE882 )); // glyphnumber: 2460, Tuplet 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tuplet3", 0xE883 )); // glyphnumber: 2461, Tuplet 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tuplet4", 0xE884 )); // glyphnumber: 2462, Tuplet 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tuplet5", 0xE885 )); // glyphnumber: 2463, Tuplet 5
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tuplet6", 0xE886 )); // glyphnumber: 2464, Tuplet 6
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tuplet7", 0xE887 )); // glyphnumber: 2465, Tuplet 7
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tuplet8", 0xE888 )); // glyphnumber: 2466, Tuplet 8
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tuplet9", 0xE889 )); // glyphnumber: 2467, Tuplet 9
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "tupletColon", 0xE88A )); // glyphnumber: 2468, Tuplet colon
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "unmeasuredTremolo", 0xE22C )); // glyphnumber: 2469, Wieniawski unmeasured tremolo
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "unmeasuredTremoloSimple", 0xE22D )); // glyphnumber: 2470, Wieniawski unmeasured tremolo (simpler)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "unpitchedPercussionClef1", 0xE069 )); // glyphnumber: 2471, Unpitched percussion clef 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "unpitchedPercussionClef2", 0xE06A )); // glyphnumber: 2472, Unpitched percussion clef 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ventiduesima", 0xE517 )); // glyphnumber: 2473, Ventiduesima
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ventiduesimaAlta", 0xE518 )); // glyphnumber: 2474, Ventiduesima alta
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ventiduesimaBassa", 0xE519 )); // glyphnumber: 2475, Ventiduesima bassa
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "ventiduesimaBassaMb", 0xE51E )); // glyphnumber: 2476, Ventiduesima bassa (mb)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "vocalFingerClickStockhausen", 0xE649 )); // glyphnumber: 2477, Finger click (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "vocalMouthClosed", 0xE640 )); // glyphnumber: 2478, Mouth closed
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "vocalMouthOpen", 0xE642 )); // glyphnumber: 2479, Mouth open
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "vocalMouthPursed", 0xE644 )); // glyphnumber: 2480, Mouth pursed
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "vocalMouthSlightlyOpen", 0xE641 )); // glyphnumber: 2481, Mouth slightly open
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "vocalMouthWideOpen", 0xE643 )); // glyphnumber: 2482, Mouth wide open
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "vocalNasalVoice", 0xE647 )); // glyphnumber: 2483, Nasal voice
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "vocalSprechgesang", 0xE645 )); // glyphnumber: 2484, Sprechgesang
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "vocalTongueClickStockhausen", 0xE648 )); // glyphnumber: 2485, Tongue click (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "vocalTongueFingerClickStockhausen", 0xE64A )); // glyphnumber: 2486, Tongue and finger click (Stockhausen)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "vocalsSussurando", 0xE646 )); // glyphnumber: 2487, Combining sussurando for stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleArpeggiatoDown", 0xEAAA )); // glyphnumber: 2488, Arpeggiato wiggle segment, downwards
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleArpeggiatoDownArrow", 0xEAAE )); // glyphnumber: 2489, Arpeggiato arrowhead down
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleArpeggiatoDownSwash", 0xEAAC )); // glyphnumber: 2490, Arpeggiato downward swash
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleArpeggiatoUp", 0xEAA9 )); // glyphnumber: 2491, Arpeggiato wiggle segment, upwards
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleArpeggiatoUpArrow", 0xEAAD )); // glyphnumber: 2492, Arpeggiato arrowhead up
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleArpeggiatoUpSwash", 0xEAAB )); // glyphnumber: 2493, Arpeggiato upward swash
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleCircular", 0xEAC9 )); // glyphnumber: 2494, Circular motion segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleCircularConstant", 0xEAC0 )); // glyphnumber: 2495, Constant circular motion segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleCircularConstantFlipped", 0xEAC1 )); // glyphnumber: 2496, Constant circular motion segment (flipped)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleCircularConstantFlippedLarge", 0xEAC3 )); // glyphnumber: 2497, Constant circular motion segment (flipped, large)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleCircularConstantLarge", 0xEAC2 )); // glyphnumber: 2498, Constant circular motion segment (large)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleCircularEnd", 0xEACB )); // glyphnumber: 2499, Circular motion end
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleCircularLarge", 0xEAC8 )); // glyphnumber: 2500, Circular motion segment, large
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleCircularLarger", 0xEAC7 )); // glyphnumber: 2501, Circular motion segment, larger
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleCircularLargerStill", 0xEAC6 )); // glyphnumber: 2502, Circular motion segment, larger still
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleCircularLargest", 0xEAC5 )); // glyphnumber: 2503, Circular motion segment, largest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleCircularSmall", 0xEACA )); // glyphnumber: 2504, Circular motion segment, small
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleCircularStart", 0xEAC4 )); // glyphnumber: 2505, Circular motion start
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleGlissando", 0xEAAF )); // glyphnumber: 2506, Glissando wiggle segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleGlissandoGroup1", 0xEABD )); // glyphnumber: 2507, Group glissando 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleGlissandoGroup2", 0xEABE )); // glyphnumber: 2508, Group glissando 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleGlissandoGroup3", 0xEABF )); // glyphnumber: 2509, Group glissando 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleRandom1", 0xEAF0 )); // glyphnumber: 2510, Quasi-random squiggle 1
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleRandom2", 0xEAF1 )); // glyphnumber: 2511, Quasi-random squiggle 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleRandom3", 0xEAF2 )); // glyphnumber: 2512, Quasi-random squiggle 3
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleRandom4", 0xEAF3 )); // glyphnumber: 2513, Quasi-random squiggle 4
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleSawtooth", 0xEABB )); // glyphnumber: 2514, Sawtooth line segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleSawtoothNarrow", 0xEABA )); // glyphnumber: 2515, Narrow sawtooth line segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleSawtoothWide", 0xEABC )); // glyphnumber: 2516, Wide sawtooth line segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleSquareWave", 0xEAB8 )); // glyphnumber: 2517, Square wave line segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleSquareWaveNarrow", 0xEAB7 )); // glyphnumber: 2518, Narrow square wave line segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleSquareWaveWide", 0xEAB9 )); // glyphnumber: 2519, Wide square wave line segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleTrill", 0xEAA4 )); // glyphnumber: 2520, Trill wiggle segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleTrillFast", 0xEAA3 )); // glyphnumber: 2521, Trill wiggle segment, fast
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleTrillFaster", 0xEAA2 )); // glyphnumber: 2522, Trill wiggle segment, faster
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleTrillFasterStill", 0xEAA1 )); // glyphnumber: 2523, Trill wiggle segment, faster still
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleTrillFastest", 0xEAA0 )); // glyphnumber: 2524, Trill wiggle segment, fastest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleTrillSlow", 0xEAA5 )); // glyphnumber: 2525, Trill wiggle segment, slow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleTrillSlower", 0xEAA6 )); // glyphnumber: 2526, Trill wiggle segment, slower
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleTrillSlowerStill", 0xEAA7 )); // glyphnumber: 2527, Trill wiggle segment, slower still
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleTrillSlowest", 0xEAA8 )); // glyphnumber: 2528, Trill wiggle segment, slowest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVIbratoLargestSlower", 0xEAEE )); // glyphnumber: 2529, Vibrato largest, slower
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVIbratoMediumSlower", 0xEAE0 )); // glyphnumber: 2530, Vibrato medium, slower
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibrato", 0xEAB0 )); // glyphnumber: 2531, Vibrato / shake wiggle segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoLargeFast", 0xEAE5 )); // glyphnumber: 2532, Vibrato large, fast
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoLargeFaster", 0xEAE4 )); // glyphnumber: 2533, Vibrato large, faster
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoLargeFasterStill", 0xEAE3 )); // glyphnumber: 2534, Vibrato large, faster still
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoLargeFastest", 0xEAE2 )); // glyphnumber: 2535, Vibrato large, fastest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoLargeSlow", 0xEAE6 )); // glyphnumber: 2536, Vibrato large, slow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoLargeSlower", 0xEAE7 )); // glyphnumber: 2537, Vibrato large, slower
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoLargeSlowest", 0xEAE8 )); // glyphnumber: 2538, Vibrato large, slowest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoLargestFast", 0xEAEC )); // glyphnumber: 2539, Vibrato largest, fast
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoLargestFaster", 0xEAEB )); // glyphnumber: 2540, Vibrato largest, faster
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoLargestFasterStill", 0xEAEA )); // glyphnumber: 2541, Vibrato largest, faster still
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoLargestFastest", 0xEAE9 )); // glyphnumber: 2542, Vibrato largest, fastest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoLargestSlow", 0xEAED )); // glyphnumber: 2543, Vibrato largest, slow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoLargestSlowest", 0xEAEF )); // glyphnumber: 2544, Vibrato largest, slowest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoMediumFast", 0xEADE )); // glyphnumber: 2545, Vibrato medium, fast
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoMediumFaster", 0xEADD )); // glyphnumber: 2546, Vibrato medium, faster
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoMediumFasterStill", 0xEADC )); // glyphnumber: 2547, Vibrato medium, faster still
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoMediumFastest", 0xEADB )); // glyphnumber: 2548, Vibrato medium, fastest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoMediumSlow", 0xEADF )); // glyphnumber: 2549, Vibrato medium, slow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoMediumSlowest", 0xEAE1 )); // glyphnumber: 2550, Vibrato medium, slowest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallFast", 0xEAD7 )); // glyphnumber: 2551, Vibrato small, fast
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallFaster", 0xEAD6 )); // glyphnumber: 2552, Vibrato small, faster
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallFasterStill", 0xEAD5 )); // glyphnumber: 2553, Vibrato small, faster still
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallFastest", 0xEAD4 )); // glyphnumber: 2554, Vibrato small, fastest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallSlow", 0xEAD8 )); // glyphnumber: 2555, Vibrato small, slow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallSlower", 0xEAD9 )); // glyphnumber: 2556, Vibrato small, slower
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallSlowest", 0xEADA )); // glyphnumber: 2557, Vibrato small, slowest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallestFast", 0xEAD0 )); // glyphnumber: 2558, Vibrato smallest, fast
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallestFaster", 0xEACF )); // glyphnumber: 2559, Vibrato smallest, faster
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallestFasterStill", 0xEACE )); // glyphnumber: 2560, Vibrato smallest, faster still
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallestFastest", 0xEACD )); // glyphnumber: 2561, Vibrato smallest, fastest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallestSlow", 0xEAD1 )); // glyphnumber: 2562, Vibrato smallest, slow
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallestSlower", 0xEAD2 )); // glyphnumber: 2563, Vibrato smallest, slower
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoSmallestSlowest", 0xEAD3 )); // glyphnumber: 2564, Vibrato smallest, slowest
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoStart", 0xEACC )); // glyphnumber: 2565, Vibrato start
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleVibratoWide", 0xEAB1 )); // glyphnumber: 2566, Wide vibrato / shake wiggle segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleWavy", 0xEAB5 )); // glyphnumber: 2567, Wavy line segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleWavyNarrow", 0xEAB4 )); // glyphnumber: 2568, Narrow wavy line segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "wiggleWavyWide", 0xEAB6 )); // glyphnumber: 2569, Wide wavy line segment
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windClosedHole", 0xE5F4 )); // glyphnumber: 2570, Closed hole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windFlatEmbouchure", 0xE5FB )); // glyphnumber: 2571, Flatter embouchure
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windHalfClosedHole1", 0xE5F6 )); // glyphnumber: 2572, Half-closed hole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windHalfClosedHole2", 0xE5F7 )); // glyphnumber: 2573, Half-closed hole 2
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windHalfClosedHole3", 0xE5F8 )); // glyphnumber: 2574, Half-open hole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windLessRelaxedEmbouchure", 0xE5FE )); // glyphnumber: 2575, Somewhat relaxed embouchure
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windLessTightEmbouchure", 0xE600 )); // glyphnumber: 2576, Somewhat tight embouchure
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windMouthpiecePop", 0xE60A )); // glyphnumber: 2577, Mouthpiece or hand pop
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windMultiphonicsBlackStem", 0xE607 )); // glyphnumber: 2578, Combining multiphonics (black) for stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windMultiphonicsBlackWhiteStem", 0xE609 )); // glyphnumber: 2579, Combining multiphonics (black and white) for stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windMultiphonicsWhiteStem", 0xE608 )); // glyphnumber: 2580, Combining multiphonics (white) for stem
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windOpenHole", 0xE5F9 )); // glyphnumber: 2581, Open hole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windReedPositionIn", 0xE606 )); // glyphnumber: 2582, Much more reed (push inwards)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windReedPositionNormal", 0xE604 )); // glyphnumber: 2583, Normal reed position
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windReedPositionOut", 0xE605 )); // glyphnumber: 2584, Very little reed (pull outwards)
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windRelaxedEmbouchure", 0xE5FD )); // glyphnumber: 2585, Relaxed embouchure
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windRimOnly", 0xE60B )); // glyphnumber: 2586, Rim only
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windSharpEmbouchure", 0xE5FC )); // glyphnumber: 2587, Sharper embouchure
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windStrongAirPressure", 0xE603 )); // glyphnumber: 2588, Very tight embouchure / strong air pressure
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windThreeQuartersClosedHole", 0xE5F5 )); // glyphnumber: 2589, Three-quarters closed hole
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windTightEmbouchure", 0xE5FF )); // glyphnumber: 2590, Tight embouchure
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windTrillKey", 0xE5FA )); // glyphnumber: 2591, Trill key
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windVeryTightEmbouchure", 0xE601 )); // glyphnumber: 2592, Very tight embouchure
            outMap.emplace(std::make_pair<const std::string, const char16_t>( "windWeakAirPressure", 0xE602 )); // glyphnumber: 2593, Very relaxed embouchure / weak air-pressure
        }
    }
}
