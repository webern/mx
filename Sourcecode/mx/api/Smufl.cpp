// MusicXML Class Library
// Copyright (c) by Matthew James Briggs
// Distributed under the MIT License

#include "mx/api/Smufl.h"
#include "mx/sparsepp/spp.h"

#include <codecvt>
#include <locale>


using SmuflPair = std::pair<std::string, char16_t>;
using SmuflMap = spp::sparse_hash_map<std::string, char16_t>;
using SmuflIter = SmuflMap::const_iterator;

namespace mx
{
    namespace api
    {
        static const SmuflMap sMap =
        {
            { "4stringTabClef", 0xE06E },  // glyphnumber: 0, 4-string tab clef
            { "6stringTabClef", 0xE06D },  // glyphnumber: 1, 6-string tab clef
            { "accSagittal11LargeDiesisDown", 0xE30D },  // glyphnumber: 2, 11 large diesis down, 3° down [46 EDO]
            { "accSagittal11LargeDiesisUp", 0xE30C },  // glyphnumber: 3, 11 large diesis up, (11L}, (sharp less 11M}, 3° up [46 EDO]
            { "accSagittal11MediumDiesisDown", 0xE30B },  // glyphnumber: 4, 11 medium diesis down, 1°[17 31] 2°46 down, 1/4-tone down
            { "accSagittal11MediumDiesisUp", 0xE30A },  // glyphnumber: 5, 11 medium diesis up, (11M}, 1°[17 31] 2°46 up, 1/4-tone up
            { "accSagittal11v19LargeDiesisDown", 0xE3AB },  // glyphnumber: 6, 11:19 large diesis down
            { "accSagittal11v19LargeDiesisUp", 0xE3AA },  // glyphnumber: 7, 11:19 large diesis up, (11:19L, apotome less 11:19M)
            { "accSagittal11v19MediumDiesisDown", 0xE3A3 },  // glyphnumber: 8, 11:19 medium diesis down
            { "accSagittal11v19MediumDiesisUp", 0xE3A2 },  // glyphnumber: 9, 11:19 medium diesis up, (11:19M, 11M plus 19s)
            { "accSagittal11v49CommaDown", 0xE397 },  // glyphnumber: 10, 11:49 comma down
            { "accSagittal11v49CommaUp", 0xE396 },  // glyphnumber: 11, 11:49 comma up, (11:49C, 11M less 49C)
            { "accSagittal143CommaDown", 0xE395 },  // glyphnumber: 12, 143 comma down
            { "accSagittal143CommaUp", 0xE394 },  // glyphnumber: 13, 143 comma up, (143C, 13L less 11M)
            { "accSagittal17CommaDown", 0xE343 },  // glyphnumber: 14, 17 comma down
            { "accSagittal17CommaUp", 0xE342 },  // glyphnumber: 15, 17 comma up, (17C)
            { "accSagittal17KleismaDown", 0xE393 },  // glyphnumber: 16, 17 kleisma down
            { "accSagittal17KleismaUp", 0xE392 },  // glyphnumber: 17, 17 kleisma up, (17k)
            { "accSagittal19CommaDown", 0xE399 },  // glyphnumber: 18, 19 comma down
            { "accSagittal19CommaUp", 0xE398 },  // glyphnumber: 19, 19 comma up, (19C)
            { "accSagittal19SchismaDown", 0xE391 },  // glyphnumber: 20, 19 schisma down
            { "accSagittal19SchismaUp", 0xE390 },  // glyphnumber: 21, 19 schisma up, (19s)
            { "accSagittal23CommaDown", 0xE371 },  // glyphnumber: 22, 23 comma down, 2° down [96 EDO], 1/8-tone down
            { "accSagittal23CommaUp", 0xE370 },  // glyphnumber: 23, 23 comma up, (23C}, 2° up [96 EDO], 1/8-tone up
            { "accSagittal23SmallDiesisDown", 0xE39F },  // glyphnumber: 24, 23 small diesis down
            { "accSagittal23SmallDiesisUp", 0xE39E },  // glyphnumber: 25, 23 small diesis up, (23S)
            { "accSagittal25SmallDiesisDown", 0xE307 },  // glyphnumber: 26, 25 small diesis down, 2° down [53 EDO]
            { "accSagittal25SmallDiesisUp", 0xE306 },  // glyphnumber: 27, 25 small diesis up, (25S, ~5:13S, ~37S, 5C plus 5C}, 2° up [53 EDO]
            { "accSagittal35LargeDiesisDown", 0xE30F },  // glyphnumber: 28, 35 large diesis down, 2° down [50 EDO], 5/18-tone down
            { "accSagittal35LargeDiesisUp", 0xE30E },  // glyphnumber: 29, 35 large diesis up, (35L, ~13L, ~125L,  sharp less 35M}, 2°50 up
            { "accSagittal35MediumDiesisDown", 0xE309 },  // glyphnumber: 30, 35 medium diesis down, 1°[50] 2°[27] down, 2/9-tone down
            { "accSagittal35MediumDiesisUp", 0xE308 },  // glyphnumber: 31, 35 medium diesis up, (35M, ~13M, ~125M, 5C plus 7C}, 2/9-tone up
            { "accSagittal49LargeDiesisDown", 0xE3A9 },  // glyphnumber: 32, 49 large diesis down
            { "accSagittal49LargeDiesisUp", 0xE3A8 },  // glyphnumber: 33, 49 large diesis up, (49L, ~31L, apotome less 49M)
            { "accSagittal49MediumDiesisDown", 0xE3A5 },  // glyphnumber: 34, 49 medium diesis down
            { "accSagittal49MediumDiesisUp", 0xE3A4 },  // glyphnumber: 35, 49 medium diesis up, (49M, ~31M, 7C plus 7C)
            { "accSagittal49SmallDiesisDown", 0xE39D },  // glyphnumber: 36, 49 small diesis down
            { "accSagittal49SmallDiesisUp", 0xE39C },  // glyphnumber: 37, 49 small diesis up, (49S, ~31S)
            { "accSagittal55CommaDown", 0xE345 },  // glyphnumber: 38, 55 comma down, 3° down [96 EDO], 3/16-tone down
            { "accSagittal55CommaUp", 0xE344 },  // glyphnumber: 39, 55 comma up, (55C, 11M less 5C}, 3°up [96 EDO], 3/16-tone up
            { "accSagittal5CommaDown", 0xE303 },  // glyphnumber: 40, 5 comma down, 1° down [22 27 29 34 41 46 53 96 EDOs], 1/12-tone down
            { "accSagittal5CommaUp", 0xE302 },  // glyphnumber: 41, 5 comma up, (5C}, 1° up [22 27 29 34 41 46 53 96 EDOs], 1/12-tone up
            { "accSagittal5v11SmallDiesisDown", 0xE349 },  // glyphnumber: 42, 5:11 small diesis down
            { "accSagittal5v11SmallDiesisUp", 0xE348 },  // glyphnumber: 43, 5:11 small diesis up, (5:11S, ~7:13S, ~11:17S, 5:7k plus 7:11C)
            { "accSagittal5v13LargeDiesisDown", 0xE3AD },  // glyphnumber: 44, 5:13 large diesis down
            { "accSagittal5v13LargeDiesisUp", 0xE3AC },  // glyphnumber: 45, 5:13 large diesis up, (5:13L, ~37L, apotome less 5:13M)
            { "accSagittal5v13MediumDiesisDown", 0xE3A1 },  // glyphnumber: 46, 5:13 medium diesis down
            { "accSagittal5v13MediumDiesisUp", 0xE3A0 },  // glyphnumber: 47, 5:13 medium diesis up, (5:13M, ~37M, 5C plus 13C)
            { "accSagittal5v19CommaDown", 0xE373 },  // glyphnumber: 48, 5:19 comma down, 1/20-tone down
            { "accSagittal5v19CommaUp", 0xE372 },  // glyphnumber: 49, 5:19 comma up, (5:19C, 5C plus 19s}, 1/20-tone up
            { "accSagittal5v23SmallDiesisDown", 0xE375 },  // glyphnumber: 50, 5:23 small diesis down, 2° down [60 EDO], 1/5-tone down
            { "accSagittal5v23SmallDiesisUp", 0xE374 },  // glyphnumber: 51, 5:23 small diesis up, (5:23S, 5C plus 23C}, 2° up [60 EDO], 1/5-tone up
            { "accSagittal5v49MediumDiesisDown", 0xE3A7 },  // glyphnumber: 52, 5:49 medium diesis down
            { "accSagittal5v49MediumDiesisUp", 0xE3A6 },  // glyphnumber: 53, 5:49 medium diesis up, (5:49M, half apotome)
            { "accSagittal5v7KleismaDown", 0xE301 },  // glyphnumber: 54, 5:7 kleisma down
            { "accSagittal5v7KleismaUp", 0xE300 },  // glyphnumber: 55, 5:7 kleisma up, (5:7k, ~11:13k, 7C less 5C)
            { "accSagittal7CommaDown", 0xE305 },  // glyphnumber: 56, 7 comma down, 1° down [43 EDO], 2° down [72 EDO], 1/6-tone down
            { "accSagittal7CommaUp", 0xE304 },  // glyphnumber: 57, 7 comma up, (7C}, 1° up [43 EDO], 2° up [72 EDO], 1/6-tone up
            { "accSagittal7v11CommaDown", 0xE347 },  // glyphnumber: 58, 7:11 comma down, 1° down [60 EDO], 1/10-tone down
            { "accSagittal7v11CommaUp", 0xE346 },  // glyphnumber: 59, 7:11 comma up, (7:11C, ~13:17S, ~29S, 11L less 7C}, 1° up [60 EDO]
            { "accSagittal7v11KleismaDown", 0xE341 },  // glyphnumber: 60, 7:11 kleisma down
            { "accSagittal7v11KleismaUp", 0xE340 },  // glyphnumber: 61, 7:11 kleisma up, (7:11k, ~29k)
            { "accSagittal7v19CommaDown", 0xE39B },  // glyphnumber: 62, 7:19 comma down
            { "accSagittal7v19CommaUp", 0xE39A },  // glyphnumber: 63, 7:19 comma up, (7:19C, 7C less 19s)
            { "accSagittalAcute", 0xE3F2 },  // glyphnumber: 64, Acute, 5 schisma up (5s}, 2 cents up
            { "accSagittalDoubleFlat", 0xE335 },  // glyphnumber: 65, Double flat, (2 apotomes down)[almost all EDOs], whole-tone down
            { "accSagittalDoubleFlat11v49CUp", 0xE3E9 },  // glyphnumber: 66, Double flat 11:49C-up
            { "accSagittalDoubleFlat143CUp", 0xE3EB },  // glyphnumber: 67, Double flat 143C-up
            { "accSagittalDoubleFlat17CUp", 0xE365 },  // glyphnumber: 68, Double flat 17C-up
            { "accSagittalDoubleFlat17kUp", 0xE3ED },  // glyphnumber: 69, Double flat 17k-up
            { "accSagittalDoubleFlat19CUp", 0xE3E7 },  // glyphnumber: 70, Double flat 19C-up
            { "accSagittalDoubleFlat19sUp", 0xE3EF },  // glyphnumber: 71, Double flat 19s-up
            { "accSagittalDoubleFlat23CUp", 0xE387 },  // glyphnumber: 72, Double flat 23C-up, 14° down [96 EDO], 7/8-tone down
            { "accSagittalDoubleFlat23SUp", 0xE3E1 },  // glyphnumber: 73, Double flat 23S-up
            { "accSagittalDoubleFlat25SUp", 0xE32D },  // glyphnumber: 74, Double flat 25S-up, 8°down [53 EDO]
            { "accSagittalDoubleFlat49SUp", 0xE3E3 },  // glyphnumber: 75, Double flat 49S-up
            { "accSagittalDoubleFlat55CUp", 0xE363 },  // glyphnumber: 76, Double flat 55C-up, 13° down [96 EDO], 13/16-tone down
            { "accSagittalDoubleFlat5CUp", 0xE331 },  // glyphnumber: 77, Double flat 5C-up, 5°[22 29] 7°[34 41] 9°53 down, 11/12 tone down
            { "accSagittalDoubleFlat5v11SUp", 0xE35F },  // glyphnumber: 78, Double flat 5:11S-up
            { "accSagittalDoubleFlat5v19CUp", 0xE385 },  // glyphnumber: 79, Double flat 5:19C-up, 19/20-tone down
            { "accSagittalDoubleFlat5v23SUp", 0xE383 },  // glyphnumber: 80, Double flat 5:23S-up, 8° down  [60 EDO], 4/5-tone down
            { "accSagittalDoubleFlat5v7kUp", 0xE333 },  // glyphnumber: 81, Double flat 5:7k-up
            { "accSagittalDoubleFlat7CUp", 0xE32F },  // glyphnumber: 82, Double flat 7C-up, 5° down [43 EDO], 10° down [72 EDO], 5/6-tone down
            { "accSagittalDoubleFlat7v11CUp", 0xE361 },  // glyphnumber: 83, Double flat 7:11C-up, 9° down [60 EDO], 9/10-tone down
            { "accSagittalDoubleFlat7v11kUp", 0xE367 },  // glyphnumber: 84, Double flat 7:11k-up
            { "accSagittalDoubleFlat7v19CUp", 0xE3E5 },  // glyphnumber: 85, Double flat 7:19C-up
            { "accSagittalDoubleSharp", 0xE334 },  // glyphnumber: 86, Double sharp, (2 apotomes up)[almost all EDOs], whole-tone up
            { "accSagittalDoubleSharp11v49CDown", 0xE3E8 },  // glyphnumber: 87, Double sharp 11:49C-down
            { "accSagittalDoubleSharp143CDown", 0xE3EA },  // glyphnumber: 88, Double sharp 143C-down
            { "accSagittalDoubleSharp17CDown", 0xE364 },  // glyphnumber: 89, Double sharp 17C-down
            { "accSagittalDoubleSharp17kDown", 0xE3EC },  // glyphnumber: 90, Double sharp 17k-down
            { "accSagittalDoubleSharp19CDown", 0xE3E6 },  // glyphnumber: 91, Double sharp 19C-down
            { "accSagittalDoubleSharp19sDown", 0xE3EE },  // glyphnumber: 92, Double sharp 19s-down
            { "accSagittalDoubleSharp23CDown", 0xE386 },  // glyphnumber: 93, Double sharp 23C-down, 14°up [96 EDO], 7/8-tone up
            { "accSagittalDoubleSharp23SDown", 0xE3E0 },  // glyphnumber: 94, Double sharp 23S-down
            { "accSagittalDoubleSharp25SDown", 0xE32C },  // glyphnumber: 95, Double sharp 25S-down, 8°up [53 EDO]
            { "accSagittalDoubleSharp49SDown", 0xE3E2 },  // glyphnumber: 96, Double sharp 49S-down
            { "accSagittalDoubleSharp55CDown", 0xE362 },  // glyphnumber: 97, Double sharp 55C-down, 13° up [96 EDO], 13/16-tone up
            { "accSagittalDoubleSharp5CDown", 0xE330 },  // glyphnumber: 98, Double sharp 5C-down, 5°[22 29] 7°[34 41] 9°53 up, 11/12 tone up
            { "accSagittalDoubleSharp5v11SDown", 0xE35E },  // glyphnumber: 99, Double sharp 5:11S-down
            { "accSagittalDoubleSharp5v19CDown", 0xE384 },  // glyphnumber: 100, Double sharp 5:19C-down, 19/20-tone up
            { "accSagittalDoubleSharp5v23SDown", 0xE382 },  // glyphnumber: 101, Double sharp 5:23S-down, 8° up [60 EDO], 4/5-tone up
            { "accSagittalDoubleSharp5v7kDown", 0xE332 },  // glyphnumber: 102, Double sharp 5:7k-down
            { "accSagittalDoubleSharp7CDown", 0xE32E },  // glyphnumber: 103, Double sharp 7C-down, 5°[43] 10°[72] up, 5/6-tone up
            { "accSagittalDoubleSharp7v11CDown", 0xE360 },  // glyphnumber: 104, Double sharp 7:11C-down, 9° up [60 EDO], 9/10-tone up
            { "accSagittalDoubleSharp7v11kDown", 0xE366 },  // glyphnumber: 105, Double sharp 7:11k-down
            { "accSagittalDoubleSharp7v19CDown", 0xE3E4 },  // glyphnumber: 106, Double sharp 7:19C-down
            { "accSagittalFlat", 0xE319 },  // glyphnumber: 107, Flat, (apotome down)[almost all EDOs], 1/2-tone down
            { "accSagittalFlat11LDown", 0xE329 },  // glyphnumber: 108, Flat 11L-down, 8° up [46 EDO]
            { "accSagittalFlat11MDown", 0xE327 },  // glyphnumber: 109, Flat 11M-down, 3° down [17 31 EDOs], 7° down [46 EDO], 3/4-tone down
            { "accSagittalFlat11v19LDown", 0xE3DB },  // glyphnumber: 110, Flat 11:19L-down
            { "accSagittalFlat11v19MDown", 0xE3D3 },  // glyphnumber: 111, Flat 11:19M-down
            { "accSagittalFlat11v49CDown", 0xE3C7 },  // glyphnumber: 112, Flat 11:49C-down
            { "accSagittalFlat11v49CUp", 0xE3B9 },  // glyphnumber: 113, Flat 11:49C-up
            { "accSagittalFlat143CDown", 0xE3C5 },  // glyphnumber: 114, Flat 143C-down
            { "accSagittalFlat143CUp", 0xE3BB },  // glyphnumber: 115, Flat 143C-up
            { "accSagittalFlat17CDown", 0xE357 },  // glyphnumber: 116, Flat 17C-down
            { "accSagittalFlat17CUp", 0xE351 },  // glyphnumber: 117, Flat 17C-up
            { "accSagittalFlat17kDown", 0xE3C3 },  // glyphnumber: 118, Flat 17k-down
            { "accSagittalFlat17kUp", 0xE3BD },  // glyphnumber: 119, Flat 17k-up
            { "accSagittalFlat19CDown", 0xE3C9 },  // glyphnumber: 120, Flat 19C-down
            { "accSagittalFlat19CUp", 0xE3B7 },  // glyphnumber: 121, Flat 19C-up
            { "accSagittalFlat19sDown", 0xE3C1 },  // glyphnumber: 122, Flat 19s-down
            { "accSagittalFlat19sUp", 0xE3BF },  // glyphnumber: 123, Flat 19s-up
            { "accSagittalFlat23CDown", 0xE37D },  // glyphnumber: 124, Flat 23C-down, 10° down [96 EDO], 5/8-tone down
            { "accSagittalFlat23CUp", 0xE37B },  // glyphnumber: 125, Flat 23C-up, 6° down [96 EDO], 3/8-tone down
            { "accSagittalFlat23SDown", 0xE3CF },  // glyphnumber: 126, Flat 23S-down
            { "accSagittalFlat23SUp", 0xE3B1 },  // glyphnumber: 127, Flat 23S-up
            { "accSagittalFlat25SDown", 0xE323 },  // glyphnumber: 128, Flat 25S-down, 7° down [53 EDO]
            { "accSagittalFlat25SUp", 0xE311 },  // glyphnumber: 129, Flat 25S-up, 3° down [53 EDO]
            { "accSagittalFlat35LDown", 0xE32B },  // glyphnumber: 130, Flat 35L-down, 5° down [50 EDO]
            { "accSagittalFlat35MDown", 0xE325 },  // glyphnumber: 131, Flat 35M-down, 4° down [50 EDO], 6° down [27 EDO], 13/18-tone down
            { "accSagittalFlat49LDown", 0xE3D9 },  // glyphnumber: 132, Flat 49L-down
            { "accSagittalFlat49MDown", 0xE3D5 },  // glyphnumber: 133, Flat 49M-down
            { "accSagittalFlat49SDown", 0xE3CD },  // glyphnumber: 134, Flat 49S-down
            { "accSagittalFlat49SUp", 0xE3B3 },  // glyphnumber: 135, Flat 49S-up
            { "accSagittalFlat55CDown", 0xE359 },  // glyphnumber: 136, Flat 55C-down, 11° down [96 EDO], 11/16-tone down
            { "accSagittalFlat55CUp", 0xE34F },  // glyphnumber: 137, Flat 55C-up, 5° down [96 EDO], 5/16-tone down
            { "accSagittalFlat5CDown", 0xE31F },  // glyphnumber: 138, Flat 5C-down, 4°[22 29] 5°[27 34 41] 6°[39 46 53] down, 7/12-tone down
            { "accSagittalFlat5CUp", 0xE315 },  // glyphnumber: 139, Flat 5C-up, 2°[22,29] 3°[34 41] 4°[46 53 60] down, 5/12-tone down
            { "accSagittalFlat5v11SDown", 0xE35D },  // glyphnumber: 140, Flat 5:11S-down
            { "accSagittalFlat5v11SUp", 0xE34B },  // glyphnumber: 141, Flat 5:11S-up
            { "accSagittalFlat5v13LDown", 0xE3DD },  // glyphnumber: 142, Flat 5:13L-down
            { "accSagittalFlat5v13MDown", 0xE3D1 },  // glyphnumber: 143, Flat 5:13M-down
            { "accSagittalFlat5v19CDown", 0xE37F },  // glyphnumber: 144, Flat 5:19C-down, 11/20-tone down
            { "accSagittalFlat5v19CUp", 0xE379 },  // glyphnumber: 145, Flat 5:19C-up, 9/20-tone down
            { "accSagittalFlat5v23SDown", 0xE381 },  // glyphnumber: 146, Flat 5:23S-down, 7° down [60 EDO], 7/10-tone down
            { "accSagittalFlat5v23SUp", 0xE377 },  // glyphnumber: 147, Flat 5:23S-up, 3° down [60 EDO], 3/10-tone down
            { "accSagittalFlat5v49MDown", 0xE3D7 },  // glyphnumber: 148, Flat 5:49M-down
            { "accSagittalFlat5v7kDown", 0xE31D },  // glyphnumber: 149, Flat 5:7k-down
            { "accSagittalFlat5v7kUp", 0xE317 },  // glyphnumber: 150, Flat 5:7k-up
            { "accSagittalFlat7CDown", 0xE321 },  // glyphnumber: 151, Flat 7C-down, 4° down [43 EDO], 8° down [72 EDO], 2/3-tone down
            { "accSagittalFlat7CUp", 0xE313 },  // glyphnumber: 152, Flat 7C-up, 2° down [43 EDO], 4° down [72 EDO], 1/3-tone down
            { "accSagittalFlat7v11CDown", 0xE35B },  // glyphnumber: 153, Flat 7:11C-down, 6° down [60 EDO], 3/5- tone down
            { "accSagittalFlat7v11CUp", 0xE34D },  // glyphnumber: 154, Flat 7:11C-up, 4° down [60 EDO], 2/5-tone down
            { "accSagittalFlat7v11kDown", 0xE355 },  // glyphnumber: 155, Flat 7:11k-down
            { "accSagittalFlat7v11kUp", 0xE353 },  // glyphnumber: 156, Flat 7:11k-up
            { "accSagittalFlat7v19CDown", 0xE3CB },  // glyphnumber: 157, Flat 7:19C-down
            { "accSagittalFlat7v19CUp", 0xE3B5 },  // glyphnumber: 158, Flat 7:19C-up
            { "accSagittalGrave", 0xE3F3 },  // glyphnumber: 159, Grave, 5 schisma down, 2 cents down
            { "accSagittalShaftDown", 0xE3F1 },  // glyphnumber: 160, Shaft down, (natural for use with only diacritics down)
            { "accSagittalShaftUp", 0xE3F0 },  // glyphnumber: 161, Shaft up, (natural for use with only diacritics up)
            { "accSagittalSharp", 0xE318 },  // glyphnumber: 162, Sharp, (apotome up)[almost all EDOs], 1/2-tone up
            { "accSagittalSharp11LUp", 0xE328 },  // glyphnumber: 163, Sharp 11L-up, 8° up [46 EDO]
            { "accSagittalSharp11MUp", 0xE326 },  // glyphnumber: 164, Sharp 11M-up, 3° up [17 31 EDOs], 7° up [46 EDO], 3/4-tone up
            { "accSagittalSharp11v19LUp", 0xE3DA },  // glyphnumber: 165, Sharp 11:19L-up
            { "accSagittalSharp11v19MUp", 0xE3D2 },  // glyphnumber: 166, Sharp 11:19M-up
            { "accSagittalSharp11v49CDown", 0xE3B8 },  // glyphnumber: 167, Sharp 11:49C-down
            { "accSagittalSharp11v49CUp", 0xE3C6 },  // glyphnumber: 168, Sharp 11:49C-up
            { "accSagittalSharp143CDown", 0xE3BA },  // glyphnumber: 169, Sharp 143C-down
            { "accSagittalSharp143CUp", 0xE3C4 },  // glyphnumber: 170, Sharp 143C-up
            { "accSagittalSharp17CDown", 0xE350 },  // glyphnumber: 171, Sharp 17C-down
            { "accSagittalSharp17CUp", 0xE356 },  // glyphnumber: 172, Sharp 17C-up
            { "accSagittalSharp17kDown", 0xE3BC },  // glyphnumber: 173, Sharp 17k-down
            { "accSagittalSharp17kUp", 0xE3C2 },  // glyphnumber: 174, Sharp 17k-up
            { "accSagittalSharp19CDown", 0xE3B6 },  // glyphnumber: 175, Sharp 19C-down
            { "accSagittalSharp19CUp", 0xE3C8 },  // glyphnumber: 176, Sharp 19C-up
            { "accSagittalSharp19sDown", 0xE3BE },  // glyphnumber: 177, Sharp 19s-down
            { "accSagittalSharp19sUp", 0xE3C0 },  // glyphnumber: 178, Sharp 19s-up
            { "accSagittalSharp23CDown", 0xE37A },  // glyphnumber: 179, Sharp 23C-down, 6° up [96 EDO], 3/8-tone up
            { "accSagittalSharp23CUp", 0xE37C },  // glyphnumber: 180, Sharp 23C-up, 10° up [96 EDO], 5/8-tone up
            { "accSagittalSharp23SDown", 0xE3B0 },  // glyphnumber: 181, Sharp 23S-down
            { "accSagittalSharp23SUp", 0xE3CE },  // glyphnumber: 182, Sharp 23S-up
            { "accSagittalSharp25SDown", 0xE310 },  // glyphnumber: 183, Sharp 25S-down, 3° up [53 EDO]
            { "accSagittalSharp25SUp", 0xE322 },  // glyphnumber: 184, Sharp 25S-up, 7° up [53 EDO]
            { "accSagittalSharp35LUp", 0xE32A },  // glyphnumber: 185, Sharp 35L-up, 5° up [50 EDO]
            { "accSagittalSharp35MUp", 0xE324 },  // glyphnumber: 186, Sharp 35M-up, 4° up [50 EDO], 6° up [27 EDO], 13/18-tone up
            { "accSagittalSharp49LUp", 0xE3D8 },  // glyphnumber: 187, Sharp 49L-up
            { "accSagittalSharp49MUp", 0xE3D4 },  // glyphnumber: 188, Sharp 49M-up
            { "accSagittalSharp49SDown", 0xE3B2 },  // glyphnumber: 189, Sharp 49S-down
            { "accSagittalSharp49SUp", 0xE3CC },  // glyphnumber: 190, Sharp 49S-up
            { "accSagittalSharp55CDown", 0xE34E },  // glyphnumber: 191, Sharp 55C-down, 5° up [96 EDO], 5/16-tone up
            { "accSagittalSharp55CUp", 0xE358 },  // glyphnumber: 192, Sharp 55C-up, 11° up [96 EDO], 11/16-tone up
            { "accSagittalSharp5CDown", 0xE314 },  // glyphnumber: 193, Sharp 5C-down, 2°[22 29] 3°[34 41] 4°[46 53 60] up, 5/12-tone up
            { "accSagittalSharp5CUp", 0xE31E },  // glyphnumber: 194, Sharp 5C-up, 4°[22 29] 5°[27 34 41] 6°[39 46 53] up, 7/12-tone up
            { "accSagittalSharp5v11SDown", 0xE34A },  // glyphnumber: 195, Sharp 5:11S-down
            { "accSagittalSharp5v11SUp", 0xE35C },  // glyphnumber: 196, Sharp 5:11S-up
            { "accSagittalSharp5v13LUp", 0xE3DC },  // glyphnumber: 197, Sharp 5:13L-up
            { "accSagittalSharp5v13MUp", 0xE3D0 },  // glyphnumber: 198, Sharp 5:13M-up
            { "accSagittalSharp5v19CDown", 0xE378 },  // glyphnumber: 199, Sharp 5:19C-down, 9/20-tone up
            { "accSagittalSharp5v19CUp", 0xE37E },  // glyphnumber: 200, Sharp 5:19C-up, 11/20-tone up
            { "accSagittalSharp5v23SDown", 0xE376 },  // glyphnumber: 201, Sharp 5:23S-down, 3° up [60 EDO], 3/10-tone up
            { "accSagittalSharp5v23SUp", 0xE380 },  // glyphnumber: 202, Sharp 5:23S-up, 7° up [60 EDO], 7/10-tone up
            { "accSagittalSharp5v49MUp", 0xE3D6 },  // glyphnumber: 203, Sharp 5:49M-up, (one and a half apotomes)
            { "accSagittalSharp5v7kDown", 0xE316 },  // glyphnumber: 204, Sharp 5:7k-down
            { "accSagittalSharp5v7kUp", 0xE31C },  // glyphnumber: 205, Sharp 5:7k-up
            { "accSagittalSharp7CDown", 0xE312 },  // glyphnumber: 206, Sharp 7C-down, 2° up [43 EDO], 4° up [72 EDO], 1/3-tone up
            { "accSagittalSharp7CUp", 0xE320 },  // glyphnumber: 207, Sharp 7C-up, 4° up [43 EDO], 8° up [72 EDO], 2/3-tone up
            { "accSagittalSharp7v11CDown", 0xE34C },  // glyphnumber: 208, Sharp 7:11C-down, 4° up [60 EDO], 2/5-tone up
            { "accSagittalSharp7v11CUp", 0xE35A },  // glyphnumber: 209, Sharp 7:11C-up, 6° up [60 EDO], 3/5- tone up
            { "accSagittalSharp7v11kDown", 0xE352 },  // glyphnumber: 210, Sharp 7:11k-down
            { "accSagittalSharp7v11kUp", 0xE354 },  // glyphnumber: 211, Sharp 7:11k-up
            { "accSagittalSharp7v19CDown", 0xE3B4 },  // glyphnumber: 212, Sharp 7:19C-down
            { "accSagittalSharp7v19CUp", 0xE3CA },  // glyphnumber: 213, Sharp 7:19C-up
            { "accSagittalUnused1", 0xE31A },  // glyphnumber: 214, Unused
            { "accSagittalUnused2", 0xE31B },  // glyphnumber: 215, Unused
            { "accSagittalUnused3", 0xE3DE },  // glyphnumber: 216, Unused
            { "accSagittalUnused4", 0xE3DF },  // glyphnumber: 217, Unused
            { "accdnCombDot", 0xE8CA },  // glyphnumber: 218, Combining accordion coupler dot
            { "accdnCombLH2RanksEmpty", 0xE8C8 },  // glyphnumber: 219, Combining left hand, 2 ranks, empty
            { "accdnCombLH3RanksEmptySquare", 0xE8C9 },  // glyphnumber: 220, Combining left hand, 3 ranks, empty (square)
            { "accdnCombRH3RanksEmpty", 0xE8C6 },  // glyphnumber: 221, Combining right hand, 3 ranks, empty
            { "accdnCombRH4RanksEmpty", 0xE8C7 },  // glyphnumber: 222, Combining right hand, 4 ranks, empty
            { "accdnDiatonicClef", 0xE079 },  // glyphnumber: 223, Diatonic accordion clef
            { "accdnLH2Ranks16Round", 0xE8BC },  // glyphnumber: 224, Left hand, 2 ranks, 16' stop (round)
            { "accdnLH2Ranks8Plus16Round", 0xE8BD },  // glyphnumber: 225, Left hand, 2 ranks, 8' stop + 16' stop (round)
            { "accdnLH2Ranks8Round", 0xE8BB },  // glyphnumber: 226, Left hand, 2 ranks, 8' stop (round)
            { "accdnLH2RanksFullMasterRound", 0xE8C0 },  // glyphnumber: 227, Left hand, 2 ranks, full master (round)
            { "accdnLH2RanksMasterPlus16Round", 0xE8BF },  // glyphnumber: 228, Left hand, 2 ranks, master + 16' stop (round)
            { "accdnLH2RanksMasterRound", 0xE8BE },  // glyphnumber: 229, Left hand, 2 ranks, master (round)
            { "accdnLH3Ranks2Plus8Square", 0xE8C4 },  // glyphnumber: 230, Left hand, 3 ranks, 2' stop + 8' stop (square)
            { "accdnLH3Ranks2Square", 0xE8C2 },  // glyphnumber: 231, Left hand, 3 ranks, 2' stop (square)
            { "accdnLH3Ranks8Square", 0xE8C1 },  // glyphnumber: 232, Left hand, 3 ranks, 8' stop (square)
            { "accdnLH3RanksDouble8Square", 0xE8C3 },  // glyphnumber: 233, Left hand, 3 ranks, double 8' stop (square)
            { "accdnLH3RanksTuttiSquare", 0xE8C5 },  // glyphnumber: 234, Left hand, 3 ranks, 2' stop + double 8' stop (tutti) (square)
            { "accdnPull", 0xE8CC },  // glyphnumber: 235, Pull
            { "accdnPush", 0xE8CB },  // glyphnumber: 236, Push
            { "accdnRH3RanksAccordion", 0xE8AC },  // glyphnumber: 237, Right hand, 3 ranks, 8' stop + upper tremolo 8' stop + 16' stop (accordion)
            { "accdnRH3RanksAuthenticMusette", 0xE8A8 },  // glyphnumber: 238, Right hand, 3 ranks, lower tremolo 8' stop + 8' stop + upper tremolo 8' stop (authentic musette)
            { "accdnRH3RanksBandoneon", 0xE8AB },  // glyphnumber: 239, Right hand, 3 ranks, 8' stop + 16' stop (bandoneón)
            { "accdnRH3RanksBassoon", 0xE8A4 },  // glyphnumber: 240, Right hand, 3 ranks, 16' stop (bassoon)
            { "accdnRH3RanksClarinet", 0xE8A1 },  // glyphnumber: 241, Right hand, 3 ranks, 8' stop (clarinet)
            { "accdnRH3RanksDoubleTremoloLower8ve", 0xE8B1 },  // glyphnumber: 242, Right hand, 3 ranks, lower tremolo 8' stop + 8' stop + upper tremolo 8' stop + 16' stop
            { "accdnRH3RanksDoubleTremoloUpper8ve", 0xE8B2 },  // glyphnumber: 243, Right hand, 3 ranks, 4' stop + lower tremolo 8' stop + 8' stop + upper tremolo 8' stop
            { "accdnRH3RanksFullFactory", 0xE8B3 },  // glyphnumber: 244, Right hand, 3 ranks, 4' stop + lower tremolo 8' stop + 8' stop + upper tremolo 8' stop + 16' stop
            { "accdnRH3RanksHarmonium", 0xE8AA },  // glyphnumber: 245, Right hand, 3 ranks, 4' stop + 8' stop + 16' stop (harmonium)
            { "accdnRH3RanksImitationMusette", 0xE8A7 },  // glyphnumber: 246, Right hand, 3 ranks, 4' stop + 8' stop + upper tremolo 8' stop (imitation musette)
            { "accdnRH3RanksLowerTremolo8", 0xE8A3 },  // glyphnumber: 247, Right hand, 3 ranks, lower tremolo 8' stop
            { "accdnRH3RanksMaster", 0xE8AD },  // glyphnumber: 248, Right hand, 3 ranks, 4' stop + lower tremolo 8' stop + upper tremolo 8' stop + 16' stop (master)
            { "accdnRH3RanksOboe", 0xE8A5 },  // glyphnumber: 249, Right hand, 3 ranks, 4' stop + 8' stop (oboe)
            { "accdnRH3RanksOrgan", 0xE8A9 },  // glyphnumber: 250, Right hand, 3 ranks, 4' stop + 16' stop (organ)
            { "accdnRH3RanksPiccolo", 0xE8A0 },  // glyphnumber: 251, Right hand, 3 ranks, 4' stop (piccolo)
            { "accdnRH3RanksTremoloLower8ve", 0xE8AF },  // glyphnumber: 252, Right hand, 3 ranks, lower tremolo 8' stop + upper tremolo 8' stop + 16' stop
            { "accdnRH3RanksTremoloUpper8ve", 0xE8B0 },  // glyphnumber: 253, Right hand, 3 ranks, 4' stop + lower tremolo 8' stop + upper tremolo 8' stop
            { "accdnRH3RanksTwoChoirs", 0xE8AE },  // glyphnumber: 254, Right hand, 3 ranks, lower tremolo 8' stop + upper tremolo 8' stop
            { "accdnRH3RanksUpperTremolo8", 0xE8A2 },  // glyphnumber: 255, Right hand, 3 ranks, upper tremolo 8' stop
            { "accdnRH3RanksViolin", 0xE8A6 },  // glyphnumber: 256, Right hand, 3 ranks, 8' stop + upper tremolo 8' stop (violin)
            { "accdnRH4RanksAlto", 0xE8B5 },  // glyphnumber: 257, Right hand, 4 ranks, alto
            { "accdnRH4RanksBassAlto", 0xE8BA },  // glyphnumber: 258, Right hand, 4 ranks, bass/alto
            { "accdnRH4RanksMaster", 0xE8B7 },  // glyphnumber: 259, Right hand, 4 ranks, master
            { "accdnRH4RanksSoftBass", 0xE8B8 },  // glyphnumber: 260, Right hand, 4 ranks, soft bass
            { "accdnRH4RanksSoftTenor", 0xE8B9 },  // glyphnumber: 261, Right hand, 4 ranks, soft tenor
            { "accdnRH4RanksSoprano", 0xE8B4 },  // glyphnumber: 262, Right hand, 4 ranks, soprano
            { "accdnRH4RanksTenor", 0xE8B6 },  // glyphnumber: 263, Right hand, 4 ranks, tenor
            { "accdnRicochet2", 0xE8CD },  // glyphnumber: 264, Ricochet (2 tones)
            { "accdnRicochet3", 0xE8CE },  // glyphnumber: 265, Ricochet (3 tones)
            { "accdnRicochet4", 0xE8CF },  // glyphnumber: 266, Ricochet (4 tones)
            { "accdnRicochet5", 0xE8D0 },  // glyphnumber: 267, Ricochet (5 tones)
            { "accdnRicochet6", 0xE8D1 },  // glyphnumber: 268, Ricochet (6 tones)
            { "accdnRicochetStem2", 0xE8D2 },  // glyphnumber: 269, Combining ricochet for stem (2 tones)
            { "accdnRicochetStem3", 0xE8D3 },  // glyphnumber: 270, Combining ricochet for stem (3 tones)
            { "accdnRicochetStem4", 0xE8D4 },  // glyphnumber: 271, Combining ricochet for stem (4 tones)
            { "accdnRicochetStem5", 0xE8D5 },  // glyphnumber: 272, Combining ricochet for stem (5 tones)
            { "accdnRicochetStem6", 0xE8D6 },  // glyphnumber: 273, Combining ricochet for stem (6 tones)
            { "accidental1CommaFlat", 0xE454 },  // glyphnumber: 274, 1-comma flat
            { "accidental1CommaSharp", 0xE450 },  // glyphnumber: 275, 1-comma sharp
            { "accidental2CommaFlat", 0xE455 },  // glyphnumber: 276, 2-comma flat
            { "accidental2CommaSharp", 0xE451 },  // glyphnumber: 277, 2-comma sharp
            { "accidental3CommaFlat", 0xE456 },  // glyphnumber: 278, 3-comma flat
            { "accidental3CommaSharp", 0xE452 },  // glyphnumber: 279, 3-comma sharp
            { "accidental4CommaFlat", 0xE457 },  // glyphnumber: 280, 4-comma flat
            { "accidental5CommaSharp", 0xE453 },  // glyphnumber: 281, 5-comma sharp
            { "accidentalArrowDown", 0xE27B },  // glyphnumber: 282, Arrow down (lower by one quarter-tone)
            { "accidentalArrowUp", 0xE27A },  // glyphnumber: 283, Arrow up (raise by one quarter-tone)
            { "accidentalBakiyeFlat", 0xE442 },  // glyphnumber: 284, Bakiye (flat)
            { "accidentalBakiyeSharp", 0xE445 },  // glyphnumber: 285, Bakiye (sharp)
            { "accidentalBracketLeft", 0xE26C },  // glyphnumber: 286, Accidental bracket, left
            { "accidentalBracketRight", 0xE26D },  // glyphnumber: 287, Accidental bracket, right
            { "accidentalBuyukMucennebFlat", 0xE440 },  // glyphnumber: 288, Büyük mücenneb (flat)
            { "accidentalBuyukMucennebSharp", 0xE447 },  // glyphnumber: 289, Büyük mücenneb (sharp)
            { "accidentalCombiningCloseCurlyBrace", 0xE2EF },  // glyphnumber: 290, Combining close curly brace
            { "accidentalCombiningLower17Schisma", 0xE2E6 },  // glyphnumber: 291, Combining lower by one 17-limit schisma
            { "accidentalCombiningLower19Schisma", 0xE2E8 },  // glyphnumber: 292, Combining lower by one 19-limit schisma
            { "accidentalCombiningLower23Limit29LimitComma", 0xE2EA },  // glyphnumber: 293, Combining lower by one 23-limit comma or 29-limit comma
            { "accidentalCombiningLower31Schisma", 0xE2EC },  // glyphnumber: 294, Combining lower by one 31-limit schisma
            { "accidentalCombiningLower53LimitComma", 0xE2F7 },  // glyphnumber: 295, Combining lower by one 53-limit comma
            { "accidentalCombiningOpenCurlyBrace", 0xE2EE },  // glyphnumber: 296, Combining open curly brace
            { "accidentalCombiningRaise17Schisma", 0xE2E7 },  // glyphnumber: 297, Combining raise by one 17-limit schisma
            { "accidentalCombiningRaise19Schisma", 0xE2E9 },  // glyphnumber: 298, Combining raise by one 19-limit schisma
            { "accidentalCombiningRaise23Limit29LimitComma", 0xE2EB },  // glyphnumber: 299, Combining raise by one 23-limit comma or 29-limit comma
            { "accidentalCombiningRaise31Schisma", 0xE2ED },  // glyphnumber: 300, Combining raise by one 31-limit schisma
            { "accidentalCombiningRaise53LimitComma", 0xE2F8 },  // glyphnumber: 301, Combining raise by one 53-limit comma
            { "accidentalCommaSlashDown", 0xE47A },  // glyphnumber: 302, Syntonic/Didymus comma (80:81) down (Bosanquet)
            { "accidentalCommaSlashUp", 0xE479 },  // glyphnumber: 303, Syntonic/Didymus comma (80:81) up (Bosanquet)
            { "accidentalDoubleFlat", 0xE264 },  // glyphnumber: 304, Double flat
            { "accidentalDoubleFlatArabic", 0xED30 },  // glyphnumber: 305, Arabic double flat
            { "accidentalDoubleFlatEqualTempered", 0xE2F0 },  // glyphnumber: 306, Double flat equal tempered semitone
            { "accidentalDoubleFlatOneArrowDown", 0xE2C0 },  // glyphnumber: 307, Double flat lowered by one syntonic comma
            { "accidentalDoubleFlatOneArrowUp", 0xE2C5 },  // glyphnumber: 308, Double flat raised by one syntonic comma
            { "accidentalDoubleFlatReversed", 0xE483 },  // glyphnumber: 309, Reversed double flat
            { "accidentalDoubleFlatThreeArrowsDown", 0xE2D4 },  // glyphnumber: 310, Double flat lowered by three syntonic commas
            { "accidentalDoubleFlatThreeArrowsUp", 0xE2D9 },  // glyphnumber: 311, Double flat raised by three syntonic commas
            { "accidentalDoubleFlatTurned", 0xE485 },  // glyphnumber: 312, Turned double flat
            { "accidentalDoubleFlatTwoArrowsDown", 0xE2CA },  // glyphnumber: 313, Double flat lowered by two syntonic commas
            { "accidentalDoubleFlatTwoArrowsUp", 0xE2CF },  // glyphnumber: 314, Double flat raised by two syntonic commas
            { "accidentalDoubleSharp", 0xE263 },  // glyphnumber: 315, Double sharp
            { "accidentalDoubleSharpArabic", 0xED38 },  // glyphnumber: 316, Arabic double sharp
            { "accidentalDoubleSharpEqualTempered", 0xE2F4 },  // glyphnumber: 317, Double sharp equal tempered semitone
            { "accidentalDoubleSharpOneArrowDown", 0xE2C4 },  // glyphnumber: 318, Double sharp lowered by one syntonic comma
            { "accidentalDoubleSharpOneArrowUp", 0xE2C9 },  // glyphnumber: 319, Double sharp raised by one syntonic comma
            { "accidentalDoubleSharpThreeArrowsDown", 0xE2D8 },  // glyphnumber: 320, Double sharp lowered by three syntonic commas
            { "accidentalDoubleSharpThreeArrowsUp", 0xE2DD },  // glyphnumber: 321, Double sharp raised by three syntonic commas
            { "accidentalDoubleSharpTwoArrowsDown", 0xE2CE },  // glyphnumber: 322, Double sharp lowered by two syntonic commas
            { "accidentalDoubleSharpTwoArrowsUp", 0xE2D3 },  // glyphnumber: 323, Double sharp raised by two syntonic commas
            { "accidentalEnharmonicAlmostEqualTo", 0xE2FA },  // glyphnumber: 324, Enharmonically reinterpret accidental almost equal to
            { "accidentalEnharmonicEquals", 0xE2FB },  // glyphnumber: 325, Enharmonically reinterpret accidental equals
            { "accidentalEnharmonicTilde", 0xE2F9 },  // glyphnumber: 326, Enharmonically reinterpret accidental tilde
            { "accidentalFilledReversedFlatAndFlat", 0xE296 },  // glyphnumber: 327, Filled reversed flat and flat
            { "accidentalFilledReversedFlatAndFlatArrowDown", 0xE298 },  // glyphnumber: 328, Filled reversed flat and flat with arrow down
            { "accidentalFilledReversedFlatAndFlatArrowUp", 0xE297 },  // glyphnumber: 329, Filled reversed flat and flat with arrow up
            { "accidentalFilledReversedFlatArrowDown", 0xE293 },  // glyphnumber: 330, Filled reversed flat with arrow down
            { "accidentalFilledReversedFlatArrowUp", 0xE292 },  // glyphnumber: 331, Filled reversed flat with arrow up
            { "accidentalFiveQuarterTonesFlatArrowDown", 0xE279 },  // glyphnumber: 332, Five-quarter-tones flat
            { "accidentalFiveQuarterTonesSharpArrowUp", 0xE276 },  // glyphnumber: 333, Five-quarter-tones sharp
            { "accidentalFlat", 0xE260 },  // glyphnumber: 334, Flat
            { "accidentalFlatArabic", 0xED32 },  // glyphnumber: 335, Arabic half-tone flat
            { "accidentalFlatEqualTempered", 0xE2F1 },  // glyphnumber: 336, Flat equal tempered semitone
            { "accidentalFlatLoweredStockhausen", 0xED53 },  // glyphnumber: 337, Lowered flat (Stockhausen)
            { "accidentalFlatOneArrowDown", 0xE2C1 },  // glyphnumber: 338, Flat lowered by one syntonic comma
            { "accidentalFlatOneArrowUp", 0xE2C6 },  // glyphnumber: 339, Flat raised by one syntonic comma
            { "accidentalFlatRaisedStockhausen", 0xED52 },  // glyphnumber: 340, Raised flat (Stockhausen)
            { "accidentalFlatRepeatedLineStockhausen", 0xED5C },  // glyphnumber: 341, Repeated flat, note on line (Stockhausen)
            { "accidentalFlatRepeatedSpaceStockhausen", 0xED5B },  // glyphnumber: 342, Repeated flat, note in space (Stockhausen)
            { "accidentalFlatThreeArrowsDown", 0xE2D5 },  // glyphnumber: 343, Flat lowered by three syntonic commas
            { "accidentalFlatThreeArrowsUp", 0xE2DA },  // glyphnumber: 344, Flat raised by three syntonic commas
            { "accidentalFlatTurned", 0xE484 },  // glyphnumber: 345, Turned flat
            { "accidentalFlatTwoArrowsDown", 0xE2CB },  // glyphnumber: 346, Flat lowered by two syntonic commas
            { "accidentalFlatTwoArrowsUp", 0xE2D0 },  // glyphnumber: 347, Flat raised by two syntonic commas
            { "accidentalHalfSharpArrowDown", 0xE29A },  // glyphnumber: 348, Half sharp with arrow down
            { "accidentalHalfSharpArrowUp", 0xE299 },  // glyphnumber: 349, Half sharp with arrow up
            { "accidentalJohnston13", 0xE2B6 },  // glyphnumber: 350, Thirteen (raise by 65:64)
            { "accidentalJohnston31", 0xE2B7 },  // glyphnumber: 351, Inverted 13 (lower by 65:64)
            { "accidentalJohnstonDown", 0xE2B5 },  // glyphnumber: 352, Down arrow (lower by 33:32)
            { "accidentalJohnstonEl", 0xE2B2 },  // glyphnumber: 353, Inverted seven (raise by 36:35)
            { "accidentalJohnstonMinus", 0xE2B1 },  // glyphnumber: 354, Minus (lower by 81:80)
            { "accidentalJohnstonPlus", 0xE2B0 },  // glyphnumber: 355, Plus (raise by 81:80)
            { "accidentalJohnstonSeven", 0xE2B3 },  // glyphnumber: 356, Seven (lower by 36:35)
            { "accidentalJohnstonUp", 0xE2B4 },  // glyphnumber: 357, Up arrow (raise by 33:32)
            { "accidentalKomaFlat", 0xE443 },  // glyphnumber: 358, Koma (flat)
            { "accidentalKomaSharp", 0xE444 },  // glyphnumber: 359, Koma (sharp)
            { "accidentalKoron", 0xE460 },  // glyphnumber: 360, Koron (quarter tone flat)
            { "accidentalKucukMucennebFlat", 0xE441 },  // glyphnumber: 361, Küçük mücenneb (flat)
            { "accidentalKucukMucennebSharp", 0xE446 },  // glyphnumber: 362, Küçük mücenneb (sharp)
            { "accidentalLargeDoubleSharp", 0xE47D },  // glyphnumber: 363, Large double sharp
            { "accidentalLowerOneSeptimalComma", 0xE2DE },  // glyphnumber: 364, Lower by one septimal comma
            { "accidentalLowerOneTridecimalQuartertone", 0xE2E4 },  // glyphnumber: 365, Lower by one tridecimal quartertone
            { "accidentalLowerOneUndecimalQuartertone", 0xE2E2 },  // glyphnumber: 366, Lower by one undecimal quartertone
            { "accidentalLowerTwoSeptimalCommas", 0xE2E0 },  // glyphnumber: 367, Lower by two septimal commas
            { "accidentalLoweredStockhausen", 0xED51 },  // glyphnumber: 368, Lowered (Stockhausen)
            { "accidentalNarrowReversedFlat", 0xE284 },  // glyphnumber: 369, Narrow reversed flat(quarter-tone flat)
            { "accidentalNarrowReversedFlatAndFlat", 0xE285 },  // glyphnumber: 370, Narrow reversed flat and flat(three-quarter-tones flat)
            { "accidentalNatural", 0xE261 },  // glyphnumber: 371, Natural
            { "accidentalNaturalArabic", 0xED34 },  // glyphnumber: 372, Arabic natural
            { "accidentalNaturalEqualTempered", 0xE2F2 },  // glyphnumber: 373, Natural equal tempered semitone
            { "accidentalNaturalFlat", 0xE267 },  // glyphnumber: 374, Natural flat
            { "accidentalNaturalLoweredStockhausen", 0xED55 },  // glyphnumber: 375, Lowered natural (Stockhausen)
            { "accidentalNaturalOneArrowDown", 0xE2C2 },  // glyphnumber: 376, Natural lowered by one syntonic comma
            { "accidentalNaturalOneArrowUp", 0xE2C7 },  // glyphnumber: 377, Natural raised by one syntonic comma
            { "accidentalNaturalRaisedStockhausen", 0xED54 },  // glyphnumber: 378, Raised natural (Stockhausen)
            { "accidentalNaturalReversed", 0xE482 },  // glyphnumber: 379, Reversed natural
            { "accidentalNaturalSharp", 0xE268 },  // glyphnumber: 380, Natural sharp
            { "accidentalNaturalThreeArrowsDown", 0xE2D6 },  // glyphnumber: 381, Natural lowered by three syntonic commas
            { "accidentalNaturalThreeArrowsUp", 0xE2DB },  // glyphnumber: 382, Natural raised by three syntonic commas
            { "accidentalNaturalTwoArrowsDown", 0xE2CC },  // glyphnumber: 383, Natural lowered by two syntonic commas
            { "accidentalNaturalTwoArrowsUp", 0xE2D1 },  // glyphnumber: 384, Natural raised by two syntonic commas
            { "accidentalOneAndAHalfSharpsArrowDown", 0xE29C },  // glyphnumber: 385, One and a half sharps with arrow down
            { "accidentalOneAndAHalfSharpsArrowUp", 0xE29B },  // glyphnumber: 386, One and a half sharps with arrow up
            { "accidentalOneQuarterToneFlatFerneyhough", 0xE48F },  // glyphnumber: 387, One-quarter-tone flat (Ferneyhough)
            { "accidentalOneQuarterToneFlatStockhausen", 0xED59 },  // glyphnumber: 388, One-quarter-tone flat (Stockhausen)
            { "accidentalOneQuarterToneSharpFerneyhough", 0xE48E },  // glyphnumber: 389, One-quarter-tone sharp (Ferneyhough)
            { "accidentalOneQuarterToneSharpStockhausen", 0xED58 },  // glyphnumber: 390, One-quarter-tone sharp (Stockhausen)
            { "accidentalOneThirdToneFlatFerneyhough", 0xE48B },  // glyphnumber: 391, One-third-tone flat (Ferneyhough)
            { "accidentalOneThirdToneSharpFerneyhough", 0xE48A },  // glyphnumber: 392, One-third-tone sharp (Ferneyhough)
            { "accidentalParensLeft", 0xE26A },  // glyphnumber: 393, Accidental parenthesis, left
            { "accidentalParensRight", 0xE26B },  // glyphnumber: 394, Accidental parenthesis, right
            { "accidentalQuarterFlatEqualTempered", 0xE2F5 },  // glyphnumber: 395, Lower by one equal tempered quarter-tone
            { "accidentalQuarterSharpEqualTempered", 0xE2F6 },  // glyphnumber: 396, Raise by one equal tempered quarter tone
            { "accidentalQuarterToneFlat4", 0xE47F },  // glyphnumber: 397, Quarter-tone flat
            { "accidentalQuarterToneFlatArabic", 0xED33 },  // glyphnumber: 398, Arabic quarter-tone flat
            { "accidentalQuarterToneFlatArrowUp", 0xE270 },  // glyphnumber: 399, Quarter-tone flat
            { "accidentalQuarterToneFlatFilledReversed", 0xE480 },  // glyphnumber: 400, Filled reversed flat (quarter-tone flat)
            { "accidentalQuarterToneFlatNaturalArrowDown", 0xE273 },  // glyphnumber: 401, Quarter-tone flat
            { "accidentalQuarterToneFlatPenderecki", 0xE478 },  // glyphnumber: 402, Quarter tone flat (Penderecki)
            { "accidentalQuarterToneFlatStein", 0xE280 },  // glyphnumber: 403, Reversed flat (quarter-tone flat) (Stein)
            { "accidentalQuarterToneFlatVanBlankenburg", 0xE488 },  // glyphnumber: 404, Quarter-tone flat (van Blankenburg)
            { "accidentalQuarterToneSharp4", 0xE47E },  // glyphnumber: 405, Quarter-tone sharp
            { "accidentalQuarterToneSharpArabic", 0xED35 },  // glyphnumber: 406, Arabic quarter-tone sharp
            { "accidentalQuarterToneSharpArrowDown", 0xE275 },  // glyphnumber: 407, Quarter-tone sharp
            { "accidentalQuarterToneSharpBusotti", 0xE472 },  // glyphnumber: 408, Quarter tone sharp (Bussotti)
            { "accidentalQuarterToneSharpNaturalArrowUp", 0xE272 },  // glyphnumber: 409, Quarter-tone sharp
            { "accidentalQuarterToneSharpStein", 0xE282 },  // glyphnumber: 410, Half sharp (quarter-tone sharp) (Stein)
            { "accidentalQuarterToneSharpWiggle", 0xE475 },  // glyphnumber: 411, Quarter tone sharp with wiggly tail
            { "accidentalRaiseOneSeptimalComma", 0xE2DF },  // glyphnumber: 412, Raise by one septimal comma
            { "accidentalRaiseOneTridecimalQuartertone", 0xE2E5 },  // glyphnumber: 413, Raise by one tridecimal quartertone
            { "accidentalRaiseOneUndecimalQuartertone", 0xE2E3 },  // glyphnumber: 414, Raise by one undecimal quartertone
            { "accidentalRaiseTwoSeptimalCommas", 0xE2E1 },  // glyphnumber: 415, Raise by two septimal commas
            { "accidentalRaisedStockhausen", 0xED50 },  // glyphnumber: 416, Raised (Stockhausen)
            { "accidentalReversedFlatAndFlatArrowDown", 0xE295 },  // glyphnumber: 417, Reversed flat and flat with arrow down
            { "accidentalReversedFlatAndFlatArrowUp", 0xE294 },  // glyphnumber: 418, Reversed flat and flat with arrow up
            { "accidentalReversedFlatArrowDown", 0xE291 },  // glyphnumber: 419, Reversed flat with arrow down
            { "accidentalReversedFlatArrowUp", 0xE290 },  // glyphnumber: 420, Reversed flat with arrow up
            { "accidentalSharp", 0xE262 },  // glyphnumber: 421, Sharp
            { "accidentalSharpArabic", 0xED36 },  // glyphnumber: 422, Arabic half-tone sharp
            { "accidentalSharpEqualTempered", 0xE2F3 },  // glyphnumber: 423, Sharp equal tempered semitone
            { "accidentalSharpLoweredStockhausen", 0xED57 },  // glyphnumber: 424, Lowered sharp (Stockhausen)
            { "accidentalSharpOneArrowDown", 0xE2C3 },  // glyphnumber: 425, Sharp lowered by one syntonic comma
            { "accidentalSharpOneArrowUp", 0xE2C8 },  // glyphnumber: 426, Sharp raised by one syntonic comma
            { "accidentalSharpOneHorizontalStroke", 0xE473 },  // glyphnumber: 427, One or three quarter tones sharp
            { "accidentalSharpRaisedStockhausen", 0xED56 },  // glyphnumber: 428, Raised sharp (Stockhausen)
            { "accidentalSharpRepeatedLineStockhausen", 0xED5E },  // glyphnumber: 429, Repeated sharp, note on line (Stockhausen)
            { "accidentalSharpRepeatedSpaceStockhausen", 0xED5D },  // glyphnumber: 430, Repeated sharp, note in space (Stockhausen)
            { "accidentalSharpReversed", 0xE481 },  // glyphnumber: 431, Reversed sharp
            { "accidentalSharpSharp", 0xE269 },  // glyphnumber: 432, Sharp sharp
            { "accidentalSharpThreeArrowsDown", 0xE2D7 },  // glyphnumber: 433, Sharp lowered by three syntonic commas
            { "accidentalSharpThreeArrowsUp", 0xE2DC },  // glyphnumber: 434, Sharp raised by three syntonic commas
            { "accidentalSharpTwoArrowsDown", 0xE2CD },  // glyphnumber: 435, Sharp lowered by two syntonic commas
            { "accidentalSharpTwoArrowsUp", 0xE2D2 },  // glyphnumber: 436, Sharp raised by two syntonic commas
            { "accidentalSims12Down", 0xE2A0 },  // glyphnumber: 437, 1/12 tone low
            { "accidentalSims12Up", 0xE2A3 },  // glyphnumber: 438, 1/12 tone high
            { "accidentalSims4Down", 0xE2A2 },  // glyphnumber: 439, 1/4 tone low
            { "accidentalSims4Up", 0xE2A5 },  // glyphnumber: 440, 1/4 tone high
            { "accidentalSims6Down", 0xE2A1 },  // glyphnumber: 441, 1/6 tone low
            { "accidentalSims6Up", 0xE2A4 },  // glyphnumber: 442, 1/6 tone high
            { "accidentalSori", 0xE461 },  // glyphnumber: 443, Sori (quarter tone sharp)
            { "accidentalTavenerFlat", 0xE477 },  // glyphnumber: 444, Byzantine-style Bakiye flat (Tavener)
            { "accidentalTavenerSharp", 0xE476 },  // glyphnumber: 445, Byzantine-style Büyük mücenneb sharp (Tavener)
            { "accidentalThreeQuarterTonesFlatArabic", 0xED31 },  // glyphnumber: 446, Arabic three-quarter-tones flat
            { "accidentalThreeQuarterTonesFlatArrowDown", 0xE271 },  // glyphnumber: 447, Three-quarter-tones flat
            { "accidentalThreeQuarterTonesFlatArrowUp", 0xE278 },  // glyphnumber: 448, Three-quarter-tones flat
            { "accidentalThreeQuarterTonesFlatCouper", 0xE489 },  // glyphnumber: 449, Three-quarter-tones flat (Couper)
            { "accidentalThreeQuarterTonesFlatGrisey", 0xE486 },  // glyphnumber: 450, Three-quarter-tones flat (Grisey)
            { "accidentalThreeQuarterTonesFlatTartini", 0xE487 },  // glyphnumber: 451, Three-quarter-tones flat (Tartini)
            { "accidentalThreeQuarterTonesFlatZimmermann", 0xE281 },  // glyphnumber: 452, Reversed flat and flat (three-quarter-tones flat)(Zimmermann)
            { "accidentalThreeQuarterTonesSharpArabic", 0xED37 },  // glyphnumber: 453, Arabic three-quarter-tones sharp
            { "accidentalThreeQuarterTonesSharpArrowDown", 0xE277 },  // glyphnumber: 454, Three-quarter-tones sharp
            { "accidentalThreeQuarterTonesSharpArrowUp", 0xE274 },  // glyphnumber: 455, Three-quarter-tones sharp
            { "accidentalThreeQuarterTonesSharpBusotti", 0xE474 },  // glyphnumber: 456, Three quarter tones sharp (Bussotti)
            { "accidentalThreeQuarterTonesSharpStein", 0xE283 },  // glyphnumber: 457, One and a half sharps (three-quarter-tones sharp) (Stein)
            { "accidentalThreeQuarterTonesSharpStockhausen", 0xED5A },  // glyphnumber: 458, Three-quarter-tones sharp (Stockhausen)
            { "accidentalTripleFlat", 0xE266 },  // glyphnumber: 459, Triple flat
            { "accidentalTripleSharp", 0xE265 },  // glyphnumber: 460, Triple sharp
            { "accidentalTwoThirdTonesFlatFerneyhough", 0xE48D },  // glyphnumber: 461, Two-third-tones flat (Ferneyhough)
            { "accidentalTwoThirdTonesSharpFerneyhough", 0xE48C },  // glyphnumber: 462, Two-third-tones sharp (Ferneyhough)
            { "accidentalWilsonMinus", 0xE47C },  // glyphnumber: 463, Wilson minus (5 comma down)
            { "accidentalWilsonPlus", 0xE47B },  // glyphnumber: 464, Wilson plus (5 comma up)
            { "accidentalWyschnegradsky10TwelfthsFlat", 0xE434 },  // glyphnumber: 465, 5/6 tone flat
            { "accidentalWyschnegradsky10TwelfthsSharp", 0xE429 },  // glyphnumber: 466, 5/6 tone sharp
            { "accidentalWyschnegradsky11TwelfthsFlat", 0xE435 },  // glyphnumber: 467, 11/12 tone flat
            { "accidentalWyschnegradsky11TwelfthsSharp", 0xE42A },  // glyphnumber: 468, 11/12 tone sharp
            { "accidentalWyschnegradsky1TwelfthsFlat", 0xE42B },  // glyphnumber: 469, 1/12 tone flat
            { "accidentalWyschnegradsky1TwelfthsSharp", 0xE420 },  // glyphnumber: 470, 1/12 tone sharp
            { "accidentalWyschnegradsky2TwelfthsFlat", 0xE42C },  // glyphnumber: 471, 1/6 tone flat
            { "accidentalWyschnegradsky2TwelfthsSharp", 0xE421 },  // glyphnumber: 472, 1/6 tone sharp
            { "accidentalWyschnegradsky3TwelfthsFlat", 0xE42D },  // glyphnumber: 473, 1/4 tone flat
            { "accidentalWyschnegradsky3TwelfthsSharp", 0xE422 },  // glyphnumber: 474, 1/4 tone sharp
            { "accidentalWyschnegradsky4TwelfthsFlat", 0xE42E },  // glyphnumber: 475, 1/3 tone flat
            { "accidentalWyschnegradsky4TwelfthsSharp", 0xE423 },  // glyphnumber: 476, 1/3 tone sharp
            { "accidentalWyschnegradsky5TwelfthsFlat", 0xE42F },  // glyphnumber: 477, 5/12 tone flat
            { "accidentalWyschnegradsky5TwelfthsSharp", 0xE424 },  // glyphnumber: 478, 5/12 tone sharp
            { "accidentalWyschnegradsky6TwelfthsFlat", 0xE430 },  // glyphnumber: 479, 1/2 tone flat
            { "accidentalWyschnegradsky6TwelfthsSharp", 0xE425 },  // glyphnumber: 480, 1/2 tone sharp
            { "accidentalWyschnegradsky7TwelfthsFlat", 0xE431 },  // glyphnumber: 481, 7/12 tone flat
            { "accidentalWyschnegradsky7TwelfthsSharp", 0xE426 },  // glyphnumber: 482, 7/12 tone sharp
            { "accidentalWyschnegradsky8TwelfthsFlat", 0xE432 },  // glyphnumber: 483, 2/3 tone flat
            { "accidentalWyschnegradsky8TwelfthsSharp", 0xE427 },  // glyphnumber: 484, 2/3 tone sharp
            { "accidentalWyschnegradsky9TwelfthsFlat", 0xE433 },  // glyphnumber: 485, 3/4 tone flat
            { "accidentalWyschnegradsky9TwelfthsSharp", 0xE428 },  // glyphnumber: 486, 3/4 tone sharp
            { "accidentalXenakisOneThirdToneSharp", 0xE470 },  // glyphnumber: 487, One-third-tone sharp (Xenakis)
            { "accidentalXenakisTwoThirdTonesSharp", 0xE471 },  // glyphnumber: 488, Two-third-tones sharp (Xenakis)
            { "analyticsChoralmelodie", 0xE86A },  // glyphnumber: 489, Choralmelodie (Berg)
            { "analyticsEndStimme", 0xE863 },  // glyphnumber: 490, End of stimme
            { "analyticsHauptrhythmus", 0xE86B },  // glyphnumber: 491, Hauptrhythmus (Berg)
            { "analyticsHauptstimme", 0xE860 },  // glyphnumber: 492, Hauptstimme
            { "analyticsInversion1", 0xE869 },  // glyphnumber: 493, Inversion 1
            { "analyticsNebenstimme", 0xE861 },  // glyphnumber: 494, Nebenstimme
            { "analyticsStartStimme", 0xE862 },  // glyphnumber: 495, Start of stimme
            { "analyticsTheme", 0xE864 },  // glyphnumber: 496, Theme
            { "analyticsTheme1", 0xE868 },  // glyphnumber: 497, Theme 1
            { "analyticsThemeInversion", 0xE867 },  // glyphnumber: 498, Inversion of theme
            { "analyticsThemeRetrograde", 0xE865 },  // glyphnumber: 499, Retrograde of theme
            { "analyticsThemeRetrogradeInversion", 0xE866 },  // glyphnumber: 500, Retrograde inversion of theme
            { "arpeggiatoDown", 0xE635 },  // glyphnumber: 501, Arpeggiato down
            { "arpeggiatoUp", 0xE634 },  // glyphnumber: 502, Arpeggiato up
            { "arrowBlackDown", 0xEB64 },  // glyphnumber: 503, Black arrow down (S)
            { "arrowBlackDownLeft", 0xEB65 },  // glyphnumber: 504, Black arrow down-left (SW)
            { "arrowBlackDownRight", 0xEB63 },  // glyphnumber: 505, Black arrow down-right (SE)
            { "arrowBlackLeft", 0xEB66 },  // glyphnumber: 506, Black arrow left (W)
            { "arrowBlackRight", 0xEB62 },  // glyphnumber: 507, Black arrow right (E)
            { "arrowBlackUp", 0xEB60 },  // glyphnumber: 508, Black arrow up (N)
            { "arrowBlackUpLeft", 0xEB67 },  // glyphnumber: 509, Black arrow up-left (NW)
            { "arrowBlackUpRight", 0xEB61 },  // glyphnumber: 510, Black arrow up-right (NE)
            { "arrowOpenDown", 0xEB74 },  // glyphnumber: 511, Open arrow down (S)
            { "arrowOpenDownLeft", 0xEB75 },  // glyphnumber: 512, Open arrow down-left (SW)
            { "arrowOpenDownRight", 0xEB73 },  // glyphnumber: 513, Open arrow down-right (SE)
            { "arrowOpenLeft", 0xEB76 },  // glyphnumber: 514, Open arrow left (W)
            { "arrowOpenRight", 0xEB72 },  // glyphnumber: 515, Open arrow right (E)
            { "arrowOpenUp", 0xEB70 },  // glyphnumber: 516, Open arrow up (N)
            { "arrowOpenUpLeft", 0xEB77 },  // glyphnumber: 517, Open arrow up-left (NW)
            { "arrowOpenUpRight", 0xEB71 },  // glyphnumber: 518, Open arrow up-right (NE)
            { "arrowWhiteDown", 0xEB6C },  // glyphnumber: 519, White arrow down (S)
            { "arrowWhiteDownLeft", 0xEB6D },  // glyphnumber: 520, White arrow down-left (SW)
            { "arrowWhiteDownRight", 0xEB6B },  // glyphnumber: 521, White arrow down-right (SE)
            { "arrowWhiteLeft", 0xEB6E },  // glyphnumber: 522, White arrow left (W)
            { "arrowWhiteRight", 0xEB6A },  // glyphnumber: 523, White arrow right (E)
            { "arrowWhiteUp", 0xEB68 },  // glyphnumber: 524, White arrow up (N)
            { "arrowWhiteUpLeft", 0xEB6F },  // glyphnumber: 525, White arrow up-left (NW)
            { "arrowWhiteUpRight", 0xEB69 },  // glyphnumber: 526, White arrow up-right (NE)
            { "arrowheadBlackDown", 0xEB7C },  // glyphnumber: 527, Black arrowhead down (S)
            { "arrowheadBlackDownLeft", 0xEB7D },  // glyphnumber: 528, Black arrowhead down-left (SW)
            { "arrowheadBlackDownRight", 0xEB7B },  // glyphnumber: 529, Black arrowhead down-right (SE)
            { "arrowheadBlackLeft", 0xEB7E },  // glyphnumber: 530, Black arrowhead left (W)
            { "arrowheadBlackRight", 0xEB7A },  // glyphnumber: 531, Black arrowhead right (E)
            { "arrowheadBlackUp", 0xEB78 },  // glyphnumber: 532, Black arrowhead up (N)
            { "arrowheadBlackUpLeft", 0xEB7F },  // glyphnumber: 533, Black arrowhead up-left (NW)
            { "arrowheadBlackUpRight", 0xEB79 },  // glyphnumber: 534, Black arrowhead up-right (NE)
            { "arrowheadOpenDown", 0xEB8C },  // glyphnumber: 535, Open arrowhead down (S)
            { "arrowheadOpenDownLeft", 0xEB8D },  // glyphnumber: 536, Open arrowhead down-left (SW)
            { "arrowheadOpenDownRight", 0xEB8B },  // glyphnumber: 537, Open arrowhead down-right (SE)
            { "arrowheadOpenLeft", 0xEB8E },  // glyphnumber: 538, Open arrowhead left (W)
            { "arrowheadOpenRight", 0xEB8A },  // glyphnumber: 539, Open arrowhead right (E)
            { "arrowheadOpenUp", 0xEB88 },  // glyphnumber: 540, Open arrowhead up (N)
            { "arrowheadOpenUpLeft", 0xEB8F },  // glyphnumber: 541, Open arrowhead up-left (NW)
            { "arrowheadOpenUpRight", 0xEB89 },  // glyphnumber: 542, Open arrowhead up-right (NE)
            { "arrowheadWhiteDown", 0xEB84 },  // glyphnumber: 543, White arrowhead down (S)
            { "arrowheadWhiteDownLeft", 0xEB85 },  // glyphnumber: 544, White arrowhead down-left (SW)
            { "arrowheadWhiteDownRight", 0xEB83 },  // glyphnumber: 545, White arrowhead down-right (SE)
            { "arrowheadWhiteLeft", 0xEB86 },  // glyphnumber: 546, White arrowhead left (W)
            { "arrowheadWhiteRight", 0xEB82 },  // glyphnumber: 547, White arrowhead right (E)
            { "arrowheadWhiteUp", 0xEB80 },  // glyphnumber: 548, White arrowhead up (N)
            { "arrowheadWhiteUpLeft", 0xEB87 },  // glyphnumber: 549, White arrowhead up-left (NW)
            { "arrowheadWhiteUpRight", 0xEB81 },  // glyphnumber: 550, White arrowhead up-right (NE)
            { "articAccentAbove", 0xE4A0 },  // glyphnumber: 551, Accent above
            { "articAccentBelow", 0xE4A1 },  // glyphnumber: 552, Accent below
            { "articAccentStaccatoAbove", 0xE4B0 },  // glyphnumber: 553, Accent-staccato above
            { "articAccentStaccatoBelow", 0xE4B1 },  // glyphnumber: 554, Accent-staccato below
            { "articLaissezVibrerAbove", 0xE4BA },  // glyphnumber: 555, Laissez vibrer (l.v.) above
            { "articLaissezVibrerBelow", 0xE4BB },  // glyphnumber: 556, Laissez vibrer (l.v.) below
            { "articMarcatoAbove", 0xE4AC },  // glyphnumber: 557, Marcato above
            { "articMarcatoBelow", 0xE4AD },  // glyphnumber: 558, Marcato below
            { "articMarcatoStaccatoAbove", 0xE4AE },  // glyphnumber: 559, Marcato-staccato above
            { "articMarcatoStaccatoBelow", 0xE4AF },  // glyphnumber: 560, Marcato-staccato below
            { "articMarcatoTenutoAbove", 0xE4BC },  // glyphnumber: 561, Marcato-tenuto above
            { "articMarcatoTenutoBelow", 0xE4BD },  // glyphnumber: 562, Marcato-tenuto below
            { "articSoftAccentAbove", 0xED40 },  // glyphnumber: 563, Soft accent above
            { "articSoftAccentBelow", 0xED41 },  // glyphnumber: 564, Soft accent below
            { "articSoftAccentStaccatoAbove", 0xED42 },  // glyphnumber: 565, Soft accent-staccato above
            { "articSoftAccentStaccatoBelow", 0xED43 },  // glyphnumber: 566, Soft accent-staccato below
            { "articSoftAccentTenutoAbove", 0xED44 },  // glyphnumber: 567, Soft accent-tenuto above
            { "articSoftAccentTenutoBelow", 0xED45 },  // glyphnumber: 568, Soft accent-tenuto below
            { "articSoftAccentTenutoStaccatoAbove", 0xED46 },  // glyphnumber: 569, Soft accent-tenuto-staccato above
            { "articSoftAccentTenutoStaccatoBelow", 0xED47 },  // glyphnumber: 570, Soft accent-tenuto-staccato below
            { "articStaccatissimoAbove", 0xE4A6 },  // glyphnumber: 571, Staccatissimo above
            { "articStaccatissimoBelow", 0xE4A7 },  // glyphnumber: 572, Staccatissimo below
            { "articStaccatissimoStrokeAbove", 0xE4AA },  // glyphnumber: 573, Staccatissimo stroke above
            { "articStaccatissimoStrokeBelow", 0xE4AB },  // glyphnumber: 574, Staccatissimo stroke below
            { "articStaccatissimoWedgeAbove", 0xE4A8 },  // glyphnumber: 575, Staccatissimo wedge above
            { "articStaccatissimoWedgeBelow", 0xE4A9 },  // glyphnumber: 576, Staccatissimo wedge below
            { "articStaccatoAbove", 0xE4A2 },  // glyphnumber: 577, Staccato above
            { "articStaccatoBelow", 0xE4A3 },  // glyphnumber: 578, Staccato below
            { "articStressAbove", 0xE4B6 },  // glyphnumber: 579, Stress above
            { "articStressBelow", 0xE4B7 },  // glyphnumber: 580, Stress below
            { "articTenutoAbove", 0xE4A4 },  // glyphnumber: 581, Tenuto above
            { "articTenutoAccentAbove", 0xE4B4 },  // glyphnumber: 582, Tenuto-accent above
            { "articTenutoAccentBelow", 0xE4B5 },  // glyphnumber: 583, Tenuto-accent below
            { "articTenutoBelow", 0xE4A5 },  // glyphnumber: 584, Tenuto below
            { "articTenutoStaccatoAbove", 0xE4B2 },  // glyphnumber: 585, Louré (tenuto-staccato) above
            { "articTenutoStaccatoBelow", 0xE4B3 },  // glyphnumber: 586, Louré (tenuto-staccato) below
            { "articUnstressAbove", 0xE4B8 },  // glyphnumber: 587, Unstress above
            { "articUnstressBelow", 0xE4B9 },  // glyphnumber: 588, Unstress below
            { "augmentationDot", 0xE1E7 },  // glyphnumber: 589, Augmentation dot
            { "barlineDashed", 0xE036 },  // glyphnumber: 590, Dashed barline
            { "barlineDotted", 0xE037 },  // glyphnumber: 591, Dotted barline
            { "barlineDouble", 0xE031 },  // glyphnumber: 592, Double barline
            { "barlineFinal", 0xE032 },  // glyphnumber: 593, Final barline
            { "barlineHeavy", 0xE034 },  // glyphnumber: 594, Heavy barline
            { "barlineHeavyHeavy", 0xE035 },  // glyphnumber: 595, Heavy double barline
            { "barlineReverseFinal", 0xE033 },  // glyphnumber: 596, Reverse final barline
            { "barlineShort", 0xE038 },  // glyphnumber: 597, Short barline
            { "barlineSingle", 0xE030 },  // glyphnumber: 598, Single barline
            { "barlineTick", 0xE039 },  // glyphnumber: 599, Tick barline
            { "beamAccelRit1", 0xEAF4 },  // glyphnumber: 600, Accel./rit. beam 1 (widest)
            { "beamAccelRit10", 0xEAFD },  // glyphnumber: 601, Accel./rit. beam 10
            { "beamAccelRit11", 0xEAFE },  // glyphnumber: 602, Accel./rit. beam 11
            { "beamAccelRit12", 0xEAFF },  // glyphnumber: 603, Accel./rit. beam 12
            { "beamAccelRit13", 0xEB00 },  // glyphnumber: 604, Accel./rit. beam 13
            { "beamAccelRit14", 0xEB01 },  // glyphnumber: 605, Accel./rit. beam 14
            { "beamAccelRit15", 0xEB02 },  // glyphnumber: 606, Accel./rit. beam 15 (narrowest)
            { "beamAccelRit2", 0xEAF5 },  // glyphnumber: 607, Accel./rit. beam 2
            { "beamAccelRit3", 0xEAF6 },  // glyphnumber: 608, Accel./rit. beam 3
            { "beamAccelRit4", 0xEAF7 },  // glyphnumber: 609, Accel./rit. beam 4
            { "beamAccelRit5", 0xEAF8 },  // glyphnumber: 610, Accel./rit. beam 5
            { "beamAccelRit6", 0xEAF9 },  // glyphnumber: 611, Accel./rit. beam 6
            { "beamAccelRit7", 0xEAFA },  // glyphnumber: 612, Accel./rit. beam 7
            { "beamAccelRit8", 0xEAFB },  // glyphnumber: 613, Accel./rit. beam 8
            { "beamAccelRit9", 0xEAFC },  // glyphnumber: 614, Accel./rit. beam 9
            { "beamAccelRitFinal", 0xEB03 },  // glyphnumber: 615, Accel./rit. beam terminating line
            { "brace", 0xE000 },  // glyphnumber: 616, Brace
            { "bracket", 0xE002 },  // glyphnumber: 617, Bracket
            { "bracketBottom", 0xE004 },  // glyphnumber: 618, Bracket bottom
            { "bracketTop", 0xE003 },  // glyphnumber: 619, Bracket top
            { "brassBend", 0xE5E3 },  // glyphnumber: 620, Bend
            { "brassDoitLong", 0xE5D6 },  // glyphnumber: 621, Doit, long
            { "brassDoitMedium", 0xE5D5 },  // glyphnumber: 622, Doit, medium
            { "brassDoitShort", 0xE5D4 },  // glyphnumber: 623, Doit, short
            { "brassFallLipLong", 0xE5D9 },  // glyphnumber: 624, Lip fall, long
            { "brassFallLipMedium", 0xE5D8 },  // glyphnumber: 625, Lip fall, medium
            { "brassFallLipShort", 0xE5D7 },  // glyphnumber: 626, Lip fall, short
            { "brassFallRoughLong", 0xE5DF },  // glyphnumber: 627, Rough fall, long
            { "brassFallRoughMedium", 0xE5DE },  // glyphnumber: 628, Rough fall, medium
            { "brassFallRoughShort", 0xE5DD },  // glyphnumber: 629, Rough fall, short
            { "brassFallSmoothLong", 0xE5DC },  // glyphnumber: 630, Smooth fall, long
            { "brassFallSmoothMedium", 0xE5DB },  // glyphnumber: 631, Smooth fall, medium
            { "brassFallSmoothShort", 0xE5DA },  // glyphnumber: 632, Smooth fall, short
            { "brassFlip", 0xE5E1 },  // glyphnumber: 633, Flip
            { "brassHarmonMuteClosed", 0xE5E8 },  // glyphnumber: 634, Harmon mute, stem in
            { "brassHarmonMuteStemHalfLeft", 0xE5E9 },  // glyphnumber: 635, Harmon mute, stem extended, left
            { "brassHarmonMuteStemHalfRight", 0xE5EA },  // glyphnumber: 636, Harmon mute, stem extended, right
            { "brassHarmonMuteStemOpen", 0xE5EB },  // glyphnumber: 637, Harmon mute, stem out
            { "brassJazzTurn", 0xE5E4 },  // glyphnumber: 638, Jazz turn
            { "brassLiftLong", 0xE5D3 },  // glyphnumber: 639, Lift, long
            { "brassLiftMedium", 0xE5D2 },  // glyphnumber: 640, Lift, medium
            { "brassLiftShort", 0xE5D1 },  // glyphnumber: 641, Lift, short
            { "brassLiftSmoothLong", 0xE5EE },  // glyphnumber: 642, Smooth lift, long
            { "brassLiftSmoothMedium", 0xE5ED },  // glyphnumber: 643, Smooth lift, medium
            { "brassLiftSmoothShort", 0xE5EC },  // glyphnumber: 644, Smooth lift, short
            { "brassMuteClosed", 0xE5E5 },  // glyphnumber: 645, Muted (closed)
            { "brassMuteHalfClosed", 0xE5E6 },  // glyphnumber: 646, Half-muted (half-closed)
            { "brassMuteOpen", 0xE5E7 },  // glyphnumber: 647, Open
            { "brassPlop", 0xE5E0 },  // glyphnumber: 648, Plop
            { "brassScoop", 0xE5D0 },  // glyphnumber: 649, Scoop
            { "brassSmear", 0xE5E2 },  // glyphnumber: 650, Smear
            { "brassValveTrill", 0xE5EF },  // glyphnumber: 651, Valve trill
            { "breathMarkComma", 0xE4CE },  // glyphnumber: 652, Breath mark (comma)
            { "breathMarkSalzedo", 0xE4D5 },  // glyphnumber: 653, Breath mark (Salzedo)
            { "breathMarkTick", 0xE4CF },  // glyphnumber: 654, Breath mark (tick-like)
            { "breathMarkUpbow", 0xE4D0 },  // glyphnumber: 655, Breath mark (upbow-like)
            { "bridgeClef", 0xE078 },  // glyphnumber: 656, Bridge clef
            { "buzzRoll", 0xE22A },  // glyphnumber: 657, Buzz roll
            { "cClef", 0xE05C },  // glyphnumber: 658, C clef
            { "cClef8vb", 0xE05D },  // glyphnumber: 659, C clef ottava bassa
            { "cClefArrowDown", 0xE05F },  // glyphnumber: 660, C clef, arrow down
            { "cClefArrowUp", 0xE05E },  // glyphnumber: 661, C clef, arrow up
            { "cClefChange", 0xE07B },  // glyphnumber: 662, C clef change
            { "cClefCombining", 0xE061 },  // glyphnumber: 663, Combining C clef
            { "cClefReversed", 0xE075 },  // glyphnumber: 664, Reversed C clef
            { "cClefSquare", 0xE060 },  // glyphnumber: 665, C clef (19th century)
            { "caesura", 0xE4D1 },  // glyphnumber: 666, Caesura
            { "caesuraCurved", 0xE4D4 },  // glyphnumber: 667, Curved caesura
            { "caesuraShort", 0xE4D3 },  // glyphnumber: 668, Short caesura
            { "caesuraThick", 0xE4D2 },  // glyphnumber: 669, Thick caesura
            { "chantAccentusAbove", 0xE9D6 },  // glyphnumber: 670, Accentus above
            { "chantAccentusBelow", 0xE9D7 },  // glyphnumber: 671, Accentus below
            { "chantAuctumAsc", 0xE994 },  // glyphnumber: 672, Punctum auctum, ascending
            { "chantAuctumDesc", 0xE995 },  // glyphnumber: 673, Punctum auctum, descending
            { "chantAugmentum", 0xE9D9 },  // glyphnumber: 674, Augmentum (mora)
            { "chantCaesura", 0xE8F8 },  // glyphnumber: 675, Caesura
            { "chantCclef", 0xE906 },  // glyphnumber: 676, Plainchant C clef
            { "chantCirculusAbove", 0xE9D2 },  // glyphnumber: 677, Circulus above
            { "chantCirculusBelow", 0xE9D3 },  // glyphnumber: 678, Circulus below
            { "chantConnectingLineAsc2nd", 0xE9BD },  // glyphnumber: 679, Connecting line, ascending 2nd
            { "chantConnectingLineAsc3rd", 0xE9BE },  // glyphnumber: 680, Connecting line, ascending 3rd
            { "chantConnectingLineAsc4th", 0xE9BF },  // glyphnumber: 681, Connecting line, ascending 4th
            { "chantConnectingLineAsc5th", 0xE9C0 },  // glyphnumber: 682, Connecting line, ascending 5th
            { "chantConnectingLineAsc6th", 0xE9C1 },  // glyphnumber: 683, Connecting line, ascending 6th
            { "chantCustosStemDownPosHigh", 0xEA08 },  // glyphnumber: 684, Plainchant custos, stem down, high position
            { "chantCustosStemDownPosHighest", 0xEA09 },  // glyphnumber: 685, Plainchant custos, stem down, highest position
            { "chantCustosStemDownPosMiddle", 0xEA07 },  // glyphnumber: 686, Plainchant custos, stem down, middle position
            { "chantCustosStemUpPosLow", 0xEA05 },  // glyphnumber: 687, Plainchant custos, stem up, low position
            { "chantCustosStemUpPosLowest", 0xEA04 },  // glyphnumber: 688, Plainchant custos, stem up, lowest position
            { "chantCustosStemUpPosMiddle", 0xEA06 },  // glyphnumber: 689, Plainchant custos, stem up, middle position
            { "chantDeminutumLower", 0xE9B3 },  // glyphnumber: 690, Punctum deminutum, lower
            { "chantDeminutumUpper", 0xE9B2 },  // glyphnumber: 691, Punctum deminutum, upper
            { "chantDivisioFinalis", 0xE8F6 },  // glyphnumber: 692, Divisio finalis
            { "chantDivisioMaior", 0xE8F4 },  // glyphnumber: 693, Divisio maior
            { "chantDivisioMaxima", 0xE8F5 },  // glyphnumber: 694, Divisio maxima
            { "chantDivisioMinima", 0xE8F3 },  // glyphnumber: 695, Divisio minima
            { "chantEntryLineAsc2nd", 0xE9B4 },  // glyphnumber: 696, Entry line, ascending 2nd
            { "chantEntryLineAsc3rd", 0xE9B5 },  // glyphnumber: 697, Entry line, ascending 3rd
            { "chantEntryLineAsc4th", 0xE9B6 },  // glyphnumber: 698, Entry line, ascending 4th
            { "chantEntryLineAsc5th", 0xE9B7 },  // glyphnumber: 699, Entry line, ascending 5th
            { "chantEntryLineAsc6th", 0xE9B8 },  // glyphnumber: 700, Entry line, ascending 6th
            { "chantEpisema", 0xE9D8 },  // glyphnumber: 701, Episema
            { "chantFclef", 0xE902 },  // glyphnumber: 702, Plainchant F clef
            { "chantIctusAbove", 0xE9D0 },  // glyphnumber: 703, Ictus above
            { "chantIctusBelow", 0xE9D1 },  // glyphnumber: 704, Ictus below
            { "chantLigaturaDesc2nd", 0xE9B9 },  // glyphnumber: 705, Ligated stroke, descending 2nd
            { "chantLigaturaDesc3rd", 0xE9BA },  // glyphnumber: 706, Ligated stroke, descending 3rd
            { "chantLigaturaDesc4th", 0xE9BB },  // glyphnumber: 707, Ligated stroke, descending 4th
            { "chantLigaturaDesc5th", 0xE9BC },  // glyphnumber: 708, Ligated stroke, descending 5th
            { "chantOriscusAscending", 0xE99C },  // glyphnumber: 709, Oriscus ascending
            { "chantOriscusDescending", 0xE99D },  // glyphnumber: 710, Oriscus descending
            { "chantOriscusLiquescens", 0xE99E },  // glyphnumber: 711, Oriscus liquescens
            { "chantPodatusLower", 0xE9B0 },  // glyphnumber: 712, Podatus, lower
            { "chantPodatusUpper", 0xE9B1 },  // glyphnumber: 713, Podatus, upper
            { "chantPunctum", 0xE990 },  // glyphnumber: 714, Punctum
            { "chantPunctumCavum", 0xE998 },  // glyphnumber: 715, Punctum cavum
            { "chantPunctumDeminutum", 0xE9A1 },  // glyphnumber: 716, Punctum deminutum
            { "chantPunctumInclinatum", 0xE991 },  // glyphnumber: 717, Punctum inclinatum
            { "chantPunctumInclinatumAuctum", 0xE992 },  // glyphnumber: 718, Punctum inclinatum auctum
            { "chantPunctumInclinatumDeminutum", 0xE993 },  // glyphnumber: 719, Punctum inclinatum deminutum
            { "chantPunctumLinea", 0xE999 },  // glyphnumber: 720, Punctum linea
            { "chantPunctumLineaCavum", 0xE99A },  // glyphnumber: 721, Punctum linea cavum
            { "chantPunctumVirga", 0xE996 },  // glyphnumber: 722, Punctum virga
            { "chantPunctumVirgaReversed", 0xE997 },  // glyphnumber: 723, Punctum virga, reversed
            { "chantQuilisma", 0xE99B },  // glyphnumber: 724, Quilisma
            { "chantSemicirculusAbove", 0xE9D4 },  // glyphnumber: 725, Semicirculus above
            { "chantSemicirculusBelow", 0xE9D5 },  // glyphnumber: 726, Semicirculus below
            { "chantStaff", 0xE8F0 },  // glyphnumber: 727, Plainchant staff
            { "chantStaffNarrow", 0xE8F2 },  // glyphnumber: 728, Plainchant staff (narrow)
            { "chantStaffWide", 0xE8F1 },  // glyphnumber: 729, Plainchant staff (wide)
            { "chantStrophicus", 0xE99F },  // glyphnumber: 730, Strophicus
            { "chantStrophicusAuctus", 0xE9A0 },  // glyphnumber: 731, Strophicus auctus
            { "chantStrophicusLiquescens2nd", 0xE9C2 },  // glyphnumber: 732, Strophicus liquescens, 2nd
            { "chantStrophicusLiquescens3rd", 0xE9C3 },  // glyphnumber: 733, Strophicus liquescens, 3rd
            { "chantStrophicusLiquescens4th", 0xE9C4 },  // glyphnumber: 734, Strophicus liquescens, 4th
            { "chantStrophicusLiquescens5th", 0xE9C5 },  // glyphnumber: 735, Strophicus liquescens, 5th
            { "chantVirgula", 0xE8F7 },  // glyphnumber: 736, Virgula
            { "clef15", 0xE07E },  // glyphnumber: 737, 15 for clefs
            { "clef8", 0xE07D },  // glyphnumber: 738, 8 for clefs
            { "clefChangeCombining", 0xE07F },  // glyphnumber: 739, Combining clef change
            { "coda", 0xE048 },  // glyphnumber: 740, Coda
            { "codaSquare", 0xE049 },  // glyphnumber: 741, Square coda
            { "conductorBeat2Compound", 0xE897 },  // glyphnumber: 742, Beat 2, compound time
            { "conductorBeat2Simple", 0xE894 },  // glyphnumber: 743, Beat 2, simple time
            { "conductorBeat3Compound", 0xE898 },  // glyphnumber: 744, Beat 3, compound time
            { "conductorBeat3Simple", 0xE895 },  // glyphnumber: 745, Beat 3, simple time
            { "conductorBeat4Compound", 0xE899 },  // glyphnumber: 746, Beat 4, compound time
            { "conductorBeat4Simple", 0xE896 },  // glyphnumber: 747, Beat 4, simple time
            { "conductorLeftBeat", 0xE891 },  // glyphnumber: 748, Left-hand beat or cue
            { "conductorRightBeat", 0xE892 },  // glyphnumber: 749, Right-hand beat or cue
            { "conductorStrongBeat", 0xE890 },  // glyphnumber: 750, Strong beat or cue
            { "conductorUnconducted", 0xE89A },  // glyphnumber: 751, Unconducted/free passages
            { "conductorWeakBeat", 0xE893 },  // glyphnumber: 752, Weak beat or cue
            { "controlBeginBeam", 0xE8E0 },  // glyphnumber: 753, Begin beam
            { "controlBeginPhrase", 0xE8E6 },  // glyphnumber: 754, Begin phrase
            { "controlBeginSlur", 0xE8E4 },  // glyphnumber: 755, Begin slur
            { "controlBeginTie", 0xE8E2 },  // glyphnumber: 756, Begin tie
            { "controlEndBeam", 0xE8E1 },  // glyphnumber: 757, End beam
            { "controlEndPhrase", 0xE8E7 },  // glyphnumber: 758, End phrase
            { "controlEndSlur", 0xE8E5 },  // glyphnumber: 759, End slur
            { "controlEndTie", 0xE8E3 },  // glyphnumber: 760, End tie
            { "csymAugmented", 0xE872 },  // glyphnumber: 761, Augmented
            { "csymBracketLeftTall", 0xE877 },  // glyphnumber: 762, Double-height left bracket
            { "csymBracketRightTall", 0xE878 },  // glyphnumber: 763, Double-height right bracket
            { "csymDiminished", 0xE870 },  // glyphnumber: 764, Diminished
            { "csymHalfDiminished", 0xE871 },  // glyphnumber: 765, Half-diminished
            { "csymMajorSeventh", 0xE873 },  // glyphnumber: 766, Major seventh
            { "csymMinor", 0xE874 },  // glyphnumber: 767, Minor
            { "csymParensLeftTall", 0xE875 },  // glyphnumber: 768, Double-height left parenthesis
            { "csymParensRightTall", 0xE876 },  // glyphnumber: 769, Double-height right parenthesis
            { "curlewSign", 0xE4D6 },  // glyphnumber: 770, Curlew (Britten)
            { "daCapo", 0xE046 },  // glyphnumber: 771, Da capo
            { "dalSegno", 0xE045 },  // glyphnumber: 772, Dal segno
            { "daseianExcellentes1", 0xEA3C },  // glyphnumber: 773, Daseian excellentes 1
            { "daseianExcellentes2", 0xEA3D },  // glyphnumber: 774, Daseian excellentes 2
            { "daseianExcellentes3", 0xEA3E },  // glyphnumber: 775, Daseian excellentes 3
            { "daseianExcellentes4", 0xEA3F },  // glyphnumber: 776, Daseian excellentes 4
            { "daseianFinales1", 0xEA34 },  // glyphnumber: 777, Daseian finales 1
            { "daseianFinales2", 0xEA35 },  // glyphnumber: 778, Daseian finales 2
            { "daseianFinales3", 0xEA36 },  // glyphnumber: 779, Daseian finales 3
            { "daseianFinales4", 0xEA37 },  // glyphnumber: 780, Daseian finales 4
            { "daseianGraves1", 0xEA30 },  // glyphnumber: 781, Daseian graves 1
            { "daseianGraves2", 0xEA31 },  // glyphnumber: 782, Daseian graves 2
            { "daseianGraves3", 0xEA32 },  // glyphnumber: 783, Daseian graves 3
            { "daseianGraves4", 0xEA33 },  // glyphnumber: 784, Daseian graves 4
            { "daseianResidua1", 0xEA40 },  // glyphnumber: 785, Daseian residua 1
            { "daseianResidua2", 0xEA41 },  // glyphnumber: 786, Daseian residua 2
            { "daseianSuperiores1", 0xEA38 },  // glyphnumber: 787, Daseian superiores 1
            { "daseianSuperiores2", 0xEA39 },  // glyphnumber: 788, Daseian superiores 2
            { "daseianSuperiores3", 0xEA3A },  // glyphnumber: 789, Daseian superiores 3
            { "daseianSuperiores4", 0xEA3B },  // glyphnumber: 790, Daseian superiores 4
            { "doubleTongueAbove", 0xE5F0 },  // glyphnumber: 791, Double-tongue above
            { "doubleTongueBelow", 0xE5F1 },  // glyphnumber: 792, Double-tongue below
            { "dynamicCombinedSeparatorColon", 0xE546 },  // glyphnumber: 793, Colon separator for combined dynamics
            { "dynamicCombinedSeparatorHyphen", 0xE547 },  // glyphnumber: 794, Hyphen separator for combined dynamics
            { "dynamicCombinedSeparatorSpace", 0xE548 },  // glyphnumber: 795, Space separator for combined dynamics
            { "dynamicCrescendoHairpin", 0xE53E },  // glyphnumber: 796, Crescendo
            { "dynamicDiminuendoHairpin", 0xE53F },  // glyphnumber: 797, Diminuendo
            { "dynamicFF", 0xE52F },  // glyphnumber: 798, ff
            { "dynamicFFF", 0xE530 },  // glyphnumber: 799, fff
            { "dynamicFFFF", 0xE531 },  // glyphnumber: 800, ffff
            { "dynamicFFFFF", 0xE532 },  // glyphnumber: 801, fffff
            { "dynamicFFFFFF", 0xE533 },  // glyphnumber: 802, ffffff
            { "dynamicForte", 0xE522 },  // glyphnumber: 803, Forte
            { "dynamicFortePiano", 0xE534 },  // glyphnumber: 804, Forte-piano
            { "dynamicForzando", 0xE535 },  // glyphnumber: 805, Forzando
            { "dynamicHairpinBracketLeft", 0xE544 },  // glyphnumber: 806, Left bracket (for hairpins)
            { "dynamicHairpinBracketRight", 0xE545 },  // glyphnumber: 807, Right bracket (for hairpins)
            { "dynamicHairpinParenthesisLeft", 0xE542 },  // glyphnumber: 808, Left parenthesis (for hairpins)
            { "dynamicHairpinParenthesisRight", 0xE543 },  // glyphnumber: 809, Right parenthesis (for hairpins)
            { "dynamicMF", 0xE52D },  // glyphnumber: 810, mf
            { "dynamicMP", 0xE52C },  // glyphnumber: 811, mp
            { "dynamicMessaDiVoce", 0xE540 },  // glyphnumber: 812, Messa di voce
            { "dynamicMezzo", 0xE521 },  // glyphnumber: 813, Mezzo
            { "dynamicNiente", 0xE526 },  // glyphnumber: 814, Niente
            { "dynamicNienteForHairpin", 0xE541 },  // glyphnumber: 815, Niente (for hairpins)
            { "dynamicPF", 0xE52E },  // glyphnumber: 816, pf
            { "dynamicPP", 0xE52B },  // glyphnumber: 817, pp
            { "dynamicPPP", 0xE52A },  // glyphnumber: 818, ppp
            { "dynamicPPPP", 0xE529 },  // glyphnumber: 819, pppp
            { "dynamicPPPPP", 0xE528 },  // glyphnumber: 820, ppppp
            { "dynamicPPPPPP", 0xE527 },  // glyphnumber: 821, pppppp
            { "dynamicPiano", 0xE520 },  // glyphnumber: 822, Piano
            { "dynamicRinforzando", 0xE523 },  // glyphnumber: 823, Rinforzando
            { "dynamicRinforzando1", 0xE53C },  // glyphnumber: 824, Rinforzando 1
            { "dynamicRinforzando2", 0xE53D },  // glyphnumber: 825, Rinforzando 2
            { "dynamicSforzando", 0xE524 },  // glyphnumber: 826, Sforzando
            { "dynamicSforzando1", 0xE536 },  // glyphnumber: 827, Sforzando 1
            { "dynamicSforzandoPianissimo", 0xE538 },  // glyphnumber: 828, Sforzando-pianissimo
            { "dynamicSforzandoPiano", 0xE537 },  // glyphnumber: 829, Sforzando-piano
            { "dynamicSforzato", 0xE539 },  // glyphnumber: 830, Sforzato
            { "dynamicSforzatoFF", 0xE53B },  // glyphnumber: 831, Sforzatissimo
            { "dynamicSforzatoPiano", 0xE53A },  // glyphnumber: 832, Sforzato-piano
            { "dynamicZ", 0xE525 },  // glyphnumber: 833, Z
            { "elecAudioChannelsEight", 0xEB46 },  // glyphnumber: 834, Eight channels (7.1 surround)
            { "elecAudioChannelsFive", 0xEB43 },  // glyphnumber: 835, Five channels
            { "elecAudioChannelsFour", 0xEB42 },  // glyphnumber: 836, Four channels
            { "elecAudioChannelsOne", 0xEB3E },  // glyphnumber: 837, One channel (mono)
            { "elecAudioChannelsSeven", 0xEB45 },  // glyphnumber: 838, Seven channels
            { "elecAudioChannelsSix", 0xEB44 },  // glyphnumber: 839, Six channels (5.1 surround)
            { "elecAudioChannelsThreeFrontal", 0xEB40 },  // glyphnumber: 840, Three channels (frontal)
            { "elecAudioChannelsThreeSurround", 0xEB41 },  // glyphnumber: 841, Three channels (surround)
            { "elecAudioChannelsTwo", 0xEB3F },  // glyphnumber: 842, Two channels (stereo)
            { "elecAudioIn", 0xEB49 },  // glyphnumber: 843, Audio in
            { "elecAudioMono", 0xEB3C },  // glyphnumber: 844, Mono audio setup
            { "elecAudioOut", 0xEB4A },  // glyphnumber: 845, Audio out
            { "elecAudioStereo", 0xEB3D },  // glyphnumber: 846, Stereo audio setup
            { "elecCamera", 0xEB1B },  // glyphnumber: 847, Camera
            { "elecDataIn", 0xEB4D },  // glyphnumber: 848, Data in
            { "elecDataOut", 0xEB4E },  // glyphnumber: 849, Data out
            { "elecDisc", 0xEB13 },  // glyphnumber: 850, Disc
            { "elecDownload", 0xEB4F },  // glyphnumber: 851, Download
            { "elecEject", 0xEB2B },  // glyphnumber: 852, Eject
            { "elecFastForward", 0xEB1F },  // glyphnumber: 853, Fast-forward
            { "elecHeadphones", 0xEB11 },  // glyphnumber: 854, Headphones
            { "elecHeadset", 0xEB12 },  // glyphnumber: 855, Headset
            { "elecLineIn", 0xEB47 },  // glyphnumber: 856, Line in
            { "elecLineOut", 0xEB48 },  // glyphnumber: 857, Line out
            { "elecLoop", 0xEB23 },  // glyphnumber: 858, Loop
            { "elecLoudspeaker", 0xEB1A },  // glyphnumber: 859, Loudspeaker
            { "elecMIDIController0", 0xEB36 },  // glyphnumber: 860, MIDI controller 0%
            { "elecMIDIController100", 0xEB3B },  // glyphnumber: 861, MIDI controller 100%
            { "elecMIDIController20", 0xEB37 },  // glyphnumber: 862, MIDI controller 20%
            { "elecMIDIController40", 0xEB38 },  // glyphnumber: 863, MIDI controller 40%
            { "elecMIDIController60", 0xEB39 },  // glyphnumber: 864, MIDI controller 60%
            { "elecMIDIController80", 0xEB3A },  // glyphnumber: 865, MIDI controller 80%
            { "elecMIDIIn", 0xEB34 },  // glyphnumber: 866, MIDI in
            { "elecMIDIOut", 0xEB35 },  // glyphnumber: 867, MIDI out
            { "elecMicrophone", 0xEB10 },  // glyphnumber: 868, Microphone
            { "elecMicrophoneMute", 0xEB28 },  // glyphnumber: 869, Mute microphone
            { "elecMicrophoneUnmute", 0xEB29 },  // glyphnumber: 870, Unmute microphone
            { "elecMixingConsole", 0xEB15 },  // glyphnumber: 871, Mixing console
            { "elecMonitor", 0xEB18 },  // glyphnumber: 872, Monitor
            { "elecMute", 0xEB26 },  // glyphnumber: 873, Mute
            { "elecPause", 0xEB1E },  // glyphnumber: 874, Pause
            { "elecPlay", 0xEB1C },  // glyphnumber: 875, Play
            { "elecPowerOnOff", 0xEB2A },  // glyphnumber: 876, Power on/off
            { "elecProjector", 0xEB19 },  // glyphnumber: 877, Projector
            { "elecReplay", 0xEB24 },  // glyphnumber: 878, Replay
            { "elecRewind", 0xEB20 },  // glyphnumber: 879, Rewind
            { "elecShuffle", 0xEB25 },  // glyphnumber: 880, Shuffle
            { "elecSkipBackwards", 0xEB22 },  // glyphnumber: 881, Skip backwards
            { "elecSkipForwards", 0xEB21 },  // glyphnumber: 882, Skip forwards
            { "elecStop", 0xEB1D },  // glyphnumber: 883, Stop
            { "elecTape", 0xEB14 },  // glyphnumber: 884, Tape
            { "elecUSB", 0xEB16 },  // glyphnumber: 885, USB connection
            { "elecUnmute", 0xEB27 },  // glyphnumber: 886, Unmute
            { "elecUpload", 0xEB50 },  // glyphnumber: 887, Upload
            { "elecVideoCamera", 0xEB17 },  // glyphnumber: 888, Video camera
            { "elecVideoIn", 0xEB4B },  // glyphnumber: 889, Video in
            { "elecVideoOut", 0xEB4C },  // glyphnumber: 890, Video out
            { "elecVolumeFader", 0xEB2C },  // glyphnumber: 891, Combining volume fader
            { "elecVolumeFaderThumb", 0xEB2D },  // glyphnumber: 892, Combining volume fader thumb
            { "elecVolumeLevel0", 0xEB2E },  // glyphnumber: 893, Volume level 0%
            { "elecVolumeLevel100", 0xEB33 },  // glyphnumber: 894, Volume level 100%
            { "elecVolumeLevel20", 0xEB2F },  // glyphnumber: 895, Volume level 20%
            { "elecVolumeLevel40", 0xEB30 },  // glyphnumber: 896, Volume level 40%
            { "elecVolumeLevel60", 0xEB31 },  // glyphnumber: 897, Volume level 60%
            { "elecVolumeLevel80", 0xEB32 },  // glyphnumber: 898, Volume level 80%
            { "fClef", 0xE062 },  // glyphnumber: 899, F clef
            { "fClef15ma", 0xE066 },  // glyphnumber: 900, F clef quindicesima alta
            { "fClef15mb", 0xE063 },  // glyphnumber: 901, F clef quindicesima bassa
            { "fClef8va", 0xE065 },  // glyphnumber: 902, F clef ottava alta
            { "fClef8vb", 0xE064 },  // glyphnumber: 903, F clef ottava bassa
            { "fClefArrowDown", 0xE068 },  // glyphnumber: 904, F clef, arrow down
            { "fClefArrowUp", 0xE067 },  // glyphnumber: 905, F clef, arrow up
            { "fClefChange", 0xE07C },  // glyphnumber: 906, F clef change
            { "fClefReversed", 0xE076 },  // glyphnumber: 907, Reversed F clef
            { "fClefTurned", 0xE077 },  // glyphnumber: 908, Turned F clef
            { "fermataAbove", 0xE4C0 },  // glyphnumber: 909, Fermata above
            { "fermataBelow", 0xE4C1 },  // glyphnumber: 910, Fermata below
            { "fermataLongAbove", 0xE4C6 },  // glyphnumber: 911, Long fermata above
            { "fermataLongBelow", 0xE4C7 },  // glyphnumber: 912, Long fermata below
            { "fermataLongHenzeAbove", 0xE4CA },  // glyphnumber: 913, Long fermata (Henze) above
            { "fermataLongHenzeBelow", 0xE4CB },  // glyphnumber: 914, Long fermata (Henze) below
            { "fermataShortAbove", 0xE4C4 },  // glyphnumber: 915, Short fermata above
            { "fermataShortBelow", 0xE4C5 },  // glyphnumber: 916, Short fermata below
            { "fermataShortHenzeAbove", 0xE4CC },  // glyphnumber: 917, Short fermata (Henze) above
            { "fermataShortHenzeBelow", 0xE4CD },  // glyphnumber: 918, Short fermata (Henze) below
            { "fermataVeryLongAbove", 0xE4C8 },  // glyphnumber: 919, Very long fermata above
            { "fermataVeryLongBelow", 0xE4C9 },  // glyphnumber: 920, Very long fermata below
            { "fermataVeryShortAbove", 0xE4C2 },  // glyphnumber: 921, Very short fermata above
            { "fermataVeryShortBelow", 0xE4C3 },  // glyphnumber: 922, Very short fermata below
            { "figbass0", 0xEA50 },  // glyphnumber: 923, Figured bass 0
            { "figbass1", 0xEA51 },  // glyphnumber: 924, Figured bass 1
            { "figbass2", 0xEA52 },  // glyphnumber: 925, Figured bass 2
            { "figbass2Raised", 0xEA53 },  // glyphnumber: 926, Figured bass 2 raised by half-step
            { "figbass3", 0xEA54 },  // glyphnumber: 927, Figured bass 3
            { "figbass4", 0xEA55 },  // glyphnumber: 928, Figured bass 4
            { "figbass4Raised", 0xEA56 },  // glyphnumber: 929, Figured bass 4 raised by half-step
            { "figbass5", 0xEA57 },  // glyphnumber: 930, Figured bass 5
            { "figbass5Raised1", 0xEA58 },  // glyphnumber: 931, Figured bass 5 raised by half-step
            { "figbass5Raised2", 0xEA59 },  // glyphnumber: 932, Figured bass 5 raised by half-step 2
            { "figbass5Raised3", 0xEA5A },  // glyphnumber: 933, Figured bass diminished 5
            { "figbass6", 0xEA5B },  // glyphnumber: 934, Figured bass 6
            { "figbass6Raised", 0xEA5C },  // glyphnumber: 935, Figured bass 6 raised by half-step
            { "figbass6Raised2", 0xEA6F },  // glyphnumber: 936, Figured bass 6 raised by half-step 2
            { "figbass7", 0xEA5D },  // glyphnumber: 937, Figured bass 7
            { "figbass7Diminished", 0xECC0 },  // glyphnumber: 938, Figured bass 7 diminished
            { "figbass7Raised1", 0xEA5E },  // glyphnumber: 939, Figured bass 7 raised by half-step
            { "figbass7Raised2", 0xEA5F },  // glyphnumber: 940, Figured bass 7 raised by a half-step 2
            { "figbass8", 0xEA60 },  // glyphnumber: 941, Figured bass 8
            { "figbass9", 0xEA61 },  // glyphnumber: 942, Figured bass 9
            { "figbass9Raised", 0xEA62 },  // glyphnumber: 943, Figured bass 9 raised by half-step
            { "figbassBracketLeft", 0xEA68 },  // glyphnumber: 944, Figured bass [
            { "figbassBracketRight", 0xEA69 },  // glyphnumber: 945, Figured bass ]
            { "figbassCombiningLowering", 0xEA6E },  // glyphnumber: 946, Combining lower
            { "figbassCombiningRaising", 0xEA6D },  // glyphnumber: 947, Combining raise
            { "figbassDoubleFlat", 0xEA63 },  // glyphnumber: 948, Figured bass double flat
            { "figbassDoubleSharp", 0xEA67 },  // glyphnumber: 949, Figured bass double sharp
            { "figbassFlat", 0xEA64 },  // glyphnumber: 950, Figured bass flat
            { "figbassNatural", 0xEA65 },  // glyphnumber: 951, Figured bass natural
            { "figbassParensLeft", 0xEA6A },  // glyphnumber: 952, Figured bass (
            { "figbassParensRight", 0xEA6B },  // glyphnumber: 953, Figured bass )
            { "figbassPlus", 0xEA6C },  // glyphnumber: 954, Figured bass +
            { "figbassSharp", 0xEA66 },  // glyphnumber: 955, Figured bass sharp
            { "fingering0", 0xED10 },  // glyphnumber: 956, Fingering 0 (open string)
            { "fingering1", 0xED11 },  // glyphnumber: 957, Fingering 1 (thumb)
            { "fingering2", 0xED12 },  // glyphnumber: 958, Fingering 2 (index finger)
            { "fingering3", 0xED13 },  // glyphnumber: 959, Fingering 3 (middle finger)
            { "fingering4", 0xED14 },  // glyphnumber: 960, Fingering 4 (ring finger)
            { "fingering5", 0xED15 },  // glyphnumber: 961, Fingering 5 (little finger)
            { "fingeringALower", 0xED1B },  // glyphnumber: 962, Fingering a (anular; right-hand ring finger for guitar)
            { "fingeringCLower", 0xED1C },  // glyphnumber: 963, Fingering c (right-hand little finger for guitar)
            { "fingeringELower", 0xED1E },  // glyphnumber: 964, Fingering e (right-hand little finger for guitar)
            { "fingeringILower", 0xED19 },  // glyphnumber: 965, Fingering i (indicio; right-hand index finger for guitar)
            { "fingeringMLower", 0xED1A },  // glyphnumber: 966, Fingering m (medio; right-hand middle finger for guitar)
            { "fingeringMultipleNotes", 0xED23 },  // glyphnumber: 967, Multiple notes played by thumb or single finger
            { "fingeringOLower", 0xED1F },  // glyphnumber: 968, Fingering o (right-hand little finger for guitar)
            { "fingeringPLower", 0xED17 },  // glyphnumber: 969, Fingering p (pulgar; right-hand thumb for guitar)
            { "fingeringSubstitutionAbove", 0xED20 },  // glyphnumber: 970, Finger substitution above
            { "fingeringSubstitutionBelow", 0xED21 },  // glyphnumber: 971, Finger substitution below
            { "fingeringSubstitutionDash", 0xED22 },  // glyphnumber: 972, Finger substitution dash
            { "fingeringTLower", 0xED18 },  // glyphnumber: 973, Fingering t (right-hand thumb for guitar)
            { "fingeringTUpper", 0xED16 },  // glyphnumber: 974, Fingering T (left-hand thumb for guitar)
            { "fingeringXLower", 0xED1D },  // glyphnumber: 975, Fingering x (right-hand little finger for guitar)
            { "flag1024thDown", 0xE24F },  // glyphnumber: 976, Combining flag 8 (1024th) below
            { "flag1024thUp", 0xE24E },  // glyphnumber: 977, Combining flag 8 (1024th) above
            { "flag128thDown", 0xE249 },  // glyphnumber: 978, Combining flag 5 (128th) below
            { "flag128thUp", 0xE248 },  // glyphnumber: 979, Combining flag 5 (128th) above
            { "flag16thDown", 0xE243 },  // glyphnumber: 980, Combining flag 2 (16th) below
            { "flag16thUp", 0xE242 },  // glyphnumber: 981, Combining flag 2 (16th) above
            { "flag256thDown", 0xE24B },  // glyphnumber: 982, Combining flag 6 (256th) below
            { "flag256thUp", 0xE24A },  // glyphnumber: 983, Combining flag 6 (256th) above
            { "flag32ndDown", 0xE245 },  // glyphnumber: 984, Combining flag 3 (32nd) below
            { "flag32ndUp", 0xE244 },  // glyphnumber: 985, Combining flag 3 (32nd) above
            { "flag512thDown", 0xE24D },  // glyphnumber: 986, Combining flag 7 (512th) below
            { "flag512thUp", 0xE24C },  // glyphnumber: 987, Combining flag 7 (512th) above
            { "flag64thDown", 0xE247 },  // glyphnumber: 988, Combining flag 4 (64th) below
            { "flag64thUp", 0xE246 },  // glyphnumber: 989, Combining flag 4 (64th) above
            { "flag8thDown", 0xE241 },  // glyphnumber: 990, Combining flag 1 (8th) below
            { "flag8thUp", 0xE240 },  // glyphnumber: 991, Combining flag 1 (8th) above
            { "flagInternalDown", 0xE251 },  // glyphnumber: 992, Internal combining flag below
            { "flagInternalUp", 0xE250 },  // glyphnumber: 993, Internal combining flag above
            { "fretboard3String", 0xE850 },  // glyphnumber: 994, 3-string fretboard
            { "fretboard3StringNut", 0xE851 },  // glyphnumber: 995, 3-string fretboard at nut
            { "fretboard4String", 0xE852 },  // glyphnumber: 996, 4-string fretboard
            { "fretboard4StringNut", 0xE853 },  // glyphnumber: 997, 4-string fretboard at nut
            { "fretboard5String", 0xE854 },  // glyphnumber: 998, 5-string fretboard
            { "fretboard5StringNut", 0xE855 },  // glyphnumber: 999, 5-string fretboard at nut
            { "fretboard6String", 0xE856 },  // glyphnumber: 1000, 6-string fretboard
            { "fretboard6StringNut", 0xE857 },  // glyphnumber: 1001, 6-string fretboard at nut
            { "fretboardFilledCircle", 0xE858 },  // glyphnumber: 1002, Fingered fret (filled circle)
            { "fretboardO", 0xE85A },  // glyphnumber: 1003, Open string (O)
            { "fretboardX", 0xE859 },  // glyphnumber: 1004, String not played (X)
            { "functionAngleLeft", 0xEA93 },  // glyphnumber: 1005, Function theory angle bracket left
            { "functionAngleRight", 0xEA94 },  // glyphnumber: 1006, Function theory angle bracket right
            { "functionBracketLeft", 0xEA8F },  // glyphnumber: 1007, Function theory bracket left
            { "functionBracketRight", 0xEA90 },  // glyphnumber: 1008, Function theory bracket right
            { "functionDD", 0xEA81 },  // glyphnumber: 1009, Function theory dominant of dominant
            { "functionDLower", 0xEA80 },  // glyphnumber: 1010, Function theory minor dominant
            { "functionDUpper", 0xEA7F },  // glyphnumber: 1011, Function theory major dominant
            { "functionEight", 0xEA78 },  // glyphnumber: 1012, Function theory 8
            { "functionFUpper", 0xEA99 },  // glyphnumber: 1013, Function theory F
            { "functionFive", 0xEA75 },  // glyphnumber: 1014, Function theory 5
            { "functionFour", 0xEA74 },  // glyphnumber: 1015, Function theory 4
            { "functionGLower", 0xEA84 },  // glyphnumber: 1016, Function theory g
            { "functionGUpper", 0xEA83 },  // glyphnumber: 1017, Function theory G
            { "functionGreaterThan", 0xEA7C },  // glyphnumber: 1018, Function theory greater than
            { "functionILower", 0xEA9B },  // glyphnumber: 1019, Function theory i
            { "functionIUpper", 0xEA9A },  // glyphnumber: 1020, Function theory I
            { "functionKLower", 0xEA9D },  // glyphnumber: 1021, Function theory k
            { "functionKUpper", 0xEA9C },  // glyphnumber: 1022, Function theory K
            { "functionLLower", 0xEA9F },  // glyphnumber: 1023, Function theory l
            { "functionLUpper", 0xEA9E },  // glyphnumber: 1024, Function theory L
            { "functionLessThan", 0xEA7A },  // glyphnumber: 1025, Function theory less than
            { "functionMLower", 0xED01 },  // glyphnumber: 1026, Function theory m
            { "functionMUpper", 0xED00 },  // glyphnumber: 1027, Function theory M
            { "functionMinus", 0xEA7B },  // glyphnumber: 1028, Function theory minus
            { "functionNLower", 0xEA86 },  // glyphnumber: 1029, Function theory n
            { "functionNUpper", 0xEA85 },  // glyphnumber: 1030, Function theory N
            { "functionNUpperSuperscript", 0xED02 },  // glyphnumber: 1031, Function theory superscript N
            { "functionNine", 0xEA79 },  // glyphnumber: 1032, Function theory 9
            { "functionOne", 0xEA71 },  // glyphnumber: 1033, Function theory 1
            { "functionPLower", 0xEA88 },  // glyphnumber: 1034, Function theory p
            { "functionPUpper", 0xEA87 },  // glyphnumber: 1035, Function theory P
            { "functionParensLeft", 0xEA91 },  // glyphnumber: 1036, Function theory parenthesis left
            { "functionParensRight", 0xEA92 },  // glyphnumber: 1037, Function theory parenthesis right
            { "functionPlus", 0xEA98 },  // glyphnumber: 1038, Function theory prefix plus
            { "functionRLower", 0xED03 },  // glyphnumber: 1039, Function theory r
            { "functionRepetition1", 0xEA95 },  // glyphnumber: 1040, Function theory repetition 1
            { "functionRepetition2", 0xEA96 },  // glyphnumber: 1041, Function theory repetition 2
            { "functionRing", 0xEA97 },  // glyphnumber: 1042, Function theory prefix ring
            { "functionSLower", 0xEA8A },  // glyphnumber: 1043, Function theory minor subdominant
            { "functionSSLower", 0xEA7E },  // glyphnumber: 1044, Function theory minor subdominant of subdominant
            { "functionSSUpper", 0xEA7D },  // glyphnumber: 1045, Function theory major subdominant of subdominant
            { "functionSUpper", 0xEA89 },  // glyphnumber: 1046, Function theory major subdominant
            { "functionSeven", 0xEA77 },  // glyphnumber: 1047, Function theory 7
            { "functionSix", 0xEA76 },  // glyphnumber: 1048, Function theory 6
            { "functionSlashedDD", 0xEA82 },  // glyphnumber: 1049, Function theory double dominant seventh
            { "functionTLower", 0xEA8C },  // glyphnumber: 1050, Function theory minor tonic
            { "functionTUpper", 0xEA8B },  // glyphnumber: 1051, Function theory tonic
            { "functionThree", 0xEA73 },  // glyphnumber: 1052, Function theory 3
            { "functionTwo", 0xEA72 },  // glyphnumber: 1053, Function theory 2
            { "functionVLower", 0xEA8E },  // glyphnumber: 1054, Function theory v
            { "functionVUpper", 0xEA8D },  // glyphnumber: 1055, Function theory V
            { "functionZero", 0xEA70 },  // glyphnumber: 1056, Function theory 0
            { "gClef", 0xE050 },  // glyphnumber: 1057, G clef
            { "gClef15ma", 0xE054 },  // glyphnumber: 1058, G clef quindicesima alta
            { "gClef15mb", 0xE051 },  // glyphnumber: 1059, G clef quindicesima bassa
            { "gClef8va", 0xE053 },  // glyphnumber: 1060, G clef ottava alta
            { "gClef8vb", 0xE052 },  // glyphnumber: 1061, G clef ottava bassa
            { "gClef8vbCClef", 0xE056 },  // glyphnumber: 1062, G clef ottava bassa with C clef
            { "gClef8vbOld", 0xE055 },  // glyphnumber: 1063, G clef ottava bassa (old style)
            { "gClef8vbParens", 0xE057 },  // glyphnumber: 1064, G clef, optionally ottava bassa
            { "gClefArrowDown", 0xE05B },  // glyphnumber: 1065, G clef, arrow down
            { "gClefArrowUp", 0xE05A },  // glyphnumber: 1066, G clef, arrow up
            { "gClefChange", 0xE07A },  // glyphnumber: 1067, G clef change
            { "gClefLigatedNumberAbove", 0xE059 },  // glyphnumber: 1068, Combining G clef, number above
            { "gClefLigatedNumberBelow", 0xE058 },  // glyphnumber: 1069, Combining G clef, number below
            { "gClefReversed", 0xE073 },  // glyphnumber: 1070, Reversed G clef
            { "gClefTurned", 0xE074 },  // glyphnumber: 1071, Turned G clef
            { "glissandoDown", 0xE586 },  // glyphnumber: 1072, Glissando down
            { "glissandoUp", 0xE585 },  // glyphnumber: 1073, Glissando up
            { "graceNoteAcciaccaturaStemDown", 0xE561 },  // glyphnumber: 1074, Slashed grace note stem down
            { "graceNoteAcciaccaturaStemUp", 0xE560 },  // glyphnumber: 1075, Slashed grace note stem up
            { "graceNoteAppoggiaturaStemDown", 0xE563 },  // glyphnumber: 1076, Grace note stem down
            { "graceNoteAppoggiaturaStemUp", 0xE562 },  // glyphnumber: 1077, Grace note stem up
            { "graceNoteSlashStemDown", 0xE565 },  // glyphnumber: 1078, Slash for stem down grace note
            { "graceNoteSlashStemUp", 0xE564 },  // glyphnumber: 1079, Slash for stem up grace note
            { "guitarBarreFull", 0xE848 },  // glyphnumber: 1080, Full barré
            { "guitarBarreHalf", 0xE849 },  // glyphnumber: 1081, Half barré
            { "guitarClosePedal", 0xE83F },  // glyphnumber: 1082, Closed wah/volume pedal
            { "guitarFadeIn", 0xE843 },  // glyphnumber: 1083, Fade in
            { "guitarFadeOut", 0xE844 },  // glyphnumber: 1084, Fade out
            { "guitarGolpe", 0xE842 },  // glyphnumber: 1085, Golpe (tapping the pick guard)
            { "guitarHalfOpenPedal", 0xE83E },  // glyphnumber: 1086, Half-open wah/volume pedal
            { "guitarLeftHandTapping", 0xE840 },  // glyphnumber: 1087, Left-hand tapping
            { "guitarOpenPedal", 0xE83D },  // glyphnumber: 1088, Open wah/volume pedal
            { "guitarRightHandTapping", 0xE841 },  // glyphnumber: 1089, Right-hand tapping
            { "guitarShake", 0xE832 },  // glyphnumber: 1090, Guitar shake
            { "guitarString0", 0xE833 },  // glyphnumber: 1091, String number 0
            { "guitarString1", 0xE834 },  // glyphnumber: 1092, String number 1
            { "guitarString2", 0xE835 },  // glyphnumber: 1093, String number 2
            { "guitarString3", 0xE836 },  // glyphnumber: 1094, String number 3
            { "guitarString4", 0xE837 },  // glyphnumber: 1095, String number 4
            { "guitarString5", 0xE838 },  // glyphnumber: 1096, String number 5
            { "guitarString6", 0xE839 },  // glyphnumber: 1097, String number 6
            { "guitarString7", 0xE83A },  // glyphnumber: 1098, String number 7
            { "guitarString8", 0xE83B },  // glyphnumber: 1099, String number 8
            { "guitarString9", 0xE83C },  // glyphnumber: 1100, String number 9
            { "guitarStrumDown", 0xE847 },  // glyphnumber: 1101, Strum direction down
            { "guitarStrumUp", 0xE846 },  // glyphnumber: 1102, Strum direction up
            { "guitarVibratoBarDip", 0xE831 },  // glyphnumber: 1103, Guitar vibrato bar dip
            { "guitarVibratoBarScoop", 0xE830 },  // glyphnumber: 1104, Guitar vibrato bar scoop
            { "guitarVibratoStroke", 0xEAB2 },  // glyphnumber: 1105, Vibrato wiggle segment
            { "guitarVolumeSwell", 0xE845 },  // glyphnumber: 1106, Volume swell
            { "guitarWideVibratoStroke", 0xEAB3 },  // glyphnumber: 1107, Wide vibrato wiggle segment
            { "handbellsBelltree", 0xE81F },  // glyphnumber: 1108, Belltree
            { "handbellsDamp3", 0xE81E },  // glyphnumber: 1109, Damp 3
            { "handbellsEcho1", 0xE81B },  // glyphnumber: 1110, Echo
            { "handbellsEcho2", 0xE81C },  // glyphnumber: 1111, Echo 2
            { "handbellsGyro", 0xE81D },  // glyphnumber: 1112, Gyro
            { "handbellsHandMartellato", 0xE812 },  // glyphnumber: 1113, Hand martellato
            { "handbellsMalletBellOnTable", 0xE815 },  // glyphnumber: 1114, Mallet, bell on table
            { "handbellsMalletBellSuspended", 0xE814 },  // glyphnumber: 1115, Mallet, bell suspended
            { "handbellsMalletLft", 0xE816 },  // glyphnumber: 1116, Mallet lift
            { "handbellsMartellato", 0xE810 },  // glyphnumber: 1117, Martellato
            { "handbellsMartellatoLift", 0xE811 },  // glyphnumber: 1118, Martellato lift
            { "handbellsMutedMartellato", 0xE813 },  // glyphnumber: 1119, Muted martellato
            { "handbellsPluckLift", 0xE817 },  // glyphnumber: 1120, Pluck lift
            { "handbellsSwing", 0xE81A },  // glyphnumber: 1121, Swing
            { "handbellsSwingDown", 0xE819 },  // glyphnumber: 1122, Swing down
            { "handbellsSwingUp", 0xE818 },  // glyphnumber: 1123, Swing up
            { "handbellsTablePairBells", 0xE821 },  // glyphnumber: 1124, Table pair of handbells
            { "handbellsTableSingleBell", 0xE820 },  // glyphnumber: 1125, Table single handbell
            { "harpMetalRod", 0xE68F },  // glyphnumber: 1126, Metal rod pictogram
            { "harpPedalCentered", 0xE681 },  // glyphnumber: 1127, Harp pedal centered (natural)
            { "harpPedalDivider", 0xE683 },  // glyphnumber: 1128, Harp pedal divider
            { "harpPedalLowered", 0xE682 },  // glyphnumber: 1129, Harp pedal lowered (sharp)
            { "harpPedalRaised", 0xE680 },  // glyphnumber: 1130, Harp pedal raised (flat)
            { "harpSalzedoAeolianAscending", 0xE695 },  // glyphnumber: 1131, Ascending aeolian chords (Salzedo)
            { "harpSalzedoAeolianDescending", 0xE696 },  // glyphnumber: 1132, Descending aeolian chords (Salzedo)
            { "harpSalzedoDampAbove", 0xE69A },  // glyphnumber: 1133, Damp above (Salzedo)
            { "harpSalzedoDampBelow", 0xE699 },  // glyphnumber: 1134, Damp below (Salzedo)
            { "harpSalzedoDampBothHands", 0xE698 },  // glyphnumber: 1135, Damp with both hands (Salzedo)
            { "harpSalzedoDampLowStrings", 0xE697 },  // glyphnumber: 1136, Damp only low strings (Salzedo)
            { "harpSalzedoFluidicSoundsLeft", 0xE68D },  // glyphnumber: 1137, Fluidic sounds, left hand (Salzedo)
            { "harpSalzedoFluidicSoundsRight", 0xE68E },  // glyphnumber: 1138, Fluidic sounds, right hand (Salzedo)
            { "harpSalzedoIsolatedSounds", 0xE69C },  // glyphnumber: 1139, Isolated sounds (Salzedo)
            { "harpSalzedoMetallicSounds", 0xE688 },  // glyphnumber: 1140, Metallic sounds (Salzedo)
            { "harpSalzedoMetallicSoundsOneString", 0xE69B },  // glyphnumber: 1141, Metallic sounds, one string (Salzedo)
            { "harpSalzedoMuffleTotally", 0xE68C },  // glyphnumber: 1142, Muffle totally (Salzedo)
            { "harpSalzedoOboicFlux", 0xE685 },  // glyphnumber: 1143, Oboic flux (Salzedo)
            { "harpSalzedoPlayUpperEnd", 0xE68A },  // glyphnumber: 1144, Play at upper end of strings (Salzedo)
            { "harpSalzedoSlideWithSuppleness", 0xE684 },  // glyphnumber: 1145, Slide with suppleness (Salzedo)
            { "harpSalzedoSnareDrum", 0xE69D },  // glyphnumber: 1146, Snare drum effect (Salzedo)
            { "harpSalzedoTamTamSounds", 0xE689 },  // glyphnumber: 1147, Tam-tam sounds (Salzedo)
            { "harpSalzedoThunderEffect", 0xE686 },  // glyphnumber: 1148, Thunder effect (Salzedo)
            { "harpSalzedoTimpanicSounds", 0xE68B },  // glyphnumber: 1149, Timpanic sounds (Salzedo)
            { "harpSalzedoWhistlingSounds", 0xE687 },  // glyphnumber: 1150, Whistling sounds (Salzedo)
            { "harpStringNoiseStem", 0xE694 },  // glyphnumber: 1151, Combining string noise for stem
            { "harpTuningKey", 0xE690 },  // glyphnumber: 1152, Tuning key pictogram
            { "harpTuningKeyGlissando", 0xE693 },  // glyphnumber: 1153, Retune strings for glissando
            { "harpTuningKeyHandle", 0xE691 },  // glyphnumber: 1154, Use handle of tuning key pictogram
            { "harpTuningKeyShank", 0xE692 },  // glyphnumber: 1155, Use shank of tuning key pictogram
            { "keyboardBebung2DotsAbove", 0xE668 },  // glyphnumber: 1156, Clavichord bebung, 2 finger movements (above)
            { "keyboardBebung2DotsBelow", 0xE669 },  // glyphnumber: 1157, Clavichord bebung, 2 finger movements (below)
            { "keyboardBebung3DotsAbove", 0xE66A },  // glyphnumber: 1158, Clavichord bebung, 3 finger movements (above)
            { "keyboardBebung3DotsBelow", 0xE66B },  // glyphnumber: 1159, Clavichord bebung, 3 finger movements (below)
            { "keyboardBebung4DotsAbove", 0xE66C },  // glyphnumber: 1160, Clavichord bebung, 4 finger movements (above)
            { "keyboardBebung4DotsBelow", 0xE66D },  // glyphnumber: 1161, Clavichord bebung, 4 finger movements (below)
            { "keyboardLeftPedalPictogram", 0xE65E },  // glyphnumber: 1162, Left pedal pictogram
            { "keyboardMiddlePedalPictogram", 0xE65F },  // glyphnumber: 1163, Middle pedal pictogram
            { "keyboardPedalD", 0xE653 },  // glyphnumber: 1164, Pedal d
            { "keyboardPedalDot", 0xE654 },  // glyphnumber: 1165, Pedal dot
            { "keyboardPedalE", 0xE652 },  // glyphnumber: 1166, Pedal e
            { "keyboardPedalHalf", 0xE656 },  // glyphnumber: 1167, Half-pedal mark
            { "keyboardPedalHalf2", 0xE65B },  // glyphnumber: 1168, Half pedal mark 1
            { "keyboardPedalHalf3", 0xE65C },  // glyphnumber: 1169, Half pedal mark 2
            { "keyboardPedalHeel1", 0xE661 },  // glyphnumber: 1170, Pedal heel 1
            { "keyboardPedalHeel2", 0xE662 },  // glyphnumber: 1171, Pedal heel 2
            { "keyboardPedalHeel3", 0xE663 },  // glyphnumber: 1172, Pedal heel 3 (Davis)
            { "keyboardPedalHeelToToe", 0xE674 },  // glyphnumber: 1173, Pedal heel to toe
            { "keyboardPedalHeelToe", 0xE666 },  // glyphnumber: 1174, Pedal heel or toe
            { "keyboardPedalHookEnd", 0xE673 },  // glyphnumber: 1175, Pedal hook end
            { "keyboardPedalHookStart", 0xE672 },  // glyphnumber: 1176, Pedal hook start
            { "keyboardPedalHyphen", 0xE658 },  // glyphnumber: 1177, Pedal hyphen
            { "keyboardPedalP", 0xE651 },  // glyphnumber: 1178, Pedal P
            { "keyboardPedalPed", 0xE650 },  // glyphnumber: 1179, Pedal mark
            { "keyboardPedalS", 0xE65A },  // glyphnumber: 1180, Pedal S
            { "keyboardPedalSost", 0xE659 },  // glyphnumber: 1181, Sostenuto pedal mark
            { "keyboardPedalToe1", 0xE664 },  // glyphnumber: 1182, Pedal toe 1
            { "keyboardPedalToe2", 0xE665 },  // glyphnumber: 1183, Pedal toe 2
            { "keyboardPedalToeToHeel", 0xE675 },  // glyphnumber: 1184, Pedal toe to heel
            { "keyboardPedalUp", 0xE655 },  // glyphnumber: 1185, Pedal up mark
            { "keyboardPedalUpNotch", 0xE657 },  // glyphnumber: 1186, Pedal up notch
            { "keyboardPedalUpSpecial", 0xE65D },  // glyphnumber: 1187, Pedal up special
            { "keyboardPlayWithLH", 0xE670 },  // glyphnumber: 1188, Play with left hand
            { "keyboardPlayWithLHEnd", 0xE671 },  // glyphnumber: 1189, Play with left hand (end)
            { "keyboardPlayWithRH", 0xE66E },  // glyphnumber: 1190, Play with right hand
            { "keyboardPlayWithRHEnd", 0xE66F },  // glyphnumber: 1191, Play with right hand (end)
            { "keyboardPluckInside", 0xE667 },  // glyphnumber: 1192, Pluck strings inside piano (Maderna)
            { "keyboardRightPedalPictogram", 0xE660 },  // glyphnumber: 1193, Right pedal pictogram
            { "kievanAccidentalFlat", 0xEC3E },  // glyphnumber: 1194, Kievan flat
            { "kievanAccidentalSharp", 0xEC3D },  // glyphnumber: 1195, Kievan sharp
            { "kievanAugmentationDot", 0xEC3C },  // glyphnumber: 1196, Kievan augmentation dot
            { "kievanCClef", 0xEC30 },  // glyphnumber: 1197, Kievan C clef (tse-fa-ut)
            { "kievanEndingSymbol", 0xEC31 },  // glyphnumber: 1198, Kievan ending symbol
            { "kievanNote8thStemDown", 0xEC3A },  // glyphnumber: 1199, Kievan eighth note, stem down
            { "kievanNote8thStemUp", 0xEC39 },  // glyphnumber: 1200, Kievan eighth note, stem up
            { "kievanNoteBeam", 0xEC3B },  // glyphnumber: 1201, Kievan beam
            { "kievanNoteHalfStaffLine", 0xEC35 },  // glyphnumber: 1202, Kievan half note (on staff line)
            { "kievanNoteHalfStaffSpace", 0xEC36 },  // glyphnumber: 1203, Kievan half note (in staff space)
            { "kievanNoteQuarterStemDown", 0xEC38 },  // glyphnumber: 1204, Kievan quarter note, stem down
            { "kievanNoteQuarterStemUp", 0xEC37 },  // glyphnumber: 1205, Kievan quarter note, stem up
            { "kievanNoteReciting", 0xEC32 },  // glyphnumber: 1206, Kievan reciting note
            { "kievanNoteWhole", 0xEC33 },  // glyphnumber: 1207, Kievan whole note
            { "kievanNoteWholeFinal", 0xEC34 },  // glyphnumber: 1208, Kievan final whole note
            { "kodalyHandDo", 0xEC40 },  // glyphnumber: 1209, Do hand sign
            { "kodalyHandFa", 0xEC43 },  // glyphnumber: 1210, Fa hand sign
            { "kodalyHandLa", 0xEC45 },  // glyphnumber: 1211, La hand sign
            { "kodalyHandMi", 0xEC42 },  // glyphnumber: 1212, Mi hand sign
            { "kodalyHandRe", 0xEC41 },  // glyphnumber: 1213, Re hand sign
            { "kodalyHandSo", 0xEC44 },  // glyphnumber: 1214, So hand sign
            { "kodalyHandTi", 0xEC46 },  // glyphnumber: 1215, Ti hand sign
            { "leftRepeatSmall", 0xE04C },  // glyphnumber: 1216, Left repeat sign within bar
            { "legerLine", 0xE022 },  // glyphnumber: 1217, Leger line
            { "legerLineNarrow", 0xE024 },  // glyphnumber: 1218, Leger line (narrow)
            { "legerLineWide", 0xE023 },  // glyphnumber: 1219, Leger line (wide)
            { "luteBarlineEndRepeat", 0xEBA4 },  // glyphnumber: 1220, Lute tablature end repeat barline
            { "luteBarlineFinal", 0xEBA5 },  // glyphnumber: 1221, Lute tablature final barline
            { "luteBarlineStartRepeat", 0xEBA3 },  // glyphnumber: 1222, Lute tablature start repeat barline
            { "luteDuration16th", 0xEBAB },  // glyphnumber: 1223, 16th note (semiquaver) duration sign
            { "luteDuration32nd", 0xEBAC },  // glyphnumber: 1224, 32nd note (demisemiquaver) duration sign
            { "luteDuration8th", 0xEBAA },  // glyphnumber: 1225, Eighth note (quaver) duration sign
            { "luteDurationDoubleWhole", 0xEBA6 },  // glyphnumber: 1226, Double whole note (breve) duration sign
            { "luteDurationHalf", 0xEBA8 },  // glyphnumber: 1227, Half note (minim) duration sign
            { "luteDurationQuarter", 0xEBA9 },  // glyphnumber: 1228, Quarter note (crotchet) duration sign
            { "luteDurationWhole", 0xEBA7 },  // glyphnumber: 1229, Whole note (semibreve) duration sign
            { "luteFingeringRHFirst", 0xEBAE },  // glyphnumber: 1230, Right-hand fingering, first finger
            { "luteFingeringRHSecond", 0xEBAF },  // glyphnumber: 1231, Right-hand fingering, second finger
            { "luteFingeringRHThird", 0xEBB0 },  // glyphnumber: 1232, Right-hand fingering, third finger
            { "luteFingeringRHThumb", 0xEBAD },  // glyphnumber: 1233, Right-hand fingering, thumb
            { "luteFrench10thCourse", 0xEBD0 },  // glyphnumber: 1234, 10th course (diapason)
            { "luteFrench7thCourse", 0xEBCD },  // glyphnumber: 1235, Seventh course (diapason)
            { "luteFrench8thCourse", 0xEBCE },  // glyphnumber: 1236, Eighth course (diapason)
            { "luteFrench9thCourse", 0xEBCF },  // glyphnumber: 1237, Ninth course (diapason)
            { "luteFrenchAppoggiaturaAbove", 0xEBD5 },  // glyphnumber: 1238, Appoggiatura from above
            { "luteFrenchAppoggiaturaBelow", 0xEBD4 },  // glyphnumber: 1239, Appoggiatura from below
            { "luteFrenchFretA", 0xEBC0 },  // glyphnumber: 1240, Open string (a)
            { "luteFrenchFretB", 0xEBC1 },  // glyphnumber: 1241, First fret (b)
            { "luteFrenchFretC", 0xEBC2 },  // glyphnumber: 1242, Second fret (c)
            { "luteFrenchFretD", 0xEBC3 },  // glyphnumber: 1243, Third fret (d)
            { "luteFrenchFretE", 0xEBC4 },  // glyphnumber: 1244, Fourth fret (e)
            { "luteFrenchFretF", 0xEBC5 },  // glyphnumber: 1245, Fifth fret (f)
            { "luteFrenchFretG", 0xEBC6 },  // glyphnumber: 1246, Sixth fret (g)
            { "luteFrenchFretH", 0xEBC7 },  // glyphnumber: 1247, Seventh fret (h)
            { "luteFrenchFretI", 0xEBC8 },  // glyphnumber: 1248, Eighth fret (i)
            { "luteFrenchFretK", 0xEBC9 },  // glyphnumber: 1249, Ninth fret (k)
            { "luteFrenchFretL", 0xEBCA },  // glyphnumber: 1250, 10th fret (l)
            { "luteFrenchFretM", 0xEBCB },  // glyphnumber: 1251, 11th fret (m)
            { "luteFrenchFretN", 0xEBCC },  // glyphnumber: 1252, 12th fret (n)
            { "luteFrenchMordentInverted", 0xEBD3 },  // glyphnumber: 1253, Inverted mordent
            { "luteFrenchMordentLower", 0xEBD2 },  // glyphnumber: 1254, Mordent with lower auxiliary
            { "luteFrenchMordentUpper", 0xEBD1 },  // glyphnumber: 1255, Mordent with upper auxiliary
            { "luteGermanALower", 0xEC00 },  // glyphnumber: 1256, 5th course, 1st fret (a)
            { "luteGermanAUpper", 0xEC17 },  // glyphnumber: 1257, 6th course, 1st fret (A)
            { "luteGermanBLower", 0xEC01 },  // glyphnumber: 1258, 4th course, 1st fret (b)
            { "luteGermanBUpper", 0xEC18 },  // glyphnumber: 1259, 6th course, 2nd fret (B)
            { "luteGermanCLower", 0xEC02 },  // glyphnumber: 1260, 3rd course, 1st fret (c)
            { "luteGermanCUpper", 0xEC19 },  // glyphnumber: 1261, 6th course, 3rd fret (C)
            { "luteGermanDLower", 0xEC03 },  // glyphnumber: 1262, 2nd course, 1st fret (d)
            { "luteGermanDUpper", 0xEC1A },  // glyphnumber: 1263, 6th course, 4th fret (D)
            { "luteGermanELower", 0xEC04 },  // glyphnumber: 1264, 1st course, 1st fret (e)
            { "luteGermanEUpper", 0xEC1B },  // glyphnumber: 1265, 6th course, 5th fret (E)
            { "luteGermanFLower", 0xEC05 },  // glyphnumber: 1266, 5th course, 2nd fret (f)
            { "luteGermanFUpper", 0xEC1C },  // glyphnumber: 1267, 6th course, 6th fret (F)
            { "luteGermanGLower", 0xEC06 },  // glyphnumber: 1268, 4th course, 2nd fret (g)
            { "luteGermanGUpper", 0xEC1D },  // glyphnumber: 1269, 6th course, 7th fret (G)
            { "luteGermanHLower", 0xEC07 },  // glyphnumber: 1270, 3rd course, 2nd fret (h)
            { "luteGermanHUpper", 0xEC1E },  // glyphnumber: 1271, 6th course, 8th fret (H)
            { "luteGermanILower", 0xEC08 },  // glyphnumber: 1272, 2nd course, 2nd fret (i)
            { "luteGermanIUpper", 0xEC1F },  // glyphnumber: 1273, 6th course, 9th fret (I)
            { "luteGermanKLower", 0xEC09 },  // glyphnumber: 1274, 1st course, 2nd fret (k)
            { "luteGermanKUpper", 0xEC20 },  // glyphnumber: 1275, 6th course, 10th fret (K)
            { "luteGermanLLower", 0xEC0A },  // glyphnumber: 1276, 5th course, 3rd fret (l)
            { "luteGermanLUpper", 0xEC21 },  // glyphnumber: 1277, 6th course, 11th fret (L)
            { "luteGermanMLower", 0xEC0B },  // glyphnumber: 1278, 4th course, 3rd fret (m)
            { "luteGermanMUpper", 0xEC22 },  // glyphnumber: 1279, 6th course, 12th fret (M)
            { "luteGermanNLower", 0xEC0C },  // glyphnumber: 1280, 3rd course, 3rd fret (n)
            { "luteGermanNUpper", 0xEC23 },  // glyphnumber: 1281, 6th course, 13th fret (N)
            { "luteGermanOLower", 0xEC0D },  // glyphnumber: 1282, 2nd course, 3rd fret (o)
            { "luteGermanPLower", 0xEC0E },  // glyphnumber: 1283, 1st course, 3rd fret (p)
            { "luteGermanQLower", 0xEC0F },  // glyphnumber: 1284, 5th course, 4th fret (q)
            { "luteGermanRLower", 0xEC10 },  // glyphnumber: 1285, 4th course, 4th fret (r)
            { "luteGermanSLower", 0xEC11 },  // glyphnumber: 1286, 3rd course, 4th fret (s)
            { "luteGermanTLower", 0xEC12 },  // glyphnumber: 1287, 2nd course, 4th fret (t)
            { "luteGermanVLower", 0xEC13 },  // glyphnumber: 1288, 1st course, 4th fret (v)
            { "luteGermanXLower", 0xEC14 },  // glyphnumber: 1289, 5th course, 5th fret (x)
            { "luteGermanYLower", 0xEC15 },  // glyphnumber: 1290, 4th course, 5th fret (y)
            { "luteGermanZLower", 0xEC16 },  // glyphnumber: 1291, 3rd course, 5th fret (z)
            { "luteItalianClefCSolFaUt", 0xEBF1 },  // glyphnumber: 1292, C sol fa ut clef
            { "luteItalianClefFFaUt", 0xEBF0 },  // glyphnumber: 1293, F fa ut clef
            { "luteItalianFret0", 0xEBE0 },  // glyphnumber: 1294, Open string (0)
            { "luteItalianFret1", 0xEBE1 },  // glyphnumber: 1295, First fret (1)
            { "luteItalianFret2", 0xEBE2 },  // glyphnumber: 1296, Second fret (2)
            { "luteItalianFret3", 0xEBE3 },  // glyphnumber: 1297, Third fret (3)
            { "luteItalianFret4", 0xEBE4 },  // glyphnumber: 1298, Fourth fret (4)
            { "luteItalianFret5", 0xEBE5 },  // glyphnumber: 1299, Fifth fret (5)
            { "luteItalianFret6", 0xEBE6 },  // glyphnumber: 1300, Sixth fret (6)
            { "luteItalianFret7", 0xEBE7 },  // glyphnumber: 1301, Seventh fret (7)
            { "luteItalianFret8", 0xEBE8 },  // glyphnumber: 1302, Eighth fret (8)
            { "luteItalianFret9", 0xEBE9 },  // glyphnumber: 1303, Ninth fret (9)
            { "luteItalianHoldFinger", 0xEBF4 },  // glyphnumber: 1304, Hold finger in place
            { "luteItalianHoldNote", 0xEBF3 },  // glyphnumber: 1305, Hold note
            { "luteItalianReleaseFinger", 0xEBF5 },  // glyphnumber: 1306, Release finger
            { "luteItalianTempoFast", 0xEBEA },  // glyphnumber: 1307, Fast tempo indication (de Mudarra)
            { "luteItalianTempoNeitherFastNorSlow", 0xEBEC },  // glyphnumber: 1308, Neither fast nor slow tempo indication (de Mudarra)
            { "luteItalianTempoSlow", 0xEBED },  // glyphnumber: 1309, Slow tempo indication (de Mudarra)
            { "luteItalianTempoSomewhatFast", 0xEBEB },  // glyphnumber: 1310, Somewhat fast tempo indication (de Narvaez)
            { "luteItalianTempoVerySlow", 0xEBEE },  // glyphnumber: 1311, Very slow indication (de Narvaez)
            { "luteItalianTimeTriple", 0xEBEF },  // glyphnumber: 1312, Triple time indication
            { "luteItalianTremolo", 0xEBF2 },  // glyphnumber: 1313, Single-finger tremolo or mordent
            { "luteItalianVibrato", 0xEBF6 },  // glyphnumber: 1314, Vibrato (verre cassé)
            { "luteStaff6Lines", 0xEBA0 },  // glyphnumber: 1315, Lute tablature staff, 6 courses
            { "luteStaff6LinesNarrow", 0xEBA2 },  // glyphnumber: 1316, Lute tablature staff, 6 courses (narrow)
            { "luteStaff6LinesWide", 0xEBA1 },  // glyphnumber: 1317, Lute tablature staff, 6 courses (wide)
            { "lyricsElision", 0xE551 },  // glyphnumber: 1318, Elision
            { "lyricsElisionNarrow", 0xE550 },  // glyphnumber: 1319, Narrow elision
            { "lyricsElisionWide", 0xE552 },  // glyphnumber: 1320, Wide elision
            { "lyricsHyphenBaseline", 0xE553 },  // glyphnumber: 1321, Baseline hyphen
            { "lyricsHyphenBaselineNonBreaking", 0xE554 },  // glyphnumber: 1322, Non-breaking baseline hyphen
            { "medRenFlatHardB", 0xE9E1 },  // glyphnumber: 1323, Flat, hard b (mi)
            { "medRenFlatSoftB", 0xE9E0 },  // glyphnumber: 1324, Flat, soft b (fa)
            { "medRenFlatWithDot", 0xE9E4 },  // glyphnumber: 1325, Flat with dot
            { "medRenGClefCMN", 0xEA24 },  // glyphnumber: 1326, G clef (Corpus Monodicum)
            { "medRenLiquescenceCMN", 0xEA22 },  // glyphnumber: 1327, Liquescence
            { "medRenLiquescentAscCMN", 0xEA26 },  // glyphnumber: 1328, Liquescent ascending (Corpus Monodicum)
            { "medRenLiquescentDescCMN", 0xEA27 },  // glyphnumber: 1329, Liquescent descending (Corpus Monodicum)
            { "medRenNatural", 0xE9E2 },  // glyphnumber: 1330, Natural
            { "medRenNaturalWithCross", 0xE9E5 },  // glyphnumber: 1331, Natural with interrupted cross
            { "medRenOriscusCMN", 0xEA2A },  // glyphnumber: 1332, Oriscus (Corpus Monodicum)
            { "medRenPlicaCMN", 0xEA23 },  // glyphnumber: 1333, Plica
            { "medRenPunctumCMN", 0xEA25 },  // glyphnumber: 1334, Punctum (Corpus Monodicum)
            { "medRenQuilismaCMN", 0xEA28 },  // glyphnumber: 1335, Quilisma (Corpus Monodicum)
            { "medRenSharpCroix", 0xE9E3 },  // glyphnumber: 1336, Croix
            { "medRenStrophicusCMN", 0xEA29 },  // glyphnumber: 1337, Strophicus (Corpus Monodicum)
            { "mensuralAlterationSign", 0xEA10 },  // glyphnumber: 1338, Alteration sign
            { "mensuralBlackBrevis", 0xE952 },  // glyphnumber: 1339, Black mensural brevis
            { "mensuralBlackBrevisVoid", 0xE956 },  // glyphnumber: 1340, Black mensural void brevis
            { "mensuralBlackDragma", 0xE95A },  // glyphnumber: 1341, Black mensural dragma
            { "mensuralBlackLonga", 0xE951 },  // glyphnumber: 1342, Black mensural longa
            { "mensuralBlackMaxima", 0xE950 },  // glyphnumber: 1343, Black mensural maxima
            { "mensuralBlackMinima", 0xE954 },  // glyphnumber: 1344, Black mensural minima
            { "mensuralBlackMinimaVoid", 0xE958 },  // glyphnumber: 1345, Black mensural void minima
            { "mensuralBlackSemibrevis", 0xE953 },  // glyphnumber: 1346, Black mensural semibrevis
            { "mensuralBlackSemibrevisCaudata", 0xE959 },  // glyphnumber: 1347, Black mensural semibrevis caudata
            { "mensuralBlackSemibrevisOblique", 0xE95B },  // glyphnumber: 1348, Black mensural oblique semibrevis
            { "mensuralBlackSemibrevisVoid", 0xE957 },  // glyphnumber: 1349, Black mensural void semibrevis
            { "mensuralBlackSemiminima", 0xE955 },  // glyphnumber: 1350, Black mensural semiminima
            { "mensuralCclef", 0xE905 },  // glyphnumber: 1351, Mensural C clef
            { "mensuralCclefPetrucciPosHigh", 0xE90A },  // glyphnumber: 1352, Petrucci C clef, high position
            { "mensuralCclefPetrucciPosHighest", 0xE90B },  // glyphnumber: 1353, Petrucci C clef, highest position
            { "mensuralCclefPetrucciPosLow", 0xE908 },  // glyphnumber: 1354, Petrucci C clef, low position
            { "mensuralCclefPetrucciPosLowest", 0xE907 },  // glyphnumber: 1355, Petrucci C clef, lowest position
            { "mensuralCclefPetrucciPosMiddle", 0xE909 },  // glyphnumber: 1356, Petrucci C clef, middle position
            { "mensuralColorationEndRound", 0xEA0F },  // glyphnumber: 1357, Coloration end, round
            { "mensuralColorationEndSquare", 0xEA0D },  // glyphnumber: 1358, Coloration end, square
            { "mensuralColorationStartRound", 0xEA0E },  // glyphnumber: 1359, Coloration start, round
            { "mensuralColorationStartSquare", 0xEA0C },  // glyphnumber: 1360, Coloration start, square
            { "mensuralCombStemDiagonal", 0xE940 },  // glyphnumber: 1361, Combining stem diagonal
            { "mensuralCombStemDown", 0xE93F },  // glyphnumber: 1362, Combining stem down
            { "mensuralCombStemDownFlagExtended", 0xE948 },  // glyphnumber: 1363, Combining stem with extended flag down
            { "mensuralCombStemDownFlagFlared", 0xE946 },  // glyphnumber: 1364, Combining stem with flared flag down
            { "mensuralCombStemDownFlagFusa", 0xE94C },  // glyphnumber: 1365, Combining stem with fusa flag down
            { "mensuralCombStemDownFlagLeft", 0xE944 },  // glyphnumber: 1366, Combining stem with flag left down
            { "mensuralCombStemDownFlagRight", 0xE942 },  // glyphnumber: 1367, Combining stem with flag right down
            { "mensuralCombStemDownFlagSemiminima", 0xE94A },  // glyphnumber: 1368, Combining stem with semiminima flag down
            { "mensuralCombStemUp", 0xE93E },  // glyphnumber: 1369, Combining stem up
            { "mensuralCombStemUpFlagExtended", 0xE947 },  // glyphnumber: 1370, Combining stem with extended flag up
            { "mensuralCombStemUpFlagFlared", 0xE945 },  // glyphnumber: 1371, Combining stem with flared flag up
            { "mensuralCombStemUpFlagFusa", 0xE94B },  // glyphnumber: 1372, Combining stem with fusa flag up
            { "mensuralCombStemUpFlagLeft", 0xE943 },  // glyphnumber: 1373, Combining stem with flag left up
            { "mensuralCombStemUpFlagRight", 0xE941 },  // glyphnumber: 1374, Combining stem with flag right up
            { "mensuralCombStemUpFlagSemiminima", 0xE949 },  // glyphnumber: 1375, Combining stem with semiminima flag up
            { "mensuralCustosCheckmark", 0xEA0A },  // glyphnumber: 1376, Checkmark custos
            { "mensuralCustosDown", 0xEA03 },  // glyphnumber: 1377, Mensural custos down
            { "mensuralCustosTurn", 0xEA0B },  // glyphnumber: 1378, Turn-like custos
            { "mensuralCustosUp", 0xEA02 },  // glyphnumber: 1379, Mensural custos up
            { "mensuralFclef", 0xE903 },  // glyphnumber: 1380, Mensural F clef
            { "mensuralFclefPetrucci", 0xE904 },  // glyphnumber: 1381, Petrucci F clef
            { "mensuralGclef", 0xE900 },  // glyphnumber: 1382, Mensural G clef
            { "mensuralGclefPetrucci", 0xE901 },  // glyphnumber: 1383, Petrucci G clef
            { "mensuralModusImperfectumVert", 0xE92D },  // glyphnumber: 1384, Modus imperfectum, vertical
            { "mensuralModusPerfectumVert", 0xE92C },  // glyphnumber: 1385, Modus perfectum, vertical
            { "mensuralNoteheadLongaBlack", 0xE934 },  // glyphnumber: 1386, Longa/brevis notehead, black
            { "mensuralNoteheadLongaBlackVoid", 0xE936 },  // glyphnumber: 1387, Longa/brevis notehead, black and void
            { "mensuralNoteheadLongaVoid", 0xE935 },  // glyphnumber: 1388, Longa/brevis notehead, void
            { "mensuralNoteheadLongaWhite", 0xE937 },  // glyphnumber: 1389, Longa/brevis notehead, white
            { "mensuralNoteheadMaximaBlack", 0xE930 },  // glyphnumber: 1390, Maxima notehead, black
            { "mensuralNoteheadMaximaBlackVoid", 0xE932 },  // glyphnumber: 1391, Maxima notehead, black and void
            { "mensuralNoteheadMaximaVoid", 0xE931 },  // glyphnumber: 1392, Maxima notehead, void
            { "mensuralNoteheadMaximaWhite", 0xE933 },  // glyphnumber: 1393, Maxima notehead, white
            { "mensuralNoteheadMinimaWhite", 0xE93C },  // glyphnumber: 1394, Minima notehead, white
            { "mensuralNoteheadSemibrevisBlack", 0xE938 },  // glyphnumber: 1395, Semibrevis notehead, black
            { "mensuralNoteheadSemibrevisBlackVoid", 0xE93A },  // glyphnumber: 1396, Semibrevis notehead, black and void
            { "mensuralNoteheadSemibrevisBlackVoidTurned", 0xE93B },  // glyphnumber: 1397, Semibrevis notehead, black and void (turned)
            { "mensuralNoteheadSemibrevisVoid", 0xE939 },  // glyphnumber: 1398, Semibrevis notehead, void
            { "mensuralNoteheadSemiminimaWhite", 0xE93D },  // glyphnumber: 1399, Semiminima/fusa notehead, white
            { "mensuralObliqueAsc2ndBlack", 0xE970 },  // glyphnumber: 1400, Oblique form, ascending 2nd, black
            { "mensuralObliqueAsc2ndBlackVoid", 0xE972 },  // glyphnumber: 1401, Oblique form, ascending 2nd, black and void
            { "mensuralObliqueAsc2ndVoid", 0xE971 },  // glyphnumber: 1402, Oblique form, ascending 2nd, void
            { "mensuralObliqueAsc2ndWhite", 0xE973 },  // glyphnumber: 1403, Oblique form, ascending 2nd, white
            { "mensuralObliqueAsc3rdBlack", 0xE974 },  // glyphnumber: 1404, Oblique form, ascending 3rd, black
            { "mensuralObliqueAsc3rdBlackVoid", 0xE976 },  // glyphnumber: 1405, Oblique form, ascending 3rd, black and void
            { "mensuralObliqueAsc3rdVoid", 0xE975 },  // glyphnumber: 1406, Oblique form, ascending 3rd, void
            { "mensuralObliqueAsc3rdWhite", 0xE977 },  // glyphnumber: 1407, Oblique form, ascending 3rd, white
            { "mensuralObliqueAsc4thBlack", 0xE978 },  // glyphnumber: 1408, Oblique form, ascending 4th, black
            { "mensuralObliqueAsc4thBlackVoid", 0xE97A },  // glyphnumber: 1409, Oblique form, ascending 4th, black and void
            { "mensuralObliqueAsc4thVoid", 0xE979 },  // glyphnumber: 1410, Oblique form, ascending 4th, void
            { "mensuralObliqueAsc4thWhite", 0xE97B },  // glyphnumber: 1411, Oblique form, ascending 4th, white
            { "mensuralObliqueAsc5thBlack", 0xE97C },  // glyphnumber: 1412, Oblique form, ascending 5th, black
            { "mensuralObliqueAsc5thBlackVoid", 0xE97E },  // glyphnumber: 1413, Oblique form, ascending 5th, black and void
            { "mensuralObliqueAsc5thVoid", 0xE97D },  // glyphnumber: 1414, Oblique form, ascending 5th, void
            { "mensuralObliqueAsc5thWhite", 0xE97F },  // glyphnumber: 1415, Oblique form, ascending 5th, white
            { "mensuralObliqueDesc2ndBlack", 0xE980 },  // glyphnumber: 1416, Oblique form, descending 2nd, black
            { "mensuralObliqueDesc2ndBlackVoid", 0xE982 },  // glyphnumber: 1417, Oblique form, descending 2nd, black and void
            { "mensuralObliqueDesc2ndVoid", 0xE981 },  // glyphnumber: 1418, Oblique form, descending 2nd, void
            { "mensuralObliqueDesc2ndWhite", 0xE983 },  // glyphnumber: 1419, Oblique form, descending 2nd, white
            { "mensuralObliqueDesc3rdBlack", 0xE984 },  // glyphnumber: 1420, Oblique form, descending 3rd, black
            { "mensuralObliqueDesc3rdBlackVoid", 0xE986 },  // glyphnumber: 1421, Oblique form, descending 3rd, black and void
            { "mensuralObliqueDesc3rdVoid", 0xE985 },  // glyphnumber: 1422, Oblique form, descending 3rd, void
            { "mensuralObliqueDesc3rdWhite", 0xE987 },  // glyphnumber: 1423, Oblique form, descending 3rd, white
            { "mensuralObliqueDesc4thBlack", 0xE988 },  // glyphnumber: 1424, Oblique form, descending 4th, black
            { "mensuralObliqueDesc4thBlackVoid", 0xE98A },  // glyphnumber: 1425, Oblique form, descending 4th, black and void
            { "mensuralObliqueDesc4thVoid", 0xE989 },  // glyphnumber: 1426, Oblique form, descending 4th, void
            { "mensuralObliqueDesc4thWhite", 0xE98B },  // glyphnumber: 1427, Oblique form, descending 4th, white
            { "mensuralObliqueDesc5thBlack", 0xE98C },  // glyphnumber: 1428, Oblique form, descending 5th, black
            { "mensuralObliqueDesc5thBlackVoid", 0xE98E },  // glyphnumber: 1429, Oblique form, descending 5th, black and void
            { "mensuralObliqueDesc5thVoid", 0xE98D },  // glyphnumber: 1430, Oblique form, descending 5th, void
            { "mensuralObliqueDesc5thWhite", 0xE98F },  // glyphnumber: 1431, Oblique form, descending 5th, white
            { "mensuralProlation1", 0xE910 },  // glyphnumber: 1432, Tempus perfectum cum prolatione perfecta (9/8)
            { "mensuralProlation10", 0xE919 },  // glyphnumber: 1433, Tempus imperfectum cum prolatione imperfecta diminution 4
            { "mensuralProlation11", 0xE91A },  // glyphnumber: 1434, Tempus imperfectum cum prolatione imperfecta diminution 5
            { "mensuralProlation2", 0xE911 },  // glyphnumber: 1435, Tempus perfectum cum prolatione imperfecta (3/4)
            { "mensuralProlation3", 0xE912 },  // glyphnumber: 1436, Tempus perfectum cum prolatione imperfecta diminution 1 (3/8)
            { "mensuralProlation4", 0xE913 },  // glyphnumber: 1437, Tempus perfectum cum prolatione perfecta diminution 2 (9/16)
            { "mensuralProlation5", 0xE914 },  // glyphnumber: 1438, Tempus imperfectum cum prolatione perfecta (6/8)
            { "mensuralProlation6", 0xE915 },  // glyphnumber: 1439, Tempus imperfectum cum prolatione imperfecta (2/4)
            { "mensuralProlation7", 0xE916 },  // glyphnumber: 1440, Tempus imperfectum cum prolatione imperfecta diminution 1 (2/2)
            { "mensuralProlation8", 0xE917 },  // glyphnumber: 1441, Tempus imperfectum cum prolatione imperfecta diminution 2 (6/16)
            { "mensuralProlation9", 0xE918 },  // glyphnumber: 1442, Tempus imperfectum cum prolatione imperfecta diminution 3 (2/2)
            { "mensuralProlationCombiningDot", 0xE920 },  // glyphnumber: 1443, Combining dot
            { "mensuralProlationCombiningDotVoid", 0xE924 },  // glyphnumber: 1444, Combining void dot
            { "mensuralProlationCombiningStroke", 0xE925 },  // glyphnumber: 1445, Combining vertical stroke
            { "mensuralProlationCombiningThreeDots", 0xE922 },  // glyphnumber: 1446, Combining three dots horizontal
            { "mensuralProlationCombiningThreeDotsTri", 0xE923 },  // glyphnumber: 1447, Combining three dots triangular
            { "mensuralProlationCombiningTwoDots", 0xE921 },  // glyphnumber: 1448, Combining two dots
            { "mensuralProportion1", 0xE926 },  // glyphnumber: 1449, Mensural proportion 1
            { "mensuralProportion2", 0xE927 },  // glyphnumber: 1450, Mensural proportion 2
            { "mensuralProportion3", 0xE928 },  // glyphnumber: 1451, Mensural proportion 3
            { "mensuralProportion4", 0xE929 },  // glyphnumber: 1452, Mensural proportion 4
            { "mensuralProportionMajor", 0xE92B },  // glyphnumber: 1453, Mensural proportion major
            { "mensuralProportionMinor", 0xE92A },  // glyphnumber: 1454, Mensural proportion minor
            { "mensuralProportionProportioDupla1", 0xE91C },  // glyphnumber: 1455, Proportio dupla 1
            { "mensuralProportionProportioDupla2", 0xE91D },  // glyphnumber: 1456, Proportio dupla 2
            { "mensuralProportionProportioQuadrupla", 0xE91F },  // glyphnumber: 1457, Proportio quadrupla
            { "mensuralProportionProportioTripla", 0xE91E },  // glyphnumber: 1458, Proportio tripla
            { "mensuralProportionTempusPerfectum", 0xE91B },  // glyphnumber: 1459, Tempus perfectum
            { "mensuralRestBrevis", 0xE9F3 },  // glyphnumber: 1460, Brevis rest
            { "mensuralRestFusa", 0xE9F7 },  // glyphnumber: 1461, Fusa rest
            { "mensuralRestLongaImperfecta", 0xE9F2 },  // glyphnumber: 1462, Longa imperfecta rest
            { "mensuralRestLongaPerfecta", 0xE9F1 },  // glyphnumber: 1463, Longa perfecta rest
            { "mensuralRestMaxima", 0xE9F0 },  // glyphnumber: 1464, Maxima rest
            { "mensuralRestMinima", 0xE9F5 },  // glyphnumber: 1465, Minima rest
            { "mensuralRestSemibrevis", 0xE9F4 },  // glyphnumber: 1466, Semibrevis rest
            { "mensuralRestSemifusa", 0xE9F8 },  // glyphnumber: 1467, Semifusa rest
            { "mensuralRestSemiminima", 0xE9F6 },  // glyphnumber: 1468, Semiminima rest
            { "mensuralSignumDown", 0xEA01 },  // glyphnumber: 1469, Signum congruentiae down
            { "mensuralSignumUp", 0xEA00 },  // glyphnumber: 1470, Signum congruentiae up
            { "mensuralTempusImperfectumHoriz", 0xE92F },  // glyphnumber: 1471, Tempus imperfectum, horizontal
            { "mensuralTempusPerfectumHoriz", 0xE92E },  // glyphnumber: 1472, Tempus perfectum, horizontal
            { "mensuralWhiteBrevis", 0xE95E },  // glyphnumber: 1473, White mensural brevis
            { "mensuralWhiteFusa", 0xE961 },  // glyphnumber: 1474, White mensural fusa
            { "mensuralWhiteLonga", 0xE95D },  // glyphnumber: 1475, White mensural longa
            { "mensuralWhiteMaxima", 0xE95C },  // glyphnumber: 1476, White mensural maxima
            { "mensuralWhiteMinima", 0xE95F },  // glyphnumber: 1477, White mensural minima
            { "mensuralWhiteSemiminima", 0xE960 },  // glyphnumber: 1478, White mensural semiminima
            { "metAugmentationDot", 0xECB7 },  // glyphnumber: 1479, Augmentation dot
            { "metNote1024thDown", 0xECB6 },  // glyphnumber: 1480, 1024th note (semihemidemisemihemidemisemiquaver) stem down
            { "metNote1024thUp", 0xECB5 },  // glyphnumber: 1481, 1024th note (semihemidemisemihemidemisemiquaver) stem up
            { "metNote128thDown", 0xECB0 },  // glyphnumber: 1482, 128th note (semihemidemisemiquaver) stem down
            { "metNote128thUp", 0xECAF },  // glyphnumber: 1483, 128th note (semihemidemisemiquaver) stem up
            { "metNote16thDown", 0xECAA },  // glyphnumber: 1484, 16th note (semiquaver) stem down
            { "metNote16thUp", 0xECA9 },  // glyphnumber: 1485, 16th note (semiquaver) stem up
            { "metNote256thDown", 0xECB2 },  // glyphnumber: 1486, 256th note (demisemihemidemisemiquaver) stem down
            { "metNote256thUp", 0xECB1 },  // glyphnumber: 1487, 256th note (demisemihemidemisemiquaver) stem up
            { "metNote32ndDown", 0xECAC },  // glyphnumber: 1488, 32nd note (demisemiquaver) stem down
            { "metNote32ndUp", 0xECAB },  // glyphnumber: 1489, 32nd note (demisemiquaver) stem up
            { "metNote512thDown", 0xECB4 },  // glyphnumber: 1490, 512th note (hemidemisemihemidemisemiquaver) stem down
            { "metNote512thUp", 0xECB3 },  // glyphnumber: 1491, 512th note (hemidemisemihemidemisemiquaver) stem up
            { "metNote64thDown", 0xECAE },  // glyphnumber: 1492, 64th note (hemidemisemiquaver) stem down
            { "metNote64thUp", 0xECAD },  // glyphnumber: 1493, 64th note (hemidemisemiquaver) stem up
            { "metNote8thDown", 0xECA8 },  // glyphnumber: 1494, Eighth note (quaver) stem down
            { "metNote8thUp", 0xECA7 },  // glyphnumber: 1495, Eighth note (quaver) stem up
            { "metNoteDoubleWhole", 0xECA0 },  // glyphnumber: 1496, Double whole note (breve)
            { "metNoteDoubleWholeSquare", 0xECA1 },  // glyphnumber: 1497, Double whole note (square)
            { "metNoteHalfDown", 0xECA4 },  // glyphnumber: 1498, Half note (minim) stem down
            { "metNoteHalfUp", 0xECA3 },  // glyphnumber: 1499, Half note (minim) stem up
            { "metNoteQuarterDown", 0xECA6 },  // glyphnumber: 1500, Quarter note (crotchet) stem down
            { "metNoteQuarterUp", 0xECA5 },  // glyphnumber: 1501, Quarter note (crotchet) stem up
            { "metNoteWhole", 0xECA2 },  // glyphnumber: 1502, Whole note (semibreve)
            { "metricModulationArrowLeft", 0xEC63 },  // glyphnumber: 1503, Left-pointing arrow for metric modulation
            { "metricModulationArrowRight", 0xEC64 },  // glyphnumber: 1504, Right-pointing arrow for metric modulation
            { "miscDoNotCopy", 0xEC61 },  // glyphnumber: 1505, Do not copy
            { "miscDoNotPhotocopy", 0xEC60 },  // glyphnumber: 1506, Do not photocopy
            { "miscEyeglasses", 0xEC62 },  // glyphnumber: 1507, Eyeglasses
            { "note1024thDown", 0xE1E6 },  // glyphnumber: 1508, 1024th note (semihemidemisemihemidemisemiquaver) stem down
            { "note1024thUp", 0xE1E5 },  // glyphnumber: 1509, 1024th note (semihemidemisemihemidemisemiquaver) stem up
            { "note128thDown", 0xE1E0 },  // glyphnumber: 1510, 128th note (semihemidemisemiquaver) stem down
            { "note128thUp", 0xE1DF },  // glyphnumber: 1511, 128th note (semihemidemisemiquaver) stem up
            { "note16thDown", 0xE1DA },  // glyphnumber: 1512, 16th note (semiquaver) stem down
            { "note16thUp", 0xE1D9 },  // glyphnumber: 1513, 16th note (semiquaver) stem up
            { "note256thDown", 0xE1E2 },  // glyphnumber: 1514, 256th note (demisemihemidemisemiquaver) stem down
            { "note256thUp", 0xE1E1 },  // glyphnumber: 1515, 256th note (demisemihemidemisemiquaver) stem up
            { "note32ndDown", 0xE1DC },  // glyphnumber: 1516, 32nd note (demisemiquaver) stem down
            { "note32ndUp", 0xE1DB },  // glyphnumber: 1517, 32nd note (demisemiquaver) stem up
            { "note512thDown", 0xE1E4 },  // glyphnumber: 1518, 512th note (hemidemisemihemidemisemiquaver) stem down
            { "note512thUp", 0xE1E3 },  // glyphnumber: 1519, 512th note (hemidemisemihemidemisemiquaver) stem up
            { "note64thDown", 0xE1DE },  // glyphnumber: 1520, 64th note (hemidemisemiquaver) stem down
            { "note64thUp", 0xE1DD },  // glyphnumber: 1521, 64th note (hemidemisemiquaver) stem up
            { "note8thDown", 0xE1D8 },  // glyphnumber: 1522, Eighth note (quaver) stem down
            { "note8thUp", 0xE1D7 },  // glyphnumber: 1523, Eighth note (quaver) stem up
            { "noteABlack", 0xE197 },  // glyphnumber: 1524, A (black note)
            { "noteAFlatBlack", 0xE196 },  // glyphnumber: 1525, A flat (black note)
            { "noteAFlatHalf", 0xE17F },  // glyphnumber: 1526, A flat (half note)
            { "noteAFlatWhole", 0xE168 },  // glyphnumber: 1527, A flat (whole note)
            { "noteAHalf", 0xE180 },  // glyphnumber: 1528, A (half note)
            { "noteASharpBlack", 0xE198 },  // glyphnumber: 1529, A sharp (black note)
            { "noteASharpHalf", 0xE181 },  // glyphnumber: 1530, A sharp (half note)
            { "noteASharpWhole", 0xE16A },  // glyphnumber: 1531, A sharp (whole note)
            { "noteAWhole", 0xE169 },  // glyphnumber: 1532, A (whole note)
            { "noteBBlack", 0xE19A },  // glyphnumber: 1533, B (black note)
            { "noteBFlatBlack", 0xE199 },  // glyphnumber: 1534, B flat (black note)
            { "noteBFlatHalf", 0xE182 },  // glyphnumber: 1535, B flat (half note)
            { "noteBFlatWhole", 0xE16B },  // glyphnumber: 1536, B flat (whole note)
            { "noteBHalf", 0xE183 },  // glyphnumber: 1537, B (half note)
            { "noteBSharpBlack", 0xE19B },  // glyphnumber: 1538, B sharp (black note)
            { "noteBSharpHalf", 0xE184 },  // glyphnumber: 1539, B sharp (half note)
            { "noteBSharpWhole", 0xE16D },  // glyphnumber: 1540, B sharp (whole note)
            { "noteBWhole", 0xE16C },  // glyphnumber: 1541, B (whole note)
            { "noteCBlack", 0xE19D },  // glyphnumber: 1542, C (black note)
            { "noteCFlatBlack", 0xE19C },  // glyphnumber: 1543, C flat (black note)
            { "noteCFlatHalf", 0xE185 },  // glyphnumber: 1544, C flat (half note)
            { "noteCFlatWhole", 0xE16E },  // glyphnumber: 1545, C flat (whole note)
            { "noteCHalf", 0xE186 },  // glyphnumber: 1546, C (half note)
            { "noteCSharpBlack", 0xE19E },  // glyphnumber: 1547, C sharp (black note)
            { "noteCSharpHalf", 0xE187 },  // glyphnumber: 1548, C sharp (half note)
            { "noteCSharpWhole", 0xE170 },  // glyphnumber: 1549, C sharp (whole note)
            { "noteCWhole", 0xE16F },  // glyphnumber: 1550, C (whole note)
            { "noteDBlack", 0xE1A0 },  // glyphnumber: 1551, D (black note)
            { "noteDFlatBlack", 0xE19F },  // glyphnumber: 1552, D flat (black note)
            { "noteDFlatHalf", 0xE188 },  // glyphnumber: 1553, D flat (half note)
            { "noteDFlatWhole", 0xE171 },  // glyphnumber: 1554, D flat (whole note)
            { "noteDHalf", 0xE189 },  // glyphnumber: 1555, D (half note)
            { "noteDSharpBlack", 0xE1A1 },  // glyphnumber: 1556, D sharp (black note)
            { "noteDSharpHalf", 0xE18A },  // glyphnumber: 1557, D sharp (half note)
            { "noteDSharpWhole", 0xE173 },  // glyphnumber: 1558, D sharp (whole note)
            { "noteDWhole", 0xE172 },  // glyphnumber: 1559, D (whole note)
            { "noteDoBlack", 0xE160 },  // glyphnumber: 1560, Do (black note)
            { "noteDoHalf", 0xE158 },  // glyphnumber: 1561, Do (half note)
            { "noteDoWhole", 0xE150 },  // glyphnumber: 1562, Do (whole note)
            { "noteDoubleWhole", 0xE1D0 },  // glyphnumber: 1563, Double whole note (breve)
            { "noteDoubleWholeSquare", 0xE1D1 },  // glyphnumber: 1564, Double whole note (square)
            { "noteEBlack", 0xE1A3 },  // glyphnumber: 1565, E (black note)
            { "noteEFlatBlack", 0xE1A2 },  // glyphnumber: 1566, E flat (black note)
            { "noteEFlatHalf", 0xE18B },  // glyphnumber: 1567, E flat (half note)
            { "noteEFlatWhole", 0xE174 },  // glyphnumber: 1568, E flat (whole note)
            { "noteEHalf", 0xE18C },  // glyphnumber: 1569, E (half note)
            { "noteESharpBlack", 0xE1A4 },  // glyphnumber: 1570, E sharp (black note)
            { "noteESharpHalf", 0xE18D },  // glyphnumber: 1571, E sharp (half note)
            { "noteESharpWhole", 0xE176 },  // glyphnumber: 1572, E sharp (whole note)
            { "noteEWhole", 0xE175 },  // glyphnumber: 1573, E (whole note)
            { "noteEmptyBlack", 0xE1AF },  // glyphnumber: 1574, Empty black note
            { "noteEmptyHalf", 0xE1AE },  // glyphnumber: 1575, Empty half note
            { "noteEmptyWhole", 0xE1AD },  // glyphnumber: 1576, Empty whole note
            { "noteFBlack", 0xE1A6 },  // glyphnumber: 1577, F (black note)
            { "noteFFlatBlack", 0xE1A5 },  // glyphnumber: 1578, F flat (black note)
            { "noteFFlatHalf", 0xE18E },  // glyphnumber: 1579, F flat (half note)
            { "noteFFlatWhole", 0xE177 },  // glyphnumber: 1580, F flat (whole note)
            { "noteFHalf", 0xE18F },  // glyphnumber: 1581, F (half note)
            { "noteFSharpBlack", 0xE1A7 },  // glyphnumber: 1582, F sharp (black note)
            { "noteFSharpHalf", 0xE190 },  // glyphnumber: 1583, F sharp (half note)
            { "noteFSharpWhole", 0xE179 },  // glyphnumber: 1584, F sharp (whole note)
            { "noteFWhole", 0xE178 },  // glyphnumber: 1585, F (whole note)
            { "noteFaBlack", 0xE163 },  // glyphnumber: 1586, Fa (black note)
            { "noteFaHalf", 0xE15B },  // glyphnumber: 1587, Fa (half note)
            { "noteFaWhole", 0xE153 },  // glyphnumber: 1588, Fa (whole note)
            { "noteGBlack", 0xE1A9 },  // glyphnumber: 1589, G (black note)
            { "noteGFlatBlack", 0xE1A8 },  // glyphnumber: 1590, G flat (black note)
            { "noteGFlatHalf", 0xE191 },  // glyphnumber: 1591, G flat (half note)
            { "noteGFlatWhole", 0xE17A },  // glyphnumber: 1592, G flat (whole note)
            { "noteGHalf", 0xE192 },  // glyphnumber: 1593, G (half note)
            { "noteGSharpBlack", 0xE1AA },  // glyphnumber: 1594, G sharp (black note)
            { "noteGSharpHalf", 0xE193 },  // glyphnumber: 1595, G sharp (half note)
            { "noteGSharpWhole", 0xE17C },  // glyphnumber: 1596, G sharp (whole note)
            { "noteGWhole", 0xE17B },  // glyphnumber: 1597, G (whole note)
            { "noteHBlack", 0xE1AB },  // glyphnumber: 1598, H (black note)
            { "noteHHalf", 0xE194 },  // glyphnumber: 1599, H (half note)
            { "noteHSharpBlack", 0xE1AC },  // glyphnumber: 1600, H sharp (black note)
            { "noteHSharpHalf", 0xE195 },  // glyphnumber: 1601, H sharp (half note)
            { "noteHSharpWhole", 0xE17E },  // glyphnumber: 1602, H sharp (whole note)
            { "noteHWhole", 0xE17D },  // glyphnumber: 1603, H (whole note)
            { "noteHalfDown", 0xE1D4 },  // glyphnumber: 1604, Half note (minim) stem down
            { "noteHalfUp", 0xE1D3 },  // glyphnumber: 1605, Half note (minim) stem up
            { "noteLaBlack", 0xE165 },  // glyphnumber: 1606, La (black note)
            { "noteLaHalf", 0xE15D },  // glyphnumber: 1607, La (half note)
            { "noteLaWhole", 0xE155 },  // glyphnumber: 1608, La (whole note)
            { "noteMiBlack", 0xE162 },  // glyphnumber: 1609, Mi (black note)
            { "noteMiHalf", 0xE15A },  // glyphnumber: 1610, Mi (half note)
            { "noteMiWhole", 0xE152 },  // glyphnumber: 1611, Mi (whole note)
            { "noteQuarterDown", 0xE1D6 },  // glyphnumber: 1612, Quarter note (crotchet) stem down
            { "noteQuarterUp", 0xE1D5 },  // glyphnumber: 1613, Quarter note (crotchet) stem up
            { "noteReBlack", 0xE161 },  // glyphnumber: 1614, Re (black note)
            { "noteReHalf", 0xE159 },  // glyphnumber: 1615, Re (half note)
            { "noteReWhole", 0xE151 },  // glyphnumber: 1616, Re (whole note)
            { "noteShapeArrowheadLeftBlack", 0xE1C9 },  // glyphnumber: 1617, Arrowhead left black (Funk 7-shape re)
            { "noteShapeArrowheadLeftDoubleWhole", 0xECDC },  // glyphnumber: 1618, Arrowhead left double whole (Funk 7-shape re)
            { "noteShapeArrowheadLeftWhite", 0xE1C8 },  // glyphnumber: 1619, Arrowhead left white (Funk 7-shape re)
            { "noteShapeDiamondBlack", 0xE1B9 },  // glyphnumber: 1620, Diamond black (4-shape mi; 7-shape mi)
            { "noteShapeDiamondDoubleWhole", 0xECD4 },  // glyphnumber: 1621, Diamond double whole (4-shape mi; 7-shape mi)
            { "noteShapeDiamondWhite", 0xE1B8 },  // glyphnumber: 1622, Diamond white (4-shape mi; 7-shape mi)
            { "noteShapeIsoscelesTriangleBlack", 0xE1C5 },  // glyphnumber: 1623, Isosceles triangle black (Walker 7-shape ti)
            { "noteShapeIsoscelesTriangleDoubleWhole", 0xECDA },  // glyphnumber: 1624, Isosceles triangle double whole (Walker 7-shape ti)
            { "noteShapeIsoscelesTriangleWhite", 0xE1C4 },  // glyphnumber: 1625, Isosceles triangle white (Walker 7-shape ti)
            { "noteShapeKeystoneBlack", 0xE1C1 },  // glyphnumber: 1626, Inverted keystone black (Walker 7-shape do)
            { "noteShapeKeystoneDoubleWhole", 0xECD8 },  // glyphnumber: 1627, Inverted keystone double whole (Walker 7-shape do)
            { "noteShapeKeystoneWhite", 0xE1C0 },  // glyphnumber: 1628, Inverted keystone white (Walker 7-shape do)
            { "noteShapeMoonBlack", 0xE1BD },  // glyphnumber: 1629, Moon black (Aikin 7-shape re)
            { "noteShapeMoonDoubleWhole", 0xECD6 },  // glyphnumber: 1630, Moon double whole (Aikin 7-shape re)
            { "noteShapeMoonLeftBlack", 0xE1C7 },  // glyphnumber: 1631, Moon left black (Funk 7-shape do)
            { "noteShapeMoonLeftDoubleWhole", 0xECDB },  // glyphnumber: 1632, Moon left double whole (Funk 7-shape do)
            { "noteShapeMoonLeftWhite", 0xE1C6 },  // glyphnumber: 1633, Moon left white (Funk 7-shape do)
            { "noteShapeMoonWhite", 0xE1BC },  // glyphnumber: 1634, Moon white (Aikin 7-shape re)
            { "noteShapeQuarterMoonBlack", 0xE1C3 },  // glyphnumber: 1635, Quarter moon black (Walker 7-shape re)
            { "noteShapeQuarterMoonDoubleWhole", 0xECD9 },  // glyphnumber: 1636, Quarter moon double whole (Walker 7-shape re)
            { "noteShapeQuarterMoonWhite", 0xE1C2 },  // glyphnumber: 1637, Quarter moon white (Walker 7-shape re)
            { "noteShapeRoundBlack", 0xE1B1 },  // glyphnumber: 1638, Round black (4-shape sol; 7-shape so)
            { "noteShapeRoundDoubleWhole", 0xECD0 },  // glyphnumber: 1639, Round double whole (4-shape sol; 7-shape so)
            { "noteShapeRoundWhite", 0xE1B0 },  // glyphnumber: 1640, Round white (4-shape sol; 7-shape so)
            { "noteShapeSquareBlack", 0xE1B3 },  // glyphnumber: 1641, Square black (4-shape la; Aikin 7-shape la)
            { "noteShapeSquareDoubleWhole", 0xECD1 },  // glyphnumber: 1642, Square double whole (4-shape la; Aikin 7-shape la)
            { "noteShapeSquareWhite", 0xE1B2 },  // glyphnumber: 1643, Square white (4-shape la; Aikin 7-shape la)
            { "noteShapeTriangleLeftBlack", 0xE1B7 },  // glyphnumber: 1644, Triangle left black (stem up; 4-shape fa; 7-shape fa)
            { "noteShapeTriangleLeftDoubleWhole", 0xECD3 },  // glyphnumber: 1645, Triangle left double whole (stem up; 4-shape fa; 7-shape fa)
            { "noteShapeTriangleLeftWhite", 0xE1B6 },  // glyphnumber: 1646, Triangle left white (stem up; 4-shape fa; 7-shape fa)
            { "noteShapeTriangleRightBlack", 0xE1B5 },  // glyphnumber: 1647, Triangle right black (stem down; 4-shape fa; 7-shape fa)
            { "noteShapeTriangleRightDoubleWhole", 0xECD2 },  // glyphnumber: 1648, Triangle right double whole (stem down; 4-shape fa; 7-shape fa)
            { "noteShapeTriangleRightWhite", 0xE1B4 },  // glyphnumber: 1649, Triangle right white (stem down; 4-shape fa; 7-shape fa)
            { "noteShapeTriangleRoundBlack", 0xE1BF },  // glyphnumber: 1650, Triangle-round black (Aikin 7-shape ti)
            { "noteShapeTriangleRoundDoubleWhole", 0xECD7 },  // glyphnumber: 1651, Triangle-round white (Aikin 7-shape ti)
            { "noteShapeTriangleRoundLeftBlack", 0xE1CB },  // glyphnumber: 1652, Triangle-round left black (Funk 7-shape ti)
            { "noteShapeTriangleRoundLeftDoubleWhole", 0xECDD },  // glyphnumber: 1653, Triangle-round left double whole (Funk 7-shape ti)
            { "noteShapeTriangleRoundLeftWhite", 0xE1CA },  // glyphnumber: 1654, Triangle-round left white (Funk 7-shape ti)
            { "noteShapeTriangleRoundWhite", 0xE1BE },  // glyphnumber: 1655, Triangle-round white (Aikin 7-shape ti)
            { "noteShapeTriangleUpBlack", 0xE1BB },  // glyphnumber: 1656, Triangle up black (Aikin 7-shape do)
            { "noteShapeTriangleUpDoubleWhole", 0xECD5 },  // glyphnumber: 1657, Triangle up double whole (Aikin 7-shape do)
            { "noteShapeTriangleUpWhite", 0xE1BA },  // glyphnumber: 1658, Triangle up white (Aikin 7-shape do)
            { "noteSiBlack", 0xE167 },  // glyphnumber: 1659, Si (black note)
            { "noteSiHalf", 0xE15F },  // glyphnumber: 1660, Si (half note)
            { "noteSiWhole", 0xE157 },  // glyphnumber: 1661, Si (whole note)
            { "noteSoBlack", 0xE164 },  // glyphnumber: 1662, So (black note)
            { "noteSoHalf", 0xE15C },  // glyphnumber: 1663, So (half note)
            
            { "noteSoWhole", 0xE154 },  // glyphnumber: 1664, So (whole note)
            
            { "noteSoWhole", 0xE154 },  // glyphnumber: 1664, So (whole note)
            { "noteTiBlack", 0xE166 },  // glyphnumber: 1665, Ti (black note)
            { "noteTiHalf", 0xE15E },  // glyphnumber: 1666, Ti (half note)
            { "noteTiWhole", 0xE156 },  // glyphnumber: 1667, Ti (whole note)
            { "noteWhole", 0xE1D2 },  // glyphnumber: 1668, Whole note (semibreve)
            { "noteheadBlack", 0xE0A4 },  // glyphnumber: 1669, Black notehead
            { "noteheadCircleSlash", 0xE0F7 },  // glyphnumber: 1670, Circle slash notehead
            { "noteheadCircleX", 0xE0B3 },  // glyphnumber: 1671, Circle X notehead
            { "noteheadCircleXDoubleWhole", 0xE0B0 },  // glyphnumber: 1672, Circle X double whole
            { "noteheadCircleXHalf", 0xE0B2 },  // glyphnumber: 1673, Circle X half
            { "noteheadCircleXWhole", 0xE0B1 },  // glyphnumber: 1674, Circle X whole
            { "noteheadCircledBlack", 0xE0E4 },  // glyphnumber: 1675, Circled black notehead
            { "noteheadCircledBlackLarge", 0xE0E8 },  // glyphnumber: 1676, Black notehead in large circle
            { "noteheadCircledDoubleWhole", 0xE0E7 },  // glyphnumber: 1677, Circled double whole notehead
            { "noteheadCircledDoubleWholeLarge", 0xE0EB },  // glyphnumber: 1678, Double whole notehead in large circle
            { "noteheadCircledHalf", 0xE0E5 },  // glyphnumber: 1679, Circled half notehead
            { "noteheadCircledHalfLarge", 0xE0E9 },  // glyphnumber: 1680, Half notehead in large circle
            { "noteheadCircledWhole", 0xE0E6 },  // glyphnumber: 1681, Circled whole notehead
            { "noteheadCircledWholeLarge", 0xE0EA },  // glyphnumber: 1682, Whole notehead in large circle
            { "noteheadCircledXLarge", 0xE0EC },  // glyphnumber: 1683, Cross notehead in large circle
            { "noteheadClusterDoubleWhole2nd", 0xE124 },  // glyphnumber: 1684, Double whole note cluster, 2nd
            { "noteheadClusterDoubleWhole3rd", 0xE128 },  // glyphnumber: 1685, Double whole note cluster, 3rd
            { "noteheadClusterDoubleWholeBottom", 0xE12E },  // glyphnumber: 1686, Combining double whole note cluster, bottom
            { "noteheadClusterDoubleWholeMiddle", 0xE12D },  // glyphnumber: 1687, Combining double whole note cluster, middle
            { "noteheadClusterDoubleWholeTop", 0xE12C },  // glyphnumber: 1688, Combining double whole note cluster, top
            { "noteheadClusterHalf2nd", 0xE126 },  // glyphnumber: 1689, Half note cluster, 2nd
            { "noteheadClusterHalf3rd", 0xE12A },  // glyphnumber: 1690, Half note cluster, 3rd
            { "noteheadClusterHalfBottom", 0xE134 },  // glyphnumber: 1691, Combining half note cluster, bottom
            { "noteheadClusterHalfMiddle", 0xE133 },  // glyphnumber: 1692, Combining half note cluster, middle
            { "noteheadClusterHalfTop", 0xE132 },  // glyphnumber: 1693, Combining half note cluster, top
            { "noteheadClusterQuarter2nd", 0xE127 },  // glyphnumber: 1694, Quarter note cluster, 2nd
            { "noteheadClusterQuarter3rd", 0xE12B },  // glyphnumber: 1695, Quarter note cluster, 3rd
            { "noteheadClusterQuarterBottom", 0xE137 },  // glyphnumber: 1696, Combining quarter note cluster, bottom
            { "noteheadClusterQuarterMiddle", 0xE136 },  // glyphnumber: 1697, Combining quarter note cluster, middle
            { "noteheadClusterQuarterTop", 0xE135 },  // glyphnumber: 1698, Combining quarter note cluster, top
            { "noteheadClusterRoundBlack", 0xE123 },  // glyphnumber: 1699, Cluster notehead black (round)
            { "noteheadClusterRoundWhite", 0xE122 },  // glyphnumber: 1700, Cluster notehead white (round)
            { "noteheadClusterSquareBlack", 0xE121 },  // glyphnumber: 1701, Cluster notehead black (square)
            { "noteheadClusterSquareWhite", 0xE120 },  // glyphnumber: 1702, Cluster notehead white (square)
            { "noteheadClusterWhole2nd", 0xE125 },  // glyphnumber: 1703, Whole note cluster, 2nd
            { "noteheadClusterWhole3rd", 0xE129 },  // glyphnumber: 1704, Whole note cluster, 3rd
            { "noteheadClusterWholeBottom", 0xE131 },  // glyphnumber: 1705, Combining whole note cluster, bottom
            { "noteheadClusterWholeMiddle", 0xE130 },  // glyphnumber: 1706, Combining whole note cluster, middle
            { "noteheadClusterWholeTop", 0xE12F },  // glyphnumber: 1707, Combining whole note cluster, top
            { "noteheadDiamondBlack", 0xE0DB },  // glyphnumber: 1708, Diamond black notehead
            { "noteheadDiamondBlackOld", 0xE0E2 },  // glyphnumber: 1709, Diamond black notehead (old)
            { "noteheadDiamondBlackWide", 0xE0DC },  // glyphnumber: 1710, Diamond black notehead (wide)
            { "noteheadDiamondClusterBlack2nd", 0xE139 },  // glyphnumber: 1711, Black diamond cluster, 2nd
            { "noteheadDiamondClusterBlack3rd", 0xE13B },  // glyphnumber: 1712, Black diamond cluster, 3rd
            { "noteheadDiamondClusterBlackBottom", 0xE141 },  // glyphnumber: 1713, Combining black diamond cluster, bottom
            { "noteheadDiamondClusterBlackMiddle", 0xE140 },  // glyphnumber: 1714, Combining black diamond cluster, middle
            { "noteheadDiamondClusterBlackTop", 0xE13F },  // glyphnumber: 1715, Combining black diamond cluster, top
            { "noteheadDiamondClusterWhite2nd", 0xE138 },  // glyphnumber: 1716, White diamond cluster, 2nd
            { "noteheadDiamondClusterWhite3rd", 0xE13A },  // glyphnumber: 1717, White diamond cluster, 3rd
            { "noteheadDiamondClusterWhiteBottom", 0xE13E },  // glyphnumber: 1718, Combining white diamond cluster, bottom
            { "noteheadDiamondClusterWhiteMiddle", 0xE13D },  // glyphnumber: 1719, Combining white diamond cluster, middle
            { "noteheadDiamondClusterWhiteTop", 0xE13C },  // glyphnumber: 1720, Combining white diamond cluster, top
            { "noteheadDiamondDoubleWhole", 0xE0D7 },  // glyphnumber: 1721, Diamond double whole notehead
            { "noteheadDiamondDoubleWholeOld", 0xE0DF },  // glyphnumber: 1722, Diamond double whole notehead (old)
            { "noteheadDiamondHalf", 0xE0D9 },  // glyphnumber: 1723, Diamond half notehead
            { "noteheadDiamondHalfFilled", 0xE0E3 },  // glyphnumber: 1724, Half-filled diamond notehead
            { "noteheadDiamondHalfOld", 0xE0E1 },  // glyphnumber: 1725, Diamond half notehead (old)
            { "noteheadDiamondHalfWide", 0xE0DA },  // glyphnumber: 1726, Diamond half notehead (wide)
            { "noteheadDiamondOpen", 0xE0FC },  // glyphnumber: 1727, Open diamond notehead
            { "noteheadDiamondWhite", 0xE0DD },  // glyphnumber: 1728, Diamond white notehead
            { "noteheadDiamondWhiteWide", 0xE0DE },  // glyphnumber: 1729, Diamond white notehead (wide)
            { "noteheadDiamondWhole", 0xE0D8 },  // glyphnumber: 1730, Diamond whole notehead
            { "noteheadDiamondWholeOld", 0xE0E0 },  // glyphnumber: 1731, Diamond whole notehead (old)
            { "noteheadDoubleWhole", 0xE0A0 },  // glyphnumber: 1732, Double whole (breve) notehead
            { "noteheadDoubleWholeSquare", 0xE0A1 },  // glyphnumber: 1733, Double whole (breve) notehead (square)
            { "noteheadDoubleWholeWithX", 0xE0B4 },  // glyphnumber: 1734, Double whole notehead with X
            { "noteheadHalf", 0xE0A3 },  // glyphnumber: 1735, Half (minim) notehead
            { "noteheadHalfFilled", 0xE0FB },  // glyphnumber: 1736, Filled half (minim) notehead
            { "noteheadHalfWithX", 0xE0B6 },  // glyphnumber: 1737, Half notehead with X
            { "noteheadHeavyX", 0xE0F8 },  // glyphnumber: 1738, Heavy X notehead
            { "noteheadHeavyXHat", 0xE0F9 },  // glyphnumber: 1739, Heavy X with hat notehead
            { "noteheadLargeArrowDownBlack", 0xE0F4 },  // glyphnumber: 1740, Large arrow down (lowest pitch) black notehead
            { "noteheadLargeArrowDownDoubleWhole", 0xE0F1 },  // glyphnumber: 1741, Large arrow down (lowest pitch) double whole notehead
            
            { "noteheadLargeArrowDownHalf", 0xE0F3 },  // glyphnumber: 1742, Large arrow down (lowest pitch) half notehead
            { "noteheadLargeArrowDownWhole", 0xE0F2 },  // glyphnumber: 1743, Large arrow down (lowest pitch) whole notehead
            { "noteheadLargeArrowUpBlack", 0xE0F0 },  // glyphnumber: 1744, Large arrow up (highest pitch) black notehead
            { "noteheadLargeArrowUpDoubleWhole", 0xE0ED },  // glyphnumber: 1745, Large arrow up (highest pitch) double whole notehead
            { "noteheadLargeArrowUpHalf", 0xE0EF },  // glyphnumber: 1746, Large arrow up (highest pitch) half notehead
            { "noteheadLargeArrowUpWhole", 0xE0EE },  // glyphnumber: 1747, Large arrow up (highest pitch) whole notehead
            { "noteheadMoonBlack", 0xE0CB },  // glyphnumber: 1748, Moon notehead black
            { "noteheadMoonWhite", 0xE0CA },  // glyphnumber: 1749, Moon notehead white
            { "noteheadNull", 0xE0A5 },  // glyphnumber: 1750, Null notehead
            { "noteheadParenthesis", 0xE0CE },  // glyphnumber: 1751, Parenthesis notehead
            { "noteheadParenthesisLeft", 0xE0F5 },  // glyphnumber: 1752, Opening parenthesis
            { "noteheadParenthesisRight", 0xE0F6 },  // glyphnumber: 1753, Closing parenthesis
            { "noteheadPlusBlack", 0xE0AF },  // glyphnumber: 1754, Plus notehead black
            { "noteheadPlusDoubleWhole", 0xE0AC },  // glyphnumber: 1755, Plus notehead double whole
            { "noteheadPlusHalf", 0xE0AE },  // glyphnumber: 1756, Plus notehead half
            { "noteheadPlusWhole", 0xE0AD },  // glyphnumber: 1757, Plus notehead whole
            { "noteheadRectangularClusterBlackBottom", 0xE144 },  // glyphnumber: 1758, Combining black rectangular cluster, bottom
            { "noteheadRectangularClusterBlackMiddle", 0xE143 },  // glyphnumber: 1759, Combining black rectangular cluster, middle
            { "noteheadRectangularClusterBlackTop", 0xE142 },  // glyphnumber: 1760, Combining black rectangular cluster, top
            { "noteheadRectangularClusterWhiteBottom", 0xE147 },  // glyphnumber: 1761, Combining white rectangular cluster, bottom
            { "noteheadRectangularClusterWhiteMiddle", 0xE146 },  // glyphnumber: 1762, Combining white rectangular cluster, middle
            { "noteheadRectangularClusterWhiteTop", 0xE145 },  // glyphnumber: 1763, Combining white rectangular cluster, top
            { "noteheadRoundBlack", 0xE113 },  // glyphnumber: 1764, Round black notehead
            { "noteheadRoundBlackDoubleSlashed", 0xE11C },  // glyphnumber: 1765, Round black notehead, double slashed
            { "noteheadRoundBlackLarge", 0xE110 },  // glyphnumber: 1766, Large round black notehead
            { "noteheadRoundBlackSlashed", 0xE118 },  // glyphnumber: 1767, Round black notehead, slashed
            { "noteheadRoundBlackSlashedLarge", 0xE116 },  // glyphnumber: 1768, Large round black notehead, slashed
            { "noteheadRoundWhite", 0xE114 },  // glyphnumber: 1769, Round white notehead
            { "noteheadRoundWhiteDoubleSlashed", 0xE11D },  // glyphnumber: 1770, Round white notehead, double slashed
            { "noteheadRoundWhiteLarge", 0xE111 },  // glyphnumber: 1771, Large round white notehead
            { "noteheadRoundWhiteSlashed", 0xE119 },  // glyphnumber: 1772, Round white notehead, slashed
            { "noteheadRoundWhiteSlashedLarge", 0xE117 },  // glyphnumber: 1773, Large round white notehead, slashed
            { "noteheadRoundWhiteWithDot", 0xE115 },  // glyphnumber: 1774, Round white notehead with dot
            { "noteheadRoundWhiteWithDotLarge", 0xE112 },  // glyphnumber: 1775, Large round white notehead with dot
            { "noteheadSlashDiamondWhite", 0xE104 },  // glyphnumber: 1776, Large white diamond
            { "noteheadSlashHorizontalEnds", 0xE101 },  // glyphnumber: 1777, Slash with horizontal ends
            { "noteheadSlashHorizontalEndsMuted", 0xE108 },  // glyphnumber: 1778, Muted slash with horizontal ends
            { "noteheadSlashVerticalEnds", 0xE100 },  // glyphnumber: 1779, Slash with vertical ends
            { "noteheadSlashVerticalEndsMuted", 0xE107 },  // glyphnumber: 1780, Muted slash with vertical ends
            { "noteheadSlashVerticalEndsSmall", 0xE105 },  // glyphnumber: 1781, Small slash with vertical ends
            { "noteheadSlashWhiteDoubleWhole", 0xE10A },  // glyphnumber: 1782, White slash double whole
            { "noteheadSlashWhiteHalf", 0xE103 },  // glyphnumber: 1783, White slash half
            { "noteheadSlashWhiteMuted", 0xE109 },  // glyphnumber: 1784, Muted white slash
            { "noteheadSlashWhiteWhole", 0xE102 },  // glyphnumber: 1785, White slash whole
            { "noteheadSlashX", 0xE106 },  // glyphnumber: 1786, Large X notehead
            { "noteheadSlashedBlack1", 0xE0CF },  // glyphnumber: 1787, Slashed black notehead (bottom left to top right)
            { "noteheadSlashedBlack2", 0xE0D0 },  // glyphnumber: 1788, Slashed black notehead (top left to bottom right)
            { "noteheadSlashedDoubleWhole1", 0xE0D5 },  // glyphnumber: 1789, Slashed double whole notehead (bottom left to top right)
            { "noteheadSlashedDoubleWhole2", 0xE0D6 },  // glyphnumber: 1790, Slashed double whole notehead (top left to bottom right)
            { "noteheadSlashedHalf1", 0xE0D1 },  // glyphnumber: 1791, Slashed half notehead (bottom left to top right)
            { "noteheadSlashedHalf2", 0xE0D2 },  // glyphnumber: 1792, Slashed half notehead (top left to bottom right)
            { "noteheadSlashedWhole1", 0xE0D3 },  // glyphnumber: 1793, Slashed whole notehead (bottom left to top right)
            { "noteheadSlashedWhole2", 0xE0D4 },  // glyphnumber: 1794, Slashed whole notehead (top left to bottom right)
            { "noteheadSquareBlack", 0xE0B9 },  // glyphnumber: 1795, Square notehead black
            { "noteheadSquareBlackLarge", 0xE11A },  // glyphnumber: 1796, Large square black notehead
            { "noteheadSquareBlackWhite", 0xE11B },  // glyphnumber: 1797, Large square white notehead
            { "noteheadSquareWhite", 0xE0B8 },  // glyphnumber: 1798, Square notehead white
            { "noteheadTriangleDownBlack", 0xE0C7 },  // glyphnumber: 1799, Triangle notehead down black
            { "noteheadTriangleDownDoubleWhole", 0xE0C3 },  // glyphnumber: 1800, Triangle notehead down double whole
            { "noteheadTriangleDownHalf", 0xE0C5 },  // glyphnumber: 1801, Triangle notehead down half
            { "noteheadTriangleDownWhite", 0xE0C6 },  // glyphnumber: 1802, Triangle notehead down white
            { "noteheadTriangleDownWhole", 0xE0C4 },  // glyphnumber: 1803, Triangle notehead down whole
            { "noteheadTriangleLeftBlack", 0xE0C0 },  // glyphnumber: 1804, Triangle notehead left black
            { "noteheadTriangleLeftWhite", 0xE0BF },  // glyphnumber: 1805, Triangle notehead left white
            { "noteheadTriangleRightBlack", 0xE0C2 },  // glyphnumber: 1806, Triangle notehead right black
            { "noteheadTriangleRightWhite", 0xE0C1 },  // glyphnumber: 1807, Triangle notehead right white
            { "noteheadTriangleRoundDownBlack", 0xE0CD },  // glyphnumber: 1808, Triangle-round notehead down black
            { "noteheadTriangleRoundDownWhite", 0xE0CC },  // glyphnumber: 1809, Triangle-round notehead down white
            { "noteheadTriangleUpBlack", 0xE0BE },  // glyphnumber: 1810, Triangle notehead up black
            { "noteheadTriangleUpDoubleWhole", 0xE0BA },  // glyphnumber: 1811, Triangle notehead up double whole
            { "noteheadTriangleUpHalf", 0xE0BC },  // glyphnumber: 1812, Triangle notehead up half
            { "noteheadTriangleUpRightBlack", 0xE0C9 },  // glyphnumber: 1813, Triangle notehead up right black
            { "noteheadTriangleUpRightWhite", 0xE0C8 },  // glyphnumber: 1814, Triangle notehead up right white
            { "noteheadTriangleUpWhite", 0xE0BD },  // glyphnumber: 1815, Triangle notehead up white
            { "noteheadTriangleUpWhole", 0xE0BB },  // glyphnumber: 1816, Triangle notehead up whole
            { "noteheadVoidWithX", 0xE0B7 },  // glyphnumber: 1817, Void notehead with X
            { "noteheadWhole", 0xE0A2 },  // glyphnumber: 1818, Whole (semibreve) notehead
            { "noteheadWholeFilled", 0xE0FA },  // glyphnumber: 1819, Filled whole (semibreve) notehead
            { "noteheadWholeWithX", 0xE0B5 },  // glyphnumber: 1820, Whole notehead with X
            { "noteheadXBlack", 0xE0A9 },  // glyphnumber: 1821, X notehead black
            { "noteheadXDoubleWhole", 0xE0A6 },  // glyphnumber: 1822, X notehead double whole
            { "noteheadXHalf", 0xE0A8 },  // glyphnumber: 1823, X notehead half
            { "noteheadXOrnate", 0xE0AA },  // glyphnumber: 1824, Ornate X notehead
            { "noteheadXOrnateEllipse", 0xE0AB },  // glyphnumber: 1825, Ornate X notehead in ellipse
            { "noteheadXWhole", 0xE0A7 },  // glyphnumber: 1826, X notehead whole
            { "octaveBaselineA", 0xEC91 },  // glyphnumber: 1827, a (baseline)
            { "octaveBaselineB", 0xEC93 },  // glyphnumber: 1828, b (baseline)
            { "octaveBaselineM", 0xEC95 },  // glyphnumber: 1829, m (baseline)
            { "octaveBaselineV", 0xEC97 },  // glyphnumber: 1830, v (baseline)
            { "octaveBassa", 0xE51F },  // glyphnumber: 1831, Bassa
            { "octaveLoco", 0xEC90 },  // glyphnumber: 1832, Loco
            { "octaveParensLeft", 0xE51A },  // glyphnumber: 1833, Left parenthesis for octave signs
            { "octaveParensRight", 0xE51B },  // glyphnumber: 1834, Right parenthesis for octave signs
            { "octaveSuperscriptA", 0xEC92 },  // glyphnumber: 1835, a (superscript)
            { "octaveSuperscriptB", 0xEC94 },  // glyphnumber: 1836, b (superscript)
            { "octaveSuperscriptM", 0xEC96 },  // glyphnumber: 1837, m (superscript)
            { "octaveSuperscriptV", 0xEC98 },  // glyphnumber: 1838, v (superscript)
            { "ornamentBottomLeftConcaveStroke", 0xE59A },  // glyphnumber: 1839, Ornament bottom left concave stroke
            { "ornamentBottomLeftConcaveStrokeLarge", 0xE59B },  // glyphnumber: 1840, Ornament bottom left concave stroke, large
            { "ornamentBottomLeftConvexStroke", 0xE59C },  // glyphnumber: 1841, Ornament bottom left convex stroke
            { "ornamentBottomRightConcaveStroke", 0xE5A7 },  // glyphnumber: 1842, Ornament bottom right concave stroke
            { "ornamentBottomRightConvexStroke", 0xE5A8 },  // glyphnumber: 1843, Ornament bottom right convex stroke
            { "ornamentComma", 0xE581 },  // glyphnumber: 1844, Comma
            { "ornamentDoubleObliqueLinesAfterNote", 0xE57E },  // glyphnumber: 1845, Double oblique straight lines NW-SE
            { "ornamentDoubleObliqueLinesBeforeNote", 0xE57D },  // glyphnumber: 1846, Double oblique straight lines SW-NE
            { "ornamentDownCurve", 0xE578 },  // glyphnumber: 1847, Curve below
            { "ornamentHaydn", 0xE56F },  // glyphnumber: 1848, Haydn ornament
            { "ornamentHighLeftConcaveStroke", 0xE592 },  // glyphnumber: 1849, Ornament high left concave stroke
            { "ornamentHighLeftConvexStroke", 0xE593 },  // glyphnumber: 1850, Ornament high left convex stroke
            { "ornamentHighRightConcaveStroke", 0xE5A2 },  // glyphnumber: 1851, Ornament high right concave stroke
            { "ornamentHighRightConvexStroke", 0xE5A3 },  // glyphnumber: 1852, Ornament high right convex stroke
            { "ornamentHookAfterNote", 0xE576 },  // glyphnumber: 1853, Hook after note
            { "ornamentHookBeforeNote", 0xE575 },  // glyphnumber: 1854, Hook before note
            { "ornamentLeftFacingHalfCircle", 0xE572 },  // glyphnumber: 1855, Left-facing half circle
            { "ornamentLeftFacingHook", 0xE574 },  // glyphnumber: 1856, Left-facing hook
            { "ornamentLeftPlus", 0xE597 },  // glyphnumber: 1857, Ornament left +
            { "ornamentLeftShakeT", 0xE596 },  // glyphnumber: 1858, Ornament left shake t
            { "ornamentLeftVerticalStroke", 0xE594 },  // glyphnumber: 1859, Ornament left vertical stroke
            { "ornamentLeftVerticalStrokeWithCross", 0xE595 },  // glyphnumber: 1860, Ornament left vertical stroke with cross (+)
            { "ornamentLowLeftConcaveStroke", 0xE598 },  // glyphnumber: 1861, Ornament low left concave stroke
            { "ornamentLowLeftConvexStroke", 0xE599 },  // glyphnumber: 1862, Ornament low left convex stroke
            { "ornamentLowRightConcaveStroke", 0xE5A5 },  // glyphnumber: 1863, Ornament low right concave stroke
            { "ornamentLowRightConvexStroke", 0xE5A6 },  // glyphnumber: 1864, Ornament low right convex stroke
            { "ornamentMiddleVerticalStroke", 0xE59F },  // glyphnumber: 1865, Ornament middle vertical stroke
            { "ornamentMordent", 0xE56C },  // glyphnumber: 1866, Mordent
            { "ornamentMordentInverted", 0xE56D },  // glyphnumber: 1867, Inverted mordent
            { "ornamentObliqueLineAfterNote", 0xE57C },  // glyphnumber: 1868, Oblique straight line NW-SE
            { "ornamentObliqueLineBeforeNote", 0xE57B },  // glyphnumber: 1869, Oblique straight line SW-NE
            { "ornamentObliqueLineHorizAfterNote", 0xE580 },  // glyphnumber: 1870, Oblique straight line tilted NW-SE
            { "ornamentObliqueLineHorizBeforeNote", 0xE57F },  // glyphnumber: 1871, Oblique straight line tilted SW-NE
            { "ornamentOriscus", 0xEA21 },  // glyphnumber: 1872, Oriscus
            { "ornamentPinceCouperin", 0xE588 },  // glyphnumber: 1873, Pincé (Couperin)
            { "ornamentPortDeVoixV", 0xE570 },  // glyphnumber: 1874, Port de voix
            { "ornamentPrecompAppoggTrill", 0xE5B2 },  // glyphnumber: 1875, Supported appoggiatura trill
            { "ornamentPrecompAppoggTrillSuffix", 0xE5B3 },  // glyphnumber: 1876, Supported appoggiatura trill with two-note suffix
            { "ornamentPrecompCadence", 0xE5BE },  // glyphnumber: 1877, Cadence
            { "ornamentPrecompCadenceUpperPrefix", 0xE5C1 },  // glyphnumber: 1878, Cadence with upper prefix
            { "ornamentPrecompCadenceUpperPrefixTurn", 0xE5C2 },  // glyphnumber: 1879, Cadence with upper prefix and turn
            { "ornamentPrecompCadenceWithTurn", 0xE5BF },  // glyphnumber: 1880, Cadence with turn
            { "ornamentPrecompDescendingSlide", 0xE5B1 },  // glyphnumber: 1881, Descending slide
            { "ornamentPrecompDoubleCadenceLowerPrefix", 0xE5C0 },  // glyphnumber: 1882, Double cadence with lower prefix
            { "ornamentPrecompDoubleCadenceUpperPrefix", 0xE5C3 },  // glyphnumber: 1883, Double cadence with upper prefix
            { "ornamentPrecompDoubleCadenceUpperPrefixTurn", 0xE5C4 },  // glyphnumber: 1884, Double cadence with upper prefix and turn
            { "ornamentPrecompInvertedMordentUpperPrefix", 0xE5C7 },  // glyphnumber: 1885, Inverted mordent with upper prefix
            { "ornamentPrecompMordentRelease", 0xE5C5 },  // glyphnumber: 1886, Mordent with release
            { "ornamentPrecompMordentUpperPrefix", 0xE5C6 },  // glyphnumber: 1887, Mordent with upper prefix
            { "ornamentPrecompPortDeVoixMordent", 0xE5BC },  // glyphnumber: 1888, Pre-beat port de voix follwed by multiple mordent (Dandrieu)
            { "ornamentPrecompSlide", 0xE5B0 },  // glyphnumber: 1889, Slide
            { "ornamentPrecompSlideTrillBach", 0xE5B8 },  // glyphnumber: 1890, Slide-trill with two-note suffix (J.S. Bach)
            { "ornamentPrecompSlideTrillDAnglebert", 0xE5B5 },  // glyphnumber: 1891, Slide-trill (D'Anglebert)
            { "ornamentPrecompSlideTrillMarpurg", 0xE5B6 },  // glyphnumber: 1892, Slide-trill with one-note suffix (Marpurg)
            { "ornamentPrecompSlideTrillMuffat", 0xE5B9 },  // glyphnumber: 1893, Slide-trill (Muffat)
            { "ornamentPrecompSlideTrillSuffixMuffat", 0xE5BA },  // glyphnumber: 1894, Slide-trill with two-note suffix (Muffat)
            { "ornamentPrecompTrillLowerSuffix", 0xE5C8 },  // glyphnumber: 1895, Trill with lower suffix
            { "ornamentPrecompTrillSuffixDandrieu", 0xE5BB },  // glyphnumber: 1896, Trill with two-note suffix (Dandrieu)
            { "ornamentPrecompTrillWithMordent", 0xE5BD },  // glyphnumber: 1897, Trill with mordent
            { "ornamentPrecompTurnTrillBach", 0xE5B7 },  // glyphnumber: 1898, Turn-trill with two-note suffix (J.S. Bach)
            { "ornamentPrecompTurnTrillDAnglebert", 0xE5B4 },  // glyphnumber: 1899, Turn-trill (D'Anglebert)
            { "ornamentQuilisma", 0xEA20 },  // glyphnumber: 1900, Quilisma
            { "ornamentRightFacingHalfCircle", 0xE571 },  // glyphnumber: 1901, Right-facing half circle
            { "ornamentRightFacingHook", 0xE573 },  // glyphnumber: 1902, Right-facing hook
            { "ornamentRightVerticalStroke", 0xE5A4 },  // glyphnumber: 1903, Ornament right vertical stroke
            { "ornamentSchleifer", 0xE587 },  // glyphnumber: 1904, Schleifer (long mordent)
            { "ornamentShake3", 0xE582 },  // glyphnumber: 1905, Shake
            { "ornamentShakeMuffat1", 0xE584 },  // glyphnumber: 1906, Shake (Muffat)
            { "ornamentShortObliqueLineAfterNote", 0xE57A },  // glyphnumber: 1907, Short oblique straight line NW-SE
            { "ornamentShortObliqueLineBeforeNote", 0xE579 },  // glyphnumber: 1908, Short oblique straight line SW-NE
            { "ornamentTopLeftConcaveStroke", 0xE590 },  // glyphnumber: 1909, Ornament top left concave stroke
            { "ornamentTopLeftConvexStroke", 0xE591 },  // glyphnumber: 1910, Ornament top left convex stroke
            { "ornamentTopRightConcaveStroke", 0xE5A0 },  // glyphnumber: 1911, Ornament top right concave stroke
            { "ornamentTopRightConvexStroke", 0xE5A1 },  // glyphnumber: 1912, Ornament top right convex stroke
            { "ornamentTremblement", 0xE56E },  // glyphnumber: 1913, Tremblement
            { "ornamentTremblementCouperin", 0xE589 },  // glyphnumber: 1914, Tremblement appuyé (Couperin)
            { "ornamentTrill", 0xE566 },  // glyphnumber: 1915, Trill
            { "ornamentTurn", 0xE567 },  // glyphnumber: 1916, Turn
            { "ornamentTurnInverted", 0xE568 },  // glyphnumber: 1917, Inverted turn
            { "ornamentTurnSlash", 0xE569 },  // glyphnumber: 1918, Turn with slash
            { "ornamentTurnUp", 0xE56A },  // glyphnumber: 1919, Turn up
            { "ornamentTurnUpS", 0xE56B },  // glyphnumber: 1920, Inverted turn up
            { "ornamentUpCurve", 0xE577 },  // glyphnumber: 1921, Curve above
            { "ornamentVerticalLine", 0xE583 },  // glyphnumber: 1922, Vertical line
            { "ornamentZigZagLineNoRightEnd", 0xE59D },  // glyphnumber: 1923, Ornament zig-zag line without right-hand end
            { "ornamentZigZagLineWithRightEnd", 0xE59E },  // glyphnumber: 1924, Ornament zig-zag line with right-hand end
            { "ottava", 0xE510 },  // glyphnumber: 1925, Ottava
            { "ottavaAlta", 0xE511 },  // glyphnumber: 1926, Ottava alta
            { "ottavaBassa", 0xE512 },  // glyphnumber: 1927, Ottava bassa
            { "ottavaBassaBa", 0xE513 },  // glyphnumber: 1928, Ottava bassa (ba)
            { "ottavaBassaVb", 0xE51C },  // glyphnumber: 1929, Ottava bassa (8vb)
            { "pendereckiTremolo", 0xE22B },  // glyphnumber: 1930, Penderecki unmeasured tremolo
            { "pictAgogo", 0xE717 },  // glyphnumber: 1931, Agogo
            { "pictAlmglocken", 0xE712 },  // glyphnumber: 1932, Almglocken
            { "pictAnvil", 0xE701 },  // glyphnumber: 1933, Anvil
            { "pictBambooChimes", 0xE6C3 },  // glyphnumber: 1934, Bamboo tube chimes
            { "pictBambooScraper", 0xE6FB },  // glyphnumber: 1935, Bamboo scraper
            { "pictBassDrum", 0xE6D4 },  // glyphnumber: 1936, Bass drum
            { "pictBassDrumOnSide", 0xE6D5 },  // glyphnumber: 1937, Bass drum on side
            { "pictBeaterBow", 0xE7DE },  // glyphnumber: 1938, Bow
            { "pictBeaterBox", 0xE7EB },  // glyphnumber: 1939, Box for percussion beater
            { "pictBeaterBrassMalletsDown", 0xE7DA },  // glyphnumber: 1940, Brass mallets down
            { "pictBeaterBrassMalletsUp", 0xE7D9 },  // glyphnumber: 1941, Brass mallets up
            { "pictBeaterCombiningDashedCircle", 0xE7EA },  // glyphnumber: 1942, Combining dashed circle for round beaters (plated)
            { "pictBeaterCombiningParentheses", 0xE7E9 },  // glyphnumber: 1943, Combining parentheses for round beaters (padded)
            { "pictBeaterDoubleBassDrumDown", 0xE7A1 },  // glyphnumber: 1944, Double bass drum stick down
            { "pictBeaterDoubleBassDrumUp", 0xE7A0 },  // glyphnumber: 1945, Double bass drum stick up
            { "pictBeaterFinger", 0xE7E4 },  // glyphnumber: 1946, Finger
            { "pictBeaterFingernails", 0xE7E6 },  // glyphnumber: 1947, Fingernails
            { "pictBeaterFist", 0xE7E5 },  // glyphnumber: 1948, Fist
            { "pictBeaterGuiroScraper", 0xE7DD },  // glyphnumber: 1949, Guiro scraper
            { "pictBeaterHammer", 0xE7E1 },  // glyphnumber: 1950, Hammer
            { "pictBeaterHammerMetalDown", 0xE7D0 },  // glyphnumber: 1951, Metal hammer, down
            { "pictBeaterHammerMetalUp", 0xE7CF },  // glyphnumber: 1952, Metal hammer, up
            { "pictBeaterHammerPlasticDown", 0xE7CE },  // glyphnumber: 1953, Plastic hammer, down
            { "pictBeaterHammerPlasticUp", 0xE7CD },  // glyphnumber: 1954, Plastic hammer, up
            { "pictBeaterHammerWoodDown", 0xE7CC },  // glyphnumber: 1955, Wooden hammer, down
            { "pictBeaterHammerWoodUp", 0xE7CB },  // glyphnumber: 1956, Wooden hammer, up
            { "pictBeaterHand", 0xE7E3 },  // glyphnumber: 1957, Hand
            { "pictBeaterHardBassDrumDown", 0xE79D },  // glyphnumber: 1958, Hard bass drum stick down
            { "pictBeaterHardBassDrumUp", 0xE79C },  // glyphnumber: 1959, Hard bass drum stick up
            { "pictBeaterHardGlockenspielDown", 0xE785 },  // glyphnumber: 1960, Hard glockenspiel stick down
            { "pictBeaterHardGlockenspielLeft", 0xE787 },  // glyphnumber: 1961, Hard glockenspiel stick left
            { "pictBeaterHardGlockenspielRight", 0xE786 },  // glyphnumber: 1962, Hard glockenspiel stick right
            { "pictBeaterHardGlockenspielUp", 0xE784 },  // glyphnumber: 1963, Hard glockenspiel stick up
            { "pictBeaterHardTimpaniDown", 0xE791 },  // glyphnumber: 1964, Hard timpani stick down
            { "pictBeaterHardTimpaniLeft", 0xE793 },  // glyphnumber: 1965, Hard timpani stick left
            { "pictBeaterHardTimpaniRight", 0xE792 },  // glyphnumber: 1966, Hard timpani stick right
            { "pictBeaterHardTimpaniUp", 0xE790 },  // glyphnumber: 1967, Hard timpani stick up
            { "pictBeaterHardXylophoneDown", 0xE779 },  // glyphnumber: 1968, Hard xylophone stick down
            { "pictBeaterHardXylophoneLeft", 0xE77B },  // glyphnumber: 1969, Hard xylophone stick left
            { "pictBeaterHardXylophoneRight", 0xE77A },  // glyphnumber: 1970, Hard xylophone stick right
            { "pictBeaterHardXylophoneUp", 0xE778 },  // glyphnumber: 1971, Hard xylophone stick up
            { "pictBeaterHardYarnDown", 0xE7AB },  // glyphnumber: 1972, Hard yarn beater down
            { "pictBeaterHardYarnLeft", 0xE7AD },  // glyphnumber: 1973, Hard yarn beater left
            { "pictBeaterHardYarnRight", 0xE7AC },  // glyphnumber: 1974, Hard yarn beater right
            { "pictBeaterHardYarnUp", 0xE7AA },  // glyphnumber: 1975, Hard yarn beater up
            { "pictBeaterJazzSticksDown", 0xE7D4 },  // glyphnumber: 1976, Jazz sticks down
            { "pictBeaterJazzSticksUp", 0xE7D3 },  // glyphnumber: 1977, Jazz sticks up
            { "pictBeaterKnittingNeedle", 0xE7E2 },  // glyphnumber: 1978, Knitting needle
            { "pictBeaterMallet", 0xE7DF },  // glyphnumber: 1979, Chime hammer
            { "pictBeaterMediumBassDrumDown", 0xE79B },  // glyphnumber: 1980, Medium bass drum stick down
            { "pictBeaterMediumBassDrumUp", 0xE79A },  // glyphnumber: 1981, Medium bass drum stick up
            { "pictBeaterMediumTimpaniDown", 0xE78D },  // glyphnumber: 1982, Medium timpani stick down
            { "pictBeaterMediumTimpaniLeft", 0xE78F },  // glyphnumber: 1983, Medium timpani stick left
            { "pictBeaterMediumTimpaniRight", 0xE78E },  // glyphnumber: 1984, Medium timpani stick right
            { "pictBeaterMediumTimpaniUp", 0xE78C },  // glyphnumber: 1985, Medium timpani stick up
            { "pictBeaterMediumXylophoneDown", 0xE775 },  // glyphnumber: 1986, Medium xylophone stick down
            { "pictBeaterMediumXylophoneLeft", 0xE777 },  // glyphnumber: 1987, Medium xylophone stick left
            { "pictBeaterMediumXylophoneRight", 0xE776 },  // glyphnumber: 1988, Medium xylophone stick right
            { "pictBeaterMediumXylophoneUp", 0xE774 },  // glyphnumber: 1989, Medium xylophone stick up
            { "pictBeaterMediumYarnDown", 0xE7A7 },  // glyphnumber: 1990, Medium yarn beater down
            { "pictBeaterMediumYarnLeft", 0xE7A9 },  // glyphnumber: 1991, Medium yarn beater left
            { "pictBeaterMediumYarnRight", 0xE7A8 },  // glyphnumber: 1992, Medium yarn beater right
            { "pictBeaterMediumYarnUp", 0xE7A6 },  // glyphnumber: 1993, Medium yarn beater up
            { "pictBeaterMetalBassDrumDown", 0xE79F },  // glyphnumber: 1994, Metal bass drum stick down
            { "pictBeaterMetalBassDrumUp", 0xE79E },  // glyphnumber: 1995, Metal bass drum stick up
            { "pictBeaterMetalDown", 0xE7C8 },  // glyphnumber: 1996, Metal beater down
            { "pictBeaterMetalHammer", 0xE7E0 },  // glyphnumber: 1997, Metal hammer
            { "pictBeaterMetalLeft", 0xE7CA },  // glyphnumber: 1998, Metal beater, left
            { "pictBeaterMetalRight", 0xE7C9 },  // glyphnumber: 1999, Metal beater, right
            { "pictBeaterMetalUp", 0xE7C7 },  // glyphnumber: 2000, Metal beater, up
            { "pictBeaterSnareSticksDown", 0xE7D2 },  // glyphnumber: 2001, Snare sticks down
            { "pictBeaterSnareSticksUp", 0xE7D1 },  // glyphnumber: 2002, Snare sticks up
            { "pictBeaterSoftBassDrumDown", 0xE799 },  // glyphnumber: 2003, Soft bass drum stick down
            { "pictBeaterSoftBassDrumUp", 0xE798 },  // glyphnumber: 2004, Soft bass drum stick up
            { "pictBeaterSoftGlockenspielDown", 0xE781 },  // glyphnumber: 2005, Soft glockenspiel stick down
            { "pictBeaterSoftGlockenspielLeft", 0xE783 },  // glyphnumber: 2006, Soft glockenspiel stick left
            { "pictBeaterSoftGlockenspielRight", 0xE782 },  // glyphnumber: 2007, Soft glockenspiel stick right
            { "pictBeaterSoftGlockenspielUp", 0xE780 },  // glyphnumber: 2008, Soft glockenspiel stick up
            { "pictBeaterSoftTimpaniDown", 0xE789 },  // glyphnumber: 2009, Soft timpani stick down
            { "pictBeaterSoftTimpaniLeft", 0xE78B },  // glyphnumber: 2010, Soft timpani stick left
            { "pictBeaterSoftTimpaniRight", 0xE78A },  // glyphnumber: 2011, Soft timpani stick right
            { "pictBeaterSoftTimpaniUp", 0xE788 },  // glyphnumber: 2012, Soft timpani stick up
            { "pictBeaterSoftXylophone", 0xE7DB },  // glyphnumber: 2013, Soft xylophone beaters
            { "pictBeaterSoftXylophoneDown", 0xE771 },  // glyphnumber: 2014, Soft xylophone stick down
            { "pictBeaterSoftXylophoneLeft", 0xE773 },  // glyphnumber: 2015, Soft xylophone stick left
            { "pictBeaterSoftXylophoneRight", 0xE772 },  // glyphnumber: 2016, Soft xylophone stick right
            { "pictBeaterSoftXylophoneUp", 0xE770 },  // glyphnumber: 2017, Soft xylophone stick up
            { "pictBeaterSoftYarnDown", 0xE7A3 },  // glyphnumber: 2018, Soft yarn beater down
            { "pictBeaterSoftYarnLeft", 0xE7A5 },  // glyphnumber: 2019, Soft yarn beater left
            { "pictBeaterSoftYarnRight", 0xE7A4 },  // glyphnumber: 2020, Soft yarn beater right
            { "pictBeaterSoftYarnUp", 0xE7A2 },  // glyphnumber: 2021, Soft yarn beater up
            { "pictBeaterSpoonWoodenMallet", 0xE7DC },  // glyphnumber: 2022, Spoon-shaped wooden mallet
            { "pictBeaterSuperballDown", 0xE7AF },  // glyphnumber: 2023, Superball beater down
            { "pictBeaterSuperballLeft", 0xE7B1 },  // glyphnumber: 2024, Superball beater left
            { "pictBeaterSuperballRight", 0xE7B0 },  // glyphnumber: 2025, Superball beater right
            { "pictBeaterSuperballUp", 0xE7AE },  // glyphnumber: 2026, Superball beater up
            { "pictBeaterTriangleDown", 0xE7D6 },  // glyphnumber: 2027, Triangle beater down
            { "pictBeaterTriangleUp", 0xE7D5 },  // glyphnumber: 2028, Triangle beater up
            { "pictBeaterWireBrushesDown", 0xE7D8 },  // glyphnumber: 2029, Wire brushes down
            { "pictBeaterWireBrushesUp", 0xE7D7 },  // glyphnumber: 2030, Wire brushes up
            { "pictBeaterWoodTimpaniDown", 0xE795 },  // glyphnumber: 2031, Wood timpani stick down
            { "pictBeaterWoodTimpaniLeft", 0xE797 },  // glyphnumber: 2032, Wood timpani stick left
            { "pictBeaterWoodTimpaniRight", 0xE796 },  // glyphnumber: 2033, Wood timpani stick right
            { "pictBeaterWoodTimpaniUp", 0xE794 },  // glyphnumber: 2034, Wood timpani stick up
            { "pictBeaterWoodXylophoneDown", 0xE77D },  // glyphnumber: 2035, Wood xylophone stick down
            { "pictBeaterWoodXylophoneLeft", 0xE77F },  // glyphnumber: 2036, Wood xylophone stick left
            { "pictBeaterWoodXylophoneRight", 0xE77E },  // glyphnumber: 2037, Wood xylophone stick right
            { "pictBeaterWoodXylophoneUp", 0xE77C },  // glyphnumber: 2038, Wood xylophone stick up
            { "pictBell", 0xE714 },  // glyphnumber: 2039, Bell
            { "pictBellOfCymbal", 0xE72A },  // glyphnumber: 2040, Bell of cymbal
            { "pictBellPlate", 0xE713 },  // glyphnumber: 2041, Bell plate
            { "pictBellTree", 0xE71A },  // glyphnumber: 2042, Bell tree
            { "pictBirdWhistle", 0xE751 },  // glyphnumber: 2043, Bird whistle
            { "pictBoardClapper", 0xE6F7 },  // glyphnumber: 2044, Board clapper
            { "pictBongos", 0xE6DD },  // glyphnumber: 2045, Bongos
            { "pictBrakeDrum", 0xE6E1 },  // glyphnumber: 2046, Brake drum
            { "pictCabasa", 0xE743 },  // glyphnumber: 2047, Cabasa
            { "pictCannon", 0xE761 },  // glyphnumber: 2048, Cannon
            { "pictCarHorn", 0xE755 },  // glyphnumber: 2049, Car horn
            { "pictCastanets", 0xE6F8 },  // glyphnumber: 2050, Castanets
            { "pictCastanetsWithHandle", 0xE6F9 },  // glyphnumber: 2051, Castanets with handle
            { "pictCelesta", 0xE6B0 },  // glyphnumber: 2052, Celesta
            { "pictCencerro", 0xE716 },  // glyphnumber: 2053, Cencerro
            { "pictCenter1", 0xE7FE },  // glyphnumber: 2054, Center (Weinberg)
            { "pictCenter2", 0xE7FF },  // glyphnumber: 2055, Center (Ghent)
            { "pictCenter3", 0xE800 },  // glyphnumber: 2056, Center (Caltabiano)
            { "pictChainRattle", 0xE748 },  // glyphnumber: 2057, Chain rattle
            { "pictChimes", 0xE6C2 },  // glyphnumber: 2058, Chimes
            { "pictChineseCymbal", 0xE726 },  // glyphnumber: 2059, Chinese cymbal
            { "pictChokeCymbal", 0xE805 },  // glyphnumber: 2060, Choke (Weinberg)
            { "pictClaves", 0xE6F2 },  // glyphnumber: 2061, Claves
            { "pictCoins", 0xE7E7 },  // glyphnumber: 2062, Coins
            { "pictConga", 0xE6DE },  // glyphnumber: 2063, Conga
            { "pictCowBell", 0xE711 },  // glyphnumber: 2064, Cow bell
            { "pictCrashCymbals", 0xE720 },  // glyphnumber: 2065, Crash cymbals
            { "pictCrotales", 0xE6AE },  // glyphnumber: 2066, Crotales
            { "pictCrushStem", 0xE80C },  // glyphnumber: 2067, Combining crush for stem
            { "pictCuica", 0xE6E4 },  // glyphnumber: 2068, Cuica
            { "pictCymbalTongs", 0xE728 },  // glyphnumber: 2069, Cymbal tongs
            { "pictDamp1", 0xE7F9 },  // glyphnumber: 2070, Damp
            { "pictDamp2", 0xE7FA },  // glyphnumber: 2071, Damp 2
            { "pictDamp3", 0xE7FB },  // glyphnumber: 2072, Damp 3
            { "pictDamp4", 0xE7FC },  // glyphnumber: 2073, Damp 4
            { "pictDeadNoteStem", 0xE80D },  // glyphnumber: 2074, Combining X for stem (dead note)
            { "pictDrumStick", 0xE7E8 },  // glyphnumber: 2075, Drum stick
            { "pictDuckCall", 0xE757 },  // glyphnumber: 2076, Duck call
            { "pictEdgeOfCymbal", 0xE729 },  // glyphnumber: 2077, Edge of cymbal
            { "pictEmptyTrap", 0xE6A9 },  // glyphnumber: 2078, Empty trapezoid
            { "pictFingerCymbals", 0xE727 },  // glyphnumber: 2079, Finger cymbals
            { "pictFlexatone", 0xE740 },  // glyphnumber: 2080, Flexatone
            { "pictFootballRatchet", 0xE6F5 },  // glyphnumber: 2081, Football rattle
            { "pictGlassHarmonica", 0xE765 },  // glyphnumber: 2082, Glass harmonica
            { "pictGlassHarp", 0xE764 },  // glyphnumber: 2083, Glass harp
            { "pictGlassPlateChimes", 0xE6C6 },  // glyphnumber: 2084, Glass plate chimes
            { "pictGlassTubeChimes", 0xE6C5 },  // glyphnumber: 2085, Glass tube chimes
            { "pictGlsp", 0xE6A0 },  // glyphnumber: 2086, Glockenspiel
            { "pictGlspSmithBrindle", 0xE6AA },  // glyphnumber: 2087, Glockenspiel (Smith Brindle)
            { "pictGobletDrum", 0xE6E2 },  // glyphnumber: 2088, Goblet drum (djembe, dumbek)
            { "pictGong", 0xE732 },  // glyphnumber: 2089, Gong
            { "pictGongWithButton", 0xE733 },  // glyphnumber: 2090, Gong with button (nipple)
            { "pictGuiro", 0xE6F3 },  // glyphnumber: 2091, Guiro
            { "pictGumHardDown", 0xE7C4 },  // glyphnumber: 2092, Hard gum beater, down
            { "pictGumHardLeft", 0xE7C6 },  // glyphnumber: 2093, Hard gum beater, left
            { "pictGumHardRight", 0xE7C5 },  // glyphnumber: 2094, Hard gum beater, right
            { "pictGumHardUp", 0xE7C3 },  // glyphnumber: 2095, Hard gum beater, up
            { "pictGumMediumDown", 0xE7C0 },  // glyphnumber: 2096, Medium gum beater, down
            { "pictGumMediumLeft", 0xE7C2 },  // glyphnumber: 2097, Medium gum beater, left
            { "pictGumMediumRight", 0xE7C1 },  // glyphnumber: 2098, Medium gum beater, right
            { "pictGumMediumUp", 0xE7BF },  // glyphnumber: 2099, Medium gum beater, up
            { "pictGumSoftDown", 0xE7BC },  // glyphnumber: 2100, Soft gum beater, down
            { "pictGumSoftLeft", 0xE7BE },  // glyphnumber: 2101, Soft gum beater, left
            { "pictGumSoftRight", 0xE7BD },  // glyphnumber: 2102, Soft gum beater, right
            { "pictGumSoftUp", 0xE7BB },  // glyphnumber: 2103, Soft gum beater, up
            { "pictHalfOpen1", 0xE7F6 },  // glyphnumber: 2104, Half-open
            { "pictHalfOpen2", 0xE7F7 },  // glyphnumber: 2105, Half-open 2 (Weinberg)
            { "pictHandbell", 0xE715 },  // glyphnumber: 2106, Handbell
            { "pictHiHat", 0xE722 },  // glyphnumber: 2107, Hi-hat
            { "pictHiHatOnStand", 0xE723 },  // glyphnumber: 2108, Hi-hat cymbals on stand
            { "pictJawHarp", 0xE767 },  // glyphnumber: 2109, Jaw harp
            { "pictJingleBells", 0xE719 },  // glyphnumber: 2110, Jingle bells
            { "pictKlaxonHorn", 0xE756 },  // glyphnumber: 2111, Klaxon horn
            { "pictLeftHandCircle", 0xE807 },  // glyphnumber: 2112, Right hand (Agostini)
            { "pictLionsRoar", 0xE763 },  // glyphnumber: 2113, Lion's roar
            { "pictLithophone", 0xE6B1 },  // glyphnumber: 2114, Lithophone
            { "pictLogDrum", 0xE6DF },  // glyphnumber: 2115, Log drum
            { "pictLotusFlute", 0xE75A },  // glyphnumber: 2116, Lotus flute
            { "pictMar", 0xE6A6 },  // glyphnumber: 2117, Marimba
            { "pictMarSmithBrindle", 0xE6AC },  // glyphnumber: 2118, Marimba (Smith Brindle)
            { "pictMaraca", 0xE741 },  // glyphnumber: 2119, Maraca
            { "pictMaracas", 0xE742 },  // glyphnumber: 2120, Maracas
            { "pictMegaphone", 0xE759 },  // glyphnumber: 2121, Megaphone
            { "pictMetalPlateChimes", 0xE6C8 },  // glyphnumber: 2122, Metal plate chimes
            { "pictMetalTubeChimes", 0xE6C7 },  // glyphnumber: 2123, Metal tube chimes
            { "pictMusicalSaw", 0xE766 },  // glyphnumber: 2124, Musical saw
            { "pictNormalPosition", 0xE804 },  // glyphnumber: 2125, Normal position (Caltabiano)
            { "pictOnRim", 0xE7F4 },  // glyphnumber: 2126, On rim
            { "pictOpen", 0xE7F8 },  // glyphnumber: 2127, Open
            { "pictOpenRimShot", 0xE7F5 },  // glyphnumber: 2128, Closed / rim shot
            { "pictPistolShot", 0xE760 },  // glyphnumber: 2129, Pistol shot
            { "pictPoliceWhistle", 0xE752 },  // glyphnumber: 2130, Police whistle
            { "pictQuijada", 0xE6FA },  // glyphnumber: 2131, Quijada (jawbone)
            { "pictRainstick", 0xE747 },  // glyphnumber: 2132, Rainstick
            { "pictRatchet", 0xE6F4 },  // glyphnumber: 2133, Ratchet
            { "pictRecoReco", 0xE6FC },  // glyphnumber: 2134, Reco-reco
            { "pictRightHandSquare", 0xE806 },  // glyphnumber: 2135, Left hand (Agostini)
            { "pictRim1", 0xE801 },  // glyphnumber: 2136, Rim or edge (Weinberg)
            { "pictRim2", 0xE802 },  // glyphnumber: 2137, Rim (Ghent)
            { "pictRim3", 0xE803 },  // glyphnumber: 2138, Rim (Caltabiano)
            { "pictRimShotOnStem", 0xE7FD },  // glyphnumber: 2139, Rim shot for stem
            { "pictSandpaperBlocks", 0xE762 },  // glyphnumber: 2140, Sandpaper blocks
            { "pictScrapeAroundRim", 0xE7F3 },  // glyphnumber: 2141, Scrape around rim (counter-clockwise)
            { "pictScrapeAroundRimClockwise", 0xE80E },  // glyphnumber: 2142, Scrape around rim (clockwise)
            { "pictScrapeCenterToEdge", 0xE7F1 },  // glyphnumber: 2143, Scrape from center to edge
            { "pictScrapeEdgeToCenter", 0xE7F2 },  // glyphnumber: 2144, Scrape from edge to center
            { "pictShellBells", 0xE718 },  // glyphnumber: 2145, Shell bells
            { "pictShellChimes", 0xE6C4 },  // glyphnumber: 2146, Shell chimes
            { "pictSiren", 0xE753 },  // glyphnumber: 2147, Siren
            { "pictSistrum", 0xE746 },  // glyphnumber: 2148, Sistrum
            { "pictSizzleCymbal", 0xE724 },  // glyphnumber: 2149, Sizzle cymbal
            { "pictSleighBell", 0xE710 },  // glyphnumber: 2150, Sleigh bell
            { "pictSlideBrushOnGong", 0xE734 },  // glyphnumber: 2151, Slide brush on gong
            { "pictSlideWhistle", 0xE750 },  // glyphnumber: 2152, Slide whistle
            { "pictSlitDrum", 0xE6E0 },  // glyphnumber: 2153, Slit drum
            { "pictSnareDrum", 0xE6D1 },  // glyphnumber: 2154, Snare drum
            { "pictSnareDrumMilitary", 0xE6D3 },  // glyphnumber: 2155, Military snare drum
            { "pictSnareDrumSnaresOff", 0xE6D2 },  // glyphnumber: 2156, Snare drum, snares off
            { "pictSteelDrums", 0xE6AF },  // glyphnumber: 2157, Steel drums
            { "pictStickShot", 0xE7F0 },  // glyphnumber: 2158, Stick shot
            { "pictSuperball", 0xE7B2 },  // glyphnumber: 2159, Superball
            { "pictSuspendedCymbal", 0xE721 },  // glyphnumber: 2160, Suspended cymbal
            { "pictSwishStem", 0xE808 },  // glyphnumber: 2161, Combining swish for stem
            { "pictTabla", 0xE6E3 },  // glyphnumber: 2162, Indian tabla
            { "pictTamTam", 0xE730 },  // glyphnumber: 2163, Tam-tam
            { "pictTamTamWithBeater", 0xE731 },  // glyphnumber: 2164, Tam-tam with beater (Smith Brindle)
            { "pictTambourine", 0xE6DB },  // glyphnumber: 2165, Tambourine
            { "pictTempleBlocks", 0xE6F1 },  // glyphnumber: 2166, Temple blocks
            { "pictTenorDrum", 0xE6D6 },  // glyphnumber: 2167, Tenor drum
            { "pictThundersheet", 0xE744 },  // glyphnumber: 2168, Thundersheet
            { "pictTimbales", 0xE6DC },  // glyphnumber: 2169, Timbales
            { "pictTimpani", 0xE6D0 },  // glyphnumber: 2170, Timpani
            { "pictTomTom", 0xE6D7 },  // glyphnumber: 2171, Tom-tom
            { "pictTomTomChinese", 0xE6D8 },  // glyphnumber: 2172, Chinese tom-tom
            { "pictTomTomIndoAmerican", 0xE6DA },  // glyphnumber: 2173, Indo-American tom tom
            { "pictTomTomJapanese", 0xE6D9 },  // glyphnumber: 2174, Japanese tom-tom
            { "pictTriangle", 0xE700 },  // glyphnumber: 2175, Triangle
            { "pictTubaphone", 0xE6B2 },  // glyphnumber: 2176, Tubaphone
            { "pictTubularBells", 0xE6C0 },  // glyphnumber: 2177, Tubular bells
            { "pictTurnLeftStem", 0xE80A },  // glyphnumber: 2178, Combining turn left for stem
            { "pictTurnRightLeftStem", 0xE80B },  // glyphnumber: 2179, Combining turn left or right for stem
            { "pictTurnRightStem", 0xE809 },  // glyphnumber: 2180, Combining turn right for stem
            { "pictVib", 0xE6A7 },  // glyphnumber: 2181, Vibraphone
            { "pictVibMotorOff", 0xE6A8 },  // glyphnumber: 2182, Metallophone (vibraphone motor off)
            { "pictVibSmithBrindle", 0xE6AD },  // glyphnumber: 2183, Vibraphone (Smith Brindle)
            { "pictVibraslap", 0xE745 },  // glyphnumber: 2184, Vibraslap
            { "pictVietnameseHat", 0xE725 },  // glyphnumber: 2185, Vietnamese hat cymbal
            { "pictWhip", 0xE6F6 },  // glyphnumber: 2186, Whip
            { "pictWindChimesGlass", 0xE6C1 },  // glyphnumber: 2187, Wind chimes (glass)
            { "pictWindMachine", 0xE754 },  // glyphnumber: 2188, Wind machine
            { "pictWindWhistle", 0xE758 },  // glyphnumber: 2189, Wind whistle (or mouth siren)
            { "pictWoodBlock", 0xE6F0 },  // glyphnumber: 2190, Wood block
            { "pictWoundHardDown", 0xE7B4 },  // glyphnumber: 2191, Wound beater, hard core down
            { "pictWoundHardLeft", 0xE7B6 },  // glyphnumber: 2192, Wound beater, hard core left
            { "pictWoundHardRight", 0xE7B5 },  // glyphnumber: 2193, Wound beater, hard core right
            { "pictWoundHardUp", 0xE7B3 },  // glyphnumber: 2194, Wound beater, hard core up
            { "pictWoundSoftDown", 0xE7B8 },  // glyphnumber: 2195, Wound beater, soft core down
            { "pictWoundSoftLeft", 0xE7BA },  // glyphnumber: 2196, Wound beater, soft core left
            { "pictWoundSoftRight", 0xE7B9 },  // glyphnumber: 2197, Wound beater, soft core right
            { "pictWoundSoftUp", 0xE7B7 },  // glyphnumber: 2198, Wound beater, soft core up
            { "pictXyl", 0xE6A1 },  // glyphnumber: 2199, Xylophone
            { "pictXylBass", 0xE6A3 },  // glyphnumber: 2200, Bass xylophone
            { "pictXylSmithBrindle", 0xE6AB },  // glyphnumber: 2201, Xylophone (Smith Brindle)
            { "pictXylTenor", 0xE6A2 },  // glyphnumber: 2202, Tenor xylophone
            { "pictXylTenorTrough", 0xE6A5 },  // glyphnumber: 2203, Trough tenor xylophone
            { "pictXylTrough", 0xE6A4 },  // glyphnumber: 2204, Trough xylophone
            { "pluckedBuzzPizzicato", 0xE632 },  // glyphnumber: 2205, Buzz pizzicato
            { "pluckedDamp", 0xE638 },  // glyphnumber: 2206, Damp
            { "pluckedDampAll", 0xE639 },  // glyphnumber: 2207, Damp all
            { "pluckedDampOnStem", 0xE63B },  // glyphnumber: 2208, Damp for stem
            { "pluckedFingernailFlick", 0xE637 },  // glyphnumber: 2209, Fingernail flick
            { "pluckedLeftHandPizzicato", 0xE633 },  // glyphnumber: 2210, Left-hand pizzicato
            { "pluckedPlectrum", 0xE63A },  // glyphnumber: 2211, Plectrum
            { "pluckedSnapPizzicatoAbove", 0xE631 },  // glyphnumber: 2212, Snap pizzicato above
            { "pluckedSnapPizzicatoBelow", 0xE630 },  // glyphnumber: 2213, Snap pizzicato below
            { "pluckedWithFingernails", 0xE636 },  // glyphnumber: 2214, With fingernails
            { "quindicesima", 0xE514 },  // glyphnumber: 2215, Quindicesima
            { "quindicesimaAlta", 0xE515 },  // glyphnumber: 2216, Quindicesima alta
            { "quindicesimaBassa", 0xE516 },  // glyphnumber: 2217, Quindicesima bassa
            { "quindicesimaBassaMb", 0xE51D },  // glyphnumber: 2218, Quindicesima bassa (mb)
            { "repeat1Bar", 0xE500 },  // glyphnumber: 2219, Repeat last bar
            { "repeat2Bars", 0xE501 },  // glyphnumber: 2220, Repeat last two bars
            { "repeat4Bars", 0xE502 },  // glyphnumber: 2221, Repeat last four bars
            { "repeatDot", 0xE044 },  // glyphnumber: 2222, Single repeat dot
            { "repeatDots", 0xE043 },  // glyphnumber: 2223, Repeat dots
            { "repeatLeft", 0xE040 },  // glyphnumber: 2224, Left (start) repeat sign
            { "repeatRight", 0xE041 },  // glyphnumber: 2225, Right (end) repeat sign
            { "repeatRightLeft", 0xE042 },  // glyphnumber: 2226, Right and left repeat sign
            { "rest1024th", 0xE4ED },  // glyphnumber: 2227, 1024th rest
            { "rest128th", 0xE4EA },  // glyphnumber: 2228, 128th (semihemidemisemiquaver) rest
            { "rest16th", 0xE4E7 },  // glyphnumber: 2229, 16th (semiquaver) rest
            { "rest256th", 0xE4EB },  // glyphnumber: 2230, 256th rest
            { "rest32nd", 0xE4E8 },  // glyphnumber: 2231, 32nd (demisemiquaver) rest
            { "rest512th", 0xE4EC },  // glyphnumber: 2232, 512th rest
            { "rest64th", 0xE4E9 },  // glyphnumber: 2233, 64th (hemidemisemiquaver) rest
            { "rest8th", 0xE4E6 },  // glyphnumber: 2234, Eighth (quaver) rest
            { "restDoubleWhole", 0xE4E2 },  // glyphnumber: 2235, Double whole (breve) rest
            { "restDoubleWholeLegerLine", 0xE4F3 },  // glyphnumber: 2236, Double whole rest on leger lines
            { "restHBar", 0xE4EE },  // glyphnumber: 2237, Multiple measure rest
            { "restHBarLeft", 0xE4EF },  // glyphnumber: 2238, H-bar, left half
            { "restHBarMiddle", 0xE4F0 },  // glyphnumber: 2239, H-bar, middle
            { "restHBarRight", 0xE4F1 },  // glyphnumber: 2240, H-bar, right half
            { "restHalf", 0xE4E4 },  // glyphnumber: 2241, Half (minim) rest
            { "restHalfLegerLine", 0xE4F5 },  // glyphnumber: 2242, Half rest on leger line
            { "restLonga", 0xE4E1 },  // glyphnumber: 2243, Longa rest
            { "restMaxima", 0xE4E0 },  // glyphnumber: 2244, Maxima rest
            { "restQuarter", 0xE4E5 },  // glyphnumber: 2245, Quarter (crotchet) rest
            { "restQuarterOld", 0xE4F2 },  // glyphnumber: 2246, Old-style quarter (crotchet) rest
            { "restQuarterZ", 0xE4F6 },  // glyphnumber: 2247, Z-style quarter (crotchet) rest
            { "restWhole", 0xE4E3 },  // glyphnumber: 2248, Whole (semibreve) rest
            { "restWholeLegerLine", 0xE4F4 },  // glyphnumber: 2249, Whole rest on leger line
            { "reversedBrace", 0xE001 },  // glyphnumber: 2250, Reversed brace
            { "reversedBracketBottom", 0xE006 },  // glyphnumber: 2251, Reversed bracket bottom
            { "reversedBracketTop", 0xE005 },  // glyphnumber: 2252, Reversed bracket top
            { "rightRepeatSmall", 0xE04D },  // glyphnumber: 2253, Right repeat sign within bar
            { "schaefferClef", 0xE06F },  // glyphnumber: 2254, Schäffer clef
            { "schaefferFClefToGClef", 0xE072 },  // glyphnumber: 2255, Schäffer F clef to G clef change
            { "schaefferGClefToFClef", 0xE071 },  // glyphnumber: 2256, Schäffer G clef to F clef change
            { "schaefferPreviousClef", 0xE070 },  // glyphnumber: 2257, Schäffer previous clef
            { "segno", 0xE047 },  // glyphnumber: 2258, Segno
            { "segnoSerpent1", 0xE04A },  // glyphnumber: 2259, Segno (serpent)
            { "segnoSerpent2", 0xE04B },  // glyphnumber: 2260, Segno (serpent with vertical lines)
            { "semipitchedPercussionClef1", 0xE06B },  // glyphnumber: 2261, Semi-pitched percussion clef 1
            { "semipitchedPercussionClef2", 0xE06C },  // glyphnumber: 2262, Semi-pitched percussion clef 2
            { "smnFlat", 0xEC52 },  // glyphnumber: 2263, Flat
            { "smnFlatWhite", 0xEC53 },  // glyphnumber: 2264, Flat (white)
            { "smnHistoryDoubleFlat", 0xEC57 },  // glyphnumber: 2265, Double flat history sign
            { "smnHistoryDoubleSharp", 0xEC55 },  // glyphnumber: 2266, Double sharp history sign
            { "smnHistoryFlat", 0xEC56 },  // glyphnumber: 2267, Flat history sign
            { "smnHistorySharp", 0xEC54 },  // glyphnumber: 2268, Sharp history sign
            { "smnNatural", 0xEC58 },  // glyphnumber: 2269, Natural (N)
            { "smnSharp", 0xEC50 },  // glyphnumber: 2270, Sharp stem up
            { "smnSharpDown", 0xEC59 },  // glyphnumber: 2271, Sharp stem down
            { "smnSharpWhite", 0xEC51 },  // glyphnumber: 2272, Sharp (white) stem up
            { "smnSharpWhiteDown", 0xEC5A },  // glyphnumber: 2273, Sharp (white) stem down
            { "splitBarDivider", 0xE00A },  // glyphnumber: 2274, Split bar divider (bar spans a system break)
            { "staff1Line", 0xE010 },  // glyphnumber: 2275, 1-line staff
            { "staff1LineNarrow", 0xE01C },  // glyphnumber: 2276, 1-line staff (narrow)
            { "staff1LineWide", 0xE016 },  // glyphnumber: 2277, 1-line staff (wide)
            { "staff2Lines", 0xE011 },  // glyphnumber: 2278, 2-line staff
            { "staff2LinesNarrow", 0xE01D },  // glyphnumber: 2279, 2-line staff (narrow)
            { "staff2LinesWide", 0xE017 },  // glyphnumber: 2280, 2-line staff (wide)
            { "staff3Lines", 0xE012 },  // glyphnumber: 2281, 3-line staff
            { "staff3LinesNarrow", 0xE01E },  // glyphnumber: 2282, 3-line staff (narrow)
            { "staff3LinesWide", 0xE018 },  // glyphnumber: 2283, 3-line staff (wide)
            { "staff4Lines", 0xE013 },  // glyphnumber: 2284, 4-line staff
            { "staff4LinesNarrow", 0xE01F },  // glyphnumber: 2285, 4-line staff (narrow)
            { "staff4LinesWide", 0xE019 },  // glyphnumber: 2286, 4-line staff (wide)
            { "staff5Lines", 0xE014 },  // glyphnumber: 2287, 5-line staff
            { "staff5LinesNarrow", 0xE020 },  // glyphnumber: 2288, 5-line staff (narrow)
            { "staff5LinesWide", 0xE01A },  // glyphnumber: 2289, 5-line staff (wide)
            { "staff6Lines", 0xE015 },  // glyphnumber: 2290, 6-line staff
            { "staff6LinesNarrow", 0xE021 },  // glyphnumber: 2291, 6-line staff (narrow)
            { "staff6LinesWide", 0xE01B },  // glyphnumber: 2292, 6-line staff (wide)
            { "staffDivideArrowDown", 0xE00B },  // glyphnumber: 2293, Staff divide arrow down
            { "staffDivideArrowUp", 0xE00C },  // glyphnumber: 2294, Staff divide arrow up
            { "staffDivideArrowUpDown", 0xE00D },  // glyphnumber: 2295, Staff divide arrows
            { "staffPosLower1", 0xEB98 },  // glyphnumber: 2296, Lower 1 staff position
            { "staffPosLower2", 0xEB99 },  // glyphnumber: 2297, Lower 2 staff positions
            { "staffPosLower3", 0xEB9A },  // glyphnumber: 2298, Lower 3 staff positions
            { "staffPosLower4", 0xEB9B },  // glyphnumber: 2299, Lower 4 staff positions
            { "staffPosLower5", 0xEB9C },  // glyphnumber: 2300, Lower 5 staff positions
            { "staffPosLower6", 0xEB9D },  // glyphnumber: 2301, Lower 6 staff positions
            { "staffPosLower7", 0xEB9E },  // glyphnumber: 2302, Lower 7 staff positions
            { "staffPosLower8", 0xEB9F },  // glyphnumber: 2303, Lower 8 staff positions
            { "staffPosRaise1", 0xEB90 },  // glyphnumber: 2304, Raise 1 staff position
            { "staffPosRaise2", 0xEB91 },  // glyphnumber: 2305, Raise 2 staff positions
            { "staffPosRaise3", 0xEB92 },  // glyphnumber: 2306, Raise 3 staff positions
            { "staffPosRaise4", 0xEB93 },  // glyphnumber: 2307, Raise 4 staff positions
            { "staffPosRaise5", 0xEB94 },  // glyphnumber: 2308, Raise 5 staff positions
            { "staffPosRaise6", 0xEB95 },  // glyphnumber: 2309, Raise 6 staff positions
            { "staffPosRaise7", 0xEB96 },  // glyphnumber: 2310, Raise 7 staff positions
            { "staffPosRaise8", 0xEB97 },  // glyphnumber: 2311, Raise 8 staff positions
            { "stem", 0xE210 },  // glyphnumber: 2312, Combining stem
            { "stemBowOnBridge", 0xE215 },  // glyphnumber: 2313, Combining bow on bridge stem
            { "stemBowOnTailpiece", 0xE216 },  // glyphnumber: 2314, Combining bow on tailpiece stem
            { "stemBuzzRoll", 0xE217 },  // glyphnumber: 2315, Combining buzz roll stem
            { "stemDamp", 0xE218 },  // glyphnumber: 2316, Combining damp stem
            { "stemHarpStringNoise", 0xE21F },  // glyphnumber: 2317, Combining harp string noise stem
            { "stemMultiphonicsBlack", 0xE21A },  // glyphnumber: 2318, Combining multiphonics (black) stem
            { "stemMultiphonicsBlackWhite", 0xE21C },  // glyphnumber: 2319, Combining multiphonics (black and white) stem
            { "stemMultiphonicsWhite", 0xE21B },  // glyphnumber: 2320, Combining multiphonics (white) stem
            { "stemPendereckiTremolo", 0xE213 },  // glyphnumber: 2321, Combining Penderecki unmeasured tremolo stem
            { "stemRimShot", 0xE21E },  // glyphnumber: 2322, Combining rim shot stem
            { "stemSprechgesang", 0xE211 },  // glyphnumber: 2323, Combining sprechgesang stem
            { "stemSulPonticello", 0xE214 },  // glyphnumber: 2324, Combining sul ponticello (bow behind bridge) stem
            { "stemSussurando", 0xE21D },  // glyphnumber: 2325, Combining sussurando stem
            { "stemSwished", 0xE212 },  // glyphnumber: 2326, Combining swished stem
            { "stemVibratoPulse", 0xE219 },  // glyphnumber: 2327, Combining vibrato pulse accent (Saunders) stem
            { "stockhausenTremolo", 0xE232 },  // glyphnumber: 2328, Stockhausen irregular tremolo ("Morsen", like Morse code)
            { "stringsBowBehindBridge", 0xE618 },  // glyphnumber: 2329, Bow behind bridge (sul ponticello)
            { "stringsBowBehindBridgeFourStrings", 0xE62A },  // glyphnumber: 2330, Bow behind bridge on four strings
            { "stringsBowBehindBridgeOneString", 0xE627 },  // glyphnumber: 2331, Bow behind bridge on one string
            { "stringsBowBehindBridgeThreeStrings", 0xE629 },  // glyphnumber: 2332, Bow behind bridge on three strings
            { "stringsBowBehindBridgeTwoStrings", 0xE628 },  // glyphnumber: 2333, Bow behind bridge on two strings
            { "stringsBowOnBridge", 0xE619 },  // glyphnumber: 2334, Bow on top of bridge
            { "stringsBowOnTailpiece", 0xE61A },  // glyphnumber: 2335, Bow on tailpiece
            { "stringsChangeBowDirection", 0xE626 },  // glyphnumber: 2336, Change bow direction, indeterminate
            { "stringsDownBow", 0xE610 },  // glyphnumber: 2337, Down bow
            { "stringsDownBowTurned", 0xE611 },  // glyphnumber: 2338, Turned down bow
            { "stringsFouette", 0xE622 },  // glyphnumber: 2339, Fouetté
            { "stringsHalfHarmonic", 0xE615 },  // glyphnumber: 2340, Half-harmonic
            { "stringsHarmonic", 0xE614 },  // glyphnumber: 2341, Harmonic
            { "stringsJeteAbove", 0xE620 },  // glyphnumber: 2342, Jeté (gettato) above
            { "stringsJeteBelow", 0xE621 },  // glyphnumber: 2343, Jeté (gettato) below
            { "stringsMuteOff", 0xE617 },  // glyphnumber: 2344, Mute off
            { "stringsMuteOn", 0xE616 },  // glyphnumber: 2345, Mute on
            { "stringsOverpressureDownBow", 0xE61B },  // glyphnumber: 2346, Overpressure, down bow
            { "stringsOverpressureNoDirection", 0xE61F },  // glyphnumber: 2347, Overpressure, no bow direction
            { "stringsOverpressurePossibileDownBow", 0xE61D },  // glyphnumber: 2348, Overpressure possibile, down bow
            { "stringsOverpressurePossibileUpBow", 0xE61E },  // glyphnumber: 2349, Overpressure possibile, up bow
            { "stringsOverpressureUpBow", 0xE61C },  // glyphnumber: 2350, Overpressure, up bow
            { "stringsThumbPosition", 0xE624 },  // glyphnumber: 2351, Thumb position
            { "stringsThumbPositionTurned", 0xE625 },  // glyphnumber: 2352, Turned thumb position
            { "stringsUpBow", 0xE612 },  // glyphnumber: 2353, Up bow
            { "stringsUpBowTurned", 0xE613 },  // glyphnumber: 2354, Turned up bow
            { "stringsVibratoPulse", 0xE623 },  // glyphnumber: 2355, Vibrato pulse accent (Saunders) for stem
            { "systemDivider", 0xE007 },  // glyphnumber: 2356, System divider
            { "systemDividerExtraLong", 0xE009 },  // glyphnumber: 2357, Extra long system divider
            { "systemDividerLong", 0xE008 },  // glyphnumber: 2358, Long system divider
            { "textAugmentationDot", 0xE1FC },  // glyphnumber: 2359, Augmentation dot
            { "textBlackNoteFrac16thLongStem", 0xE1F5 },  // glyphnumber: 2360, Black note, fractional 16th beam, long stem
            { "textBlackNoteFrac16thShortStem", 0xE1F4 },  // glyphnumber: 2361, Black note, fractional 16th beam, short stem
            { "textBlackNoteFrac32ndLongStem", 0xE1F6 },  // glyphnumber: 2362, Black note, fractional 32nd beam, long stem
            { "textBlackNoteFrac8thLongStem", 0xE1F3 },  // glyphnumber: 2363, Black note, fractional 8th beam, long stem
            { "textBlackNoteFrac8thShortStem", 0xE1F2 },  // glyphnumber: 2364, Black note, fractional 8th beam, short stem
            { "textBlackNoteLongStem", 0xE1F1 },  // glyphnumber: 2365, Black note, long stem
            { "textBlackNoteShortStem", 0xE1F0 },  // glyphnumber: 2366, Black note, short stem
            { "textCont16thBeamLongStem", 0xE1FA },  // glyphnumber: 2367, Continuing 16th beam for long stem
            { "textCont16thBeamShortStem", 0xE1F9 },  // glyphnumber: 2368, Continuing 16th beam for short stem
            { "textCont32ndBeamLongStem", 0xE1FB },  // glyphnumber: 2369, Continuing 32nd beam for long stem
            { "textCont8thBeamLongStem", 0xE1F8 },  // glyphnumber: 2370, Continuing 8th beam for long stem
            { "textCont8thBeamShortStem", 0xE1F7 },  // glyphnumber: 2371, Continuing 8th beam for short stem
            { "textTie", 0xE1FD },  // glyphnumber: 2372, Tie
            { "textTuplet3LongStem", 0xE202 },  // glyphnumber: 2373, Tuplet number 3 for long stem
            { "textTuplet3ShortStem", 0xE1FF },  // glyphnumber: 2374, Tuplet number 3 for short stem
            { "textTupletBracketEndLongStem", 0xE203 },  // glyphnumber: 2375, Tuplet bracket end for long stem
            { "textTupletBracketEndShortStem", 0xE200 },  // glyphnumber: 2376, Tuplet bracket end for short stem
            { "textTupletBracketStartLongStem", 0xE201 },  // glyphnumber: 2377, Tuplet bracket start for long stem
            { "textTupletBracketStartShortStem", 0xE1FE },  // glyphnumber: 2378, Tuplet bracket start for short stem
            { "timeSig0", 0xE080 },  // glyphnumber: 2379, Time signature 0
            { "timeSig0Reversed", 0xECF0 },  // glyphnumber: 2380, Reversed time signature 0
            { "timeSig0Turned", 0xECE0 },  // glyphnumber: 2381, Turned time signature 0
            { "timeSig1", 0xE081 },  // glyphnumber: 2382, Time signature 1
            { "timeSig1Reversed", 0xECF1 },  // glyphnumber: 2383, Reversed time signature 1
            { "timeSig1Turned", 0xECE1 },  // glyphnumber: 2384, Turned time signature 1
            { "timeSig2", 0xE082 },  // glyphnumber: 2385, Time signature 2
            { "timeSig2Reversed", 0xECF2 },  // glyphnumber: 2386, Reversed time signature 2
            { "timeSig2Turned", 0xECE2 },  // glyphnumber: 2387, Turned time signature 2
            { "timeSig3", 0xE083 },  // glyphnumber: 2388, Time signature 3
            { "timeSig3Reversed", 0xECF3 },  // glyphnumber: 2389, Reversed time signature 3
            { "timeSig3Turned", 0xECE3 },  // glyphnumber: 2390, Turned time signature 3
            { "timeSig4", 0xE084 },  // glyphnumber: 2391, Time signature 4
            { "timeSig4Reversed", 0xECF4 },  // glyphnumber: 2392, Reversed time signature 4
            { "timeSig4Turned", 0xECE4 },  // glyphnumber: 2393, Turned time signature 4
            { "timeSig5", 0xE085 },  // glyphnumber: 2394, Time signature 5
            { "timeSig5Reversed", 0xECF5 },  // glyphnumber: 2395, Reversed time signature 5
            { "timeSig5Turned", 0xECE5 },  // glyphnumber: 2396, Turned time signature 5
            { "timeSig6", 0xE086 },  // glyphnumber: 2397, Time signature 6
            { "timeSig6Reversed", 0xECF6 },  // glyphnumber: 2398, Reversed time signature 6
            { "timeSig6Turned", 0xECE6 },  // glyphnumber: 2399, Turned time signature 6
            { "timeSig7", 0xE087 },  // glyphnumber: 2400, Time signature 7
            { "timeSig7Reversed", 0xECF7 },  // glyphnumber: 2401, Reversed time signature 7
            { "timeSig7Turned", 0xECE7 },  // glyphnumber: 2402, Turned time signature 7
            { "timeSig8", 0xE088 },  // glyphnumber: 2403, Time signature 8
            { "timeSig8Reversed", 0xECF8 },  // glyphnumber: 2404, Reversed time signature 8
            { "timeSig8Turned", 0xECE8 },  // glyphnumber: 2405, Turned time signature 8
            { "timeSig9", 0xE089 },  // glyphnumber: 2406, Time signature 9
            { "timeSig9Reversed", 0xECF9 },  // glyphnumber: 2407, Reversed time signature 9
            { "timeSig9Turned", 0xECE9 },  // glyphnumber: 2408, Turned time signature 9
            { "timeSigBracketLeft", 0xEC80 },  // glyphnumber: 2409, Left bracket for whole time signature
            { "timeSigBracketLeftSmall", 0xEC82 },  // glyphnumber: 2410, Left bracket for numerator only
            { "timeSigBracketRight", 0xEC81 },  // glyphnumber: 2411, Right bracket for whole time signature
            { "timeSigBracketRightSmall", 0xEC83 },  // glyphnumber: 2412, Right bracket for numerator only
            { "timeSigCombDenominator", 0xE09F },  // glyphnumber: 2413, Control character for denominator digit
            { "timeSigCombNumerator", 0xE09E },  // glyphnumber: 2414, Control character for numerator digit
            { "timeSigComma", 0xE096 },  // glyphnumber: 2415, Time signature comma
            { "timeSigCommon", 0xE08A },  // glyphnumber: 2416, Common time
            { "timeSigCommonReversed", 0xECFA },  // glyphnumber: 2417, Reversed common time
            { "timeSigCommonTurned", 0xECEA },  // glyphnumber: 2418, Turned common time
            { "timeSigCut2", 0xEC85 },  // glyphnumber: 2419, Cut time (Bach)
            { "timeSigCut3", 0xEC86 },  // glyphnumber: 2420, Cut triple time (9/8)
            { "timeSigCutCommon", 0xE08B },  // glyphnumber: 2421, Cut time
            { "timeSigCutCommonReversed", 0xECFB },  // glyphnumber: 2422, Reversed cut time
            { "timeSigCutCommonTurned", 0xECEB },  // glyphnumber: 2423, Turned cut time
            { "timeSigEquals", 0xE08F },  // glyphnumber: 2424, Time signature equals
            { "timeSigFractionHalf", 0xE098 },  // glyphnumber: 2425, Time signature fraction ½
            { "timeSigFractionOneThird", 0xE09A },  // glyphnumber: 2426, Time signature fraction ⅓
            { "timeSigFractionQuarter", 0xE097 },  // glyphnumber: 2427, Time signature fraction ¼
            { "timeSigFractionThreeQuarters", 0xE099 },  // glyphnumber: 2428, Time signature fraction ¾
            { "timeSigFractionTwoThirds", 0xE09B },  // glyphnumber: 2429, Time signature fraction ⅔
            { "timeSigFractionalSlash", 0xE08E },  // glyphnumber: 2430, Time signature fraction slash
            { "timeSigMinus", 0xE090 },  // glyphnumber: 2431, Time signature minus
            { "timeSigMultiply", 0xE091 },  // glyphnumber: 2432, Time signature multiply
            { "timeSigOpenPenderecki", 0xE09D },  // glyphnumber: 2433, Open time signature (Penderecki)
            { "timeSigParensLeft", 0xE094 },  // glyphnumber: 2434, Left parenthesis for whole time signature
            { "timeSigParensLeftSmall", 0xE092 },  // glyphnumber: 2435, Left parenthesis for numerator only
            { "timeSigParensRight", 0xE095 },  // glyphnumber: 2436, Right parenthesis for whole time signature
            { "timeSigParensRightSmall", 0xE093 },  // glyphnumber: 2437, Right parenthesis for numerator only
            { "timeSigPlus", 0xE08C },  // glyphnumber: 2438, Time signature +
            { "timeSigPlusSmall", 0xE08D },  // glyphnumber: 2439, Time signature + (for numerators)
            { "timeSigSlash", 0xEC84 },  // glyphnumber: 2440, Time signature slash separator
            { "timeSigX", 0xE09C },  // glyphnumber: 2441, Open time signature
            { "tremolo1", 0xE220 },  // glyphnumber: 2442, Combining tremolo 1
            { "tremolo2", 0xE221 },  // glyphnumber: 2443, Combining tremolo 2
            { "tremolo3", 0xE222 },  // glyphnumber: 2444, Combining tremolo 3
            { "tremolo4", 0xE223 },  // glyphnumber: 2445, Combining tremolo 4
            { "tremolo5", 0xE224 },  // glyphnumber: 2446, Combining tremolo 5
            { "tremoloDivisiDots2", 0xE22E },  // glyphnumber: 2447, Divide measured tremolo by 2
            { "tremoloDivisiDots3", 0xE22F },  // glyphnumber: 2448, Divide measured tremolo by 3
            { "tremoloDivisiDots4", 0xE230 },  // glyphnumber: 2449, Divide measured tremolo by 4
            { "tremoloDivisiDots6", 0xE231 },  // glyphnumber: 2450, Divide measured tremolo by 6
            { "tremoloFingered1", 0xE225 },  // glyphnumber: 2451, Fingered tremolo 1
            { "tremoloFingered2", 0xE226 },  // glyphnumber: 2452, Fingered tremolo 2
            { "tremoloFingered3", 0xE227 },  // glyphnumber: 2453, Fingered tremolo 3
            { "tremoloFingered4", 0xE228 },  // glyphnumber: 2454, Fingered tremolo 4
            { "tremoloFingered5", 0xE229 },  // glyphnumber: 2455, Fingered tremolo 5
            { "tripleTongueAbove", 0xE5F2 },  // glyphnumber: 2456, Triple-tongue above
            { "tripleTongueBelow", 0xE5F3 },  // glyphnumber: 2457, Triple-tongue below
            { "tuplet0", 0xE880 },  // glyphnumber: 2458, Tuplet 0
            { "tuplet1", 0xE881 },  // glyphnumber: 2459, Tuplet 1
            { "tuplet2", 0xE882 },  // glyphnumber: 2460, Tuplet 2
            { "tuplet3", 0xE883 },  // glyphnumber: 2461, Tuplet 3
            { "tuplet4", 0xE884 },  // glyphnumber: 2462, Tuplet 4
            { "tuplet5", 0xE885 },  // glyphnumber: 2463, Tuplet 5
            { "tuplet6", 0xE886 },  // glyphnumber: 2464, Tuplet 6
            { "tuplet7", 0xE887 },  // glyphnumber: 2465, Tuplet 7
            { "tuplet8", 0xE888 },  // glyphnumber: 2466, Tuplet 8
            { "tuplet9", 0xE889 },  // glyphnumber: 2467, Tuplet 9
            { "tupletColon", 0xE88A },  // glyphnumber: 2468, Tuplet colon
            { "unmeasuredTremolo", 0xE22C },  // glyphnumber: 2469, Wieniawski unmeasured tremolo
            { "unmeasuredTremoloSimple", 0xE22D },  // glyphnumber: 2470, Wieniawski unmeasured tremolo (simpler)
            { "unpitchedPercussionClef1", 0xE069 },  // glyphnumber: 2471, Unpitched percussion clef 1
            { "unpitchedPercussionClef2", 0xE06A },  // glyphnumber: 2472, Unpitched percussion clef 2
            { "ventiduesima", 0xE517 },  // glyphnumber: 2473, Ventiduesima
            { "ventiduesimaAlta", 0xE518 },  // glyphnumber: 2474, Ventiduesima alta
            { "ventiduesimaBassa", 0xE519 },  // glyphnumber: 2475, Ventiduesima bassa
            { "ventiduesimaBassaMb", 0xE51E },  // glyphnumber: 2476, Ventiduesima bassa (mb)
            { "vocalFingerClickStockhausen", 0xE649 },  // glyphnumber: 2477, Finger click (Stockhausen)
            { "vocalMouthClosed", 0xE640 },  // glyphnumber: 2478, Mouth closed
            { "vocalMouthOpen", 0xE642 },  // glyphnumber: 2479, Mouth open
            { "vocalMouthPursed", 0xE644 },  // glyphnumber: 2480, Mouth pursed
            { "vocalMouthSlightlyOpen", 0xE641 },  // glyphnumber: 2481, Mouth slightly open
            { "vocalMouthWideOpen", 0xE643 },  // glyphnumber: 2482, Mouth wide open
            { "vocalNasalVoice", 0xE647 },  // glyphnumber: 2483, Nasal voice
            { "vocalSprechgesang", 0xE645 },  // glyphnumber: 2484, Sprechgesang
            { "vocalTongueClickStockhausen", 0xE648 },  // glyphnumber: 2485, Tongue click (Stockhausen)
            { "vocalTongueFingerClickStockhausen", 0xE64A },  // glyphnumber: 2486, Tongue and finger click (Stockhausen)
            { "vocalsSussurando", 0xE646 },  // glyphnumber: 2487, Combining sussurando for stem
            { "wiggleArpeggiatoDown", 0xEAAA },  // glyphnumber: 2488, Arpeggiato wiggle segment, downwards
            { "wiggleArpeggiatoDownArrow", 0xEAAE },  // glyphnumber: 2489, Arpeggiato arrowhead down
            { "wiggleArpeggiatoDownSwash", 0xEAAC },  // glyphnumber: 2490, Arpeggiato downward swash
            { "wiggleArpeggiatoUp", 0xEAA9 },  // glyphnumber: 2491, Arpeggiato wiggle segment, upwards
            { "wiggleArpeggiatoUpArrow", 0xEAAD },  // glyphnumber: 2492, Arpeggiato arrowhead up
            { "wiggleArpeggiatoUpSwash", 0xEAAB },  // glyphnumber: 2493, Arpeggiato upward swash
            { "wiggleCircular", 0xEAC9 },  // glyphnumber: 2494, Circular motion segment
            { "wiggleCircularConstant", 0xEAC0 },  // glyphnumber: 2495, Constant circular motion segment
            { "wiggleCircularConstantFlipped", 0xEAC1 },  // glyphnumber: 2496, Constant circular motion segment (flipped)
            { "wiggleCircularConstantFlippedLarge", 0xEAC3 },  // glyphnumber: 2497, Constant circular motion segment (flipped, large)
            { "wiggleCircularConstantLarge", 0xEAC2 },  // glyphnumber: 2498, Constant circular motion segment (large)
            { "wiggleCircularEnd", 0xEACB },  // glyphnumber: 2499, Circular motion end
            { "wiggleCircularLarge", 0xEAC8 },  // glyphnumber: 2500, Circular motion segment, large
            { "wiggleCircularLarger", 0xEAC7 },  // glyphnumber: 2501, Circular motion segment, larger
            { "wiggleCircularLargerStill", 0xEAC6 },  // glyphnumber: 2502, Circular motion segment, larger still
            { "wiggleCircularLargest", 0xEAC5 },  // glyphnumber: 2503, Circular motion segment, largest
            { "wiggleCircularSmall", 0xEACA },  // glyphnumber: 2504, Circular motion segment, small
            { "wiggleCircularStart", 0xEAC4 },  // glyphnumber: 2505, Circular motion start
            { "wiggleGlissando", 0xEAAF },  // glyphnumber: 2506, Glissando wiggle segment
            { "wiggleGlissandoGroup1", 0xEABD },  // glyphnumber: 2507, Group glissando 1
            { "wiggleGlissandoGroup2", 0xEABE },  // glyphnumber: 2508, Group glissando 2
            { "wiggleGlissandoGroup3", 0xEABF },  // glyphnumber: 2509, Group glissando 3
            { "wiggleRandom1", 0xEAF0 },  // glyphnumber: 2510, Quasi-random squiggle 1
            { "wiggleRandom2", 0xEAF1 },  // glyphnumber: 2511, Quasi-random squiggle 2
            { "wiggleRandom3", 0xEAF2 },  // glyphnumber: 2512, Quasi-random squiggle 3
            { "wiggleRandom4", 0xEAF3 },  // glyphnumber: 2513, Quasi-random squiggle 4
            { "wiggleSawtooth", 0xEABB },  // glyphnumber: 2514, Sawtooth line segment
            { "wiggleSawtoothNarrow", 0xEABA },  // glyphnumber: 2515, Narrow sawtooth line segment
            { "wiggleSawtoothWide", 0xEABC },  // glyphnumber: 2516, Wide sawtooth line segment
            { "wiggleSquareWave", 0xEAB8 },  // glyphnumber: 2517, Square wave line segment
            { "wiggleSquareWaveNarrow", 0xEAB7 },  // glyphnumber: 2518, Narrow square wave line segment
            { "wiggleSquareWaveWide", 0xEAB9 },  // glyphnumber: 2519, Wide square wave line segment
            { "wiggleTrill", 0xEAA4 },  // glyphnumber: 2520, Trill wiggle segment
            { "wiggleTrillFast", 0xEAA3 },  // glyphnumber: 2521, Trill wiggle segment, fast
            { "wiggleTrillFaster", 0xEAA2 },  // glyphnumber: 2522, Trill wiggle segment, faster
            { "wiggleTrillFasterStill", 0xEAA1 },  // glyphnumber: 2523, Trill wiggle segment, faster still
            { "wiggleTrillFastest", 0xEAA0 },  // glyphnumber: 2524, Trill wiggle segment, fastest
            { "wiggleTrillSlow", 0xEAA5 },  // glyphnumber: 2525, Trill wiggle segment, slow
            { "wiggleTrillSlower", 0xEAA6 },  // glyphnumber: 2526, Trill wiggle segment, slower
            { "wiggleTrillSlowerStill", 0xEAA7 },  // glyphnumber: 2527, Trill wiggle segment, slower still
            { "wiggleTrillSlowest", 0xEAA8 },  // glyphnumber: 2528, Trill wiggle segment, slowest
            { "wiggleVIbratoLargestSlower", 0xEAEE },  // glyphnumber: 2529, Vibrato largest, slower
            { "wiggleVIbratoMediumSlower", 0xEAE0 },  // glyphnumber: 2530, Vibrato medium, slower
            { "wiggleVibrato", 0xEAB0 },  // glyphnumber: 2531, Vibrato / shake wiggle segment
            { "wiggleVibratoLargeFast", 0xEAE5 },  // glyphnumber: 2532, Vibrato large, fast
            { "wiggleVibratoLargeFaster", 0xEAE4 },  // glyphnumber: 2533, Vibrato large, faster
            { "wiggleVibratoLargeFasterStill", 0xEAE3 },  // glyphnumber: 2534, Vibrato large, faster still
            { "wiggleVibratoLargeFastest", 0xEAE2 },  // glyphnumber: 2535, Vibrato large, fastest
            { "wiggleVibratoLargeSlow", 0xEAE6 },  // glyphnumber: 2536, Vibrato large, slow
            { "wiggleVibratoLargeSlower", 0xEAE7 },  // glyphnumber: 2537, Vibrato large, slower
            { "wiggleVibratoLargeSlowest", 0xEAE8 },  // glyphnumber: 2538, Vibrato large, slowest
            { "wiggleVibratoLargestFast", 0xEAEC },  // glyphnumber: 2539, Vibrato largest, fast
            { "wiggleVibratoLargestFaster", 0xEAEB },  // glyphnumber: 2540, Vibrato largest, faster
            { "wiggleVibratoLargestFasterStill", 0xEAEA },  // glyphnumber: 2541, Vibrato largest, faster still
            { "wiggleVibratoLargestFastest", 0xEAE9 },  // glyphnumber: 2542, Vibrato largest, fastest
            { "wiggleVibratoLargestSlow", 0xEAED },  // glyphnumber: 2543, Vibrato largest, slow
            { "wiggleVibratoLargestSlowest", 0xEAEF },  // glyphnumber: 2544, Vibrato largest, slowest
            { "wiggleVibratoMediumFast", 0xEADE },  // glyphnumber: 2545, Vibrato medium, fast
            { "wiggleVibratoMediumFaster", 0xEADD },  // glyphnumber: 2546, Vibrato medium, faster
            { "wiggleVibratoMediumFasterStill", 0xEADC },  // glyphnumber: 2547, Vibrato medium, faster still
            { "wiggleVibratoMediumFastest", 0xEADB },  // glyphnumber: 2548, Vibrato medium, fastest
            { "wiggleVibratoMediumSlow", 0xEADF },  // glyphnumber: 2549, Vibrato medium, slow
            { "wiggleVibratoMediumSlowest", 0xEAE1 },  // glyphnumber: 2550, Vibrato medium, slowest
            { "wiggleVibratoSmallFast", 0xEAD7 },  // glyphnumber: 2551, Vibrato small, fast
            { "wiggleVibratoSmallFaster", 0xEAD6 },  // glyphnumber: 2552, Vibrato small, faster
            { "wiggleVibratoSmallFasterStill", 0xEAD5 },  // glyphnumber: 2553, Vibrato small, faster still
            { "wiggleVibratoSmallFastest", 0xEAD4 },  // glyphnumber: 2554, Vibrato small, fastest
            { "wiggleVibratoSmallSlow", 0xEAD8 },  // glyphnumber: 2555, Vibrato small, slow
            { "wiggleVibratoSmallSlower", 0xEAD9 },  // glyphnumber: 2556, Vibrato small, slower
            { "wiggleVibratoSmallSlowest", 0xEADA },  // glyphnumber: 2557, Vibrato small, slowest
            { "wiggleVibratoSmallestFast", 0xEAD0 },  // glyphnumber: 2558, Vibrato smallest, fast
            { "wiggleVibratoSmallestFaster", 0xEACF },  // glyphnumber: 2559, Vibrato smallest, faster
            { "wiggleVibratoSmallestFasterStill", 0xEACE },  // glyphnumber: 2560, Vibrato smallest, faster still
            { "wiggleVibratoSmallestFastest", 0xEACD },  // glyphnumber: 2561, Vibrato smallest, fastest
            { "wiggleVibratoSmallestSlow", 0xEAD1 },  // glyphnumber: 2562, Vibrato smallest, slow
            { "wiggleVibratoSmallestSlower", 0xEAD2 },  // glyphnumber: 2563, Vibrato smallest, slower
            { "wiggleVibratoSmallestSlowest", 0xEAD3 },  // glyphnumber: 2564, Vibrato smallest, slowest
            { "wiggleVibratoStart", 0xEACC },  // glyphnumber: 2565, Vibrato start
            { "wiggleVibratoWide", 0xEAB1 },  // glyphnumber: 2566, Wide vibrato / shake wiggle segment
            { "wiggleWavy", 0xEAB5 },  // glyphnumber: 2567, Wavy line segment
            { "wiggleWavyNarrow", 0xEAB4 },  // glyphnumber: 2568, Narrow wavy line segment
            { "wiggleWavyWide", 0xEAB6 },  // glyphnumber: 2569, Wide wavy line segment
            { "windClosedHole", 0xE5F4 },  // glyphnumber: 2570, Closed hole
            { "windFlatEmbouchure", 0xE5FB },  // glyphnumber: 2571, Flatter embouchure
            { "windHalfClosedHole1", 0xE5F6 },  // glyphnumber: 2572, Half-closed hole
            { "windHalfClosedHole2", 0xE5F7 },  // glyphnumber: 2573, Half-closed hole 2
            { "windHalfClosedHole3", 0xE5F8 },  // glyphnumber: 2574, Half-open hole
            { "windLessRelaxedEmbouchure", 0xE5FE },  // glyphnumber: 2575, Somewhat relaxed embouchure
            { "windLessTightEmbouchure", 0xE600 },  // glyphnumber: 2576, Somewhat tight embouchure
            { "windMouthpiecePop", 0xE60A },  // glyphnumber: 2577, Mouthpiece or hand pop
            { "windMultiphonicsBlackStem", 0xE607 },  // glyphnumber: 2578, Combining multiphonics (black) for stem
            { "windMultiphonicsBlackWhiteStem", 0xE609 },  // glyphnumber: 2579, Combining multiphonics (black and white) for stem
            { "windMultiphonicsWhiteStem", 0xE608 },  // glyphnumber: 2580, Combining multiphonics (white) for stem
            { "windOpenHole", 0xE5F9 },  // glyphnumber: 2581, Open hole
            { "windReedPositionIn", 0xE606 },  // glyphnumber: 2582, Much more reed (push inwards)
            { "windReedPositionNormal", 0xE604 },  // glyphnumber: 2583, Normal reed position
            { "windReedPositionOut", 0xE605 },  // glyphnumber: 2584, Very little reed (pull outwards)
            { "windRelaxedEmbouchure", 0xE5FD },  // glyphnumber: 2585, Relaxed embouchure
            { "windRimOnly", 0xE60B },  // glyphnumber: 2586, Rim only
            { "windSharpEmbouchure", 0xE5FC },  // glyphnumber: 2587, Sharper embouchure
            { "windStrongAirPressure", 0xE603 },  // glyphnumber: 2588, Very tight embouchure / strong air pressure
            { "windThreeQuartersClosedHole", 0xE5F5 },  // glyphnumber: 2589, Three-quarters closed hole
            { "windTightEmbouchure", 0xE5FF },  // glyphnumber: 2590, Tight embouchure
            { "windTrillKey", 0xE5FA },  // glyphnumber: 2591, Trill key
            { "windVeryTightEmbouchure", 0xE601 },  // glyphnumber: 2592, Very tight embouchure
            { "windWeakAirPressure", 0xE602 },  // glyphnumber: 2593, Very relaxed embouchure / weak air-pressure
        };
        
        SmuflGlyphname::SmuflGlyphname( std::string inNameAbove,
                                        std::string inNameBelow,
                                        std::string inNameUnspecified )
        : above{ inNameAbove }
        , below{ inNameBelow }
        , unspecified{ inNameUnspecified }
        {
            
        }
        
        
        const std::string& SmuflGlyphname::getName() const
        {
            return unspecified;
        }
        
        
        const std::string& SmuflGlyphname::getName( Placement placement ) const
        {
            switch ( placement )
            {
                case Placement::above: return above;
                case Placement::below: return below;
                case Placement::unspecified: return unspecified;
                default: break;
            }
            return unspecified;
        }

        Smufl& Smufl::getInstance()
        {
            static Smufl instance;
            return instance;
                
        }
        
        Smufl::Smufl()
        {
        }
        
        char16_t Smufl::findCodepoint(const std::string& inName)
        {
            const SmuflIter iter = sMap.find( inName );
            if( iter == sMap.cend() )
            {
                std::cout << "WARN: findCodepoint missing: " << inName << std::endl;
                return 0;
            }
            return iter->second;
        }
        
        
        const std::string& Smufl::findName( char16_t codepoint )
        {
            static std::string empty = "";
            
            auto lambda = [&codepoint](const SmuflPair item)
            {
                return item.second == codepoint;
            };
            
            const SmuflIter iter = std::find_if( sMap.cbegin(), sMap.cend(), lambda );
            
            if( iter == sMap.cend() )
            {
                std::cout << "WARN: findName missing: " << codepoint << std::endl;
                return empty;
            }
            
            return iter->first;
        }
    }
}
