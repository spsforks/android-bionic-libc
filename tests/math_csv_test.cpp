/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /*
  * This file is auto-generated.
  * Create with: python bionic/tests/create_bionic_tests.py libcore/luni/src/test/resources/math_tests.csv bionic/tests/math_csv_test.cpp
  */

#include <math.h>
#include <gtest/gtest.h>

#if defined(__BIONIC__)
struct TestCase {
  double expected;
  double call_data;
};

static TestCase g_sinh_data[] = {
  { // Entry 0
    0x1.e2d3246d48e6fp2,
    0x1.5bf0a8b145769p1
  },
  { // Entry 1
    -0x1.e2d3246d48e6fp2,
    -0x1.5bf0a8b145769p1
  },
  { // Entry 2
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 3
    -0x0.0p0,
    -0x0.0p0
  },
  { // Entry 4
    0x1.2cd9fc44eb982p0,
    0x1.0p0
  },
  { // Entry 5
    -0x1.2cd9fc44eb982p0,
    -0x1.0p0
  },
  { // Entry 6
    -0x1.0acd00fe63b97p-1,
    -0x1.0p-1
  },
  { // Entry 7
    0x1.0acd00fe63b97p-1,
    0x1.0p-1
  },
  { // Entry 8
    -0x1.0bbeb1603926ap8,
    -0x1.921fb54442d18p2
  },
  { // Entry 9
    -0x1.bd3c34cf00a25p5,
    -0x1.2d97c7f3321d2p2
  },
  { // Entry 10
    -0x1.718f45d72e672p3,
    -0x1.921fb54442d18p1
  },
  { // Entry 11
    -0x1.2690f661dd82p1,
    -0x1.921fb54442d18p0
  },
  { // Entry 12
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 13
    0x1.2690f661dd82p1,
    0x1.921fb54442d18p0
  },
  { // Entry 14
    0x1.718f45d72e672p3,
    0x1.921fb54442d18p1
  },
  { // Entry 15
    0x1.bd3c34cf00a25p5,
    0x1.2d97c7f3321d2p2
  },
  { // Entry 16
    0x1.0bbeb1603926ap8,
    0x1.921fb54442d18p2
  },
};

static TestCase g_asin_data[] = {
  { // Entry 0
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 1
    -0x0.0p0,
    -0x0.0p0
  },
  { // Entry 2
    0x1.921fb54442d18p0,
    0x1.0p0
  },
  { // Entry 3
    -0x1.921fb54442d18p0,
    -0x1.0p0
  },
  { // Entry 4
    -0x1.0c152382d7366p-1,
    -0x1.0p-1
  },
  { // Entry 5
    0x1.0c152382d7366p-1,
    0x1.0p-1
  },
  { // Entry 6
    0x0.0p0,
    0x0.0p0
  },
};

static TestCase g_cos_data[] = {
  { // Entry 0
    0x1.c1a27ae836f13p-1,
    0x1.feb1f7920e248p-2

  },
  { // Entry 1
    0x1.c1a27ae836f13p-1,
    -0x1.feb1f7920e248p-2

  },
  { // Entry 2
    0x1.78daf01036d0dp-1,
    0x1.7cb7648526f99p-1

  },
  { // Entry 3
    0x1.78daf01036d0dp-1,
    -0x1.7cb7648526f99p-1

  },
  { // Entry 4
    0x1.ff8eb6a91ecbp-1,
    0x1.549ec0c0c5afap-5

  },
  { // Entry 5
    0x1.ff8eb6a91ecbp-1,
    -0x1.549ec0c0c5afap-5

  },
  { // Entry 6
    0x1.fed0476fc75cap-1,
    0x1.16e534ee3658p-4

  },
  { // Entry 7
    0x1.fed0476fc75cap-1,
    -0x1.16e534ee3658p-4

  },
  { // Entry 8
    0x1.f10fc61e2c78fp-1,
    0x1.efeef61d39ac2p-3

  },
  { // Entry 9
    0x1.f10fc61e2c78fp-1,
    -0x1.efeef61d39ac2p-3

  },
  { // Entry 10
    0x1.434a3645be208p-1,
    0x1.c65a170474549p-1

  },
  { // Entry 11
    0x1.434a3645be208p-1,
    -0x1.c65a170474549p-1

  },
  { // Entry 12
    0x1.337fc5b072c53p-3,
    0x1.6b8a6273d7c21p0

  },
  { // Entry 13
    0x1.337fc5b072c53p-3,
    -0x1.6b8a6273d7c21p0

  },
  { // Entry 14
    0x1.efa7cddb128fcp-1,
    -0x1.036f4ba7e90aap-2

  },
  { // Entry 15
    0x1.efa7cddb128fcp-1,
    0x1.036f4ba7e90aap-2

  },
  { // Entry 16
    0x1.0p0,
    -0x1.1500766c9df2p-31

  },
  { // Entry 17
    0x1.0p0,
    0x1.1500766c9df2p-31

  },
  { // Entry 18
    0x1.ec231802917bep-1,
    -0x1.1e2a1563e068ep-2

  },
  { // Entry 19
    0x1.ec231802917bep-1,
    0x1.1e2a1563e068ep-2

  },
  { // Entry 20
    0x1.dc044ac92b7fcp-8,
    -0x1.2115aa73f8d05p5

  },
  { // Entry 21
    0x1.dc044ac92b7fcp-8,
    0x1.2115aa73f8d05p5

  },
  { // Entry 22
    0x1.d1fa67c50dd53p-4,
    -0x1.34e3bcdf8f69ap2

  },
  { // Entry 23
    0x1.d1fa67c50dd53p-4,
    0x1.34e3bcdf8f69ap2

  },
  { // Entry 24
    0x1.e2f8d19fb8db8p-2,
    -0x1.380000000000bp7

  },
  { // Entry 25
    0x1.e2f8d19fb8db8p-2,
    0x1.380000000000bp7

  },
  { // Entry 26
    0x1.8da9c90c3eda1p-1,
    -0x1.440000004p6

  },
  { // Entry 27
    0x1.8da9c90c3eda1p-1,
    0x1.440000004p6

  },
  { // Entry 28
    0x1.b59b320603f83p-1,
    -0x1.550c8ee67a4c4p29

  },
  { // Entry 29
    0x1.b59b320603f83p-1,
    0x1.550c8ee67a4c4p29

  },
  { // Entry 30
    0x1.ffffff7af6c88p-1,
    -0x1.711789fdb2e8ap-13

  },
  { // Entry 31
    0x1.ffffff7af6c88p-1,
    0x1.711789fdb2e8ap-13

  },
  { // Entry 32
    0x1.c1b68ebb0b4fep-2,
    -0x1.77e000002p8

  },
  { // Entry 33
    0x1.c1b68ebb0b4fep-2,
    0x1.77e000002p8

  },
  { // Entry 34
    0x1.1161e1dad76dcp-4,
    -0x1.8106561931b43p0

  },
  { // Entry 35
    0x1.1161e1dad76dcp-4,
    0x1.8106561931b43p0

  },
  { // Entry 36
    0x1.f828c3226b3d7p-5,
    -0x1.825be2461cad4p0

  },
  { // Entry 37
    0x1.f828c3226b3d7p-5,
    0x1.825be2461cad4p0

  },
  { // Entry 38
    0x1.f2990d742e9fbp-5,
    -0x1.8288755803b08p0

  },
  { // Entry 39
    0x1.f2990d742e9fbp-5,
    0x1.8288755803b08p0

  },
  { // Entry 40
    -0x1.ff150dda7524dp-1,
    -0x1.8a75701f4ccd3p1

  },
  { // Entry 41
    -0x1.ff150dda7524dp-1,
    0x1.8a75701f4ccd3p1

  },
  { // Entry 42
    0x1.015c47c32b574p-1,
    -0x1.b389316f37f37p3

  },
  { // Entry 43
    0x1.015c47c32b574p-1,
    0x1.b389316f37f37p3

  },
  { // Entry 44
    0x1.d681a366a0534p-1,
    -0x1.c602c465d7d27p6

  },
  { // Entry 45
    0x1.d681a366a0534p-1,
    0x1.c602c465d7d27p6

  },
  { // Entry 46
    -0x1.84e896c7543d6p-1,
    -0x1.cfb81fe69664cp4

  },
  { // Entry 47
    -0x1.84e896c7543d6p-1,
    0x1.cfb81fe69664cp4

  },
  { // Entry 48
    0x1.fc5dcfddd54cp-1,
    -0x1.d08f2d86b12c6p13

  },
  { // Entry 49
    0x1.fc5dcfddd54cp-1,
    0x1.d08f2d86b12c6p13

  },
  { // Entry 50
    0x1.fe83235fbe016p-3,
    -0x1.de13f0943c494p99

  },
  { // Entry 51
    0x1.fe83235fbe016p-3,
    0x1.de13f0943c494p99

  },
  { // Entry 52
    -0x1.720321239ec5p-1,
    -0x1.de3c1f1285e8bp3

  },
  { // Entry 53
    -0x1.720321239ec5p-1,
    0x1.de3c1f1285e8bp3

  },
  { // Entry 54
    0x1.f7143c8bba407p-4,
    -0x1.fffffffffff7fp1023

  },
  { // Entry 55
    0x1.f7143c8bba407p-4,
    0x1.fffffffffff7fp1023

  },
  { // Entry 56
    -0x1.fffe62ecfab75p-1,
    -0x1.fffffffffffffp1023

  },
  { // Entry 57
    -0x1.fffe62ecfab75p-1,
    0x1.fffffffffffffp1023

  },
  { // Entry 58
    0x1.055e457ac1227p-5,
    0x1.0000000000001p51

  },
  { // Entry 59
    0x1.055e457ac1227p-5,
    -0x1.0000000000001p51

  },
  { // Entry 60
    0x1.c1528065b7d4ep-1,
    0x1.0000000000003p-1

  },
  { // Entry 61
    0x1.c1528065b7d4ep-1,
    -0x1.0000000000003p-1

  },
  { // Entry 62
    0x1.0p0,
    0x1.0000000000003p-32

  },
  { // Entry 63
    0x1.0p0,
    -0x1.0000000000003p-32

  },
  { // Entry 64
    0x1.fffea444bc05ep-1,
    0x1.000000000002p150

  },
  { // Entry 65
    0x1.fffea444bc05ep-1,
    -0x1.000000000002p150

  },
  { // Entry 66
    -0x1.ebddee876f434p-1,
    0x1.0000000000038p380

  },
  { // Entry 67
    -0x1.ebddee876f434p-1,
    -0x1.0000000000038p380

  },
  { // Entry 68
    -0x1.f2ffc51dc6968p-1,
    0x1.0000000000118p380

  },
  { // Entry 69
    -0x1.f2ffc51dc6968p-1,
    -0x1.0000000000118p380

  },
  { // Entry 70
    -0x1.f8fbb4d358b2p-1,
    0x1.00000000003ffp641

  },
  { // Entry 71
    -0x1.f8fbb4d358b2p-1,
    -0x1.00000000003ffp641

  },
  { // Entry 72
    -0x1.aa2265753e668p-2,
    0x1.0000000001p1

  },
  { // Entry 73
    -0x1.aa2265753e668p-2,
    -0x1.0000000001p1

  },
  { // Entry 74
    0x1.fd1242c25994dp-1,
    0x1.000000008p452

  },
  { // Entry 75
    0x1.fd1242c25994dp-1,
    -0x1.000000008p452

  },
  { // Entry 76
    0x1.fffc0001554dap-1,
    0x1.00000000effafp-7

  },
  { // Entry 77
    0x1.fffc0001554dap-1,
    -0x1.00000000effafp-7

  },
  { // Entry 78
    0x1.14a27f2925522p-1,
    0x1.00000114fefe2p0

  },
  { // Entry 79
    0x1.14a27f2925522p-1,
    -0x1.00000114fefe2p0

  },
  { // Entry 80
    0x1.bf81e0269c59dp-3,
    0x1.000007p40

  },
  { // Entry 81
    0x1.bf81e0269c59dp-3,
    -0x1.000007p40

  },
  { // Entry 82
    0x1.14a26ed1960d6p-1,
    0x1.00000acadb3d3p0

  },
  { // Entry 83
    0x1.14a26ed1960d6p-1,
    -0x1.00000acadb3d3p0

  },
  { // Entry 84
    0x1.ffffffffbfffep-1,
    0x1.00003p-17

  },
  { // Entry 85
    0x1.ffffffffbfffep-1,
    -0x1.00003p-17

  },
  { // Entry 86
    0x1.ffffffffeffffp-1,
    0x1.00003ffffffaep-18

  },
  { // Entry 87
    0x1.ffffffffeffffp-1,
    -0x1.00003ffffffaep-18

  },
  { // Entry 88
    0x1.ffffffffeffffp-1,
    0x1.00003ffffffffp-18

  },
  { // Entry 89
    0x1.ffffffffeffffp-1,
    -0x1.00003ffffffffp-18

  },
  { // Entry 90
    -0x1.dab7efeb35baep-2,
    0x1.00007ffffdeap41

  },
  { // Entry 91
    -0x1.dab7efeb35baep-2,
    -0x1.00007ffffdeap41

  },
  { // Entry 92
    0x1.fffffffffcp-1,
    0x1.0000ffff8p-19

  },
  { // Entry 93
    0x1.fffffffffcp-1,
    -0x1.0000ffff8p-19

  },
  { // Entry 94
    0x1.ffffffffffp-1,
    0x1.0003fff800051p-20

  },
  { // Entry 95
    0x1.ffffffffffp-1,
    -0x1.0003fff800051p-20

  },
  { // Entry 96
    0x1.ffffffffffp-1,
    0x1.0003fff800096p-20

  },
  { // Entry 97
    0x1.ffffffffffp-1,
    -0x1.0003fff800096p-20

  },
  { // Entry 98
    -0x1.fbf2b71a23a58p-2,
    0x1.000fd2p334

  },
  { // Entry 99
    -0x1.fbf2b71a23a58p-2,
    -0x1.000fd2p334

  },
  { // Entry 100
    0x1.fccc87eae7737p-5,
    0x1.003p514

  },
  { // Entry 101
    0x1.fccc87eae7737p-5,
    -0x1.003p514

  },
  { // Entry 102
    -0x1.a43f40d92b7edp-7,
    0x1.00600000015f4p41

  },
  { // Entry 103
    -0x1.a43f40d92b7edp-7,
    -0x1.00600000015f4p41

  },
  { // Entry 104
    0x1.c11cc38f40ab3p-1,
    0x1.007p-1

  },
  { // Entry 105
    0x1.c11cc38f40ab3p-1,
    -0x1.007p-1

  },
  { // Entry 106
    0x1.ffffffffffbfcp-1,
    0x1.007p-21

  },
  { // Entry 107
    0x1.ffffffffffbfcp-1,
    -0x1.007p-21

  },
  { // Entry 108
    0x1.e9ba98231f734p-8,
    0x1.00cp40

  },
  { // Entry 109
    0x1.e9ba98231f734p-8,
    -0x1.00cp40

  },
  { // Entry 110
    0x1.fefdf48ed649dp-1,
    0x1.011p-4

  },
  { // Entry 111
    0x1.fefdf48ed649dp-1,
    -0x1.011p-4

  },
  { // Entry 112
    -0x1.ffc16a0f12ff2p-1,
    0x1.011p996

  },
  { // Entry 113
    -0x1.ffc16a0f12ff2p-1,
    -0x1.011p996

  },
  { // Entry 114
    0x1.efd5b61a30a38p-1,
    0x1.02p-2

  },
  { // Entry 115
    0x1.efd5b61a30a38p-1,
    -0x1.02p-2

  },
  { // Entry 116
    0x1.c97b8161dc50ap-2,
    0x1.0204260c18307p59

  },
  { // Entry 117
    0x1.c97b8161dc50ap-2,
    -0x1.0204260c18307p59

  },
  { // Entry 118
    -0x1.bf26a3c9b9fbfp-2,
    0x1.02e78a321155ep1

  },
  { // Entry 119
    -0x1.bf26a3c9b9fbfp-2,
    -0x1.02e78a321155ep1

  },
  { // Entry 120
    0x1.fef806b1f84e5p-1,
    0x1.04p-4

  },
  { // Entry 121
    0x1.fef806b1f84e5p-1,
    -0x1.04p-4

  },
  { // Entry 122
    0x1.fe851fbf87d17p-1,
    0x1.04bde8bb80258p98

  },
  { // Entry 123
    0x1.fe851fbf87d17p-1,
    -0x1.04bde8bb80258p98

  },
  { // Entry 124
    0x1.70f6a51da8effp-1,
    0x1.077e749e37ceep236

  },
  { // Entry 125
    0x1.70f6a51da8effp-1,
    -0x1.077e749e37ceep236

  },
  { // Entry 126
    0x1.6b408c856bda6p-3,
    0x1.07f8p300

  },
  { // Entry 127
    0x1.6b408c856bda6p-3,
    -0x1.07f8p300

  },
  { // Entry 128
    0x1.2b2f965ae40fcp-1,
    0x1.07f9bea1b3546p27

  },
  { // Entry 129
    0x1.2b2f965ae40fcp-1,
    -0x1.07f9bea1b3546p27

  },
  { // Entry 130
    -0x1.4eed2f3fc76a8p-1,
    0x1.090d18372f2d5p4

  },
  { // Entry 131
    -0x1.4eed2f3fc76a8p-1,
    -0x1.090d18372f2d5p4

  },
  { // Entry 132
    0x1.fba59aecee5p-1,
    0x1.0b4p-3

  },
  { // Entry 133
    0x1.fba59aecee5p-1,
    -0x1.0b4p-3

  },
  { // Entry 134
    0x1.fffd1bcda7a7dp-1,
    0x1.0c0d5c2af3c2ep346

  },
  { // Entry 135
    0x1.fffd1bcda7a7dp-1,
    -0x1.0c0d5c2af3c2ep346

  },
  { // Entry 136
    -0x1.e4dfe83129286p-1,
    0x1.0d30596ee91fdp216

  },
  { // Entry 137
    -0x1.e4dfe83129286p-1,
    -0x1.0d30596ee91fdp216

  },
  { // Entry 138
    0x1.fb8432886a284p-2,
    0x1.0d6p0

  },
  { // Entry 139
    0x1.fb8432886a284p-2,
    -0x1.0d6p0

  },
  { // Entry 140
    0x1.ffffee202854p-1,
    0x1.0e9474c68831cp-10

  },
  { // Entry 141
    0x1.ffffee202854p-1,
    -0x1.0e9474c68831cp-10

  },
  { // Entry 142
    -0x1.b70d3d5584b1bp-2,
    0x1.113bae4049849p2

  },
  { // Entry 143
    -0x1.b70d3d5584b1bp-2,
    -0x1.113bae4049849p2

  },
  { // Entry 144
    0x1.fed8df58f626p-1,
    0x1.12eb87097654p-4

  },
  { // Entry 145
    0x1.fed8df58f626p-1,
    -0x1.12eb87097654p-4

  },
  { // Entry 146
    0x1.e536ae395dfcep-2,
    0x1.13cp0

  },
  { // Entry 147
    0x1.e536ae395dfcep-2,
    -0x1.13cp0

  },
  { // Entry 148
    0x1.fed0476fc75cap-1,
    0x1.16e534ee3658p-4

  },
  { // Entry 149
    0x1.fed0476fc75cap-1,
    -0x1.16e534ee3658p-4

  },
  { // Entry 150
    0x1.fb38e82e3193ap-1,
    0x1.17fffffffea98p-3

  },
  { // Entry 151
    0x1.fb38e82e3193ap-1,
    -0x1.17fffffffea98p-3

  },
  { // Entry 152
    0x1.fb38e82e3188p-1,
    0x1.18p-3

  },
  { // Entry 153
    0x1.fb38e82e3188p-1,
    -0x1.18p-3

  },
  { // Entry 154
    -0x1.e59647f1fe9c7p-1,
    0x1.1a191ebbb4d7fp7

  },
  { // Entry 155
    -0x1.e59647f1fe9c7p-1,
    -0x1.1a191ebbb4d7fp7

  },
  { // Entry 156
    -0x1.d0dca1f8715bep-4,
    0x1.1da84f2b7b1d8p7

  },
  { // Entry 157
    -0x1.d0dca1f8715bep-4,
    -0x1.1da84f2b7b1d8p7

  },
  { // Entry 158
    0x1.b917ebbc30e1ep-2,
    0x1.201e973251302p0

  },
  { // Entry 159
    0x1.b917ebbc30e1ep-2,
    -0x1.201e973251302p0

  },
  { // Entry 160
    0x1.fffadf12ff414p-1,
    0x1.21e02p-7

  },
  { // Entry 161
    0x1.fffadf12ff414p-1,
    -0x1.21e02p-7

  },
  { // Entry 162
    -0x1.598a4dab3de5ap-1,
    0x1.27e29a4b985bfp1

  },
  { // Entry 163
    -0x1.598a4dab3de5ap-1,
    -0x1.27e29a4b985bfp1

  },
  { // Entry 164
    0x1.fa95c1154abf5p-1,
    0x1.2a1f28dbfb6cp-3

  },
  { // Entry 165
    0x1.fa95c1154abf5p-1,
    -0x1.2a1f28dbfb6cp-3

  },
  { // Entry 166
    -0x1.6412293adb7bcp-1,
    0x1.2b8p1

  },
  { // Entry 167
    -0x1.6412293adb7bcp-1,
    -0x1.2b8p1

  },
  { // Entry 168
    0x1.fffa518a7d0e7p-1,
    0x1.31199def72f4dp-7

  },
  { // Entry 169
    0x1.fffa518a7d0e7p-1,
    -0x1.31199def72f4dp-7

  },
  { // Entry 170
    0x1.f36895fe177f8p-1,
    0x1.31260e1485014p4

  },
  { // Entry 171
    0x1.f36895fe177f8p-1,
    -0x1.31260e1485014p4

  },
  { // Entry 172
    0x1.d36207b4fee17p-4,
    0x1.34e964cd103bdp2

  },
  { // Entry 173
    0x1.d36207b4fee17p-4,
    -0x1.34e964cd103bdp2

  },
  { // Entry 174
    -0x1.84a37f4fa7616p-1,
    0x1.37618a0ba785p1

  },
  { // Entry 175
    -0x1.84a37f4fa7616p-1,
    -0x1.37618a0ba785p1

  },
  { // Entry 176
    -0x1.c830bbc99e229p-39,
    0x1.379704f5f1eb3p24

  },
  { // Entry 177
    -0x1.c830bbc99e229p-39,
    -0x1.379704f5f1eb3p24

  },
  { // Entry 178
    0x1.b5daaa233bd5p-3,
    0x1.3b61dd166d47p2

  },
  { // Entry 179
    0x1.b5daaa233bd5p-3,
    -0x1.3b61dd166d47p2

  },
  { // Entry 180
    -0x1.ffd00dc4db401p-4,
    0x1.3c011022acbdp37

  },
  { // Entry 181
    -0x1.ffd00dc4db401p-4,
    -0x1.3c011022acbdp37

  },
  { // Entry 182
    -0x1.14052b4016ff5p-1,
    0x1.3e7788e900b7p727

  },
  { // Entry 183
    -0x1.14052b4016ff5p-1,
    -0x1.3e7788e900b7p727

  },
  { // Entry 184
    0x1.ffffe6a5e4198p-1,
    0x1.423eafdcc2779p-10

  },
  { // Entry 185
    0x1.ffffe6a5e4198p-1,
    -0x1.423eafdcc2779p-10

  },
  { // Entry 186
    -0x1.fe09fc3d16feep-6,
    0x1.4321828c1b538p119

  },
  { // Entry 187
    -0x1.fe09fc3d16feep-6,
    -0x1.4321828c1b538p119

  },
  { // Entry 188
    0x1.b685d949a27ap-14,
    0x1.43506cb22975dp22

  },
  { // Entry 189
    0x1.b685d949a27ap-14,
    -0x1.43506cb22975dp22

  },
  { // Entry 190
    0x1.fe398090e203cp-1,
    0x1.439f63495786ap67

  },
  { // Entry 191
    0x1.fe398090e203cp-1,
    -0x1.439f63495786ap67

  },
  { // Entry 192
    0x1.fe6274e000974p-1,
    0x1.457538a6bd073p-4

  },
  { // Entry 193
    0x1.fe6274e000974p-1,
    -0x1.457538a6bd073p-4

  },
  { // Entry 194
    0x1.09fcb69359c0ap-1,
    0x1.478fc08p43

  },
  { // Entry 195
    0x1.09fcb69359c0ap-1,
    -0x1.478fc08p43

  },
  { // Entry 196
    -0x1.20c2158511e79p-9,
    0x1.48a45797cbe63p61

  },
  { // Entry 197
    -0x1.20c2158511e79p-9,
    -0x1.48a45797cbe63p61

  },
  { // Entry 198
    0x1.990d17aae253p-1,
    0x1.4a62e0e12c173p-1

  },
  { // Entry 199
    0x1.990d17aae253p-1,
    -0x1.4a62e0e12c173p-1

  },
  { // Entry 200
    0x1.fdd4f1e00b387p-3,
    0x1.4c596642a9488p9

  },
  { // Entry 201
    0x1.fdd4f1e00b387p-3,
    -0x1.4c596642a9488p9

  },
  { // Entry 202
    0x1.fe4f141032f38p-1,
    0x1.4dp-4

  },
  { // Entry 203
    0x1.fe4f141032f38p-1,
    -0x1.4dp-4

  },
  { // Entry 204
    0x1.94e9f45d43c14p-2,
    0x1.4f0f308p488

  },
  { // Entry 205
    0x1.94e9f45d43c14p-2,
    -0x1.4f0f308p488

  },
  { // Entry 206
    0x1.9355f69ad4326p-2,
    0x1.52f00ep793

  },
  { // Entry 207
    0x1.9355f69ad4326p-2,
    -0x1.52f00ep793

  },
  { // Entry 208
    0x1.1a19be8bea10ap-1,
    0x1.52f06c730ec02p2

  },
  { // Entry 209
    0x1.1a19be8bea10ap-1,
    -0x1.52f06c730ec02p2

  },
  { // Entry 210
    0x1.385d92ec0c734p-1,
    0x1.53e7d5845fe3dp220

  },
  { // Entry 211
    0x1.385d92ec0c734p-1,
    -0x1.53e7d5845fe3dp220

  },
  { // Entry 212
    0x1.fffffffffe2f1p-1,
    0x1.59p-20

  },
  { // Entry 213
    0x1.fffffffffe2f1p-1,
    -0x1.59p-20

  },
  { // Entry 214
    -0x1.ffd7bc28ded92p-1,
    0x1.592f1176f098p86

  },
  { // Entry 215
    -0x1.ffd7bc28ded92p-1,
    -0x1.592f1176f098p86

  },
  { // Entry 216
    -0x1.cee28b3d79799p-1,
    0x1.5999999dc09dcp1

  },
  { // Entry 217
    -0x1.cee28b3d79799p-1,
    -0x1.5999999dc09dcp1

  },
  { // Entry 218
    0x1.c1f1eb08c2604p-1,
    0x1.5bea01p468

  },
  { // Entry 219
    0x1.c1f1eb08c2604p-1,
    -0x1.5bea01p468

  },
  { // Entry 220
    0x1.fffe35ab09a65p-1,
    0x1.5cb80a6135e5ap1000

  },
  { // Entry 221
    0x1.fffe35ab09a65p-1,
    -0x1.5cb80a6135e5ap1000

  },
  { // Entry 222
    -0x1.07b85f606e75dp-3,
    0x1.5d5be48730d2dp13

  },
  { // Entry 223
    -0x1.07b85f606e75dp-3,
    -0x1.5d5be48730d2dp13

  },
  { // Entry 224
    0x1.ffffffffff862p-1,
    0x1.614p-21

  },
  { // Entry 225
    0x1.ffffffffff862p-1,
    -0x1.614p-21

  },
  { // Entry 226
    -0x1.dd3a806e89cf2p-1,
    0x1.62adc8a660364p1

  },
  { // Entry 227
    -0x1.dd3a806e89cf2p-1,
    -0x1.62adc8a660364p1

  },
  { // Entry 228
    -0x1.4308b14f4b6eep-1,
    0x1.64ef438p142

  },
  { // Entry 229
    -0x1.4308b14f4b6eep-1,
    -0x1.64ef438p142

  },
  { // Entry 230
    0x1.6623d2eb6add2p-3,
    0x1.652p0

  },
  { // Entry 231
    0x1.6623d2eb6add2p-3,
    -0x1.652p0

  },
  { // Entry 232
    0x1.fff832c50f472p-1,
    0x1.65865b2cb08a2p-7

  },
  { // Entry 233
    0x1.fff832c50f472p-1,
    -0x1.65865b2cb08a2p-7

  },
  { // Entry 234
    0x1.acc251be33023p-1,
    0x1.6a937daabc20ep375

  },
  { // Entry 235
    0x1.acc251be33023p-1,
    -0x1.6a937daabc20ep375

  },
  { // Entry 236
    -0x1.14ae72e6ba22fp-61,
    0x1.6ac5b262ca1ffp849

  },
  { // Entry 237
    -0x1.14ae72e6ba22fp-61,
    -0x1.6ac5b262ca1ffp849

  },
  { // Entry 238
    0x1.e0619960a11c6p-2,
    0x1.6f7bdef7bdef4p3

  },
  { // Entry 239
    0x1.e0619960a11c6p-2,
    -0x1.6f7bdef7bdef4p3

  },
  { // Entry 240
    0x1.8d23f97901a3p-1,
    0x1.739ce759ce738p200

  },
  { // Entry 241
    0x1.8d23f97901a3p-1,
    -0x1.739ce759ce738p200

  },
  { // Entry 242
    0x1.fffff78a14ba1p-1,
    0x1.7450c3f49d0b2p-11

  },
  { // Entry 243
    0x1.fffff78a14ba1p-1,
    -0x1.7450c3f49d0b2p-11

  },
  { // Entry 244
    0x1.d6f1c727fb2ccp-4,
    0x1.749fe53f963fdp0

  },
  { // Entry 245
    0x1.d6f1c727fb2ccp-4,
    -0x1.749fe53f963fdp0

  },
  { // Entry 246
    -0x1.f284b5028c184p-1,
    0x1.74af6725c6206p1

  },
  { // Entry 247
    -0x1.f284b5028c184p-1,
    -0x1.74af6725c6206p1

  },
  { // Entry 248
    -0x1.f3165a0b306b2p-1,
    0x1.7550d28ffccc4p1

  },
  { // Entry 249
    -0x1.f3165a0b306b2p-1,
    -0x1.7550d28ffccc4p1

  },
  { // Entry 250
    0x1.d66d2078ebdecp-1,
    0x1.775e397cd6aap6

  },
  { // Entry 251
    0x1.d66d2078ebdecp-1,
    -0x1.775e397cd6aap6

  },
  { // Entry 252
    0x1.7af9a13085f53p-1,
    0x1.799302bf7f29p-1

  },
  { // Entry 253
    0x1.7af9a13085f53p-1,
    -0x1.799302bf7f29p-1

  },
  { // Entry 254
    0x1.ffdd2fdac0c25p-1,
    0x1.799fffffffffdp-6

  },
  { // Entry 255
    0x1.ffdd2fdac0c25p-1,
    -0x1.799fffffffffdp-6

  },
  { // Entry 256
    0x1.fff744f185a73p-1,
    0x1.7a3692ca9449p-7

  },
  { // Entry 257
    0x1.fff744f185a73p-1,
    -0x1.7a3692ca9449p-7

  },
  { // Entry 258
    0x1.7a6b326b690fbp-1,
    0x1.7a66a638ac5b5p-1

  },
  { // Entry 259
    0x1.7a6b326b690fbp-1,
    -0x1.7a66a638ac5b5p-1

  },
  { // Entry 260
    0x1.671fdb64ffbeep-4,
    0x1.7ba65462b49ap0

  },
  { // Entry 261
    0x1.671fdb64ffbeep-4,
    -0x1.7ba65462b49ap0

  },
  { // Entry 262
    0x1.ffa55490f206ep-1,
    0x1.7cdf37cdf37c9p239

  },
  { // Entry 263
    0x1.ffa55490f206ep-1,
    -0x1.7cdf37cdf37c9p239

  },
  { // Entry 264
    0x1.4c5b5970a3a49p-4,
    0x1.7d542565f472ep0

  },
  { // Entry 265
    0x1.4c5b5970a3a49p-4,
    -0x1.7d542565f472ep0

  },
  { // Entry 266
    0x1.479a5667c63f6p-4,
    0x1.7da0751649058p0

  },
  { // Entry 267
    0x1.479a5667c63f6p-4,
    -0x1.7da0751649058p0

  },
  { // Entry 268
    0x1.fff717511dcb5p-1,
    0x1.7e0ddcda6cc0dp-7

  },
  { // Entry 269
    0x1.fff717511dcb5p-1,
    -0x1.7e0ddcda6cc0dp-7

  },
  { // Entry 270
    0x1.ffffffffff707p-1,
    0x1.7f6p-21

  },
  { // Entry 271
    0x1.ffffffffff707p-1,
    -0x1.7f6p-21

  },
  { // Entry 272
    -0x1.fff9e1554698p-1,
    0x1.7f90117d44c74p100

  },
  { // Entry 273
    -0x1.fff9e1554698p-1,
    -0x1.7f90117d44c74p100

  },
  { // Entry 274
    0x1.ffdc006bff7eap-1,
    0x1.7ffffffffef7ap-6

  },
  { // Entry 275
    0x1.ffdc006bff7eap-1,
    -0x1.7ffffffffef7ap-6

  },
  { // Entry 276
    0x1.ffdc006bff7e8p-1,
    0x1.7fffffffffa26p-6

  },
  { // Entry 277
    0x1.ffdc006bff7e8p-1,
    -0x1.7fffffffffa26p-6

  },
  { // Entry 278
    0x1.ffdc006bff7e7p-1,
    0x1.7ffffffffff8p-6

  },
  { // Entry 279
    0x1.ffdc006bff7e7p-1,
    -0x1.7ffffffffff8p-6

  },
  { // Entry 280
    0x1.760718ab44398p-1,
    0x1.80ep-1

  },
  { // Entry 281
    0x1.760718ab44398p-1,
    -0x1.80ep-1

  },
  { // Entry 282
    -0x1.0p0,
    0x1.81ae0dffa3b33p959

  },
  { // Entry 283
    -0x1.0p0,
    -0x1.81ae0dffa3b33p959

  },
  { // Entry 284
    -0x1.fbdc48125b345p-1,
    0x1.81d612289c5cfp1

  },
  { // Entry 285
    -0x1.fbdc48125b345p-1,
    -0x1.81d612289c5cfp1

  },
  { // Entry 286
    0x1.ff9e396651ccap-5,
    0x1.8220192270a0ep0

  },
  { // Entry 287
    0x1.ff9e396651ccap-5,
    -0x1.8220192270a0ep0

  },
  { // Entry 288
    0x1.fe2b26dddb5c9p-5,
    0x1.822bb780e9104p0

  },
  { // Entry 289
    0x1.fe2b26dddb5c9p-5,
    -0x1.822bb780e9104p0

  },
  { // Entry 290
    0x1.eb87cff7c9115p-5,
    0x1.82c119c4b8e49p0

  },
  { // Entry 291
    0x1.eb87cff7c9115p-5,
    -0x1.82c119c4b8e49p0

  },
  { // Entry 292
    0x1.eb87cff7a62b7p-5,
    0x1.82c119c4b9fc4p0

  },
  { // Entry 293
    0x1.eb87cff7a62b7p-5,
    -0x1.82c119c4b9fc4p0

  },
  { // Entry 294
    0x1.eb87cff795ab1p-5,
    0x1.82c119c4ba808p0

  },
  { // Entry 295
    0x1.eb87cff795ab1p-5,
    -0x1.82c119c4ba808p0

  },
  { // Entry 296
    0x1.de1d17ab0d6a5p-5,
    0x1.832c9fc76527p0

  },
  { // Entry 297
    0x1.de1d17ab0d6a5p-5,
    -0x1.832c9fc76527p0

  },
  { // Entry 298
    0x1.dc86e7bec0c45p-5,
    0x1.833956ce7d1f9p0

  },
  { // Entry 299
    0x1.dc86e7bec0c45p-5,
    -0x1.833956ce7d1f9p0

  },
  { // Entry 300
    0x1.db03cbb942a7bp-5,
    0x1.834574eb1c099p0

  },
  { // Entry 301
    0x1.db03cbb942a7bp-5,
    -0x1.834574eb1c099p0

  },
  { // Entry 302
    0x1.ce431710d1507p-5,
    0x1.83aba5688e13ep0

  },
  { // Entry 303
    0x1.ce431710d1507p-5,
    -0x1.83aba5688e13ep0

  },
  { // Entry 304
    0x1.cd46b3a77f6ddp-5,
    0x1.83b38bbafd75bp0

  },
  { // Entry 305
    0x1.cd46b3a77f6ddp-5,
    -0x1.83b38bbafd75bp0

  },
  { // Entry 306
    -0x1.ff29bc666bee7p-1,
    0x1.86a017cb1c31cp16

  },
  { // Entry 307
    -0x1.ff29bc666bee7p-1,
    -0x1.86a017cb1c31cp16

  },
  { // Entry 308
    -0x1.7968916e4c646p-2,
    0x1.8720588p392

  },
  { // Entry 309
    -0x1.7968916e4c646p-2,
    -0x1.8720588p392

  },
  { // Entry 310
    0x1.fb97c7e452918p-1,
    0x1.88a2288a22888p9

  },
  { // Entry 311
    0x1.fb97c7e452918p-1,
    -0x1.88a2288a22888p9

  },
  { // Entry 312
    -0x1.ae44a5f01bf63p-1,
    0x1.8cf013991c308p1000

  },
  { // Entry 313
    -0x1.ae44a5f01bf63p-1,
    -0x1.8cf013991c308p1000

  },
  { // Entry 314
    0x1.d96e82f71a9dcp-1,
    0x1.9p-2

  },
  { // Entry 315
    0x1.d96e82f71a9dcp-1,
    -0x1.9p-2

  },
  { // Entry 316
    0x1.0fd9d5c093df5p-7,
    0x1.9p0

  },
  { // Entry 317
    0x1.0fd9d5c093df5p-7,
    -0x1.9p0

  },
  { // Entry 318
    0x1.0fd9d5c05e5fdp-7,
    0x1.90000000006bp0

  },
  { // Entry 319
    0x1.0fd9d5c05e5fdp-7,
    -0x1.90000000006bp0

  },
  { // Entry 320
    0x1.bc8be725417d8p-1,
    0x1.900c206d44162p6

  },
  { // Entry 321
    0x1.bc8be725417d8p-1,
    -0x1.900c206d44162p6

  },
  { // Entry 322
    0x1.fffffffff63b6p-1,
    0x1.900c2af7baef3p-19

  },
  { // Entry 323
    0x1.fffffffff63b6p-1,
    -0x1.900c2af7baef3p-19

  },
  { // Entry 324
    0x1.bd464c9352d11p-1,
    0x1.900f11bd8955dp6

  },
  { // Entry 325
    0x1.bd464c9352d11p-1,
    -0x1.900f11bd8955dp6

  },
  { // Entry 326
    0x1.fffffda85cdd1p-1,
    0x1.910b35c3253d4p100

  },
  { // Entry 327
    0x1.fffffda85cdd1p-1,
    -0x1.910b35c3253d4p100

  },
  { // Entry 328
    0x1.1a62633145c07p-54,
    0x1.921fb54442d18p0

  },
  { // Entry 329
    0x1.1a62633145c07p-54,
    -0x1.921fb54442d18p0

  },
  { // Entry 330
    -0x1.2aeef4b9ea1aep-18,
    0x1.922p0

  },
  { // Entry 331
    -0x1.2aeef4b9ea1aep-18,
    -0x1.922p0

  },
  { // Entry 332
    -0x1.ffffffffd9048p-18,
    0x1.9220354442d18p0

  },
  { // Entry 333
    -0x1.ffffffffd9048p-18,
    -0x1.9220354442d18p0

  },
  { // Entry 334
    -0x1.0p0,
    0x1.9251f93aeb59dp12

  },
  { // Entry 335
    -0x1.0p0,
    -0x1.9251f93aeb59dp12

  },
  { // Entry 336
    0x1.ffb8c4d1f78a8p-1,
    0x1.943be221d909ap2

  },
  { // Entry 337
    0x1.ffb8c4d1f78a8p-1,
    -0x1.943be221d909ap2

  },
  { // Entry 338
    0x1.fff6011fdddabp-1,
    0x1.94af699302875p-7

  },
  { // Entry 339
    0x1.fff6011fdddabp-1,
    -0x1.94af699302875p-7

  },
  { // Entry 340
    0x1.d7954e7a3ee99p-1,
    0x1.999999ab7b0edp-2

  },
  { // Entry 341
    0x1.d7954e7a3ee99p-1,
    -0x1.999999ab7b0edp-2

  },
  { // Entry 342
    0x1.d7954e76c8e31p-1,
    0x1.999999bd4190bp-2

  },
  { // Entry 343
    0x1.d7954e76c8e31p-1,
    -0x1.999999bd4190bp-2

  },
  { // Entry 344
    0x1.fa23cfb820224p-1,
    0x1.9bd0f19479a24p2

  },
  { // Entry 345
    0x1.fa23cfb820224p-1,
    -0x1.9bd0f19479a24p2

  },
  { // Entry 346
    -0x1.6a09e667f3af1p-1,
    0x1.9c55835e7e83ep8

  },
  { // Entry 347
    -0x1.6a09e667f3af1p-1,
    -0x1.9c55835e7e83ep8

  },
  { // Entry 348
    0x1.fff59c1255809p-1,
    0x1.9c9942b14448dp-7

  },
  { // Entry 349
    0x1.fff59c1255809p-1,
    -0x1.9c9942b14448dp-7

  },
  { // Entry 350
    0x1.ffece5cab4ca5p-1,
    0x1.9d3d92485e2b5p523

  },
  { // Entry 351
    0x1.ffece5cab4ca5p-1,
    -0x1.9d3d92485e2b5p523

  },
  { // Entry 352
    -0x1.ff55301d3a781p-5,
    0x1.a0d068341a08p1000

  },
  { // Entry 353
    -0x1.ff55301d3a781p-5,
    -0x1.a0d068341a08p1000

  },
  { // Entry 354
    0x1.5a5615acd0dcp-1,
    0x1.a7ep-1

  },
  { // Entry 355
    0x1.5a5615acd0dcp-1,
    -0x1.a7ep-1

  },
  { // Entry 356
    0x1.766ad27a1de5p-14,
    0x1.a858343863965p119

  },
  { // Entry 357
    0x1.766ad27a1de5p-14,
    -0x1.a858343863965p119

  },
  { // Entry 358
    0x1.6bd4d5be72494p-1,
    0x1.ab190633d88eap3

  },
  { // Entry 359
    0x1.6bd4d5be72494p-1,
    -0x1.ab190633d88eap3

  },
  { // Entry 360
    0x1.ffffffffff4a5p-1,
    0x1.af4bd2f4bd2fp-21

  },
  { // Entry 361
    0x1.ffffffffff4a5p-1,
    -0x1.af4bd2f4bd2fp-21

  },
  { // Entry 362
    0x1.7ff2934ad29a8p-1,
    0x1.afa70300aee6p72

  },
  { // Entry 363
    0x1.7ff2934ad29a8p-1,
    -0x1.afa70300aee6p72

  },
  { // Entry 364
    0x1.ff866aebdce0ap-1,
    0x1.b5ab427cffb4cp94

  },
  { // Entry 365
    0x1.ff866aebdce0ap-1,
    -0x1.b5ab427cffb4cp94

  },
  { // Entry 366
    -0x1.f54f5227a4e84p-60,
    0x1.b951f1572eba5p23

  },
  { // Entry 367
    -0x1.f54f5227a4e84p-60,
    -0x1.b951f1572eba5p23

  },
  { // Entry 368
    0x1.fffd06d35579cp-1,
    0x1.b96e5b96e5b91p-8

  },
  { // Entry 369
    0x1.fffd06d35579cp-1,
    -0x1.b96e5b96e5b91p-8

  },
  { // Entry 370
    -0x1.7c4128e2aff4cp-1,
    0x1.ba3b18395d17bp8

  },
  { // Entry 371
    -0x1.7c4128e2aff4cp-1,
    -0x1.ba3b18395d17bp8

  },
  { // Entry 372
    -0x1.0p0,
    0x1.bab62ed655019p970

  },
  { // Entry 373
    -0x1.0p0,
    -0x1.bab62ed655019p970

  },
  { // Entry 374
    0x1.ffffff3e53446p-1,
    0x1.bd55aa411ab46p-13

  },
  { // Entry 375
    0x1.ffffff3e53446p-1,
    -0x1.bd55aa411ab46p-13

  },
  { // Entry 376
    -0x1.7fdb07b9f77ep-1,
    0x1.bd616d4fe95cdp36

  },
  { // Entry 377
    -0x1.7fdb07b9f77ep-1,
    -0x1.bd616d4fe95cdp36

  },
  { // Entry 378
    0x1.ffcf4da76222dp-1,
    0x1.beap-6

  },
  { // Entry 379
    0x1.ffcf4da76222dp-1,
    -0x1.beap-6

  },
  { // Entry 380
    -0x1.ddee13357ec6fp-1,
    0x1.c11516af585a4p1

  },
  { // Entry 381
    -0x1.ddee13357ec6fp-1,
    -0x1.c11516af585a4p1

  },
  { // Entry 382
    0x1.58cccec059da2p-1,
    0x1.c75e54de4c06ep2

  },
  { // Entry 383
    0x1.58cccec059da2p-1,
    -0x1.c75e54de4c06ep2

  },
  { // Entry 384
    -0x1.0p0,
    0x1.cb44e86bc192bp648

  },
  { // Entry 385
    -0x1.0p0,
    -0x1.cb44e86bc192bp648

  },
  { // Entry 386
    0x1.0p0,
    0x1.cb44e86bc192bp649

  },
  { // Entry 387
    0x1.0p0,
    -0x1.cb44e86bc192bp649

  },
  { // Entry 388
    -0x1.ca281d7fe44bp-1,
    0x1.cd5a6f8762affp1

  },
  { // Entry 389
    -0x1.ca281d7fe44bp-1,
    -0x1.cd5a6f8762affp1

  },
  { // Entry 390
    0x1.e80ad4fe54c72p-5,
    0x1.d0cb95f02ad77p464

  },
  { // Entry 391
    0x1.e80ad4fe54c72p-5,
    -0x1.d0cb95f02ad77p464

  },
  { // Entry 392
    0x1.0df8eb409efe4p-1,
    0x1.d31bd604903ap2

  },
  { // Entry 393
    0x1.0df8eb409efe4p-1,
    -0x1.d31bd604903ap2

  },
  { // Entry 394
    0x1.ff2ae968efe71p-1,
    0x1.d32f4610180f6p-5

  },
  { // Entry 395
    0x1.ff2ae968efe71p-1,
    -0x1.d32f4610180f6p-5

  },
  { // Entry 396
    -0x1.cec307a674d3fp-3,
    0x1.d96e058p488

  },
  { // Entry 397
    -0x1.cec307a674d3fp-3,
    -0x1.d96e058p488

  },
  { // Entry 398
    -0x1.ac8dbf9cdc955p-5,
    0x1.db0803c392b4cp15

  },
  { // Entry 399
    -0x1.ac8dbf9cdc955p-5,
    -0x1.db0803c392b4cp15

  },
  { // Entry 400
    -0x1.ac94870ca6317p-5,
    0x1.db0803c3ff51dp15

  },
  { // Entry 401
    -0x1.ac94870ca6317p-5,
    -0x1.db0803c3ff51dp15

  },
  { // Entry 402
    0x1.ff229073fd8b6p-1,
    0x1.dc4p-5

  },
  { // Entry 403
    0x1.ff229073fd8b6p-1,
    -0x1.dc4p-5

  },
  { // Entry 404
    0x1.ff21e5f976p-1,
    0x1.dcf73dcf73dccp-5

  },
  { // Entry 405
    0x1.ff21e5f976p-1,
    -0x1.dcf73dcf73dccp-5

  },
  { // Entry 406
    0x1.2f011326420e5p-1,
    0x1.dffffffffffffp-1

  },
  { // Entry 407
    0x1.2f011326420e5p-1,
    -0x1.dffffffffffffp-1

  },
  { // Entry 408
    0x1.f72c8e16dbc79p-1,
    0x1.e123691a7c4bep26

  },
  { // Entry 409
    0x1.f72c8e16dbc79p-1,
    -0x1.e123691a7c4bep26

  },
  { // Entry 410
    -0x1.4b0c6bb623f58p-2,
    0x1.e666666f9cf49p0

  },
  { // Entry 411
    -0x1.4b0c6bb623f58p-2,
    -0x1.e666666f9cf49p0

  },
  { // Entry 412
    0x1.fd74b55875885p-1,
    0x1.e83accfc50b7p995

  },
  { // Entry 413
    0x1.fd74b55875885p-1,
    -0x1.e83accfc50b7p995

  },
  { // Entry 414
    0x1.fff169b6ab7d1p-1,
    0x1.e8ep-7

  },
  { // Entry 415
    0x1.fff169b6ab7d1p-1,
    -0x1.e8ep-7

  },
  { // Entry 416
    0x1.7d39c9f1b0b3cp-1,
    0x1.eaf5ea5317442p4

  },
  { // Entry 417
    0x1.7d39c9f1b0b3cp-1,
    -0x1.eaf5ea5317442p4

  },
  { // Entry 418
    0x1.7f13af7081a68p-1,
    0x1.eb0c2b00b1b83p4

  },
  { // Entry 419
    0x1.7f13af7081a68p-1,
    -0x1.eb0c2b00b1b83p4

  },
  { // Entry 420
    -0x1.7ad7b88a1fe1p-1,
    0x1.ebc6b555311c4p15

  },
  { // Entry 421
    -0x1.7ad7b88a1fe1p-1,
    -0x1.ebc6b555311c4p15

  },
  { // Entry 422
    0x1.b06b2b58a2a24p-5,
    0x1.ef7bdef7bdef2p239

  },
  { // Entry 423
    0x1.b06b2b58a2a24p-5,
    -0x1.ef7bdef7bdef2p239

  },
  { // Entry 424
    0x1.fe6ded53172a7p-1,
    0x1.efbbeefbbeef8p15

  },
  { // Entry 425
    0x1.fe6ded53172a7p-1,
    -0x1.efbbeefbbeef8p15

  },
  { // Entry 426
    -0x1.fe2bcb87a7e16p-1,
    0x1.f07c1f07c1ef7p239

  },
  { // Entry 427
    -0x1.fe2bcb87a7e16p-1,
    -0x1.f07c1f07c1ef7p239

  },
  { // Entry 428
    -0x1.79d08d6b3a883p-1,
    0x1.f0f2b5e060b29p1

  },
  { // Entry 429
    -0x1.79d08d6b3a883p-1,
    -0x1.f0f2b5e060b29p1

  },
  { // Entry 430
    0x1.f0d11d321178ep-1,
    0x1.f4p-3

  },
  { // Entry 431
    0x1.f0d11d321178ep-1,
    -0x1.f4p-3

  },
  { // Entry 432
    0x1.e3ff5b15f723ep-4,
    0x1.f43d49f947e87p9

  },
  { // Entry 433
    0x1.e3ff5b15f723ep-4,
    -0x1.f43d49f947e87p9

  },
  { // Entry 434
    -0x1.6636c9f6a87aap-1,
    0x1.f7fffffffffffp1

  },
  { // Entry 435
    -0x1.6636c9f6a87aap-1,
    -0x1.f7fffffffffffp1

  },
  { // Entry 436
    0x1.ffc1be3309286p-1,
    0x1.f8fffffffffffp-6

  },
  { // Entry 437
    0x1.ffc1be3309286p-1,
    -0x1.f8fffffffffffp-6

  },
  { // Entry 438
    0x1.ffc1be3309285p-1,
    0x1.f9p-6

  },
  { // Entry 439
    0x1.ffc1be3309285p-1,
    -0x1.f9p-6

  },
  { // Entry 440
    -0x1.fffffffcab0d6p-1,
    0x1.fa0236523ce54p344

  },
  { // Entry 441
    -0x1.fffffffcab0d6p-1,
    -0x1.fa0236523ce54p344

  },
  { // Entry 442
    0x1.fc0d98ace2308p-1,
    0x1.fceab54d37dap-4

  },
  { // Entry 443
    0x1.fc0d98ace2308p-1,
    -0x1.fceab54d37dap-4

  },
  { // Entry 444
    -0x1.9589bca128b92p-4,
    0x1.fd0072fffffffp2

  },
  { // Entry 445
    -0x1.9589bca128b92p-4,
    -0x1.fd0072fffffffp2

  },
  { // Entry 446
    -0x1.4d304b07fc898p-2,
    0x1.fe0f827673422p62

  },
  { // Entry 447
    -0x1.4d304b07fc898p-2,
    -0x1.fe0f827673422p62

  },
  { // Entry 448
    0x1.c1a27ae836f13p-1,
    0x1.feb1f7920e248p-2

  },
  { // Entry 449
    0x1.c1a27ae836f13p-1,
    -0x1.feb1f7920e248p-2

  },
  { // Entry 450
    -0x1.936b64e955979p-1,
    0x1.feeffffffffc6p995

  },
  { // Entry 451
    -0x1.936b64e955979p-1,
    -0x1.feeffffffffc6p995

  },
  { // Entry 452
    0x1.fff007147ea57p-1,
    0x1.ff8ffffffffffp-7

  },
  { // Entry 453
    0x1.fff007147ea57p-1,
    -0x1.ff8ffffffffffp-7

  },
  { // Entry 454
    0x1.ffffc01bfe443p-1,
    0x1.ff8ffffffffffp-10

  },
  { // Entry 455
    0x1.ffffc01bfe443p-1,
    -0x1.ff8ffffffffffp-10

  },
  { // Entry 456
    0x1.7cc9fb75317aep-1,
    0x1.ff8ffffffffffp870

  },
  { // Entry 457
    0x1.7cc9fb75317aep-1,
    -0x1.ff8ffffffffffp870

  },
  { // Entry 458
    0x1.d6aea48015589p-1,
    0x1.ffcfff8p19

  },
  { // Entry 459
    0x1.d6aea48015589p-1,
    -0x1.ffcfff8p19

  },
  { // Entry 460
    -0x1.6a9972eee19bbp-2,
    0x1.ffcfff8p365

  },
  { // Entry 461
    -0x1.6a9972eee19bbp-2,
    -0x1.ffcfff8p365

  },
  { // Entry 462
    -0x1.3aaa15f7544b7p-1,
    0x1.ffcffffffff6cp720

  },
  { // Entry 463
    -0x1.3aaa15f7544b7p-1,
    -0x1.ffcffffffff6cp720

  },
  { // Entry 464
    0x1.3f164bce055c5p-1,
    0x1.ffcfffffffff9p320

  },
  { // Entry 465
    0x1.3f164bce055c5p-1,
    -0x1.ffcfffffffff9p320

  },
  { // Entry 466
    0x1.fffff002fff15p-1,
    0x1.ffcffffffffffp-11

  },
  { // Entry 467
    0x1.fffff002fff15p-1,
    -0x1.ffcffffffffffp-11

  },
  { // Entry 468
    -0x1.ffffff987f986p-1,
    0x1.ffcffffffffffp405

  },
  { // Entry 469
    -0x1.ffffff987f986p-1,
    -0x1.ffcffffffffffp405

  },
  { // Entry 470
    -0x1.ffff6235a25eep-1,
    0x1.ffcffffffffffp567

  },
  { // Entry 471
    -0x1.ffff6235a25eep-1,
    -0x1.ffcffffffffffp567

  },
  { // Entry 472
    0x1.fdf11ae4608b1p-3,
    0x1.ffefff8ffffffp16

  },
  { // Entry 473
    0x1.fdf11ae4608b1p-3,
    -0x1.ffefff8ffffffp16

  },
  { // Entry 474
    0x1.8f5525ab4583cp-1,
    0x1.ffeffffffffccp995

  },
  { // Entry 475
    0x1.8f5525ab4583cp-1,
    -0x1.ffeffffffffccp995

  },
  { // Entry 476
    0x1.a0af44a45c057p-8,
    0x1.ffeffffffffffp77

  },
  { // Entry 477
    0x1.a0af44a45c057p-8,
    -0x1.ffeffffffffffp77

  },
  { // Entry 478
    -0x1.df7546c31bf8dp-1,
    0x1.ffeffffffffffp122

  },
  { // Entry 479
    -0x1.df7546c31bf8dp-1,
    -0x1.ffeffffffffffp122

  },
  { // Entry 480
    -0x1.825a7bea27d5bp-1,
    0x1.ffeffffffffffp179

  },
  { // Entry 481
    -0x1.825a7bea27d5bp-1,
    -0x1.ffeffffffffffp179

  },
  { // Entry 482
    -0x1.1be2ab2078d54p-1,
    0x1.ffeffffffffffp238

  },
  { // Entry 483
    -0x1.1be2ab2078d54p-1,
    -0x1.ffeffffffffffp238

  },
  { // Entry 484
    -0x1.a4cc5f838f529p-7,
    0x1.fff0000002511p492

  },
  { // Entry 485
    -0x1.a4cc5f838f529p-7,
    -0x1.fff0000002511p492

  },
  { // Entry 486
    0x1.f16437d6119f9p-10,
    0x1.fff1fffffffffp41

  },
  { // Entry 487
    0x1.f16437d6119f9p-10,
    -0x1.fff1fffffffffp41

  },
  { // Entry 488
    0x1.898324c2f1cfcp-11,
    0x1.ffffc7fffffffp45

  },
  { // Entry 489
    0x1.898324c2f1cfcp-11,
    -0x1.ffffc7fffffffp45

  },
  { // Entry 490
    0x1.f0154c00688f8p-1,
    0x1.ffffdf1ffffffp-3

  },
  { // Entry 491
    0x1.f0154c00688f8p-1,
    -0x1.ffffdf1ffffffp-3

  },
  { // Entry 492
    0x1.ffc00157126a8p-1,
    0x1.fffff8fffffffp-6

  },
  { // Entry 493
    0x1.ffc00157126a8p-1,
    -0x1.fffff8fffffffp-6

  },
  { // Entry 494
    -0x1.e0d9f0f38c73fp-2,
    0x1.fffffbfffffffp968

  },
  { // Entry 495
    -0x1.e0d9f0f38c73fp-2,
    -0x1.fffffbfffffffp968

  },
  { // Entry 496
    0x1.fff4699dd560bp-1,
    0x1.fffffcfffffffp40

  },
  { // Entry 497
    0x1.fff4699dd560bp-1,
    -0x1.fffffcfffffffp40

  },
  { // Entry 498
    0x1.ff0015559f228p-1,
    0x1.ffffff000004p-5

  },
  { // Entry 499
    0x1.ff0015559f228p-1,
    -0x1.ffffff000004p-5

  },
  { // Entry 500
    -0x1.9c6951cccd39cp-2,
    0x1.ffffff8p119

  },
  { // Entry 501
    -0x1.9c6951cccd39cp-2,
    -0x1.ffffff8p119

  },
  { // Entry 502
    -0x1.f2c2263590035p-1,
    0x1.ffffff8p192

  },
  { // Entry 503
    -0x1.f2c2263590035p-1,
    -0x1.ffffff8p192

  },
  { // Entry 504
    0x1.c7884d6cfb551p-1,
    0x1.ffffff8p543

  },
  { // Entry 505
    0x1.c7884d6cfb551p-1,
    -0x1.ffffff8p543

  },
  { // Entry 506
    0x1.e66c79e776a1fp-2,
    0x1.ffffffc3fffffp500

  },
  { // Entry 507
    0x1.e66c79e776a1fp-2,
    -0x1.ffffffc3fffffp500

  },
  { // Entry 508
    0x1.c7c9a9c57c0b2p-3,
    0x1.ffffffe1fffffp700

  },
  { // Entry 509
    0x1.c7c9a9c57c0b2p-3,
    -0x1.ffffffe1fffffp700

  },
  { // Entry 510
    0x1.7bb28daf5f9aep-1,
    0x1.ffffffff0f0ffp400

  },
  { // Entry 511
    0x1.7bb28daf5f9aep-1,
    -0x1.ffffffff0f0ffp400

  },
  { // Entry 512
    0x1.fc015527d8bb3p-1,
    0x1.ffffffff3ffffp-4

  },
  { // Entry 513
    0x1.fc015527d8bb3p-1,
    -0x1.ffffffff3ffffp-4

  },
  { // Entry 514
    -0x1.ea5257eb66e3cp-1,
    0x1.ffffffff8ffffp3

  },
  { // Entry 515
    -0x1.ea5257eb66e3cp-1,
    -0x1.ffffffff8ffffp3

  },
  { // Entry 516
    -0x1.4eaa606dbef97p-1,
    0x1.fffffffffbcffp1

  },
  { // Entry 517
    -0x1.4eaa606dbef97p-1,
    -0x1.fffffffffbcffp1

  },
  { // Entry 518
    -0x1.fc9cd6b5f0095p-1,
    0x1.fffffffffe0b5p720

  },
  { // Entry 519
    -0x1.fc9cd6b5f0095p-1,
    -0x1.fffffffffe0b5p720

  },
  { // Entry 520
    0x1.e96ac045dd139p-3,
    0x1.fffffffffe7ffp41

  },
  { // Entry 521
    0x1.e96ac045dd139p-3,
    -0x1.fffffffffe7ffp41

  },
  { // Entry 522
    -0x1.fcaf39cfb94d5p-1,
    0x1.fffffffffee09p720

  },
  { // Entry 523
    -0x1.fcaf39cfb94d5p-1,
    -0x1.fffffffffee09p720

  },
  { // Entry 524
    0x1.8432232a6d1dap-1,
    0x1.ffffffffffdffp40

  },
  { // Entry 525
    0x1.8432232a6d1dap-1,
    -0x1.ffffffffffdffp40

  },
  { // Entry 526
    0x1.9e375143139dap-6,
    0x1.ffffffffffeffp41

  },
  { // Entry 527
    0x1.9e375143139dap-6,
    -0x1.ffffffffffeffp41

  },
  { // Entry 528
    0x1.fffc000155552p-1,
    0x1.fffffffffff4ap-8

  },
  { // Entry 529
    0x1.fffc000155552p-1,
    -0x1.fffffffffff4ap-8

  },
  { // Entry 530
    0x1.463a895c4ea5dp-1,
    0x1.fffffffffff78p920

  },
  { // Entry 531
    0x1.463a895c4ea5dp-1,
    -0x1.fffffffffff78p920

  },
  { // Entry 532
    0x1.3c1a48635cf38p-1,
    0x1.fffffffffffd5p995

  },
  { // Entry 533
    0x1.3c1a48635cf38p-1,
    -0x1.fffffffffffd5p995

  },
  { // Entry 534
    0x1.91c4e0708bd48p-1,
    0x1.fffffffffffe8p720

  },
  { // Entry 535
    0x1.91c4e0708bd48p-1,
    -0x1.fffffffffffe8p720

  },
  { // Entry 536
    -0x1.3e15cb849b5eap-1,
    0x1.fffffffffffebp920

  },
  { // Entry 537
    -0x1.3e15cb849b5eap-1,
    -0x1.fffffffffffebp920

  },
  { // Entry 538
    -0x1.816808349b80ep-1,
    0x1.ffffffffffff1p245

  },
  { // Entry 539
    -0x1.816808349b80ep-1,
    -0x1.ffffffffffff1p245

  },
  { // Entry 540
    0x1.4699c814c5f07p-1,
    0x1.ffffffffffff4p845

  },
  { // Entry 541
    0x1.4699c814c5f07p-1,
    -0x1.ffffffffffff4p845

  },
  { // Entry 542
    -0x1.815e92b7a2a01p-1,
    0x1.ffffffffffff4p1020

  },
  { // Entry 543
    -0x1.815e92b7a2a01p-1,
    -0x1.ffffffffffff4p1020

  },
  { // Entry 544
    -0x1.3e8d028153202p-10,
    0x1.ffffffffffffcp45

  },
  { // Entry 545
    -0x1.3e8d028153202p-10,
    -0x1.ffffffffffffcp45

  },
  { // Entry 546
    0x1.7d6765714c786p-1,
    0x1.ffffffffffffep105

  },
  { // Entry 547
    0x1.7d6765714c786p-1,
    -0x1.ffffffffffffep105

  },
  { // Entry 548
    -0x1.f869fb14d2569p-3,
    0x1.ffffffffffffep480

  },
  { // Entry 549
    -0x1.f869fb14d2569p-3,
    -0x1.ffffffffffffep480

  },
  { // Entry 550
    -0x1.80a75b369d3c4p-1,
    0x1.ffffffffffffep970

  },
  { // Entry 551
    -0x1.80a75b369d3c4p-1,
    -0x1.ffffffffffffep970

  },
  { // Entry 552
    -0x1.9dba69e853bd8p-4,
    0x1.0000000000001p42

  },
  { // Entry 553
    -0x1.9dba69e853bd8p-4,
    -0x1.0000000000001p42

  },
  { // Entry 554
    0x1.0p0,
    -0x0.0000000000001p-1022

  },
  { // Entry 555
    0x1.0p0,
    0x0.0000000000001p-1022

  },
  { // Entry 556
    0x1.0p0,
    -0x0.0p0

  },
  { // Entry 557
    0x1.0p0,
    0x0.0000000000001p-1022

  },
  { // Entry 558
    0x1.0p0,
    -0x0.0000000000001p-1022

  },
  { // Entry 559
    0x1.0p0,
    -0x1.0000000000001p-1022

  },
  { // Entry 560
    0x1.0p0,
    0x1.0000000000001p-1022

  },
  { // Entry 561
    0x1.0p0,
    -0x1.0p-1022

  },
  { // Entry 562
    0x1.0p0,
    0x1.0p-1022

  },
  { // Entry 563
    0x1.0p0,
    -0x0.fffffffffffffp-1022

  },
  { // Entry 564
    0x1.0p0,
    0x0.fffffffffffffp-1022

  },
  { // Entry 565
    0x1.0p0,
    0x0.fffffffffffffp-1022

  },
  { // Entry 566
    0x1.0p0,
    -0x0.fffffffffffffp-1022

  },
  { // Entry 567
    0x1.0p0,
    0x1.0p-1022

  },
  { // Entry 568
    0x1.0p0,
    -0x1.0p-1022

  },
  { // Entry 569
    0x1.0p0,
    0x1.0000000000001p-1022

  },
  { // Entry 570
    0x1.0p0,
    -0x1.0000000000001p-1022

  },
  { // Entry 571
    0x1.ffffff5c28f5dp-1,
    0x1.999999999999ap-13

  },
  { // Entry 572
    0x1.ffffff5c28f5dp-1,
    -0x1.999999999999ap-13

  },
  { // Entry 573
    0x1.fffffd70a3d79p-1,
    0x1.999999999999ap-12

  },
  { // Entry 574
    0x1.fffffd70a3d79p-1,
    -0x1.999999999999ap-12

  },
  { // Entry 575
    0x1.fffffa3d70a6ap-1,
    0x1.3333333333334p-11

  },
  { // Entry 576
    0x1.fffffa3d70a6ap-1,
    -0x1.3333333333334p-11

  },
  { // Entry 577
    0x1.fffff5c28f64ep-1,
    0x1.999999999999ap-11

  },
  { // Entry 578
    0x1.fffff5c28f64ep-1,
    -0x1.999999999999ap-11

  },
  { // Entry 579
    0x1.fffff00000155p-1,
    0x1.0p-10

  },
  { // Entry 580
    0x1.fffff00000155p-1,
    -0x1.0p-10

  },
  { // Entry 581
    0x1.ffffe8f5c2bbap-1,
    0x1.3333333333333p-10

  },
  { // Entry 582
    0x1.ffffe8f5c2bbap-1,
    -0x1.3333333333333p-10

  },
  { // Entry 583
    0x1.ffffe0a3d75c3p-1,
    0x1.6666666666666p-10

  },
  { // Entry 584
    0x1.ffffe0a3d75c3p-1,
    -0x1.6666666666666p-10

  },
  { // Entry 585
    0x1.ffffd70a3dfc7p-1,
    0x1.9999999999999p-10

  },
  { // Entry 586
    0x1.ffffd70a3dfc7p-1,
    -0x1.9999999999999p-10

  },
  { // Entry 587
    0x1.ffffcc28f6a28p-1,
    0x1.cccccccccccccp-10

  },
  { // Entry 588
    0x1.ffffcc28f6a28p-1,
    -0x1.cccccccccccccp-10

  },
  { // Entry 589
    0x1.fffbcc2a6e87p-1,
    0x1.0666666666666p-7

  },
  { // Entry 590
    0x1.fffbcc2a6e87p-1,
    -0x1.0666666666666p-7

  },
  { // Entry 591
    0x1.fff30a4b6fcc1p-1,
    0x1.cccccccccccccp-7

  },
  { // Entry 592
    0x1.fff30a4b6fcc1p-1,
    -0x1.cccccccccccccp-7

  },
  { // Entry 593
    0x1.ffe57a780f38cp-1,
    0x1.4999999999999p-6

  },
  { // Entry 594
    0x1.ffe57a780f38cp-1,
    -0x1.4999999999999p-6

  },
  { // Entry 595
    0x1.ffd31cd0e1d63p-1,
    0x1.accccccccccccp-6

  },
  { // Entry 596
    0x1.ffd31cd0e1d63p-1,
    -0x1.accccccccccccp-6

  },
  { // Entry 597
    0x1.ffbbf18207543p-1,
    0x1.08p-5

  },
  { // Entry 598
    0x1.ffbbf18207543p-1,
    -0x1.08p-5

  },
  { // Entry 599
    0x1.ff9ff8c3299f5p-1,
    0x1.399999999999ap-5

  },
  { // Entry 600
    0x1.ff9ff8c3299f5p-1,
    -0x1.399999999999ap-5

  },
  { // Entry 601
    0x1.ff7f32d77c5b2p-1,
    0x1.6b33333333334p-5

  },
  { // Entry 602
    0x1.ff7f32d77c5b2p-1,
    -0x1.6b33333333334p-5

  },
  { // Entry 603
    0x1.ff59a00dbc409p-1,
    0x1.9cccccccccccep-5

  },
  { // Entry 604
    0x1.ff59a00dbc409p-1,
    -0x1.9cccccccccccep-5

  },
  { // Entry 605
    0x1.ff2f40c02e60fp-1,
    0x1.ce66666666666p-5

  },
  { // Entry 606
    0x1.ff2f40c02e60fp-1,
    -0x1.ce66666666666p-5

  },
  { // Entry 607
    0x1.8ca46c7d8975ep-1,
    0x1.5e7fc4369bdadp-1

  },
  { // Entry 608
    0x1.8ca46c7d8975ep-1,
    -0x1.5e7fc4369bdadp-1

  },
  { // Entry 609
    0x1.0b5d3802fc799p-2,
    0x1.4e7fc4369bdadp0

  },
  { // Entry 610
    0x1.0b5d3802fc799p-2,
    -0x1.4e7fc4369bdadp0

  },
  { // Entry 611
    -0x1.66b96f53323afp-2,
    0x1.edbfa651e9c84p0

  },
  { // Entry 612
    -0x1.66b96f53323afp-2,
    -0x1.edbfa651e9c84p0

  },
  { // Entry 613
    -0x1.a93554888c33p-1,
    0x1.467fc4369bdadp1

  },
  { // Entry 614
    -0x1.a93554888c33p-1,
    -0x1.467fc4369bdadp1

  },
  { // Entry 615
    -0x1.ffc00155527d3p-1,
    0x1.961fb54442d18p1

  },
  { // Entry 616
    -0x1.ffc00155527d3p-1,
    -0x1.961fb54442d18p1

  },
  { // Entry 617
    -0x1.96907c5c7c25cp-1,
    0x1.e5bfa651e9c83p1

  },
  { // Entry 618
    -0x1.96907c5c7c25cp-1,
    -0x1.e5bfa651e9c83p1

  },
  { // Entry 619
    -0x1.2a1e5a50f948dp-2,
    0x1.1aafcbafc85f7p2

  },
  { // Entry 620
    -0x1.2a1e5a50f948dp-2,
    -0x1.1aafcbafc85f7p2

  },
  { // Entry 621
    0x1.4894f695dc56cp-2,
    0x1.427fc4369bdadp2

  },
  { // Entry 622
    0x1.4894f695dc56cp-2,
    -0x1.427fc4369bdadp2

  },
  { // Entry 623
    0x1.a016ea3a692cep-1,
    0x1.6a4fbcbd6f562p2

  },
  { // Entry 624
    0x1.a016ea3a692cep-1,
    -0x1.6a4fbcbd6f562p2

  },
  { // Entry 625
    0x1.a30a69f5537ecp-1,
    0x1.6af2eff0a2896p2

  },
  { // Entry 626
    0x1.a30a69f5537ecp-1,
    -0x1.6af2eff0a2896p2

  },
  { // Entry 627
    0x1.5bd62e8b04ad6p-2,
    0x1.43c62a9d02414p2

  },
  { // Entry 628
    0x1.5bd62e8b04ad6p-2,
    -0x1.43c62a9d02414p2

  },
  { // Entry 629
    -0x1.0cb71f671e634p-2,
    0x1.1c99654961f92p2

  },
  { // Entry 630
    -0x1.0cb71f671e634p-2,
    -0x1.1c99654961f92p2

  },
  { // Entry 631
    -0x1.89d86aa8521c1p-1,
    0x1.ead93feb8361fp1

  },
  { // Entry 632
    -0x1.89d86aa8521c1p-1,
    -0x1.ead93feb8361fp1

  },
  { // Entry 633
    -0x1.fe51ac554a16bp-1,
    0x1.9c7fb54442d1ap1

  },
  { // Entry 634
    -0x1.fe51ac554a16bp-1,
    -0x1.9c7fb54442d1ap1

  },
  { // Entry 635
    -0x1.b97c04d08bc5dp-1,
    0x1.4e262a9d02415p1

  },
  { // Entry 636
    -0x1.b97c04d08bc5dp-1,
    -0x1.4e262a9d02415p1

  },
  { // Entry 637
    -0x1.a8ac8a3e58f6dp-2,
    0x1.ff993feb8362p0

  },
  { // Entry 638
    -0x1.a8ac8a3e58f6dp-2,
    -0x1.ff993feb8362p0

  },
  { // Entry 639
    0x1.77a8b9b3d254bp-3,
    0x1.62e62a9d02416p0

  },
  { // Entry 640
    0x1.77a8b9b3d254bp-3,
    -0x1.62e62a9d02416p0

  },
  { // Entry 641
    0x1.6e1061205dd79p-1,
    0x1.8c662a9d02419p-1

  },
  { // Entry 642
    0x1.6e1061205dd79p-1,
    -0x1.8c662a9d02419p-1

  },
  { // Entry 643
    -0x1.682f3cc3c7a09p-4,
    -0x1.a8aa1d11c44ffp0

  },
  { // Entry 644
    -0x1.682f3cc3c7a09p-4,
    0x1.a8aa1d11c44ffp0

  },
  { // Entry 645
    -0x1.e6669a270c36dp-7,
    -0x1.95ec8b9e03d54p0

  },
  { // Entry 646
    -0x1.e6669a270c36dp-7,
    0x1.95ec8b9e03d54p0

  },
  { // Entry 647
    0x1.ddd1ec25e209fp-5,
    -0x1.832efa2a435a9p0

  },
  { // Entry 648
    0x1.ddd1ec25e209fp-5,
    0x1.832efa2a435a9p0

  },
  { // Entry 649
    0x1.0cab9115640dap-3,
    -0x1.707168b682dfep0

  },
  { // Entry 650
    0x1.0cab9115640dap-3,
    0x1.707168b682dfep0

  },
  { // Entry 651
    0x1.a0723a95492eep-3,
    -0x1.5db3d742c2653p0

  },
  { // Entry 652
    0x1.a0723a95492eep-3,
    0x1.5db3d742c2653p0

  },
  { // Entry 653
    0x1.18fee96a1a586p-2,
    -0x1.4af645cf01ea8p0

  },
  { // Entry 654
    0x1.18fee96a1a586p-2,
    0x1.4af645cf01ea8p0

  },
  { // Entry 655
    0x1.6043621b13be3p-2,
    -0x1.3838b45b416fdp0

  },
  { // Entry 656
    0x1.6043621b13be3p-2,
    0x1.3838b45b416fdp0

  },
  { // Entry 657
    0x1.a5a4ccf40d9dap-2,
    -0x1.257b22e780f52p0

  },
  { // Entry 658
    0x1.a5a4ccf40d9dap-2,
    0x1.257b22e780f52p0

  },
  { // Entry 659
    0x1.e8c405f36f85cp-2,
    -0x1.12bd9173c07abp0

  },
  { // Entry 660
    0x1.e8c405f36f85cp-2,
    0x1.12bd9173c07abp0

  },
  { // Entry 661
    0x1.26976a6c4e0f8p-1,
    -0x1.ea5c3ed5b385p-1

  },
  { // Entry 662
    0x1.26976a6c4e0f8p-1,
    0x1.ea5c3ed5b385p-1

  },
  { // Entry 663
    0x1.3805a1882009fp-1,
    -0x1.d4b87dab670ap-1

  },
  { // Entry 664
    0x1.3805a1882009fp-1,
    0x1.d4b87dab670ap-1

  },
  { // Entry 665
    0x1.48e52e0a65bcbp-1,
    -0x1.bf14bc811a8fp-1

  },
  { // Entry 666
    0x1.48e52e0a65bcbp-1,
    0x1.bf14bc811a8fp-1

  },
  { // Entry 667
    0x1.592e58ea0a9efp-1,
    -0x1.a970fb56ce14p-1

  },
  { // Entry 668
    0x1.592e58ea0a9efp-1,
    0x1.a970fb56ce14p-1

  },
  { // Entry 669
    0x1.68d9afe052d1fp-1,
    -0x1.93cd3a2c8199p-1

  },
  { // Entry 670
    0x1.68d9afe052d1fp-1,
    0x1.93cd3a2c8199p-1

  },
  { // Entry 671
    0x1.77e008d0775e7p-1,
    -0x1.7e297902351ep-1

  },
  { // Entry 672
    0x1.77e008d0775e7p-1,
    0x1.7e297902351ep-1

  },
  { // Entry 673
    0x1.863a850e438fep-1,
    -0x1.6885b7d7e8a3p-1

  },
  { // Entry 674
    0x1.863a850e438fep-1,
    0x1.6885b7d7e8a3p-1

  },
  { // Entry 675
    0x1.93e2948233fcep-1,
    -0x1.52e1f6ad9c28p-1

  },
  { // Entry 676
    0x1.93e2948233fcep-1,
    0x1.52e1f6ad9c28p-1

  },
  { // Entry 677
    0x1.a0d1f8a9a791dp-1,
    -0x1.3d3e35834fadp-1

  },
  { // Entry 678
    0x1.a0d1f8a9a791dp-1,
    0x1.3d3e35834fadp-1

  },
  { // Entry 679
    0x1.bc6bd861e13dep-1,
    -0x1.0a0b02501c799p-1

  },
  { // Entry 680
    0x1.bc6bd861e13dep-1,
    0x1.0a0b02501c799p-1

  },
  { // Entry 681
    0x1.ca59c6fa3d9cep-1,
    -0x1.d8f7208e6b82cp-2

  },
  { // Entry 682
    0x1.ca59c6fa3d9cep-1,
    0x1.d8f7208e6b82cp-2

  },
  { // Entry 683
    0x1.d6c0b125791dp-1,
    -0x1.9dd83c7c9e126p-2

  },
  { // Entry 684
    0x1.d6c0b125791dp-1,
    0x1.9dd83c7c9e126p-2

  },
  { // Entry 685
    0x1.e196026182986p-1,
    -0x1.62b9586ad0a2p-2

  },
  { // Entry 686
    0x1.e196026182986p-1,
    0x1.62b9586ad0a2p-2

  },
  { // Entry 687
    0x1.ead07cc635696p-1,
    -0x1.279a74590331ap-2

  },
  { // Entry 688
    0x1.ead07cc635696p-1,
    0x1.279a74590331ap-2

  },
  { // Entry 689
    0x1.f26840e7b2189p-1,
    -0x1.d8f7208e6b829p-3

  },
  { // Entry 690
    0x1.f26840e7b2189p-1,
    0x1.d8f7208e6b829p-3

  },
  { // Entry 691
    0x1.f856d48db797ep-1,
    -0x1.62b9586ad0a1ep-3

  },
  { // Entry 692
    0x1.f856d48db797ep-1,
    0x1.62b9586ad0a1ep-3

  },
  { // Entry 693
    0x1.fc97283a42479p-1,
    -0x1.d8f7208e6b826p-4

  },
  { // Entry 694
    0x1.fc97283a42479p-1,
    0x1.d8f7208e6b826p-4

  },
  { // Entry 695
    0x1.ff259b7ab9f5p-1,
    -0x1.d8f7208e6b82dp-5

  },
  { // Entry 696
    0x1.ff259b7ab9f5p-1,
    0x1.d8f7208e6b82dp-5

  },
  { // Entry 697
    0x1.ff259b7ab9f5p-1,
    0x1.d8f7208e6b82dp-5

  },
  { // Entry 698
    0x1.ff259b7ab9f5p-1,
    -0x1.d8f7208e6b82dp-5

  },
  { // Entry 699
    0x1.fc97283a42479p-1,
    0x1.d8f7208e6b82dp-4

  },
  { // Entry 700
    0x1.fc97283a42479p-1,
    -0x1.d8f7208e6b82dp-4

  },
  { // Entry 701
    0x1.f856d48db797ep-1,
    0x1.62b9586ad0a22p-3

  },
  { // Entry 702
    0x1.f856d48db797ep-1,
    -0x1.62b9586ad0a22p-3

  },
  { // Entry 703
    0x1.f26840e7b2189p-1,
    0x1.d8f7208e6b82dp-3

  },
  { // Entry 704
    0x1.f26840e7b2189p-1,
    -0x1.d8f7208e6b82dp-3

  },
  { // Entry 705
    0x1.ead07cc635696p-1,
    0x1.279a74590331cp-2

  },
  { // Entry 706
    0x1.ead07cc635696p-1,
    -0x1.279a74590331cp-2

  },
  { // Entry 707
    0x1.e196026182985p-1,
    0x1.62b9586ad0a22p-2

  },
  { // Entry 708
    0x1.e196026182985p-1,
    -0x1.62b9586ad0a22p-2

  },
  { // Entry 709
    0x1.d6c0b125791dp-1,
    0x1.9dd83c7c9e128p-2

  },
  { // Entry 710
    0x1.d6c0b125791dp-1,
    -0x1.9dd83c7c9e128p-2

  },
  { // Entry 711
    0x1.ca59c6fa3d9cep-1,
    0x1.d8f7208e6b82ep-2

  },
  { // Entry 712
    0x1.ca59c6fa3d9cep-1,
    -0x1.d8f7208e6b82ep-2

  },
  { // Entry 713
    0x1.bc6bd861e13dep-1,
    0x1.0a0b02501c799p-1

  },
  { // Entry 714
    0x1.bc6bd861e13dep-1,
    -0x1.0a0b02501c799p-1

  },
  { // Entry 715
    0x1.a0d1f8a9a792p-1,
    0x1.3d3e35834faccp-1

  },
  { // Entry 716
    0x1.a0d1f8a9a792p-1,
    -0x1.3d3e35834faccp-1

  },
  { // Entry 717
    0x1.93e2948233fd1p-1,
    0x1.52e1f6ad9c27cp-1

  },
  { // Entry 718
    0x1.93e2948233fd1p-1,
    -0x1.52e1f6ad9c27cp-1

  },
  { // Entry 719
    0x1.863a850e439p-1,
    0x1.6885b7d7e8a2cp-1

  },
  { // Entry 720
    0x1.863a850e439p-1,
    -0x1.6885b7d7e8a2cp-1

  },
  { // Entry 721
    0x1.77e008d0775eap-1,
    0x1.7e297902351dcp-1

  },
  { // Entry 722
    0x1.77e008d0775eap-1,
    -0x1.7e297902351dcp-1

  },
  { // Entry 723
    0x1.68d9afe052d22p-1,
    0x1.93cd3a2c8198cp-1

  },
  { // Entry 724
    0x1.68d9afe052d22p-1,
    -0x1.93cd3a2c8198cp-1

  },
  { // Entry 725
    0x1.592e58ea0a9f2p-1,
    0x1.a970fb56ce13cp-1

  },
  { // Entry 726
    0x1.592e58ea0a9f2p-1,
    -0x1.a970fb56ce13cp-1

  },
  { // Entry 727
    0x1.48e52e0a65bcep-1,
    0x1.bf14bc811a8ecp-1

  },
  { // Entry 728
    0x1.48e52e0a65bcep-1,
    -0x1.bf14bc811a8ecp-1

  },
  { // Entry 729
    0x1.3805a188200a2p-1,
    0x1.d4b87dab6709cp-1

  },
  { // Entry 730
    0x1.3805a188200a2p-1,
    -0x1.d4b87dab6709cp-1

  },
  { // Entry 731
    0x1.26976a6c4e0fcp-1,
    0x1.ea5c3ed5b384cp-1

  },
  { // Entry 732
    0x1.26976a6c4e0fcp-1,
    -0x1.ea5c3ed5b384cp-1

  },
  { // Entry 733
    0x1.e8c405f36f85cp-2,
    0x1.12bd9173c07abp0

  },
  { // Entry 734
    0x1.e8c405f36f85cp-2,
    -0x1.12bd9173c07abp0

  },
  { // Entry 735
    0x1.a5a4ccf40d9cbp-2,
    0x1.257b22e780f56p0

  },
  { // Entry 736
    0x1.a5a4ccf40d9cbp-2,
    -0x1.257b22e780f56p0

  },
  { // Entry 737
    0x1.6043621b13bd4p-2,
    0x1.3838b45b41701p0

  },
  { // Entry 738
    0x1.6043621b13bd4p-2,
    -0x1.3838b45b41701p0

  },
  { // Entry 739
    0x1.18fee96a1a576p-2,
    0x1.4af645cf01eacp0

  },
  { // Entry 740
    0x1.18fee96a1a576p-2,
    -0x1.4af645cf01eacp0

  },
  { // Entry 741
    0x1.a0723a95492cfp-3,
    0x1.5db3d742c2657p0

  },
  { // Entry 742
    0x1.a0723a95492cfp-3,
    -0x1.5db3d742c2657p0

  },
  { // Entry 743
    0x1.0cab9115640bap-3,
    0x1.707168b682e02p0

  },
  { // Entry 744
    0x1.0cab9115640bap-3,
    -0x1.707168b682e02p0

  },
  { // Entry 745
    0x1.ddd1ec25e201fp-5,
    0x1.832efa2a435adp0

  },
  { // Entry 746
    0x1.ddd1ec25e201fp-5,
    -0x1.832efa2a435adp0

  },
  { // Entry 747
    -0x1.e6669a270c56dp-7,
    0x1.95ec8b9e03d58p0

  },
  { // Entry 748
    -0x1.e6669a270c56dp-7,
    -0x1.95ec8b9e03d58p0

  },
  { // Entry 749
    -0x1.682f3cc3c7a09p-4,
    0x1.a8aa1d11c44ffp0

  },
  { // Entry 750
    -0x1.682f3cc3c7a09p-4,
    -0x1.a8aa1d11c44ffp0

  },
  { // Entry 751
    0x1.0cb3469a29ea6p-1,
    0x1.04aff6d330942p0

  },
  { // Entry 752
    0x1.0cb3469a29ea6p-1,
    -0x1.04aff6d330942p0

  },
  { // Entry 753
    0x1.0cb228fa7f811p-1,
    0x1.04b09e98dcdb4p0

  },
  { // Entry 754
    0x1.0cb228fa7f811p-1,
    -0x1.04b09e98dcdb4p0

  },
  { // Entry 755
    0x1.0cb10b5a61b06p-1,
    0x1.04b1465e89226p0

  },
  { // Entry 756
    0x1.0cb10b5a61b06p-1,
    -0x1.04b1465e89226p0

  },
  { // Entry 757
    0x1.0cafedb9d078bp-1,
    0x1.04b1ee2435698p0

  },
  { // Entry 758
    0x1.0cafedb9d078bp-1,
    -0x1.04b1ee2435698p0

  },
  { // Entry 759
    0x1.0caed018cbda8p-1,
    0x1.04b295e9e1b0ap0

  },
  { // Entry 760
    0x1.0caed018cbda8p-1,
    -0x1.04b295e9e1b0ap0

  },
  { // Entry 761
    0x1.0cadb27753d65p-1,
    0x1.04b33daf8df7cp0

  },
  { // Entry 762
    0x1.0cadb27753d65p-1,
    -0x1.04b33daf8df7cp0

  },
  { // Entry 763
    0x1.0cac94d5686cbp-1,
    0x1.04b3e5753a3eep0

  },
  { // Entry 764
    0x1.0cac94d5686cbp-1,
    -0x1.04b3e5753a3eep0

  },
  { // Entry 765
    0x1.0cab7733099dfp-1,
    0x1.04b48d3ae686p0

  },
  { // Entry 766
    0x1.0cab7733099dfp-1,
    -0x1.04b48d3ae686p0

  },
  { // Entry 767
    0x1.0caa5990376bp-1,
    0x1.04b5350092ccfp0

  },
  { // Entry 768
    0x1.0caa5990376bp-1,
    -0x1.04b5350092ccfp0

  },
  { // Entry 769
    0x1.0p0,
    -0x0.0000000000001p-1022

  },
  { // Entry 770
    0x1.0p0,
    0x0.0000000000001p-1022

  },
  { // Entry 771
    0x1.0p0,
    -0x0.0p0

  },
  { // Entry 772
    0x1.0p0,
    0x0.0000000000001p-1022

  },
  { // Entry 773
    0x1.0p0,
    -0x0.0000000000001p-1022

  },
  { // Entry 774
    0x1.ad02c771c35edp-1,
    0x1.279a74590331bp-1

  },
  { // Entry 775
    0x1.ad02c771c35edp-1,
    -0x1.279a74590331bp-1

  },
  { // Entry 776
    0x1.ad02c771c35edp-1,
    0x1.279a74590331cp-1

  },
  { // Entry 777
    0x1.ad02c771c35edp-1,
    -0x1.279a74590331cp-1

  },
  { // Entry 778
    0x1.ad02c771c35ecp-1,
    0x1.279a74590331dp-1

  },
  { // Entry 779
    0x1.ad02c771c35ecp-1,
    -0x1.279a74590331dp-1

  },
  { // Entry 780
    -0x1.48d1ddd2b2b4p-3,
    0x1.bb67ae8584ca9p0

  },
  { // Entry 781
    -0x1.48d1ddd2b2b4p-3,
    -0x1.bb67ae8584ca9p0

  },
  { // Entry 782
    -0x1.48d1ddd2b2b47p-3,
    0x1.bb67ae8584caap0

  },
  { // Entry 783
    -0x1.48d1ddd2b2b47p-3,
    -0x1.bb67ae8584caap0

  },
  { // Entry 784
    -0x1.48d1ddd2b2b4fp-3,
    0x1.bb67ae8584cabp0

  },
  { // Entry 785
    -0x1.48d1ddd2b2b4fp-3,
    -0x1.bb67ae8584cabp0

  },
  { // Entry 786
    0x1.cfc6cfa52adap-1,
    0x1.bffffffffffffp-2

  },
  { // Entry 787
    0x1.cfc6cfa52adap-1,
    -0x1.bffffffffffffp-2

  },
  { // Entry 788
    0x1.cfc6cfa52ad9fp-1,
    0x1.cp-2

  },
  { // Entry 789
    0x1.cfc6cfa52ad9fp-1,
    -0x1.cp-2

  },
  { // Entry 790
    0x1.cfc6cfa52ad9fp-1,
    0x1.c000000000001p-2

  },
  { // Entry 791
    0x1.cfc6cfa52ad9fp-1,
    -0x1.c000000000001p-2

  },
  { // Entry 792
    0x1.8bb105a5dc901p-1,
    0x1.5ffffffffffffp-1

  },
  { // Entry 793
    0x1.8bb105a5dc901p-1,
    -0x1.5ffffffffffffp-1

  },
  { // Entry 794
    0x1.8bb105a5dc9p-1,
    0x1.6p-1

  },
  { // Entry 795
    0x1.8bb105a5dc9p-1,
    -0x1.6p-1

  },
  { // Entry 796
    0x1.8bb105a5dc9p-1,
    0x1.6000000000001p-1

  },
  { // Entry 797
    0x1.8bb105a5dc9p-1,
    -0x1.6000000000001p-1

  },
  { // Entry 798
    0x1.7ef4842f0bcd1p-2,
    0x1.2ffffffffffffp0

  },
  { // Entry 799
    0x1.7ef4842f0bcd1p-2,
    -0x1.2ffffffffffffp0

  },
  { // Entry 800
    0x1.7ef4842f0bccdp-2,
    0x1.3p0

  },
  { // Entry 801
    0x1.7ef4842f0bccdp-2,
    -0x1.3p0

  },
  { // Entry 802
    0x1.7ef4842f0bccap-2,
    0x1.3000000000001p0

  },
  { // Entry 803
    0x1.7ef4842f0bccap-2,
    -0x1.3000000000001p0

  },
  { // Entry 804
    -0x1.863efa361dc22p-1,
    0x1.37fffffffffffp1

  },
  { // Entry 805
    -0x1.863efa361dc22p-1,
    -0x1.37fffffffffffp1

  },
  { // Entry 806
    -0x1.863efa361dc25p-1,
    0x1.38p1

  },
  { // Entry 807
    -0x1.863efa361dc25p-1,
    -0x1.38p1

  },
  { // Entry 808
    -0x1.863efa361dc28p-1,
    0x1.3800000000001p1

  },
  { // Entry 809
    -0x1.863efa361dc28p-1,
    -0x1.3800000000001p1

  },
  { // Entry 810
    0x1.fef2b2d21cf6cp-1,
    0x1.069c8b46b3792p-4

  },
  { // Entry 811
    0x1.fef2b2d21cf6cp-1,
    -0x1.069c8b46b3792p-4

  },
  { // Entry 812
    0x1.fbcbe693bd8edp-1,
    0x1.069c8b46b3792p-3

  },
  { // Entry 813
    0x1.fbcbe693bd8edp-1,
    -0x1.069c8b46b3792p-3

  },
  { // Entry 814
    0x1.f68eebfcbb5e8p-1,
    0x1.89ead0ea0d35bp-3

  },
  { // Entry 815
    0x1.f68eebfcbb5e8p-1,
    -0x1.89ead0ea0d35bp-3

  },
  { // Entry 816
    0x1.ef4145b4aedp-1,
    0x1.069c8b46b3792p-2

  },
  { // Entry 817
    0x1.ef4145b4aedp-1,
    -0x1.069c8b46b3792p-2

  },
  { // Entry 818
    0x1.e5eaa286fbbc6p-1,
    0x1.4843ae1860576p-2

  },
  { // Entry 819
    0x1.e5eaa286fbbc6p-1,
    -0x1.4843ae1860576p-2

  },
  { // Entry 820
    0x1.da94d54dd4c08p-1,
    0x1.89ead0ea0d35ap-2

  },
  { // Entry 821
    0x1.da94d54dd4c08p-1,
    -0x1.89ead0ea0d35ap-2

  },
  { // Entry 822
    0x1.cd4bca9cb5c71p-1,
    0x1.cb91f3bbba13ep-2

  },
  { // Entry 823
    0x1.cd4bca9cb5c71p-1,
    -0x1.cb91f3bbba13ep-2

  },
  { // Entry 824
    0x1.be1d7c3534c4p-1,
    0x1.069c8b46b3791p-1

  },
  { // Entry 825
    0x1.be1d7c3534c4p-1,
    -0x1.069c8b46b3791p-1

  },
  { // Entry 826
    0x1.ad19e2535aa96p-1,
    0x1.27701caf89e83p-1

  },
  { // Entry 827
    0x1.ad19e2535aa96p-1,
    -0x1.27701caf89e83p-1

  },
  { // Entry 828
    0x1.9a52e2e0fbcb4p-1,
    0x1.4843ae1860575p-1

  },
  { // Entry 829
    0x1.9a52e2e0fbcb4p-1,
    -0x1.4843ae1860575p-1

  },
  { // Entry 830
    0x1.85dc3ea1bbceap-1,
    0x1.69173f8136c67p-1

  },
  { // Entry 831
    0x1.85dc3ea1bbceap-1,
    -0x1.69173f8136c67p-1

  },
  { // Entry 832
    0x1.6fcb7c6b8b91ap-1,
    0x1.89ead0ea0d359p-1

  },
  { // Entry 833
    0x1.6fcb7c6b8b91ap-1,
    -0x1.89ead0ea0d359p-1

  },
  { // Entry 834
    0x1.5837d2817cf3p-1,
    0x1.aabe6252e3a4bp-1

  },
  { // Entry 835
    0x1.5837d2817cf3p-1,
    -0x1.aabe6252e3a4bp-1

  },
  { // Entry 836
    0x1.3f3a0e28bedd4p-1,
    0x1.cb91f3bbba13dp-1

  },
  { // Entry 837
    0x1.3f3a0e28bedd4p-1,
    -0x1.cb91f3bbba13dp-1

  },
  { // Entry 838
    0x1.24ec799171643p-1,
    0x1.ec6585249082fp-1

  },
  { // Entry 839
    0x1.24ec799171643p-1,
    -0x1.ec6585249082fp-1

  },
  { // Entry 840
    0x1.096ac02ec42c8p-1,
    0x1.069c8b46b3791p0

  },
  { // Entry 841
    0x1.096ac02ec42c8p-1,
    -0x1.069c8b46b3791p0

  },
  { // Entry 842
    0x1.d9a3a336edb76p-2,
    0x1.170653fb1eb0ap0

  },
  { // Entry 843
    0x1.d9a3a336edb76p-2,
    -0x1.170653fb1eb0ap0

  },
  { // Entry 844
    0x1.9e7f8652b4758p-2,
    0x1.27701caf89e83p0

  },
  { // Entry 845
    0x1.9e7f8652b4758p-2,
    -0x1.27701caf89e83p0

  },
  { // Entry 846
    0x1.61a76077aee08p-2,
    0x1.37d9e563f51fcp0

  },
  { // Entry 847
    0x1.61a76077aee08p-2,
    -0x1.37d9e563f51fcp0

  },
  { // Entry 848
    0x1.235b331d8f749p-2,
    0x1.4843ae1860575p0

  },
  { // Entry 849
    0x1.235b331d8f749p-2,
    -0x1.4843ae1860575p0

  },
  { // Entry 850
    0x1.c7b90e3024594p-3,
    0x1.58ad76cccb8eep0

  },
  { // Entry 851
    0x1.c7b90e3024594p-3,
    -0x1.58ad76cccb8eep0

  },
  { // Entry 852
    0x1.46dc4f4ce83dap-3,
    0x1.69173f8136c67p0

  },
  { // Entry 853
    0x1.46dc4f4ce83dap-3,
    -0x1.69173f8136c67p0

  },
  { // Entry 854
    0x1.894f70befbb9ap-4,
    0x1.79810835a1fep0

  },
  { // Entry 855
    0x1.894f70befbb9ap-4,
    -0x1.79810835a1fep0

  },
  { // Entry 856
    0x1.069107ae9333p-5,
    0x1.89ead0ea0d359p0

  },
  { // Entry 857
    0x1.069107ae9333p-5,
    -0x1.89ead0ea0d359p0

  },
  { // Entry 858
    -0x1.069107ae9327ep-5,
    0x1.9a54999e786d2p0

  },
  { // Entry 859
    -0x1.069107ae9327ep-5,
    -0x1.9a54999e786d2p0

  },
  { // Entry 860
    -0x1.894f70befbb41p-4,
    0x1.aabe6252e3a4bp0

  },
  { // Entry 861
    -0x1.894f70befbb41p-4,
    -0x1.aabe6252e3a4bp0

  },
  { // Entry 862
    -0x1.46dc4f4ce83afp-3,
    0x1.bb282b074edc4p0

  },
  { // Entry 863
    -0x1.46dc4f4ce83afp-3,
    -0x1.bb282b074edc4p0

  },
  { // Entry 864
    -0x1.c7b90e3024569p-3,
    0x1.cb91f3bbba13dp0

  },
  { // Entry 865
    -0x1.c7b90e3024569p-3,
    -0x1.cb91f3bbba13dp0

  },
  { // Entry 866
    -0x1.235b331d8f734p-2,
    0x1.dbfbbc70254b6p0

  },
  { // Entry 867
    -0x1.235b331d8f734p-2,
    -0x1.dbfbbc70254b6p0

  },
  { // Entry 868
    -0x1.61a76077aedf3p-2,
    0x1.ec6585249082fp0

  },
  { // Entry 869
    -0x1.61a76077aedf3p-2,
    -0x1.ec6585249082fp0

  },
  { // Entry 870
    -0x1.9e7f8652b4744p-2,
    0x1.fccf4dd8fbba8p0

  },
  { // Entry 871
    -0x1.9e7f8652b4744p-2,
    -0x1.fccf4dd8fbba8p0

  },
  { // Entry 872
    -0x1.d9a3a336edb66p-2,
    0x1.069c8b46b3791p1

  },
  { // Entry 873
    -0x1.d9a3a336edb66p-2,
    -0x1.069c8b46b3791p1

  },
  { // Entry 874
    -0x1.096ac02ec42c2p-1,
    0x1.0ed16fa0e914ep1

  },
  { // Entry 875
    -0x1.096ac02ec42c2p-1,
    -0x1.0ed16fa0e914ep1

  },
  { // Entry 876
    -0x1.24ec79917163ep-1,
    0x1.170653fb1eb0bp1

  },
  { // Entry 877
    -0x1.24ec79917163ep-1,
    -0x1.170653fb1eb0bp1

  },
  { // Entry 878
    -0x1.3f3a0e28bedd1p-1,
    0x1.1f3b3855544c8p1

  },
  { // Entry 879
    -0x1.3f3a0e28bedd1p-1,
    -0x1.1f3b3855544c8p1

  },
  { // Entry 880
    -0x1.5837d2817cf2fp-1,
    0x1.27701caf89e85p1

  },
  { // Entry 881
    -0x1.5837d2817cf2fp-1,
    -0x1.27701caf89e85p1

  },
  { // Entry 882
    -0x1.6fcb7c6b8b91ap-1,
    0x1.2fa50109bf842p1

  },
  { // Entry 883
    -0x1.6fcb7c6b8b91ap-1,
    -0x1.2fa50109bf842p1

  },
  { // Entry 884
    -0x1.85dc3ea1bbcebp-1,
    0x1.37d9e563f51ffp1

  },
  { // Entry 885
    -0x1.85dc3ea1bbcebp-1,
    -0x1.37d9e563f51ffp1

  },
  { // Entry 886
    -0x1.9a52e2e0fbcb6p-1,
    0x1.400ec9be2abbcp1

  },
  { // Entry 887
    -0x1.9a52e2e0fbcb6p-1,
    -0x1.400ec9be2abbcp1

  },
  { // Entry 888
    -0x1.ad19e2535aa9ap-1,
    0x1.4843ae1860579p1

  },
  { // Entry 889
    -0x1.ad19e2535aa9ap-1,
    -0x1.4843ae1860579p1

  },
  { // Entry 890
    -0x1.be1d7c3534c44p-1,
    0x1.5078927295f36p1

  },
  { // Entry 891
    -0x1.be1d7c3534c44p-1,
    -0x1.5078927295f36p1

  },
  { // Entry 892
    -0x1.cd4bca9cb5c76p-1,
    0x1.58ad76cccb8f3p1

  },
  { // Entry 893
    -0x1.cd4bca9cb5c76p-1,
    -0x1.58ad76cccb8f3p1

  },
  { // Entry 894
    -0x1.da94d54dd4c0dp-1,
    0x1.60e25b27012bp1

  },
  { // Entry 895
    -0x1.da94d54dd4c0dp-1,
    -0x1.60e25b27012bp1

  },
  { // Entry 896
    -0x1.e5eaa286fbbcbp-1,
    0x1.69173f8136c6dp1

  },
  { // Entry 897
    -0x1.e5eaa286fbbcbp-1,
    -0x1.69173f8136c6dp1

  },
  { // Entry 898
    -0x1.ef4145b4aed04p-1,
    0x1.714c23db6c62ap1

  },
  { // Entry 899
    -0x1.ef4145b4aed04p-1,
    -0x1.714c23db6c62ap1

  },
  { // Entry 900
    -0x1.f68eebfcbb5ecp-1,
    0x1.79810835a1fe7p1

  },
  { // Entry 901
    -0x1.f68eebfcbb5ecp-1,
    -0x1.79810835a1fe7p1

  },
  { // Entry 902
    -0x1.fbcbe693bd8efp-1,
    0x1.81b5ec8fd79a4p1

  },
  { // Entry 903
    -0x1.fbcbe693bd8efp-1,
    -0x1.81b5ec8fd79a4p1

  },
  { // Entry 904
    -0x1.fef2b2d21cf6cp-1,
    0x1.89ead0ea0d35bp1

  },
  { // Entry 905
    -0x1.fef2b2d21cf6cp-1,
    -0x1.89ead0ea0d35bp1

  },
  { // Entry 906
    0x1.ef4145b4aecffp-1,
    -0x1.81b5ec8fd799fp2

  },
  { // Entry 907
    0x1.ef4145b4aecffp-1,
    0x1.81b5ec8fd799fp2

  },
  { // Entry 908
    0x1.be1d7c3534c4p-1,
    -0x1.714c23db6c626p2

  },
  { // Entry 909
    0x1.be1d7c3534c4p-1,
    0x1.714c23db6c626p2

  },
  { // Entry 910
    0x1.6fcb7c6b8b919p-1,
    -0x1.60e25b27012adp2

  },
  { // Entry 911
    0x1.6fcb7c6b8b919p-1,
    0x1.60e25b27012adp2

  },
  { // Entry 912
    0x1.096ac02ec42c8p-1,
    -0x1.5078927295f34p2

  },
  { // Entry 913
    0x1.096ac02ec42c8p-1,
    0x1.5078927295f34p2

  },
  { // Entry 914
    0x1.235b331d8f748p-2,
    -0x1.400ec9be2abbbp2

  },
  { // Entry 915
    0x1.235b331d8f748p-2,
    0x1.400ec9be2abbbp2

  },
  { // Entry 916
    0x1.069107ae9332cp-5,
    -0x1.2fa50109bf842p2

  },
  { // Entry 917
    0x1.069107ae9332cp-5,
    0x1.2fa50109bf842p2

  },
  { // Entry 918
    -0x1.c7b90e3024569p-3,
    -0x1.1f3b3855544c9p2

  },
  { // Entry 919
    -0x1.c7b90e3024569p-3,
    0x1.1f3b3855544c9p2

  },
  { // Entry 920
    -0x1.d9a3a336edb63p-2,
    -0x1.0ed16fa0e915p2

  },
  { // Entry 921
    -0x1.d9a3a336edb63p-2,
    0x1.0ed16fa0e915p2

  },
  { // Entry 922
    -0x1.5837d2817cf28p-1,
    -0x1.fccf4dd8fbbaep1

  },
  { // Entry 923
    -0x1.5837d2817cf28p-1,
    0x1.fccf4dd8fbbaep1

  },
  { // Entry 924
    -0x1.ad19e2535aa9p-1,
    -0x1.dbfbbc70254bcp1

  },
  { // Entry 925
    -0x1.ad19e2535aa9p-1,
    0x1.dbfbbc70254bcp1

  },
  { // Entry 926
    -0x1.e5eaa286fbbc3p-1,
    -0x1.bb282b074edcap1

  },
  { // Entry 927
    -0x1.e5eaa286fbbc3p-1,
    0x1.bb282b074edcap1

  },
  { // Entry 928
    -0x1.fef2b2d21cf6bp-1,
    -0x1.9a54999e786d8p1

  },
  { // Entry 929
    -0x1.fef2b2d21cf6bp-1,
    0x1.9a54999e786d8p1

  },
  { // Entry 930
    -0x1.f68eebfcbb5ebp-1,
    -0x1.79810835a1fe6p1

  },
  { // Entry 931
    -0x1.f68eebfcbb5ebp-1,
    0x1.79810835a1fe6p1

  },
  { // Entry 932
    -0x1.cd4bca9cb5c77p-1,
    -0x1.58ad76cccb8f4p1

  },
  { // Entry 933
    -0x1.cd4bca9cb5c77p-1,
    0x1.58ad76cccb8f4p1

  },
  { // Entry 934
    -0x1.85dc3ea1bbcf3p-1,
    -0x1.37d9e563f5202p1

  },
  { // Entry 935
    -0x1.85dc3ea1bbcf3p-1,
    0x1.37d9e563f5202p1

  },
  { // Entry 936
    -0x1.24ec79917164ep-1,
    -0x1.170653fb1eb1p1

  },
  { // Entry 937
    -0x1.24ec79917164ep-1,
    0x1.170653fb1eb1p1

  },
  { // Entry 938
    -0x1.61a76077aee24p-2,
    -0x1.ec6585249083cp0

  },
  { // Entry 939
    -0x1.61a76077aee24p-2,
    0x1.ec6585249083cp0

  },
  { // Entry 940
    -0x1.894f70befbc1p-4,
    -0x1.aabe6252e3a58p0

  },
  { // Entry 941
    -0x1.894f70befbc1p-4,
    0x1.aabe6252e3a58p0

  },
  { // Entry 942
    0x1.46dc4f4ce8374p-3,
    -0x1.69173f8136c74p0

  },
  { // Entry 943
    0x1.46dc4f4ce8374p-3,
    0x1.69173f8136c74p0

  },
  { // Entry 944
    0x1.9e7f8652b4729p-2,
    -0x1.27701caf89e9p0

  },
  { // Entry 945
    0x1.9e7f8652b4729p-2,
    0x1.27701caf89e9p0

  },
  { // Entry 946
    0x1.3f3a0e28bedcp-1,
    -0x1.cb91f3bbba157p-1

  },
  { // Entry 947
    0x1.3f3a0e28bedcp-1,
    0x1.cb91f3bbba157p-1

  },
  { // Entry 948
    0x1.9a52e2e0fbca5p-1,
    -0x1.4843ae186058ep-1

  },
  { // Entry 949
    0x1.9a52e2e0fbca5p-1,
    0x1.4843ae186058ep-1

  },
  { // Entry 950
    0x1.da94d54dd4bffp-1,
    -0x1.89ead0ea0d38ap-2

  },
  { // Entry 951
    0x1.da94d54dd4bffp-1,
    0x1.89ead0ea0d38ap-2

  },
  { // Entry 952
    0x1.fbcbe693bd8eap-1,
    -0x1.069c8b46b37fp-3

  },
  { // Entry 953
    0x1.fbcbe693bd8eap-1,
    0x1.069c8b46b37fp-3

  },
  { // Entry 954
    0x1.fbcbe693bd8fp-1,
    0x1.069c8b46b3734p-3

  },
  { // Entry 955
    0x1.fbcbe693bd8fp-1,
    -0x1.069c8b46b3734p-3

  },
  { // Entry 956
    0x1.da94d54dd4c11p-1,
    0x1.89ead0ea0d32cp-2

  },
  { // Entry 957
    0x1.da94d54dd4c11p-1,
    -0x1.89ead0ea0d32cp-2

  },
  { // Entry 958
    0x1.9a52e2e0fbcc1p-1,
    0x1.4843ae186055fp-1

  },
  { // Entry 959
    0x1.9a52e2e0fbcc1p-1,
    -0x1.4843ae186055fp-1

  },
  { // Entry 960
    0x1.3f3a0e28bede4p-1,
    0x1.cb91f3bbba128p-1

  },
  { // Entry 961
    0x1.3f3a0e28bede4p-1,
    -0x1.cb91f3bbba128p-1

  },
  { // Entry 962
    0x1.9e7f8652b478p-2,
    0x1.27701caf89e78p0

  },
  { // Entry 963
    0x1.9e7f8652b478p-2,
    -0x1.27701caf89e78p0

  },
  { // Entry 964
    0x1.46dc4f4ce8431p-3,
    0x1.69173f8136c5cp0

  },
  { // Entry 965
    0x1.46dc4f4ce8431p-3,
    -0x1.69173f8136c5cp0

  },
  { // Entry 966
    -0x1.894f70befba92p-4,
    0x1.aabe6252e3a4p0

  },
  { // Entry 967
    -0x1.894f70befba92p-4,
    -0x1.aabe6252e3a4p0

  },
  { // Entry 968
    -0x1.61a76077aedcap-2,
    0x1.ec65852490824p0

  },
  { // Entry 969
    -0x1.61a76077aedcap-2,
    -0x1.ec65852490824p0

  },
  { // Entry 970
    -0x1.24ec799171627p-1,
    0x1.170653fb1eb04p1

  },
  { // Entry 971
    -0x1.24ec799171627p-1,
    -0x1.170653fb1eb04p1

  },
  { // Entry 972
    -0x1.85dc3ea1bbcd3p-1,
    0x1.37d9e563f51f6p1

  },
  { // Entry 973
    -0x1.85dc3ea1bbcd3p-1,
    -0x1.37d9e563f51f6p1

  },
  { // Entry 974
    -0x1.cd4bca9cb5c63p-1,
    0x1.58ad76cccb8e8p1

  },
  { // Entry 975
    -0x1.cd4bca9cb5c63p-1,
    -0x1.58ad76cccb8e8p1

  },
  { // Entry 976
    -0x1.f68eebfcbb5e2p-1,
    0x1.79810835a1fdap1

  },
  { // Entry 977
    -0x1.f68eebfcbb5e2p-1,
    -0x1.79810835a1fdap1

  },
  { // Entry 978
    -0x1.fef2b2d21cf6ep-1,
    0x1.9a54999e786ccp1

  },
  { // Entry 979
    -0x1.fef2b2d21cf6ep-1,
    -0x1.9a54999e786ccp1

  },
  { // Entry 980
    -0x1.e5eaa286fbbd2p-1,
    0x1.bb282b074edbep1

  },
  { // Entry 981
    -0x1.e5eaa286fbbd2p-1,
    -0x1.bb282b074edbep1

  },
  { // Entry 982
    -0x1.ad19e2535aaaap-1,
    0x1.dbfbbc70254bp1

  },
  { // Entry 983
    -0x1.ad19e2535aaaap-1,
    -0x1.dbfbbc70254bp1

  },
  { // Entry 984
    -0x1.5837d2817cf4bp-1,
    0x1.fccf4dd8fbba2p1

  },
  { // Entry 985
    -0x1.5837d2817cf4bp-1,
    -0x1.fccf4dd8fbba2p1

  },
  { // Entry 986
    -0x1.d9a3a336edbb8p-2,
    0x1.0ed16fa0e914ap2

  },
  { // Entry 987
    -0x1.d9a3a336edbb8p-2,
    -0x1.0ed16fa0e914ap2

  },
  { // Entry 988
    -0x1.c7b90e3024625p-3,
    0x1.1f3b3855544c3p2

  },
  { // Entry 989
    -0x1.c7b90e3024625p-3,
    -0x1.1f3b3855544c3p2

  },
  { // Entry 990
    0x1.069107ae9302dp-5,
    0x1.2fa50109bf83cp2

  },
  { // Entry 991
    0x1.069107ae9302dp-5,
    -0x1.2fa50109bf83cp2

  },
  { // Entry 992
    0x1.235b331d8f6ecp-2,
    0x1.400ec9be2abb5p2

  },
  { // Entry 993
    0x1.235b331d8f6ecp-2,
    -0x1.400ec9be2abb5p2

  },
  { // Entry 994
    0x1.096ac02ec429fp-1,
    0x1.5078927295f2ep2

  },
  { // Entry 995
    0x1.096ac02ec429fp-1,
    -0x1.5078927295f2ep2

  },
  { // Entry 996
    0x1.6fcb7c6b8b8f7p-1,
    0x1.60e25b27012a7p2

  },
  { // Entry 997
    0x1.6fcb7c6b8b8f7p-1,
    -0x1.60e25b27012a7p2

  },
  { // Entry 998
    0x1.be1d7c3534c28p-1,
    0x1.714c23db6c62p2

  },
  { // Entry 999
    0x1.be1d7c3534c28p-1,
    -0x1.714c23db6c62p2

  },
  { // Entry 1000
    0x1.ef4145b4aecf3p-1,
    0x1.81b5ec8fd7999p2

  },
  { // Entry 1001
    0x1.ef4145b4aecf3p-1,
    -0x1.81b5ec8fd7999p2

  },
  { // Entry 1002
    0x1.ff0fd2c96adfcp-1,
    0x1.effffffffffffp-5

  },
  { // Entry 1003
    0x1.ff0fd2c96adfcp-1,
    -0x1.effffffffffffp-5

  },
  { // Entry 1004
    0x1.ff0fd2c96adfcp-1,
    0x1.fp-5

  },
  { // Entry 1005
    0x1.ff0fd2c96adfcp-1,
    -0x1.fp-5

  },
  { // Entry 1006
    0x1.ff0fd2c96adfcp-1,
    0x1.f000000000001p-5

  },
  { // Entry 1007
    0x1.ff0fd2c96adfcp-1,
    -0x1.f000000000001p-5

  },
  { // Entry 1008
    0x1.fc210055467fep-1,
    0x1.f7fffffffffffp-4

  },
  { // Entry 1009
    0x1.fc210055467fep-1,
    -0x1.f7fffffffffffp-4

  },
  { // Entry 1010
    0x1.fc210055467fep-1,
    0x1.f8p-4

  },
  { // Entry 1011
    0x1.fc210055467fep-1,
    -0x1.f8p-4

  },
  { // Entry 1012
    0x1.fc210055467fep-1,
    0x1.f800000000001p-4

  },
  { // Entry 1013
    0x1.fc210055467fep-1,
    -0x1.f800000000001p-4

  },
  { // Entry 1014
    0x1.f94984b2552e2p-1,
    0x1.4bfffffffffffp-3

  },
  { // Entry 1015
    0x1.f94984b2552e2p-1,
    -0x1.4bfffffffffffp-3

  },
  { // Entry 1016
    0x1.f94984b2552e2p-1,
    0x1.4cp-3

  },
  { // Entry 1017
    0x1.f94984b2552e2p-1,
    -0x1.4cp-3

  },
  { // Entry 1018
    0x1.f94984b2552e2p-1,
    0x1.4c00000000001p-3

  },
  { // Entry 1019
    0x1.f94984b2552e2p-1,
    -0x1.4c00000000001p-3

  },
  { // Entry 1020
    0x1.e921dd42f09bbp-1,
    0x1.3333333333332p-2

  },
  { // Entry 1021
    0x1.e921dd42f09bbp-1,
    -0x1.3333333333332p-2

  },
  { // Entry 1022
    0x1.e921dd42f09bap-1,
    0x1.3333333333333p-2

  },
  { // Entry 1023
    0x1.e921dd42f09bap-1,
    -0x1.3333333333333p-2

  },
  { // Entry 1024
    0x1.e921dd42f09bap-1,
    0x1.3333333333334p-2

  },
  { // Entry 1025
    0x1.e921dd42f09bap-1,
    -0x1.3333333333334p-2

  },
  { // Entry 1026
    0x1.8feedb86bf0efp-1,
    0x1.594317acc4ef8p-1

  },
  { // Entry 1027
    0x1.8feedb86bf0efp-1,
    -0x1.594317acc4ef8p-1

  },
  { // Entry 1028
    0x1.8feedb86bf0efp-1,
    0x1.594317acc4ef9p-1

  },
  { // Entry 1029
    0x1.8feedb86bf0efp-1,
    -0x1.594317acc4ef9p-1

  },
  { // Entry 1030
    0x1.8feedb86bf0eep-1,
    0x1.594317acc4efap-1

  },
  { // Entry 1031
    0x1.8feedb86bf0eep-1,
    -0x1.594317acc4efap-1

  },
  { // Entry 1032
    0x1.6b898fa9efb5ep-1,
    0x1.8ffffffffffffp-1

  },
  { // Entry 1033
    0x1.6b898fa9efb5ep-1,
    -0x1.8ffffffffffffp-1

  },
  { // Entry 1034
    0x1.6b898fa9efb5dp-1,
    0x1.9p-1

  },
  { // Entry 1035
    0x1.6b898fa9efb5dp-1,
    -0x1.9p-1

  },
  { // Entry 1036
    0x1.6b898fa9efb5cp-1,
    0x1.9000000000001p-1

  },
  { // Entry 1037
    0x1.6b898fa9efb5cp-1,
    -0x1.9000000000001p-1

  },
  { // Entry 1038
    0x1.0p0,
    -0x0.0000000000001p-1022

  },
  { // Entry 1039
    0x1.0p0,
    0x0.0000000000001p-1022

  },
  { // Entry 1040
    0x1.0p0,
    -0x0.0p0

  },
  { // Entry 1041
    0x1.0p0,
    0x0.0000000000001p-1022

  },
  { // Entry 1042
    0x1.0p0,
    -0x0.0000000000001p-1022

  },
  { // Entry 1043
    0x1.ff621e3796d7ep-1,
    0x1.921fb54442d17p-5

  },
  { // Entry 1044
    0x1.ff621e3796d7ep-1,
    -0x1.921fb54442d17p-5

  },
  { // Entry 1045
    0x1.ff621e3796d7ep-1,
    0x1.921fb54442d18p-5

  },
  { // Entry 1046
    0x1.ff621e3796d7ep-1,
    -0x1.921fb54442d18p-5

  },
  { // Entry 1047
    0x1.ff621e3796d7ep-1,
    0x1.921fb54442d19p-5

  },
  { // Entry 1048
    0x1.ff621e3796d7ep-1,
    -0x1.921fb54442d19p-5

  },
  { // Entry 1049
    0x1.fd88da3d12526p-1,
    0x1.921fb54442d17p-4

  },
  { // Entry 1050
    0x1.fd88da3d12526p-1,
    -0x1.921fb54442d17p-4

  },
  { // Entry 1051
    0x1.fd88da3d12526p-1,
    0x1.921fb54442d18p-4

  },
  { // Entry 1052
    0x1.fd88da3d12526p-1,
    -0x1.921fb54442d18p-4

  },
  { // Entry 1053
    0x1.fd88da3d12526p-1,
    0x1.921fb54442d19p-4

  },
  { // Entry 1054
    0x1.fd88da3d12526p-1,
    -0x1.921fb54442d19p-4

  },
  { // Entry 1055
    0x1.f6297cff75cbp-1,
    0x1.921fb54442d17p-3

  },
  { // Entry 1056
    0x1.f6297cff75cbp-1,
    -0x1.921fb54442d17p-3

  },
  { // Entry 1057
    0x1.f6297cff75cbp-1,
    0x1.921fb54442d18p-3

  },
  { // Entry 1058
    0x1.f6297cff75cbp-1,
    -0x1.921fb54442d18p-3

  },
  { // Entry 1059
    0x1.f6297cff75cbp-1,
    0x1.921fb54442d19p-3

  },
  { // Entry 1060
    0x1.f6297cff75cbp-1,
    -0x1.921fb54442d19p-3

  },
  { // Entry 1061
    0x1.d906bcf328d46p-1,
    0x1.921fb54442d17p-2

  },
  { // Entry 1062
    0x1.d906bcf328d46p-1,
    -0x1.921fb54442d17p-2

  },
  { // Entry 1063
    0x1.d906bcf328d46p-1,
    0x1.921fb54442d18p-2

  },
  { // Entry 1064
    0x1.d906bcf328d46p-1,
    -0x1.921fb54442d18p-2

  },
  { // Entry 1065
    0x1.d906bcf328d46p-1,
    0x1.921fb54442d19p-2

  },
  { // Entry 1066
    0x1.d906bcf328d46p-1,
    -0x1.921fb54442d19p-2

  },
  { // Entry 1067
    0x1.6a09e667f3bcep-1,
    0x1.921fb54442d17p-1

  },
  { // Entry 1068
    0x1.6a09e667f3bcep-1,
    -0x1.921fb54442d17p-1

  },
  { // Entry 1069
    0x1.6a09e667f3bcdp-1,
    0x1.921fb54442d18p-1

  },
  { // Entry 1070
    0x1.6a09e667f3bcdp-1,
    -0x1.921fb54442d18p-1

  },
  { // Entry 1071
    0x1.6a09e667f3bccp-1,
    0x1.921fb54442d19p-1

  },
  { // Entry 1072
    0x1.6a09e667f3bccp-1,
    -0x1.921fb54442d19p-1

  },
  { // Entry 1073
    0x1.469898cc51702p-52,
    0x1.921fb54442d17p0

  },
  { // Entry 1074
    0x1.469898cc51702p-52,
    -0x1.921fb54442d17p0

  },
  { // Entry 1075
    0x1.1a62633145c07p-54,
    0x1.921fb54442d18p0

  },
  { // Entry 1076
    0x1.1a62633145c07p-54,
    -0x1.921fb54442d18p0

  },
  { // Entry 1077
    -0x1.72cece675d1fdp-53,
    0x1.921fb54442d19p0

  },
  { // Entry 1078
    -0x1.72cece675d1fdp-53,
    -0x1.921fb54442d19p0

  },
  { // Entry 1079
    -0x1.0p0,
    0x1.921fb54442d17p1

  },
  { // Entry 1080
    -0x1.0p0,
    -0x1.921fb54442d17p1

  },
  { // Entry 1081
    -0x1.0p0,
    0x1.921fb54442d18p1

  },
  { // Entry 1082
    -0x1.0p0,
    -0x1.921fb54442d18p1

  },
  { // Entry 1083
    -0x1.0p0,
    0x1.921fb54442d19p1

  },
  { // Entry 1084
    -0x1.0p0,
    -0x1.921fb54442d19p1

  },
  { // Entry 1085
    0x1.0p0,
    0x1.921fb54442d17p2

  },
  { // Entry 1086
    0x1.0p0,
    -0x1.921fb54442d17p2

  },
  { // Entry 1087
    0x1.0p0,
    0x1.921fb54442d18p2

  },
  { // Entry 1088
    0x1.0p0,
    -0x1.921fb54442d18p2

  },
  { // Entry 1089
    0x1.0p0,
    0x1.921fb54442d19p2

  },
  { // Entry 1090
    0x1.0p0,
    -0x1.921fb54442d19p2

  },
  { // Entry 1091
    0x1.0p0,
    0x1.921fb54442d17p3

  },
  { // Entry 1092
    0x1.0p0,
    -0x1.921fb54442d17p3

  },
  { // Entry 1093
    0x1.0p0,
    0x1.921fb54442d18p3

  },
  { // Entry 1094
    0x1.0p0,
    -0x1.921fb54442d18p3

  },
  { // Entry 1095
    0x1.0p0,
    0x1.921fb54442d19p3

  },
  { // Entry 1096
    0x1.0p0,
    -0x1.921fb54442d19p3

  },
  { // Entry 1097
    0x1.0p0,
    0x1.921fb54442d17p4

  },
  { // Entry 1098
    0x1.0p0,
    -0x1.921fb54442d17p4

  },
  { // Entry 1099
    0x1.0p0,
    0x1.921fb54442d18p4

  },
  { // Entry 1100
    0x1.0p0,
    -0x1.921fb54442d18p4

  },
  { // Entry 1101
    0x1.0p0,
    0x1.921fb54442d19p4

  },
  { // Entry 1102
    0x1.0p0,
    -0x1.921fb54442d19p4

  },
  { // Entry 1103
    0x1.0p0,
    0x1.921fb54442d17p5

  },
  { // Entry 1104
    0x1.0p0,
    -0x1.921fb54442d17p5

  },
  { // Entry 1105
    0x1.0p0,
    0x1.921fb54442d18p5

  },
  { // Entry 1106
    0x1.0p0,
    -0x1.921fb54442d18p5

  },
  { // Entry 1107
    0x1.0p0,
    0x1.921fb54442d19p5

  },
  { // Entry 1108
    0x1.0p0,
    -0x1.921fb54442d19p5

  },
  { // Entry 1109
    0x1.0p0,
    0x1.921fb54442d17p6

  },
  { // Entry 1110
    0x1.0p0,
    -0x1.921fb54442d17p6

  },
  { // Entry 1111
    0x1.0p0,
    0x1.921fb54442d18p6

  },
  { // Entry 1112
    0x1.0p0,
    -0x1.921fb54442d18p6

  },
  { // Entry 1113
    0x1.0p0,
    0x1.921fb54442d19p6

  },
  { // Entry 1114
    0x1.0p0,
    -0x1.921fb54442d19p6

  },
  { // Entry 1115
    0x1.0p0,
    0x1.921fb54442d17p7

  },
  { // Entry 1116
    0x1.0p0,
    -0x1.921fb54442d17p7

  },
  { // Entry 1117
    0x1.0p0,
    0x1.921fb54442d18p7

  },
  { // Entry 1118
    0x1.0p0,
    -0x1.921fb54442d18p7

  },
  { // Entry 1119
    0x1.0p0,
    0x1.921fb54442d19p7

  },
  { // Entry 1120
    0x1.0p0,
    -0x1.921fb54442d19p7

  },
  { // Entry 1121
    -0x1.6a09e667f3bc9p-1,
    0x1.2d97c7f3321d1p1

  },
  { // Entry 1122
    -0x1.6a09e667f3bc9p-1,
    -0x1.2d97c7f3321d1p1

  },
  { // Entry 1123
    -0x1.6a09e667f3bccp-1,
    0x1.2d97c7f3321d2p1

  },
  { // Entry 1124
    -0x1.6a09e667f3bccp-1,
    -0x1.2d97c7f3321d2p1

  },
  { // Entry 1125
    -0x1.6a09e667f3bcfp-1,
    0x1.2d97c7f3321d3p1

  },
  { // Entry 1126
    -0x1.6a09e667f3bcfp-1,
    -0x1.2d97c7f3321d3p1

  },
  { // Entry 1127
    -0x1.6a09e667f3bdp-1,
    0x1.f6a7a2955385dp1

  },
  { // Entry 1128
    -0x1.6a09e667f3bdp-1,
    -0x1.f6a7a2955385dp1

  },
  { // Entry 1129
    -0x1.6a09e667f3bcep-1,
    0x1.f6a7a2955385ep1

  },
  { // Entry 1130
    -0x1.6a09e667f3bcep-1,
    -0x1.f6a7a2955385ep1

  },
  { // Entry 1131
    -0x1.6a09e667f3bcbp-1,
    0x1.f6a7a2955385fp1

  },
  { // Entry 1132
    -0x1.6a09e667f3bcbp-1,
    -0x1.f6a7a2955385fp1

  },
  { // Entry 1133
    -0x1.34f272993d141p-50,
    0x1.2d97c7f3321d1p2

  },
  { // Entry 1134
    -0x1.34f272993d141p-50,
    -0x1.2d97c7f3321d1p2

  },
  { // Entry 1135
    -0x1.a79394c9e8a0ap-53,
    0x1.2d97c7f3321d2p2

  },
  { // Entry 1136
    -0x1.a79394c9e8a0ap-53,
    -0x1.2d97c7f3321d2p2

  },
  { // Entry 1137
    0x1.961b1acd85d7dp-51,
    0x1.2d97c7f3321d3p2

  },
  { // Entry 1138
    0x1.961b1acd85d7dp-51,
    -0x1.2d97c7f3321d3p2

  },
  { // Entry 1139
    0x1.6a09e667f3bc6p-1,
    0x1.5fdbbe9bba774p2

  },
  { // Entry 1140
    0x1.6a09e667f3bc6p-1,
    -0x1.5fdbbe9bba774p2

  },
  { // Entry 1141
    0x1.6a09e667f3bcbp-1,
    0x1.5fdbbe9bba775p2

  },
  { // Entry 1142
    0x1.6a09e667f3bcbp-1,
    -0x1.5fdbbe9bba775p2

  },
  { // Entry 1143
    0x1.6a09e667f3bd1p-1,
    0x1.5fdbbe9bba776p2

  },
  { // Entry 1144
    0x1.6a09e667f3bd1p-1,
    -0x1.5fdbbe9bba776p2

  },
  { // Entry 1145
    0x1.6a09e667f3bd4p-1,
    0x1.c463abeccb2bap2

  },
  { // Entry 1146
    0x1.6a09e667f3bd4p-1,
    -0x1.c463abeccb2bap2

  },
  { // Entry 1147
    0x1.6a09e667f3bcep-1,
    0x1.c463abeccb2bbp2

  },
  { // Entry 1148
    0x1.6a09e667f3bcep-1,
    -0x1.c463abeccb2bbp2

  },
  { // Entry 1149
    0x1.6a09e667f3bc9p-1,
    0x1.c463abeccb2bcp2

  },
  { // Entry 1150
    0x1.6a09e667f3bc9p-1,
    -0x1.c463abeccb2bcp2

  },
  { // Entry 1151
    0x1.583ebeff65cc2p-50,
    0x1.f6a7a2955385dp2

  },
  { // Entry 1152
    0x1.583ebeff65cc2p-50,
    -0x1.f6a7a2955385dp2

  },
  { // Entry 1153
    0x1.60fafbfd97309p-52,
    0x1.f6a7a2955385ep2

  },
  { // Entry 1154
    0x1.60fafbfd97309p-52,
    -0x1.f6a7a2955385ep2

  },
  { // Entry 1155
    -0x1.4f8282013467cp-51,
    0x1.f6a7a2955385fp2

  },
  { // Entry 1156
    -0x1.4f8282013467cp-51,
    -0x1.f6a7a2955385fp2

  },
  { // Entry 1157
    -0x1.6a09e667f3bb9p-1,
    0x1.1475cc9eedeffp3

  },
  { // Entry 1158
    -0x1.6a09e667f3bb9p-1,
    -0x1.1475cc9eedeffp3

  },
  { // Entry 1159
    -0x1.6a09e667f3bc5p-1,
    0x1.1475cc9eedfp3

  },
  { // Entry 1160
    -0x1.6a09e667f3bc5p-1,
    -0x1.1475cc9eedfp3

  },
  { // Entry 1161
    -0x1.6a09e667f3bdp-1,
    0x1.1475cc9eedf01p3

  },
  { // Entry 1162
    -0x1.6a09e667f3bdp-1,
    -0x1.1475cc9eedf01p3

  },
  { // Entry 1163
    -0x1.0p0,
    0x1.2d97c7f3321d1p3

  },
  { // Entry 1164
    -0x1.0p0,
    -0x1.2d97c7f3321d1p3

  },
  { // Entry 1165
    -0x1.0p0,
    0x1.2d97c7f3321d2p3

  },
  { // Entry 1166
    -0x1.0p0,
    -0x1.2d97c7f3321d2p3

  },
  { // Entry 1167
    -0x1.0p0,
    0x1.2d97c7f3321d3p3

  },
  { // Entry 1168
    -0x1.0p0,
    -0x1.2d97c7f3321d3p3

  },
  { // Entry 1169
    -0x1.6a09e667f3bep-1,
    0x1.46b9c347764a2p3

  },
  { // Entry 1170
    -0x1.6a09e667f3bep-1,
    -0x1.46b9c347764a2p3

  },
  { // Entry 1171
    -0x1.6a09e667f3bd5p-1,
    0x1.46b9c347764a3p3

  },
  { // Entry 1172
    -0x1.6a09e667f3bd5p-1,
    -0x1.46b9c347764a3p3

  },
  { // Entry 1173
    -0x1.6a09e667f3bc9p-1,
    0x1.46b9c347764a4p3

  },
  { // Entry 1174
    -0x1.6a09e667f3bc9p-1,
    -0x1.46b9c347764a4p3

  },
  { // Entry 1175
    -0x1.3dc585b2c7422p-49,
    0x1.5fdbbe9bba774p3

  },
  { // Entry 1176
    -0x1.3dc585b2c7422p-49,
    -0x1.5fdbbe9bba774p3

  },
  { // Entry 1177
    -0x1.ee2c2d963a10cp-52,
    0x1.5fdbbe9bba775p3

  },
  { // Entry 1178
    -0x1.ee2c2d963a10cp-52,
    -0x1.5fdbbe9bba775p3

  },
  { // Entry 1179
    0x1.8474f49a717bdp-50,
    0x1.5fdbbe9bba776p3

  },
  { // Entry 1180
    0x1.8474f49a717bdp-50,
    -0x1.5fdbbe9bba776p3

  },
  { // Entry 1181
    0x1.6a09e667f3bb9p-1,
    0x1.78fdb9effea45p3

  },
  { // Entry 1182
    0x1.6a09e667f3bb9p-1,
    -0x1.78fdb9effea45p3

  },
  { // Entry 1183
    0x1.6a09e667f3bc4p-1,
    0x1.78fdb9effea46p3

  },
  { // Entry 1184
    0x1.6a09e667f3bc4p-1,
    -0x1.78fdb9effea46p3

  },
  { // Entry 1185
    0x1.6a09e667f3bcfp-1,
    0x1.78fdb9effea47p3

  },
  { // Entry 1186
    0x1.6a09e667f3bcfp-1,
    -0x1.78fdb9effea47p3

  },
  { // Entry 1187
    0x1.6a09e667f3be1p-1,
    0x1.ab41b09886fe8p3

  },
  { // Entry 1188
    0x1.6a09e667f3be1p-1,
    -0x1.ab41b09886fe8p3

  },
  { // Entry 1189
    0x1.6a09e667f3bd6p-1,
    0x1.ab41b09886fe9p3

  },
  { // Entry 1190
    0x1.6a09e667f3bd6p-1,
    -0x1.ab41b09886fe9p3

  },
  { // Entry 1191
    0x1.6a09e667f3bcap-1,
    0x1.ab41b09886feap3

  },
  { // Entry 1192
    0x1.6a09e667f3bcap-1,
    -0x1.ab41b09886feap3

  },
  { // Entry 1193
    0x1.4f6babe5db9e2p-49,
    0x1.c463abeccb2bap3

  },
  { // Entry 1194
    0x1.4f6babe5db9e2p-49,
    -0x1.c463abeccb2bap3

  },
  { // Entry 1195
    0x1.3daeaf976e788p-51,
    0x1.c463abeccb2bbp3

  },
  { // Entry 1196
    0x1.3daeaf976e788p-51,
    -0x1.c463abeccb2bbp3

  },
  { // Entry 1197
    -0x1.6128a83448c3cp-50,
    0x1.c463abeccb2bcp3

  },
  { // Entry 1198
    -0x1.6128a83448c3cp-50,
    -0x1.c463abeccb2bcp3

  },
  { // Entry 1199
    -0x1.6a09e667f3bb8p-1,
    0x1.dd85a7410f58bp3

  },
  { // Entry 1200
    -0x1.6a09e667f3bb8p-1,
    -0x1.dd85a7410f58bp3

  },
  { // Entry 1201
    -0x1.6a09e667f3bc3p-1,
    0x1.dd85a7410f58cp3

  },
  { // Entry 1202
    -0x1.6a09e667f3bc3p-1,
    -0x1.dd85a7410f58cp3

  },
  { // Entry 1203
    -0x1.6a09e667f3bcep-1,
    0x1.dd85a7410f58dp3

  },
  { // Entry 1204
    -0x1.6a09e667f3bcep-1,
    -0x1.dd85a7410f58dp3

  },
  { // Entry 1205
    -0x1.0p0,
    0x1.f6a7a2955385dp3

  },
  { // Entry 1206
    -0x1.0p0,
    -0x1.f6a7a2955385dp3

  },
  { // Entry 1207
    -0x1.0p0,
    0x1.f6a7a2955385ep3

  },
  { // Entry 1208
    -0x1.0p0,
    -0x1.f6a7a2955385ep3

  },
  { // Entry 1209
    -0x1.0p0,
    0x1.f6a7a2955385fp3

  },
  { // Entry 1210
    -0x1.0p0,
    -0x1.f6a7a2955385fp3

  },
  { // Entry 1211
    -0x1.6a09e667f3bf8p-1,
    0x1.07e4cef4cbd96p4

  },
  { // Entry 1212
    -0x1.6a09e667f3bf8p-1,
    -0x1.07e4cef4cbd96p4

  },
  { // Entry 1213
    -0x1.6a09e667f3be2p-1,
    0x1.07e4cef4cbd97p4

  },
  { // Entry 1214
    -0x1.6a09e667f3be2p-1,
    -0x1.07e4cef4cbd97p4

  },
  { // Entry 1215
    -0x1.6a09e667f3bcbp-1,
    0x1.07e4cef4cbd98p4

  },
  { // Entry 1216
    -0x1.6a09e667f3bcbp-1,
    -0x1.07e4cef4cbd98p4

  },
  { // Entry 1217
    -0x1.b088e90c77fd1p-48,
    0x1.1475cc9eedeffp4

  },
  { // Entry 1218
    -0x1.b088e90c77fd1p-48,
    -0x1.1475cc9eedeffp4

  },
  { // Entry 1219
    -0x1.6111d218effa2p-49,
    0x1.1475cc9eedfp4

  },
  { // Entry 1220
    -0x1.6111d218effa2p-49,
    -0x1.1475cc9eedfp4

  },
  { // Entry 1221
    0x1.3ddc5bce200bbp-50,
    0x1.1475cc9eedf01p4

  },
  { // Entry 1222
    0x1.3ddc5bce200bbp-50,
    -0x1.1475cc9eedf01p4

  },
  { // Entry 1223
    0x1.6a09e667f3bacp-1,
    0x1.2106ca4910068p4

  },
  { // Entry 1224
    0x1.6a09e667f3bacp-1,
    -0x1.2106ca4910068p4

  },
  { // Entry 1225
    0x1.6a09e667f3bc3p-1,
    0x1.2106ca4910069p4

  },
  { // Entry 1226
    0x1.6a09e667f3bc3p-1,
    -0x1.2106ca4910069p4

  },
  { // Entry 1227
    0x1.6a09e667f3bd9p-1,
    0x1.2106ca491006ap4

  },
  { // Entry 1228
    0x1.6a09e667f3bd9p-1,
    -0x1.2106ca491006ap4

  },
  { // Entry 1229
    0x1.0p0,
    0x1.2d97c7f3321d1p4

  },
  { // Entry 1230
    0x1.0p0,
    -0x1.2d97c7f3321d1p4

  },
  { // Entry 1231
    0x1.0p0,
    0x1.2d97c7f3321d2p4

  },
  { // Entry 1232
    0x1.0p0,
    -0x1.2d97c7f3321d2p4

  },
  { // Entry 1233
    0x1.0p0,
    0x1.2d97c7f3321d3p4

  },
  { // Entry 1234
    0x1.0p0,
    -0x1.2d97c7f3321d3p4

  },
  { // Entry 1235
    0x1.6a09e667f3bf9p-1,
    0x1.3a28c59d54339p4

  },
  { // Entry 1236
    0x1.6a09e667f3bf9p-1,
    -0x1.3a28c59d54339p4

  },
  { // Entry 1237
    0x1.6a09e667f3be2p-1,
    0x1.3a28c59d5433ap4

  },
  { // Entry 1238
    0x1.6a09e667f3be2p-1,
    -0x1.3a28c59d5433ap4

  },
  { // Entry 1239
    0x1.6a09e667f3bccp-1,
    0x1.3a28c59d5433bp4

  },
  { // Entry 1240
    0x1.6a09e667f3bccp-1,
    -0x1.3a28c59d5433bp4

  },
  { // Entry 1241
    0x1.b95bfc26022b1p-48,
    0x1.46b9c347764a2p4

  },
  { // Entry 1242
    0x1.b95bfc26022b1p-48,
    -0x1.46b9c347764a2p4

  },
  { // Entry 1243
    0x1.72b7f84c04563p-49,
    0x1.46b9c347764a3p4

  },
  { // Entry 1244
    0x1.72b7f84c04563p-49,
    -0x1.46b9c347764a3p4

  },
  { // Entry 1245
    -0x1.1a900f67f753ap-50,
    0x1.46b9c347764a4p4

  },
  { // Entry 1246
    -0x1.1a900f67f753ap-50,
    -0x1.46b9c347764a4p4

  },
  { // Entry 1247
    -0x1.6a09e667f3babp-1,
    0x1.534ac0f19860bp4

  },
  { // Entry 1248
    -0x1.6a09e667f3babp-1,
    -0x1.534ac0f19860bp4

  },
  { // Entry 1249
    -0x1.6a09e667f3bc2p-1,
    0x1.534ac0f19860cp4

  },
  { // Entry 1250
    -0x1.6a09e667f3bc2p-1,
    -0x1.534ac0f19860cp4

  },
  { // Entry 1251
    -0x1.6a09e667f3bd8p-1,
    0x1.534ac0f19860dp4

  },
  { // Entry 1252
    -0x1.6a09e667f3bd8p-1,
    -0x1.534ac0f19860dp4

  },
  { // Entry 1253
    -0x1.0p0,
    0x1.5fdbbe9bba774p4

  },
  { // Entry 1254
    -0x1.0p0,
    -0x1.5fdbbe9bba774p4

  },
  { // Entry 1255
    -0x1.0p0,
    0x1.5fdbbe9bba775p4

  },
  { // Entry 1256
    -0x1.0p0,
    -0x1.5fdbbe9bba775p4

  },
  { // Entry 1257
    -0x1.0p0,
    0x1.5fdbbe9bba776p4

  },
  { // Entry 1258
    -0x1.0p0,
    -0x1.5fdbbe9bba776p4

  },
  { // Entry 1259
    -0x1.6a09e667f3bfap-1,
    0x1.6c6cbc45dc8dcp4

  },
  { // Entry 1260
    -0x1.6a09e667f3bfap-1,
    -0x1.6c6cbc45dc8dcp4

  },
  { // Entry 1261
    -0x1.6a09e667f3be3p-1,
    0x1.6c6cbc45dc8ddp4

  },
  { // Entry 1262
    -0x1.6a09e667f3be3p-1,
    -0x1.6c6cbc45dc8ddp4

  },
  { // Entry 1263
    -0x1.6a09e667f3bcdp-1,
    0x1.6c6cbc45dc8dep4

  },
  { // Entry 1264
    -0x1.6a09e667f3bcdp-1,
    -0x1.6c6cbc45dc8dep4

  },
  { // Entry 1265
    -0x1.c22f0f3f8c592p-48,
    0x1.78fdb9effea45p4

  },
  { // Entry 1266
    -0x1.c22f0f3f8c592p-48,
    -0x1.78fdb9effea45p4

  },
  { // Entry 1267
    -0x1.845e1e7f18b23p-49,
    0x1.78fdb9effea46p4

  },
  { // Entry 1268
    -0x1.845e1e7f18b23p-49,
    -0x1.78fdb9effea46p4

  },
  { // Entry 1269
    0x1.ee8786039d373p-51,
    0x1.78fdb9effea47p4

  },
  { // Entry 1270
    0x1.ee8786039d373p-51,
    -0x1.78fdb9effea47p4

  },
  { // Entry 1271
    0x1.6a09e667f3baap-1,
    0x1.858eb79a20baep4

  },
  { // Entry 1272
    0x1.6a09e667f3baap-1,
    -0x1.858eb79a20baep4

  },
  { // Entry 1273
    0x1.6a09e667f3bc1p-1,
    0x1.858eb79a20bafp4

  },
  { // Entry 1274
    0x1.6a09e667f3bc1p-1,
    -0x1.858eb79a20bafp4

  },
  { // Entry 1275
    0x1.6a09e667f3bd8p-1,
    0x1.858eb79a20bbp4

  },
  { // Entry 1276
    0x1.6a09e667f3bd8p-1,
    -0x1.858eb79a20bbp4

  },
  { // Entry 1277
    -0x1.2ccaf641d4262p-3,
    0x1.fffffffffffffp62

  },
  { // Entry 1278
    -0x1.2ccaf641d4262p-3,
    -0x1.fffffffffffffp62

  },
  { // Entry 1279
    0x1.82aa375b3c33ep-7,
    0x1.0p63

  },
  { // Entry 1280
    0x1.82aa375b3c33ep-7,
    -0x1.0p63

  },
  { // Entry 1281
    0x1.4c0622a6e35dep-2,
    0x1.0000000000001p63

  },
  { // Entry 1282
    0x1.4c0622a6e35dep-2,
    -0x1.0000000000001p63

  },
  { // Entry 1283
    0x1.4ab650b8c6073p-1,
    0x1.fffffffffffffp26

  },
  { // Entry 1284
    0x1.4ab650b8c6073p-1,
    -0x1.fffffffffffffp26

  },
  { // Entry 1285
    0x1.4ab6511a7d39bp-1,
    0x1.0p27

  },
  { // Entry 1286
    0x1.4ab6511a7d39bp-1,
    -0x1.0p27

  },
  { // Entry 1287
    0x1.4ab651ddeb9e6p-1,
    0x1.0000000000001p27

  },
  { // Entry 1288
    0x1.4ab651ddeb9e6p-1,
    -0x1.0000000000001p27

  },
  { // Entry 1289
    0x1.40ad67e777b1ep-1,
    0x1.fffffffffffffp23

  },
  { // Entry 1290
    0x1.40ad67e777b1ep-1,
    -0x1.fffffffffffffp23

  },
  { // Entry 1291
    0x1.40ad67f3f0c9ap-1,
    0x1.0p24

  },
  { // Entry 1292
    0x1.40ad67f3f0c9ap-1,
    -0x1.0p24

  },
  { // Entry 1293
    0x1.40ad680ce2f92p-1,
    0x1.0000000000001p24

  },
  { // Entry 1294
    0x1.40ad680ce2f92p-1,
    -0x1.0000000000001p24

  },
  { // Entry 1295
    -0x1.4eaa606db24c4p-1,
    0x1.fffffffffffffp1

  },
  { // Entry 1296
    -0x1.4eaa606db24c4p-1,
    -0x1.fffffffffffffp1

  },
  { // Entry 1297
    -0x1.4eaa606db24c1p-1,
    0x1.0p2

  },
  { // Entry 1298
    -0x1.4eaa606db24c1p-1,
    -0x1.0p2

  },
  { // Entry 1299
    -0x1.4eaa606db24bbp-1,
    0x1.0000000000001p2

  },
  { // Entry 1300
    -0x1.4eaa606db24bbp-1,
    -0x1.0000000000001p2

  },
  { // Entry 1301
    -0x1.aa22657537201p-2,
    0x1.fffffffffffffp0

  },
  { // Entry 1302
    -0x1.aa22657537201p-2,
    -0x1.fffffffffffffp0

  },
  { // Entry 1303
    -0x1.aa22657537205p-2,
    0x1.0p1

  },
  { // Entry 1304
    -0x1.aa22657537205p-2,
    -0x1.0p1

  },
  { // Entry 1305
    -0x1.aa2265753720cp-2,
    0x1.0000000000001p1

  },
  { // Entry 1306
    -0x1.aa2265753720cp-2,
    -0x1.0000000000001p1

  },
  { // Entry 1307
    0x1.14a280fb5068cp-1,
    0x1.fffffffffffffp-1

  },
  { // Entry 1308
    0x1.14a280fb5068cp-1,
    -0x1.fffffffffffffp-1

  },
  { // Entry 1309
    0x1.14a280fb5068cp-1,
    0x1.0p0

  },
  { // Entry 1310
    0x1.14a280fb5068cp-1,
    -0x1.0p0

  },
  { // Entry 1311
    0x1.14a280fb5068ap-1,
    0x1.0000000000001p0

  },
  { // Entry 1312
    0x1.14a280fb5068ap-1,
    -0x1.0000000000001p0

  },
  { // Entry 1313
    0x1.c1528065b7d5p-1,
    0x1.fffffffffffffp-2

  },
  { // Entry 1314
    0x1.c1528065b7d5p-1,
    -0x1.fffffffffffffp-2

  },
  { // Entry 1315
    0x1.c1528065b7d5p-1,
    0x1.0p-1

  },
  { // Entry 1316
    0x1.c1528065b7d5p-1,
    -0x1.0p-1

  },
  { // Entry 1317
    0x1.c1528065b7d4fp-1,
    0x1.0000000000001p-1

  },
  { // Entry 1318
    0x1.c1528065b7d4fp-1,
    -0x1.0000000000001p-1

  },
  { // Entry 1319
    0x1.f01549f7deea2p-1,
    0x1.fffffffffffffp-3

  },
  { // Entry 1320
    0x1.f01549f7deea2p-1,
    -0x1.fffffffffffffp-3

  },
  { // Entry 1321
    0x1.f01549f7deea1p-1,
    0x1.0p-2

  },
  { // Entry 1322
    0x1.f01549f7deea1p-1,
    -0x1.0p-2

  },
  { // Entry 1323
    0x1.f01549f7deea1p-1,
    0x1.0000000000001p-2

  },
  { // Entry 1324
    0x1.f01549f7deea1p-1,
    -0x1.0000000000001p-2

  },
  { // Entry 1325
    0x1.fc015527d5bd3p-1,
    0x1.fffffffffffffp-4

  },
  { // Entry 1326
    0x1.fc015527d5bd3p-1,
    -0x1.fffffffffffffp-4

  },
  { // Entry 1327
    0x1.fc015527d5bd3p-1,
    0x1.0p-3

  },
  { // Entry 1328
    0x1.fc015527d5bd3p-1,
    -0x1.0p-3

  },
  { // Entry 1329
    0x1.fc015527d5bd3p-1,
    0x1.0000000000001p-3

  },
  { // Entry 1330
    0x1.fc015527d5bd3p-1,
    -0x1.0000000000001p-3

  },
  { // Entry 1331
    0x1.ff0015549f4d3p-1,
    0x1.fffffffffffffp-5

  },
  { // Entry 1332
    0x1.ff0015549f4d3p-1,
    -0x1.fffffffffffffp-5

  },
  { // Entry 1333
    0x1.ff0015549f4d3p-1,
    0x1.0p-4

  },
  { // Entry 1334
    0x1.ff0015549f4d3p-1,
    -0x1.0p-4

  },
  { // Entry 1335
    0x1.ff0015549f4d3p-1,
    0x1.0000000000001p-4

  },
  { // Entry 1336
    0x1.ff0015549f4d3p-1,
    -0x1.0000000000001p-4

  },
  { // Entry 1337
    0x1.ffc00155527d3p-1,
    0x1.fffffffffffffp-6

  },
  { // Entry 1338
    0x1.ffc00155527d3p-1,
    -0x1.fffffffffffffp-6

  },
  { // Entry 1339
    0x1.ffc00155527d3p-1,
    0x1.0p-5

  },
  { // Entry 1340
    0x1.ffc00155527d3p-1,
    -0x1.0p-5

  },
  { // Entry 1341
    0x1.ffc00155527d3p-1,
    0x1.0000000000001p-5

  },
  { // Entry 1342
    0x1.ffc00155527d3p-1,
    -0x1.0000000000001p-5

  },
  { // Entry 1343
    0x1.fff000155549fp-1,
    0x1.fffffffffffffp-7

  },
  { // Entry 1344
    0x1.fff000155549fp-1,
    -0x1.fffffffffffffp-7

  },
  { // Entry 1345
    0x1.fff000155549fp-1,
    0x1.0p-6

  },
  { // Entry 1346
    0x1.fff000155549fp-1,
    -0x1.0p-6

  },
  { // Entry 1347
    0x1.fff000155549fp-1,
    0x1.0000000000001p-6

  },
  { // Entry 1348
    0x1.fff000155549fp-1,
    -0x1.0000000000001p-6

  },
  { // Entry 1349
    0x1.fffffffp-1,
    0x1.fffffffffffffp-15

  },
  { // Entry 1350
    0x1.fffffffp-1,
    -0x1.fffffffffffffp-15

  },
  { // Entry 1351
    0x1.fffffffp-1,
    0x1.0p-14

  },
  { // Entry 1352
    0x1.fffffffp-1,
    -0x1.0p-14

  },
  { // Entry 1353
    0x1.fffffffp-1,
    0x1.0000000000001p-14

  },
  { // Entry 1354
    0x1.fffffffp-1,
    -0x1.0000000000001p-14

  },
  { // Entry 1355
    0x1.0p0,
    0x1.fffffffffffffp-28

  },
  { // Entry 1356
    0x1.0p0,
    -0x1.fffffffffffffp-28

  },
  { // Entry 1357
    0x1.0p0,
    0x1.0p-27

  },
  { // Entry 1358
    0x1.0p0,
    -0x1.0p-27

  },
  { // Entry 1359
    0x1.0p0,
    0x1.0000000000001p-27

  },
  { // Entry 1360
    0x1.0p0,
    -0x1.0000000000001p-27

  },
  { // Entry 1361
    0x1.0p0,
    0x1.fffffffffffffp-31

  },
  { // Entry 1362
    0x1.0p0,
    -0x1.fffffffffffffp-31

  },
  { // Entry 1363
    0x1.0p0,
    0x1.0p-30

  },
  { // Entry 1364
    0x1.0p0,
    -0x1.0p-30

  },
  { // Entry 1365
    0x1.0p0,
    0x1.0000000000001p-30

  },
  { // Entry 1366
    0x1.0p0,
    -0x1.0000000000001p-30

  },
  { // Entry 1367
    -0x1.fffe62ecfab75p-1,
    -0x1.fffffffffffffp1023

  },
  { // Entry 1368
    -0x1.fffe62ecfab75p-1,
    0x1.fffffffffffffp1023

  },
  { // Entry 1369
    -0x1.fffe62ecfab75p-1,
    0x1.fffffffffffffp1023

  },
  { // Entry 1370
    -0x1.fffe62ecfab75p-1,
    -0x1.fffffffffffffp1023

  },
  { // Entry 1371
    -0x1.fffe62ecfab75p-1,
    0x1.fffffffffffffp1023

  },
  { // Entry 1372
    -0x1.fffe62ecfab75p-1,
    -0x1.fffffffffffffp1023

  },
  { // Entry 1373
    0x1.7ffdfb4c5309p-2,
    0x1.ffffffffffffep1023

  },
  { // Entry 1374
    0x1.7ffdfb4c5309p-2,
    -0x1.ffffffffffffep1023

  },
  { // Entry 1375
    -0x1.0p0,
    0x1.921fb54442d18p1

  },
  { // Entry 1376
    -0x1.0p0,
    -0x1.921fb54442d18p1

  },
  { // Entry 1377
    0x1.1a62633145c07p-54,
    0x1.921fb54442d18p0

  },
  { // Entry 1378
    0x1.1a62633145c07p-54,
    -0x1.921fb54442d18p0

  },
  { // Entry 1379
    0x1.14a280fb5068ap-1,
    0x1.0000000000001p0

  },
  { // Entry 1380
    0x1.14a280fb5068ap-1,
    -0x1.0000000000001p0

  },
  { // Entry 1381
    0x1.14a280fb5068cp-1,
    0x1.0p0

  },
  { // Entry 1382
    0x1.14a280fb5068cp-1,
    -0x1.0p0

  },
  { // Entry 1383
    0x1.14a280fb5068cp-1,
    0x1.fffffffffffffp-1

  },
  { // Entry 1384
    0x1.14a280fb5068cp-1,
    -0x1.fffffffffffffp-1

  },
  { // Entry 1385
    0x1.6a09e667f3bcdp-1,
    0x1.921fb54442d18p-1

  },
  { // Entry 1386
    0x1.6a09e667f3bcdp-1,
    -0x1.921fb54442d18p-1

  },
  { // Entry 1387
    0x1.0p0,
    0x1.0000000000001p-1022

  },
  { // Entry 1388
    0x1.0p0,
    -0x1.0000000000001p-1022

  },
  { // Entry 1389
    0x1.0p0,
    0x1.0p-1022

  },
  { // Entry 1390
    0x1.0p0,
    -0x1.0p-1022

  },
  { // Entry 1391
    0x1.0p0,
    0x0.fffffffffffffp-1022

  },
  { // Entry 1392
    0x1.0p0,
    -0x0.fffffffffffffp-1022

  },
  { // Entry 1393
    0x1.0p0,
    0x0.ffffffffffffep-1022

  },
  { // Entry 1394
    0x1.0p0,
    -0x0.ffffffffffffep-1022

  },
  { // Entry 1395
    0x1.0p0,
    0x0.0000000000002p-1022

  },
  { // Entry 1396
    0x1.0p0,
    -0x0.0000000000002p-1022

  },
  { // Entry 1397
    0x1.0p0,
    0x0.0000000000001p-1022

  },
  { // Entry 1398
    0x1.0p0,
    -0x0.0000000000001p-1022

  },
  { // Entry 1399
    0x1.0p0,
    0x0.0p0

  },
  { // Entry 1400
    0x1.0p0,
    -0x0.0p0

  },
  { // Entry 1401
    -0x1.d2cec9a554007p-1,
    0x1.5bf0a8b145769p1
  },
  { // Entry 1402
    -0x1.d2cec9a554007p-1,
    -0x1.5bf0a8b145769p1
  },
  { // Entry 1403
    0x1.0p0,
    0x0.0p0
  },
  { // Entry 1404
    0x1.0p0,
    -0x0.0p0
  },
  { // Entry 1405
    0x1.14a280fb5068cp-1,
    0x1.0p0
  },
  { // Entry 1406
    0x1.14a280fb5068cp-1,
    -0x1.0p0
  },
  { // Entry 1407
    0x1.c1528065b7d5p-1,
    -0x1.0p-1
  },
  { // Entry 1408
    0x1.c1528065b7d5p-1,
    0x1.0p-1
  },
  { // Entry 1409
    0x1.0p0,
    -0x1.921fb54442d18p2
  },
  { // Entry 1410
    -0x1.a79394c9e8a0ap-53,
    -0x1.2d97c7f3321d2p2
  },
  { // Entry 1411
    -0x1.0p0,
    -0x1.921fb54442d18p1
  },
  { // Entry 1412
    0x1.1a62633145c07p-54,
    -0x1.921fb54442d18p0
  },
  { // Entry 1413
    0x1.0p0,
    0x0.0p0
  },
  { // Entry 1414
    0x1.1a62633145c07p-54,
    0x1.921fb54442d18p0
  },
  { // Entry 1415
    -0x1.0p0,
    0x1.921fb54442d18p1
  },
  { // Entry 1416
    -0x1.a79394c9e8a0ap-53,
    0x1.2d97c7f3321d2p2
  },
  { // Entry 1417
    0x1.0p0,
    0x1.921fb54442d18p2
  },
};

static TestCase g_log_data[] = {
  { // Entry 0
    0x1.d77fd13d27fffp-11,
    0x1.003af6c37c1d3p0

  },
  { // Entry 1
    0x1.411efd297c781p-6,
    0x1.05112792934b3p0

  },
  { // Entry 2
    0x1.d4840664446fcp-6,
    0x1.076d1d8c75ea6p0

  },
  { // Entry 3
    0x1.f6e4c3ced7c72p-3,
    0x1.47408cb9583cep0

  },
  { // Entry 4
    0x1.1a0408712e00ap-2,
    0x1.512b3126454f3p0

  },
  { // Entry 5
    0x1.8b52c15ea9c24p-2,
    0x1.789e95b11578cp0

  },
  { // Entry 6
    0x1.d707029bb59d9p-2,
    0x1.958497f7b353fp0

  },
  { // Entry 7
    0x1.0727af5fee8f6p-1,
    0x1.ac032a8d2ec23p0

  },
  { // Entry 8
    0x1.12fcce02efb32p-1,
    0x1.b604e1942098dp0

  },
  { // Entry 9
    0x1.178e6d3ecaceap-1,
    0x1.b9f1fa4587967p0

  },
  { // Entry 10
    0x1.62f71c4656b61p-1,
    0x1.000976581ce4ep1

  },
  { // Entry 11
    0x1.d6336a88077aap0,
    0x1.91a8dff540ff7p2

  },
  { // Entry 12
    0x1.016e82ceda359p1,
    0x1.de37fb31fd5fcp2

  },
  { // Entry 13
    0x1.1e126f5d95f38p1,
    0x1.2b1199e497739p3

  },
  { // Entry 14
    0x1.2f3832cad3d5fp1,
    0x1.55f0eaa1b2fc8p3

  },
  { // Entry 15
    0x1.42ee3c7dc4946p1,
    0x1.8ede492d96072p3

  },
  { // Entry 16
    0x1.6b5df7e3cd422p1,
    0x1.11867637cbd03p4

  },
  { // Entry 17
    0x1.7e7f095703eeep1,
    0x1.3d9d7d597a9ddp4

  },
  { // Entry 18
    0x1.9687c83faf006p1,
    0x1.7f3825778aaafp4

  },
  { // Entry 19
    0x1.83d4bcdebb3f4p2,
    0x1.ac50b409c8aeep8

  },
  { // Entry 20
    0x1.fffffffffffffp-53,
    0x1.0000000000001p0

  },
  { // Entry 21
    0x1.76e7e5d7b6eacp3,
    0x1.de7cd6751029ap16

  },
  { // Entry 22
    0x1.fffffffffffffp-53,
    0x1.0000000000001p0

  },
  { // Entry 23
    -0x1.91550c357f884p8,
    0x1.0000000000003p-579

  },
  { // Entry 24
    0x1.7fffffffffffcp-50,
    0x1.0000000000006p0

  },
  { // Entry 25
    0x1.bffffffffffe8p-48,
    0x1.000000000001cp0

  },
  { // Entry 26
    0x1.7fdfffffff701p-41,
    0x1.0000000000bffp0

  },
  { // Entry 27
    0x1.0fffffffedfp-35,
    0x1.0000000022p0

  },
  { // Entry 28
    0x1.ffffffffp-33,
    0x1.00000001p0

  },
  { // Entry 29
    0x1.7fffff7000004p-25,
    0x1.000000cp0

  },
  { // Entry 30
    0x1.0a2ea3e77af06p1,
    0x1.0007p3

  },
  { // Entry 31
    0x1.90412094d368p-13,
    0x1.000c82573f5f9p0

  },
  { // Entry 32
    0x1.ffe002aa6ab11p-12,
    0x1.002p0

  },
  { // Entry 33
    0x1.bfcf07242969dp-11,
    0x1.0038p0

  },
  { // Entry 34
    0x1.d77fd13d27fffp-11,
    0x1.003af6c37c1d3p0

  },
  { // Entry 35
    0x1.ea054ce8508e5p-10,
    0x1.007a9ea7a9e8p0

  },
  { // Entry 36
    0x1.ff004a7ab9084p-10,
    0x1.007fdff7fdfep0

  },
  { // Entry 37
    0x1.ffda65d44dccdp-10,
    0x1.0080169a16cd5p0

  },
  { // Entry 38
    0x1.b956989d2589dp-9,
    0x1.00dd0a8317176p0

  },
  { // Entry 39
    0x1.fd0816d97152cp-9,
    0x1.00ff02ba8a543p0

  },
  { // Entry 40
    -0x1.37915555785bp9,
    0x1.00fffffffffffp-899

  },
  { // Entry 41
    0x1.64a2a9b6a4058p0,
    0x1.01c00000000dcp2

  },
  { // Entry 42
    0x1.d465957106bd3p-8,
    0x1.01d6131d09dc5p0

  },
  { // Entry 43
    0x1.fdf639bea5ad3p-8,
    0x1.01fff37a34084p0

  },
  { // Entry 44
    0x1.fdf6e4fcf4569p-8,
    0x1.01fff426c8cb6p0

  },
  { // Entry 45
    0x1.fdf6e4fd0426fp-8,
    0x1.01fff426c8db5p0

  },
  { // Entry 46
    0x1.fdfe9c57debe1p-8,
    0x1.01fffbed922e3p0

  },
  { // Entry 47
    -0x1.d17be245122ap3,
    0x1.028p-21

  },
  { // Entry 48
    0x1.6dcb2fed7f25dp-7,
    0x1.02dfafa07df6dp0

  },
  { // Entry 49
    0x1.ff092a85ee03p-7,
    0x1.040615461f6cap0

  },
  { // Entry 50
    0x1.0aa53cea3f1acp-6,
    0x1.04334eec5a65dp0

  },
  { // Entry 51
    0x1.6ea07021c1335p-6,
    0x1.05cb09bb9fed7p0

  },
  { // Entry 52
    0x1.7199f984f5608p-6,
    0x1.05d735184261cp0

  },
  { // Entry 53
    0x1.7788bc9c08318p-6,
    0x1.05ef7bdee7bep0

  },
  { // Entry 54
    0x1.c2688e952a724p-6,
    0x1.0722a05711778p0

  },
  { // Entry 55
    0x1.e02151352512fp-6,
    0x1.079cf00fe24f9p0

  },
  { // Entry 56
    -0x1.3e1fee699c6bcp8,
    0x1.07cp-459

  },
  { // Entry 57
    0x1.f31b56b8b6f1ap-6,
    0x1.07eb259ee01b5p0

  },
  { // Entry 58
    -0x1.d0819095fcd6cp3,
    0x1.0a8551f8fc7f8p-21

  },
  { // Entry 59
    -0x1.0268d40000c72p1,
    0x1.0fff8p-3

  },
  { // Entry 60
    0x1.6b5df7e3cd422p1,
    0x1.11867637cbd03p4

  },
  { // Entry 61
    0x1.6b96cb66f55c2p1,
    0x1.11ffffffffff8p4

  },
  { // Entry 62
    -0x1.f7a213a7cd381p0,
    0x1.1e6p-3

  },
  { // Entry 63
    -0x1.a0765853dec07p8,
    0x1.206658d9b0f05p-601

  },
  { // Entry 64
    0x1.0d7b6ff6e3a24p6,
    0x1.2514738e6dcadp97

  },
  { // Entry 65
    0x1.1ee3899f6818ep1,
    0x1.2cfbb9e4a1599p3

  },
  { // Entry 66
    -0x1.331533367928ap6,
    0x1.2f0617037e59p-111

  },
  { // Entry 67
    -0x1.04c1e6cec9638p3,
    0x1.2f29fd4b42515p-12

  },
  { // Entry 68
    -0x1.0578bc4a143d6p-1,
    0x1.333e1f03af55ep-1

  },
  { // Entry 69
    -0x1.fd2a1d4dcf23ep-2,
    0x1.3767a0aaf1452p-1

  },
  { // Entry 70
    -0x1.fb0ce0ec79bcap-2,
    0x1.380c640e6246cp-1

  },
  { // Entry 71
    -0x1.ee693caa501aap-2,
    0x1.3bec837d601c1p-1

  },
  { // Entry 72
    -0x1.ed67b9b0f2f8ep-2,
    0x1.3c3cp-1

  },
  { // Entry 73
    0x1.25397dc9f85bep1,
    0x1.3c419b9db662ap3

  },
  { // Entry 74
    0x1.9c041f7ed8d33p0,
    0x1.3ffffffffffffp2

  },
  { // Entry 75
    0x1.c8ff7c79a9a22p-3,
    0x1.4p0

  },
  { // Entry 76
    0x1.d42adfec35d14p-3,
    0x1.41cp0

  },
  { // Entry 77
    0x1.d7577477b9eaep-3,
    0x1.423fc24d04fc6p0

  },
  { // Entry 78
    -0x1.d360e90c3850cp-2,
    0x1.446p-1

  },
  { // Entry 79
    -0x1.cdeabe01a86b2p-2,
    0x1.461c159a76d4bp-1

  },
  { // Entry 80
    0x1.fb9186d5e3e24p-3,
    0x1.47fffffffffffp0

  },
  { // Entry 81
    0x1.fb9186d5e3e2ap-3,
    0x1.48p0

  },
  { // Entry 82
    0x1.fb920464f5fc8p-3,
    0x1.4800141bea83fp0

  },
  { // Entry 83
    0x1.fb9c2792045eap-3,
    0x1.4801b3bf42a4ap0

  },
  { // Entry 84
    0x1.fc46718a2dc22p-3,
    0x1.481cfade8a64dp0

  },
  { // Entry 85
    0x1.ff9bd7855aaebp-3,
    0x1.48a5d0c7ac5b5p0

  },
  { // Entry 86
    0x1.ffd65f29bfd4ep-3,
    0x1.48af356081ca1p0

  },
  { // Entry 87
    0x1.e737cb23865c6p-1,
    0x1.4b8p1

  },
  { // Entry 88
    0x1.90ed52d1a23c5p8,
    0x1.556ee611fa309p578

  },
  { // Entry 89
    -0x1.b25b8d863b3b4p3,
    0x1.55ep-20

  },
  { // Entry 90
    0x1.16758a93d29bp6,
    0x1.5996659966598p100

  },
  { // Entry 91
    -0x1.7e3a93bf3540dp-2,
    0x1.608054de74582p-1

  },
  { // Entry 92
    0x1.583a417be2216p-2,
    0x1.6649bb515354fp0

  },
  { // Entry 93
    -0x1.4f7dac3f1d7f4p5,
    0x1.69b109377f7p-61

  },
  { // Entry 94
    0x1.62e42fefa3837p-2,
    0x1.6a09e667f3b31p0

  },
  { // Entry 95
    -0x1.e7a2dd016daa8p1,
    0x1.6bp-6

  },
  { // Entry 96
    0x1.739d7f6bbd006p-2,
    0x1.7p0

  },
  { // Entry 97
    -0x1.4c3af47972063p-2,
    0x1.7223cef78e25dp-1

  },
  { // Entry 98
    -0x1.48e65fe2c2305p-2,
    0x1.73587ce75368p-1

  },
  { // Entry 99
    -0x1.4057b02bfdd0ap-2,
    0x1.76764087395fcp-1

  },
  { // Entry 100
    0x1.4a5f12ed407a4p8,
    0x1.8ad934b838bdap476

  },
  { // Entry 101
    -0x1.83d0f90690d1p1,
    0x1.8bep-5

  },
  { // Entry 102
    -0x1.e387545735931p-1,
    0x1.8e4p-2

  },
  { // Entry 103
    -0x1.fcc59e0136976p-3,
    0x1.8f6p-1

  },
  { // Entry 104
    -0x1.edd6149b33156p-3,
    0x1.924c57d629082p-1

  },
  { // Entry 105
    0x1.cfbe6164a73f2p-2,
    0x1.92a4cb798664bp0

  },
  { // Entry 106
    -0x1.b428216a5c158p4,
    0x1.98p-40

  },
  { // Entry 107
    -0x1.d081f0f25d664p-3,
    0x1.9819d0255be6p-1

  },
  { // Entry 108
    -0x1.cc0d6a92b7916p-1,
    0x1.a0ef883be2177p-2

  },
  { // Entry 109
    -0x1.8f62e2cb65864p-3,
    0x1.a549336310e6fp-1

  },
  { // Entry 110
    -0x1.3651d0de2368ep-3,
    0x1.b80300c030118p-1

  },
  { // Entry 111
    0x1.20f25e686e7fap-1,
    0x1.c22024dc8289cp0

  },
  { // Entry 112
    -0x1.9e327eb6ac2c4p-1,
    0x1.c8p-2

  },
  { // Entry 113
    -0x1.b76f3d0aaf0cap-4,
    0x1.cbea22c8a9a16p-1

  },
  { // Entry 114
    -0x1.9335e5d594992p-4,
    0x1.cffffffffffffp-1

  },
  { // Entry 115
    0x1.b6cc2b05c90a8p2,
    0x1.dae1894a2dbedp9

  },
  { // Entry 116
    -0x1.fc702b8d96d0ep-5,
    0x1.e1304c1304c13p-1

  },
  { // Entry 117
    -0x1.f6c7f7d0b0356p-5,
    0x1.e185646bc7856p-1

  },
  { // Entry 118
    -0x1.eb8dcb764e07ep-5,
    0x1.e22e7349f6e97p-1

  },
  { // Entry 119
    -0x1.d94a9c323e77cp-5,
    0x1.e341f0592c2abp-1

  },
  { // Entry 120
    -0x1.ccb106085394fp-5,
    0x1.e4005dfd66c32p-1

  },
  { // Entry 121
    -0x1.a9bd8afc6d84cp-5,
    0x1.e61222241ca7fp-1

  },
  { // Entry 122
    -0x1.787392a45888ep-5,
    0x1.e901133e161e4p-1

  },
  { // Entry 123
    -0x1.79a634cdfb256p4,
    0x1.eddc0821c76c1p-35

  },
  { // Entry 124
    0x1.5172153d5fe9cp-1,
    0x1.eed9208a6a349p0

  },
  { // Entry 125
    -0x1.12b227244ff98p-5,
    0x1.ef1dbcea195cdp-1

  },
  { // Entry 126
    -0x1.f8a34d2505f81p-6,
    0x1.f0786fa48bd9cp-1

  },
  { // Entry 127
    0x1.06885d03c19c4p1,
    0x1.f1a945a457d96p2

  },
  { // Entry 128
    0x1.a28cccb9c6f0dp6,
    0x1.f1fffffffffffp150

  },
  { // Entry 129
    -0x1.b032da095671ep-6,
    0x1.f2ab9dee841b5p-1

  },
  { // Entry 130
    -0x1.9d66ada690565p-6,
    0x1.f33e2a944ac96p-1

  },
  { // Entry 131
    -0x1.88e7a6bf1b5bap-6,
    0x1.f3de268394e8dp-1

  },
  { // Entry 132
    -0x1.84e362ad21702p-6,
    0x1.f3fd86b392884p-1

  },
  { // Entry 133
    -0x1.744eff6d77ddep-6,
    0x1.f47f1e1f53f24p-1

  },
  { // Entry 134
    -0x1.68b4f86497a0ap0,
    0x1.f48p-3

  },
  { // Entry 135
    -0x1.6f51e68fcd7cep-6,
    0x1.f4a622df760c9p-1

  },
  { // Entry 136
    -0x1.6bab4f93a372p-6,
    0x1.f4c2b2a5a5c1dp-1

  },
  { // Entry 137
    -0x1.65ae4deeda7b8p-6,
    0x1.f4f18fab5c3b8p-1

  },
  { // Entry 138
    -0x1.4c2a64cb22442p-6,
    0x1.f5b96e5b96eb7p-1

  },
  { // Entry 139
    -0x1.43ae6c44b9f76p-6,
    0x1.f5fbf5cedc48p-1

  },
  { // Entry 140
    -0x1.41f7812310c9bp-6,
    0x1.f60968a5952d9p-1

  },
  { // Entry 141
    -0x1.3b0cb0b1469dcp-6,
    0x1.f63fae6bae321p-1

  },
  { // Entry 142
    -0x1.311e67575b4f5p-6,
    0x1.f68da368da368p-1

  },
  { // Entry 143
    -0x1.020da703f2f9dp-6,
    0x1.f7ffbefc5d9d8p-1

  },
  { // Entry 144
    -0x1.0205658935b94p-6,
    0x1.f7fffffffffe6p-1

  },
  { // Entry 145
    -0x1.ae825fe2eb67p-7,
    0x1.f95139c561139p-1

  },
  { // Entry 146
    -0x1.23851c80dddf2p-7,
    0x1.fb77177fb5d35p-1

  },
  { // Entry 147
    -0x1.7c498eb87ed68p-8,
    0x1.fd09a0b5b17fp-1

  },
  { // Entry 148
    -0x1.50dd41aeb4865p-8,
    0x1.fd6p-1

  },
  { // Entry 149
    -0x1.08b8bba5db3a8p-8,
    0x1.fdef9fe7f9fe3p-1

  },
  { // Entry 150
    -0x1.07c989add9f58p-8,
    0x1.fdf17c5f17cp-1

  },
  { // Entry 151
    -0x1.008055958e10bp-8,
    0x1.fdfffffffff55p-1

  },
  { // Entry 152
    -0x1.1f3e47178f97cp-9,
    0x1.fee1123d05c1bp-1

  },
  { // Entry 153
    -0x1.ff7faa9ab1367p-10,
    0x1.ff007ffffffffp-1

  },
  { // Entry 154
    -0x1.ff39ecbe26759p-10,
    0x1.ff00a2cd88b55p-1

  },
  { // Entry 155
    -0x1.6363477698d25p0,
    0x1.ff020ffffffffp-3

  },
  { // Entry 156
    -0x1.f7a248bf22faap-10,
    0x1.ff046cbe6cbddp-1

  },
  { // Entry 157
    -0x1.dae60fee9fa9fp-10,
    0x1.ff12c3ff12c3p-1

  },
  { // Entry 158
    -0x1.d208bc8587776p-10,
    0x1.ff17309fefcd2p-1

  },
  { // Entry 159
    -0x1.babff263eabf8p-12,
    0x1.ffc8aaff534d4p-1

  },
  { // Entry 160
    -0x1.c00c40725b061p-13,
    0x1.ffe3fffffffffp-1

  },
  { // Entry 161
    -0x1.78bfa50041531p-13,
    0x1.ffe874904abdp-1

  },
  { // Entry 162
    -0x1.1148dff957e7cp-13,
    0x1.ffeeebbaeebb8p-1

  },
  { // Entry 163
    -0x1.4aa196aae1ef9p8,
    0x1.fffffbbffffffp-478

  },
  { // Entry 164
    0x1.8e8f43d38041p8,
    0x1.fffffbbffffffp574

  },
  { // Entry 165
    -0x1.0000040100001p-31,
    0x1.fffffffbfffffp-1

  },
  { // Entry 166
    -0x1.ff8040007fc02p-34,
    0x1.ffffffff003fep-1

  },
  { // Entry 167
    0x1.86ef5ccdfa1b1p7,
    0x1.ffffffffddfffp281

  },
  { // Entry 168
    -0x1.0008000000801p-40,
    0x1.fffffffffdfffp-1

  },
  { // Entry 169
    -0x1.0020000000201p-42,
    0x1.ffffffffff7ffp-1

  },
  { // Entry 170
    -0x1.0040000000101p-43,
    0x1.ffffffffffbffp-1

  },
  { // Entry 171
    0x1.30fc1931f09c9p7,
    0x1.fffffffffffeep219

  },
  { // Entry 172
    -0x1.0000000000001p-51,
    0x1.ffffffffffffcp-1

  },
  { // Entry 173
    -0x1.0000000000001p-52,
    0x1.ffffffffffffep-1

  },
  { // Entry 174
    -0x1.03fe55a061c76p-1,
    0x1.342185798f6d6p-1

  },
  { // Entry 175
    -0x1.62e42fefa39f1p-2,
    0x1.6a09e667f3bccp-1

  },
  { // Entry 176
    -0x1.da391c9043a0fp-3,
    0x1.962b5f9438d25p-1

  },
  { // Entry 177
    -0x1.06fbec52082bdp-3,
    0x1.c24cd8c07de7ep-1

  },
  { // Entry 178
    -0x1.1e0a8f670c50bp-5,
    0x1.ee6e51ecc2fd7p-1

  },
  { // Entry 179
    0x1.9e548e7e3dde8p-5,
    0x1.0d47e58c84098p0

  },
  { // Entry 180
    0x1.08e0bae73ac24p-3,
    0x1.2358a222a6944p0

  },
  { // Entry 181
    0x1.9e6462187c36bp-3,
    0x1.39695eb8c91fp0

  },
  { // Entry 182
    0x1.14dd6b0af939bp-2,
    0x1.4f7a1b4eeba9cp0

  },
  { // Entry 183
    0x1.56181f19d7198p-2,
    0x1.658ad7e50e348p0

  },
  { // Entry 184
    0x1.936a82fadcd86p-2,
    0x1.7b9b947b30bf4p0

  },
  { // Entry 185
    0x1.cd45b0a9f2502p-2,
    0x1.91ac5111534ap0

  },
  { // Entry 186
    0x1.02044c9b70627p-1,
    0x1.a7bd0da775d4cp0

  },
  { // Entry 187
    0x1.1c01dad59e73p-1,
    0x1.bdcdca3d985f8p0

  },
  { // Entry 188
    0x1.34bde9b821fe3p-1,
    0x1.d3de86d3baea4p0

  },
  { // Entry 189
    0x1.4c561dbbb21dap-1,
    0x1.e9ef4369dd75p0

  },
  { // Entry 190
    0x1.62e42fefa39ebp-1,
    0x1.ffffffffffffcp0

  },
  { // Entry 191
    -0x1.62e42fefa39f1p-2,
    0x1.6a09e667f3bccp-1

  },
  { // Entry 192
    -0x1.24cfce6f80d9bp-2,
    0x1.80aa84ce72f89p-1

  },
  { // Entry 193
    -0x1.d490246defa6cp-3,
    0x1.974b2334f2346p-1

  },
  { // Entry 194
    -0x1.65d558d4ce00ap-3,
    0x1.adebc19b71703p-1

  },
  { // Entry 195
    -0x1.f991c6cb3b377p-4,
    0x1.c48c6001f0acp-1

  },
  { // Entry 196
    -0x1.31b994d3a4f8p-4,
    0x1.db2cfe686fe7dp-1

  },
  { // Entry 197
    -0x1.ccb73cdddb2b4p-6,
    0x1.f1cd9cceef23ap-1

  },
  { // Entry 198
    0x1.0b94f7c196175p-6,
    0x1.04371d9ab72fbp0

  },
  { // Entry 199
    0x1.e27076e2af2d9p-5,
    0x1.0f876ccdf6cd9p0

  },
  { // Entry 200
    0x1.986d3228180bep-4,
    0x1.1ad7bc01366b7p0

  },
  { // Entry 201
    0x1.1c898c16999f3p-3,
    0x1.26280b3476095p0

  },
  { // Entry 202
    0x1.69d4592a03622p-3,
    0x1.31785a67b5a73p0

  },
  { // Entry 203
    0x1.b44f77bcc8f56p-3,
    0x1.3cc8a99af5451p0

  },
  { // Entry 204
    0x1.fc2d6cf47cf0cp-3,
    0x1.4818f8ce34e2fp0

  },
  { // Entry 205
    0x1.20cdcd192ab65p-2,
    0x1.536948017480dp0

  },
  { // Entry 206
    0x1.426174dbd515ep-2,
    0x1.5eb99734b41ebp0

  },
  { // Entry 207
    0x1.62e42fefa39e5p-2,
    0x1.6a09e667f3bc9p0

  },
  { // Entry 208
    -0x1.269621134db92p-2,
    0x1.8p-1

  },
  { // Entry 209
    -0x1.89fa465cb93c4p-3,
    0x1.a666666666666p-1

  },
  { // Entry 210
    -0x1.af8e8210a4165p-4,
    0x1.cccccccccccccp-1

  },
  { // Entry 211
    -0x1.9ece955321b91p-6,
    0x1.f333333333332p-1

  },
  { // Entry 212
    0x1.8fb063ef2c7d1p-5,
    0x1.0ccccccccccccp0

  },
  { // Entry 213
    0x1.e27076e2af2d8p-4,
    0x1.1ffffffffffffp0

  },
  { // Entry 214
    0x1.7565011e4966ep-3,
    0x1.3333333333332p0

  },
  { // Entry 215
    0x1.f18dc41e8ef17p-3,
    0x1.4666666666665p0

  },
  { // Entry 216
    0x1.334e9e47d07fp-2,
    0x1.5999999999998p0

  },
  { // Entry 217
    0x1.6aac04146152p-2,
    0x1.6cccccccccccbp0

  },
  { // Entry 218
    0x1.9f323ecbf9846p-2,
    0x1.7fffffffffffep0

  },
  { // Entry 219
    0x0.0p0,
    0x1.0p0

  },
  { // Entry 220
    0x1.8663f793c46ccp-4,
    0x1.199999999999ap0

  },
  { // Entry 221
    0x1.7565011e4967cp-3,
    0x1.3333333333334p0

  },
  { // Entry 222
    0x1.0ca937be1b9ep-2,
    0x1.4cccccccccccep0

  },
  { // Entry 223
    0x1.588c2d9133494p-2,
    0x1.6666666666668p0

  },
  { // Entry 224
    0x1.9f323ecbf9851p-2,
    0x1.8000000000002p0

  },
  { // Entry 225
    0x1.e148a1a2726d4p-2,
    0x1.999999999999cp0

  },
  { // Entry 226
    0x1.0fae81914a994p-1,
    0x1.b333333333336p0

  },
  { // Entry 227
    0x1.2cf25fad8f1c7p-1,
    0x1.cccccccccccdp0

  },
  { // Entry 228
    0x1.48a11293d786p-1,
    0x1.e66666666666ap0

  },
  { // Entry 229
    0x1.62e42fefa39efp-1,
    0x1.0p1

  },
  { // Entry 230
    0x1.1542457337d43p6,
    0x1.0p100

  },
  { // Entry 231
    0x1.15a3de711cc55p6,
    0x1.199999999999ap100

  },
  { // Entry 232
    0x1.15fcf7f3c6f8ep6,
    0x1.3333333333334p100

  },
  { // Entry 233
    0x1.164eeeaaf5efdp6,
    0x1.4cccccccccccep100

  },
  { // Entry 234
    0x1.169ad1a0c9077p6,
    0x1.6666666666668p100

  },
  { // Entry 235
    0x1.16e177b203cdbp6,
    0x1.8000000000002p100

  },
  { // Entry 236
    0x1.17238e14da46ap6,
    0x1.999999999999cp100

  },
  { // Entry 237
    0x1.1761a2765a696p6,
    0x1.b333333333336p100

  },
  { // Entry 238
    0x1.179c2a3292f26p6,
    0x1.cccccccccccdp100

  },
  { // Entry 239
    0x1.17d387985f834p6,
    0x1.e66666666666ap100

  },
  { // Entry 240
    0x1.18080dd3171b7p6,
    0x1.0p101

  },
  { // Entry 241
    0x1.1542457337d43p7,
    0x1.0p200

  },
  { // Entry 242
    0x1.157311f22a4ccp7,
    0x1.199999999999ap200

  },
  { // Entry 243
    0x1.159f9eb37f669p7,
    0x1.3333333333334p200

  },
  { // Entry 244
    0x1.15c89a0f16e2p7,
    0x1.4cccccccccccep200

  },
  { // Entry 245
    0x1.15ee8b8a006ddp7,
    0x1.6666666666668p200

  },
  { // Entry 246
    0x1.1611de929dd0fp7,
    0x1.8000000000002p200

  },
  { // Entry 247
    0x1.1632e9c4090d6p7,
    0x1.999999999999cp200

  },
  { // Entry 248
    0x1.1651f3f4c91ecp7,
    0x1.b333333333336p200

  },
  { // Entry 249
    0x1.166f37d2e5635p7,
    0x1.cccccccccccdp200

  },
  { // Entry 250
    0x1.168ae685cbabbp7,
    0x1.e66666666666ap200

  },
  { // Entry 251
    0x1.16a529a32777dp7,
    0x1.0p201

  },
  { // Entry 252
    0x1.5a92d6d005c94p9,
    0x1.0p1000

  },
  { // Entry 253
    -0x1.0000080000555p-20,
    0x1.ffffep-1

  },
  { // Entry 254
    -0x1.0000040000155p-21,
    0x1.fffffp-1

  },
  { // Entry 255
    0x0.0p0,
    0x1.0p0

  },
  { // Entry 256
    0x1.fffff800002abp-22,
    0x1.000008p0

  },
  { // Entry 257
    0x1.fffff00000aabp-21,
    0x1.00001p0

  },
  { // Entry 258
    -0x1.00000002p-30,
    0x1.fffffff8p-1

  },
  { // Entry 259
    -0x1.00000001p-31,
    0x1.fffffffcp-1

  },
  { // Entry 260
    0x0.0p0,
    0x1.0p0

  },
  { // Entry 261
    0x1.fffffffep-32,
    0x1.00000002p0

  },
  { // Entry 262
    0x1.fffffffcp-31,
    0x1.00000004p0

  },
  { // Entry 263
    -0x1.00000000008p-40,
    0x1.fffffffffep-1

  },
  { // Entry 264
    -0x1.00000000004p-41,
    0x1.ffffffffffp-1

  },
  { // Entry 265
    0x0.0p0,
    0x1.0p0

  },
  { // Entry 266
    0x1.ffffffffff8p-42,
    0x1.00000000008p0

  },
  { // Entry 267
    0x1.ffffffffffp-41,
    0x1.0000000001p0

  },
  { // Entry 268
    -0x1.0000000000002p-50,
    0x1.ffffffffffff8p-1

  },
  { // Entry 269
    -0x1.0000000000001p-51,
    0x1.ffffffffffffcp-1

  },
  { // Entry 270
    0x0.0p0,
    0x1.0p0

  },
  { // Entry 271
    0x1.ffffffffffffep-52,
    0x1.0000000000002p0

  },
  { // Entry 272
    0x1.ffffffffffffcp-51,
    0x1.0000000000004p0

  },
  { // Entry 273
    0x1.62e42fefa39efp9,
    0x1.fffffffffffffp1023

  },
  { // Entry 274
    -0x1.74385446d71c3p9,
    0x0.0000000000001p-1022

  },
  { // Entry 275
    -0x1.62e42fefa39f4p-2,
    0x1.6a09e667f3bcbp-1

  },
  { // Entry 276
    -0x1.62e42fefa39f1p-2,
    0x1.6a09e667f3bccp-1

  },
  { // Entry 277
    -0x1.62e42fefa39eep-2,
    0x1.6a09e667f3bcdp-1

  },
  { // Entry 278
    0x1.62e42fefa39ebp-2,
    0x1.6a09e667f3bcbp0

  },
  { // Entry 279
    0x1.62e42fefa39eep-2,
    0x1.6a09e667f3bccp0

  },
  { // Entry 280
    0x1.62e42fefa39fp-2,
    0x1.6a09e667f3bcdp0

  },
  { // Entry 281
    -0x1.62e42fefa39fp-1,
    0x1.fffffffffffffp-2

  },
  { // Entry 282
    -0x1.62e42fefa39efp-1,
    0x1.0p-1

  },
  { // Entry 283
    -0x1.62e42fefa39edp-1,
    0x1.0000000000001p-1

  },
  { // Entry 284
    -0x1.269621134db95p-2,
    0x1.7ffffffffffffp-1

  },
  { // Entry 285
    -0x1.269621134db92p-2,
    0x1.8p-1

  },
  { // Entry 286
    -0x1.269621134db9p-2,
    0x1.8000000000001p-1

  },
  { // Entry 287
    0x1.9f323ecbf9849p-2,
    0x1.7ffffffffffffp0

  },
  { // Entry 288
    0x1.9f323ecbf984cp-2,
    0x1.8p0

  },
  { // Entry 289
    0x1.9f323ecbf984ep-2,
    0x1.8000000000001p0

  },
  { // Entry 290
    0x1.54e3c0b10a364p-9,
    0x1.00aaaaaaaaaaap0

  },
  { // Entry 291
    0x1.54e3c0b10a563p-9,
    0x1.00aaaaaaaaaabp0

  },
  { // Entry 292
    0x1.54e3c0b10a762p-9,
    0x1.00aaaaaaaaaacp0

  },
  { // Entry 293
    0x1.62e42fefa39efp0,
    0x1.fffffffffffffp1

  },
  { // Entry 294
    0x1.62e42fefa39efp0,
    0x1.0p2

  },
  { // Entry 295
    0x1.62e42fefa39fp0,
    0x1.0000000000001p2

  },
  { // Entry 296
    0x1.62e42fefa39eep-1,
    0x1.fffffffffffffp0

  },
  { // Entry 297
    0x1.62e42fefa39efp-1,
    0x1.0p1

  },
  { // Entry 298
    0x1.62e42fefa39f1p-1,
    0x1.0000000000001p1

  },
  { // Entry 299
    -0x1.0p-53,
    0x1.fffffffffffffp-1

  },
  { // Entry 300
    0x0.0p0,
    0x1.0p0

  },
  { // Entry 301
    0x1.fffffffffffffp-53,
    0x1.0000000000001p0

  },
  { // Entry 302
    -0x1.62e42fefa39fp-1,
    0x1.fffffffffffffp-2

  },
  { // Entry 303
    -0x1.62e42fefa39efp-1,
    0x1.0p-1

  },
  { // Entry 304
    -0x1.62e42fefa39edp-1,
    0x1.0000000000001p-1

  },
  { // Entry 305
    -0x1.62e42fefa39fp0,
    0x1.fffffffffffffp-3

  },
  { // Entry 306
    -0x1.62e42fefa39efp0,
    0x1.0p-2

  },
  { // Entry 307
    -0x1.62e42fefa39eep0,
    0x1.0000000000001p-2

  },
  { // Entry 308
    -0x1.0a2b23f3bab74p1,
    0x1.fffffffffffffp-4

  },
  { // Entry 309
    -0x1.0a2b23f3bab73p1,
    0x1.0p-3

  },
  { // Entry 310
    -0x1.0a2b23f3bab73p1,
    0x1.0000000000001p-3

  },
  { // Entry 311
    -0x1.62e42fefa39efp1,
    0x1.fffffffffffffp-5

  },
  { // Entry 312
    -0x1.62e42fefa39efp1,
    0x1.0p-4

  },
  { // Entry 313
    -0x1.62e42fefa39efp1,
    0x1.0000000000001p-4

  },
  { // Entry 314
    -0x1.bb9d3beb8c86bp1,
    0x1.fffffffffffffp-6

  },
  { // Entry 315
    -0x1.bb9d3beb8c86bp1,
    0x1.0p-5

  },
  { // Entry 316
    -0x1.bb9d3beb8c86bp1,
    0x1.0000000000001p-5

  },
  { // Entry 317
    -0x1.0a2b23f3bab74p2,
    0x1.fffffffffffffp-7

  },
  { // Entry 318
    -0x1.0a2b23f3bab73p2,
    0x1.0p-6

  },
  { // Entry 319
    -0x1.0a2b23f3bab73p2,
    0x1.0000000000001p-6

  },
  { // Entry 320
    -0x1.3687a9f1af2b1p2,
    0x1.fffffffffffffp-8

  },
  { // Entry 321
    -0x1.3687a9f1af2b1p2,
    0x1.0p-7

  },
  { // Entry 322
    -0x1.3687a9f1af2b1p2,
    0x1.0000000000001p-7

  },
  { // Entry 323
    -0x1.62e42fefa39efp2,
    0x1.fffffffffffffp-9

  },
  { // Entry 324
    -0x1.62e42fefa39efp2,
    0x1.0p-8

  },
  { // Entry 325
    -0x1.62e42fefa39efp2,
    0x1.0000000000001p-8

  },
  { // Entry 326
    -0x1.8f40b5ed9812dp2,
    0x1.fffffffffffffp-10

  },
  { // Entry 327
    -0x1.8f40b5ed9812dp2,
    0x1.0p-9

  },
  { // Entry 328
    -0x1.8f40b5ed9812dp2,
    0x1.0000000000001p-9

  },
  { // Entry 329
    -0x1.bb9d3beb8c86bp2,
    0x1.fffffffffffffp-11

  },
  { // Entry 330
    -0x1.bb9d3beb8c86bp2,
    0x1.0p-10

  },
  { // Entry 331
    -0x1.bb9d3beb8c86bp2,
    0x1.0000000000001p-10

  },
  { // Entry 332
    -0x1.205966f2b4f12p3,
    0x1.fffffffffffffp-14

  },
  { // Entry 333
    -0x1.205966f2b4f12p3,
    0x1.0p-13

  },
  { // Entry 334
    -0x1.205966f2b4f12p3,
    0x1.0000000000001p-13

  },
  { // Entry 335
    -0x1.205966f2b4f12p3,
    0x1.fffffffffffffp-14

  },
  { // Entry 336
    -0x1.205966f2b4f12p3,
    0x1.0p-13

  },
  { // Entry 337
    -0x1.205966f2b4f12p3,
    0x1.0000000000001p-13

  },
  { // Entry 338
    -0x1.0a2b23f3bab74p1,
    0x1.fffffffffffffp-4

  },
  { // Entry 339
    -0x1.0a2b23f3bab73p1,
    0x1.0p-3

  },
  { // Entry 340
    -0x1.0a2b23f3bab73p1,
    0x1.0000000000001p-3

  },
  { // Entry 341
    -0x1.1178e8227e48p-3,
    0x1.bffffffffffffp-1

  },
  { // Entry 342
    -0x1.1178e8227e47cp-3,
    0x1.cp-1

  },
  { // Entry 343
    -0x1.1178e8227e477p-3,
    0x1.c000000000001p-1

  },
  { // Entry 344
    -0x1.62e42fefa39efp1,
    0x1.fffffffffffffp-5

  },
  { // Entry 345
    -0x1.62e42fefa39efp1,
    0x1.0p-4

  },
  { // Entry 346
    -0x1.62e42fefa39efp1,
    0x1.0000000000001p-4

  },
  { // Entry 347
    -0x1.08598b59e3a0fp-4,
    0x1.dffffffffffffp-1

  },
  { // Entry 348
    -0x1.08598b59e3a07p-4,
    0x1.ep-1

  },
  { // Entry 349
    -0x1.08598b59e39fep-4,
    0x1.e000000000001p-1

  },
  { // Entry 350
    -0x1.bb9d3beb8c86bp1,
    0x1.fffffffffffffp-6

  },
  { // Entry 351
    -0x1.bb9d3beb8c86bp1,
    0x1.0p-5

  },
  { // Entry 352
    -0x1.bb9d3beb8c86bp1,
    0x1.0000000000001p-5

  },
  { // Entry 353
    -0x1.0415d89e74455p-5,
    0x1.effffffffffffp-1

  },
  { // Entry 354
    -0x1.0415d89e74444p-5,
    0x1.fp-1

  },
  { // Entry 355
    -0x1.0415d89e74434p-5,
    0x1.f000000000001p-1

  },
  { // Entry 356
    -0x1.0a2b23f3bab74p2,
    0x1.fffffffffffffp-7

  },
  { // Entry 357
    -0x1.0a2b23f3bab73p2,
    0x1.0p-6

  },
  { // Entry 358
    -0x1.0a2b23f3bab73p2,
    0x1.0000000000001p-6

  },
  { // Entry 359
    -0x1.0205658935868p-6,
    0x1.f7fffffffffffp-1

  },
  { // Entry 360
    -0x1.0205658935847p-6,
    0x1.f8p-1

  },
  { // Entry 361
    -0x1.0205658935827p-6,
    0x1.f800000000001p-1

  },
  { // Entry 362
    -0x1.3687a9f1af2b1p2,
    0x1.fffffffffffffp-8

  },
  { // Entry 363
    -0x1.3687a9f1af2b1p2,
    0x1.0p-7

  },
  { // Entry 364
    -0x1.3687a9f1af2b1p2,
    0x1.0000000000001p-7

  },
  { // Entry 365
    -0x1.010157588deb2p-7,
    0x1.fbfffffffffffp-1

  },
  { // Entry 366
    -0x1.010157588de71p-7,
    0x1.fcp-1

  },
  { // Entry 367
    -0x1.010157588de31p-7,
    0x1.fc00000000001p-1

  },
  { // Entry 368
    -0x1.62e42fefa39efp2,
    0x1.fffffffffffffp-9

  },
  { // Entry 369
    -0x1.62e42fefa39efp2,
    0x1.0p-8

  },
  { // Entry 370
    -0x1.62e42fefa39efp2,
    0x1.0000000000001p-8

  },
  { // Entry 371
    -0x1.0080559588bb6p-8,
    0x1.fdfffffffffffp-1

  },
  { // Entry 372
    -0x1.0080559588b35p-8,
    0x1.fep-1

  },
  { // Entry 373
    -0x1.0080559588ab5p-8,
    0x1.fe00000000001p-1

  },
  { // Entry 374
    -0x1.8f40b5ed9812dp2,
    0x1.fffffffffffffp-10

  },
  { // Entry 375
    -0x1.8f40b5ed9812dp2,
    0x1.0p-9

  },
  { // Entry 376
    -0x1.8f40b5ed9812dp2,
    0x1.0000000000001p-9

  },
  { // Entry 377
    -0x1.0040155d5899ep-9,
    0x1.fefffffffffffp-1

  },
  { // Entry 378
    -0x1.0040155d5889ep-9,
    0x1.ffp-1

  },
  { // Entry 379
    -0x1.0040155d5879dp-9,
    0x1.ff00000000001p-1

  },
  { // Entry 380
    -0x1.bb9d3beb8c86bp2,
    0x1.fffffffffffffp-11

  },
  { // Entry 381
    -0x1.bb9d3beb8c86bp2,
    0x1.0p-10

  },
  { // Entry 382
    -0x1.bb9d3beb8c86bp2,
    0x1.0000000000001p-10

  },
  { // Entry 383
    -0x1.0020055655a8ap-10,
    0x1.ff7ffffffffffp-1

  },
  { // Entry 384
    -0x1.0020055655889p-10,
    0x1.ff8p-1

  },
  { // Entry 385
    -0x1.0020055655689p-10,
    0x1.ff80000000001p-1

  },
  { // Entry 386
    -0x1.205966f2b4f12p3,
    0x1.fffffffffffffp-14

  },
  { // Entry 387
    -0x1.205966f2b4f12p3,
    0x1.0p-13

  },
  { // Entry 388
    -0x1.205966f2b4f12p3,
    0x1.0000000000001p-13

  },
  { // Entry 389
    -0x1.0004001556d56p-13,
    0x1.ffeffffffffffp-1

  },
  { // Entry 390
    -0x1.0004001555d56p-13,
    0x1.fffp-1

  },
  { // Entry 391
    -0x1.0004001554d55p-13,
    0x1.fff0000000001p-1

  },
  { // Entry 392
    0x1.62e42fefa39efp9,
    0x1.fffffffffffffp1023

  },
  { // Entry 393
    0x1.62e42fefa39efp9,
    0x1.ffffffffffffep1023

  },
  { // Entry 394
    0x1.250d048e7a1bdp0,
    0x1.921fb54442d18p1

  },
  { // Entry 395
    0x1.ce6bb25aa1315p-2,
    0x1.921fb54442d18p0

  },
  { // Entry 396
    0x1.fffffffffffffp-53,
    0x1.0000000000001p0

  },
  { // Entry 397
    0x0.0p0,
    0x1.0p0

  },
  { // Entry 398
    -0x1.0p-53,
    0x1.fffffffffffffp-1

  },
  { // Entry 399
    -0x1.eeb95b094c193p-3,
    0x1.921fb54442d18p-1

  },
  { // Entry 400
    -0x1.6232bdd7abcd2p9,
    0x1.0000000000001p-1022

  },
  { // Entry 401
    -0x1.6232bdd7abcd2p9,
    0x1.0p-1022

  },
  { // Entry 402
    -0x1.6232bdd7abcd2p9,
    0x0.fffffffffffffp-1022

  },
  { // Entry 403
    -0x1.6232bdd7abcd2p9,
    0x0.ffffffffffffep-1022

  },
  { // Entry 404
    -0x1.73df9b3adb335p9,
    0x0.0000000000002p-1022

  },
  { // Entry 405
    -0x1.74385446d71c3p9,
    0x0.0000000000001p-1022

  },
  { // Entry 406
    0x1.0p0,
    0x1.5bf0a8b145769p1
  },
  { // Entry 407
    0x0.0p0,
    0x1.0p0
  },
  { // Entry 408
    -0x1.62e42fefa39efp-1,
    0x1.0p-1
  },
  { // Entry 409
    0x1.ce6bb25aa1315p-2,
    0x1.921fb54442d18p0
  },
  { // Entry 410
    0x1.250d048e7a1bdp0,
    0x1.921fb54442d18p1
  },
  { // Entry 411
    0x1.8cd99441787dp0,
    0x1.2d97c7f3321d2p2
  },
  { // Entry 412
    0x1.d67f1c864beb4p0,
    0x1.921fb54442d18p2
  },
};

static TestCase g_atan_data[] = {
  { // Entry 0
    0x1.37e1637253389p0,
    0x1.5bf0a8b145769p1
  },
  { // Entry 1
    -0x1.37e1637253389p0,
    -0x1.5bf0a8b145769p1
  },
  { // Entry 2
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 3
    -0x0.0p0,
    -0x0.0p0
  },
  { // Entry 4
    0x1.921fb54442d18p-1,
    0x1.0p0
  },
  { // Entry 5
    -0x1.921fb54442d18p-1,
    -0x1.0p0
  },
  { // Entry 6
    -0x1.dac670561bb4fp-2,
    -0x1.0p-1
  },
  { // Entry 7
    0x1.dac670561bb4fp-2,
    0x1.0p-1
  },
  { // Entry 8
    -0x1.69b8154baf42ep0,
    -0x1.921fb54442d18p2
  },
  { // Entry 9
    -0x1.5c97d37d98aa4p0,
    -0x1.2d97c7f3321d2p2
  },
  { // Entry 10
    -0x1.433b8a322ddd2p0,
    -0x1.921fb54442d18p1
  },
  { // Entry 11
    -0x1.00fe987ed02ffp0,
    -0x1.921fb54442d18p0
  },
  { // Entry 12
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 13
    0x1.00fe987ed02ffp0,
    0x1.921fb54442d18p0
  },
  { // Entry 14
    0x1.433b8a322ddd2p0,
    0x1.921fb54442d18p1
  },
  { // Entry 15
    0x1.5c97d37d98aa4p0,
    0x1.2d97c7f3321d2p2
  },
  { // Entry 16
    0x1.69b8154baf42ep0,
    0x1.921fb54442d18p2
  },
};

static TestCase g_tanh_data[] = {
  { // Entry 0
    0x1.fb8f76b1e2ab6p-1,
    0x1.5bf0a8b145769p1
  },
  { // Entry 1
    -0x1.fb8f76b1e2ab6p-1,
    -0x1.5bf0a8b145769p1
  },
  { // Entry 2
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 3
    -0x0.0p0,
    -0x0.0p0
  },
  { // Entry 4
    0x1.85efab514f394p-1,
    0x1.0p0
  },
  { // Entry 5
    -0x1.85efab514f394p-1,
    -0x1.0p0
  },
  { // Entry 6
    -0x1.d9353d7568af3p-2,
    -0x1.0p-1
  },
  { // Entry 7
    0x1.d9353d7568af3p-2,
    0x1.0p-1
  },
  { // Entry 8
    -0x1.ffff15f81f9abp-1,
    -0x1.921fb54442d18p2
  },
  { // Entry 9
    -0x1.ffead8c8b7e1ep-1,
    -0x1.2d97c7f3321d2p2
  },
  { // Entry 10
    -0x1.fe175fa29281p-1,
    -0x1.921fb54442d18p1
  },
  { // Entry 11
    -0x1.d594fdae482bap-1,
    -0x1.921fb54442d18p0
  },
  { // Entry 12
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 13
    0x1.d594fdae482bap-1,
    0x1.921fb54442d18p0
  },
  { // Entry 14
    0x1.fe175fa29281p-1,
    0x1.921fb54442d18p1
  },
  { // Entry 15
    0x1.ffead8c8b7e1ep-1,
    0x1.2d97c7f3321d2p2
  },
  { // Entry 16
    0x1.ffff15f81f9abp-1,
    0x1.921fb54442d18p2
  },
};

static TestCase g_acos_data[] = {
  { // Entry 0
    0x1.921fb54442d18p0,
    0x0.0p0
  },
  { // Entry 1
    0x1.921fb54442d18p0,
    -0x0.0p0
  },
  { // Entry 2
    0x0.0p0,
    0x1.0p0
  },
  { // Entry 3
    0x1.921fb54442d18p1,
    -0x1.0p0
  },
  { // Entry 4
    0x1.0c152382d7366p1,
    -0x1.0p-1
  },
  { // Entry 5
    0x1.0c152382d7366p0,
    0x1.0p-1
  },
  { // Entry 6
    0x1.921fb54442d18p0,
    0x0.0p0
  },
};

static TestCase g_sqrt_data[] = {
  { // Entry 0
    0x1.a61298e1e069cp0,
    0x1.5bf0a8b145769p1
  },
  { // Entry 1
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 2
    -0x0.0p0,
    -0x0.0p0
  },
  { // Entry 3
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 4
    0x1.6a09e667f3bcdp-1,
    0x1.0p-1
  },
  { // Entry 5
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 6
    0x1.40d931ff62705p0,
    0x1.921fb54442d18p0
  },
  { // Entry 7
    0x1.c5bf891b4ef6ap0,
    0x1.921fb54442d18p1
  },
  { // Entry 8
    0x1.15dce5d1822ccp1,
    0x1.2d97c7f3321d2p2
  },
  { // Entry 9
    0x1.40d931ff62705p1,
    0x1.921fb54442d18p2
  },
};

static TestCase g_cosh_data[] = {
  { // Entry 0
    0x1.e70c4a4f41685p2,
    0x1.5bf0a8b145769p1
  },
  { // Entry 1
    0x1.e70c4a4f41685p2,
    -0x1.5bf0a8b145769p1
  },
  { // Entry 2
    0x1.0p0,
    0x0.0p0
  },
  { // Entry 3
    0x1.0p0,
    -0x0.0p0
  },
  { // Entry 4
    0x1.8b07551d9f551p0,
    0x1.0p0
  },
  { // Entry 5
    0x1.8b07551d9f551p0,
    -0x1.0p0
  },
  { // Entry 6
    0x1.20ac1862ae8dp0,
    -0x1.0p-1
  },
  { // Entry 7
    0x1.20ac1862ae8dp0,
    0x1.0p-1
  },
  { // Entry 8
    0x1.0bbf2bc2b69c6p8,
    -0x1.921fb54442d18p2
  },
  { // Entry 9
    0x1.bd4e9aa3e9cdbp5,
    -0x1.2d97c7f3321d2p2
  },
  { // Entry 10
    0x1.72f147fee4p3,
    -0x1.921fb54442d18p1
  },
  { // Entry 11
    0x1.412cc2a8d4e9ep1,
    -0x1.921fb54442d18p0
  },
  { // Entry 12
    0x1.0p0,
    0x0.0p0
  },
  { // Entry 13
    0x1.412cc2a8d4e9ep1,
    0x1.921fb54442d18p0
  },
  { // Entry 14
    0x1.72f147fee4p3,
    0x1.921fb54442d18p1
  },
  { // Entry 15
    0x1.bd4e9aa3e9cdbp5,
    0x1.2d97c7f3321d2p2
  },
  { // Entry 16
    0x1.0bbf2bc2b69c6p8,
    0x1.921fb54442d18p2
  },
};

static TestCase g_abs_data[] = {
  { // Entry 0
    0x1.5bf0a8b145769p1,
    0x1.5bf0a8b145769p1
  },
  { // Entry 1
    0x1.5bf0a8b145769p1,
    -0x1.5bf0a8b145769p1
  },
  { // Entry 2
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 3
    0x0.0p0,
    -0x0.0p0
  },
  { // Entry 4
    0x1.0p0,
    0x1.0p0
  },
  { // Entry 5
    0x1.0p0,
    -0x1.0p0
  },
  { // Entry 6
    0x1.0p-1,
    -0x1.0p-1
  },
  { // Entry 7
    0x1.0p-1,
    0x1.0p-1
  },
  { // Entry 8
    0x1.921fb54442d18p2,
    -0x1.921fb54442d18p2
  },
  { // Entry 9
    0x1.2d97c7f3321d2p2,
    -0x1.2d97c7f3321d2p2
  },
  { // Entry 10
    0x1.921fb54442d18p1,
    -0x1.921fb54442d18p1
  },
  { // Entry 11
    0x1.921fb54442d18p0,
    -0x1.921fb54442d18p0
  },
  { // Entry 12
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 13
    0x1.921fb54442d18p0,
    0x1.921fb54442d18p0
  },
  { // Entry 14
    0x1.921fb54442d18p1,
    0x1.921fb54442d18p1
  },
  { // Entry 15
    0x1.2d97c7f3321d2p2,
    0x1.2d97c7f3321d2p2
  },
  { // Entry 16
    0x1.921fb54442d18p2,
    0x1.921fb54442d18p2
  },
};

static TestCase g_exp_data[] = {
  { // Entry 0
    0x1.0000000000002p0,
    0x1.ffffffffffffep-52

  },
  { // Entry 1
    0x1.000000000012p0,
    0x1.1ffffffffff5ep-44

  },
  { // Entry 2
    0x1.000000000208p0,
    0x1.03fffffffef7fp-39

  },
  { // Entry 3
    0x1.00000000c1b59p0,
    0x1.836b2fff6d6cbp-33

  },
  { // Entry 4
    0x1.000000033d397p0,
    0x1.9e9cbbfd6080bp-31

  },
  { // Entry 5
    0x1.003af6c37c1d3p0,
    0x1.d77fd13d27fffp-11

  },
  { // Entry 6
    0x1.016b4df3299d8p0,
    0x1.6a4d1af9cc989p-8

  },
  { // Entry 7
    0x1.0ca4a41663fep0,
    0x1.8ae823850230bp-5

  },
  { // Entry 8
    0x1.1538ea18a4585p0,
    0x1.46370d915991bp-4

  },
  { // Entry 9
    0x1.47408cb9583cep0,
    0x1.f6e4c3ced7c72p-3

  },
  { // Entry 10
    0x1.27c2e4bc1ee7p1,
    0x1.accfbe46b4efp-1

  },
  { // Entry 11
    0x1.557d4acd7e556p2,
    0x1.aca7ae8da5a7bp0

  },
  { // Entry 12
    0x1.91a8dff540ff7p2,
    0x1.d6336a88077aap0

  },
  { // Entry 13
    0x1.8ede492d96072p3,
    0x1.42ee3c7dc4946p1

  },
  { // Entry 14
    0x1.ac50b409c8aeep8,
    0x1.83d4bcdebb3f4p2

  },
  { // Entry 15
    0x1.f3e558cf4de54p-23,
    -0x1.e8bdbfcd9144ep3

  },
  { // Entry 16
    0x1.951c6dc5d24e2p-9,
    -0x1.71e0b869b5e79p2

  },
  { // Entry 17
    0x1.1064b2c103ddap-3,
    -0x1.02393d5976769p1

  },
  { // Entry 18
    0x1.3ef1e9b3a81c8p-2,
    -0x1.2a9cad9998262p0

  },
  { // Entry 19
    0x1.534d4de870713p-3,
    -0x1.cc37ef7de7501p0

  },
  { // Entry 20
    0x1.2217147b85eaap-1,
    -0x1.22e24fa3d5cf9p-1

  },
  { // Entry 21
    0x1.9403fd0ee51c8p-2,
    -0x1.dc2b5df1f7d3dp-1

  },
  { // Entry 22
    0x1.baded30cbf1c4p-1,
    -0x1.290ea09e36479p-3

  },
  { // Entry 23
    0x1.ffe5d0bb7eabfp-1,
    -0x1.a2fefefd580dfp-13

  },
  { // Entry 24
    0x1.ffffff84b39c5p-1,
    -0x1.ed318efb627eap-27

  },
  { // Entry 25
    0x1.fffffffad0ae6p-1,
    -0x1.4bd46601ae1efp-31

  },
  { // Entry 26
    0x1.ffffffffff7p-1,
    -0x1.2000000000288p-42

  },
  { // Entry 27
    0x1.fffffffffffdp-1,
    -0x1.8000000000012p-48

  },
  { // Entry 28
    0x1.ffffffffffffcp-1,
    -0x1.0000000000001p-51

  },
  { // Entry 29
    0x1.0000000000001p0,
    0x1.fffffffffffffp-53

  },
  { // Entry 30
    0x1.000000000002p0,
    0x1.fffffffffffep-48

  },
  { // Entry 31
    0x1.000000017ffe8p0,
    0x1.7ffe7ffee0024p-32

  },
  { // Entry 32
    0x1.0000000180018p0,
    0x1.80017ffedffdcp-32

  },
  { // Entry 33
    0x1.00000075e9f64p0,
    0x1.d7a7d893609e5p-26

  },
  { // Entry 34
    0x1.0006e83736f8cp0,
    0x1.ba07d73250de7p-14

  },
  { // Entry 35
    0x1.de7cd6751029ap16,
    0x1.76e7e5d7b6eacp3

  },
  { // Entry 36
    0x1.1d71965f516aep19,
    0x1.a8ead058bc6b8p3

  },
  { // Entry 37
    0x1.a8c02e974c315p25,
    0x1.1d5c2daebe367p4

  },
  { // Entry 38
    0x1.b890ca8637ae2p40,
    0x1.c44ce0d716a1ap4

  },
  { // Entry 39
    0x1.f03f56a88b5d8p-1,
    -0x1.0000000000001p-5

  },
  { // Entry 40
    0x1.ff003ff556aa8p-1,
    -0x1.000000000000ap-9

  },
  { // Entry 41
    0x1.8ebef9eac81fcp-1,
    -0x1.0000000000025p-2

  },
  { // Entry 42
    0x1.fc03fd56a469cp-1,
    -0x1.0000000171051p-7

  },
  { // Entry 43
    0x1.78b5612dbee01p-2,
    -0x1.0000018p0

  },
  { // Entry 44
    0x1.969c870ea9f5ap-93,
    -0x1.000001e3ep6

  },
  { // Entry 45
    0x1.78af867bc511ep-2,
    -0x1.0003fcp0

  },
  { // Entry 46
    0x1.fffffffffefffp-1,
    -0x1.00080000004p-41

  },
  { // Entry 47
    0x1.789214093c81ep-2,
    -0x1.0018p0

  },
  { // Entry 48
    0x1.8e298e52fcce8p-1,
    -0x1.018p-2

  },
  { // Entry 49
    0x1.aa6ffb0ba51ap-24,
    -0x1.0201p4

  },
  { // Entry 50
    0x1.dd7a46b8d85d6p-376,
    -0x1.04p8

  },
  { // Entry 51
    0x1.33e96ca3bcf3ep-1,
    -0x1.045b94872413p-1

  },
  { // Entry 52
    0x1.ef2f652dc2606p-1,
    -0x1.118e05cfa3e8p-5

  },
  { // Entry 53
    0x1.a5d2ce977bbe2p-7,
    -0x1.169156e17b9b9p2

  },
  { // Entry 54
    0x1.a0207cf4190f7p-7,
    -0x1.17701b3bf0502p2

  },
  { // Entry 55
    0x1.fb97e1ff8cbbap-1,
    -0x1.1b4p-7

  },
  { // Entry 56
    0x1.bcff09ae30c46p-1,
    -0x1.1f4p-3

  },
  { // Entry 57
    0x1.fee70cf5e86efp-833,
    -0x1.2059ad42c3cf7p9

  },
  { // Entry 58
    0x1.7ccd3f3340746p-1,
    -0x1.2f269d2ca38d2p-2

  },
  { // Entry 59
    0x1.09cdb36e977bcp-14,
    -0x1.3553cf1828b0bp3

  },
  { // Entry 60
    0x1.f9dc5cd546d9cp-449,
    -0x1.368ac083128fcp8

  },
  { // Entry 61
    0x1.f1c53c907f6f5p-15,
    -0x1.376e8f679ae16p3

  },
  { // Entry 62
    0x1.f24595954e662p-8,
    -0x1.3844f67495f7dp2

  },
  { // Entry 63
    0x1.fb56521290a22p-905,
    -0x1.394e9e1b089d7p9

  },
  { // Entry 64
    0x1.dc1658ff6e07p-114,
    -0x1.3997ea51e5271p6

  },
  { // Entry 65
    0x1.ec8b3c090f8b9p-1,
    -0x1.3d5de560e1906p-5

  },
  { // Entry 66
    0x1.af411bf985221p-951,
    -0x1.4954aa552a96p9

  },
  { // Entry 67
    0x1.e1b71d238d508p-957,
    -0x1.4b5ad6a9ad6adp9

  },
  { // Entry 68
    0x1.cc666a3519cap-973,
    -0x1.50ec32686a834p9

  },
  { // Entry 69
    0x1.22462bd5f72b1p-976,
    -0x1.52316872b0222p9

  },
  { // Entry 70
    0x1.c1ebc18610302p-981,
    -0x1.53b4ed3b4ec77p9

  },
  { // Entry 71
    0x1.8d015bcaf9e19p-981,
    -0x1.53c4f13c4f079p9

  },
  { // Entry 72
    0x1.fd5835460ed49p-989,
    -0x1.566ad4a41becp9

  },
  { // Entry 73
    0x1.58733fbc088fp-994,
    -0x1.585883e7b3b38p9

  },
  { // Entry 74
    0x1.b052090690482p-32,
    -0x1.5a81ecf68beccp4

  },
  { // Entry 75
    0x1.df6154bc644dbp-1015,
    -0x1.5f75629af4eb7p9

  },
  { // Entry 76
    0x0.ffa586d1b841ap-1022,
    -0x1.6232eb1c432f2p9

  },
  { // Entry 77
    0x0.b12f6ea63a89fp-1022,
    -0x1.6261d7dbf48ccp9

  },
  { // Entry 78
    0x1.d572d24973be2p-1,
    -0x1.6364b8f747e32p-4

  },
  { // Entry 79
    0x1.ee4dd79224578p-9,
    -0x1.6524796b40895p2

  },
  { // Entry 80
    0x1.f660c59b2945p-5,
    -0x1.6551daae369cep1

  },
  { // Entry 81
    0x0.0003540e0893ep-1022,
    -0x1.672463f141607p9

  },
  { // Entry 82
    0x1.d4e7fc331b732p-1,
    -0x1.6820c99c1dc16p-4

  },
  { // Entry 83
    0x1.fcc448acf4765p-528,
    -0x1.6d4b7e82c3f02p8

  },
  { // Entry 84
    0x1.7fff5a1bffcb2p-529,
    -0x1.6e44f765fda76p8

  },
  { // Entry 85
    0x1.fa4ecd8ae5718p-1,
    -0x1.6e56e725b8304p-7

  },
  { // Entry 86
    0x1.e8d2dafd017cep-3,
    -0x1.6ecp0

  },
  { // Entry 87
    0x0.0000000001215p-1022,
    -0x1.7p9

  },
  { // Entry 88
    0x0.000000000000ep-1022,
    -0x1.72e42p9

  },
  { // Entry 89
    0x1.a349b2329c678p-273,
    -0x1.7978bac71121cp7

  },
  { // Entry 90
    0x1.d2a6d5ea995c2p-1,
    -0x1.7bde79e0f970cp-4

  },
  { // Entry 91
    0x1.b776dc64c76d2p-3,
    -0x1.8ap0

  },
  { // Entry 92
    0x1.4446fcf7da68ap-72,
    -0x1.8d5c84f0bac8cp5

  },
  { // Entry 93
    0x1.e72a0b68bb82cp-73,
    -0x1.8fa68b444723p5

  },
  { // Entry 94
    0x1.fc33d5e1ca6dfp-75,
    -0x1.9a671693b946bp5

  },
  { // Entry 95
    0x1.e6981fd6ef79cp-1,
    -0x1.a0ec89f897a75p-5

  },
  { // Entry 96
    0x1.f2b9d3b6cf227p-1,
    -0x1.ae6p-6

  },
  { // Entry 97
    0x1.e5034c8d191bcp-1,
    -0x1.bb9706e2f1bb4p-5

  },
  { // Entry 98
    0x1.f24dc90f9fbecp-1,
    -0x1.bc3edad2e1efbp-6

  },
  { // Entry 99
    0x1.71f237d64a6ecp-11,
    -0x1.d0697edbe0052p2

  },
  { // Entry 100
    0x1.f12c0d3addaa4p-1,
    -0x1.e18p-6

  },
  { // Entry 101
    0x1.f110dd9073d72p-1,
    -0x1.e5p-6

  },
  { // Entry 102
    0x1.f3aa7a860574cp-705,
    -0x1.e7fffffffffffp8

  },
  { // Entry 103
    0x1.f0f898d55117p-1,
    -0x1.e82p-6

  },
  { // Entry 104
    0x1.ff814fff18dadp-1,
    -0x1.fafebfafebfbp-11

  },
  { // Entry 105
    0x1.ffffffff01c07p-1,
    -0x1.fc7f1fc7f2p-34

  },
  { // Entry 106
    0x1.7907d4148bd92p-2,
    -0x1.ff8ffffffffffp-1

  },
  { // Entry 107
    0x1.ffffffffffff8p-1,
    -0x1.ff8ffffffffffp-51

  },
  { // Entry 108
    0x1.e0fbb03a0c27ep-1,
    -0x1.ffeffffffffffp-5

  },
  { // Entry 109
    0x1.ffffffffff001p-1,
    -0x1.ffeffffffffffp-42

  },
  { // Entry 110
    0x1.ffffffffffp-1,
    -0x1.fff00000008p-42

  },
  { // Entry 111
    0x1.0p0,
    -0x1.fff8e61eadd48p-101

  },
  { // Entry 112
    0x1.fffffffffc001p-1,
    -0x1.fffbfffffffffp-40

  },
  { // Entry 113
    0x1.fffffffffcp-1,
    -0x1.fffff1fffffffp-40

  },
  { // Entry 114
    0x1.ff800ffeaac01p-1,
    -0x1.ffffffffffda5p-11

  },
  { // Entry 115
    0x1.e355bbaee8d86p-24,
    -0x1.fffffffffff7dp3

  },
  { // Entry 116
    0x1.78b56362cef3ep-2,
    -0x1.ffffffffffff8p-1

  },
  { // Entry 117
    0x1.0p0,
    -0x0.ffffffffffffep-1022

  },
  { // Entry 118
    0x1.42eb9f39afbacp-185,
    -0x1.ffffffffffffep6

  },
  { // Entry 119
    0x1.0p0,
    0x1.0p-53

  },
  { // Entry 120
    0x1.95e54c5dd4218p184,
    0x1.0p7

  },
  { // Entry 121
    0x1.9476504ba8857p738,
    0x1.0000000000001p9

  },
  { // Entry 122
    0x1.0100802ab5578p0,
    0x1.0000000000003p-8

  },
  { // Entry 123
    0x1.0000004000001p0,
    0x1.0000000000007p-26

  },
  { // Entry 124
    0x1.d8e64b8f26c12p2,
    0x1.000000008p1

  },
  { // Entry 125
    0x1.749ea7e015bc2p11,
    0x1.00000001p3

  },
  { // Entry 126
    0x1.0f2ebd2c65d9ap23,
    0x1.00000002p4

  },
  { // Entry 127
    0x1.0f2ec1473afeap23,
    0x1.0000004p4

  },
  { // Entry 128
    0x1.0000000001p0,
    0x1.0008p-40

  },
  { // Entry 129
    0x1.00000000008p0,
    0x1.001p-41

  },
  { // Entry 130
    0x1.0000000000001p0,
    0x1.001p-53

  },
  { // Entry 131
    0x1.a8bf81a597eddp184,
    0x1.00173eab3623ap7

  },
  { // Entry 132
    0x1.75e54175aabb4p11,
    0x1.001cp3

  },
  { // Entry 133
    0x1.60008fbed6c52p1,
    0x1.02f8af8af8affp0

  },
  { // Entry 134
    0x1.04184bb80ff44p0,
    0x1.04p-6

  },
  { // Entry 135
    0x1.0000084000221p0,
    0x1.08p-21

  },
  { // Entry 136
    0x1.2338ab9b7432p0,
    0x1.08000001d0fc8p-3

  },
  { // Entry 137
    0x1.acc91bfa7c54cp0,
    0x1.0814419d6a65ap-1

  },
  { // Entry 138
    0x1.a4e4693413b9ap400,
    0x1.15c18de877563p8

  },
  { // Entry 139
    0x1.0000000000004p0,
    0x1.1ffffffffffffp-50

  },
  { // Entry 140
    0x1.eaa521edf1bc2p846,
    0x1.2586ca9cf411bp9

  },
  { // Entry 141
    0x1.fca9c47016cb1p26,
    0x1.2b55c037ebb9dp4

  },
  { // Entry 142
    0x1.fcbb1e5c8d535p26,
    0x1.2b564bee0a6cap4

  },
  { // Entry 143
    0x1.cc0f9d911f1d1p0,
    0x1.2c2p-1

  },
  { // Entry 144
    0x1.b68a28b09fe94p6,
    0x1.2c9e33f794769p2

  },
  { // Entry 145
    0x1.d2e035b1892d6p0,
    0x1.33a74ff06fb66p-1

  },
  { // Entry 146
    0x1.6acde844356cap3,
    0x1.36cce3c3925p1

  },
  { // Entry 147
    0x1.06dadee28c11cp7,
    0x1.38389c48b0fcep2

  },
  { // Entry 148
    0x1.c78f37f07cea6p904,
    0x1.3997381d7d94fp9

  },
  { // Entry 149
    0x1.bfe0c519166c4p1,
    0x1.40a339c81cecbp0

  },
  { // Entry 150
    0x1.fbc9c30603087p927,
    0x1.419dbcc48677p9

  },
  { // Entry 151
    0x1.f1ef9c8a4fdaep930,
    0x1.42a565e456e04p9

  },
  { // Entry 152
    0x1.da134d5a4d1e2p934,
    0x1.44020100804p9

  },
  { // Entry 153
    0x1.f4ec44194b642p938,
    0x1.456bf23e02428p9

  },
  { // Entry 154
    0x1.ea91d9533b394p939,
    0x1.45c1feef8086cp9

  },
  { // Entry 155
    0x1.cb419b9279b36p943,
    0x1.471c71c71c71cp9

  },
  { // Entry 156
    0x1.6d14308828322p29,
    0x1.474c9cc44cc25p4

  },
  { // Entry 157
    0x1.c0194b4186e28p956,
    0x1.4b9ab1781203p9

  },
  { // Entry 158
    0x1.15c605853476cp0,
    0x1.4e59dc7e2b053p-4

  },
  { // Entry 159
    0x1.6dee38735a6d4p972,
    0x1.510c5465d984bp9

  },
  { // Entry 160
    0x1.f1e96be2a52cdp0,
    0x1.549b0f5742382p-1

  },
  { // Entry 161
    0x1.f253c5990aad2p0,
    0x1.55085fb86501ap-1

  },
  { // Entry 162
    0x1.279b1c43a2662p993,
    0x1.58383126e94bfp9

  },
  { // Entry 163
    0x1.6fd5ab59153f4p996,
    0x1.595e5795e592bp9

  },
  { // Entry 164
    0x1.5ecf187653954p1004,
    0x1.5c1e0f0783c1p9

  },
  { // Entry 165
    0x1.f2a3c29fc6d72p1008,
    0x1.5dadf5d1e452cp9

  },
  { // Entry 166
    0x1.edcb14879613ep1009,
    0x1.5e056ed40e56ep9

  },
  { // Entry 167
    0x1.fcc7e5ca80b1fp0,
    0x1.5fa97bb4e906p-1

  },
  { // Entry 168
    0x1.f8b5c987a3d88p3,
    0x1.610e47e1325dcp1

  },
  { // Entry 169
    0x1.f1e9d8cf24fcep7,
    0x1.611b19dcc53e4p2

  },
  { // Entry 170
    0x1.ffc045692e8ap1023,
    0x1.62e41fffffffap9

  },
  { // Entry 171
    0x1.ffc045693009dp1023,
    0x1.62e42p9

  },
  { // Entry 172
    0x1.ffc04569a2fb8p1023,
    0x1.62e42000001ccp9

  },
  { // Entry 173
    0x1.fffffffffeb2ap1023,
    0x1.62e42fefa39eap9

  },
  { // Entry 174
    0x1.00000b40003f5p0,
    0x1.68p-21

  },
  { // Entry 175
    0x1.17e62f70bec9ap8,
    0x1.689a7dc5921b8p2

  },
  { // Entry 176
    0x1.6daaa1336a315p264,
    0x1.6eb1de69ace32p7

  },
  { // Entry 177
    0x1.72a9dadea7498p0,
    0x1.7bp-2

  },
  { // Entry 178
    0x1.1ed3fe64fc53ap2,
    0x1.7fffffffffff9p0

  },
  { // Entry 179
    0x1.0000000000001p0,
    0x1.7ffffffffffffp-52

  },
  { // Entry 180
    0x1.fe31152b7ef6bp553,
    0x1.8p8

  },
  { // Entry 181
    0x1.74d77b76d9516p0,
    0x1.81p-2

  },
  { // Entry 182
    0x1.f0e3e5b6c17cp556,
    0x1.820d92fc4b42ap8

  },
  { // Entry 183
    0x1.00000000c1b59p0,
    0x1.836b2fff6d6cbp-33

  },
  { // Entry 184
    0x1.78b69d690db7ap570,
    0x1.8b7aee631f489p8

  },
  { // Entry 185
    0x1.4dad95877ae08p144,
    0x1.905011e0df629p6

  },
  { // Entry 186
    0x1.fcbb8778f6121p144,
    0x1.91fffp6

  },
  { // Entry 187
    0x1.7a5b2771e0a16p4,
    0x1.94e54738fb4b7p1

  },
  { // Entry 188
    0x1.7d676a26fe5cap0,
    0x1.984p-2

  },
  { // Entry 189
    0x1.698764128a444p147,
    0x1.98f381d7db44fp6

  },
  { // Entry 190
    0x1.44e5a752f9e1fp9,
    0x1.9e8186b267a28p2

  },
  { // Entry 191
    0x1.8023d298e373p0,
    0x1.9f91c1449c421p-2

  },
  { // Entry 192
    0x1.df9bd06daf8cep299,
    0x1.9fc1f212d70f6p7

  },
  { // Entry 193
    0x1.81a4fbca9dcdcp0,
    0x1.a392789eafce9p-2

  },
  { // Entry 194
    0x1.825f354ccf6f3p0,
    0x1.a5807ca1392b2p-2

  },
  { // Entry 195
    0x1.3c1e2876834aap0,
    0x1.bp-3

  },
  { // Entry 196
    0x1.1c7e1aca53796p0,
    0x1.b04p-4

  },
  { // Entry 197
    0x1.a2d50b8b2880ep331,
    0x1.cbd917ccad096p7

  },
  { // Entry 198
    0x1.419c8a8da1b7ap1,
    0x1.d7b677e333d38p-1

  },
  { // Entry 199
    0x1.41d1b8b4649ecp1,
    0x1.d80b1a6330bebp-1

  },
  { // Entry 200
    0x1.bf3447921fd3ap10,
    0x1.df50d9af9e41p2

  },
  { // Entry 201
    0x1.9919794bc862ap0,
    0x1.e0081ebc3264ep-2

  },
  { // Entry 202
    0x1.76a62bad715b4p5,
    0x1.ec5bb209e5707p1

  },
  { // Entry 203
    0x1.c701554c761aap2,
    0x1.f62p0

  },
  { // Entry 204
    0x1.c3dda3c952b92p727,
    0x1.f87c7820d8446p8

  },
  { // Entry 205
    0x1.d28c6097b9d56p2,
    0x1.fc89dc1aa909cp0

  },
  { // Entry 206
    0x1.4231178c2349p734,
    0x1.fcfffffffffffp8

  },
  { // Entry 207
    0x1.04034591911e8p0,
    0x1.fda6dfe27ffffp-7

  },
  { // Entry 208
    0x1.a5b5691c4b0fp0,
    0x1.ff1dd1fffffffp-2

  },
  { // Entry 209
    0x1.0000000000004p0,
    0x1.ff8ffffffffffp-51

  },
  { // Entry 210
    0x1.6d4fd9ab47c92p738,
    0x1.ffe5effffffffp8

  },
  { // Entry 211
    0x1.0000000000fffp0,
    0x1.ffefffffff001p-41

  },
  { // Entry 212
    0x1.74418bce788ecp11,
    0x1.ffeffffffffffp2

  },
  { // Entry 213
    0x1.0000000001p0,
    0x1.fffp-41

  },
  { // Entry 214
    0x1.0000000001p0,
    0x1.ffffe3fffffffp-41

  },
  { // Entry 215
    0x1.d8e64b8d21852p2,
    0x1.ffffffffe7fffp0

  },
  { // Entry 216
    0x1.0p0,
    0x0.ffffffffffffep-1022

  },
  { // Entry 217
    0x1.0p0,
    0x1.ffffffffffffep-54

  },
  { // Entry 218
    0x1.0p0,
    0x1.fffffffffffffp-54

  },
  { // Entry 219
    0x1.304d6aeca2525p69,
    0x1.7ffffffffffffp5

  },
  { // Entry 220
    0x1.304d6aeca254bp69,
    0x1.8p5

  },
  { // Entry 221
    0x1.304d6aeca2571p69,
    0x1.8000000000001p5

  },
  { // Entry 222
    0x1.f8e6c24b558efp-76,
    -0x1.a000000000001p5

  },
  { // Entry 223
    0x1.f8e6c24b5592ep-76,
    -0x1.ap5

  },
  { // Entry 224
    0x1.f8e6c24b5596dp-76,
    -0x1.9ffffffffffffp5

  },
  { // Entry 225
    0x1.55779b984f396p115,
    0x1.3ffffffffffffp6

  },
  { // Entry 226
    0x1.55779b984f3ebp115,
    0x1.4p6

  },
  { // Entry 227
    0x1.55779b984f44p115,
    0x1.4000000000001p6

  },
  { // Entry 228
    0x1.07b7112bc1fbcp-127,
    -0x1.6000000000001p6

  },
  { // Entry 229
    0x1.07b7112bc1ffep-127,
    -0x1.6p6

  },
  { // Entry 230
    0x1.07b7112bc204p-127,
    -0x1.5ffffffffffffp6

  },
  { // Entry 231
    0x1.40a4b9c271508p923,
    0x1.3ffffffffffffp9

  },
  { // Entry 232
    0x1.40a4b9c27178ap923,
    0x1.4p9

  },
  { // Entry 233
    0x1.40a4b9c271a0bp923,
    0x1.4000000000001p9

  },
  { // Entry 234
    0x1.44a3824e525d6p-1016,
    -0x1.6000000000001p9

  },
  { // Entry 235
    0x1.44a3824e5285fp-1016,
    -0x1.6p9

  },
  { // Entry 236
    0x1.44a3824e52ae8p-1016,
    -0x1.5ffffffffffffp9

  },
  { // Entry 237
    0x1.03996528e072bp75,
    0x1.9ffffffffffffp5

  },
  { // Entry 238
    0x1.03996528e074cp75,
    0x1.ap5

  },
  { // Entry 239
    0x1.03996528e076cp75,
    0x1.a000000000001p5

  },
  { // Entry 240
    0x1.aebabae3a417fp-70,
    -0x1.8000000000001p5

  },
  { // Entry 241
    0x1.aebabae3a41b5p-70,
    -0x1.8p5

  },
  { // Entry 242
    0x1.aebabae3a41eap-70,
    -0x1.7ffffffffffffp5

  },
  { // Entry 243
    0x1.f1056dc7bf1b1p126,
    0x1.5ffffffffffffp6

  },
  { // Entry 244
    0x1.f1056dc7bf22dp126,
    0x1.6p6

  },
  { // Entry 245
    0x1.f1056dc7bf2aap126,
    0x1.6000000000001p6

  },
  { // Entry 246
    0x1.7fd974d372de4p-116,
    -0x1.4000000000001p6

  },
  { // Entry 247
    0x1.7fd974d372e44p-116,
    -0x1.4p6

  },
  { // Entry 248
    0x1.7fd974d372ea4p-116,
    -0x1.3ffffffffffffp6

  },
  { // Entry 249
    0x1.93bf4ec282bd4p1015,
    0x1.5ffffffffffffp9

  },
  { // Entry 250
    0x1.93bf4ec282efbp1015,
    0x1.6p9

  },
  { // Entry 251
    0x1.93bf4ec283223p1015,
    0x1.6000000000001p9

  },
  { // Entry 252
    0x1.98c72ca0cab15p-924,
    -0x1.4000000000001p9

  },
  { // Entry 253
    0x1.98c72ca0cae46p-924,
    -0x1.4p9

  },
  { // Entry 254
    0x1.98c72ca0cb178p-924,
    -0x1.3ffffffffffffp9

  },
  { // Entry 255
    0x1.61013a44a981cp-822,
    -0x1.1cb90bfbe8e7cp9

  },
  { // Entry 256
    0x1.8087717a7f08ap-905,
    -0x1.397217f7d1cf8p9

  },
  { // Entry 257
    0x1.a2de59d854521p-988,
    -0x1.562b23f3bab73p9

  },
  { // Entry 258
    0x1.fffffffffffffp-2,
    -0x1.62e42fefa39fp-1

  },
  { // Entry 259
    0x1.0p-1,
    -0x1.62e42fefa39efp-1

  },
  { // Entry 260
    0x1.0000000000001p-1,
    -0x1.62e42fefa39eep-1

  },
  { // Entry 261
    0x1.6a09e667f3bccp-1,
    -0x1.62e42fefa39fp-2

  },
  { // Entry 262
    0x1.6a09e667f3bccp-1,
    -0x1.62e42fefa39efp-2

  },
  { // Entry 263
    0x1.6a09e667f3bcdp-1,
    -0x1.62e42fefa39eep-2

  },
  { // Entry 264
    0x1.ae89f995ad3adp-1,
    -0x1.62e42fefa39fp-3

  },
  { // Entry 265
    0x1.ae89f995ad3aep-1,
    -0x1.62e42fefa39efp-3

  },
  { // Entry 266
    0x1.ae89f995ad3aep-1,
    -0x1.62e42fefa39eep-3

  },
  { // Entry 267
    0x1.d5818dcfba487p-1,
    -0x1.62e42fefa39fp-4

  },
  { // Entry 268
    0x1.d5818dcfba487p-1,
    -0x1.62e42fefa39efp-4

  },
  { // Entry 269
    0x1.d5818dcfba487p-1,
    -0x1.62e42fefa39eep-4

  },
  { // Entry 270
    0x1.ea4afa2a490dap-1,
    -0x1.62e42fefa39fp-5

  },
  { // Entry 271
    0x1.ea4afa2a490dap-1,
    -0x1.62e42fefa39efp-5

  },
  { // Entry 272
    0x1.ea4afa2a490dap-1,
    -0x1.62e42fefa39eep-5

  },
  { // Entry 273
    0x1.f50765b6e454p-1,
    -0x1.62e42fefa39fp-6

  },
  { // Entry 274
    0x1.f50765b6e454p-1,
    -0x1.62e42fefa39efp-6

  },
  { // Entry 275
    0x1.f50765b6e454p-1,
    -0x1.62e42fefa39eep-6

  },
  { // Entry 276
    0x1.059b0d3158574p0,
    0x1.62e42fefa39eep-6

  },
  { // Entry 277
    0x1.059b0d3158574p0,
    0x1.62e42fefa39efp-6

  },
  { // Entry 278
    0x1.059b0d3158574p0,
    0x1.62e42fefa39fp-6

  },
  { // Entry 279
    0x1.0b5586cf9890fp0,
    0x1.62e42fefa39eep-5

  },
  { // Entry 280
    0x1.0b5586cf9890fp0,
    0x1.62e42fefa39efp-5

  },
  { // Entry 281
    0x1.0b5586cf9890fp0,
    0x1.62e42fefa39fp-5

  },
  { // Entry 282
    0x1.172b83c7d517bp0,
    0x1.62e42fefa39eep-4

  },
  { // Entry 283
    0x1.172b83c7d517bp0,
    0x1.62e42fefa39efp-4

  },
  { // Entry 284
    0x1.172b83c7d517bp0,
    0x1.62e42fefa39fp-4

  },
  { // Entry 285
    0x1.306fe0a31b715p0,
    0x1.62e42fefa39eep-3

  },
  { // Entry 286
    0x1.306fe0a31b715p0,
    0x1.62e42fefa39efp-3

  },
  { // Entry 287
    0x1.306fe0a31b715p0,
    0x1.62e42fefa39fp-3

  },
  { // Entry 288
    0x1.6a09e667f3bccp0,
    0x1.62e42fefa39eep-2

  },
  { // Entry 289
    0x1.6a09e667f3bccp0,
    0x1.62e42fefa39efp-2

  },
  { // Entry 290
    0x1.6a09e667f3bcdp0,
    0x1.62e42fefa39fp-2

  },
  { // Entry 291
    0x1.fffffffffffffp0,
    0x1.62e42fefa39eep-1

  },
  { // Entry 292
    0x1.0p1,
    0x1.62e42fefa39efp-1

  },
  { // Entry 293
    0x1.0p1,
    0x1.62e42fefa39fp-1

  },
  { // Entry 294
    0x1.ffffffffffffep1,
    0x1.62e42fefa39eep0

  },
  { // Entry 295
    0x1.0p2,
    0x1.62e42fefa39efp0

  },
  { // Entry 296
    0x1.0000000000001p2,
    0x1.62e42fefa39fp0

  },
  { // Entry 297
    0x1.ffffffffffffbp3,
    0x1.62e42fefa39eep1

  },
  { // Entry 298
    0x1.fffffffffffffp3,
    0x1.62e42fefa39efp1

  },
  { // Entry 299
    0x1.0000000000002p4,
    0x1.62e42fefa39fp1

  },
  { // Entry 300
    0x1.ffffffffffff6p7,
    0x1.62e42fefa39eep2

  },
  { // Entry 301
    0x1.ffffffffffffep7,
    0x1.62e42fefa39efp2

  },
  { // Entry 302
    0x1.0000000000003p8,
    0x1.62e42fefa39fp2

  },
  { // Entry 303
    0x1.fffffffffffedp15,
    0x1.62e42fefa39eep3

  },
  { // Entry 304
    0x1.ffffffffffffdp15,
    0x1.62e42fefa39efp3

  },
  { // Entry 305
    0x1.0000000000006p16,
    0x1.62e42fefa39fp3

  },
  { // Entry 306
    0x1.fffffffffffd9p31,
    0x1.62e42fefa39eep4

  },
  { // Entry 307
    0x1.ffffffffffff9p31,
    0x1.62e42fefa39efp4

  },
  { // Entry 308
    0x1.000000000000dp32,
    0x1.62e42fefa39fp4

  },
  { // Entry 309
    0x1.ffffffffffb2ap1023,
    0x1.62e42fefa39eep9

  },
  { // Entry 310
    0x1.fffffffffff2ap1023,
    0x1.62e42fefa39efp9

  },
  { // Entry 311
    0x0.000000000000ep-1022,
    -0x1.72e42fefa39fp9

  },
  { // Entry 312
    0x0.000000000000ep-1022,
    -0x1.72e42fefa39efp9

  },
  { // Entry 313
    0x0.000000000000ep-1022,
    -0x1.72e42fefa39eep9

  },
  { // Entry 314
    0x1.8ebef9eac820ap-1,
    -0x1.0000000000001p-2

  },
  { // Entry 315
    0x1.8ebef9eac820bp-1,
    -0x1.0p-2

  },
  { // Entry 316
    0x1.8ebef9eac820bp-1,
    -0x1.fffffffffffffp-3

  },
  { // Entry 317
    0x1.c3d6a24ed8221p-1,
    -0x1.0000000000001p-3

  },
  { // Entry 318
    0x1.c3d6a24ed8222p-1,
    -0x1.0p-3

  },
  { // Entry 319
    0x1.c3d6a24ed8222p-1,
    -0x1.fffffffffffffp-4

  },
  { // Entry 320
    0x1.e0fabfbc702a4p-1,
    -0x1.0000000000001p-4

  },
  { // Entry 321
    0x1.e0fabfbc702a4p-1,
    -0x1.0p-4

  },
  { // Entry 322
    0x1.e0fabfbc702a4p-1,
    -0x1.fffffffffffffp-5

  },
  { // Entry 323
    0x1.f03f56a88b5d8p-1,
    -0x1.0000000000001p-5

  },
  { // Entry 324
    0x1.f03f56a88b5d8p-1,
    -0x1.0p-5

  },
  { // Entry 325
    0x1.f03f56a88b5d8p-1,
    -0x1.fffffffffffffp-6

  },
  { // Entry 326
    0x1.f80feabfeefa5p-1,
    -0x1.0000000000001p-6

  },
  { // Entry 327
    0x1.f80feabfeefa5p-1,
    -0x1.0p-6

  },
  { // Entry 328
    0x1.f80feabfeefa5p-1,
    -0x1.fffffffffffffp-7

  },
  { // Entry 329
    0x1.fc03fd56aa225p-1,
    -0x1.0000000000001p-7

  },
  { // Entry 330
    0x1.fc03fd56aa225p-1,
    -0x1.0p-7

  },
  { // Entry 331
    0x1.fc03fd56aa225p-1,
    -0x1.fffffffffffffp-8

  },
  { // Entry 332
    0x1.fe00ffaabffbcp-1,
    -0x1.0000000000001p-8

  },
  { // Entry 333
    0x1.fe00ffaabffbcp-1,
    -0x1.0p-8

  },
  { // Entry 334
    0x1.fe00ffaabffbcp-1,
    -0x1.fffffffffffffp-9

  },
  { // Entry 335
    0x1.ff003ff556aa9p-1,
    -0x1.0000000000001p-9

  },
  { // Entry 336
    0x1.ff003ff556aa9p-1,
    -0x1.0p-9

  },
  { // Entry 337
    0x1.ff003ff556aa9p-1,
    -0x1.fffffffffffffp-10

  },
  { // Entry 338
    0x1.ff800ffeaacp-1,
    -0x1.0000000000001p-10

  },
  { // Entry 339
    0x1.ff800ffeaacp-1,
    -0x1.0p-10

  },
  { // Entry 340
    0x1.ff800ffeaacp-1,
    -0x1.fffffffffffffp-11

  },
  { // Entry 341
    0x1.fff0003fff555p-1,
    -0x1.0000000000001p-13

  },
  { // Entry 342
    0x1.fff0003fff555p-1,
    -0x1.0p-13

  },
  { // Entry 343
    0x1.fff0003fff555p-1,
    -0x1.fffffffffffffp-14

  },
  { // Entry 344
    0x1.48b5e3c3e8186p0,
    0x1.fffffffffffffp-3

  },
  { // Entry 345
    0x1.48b5e3c3e8186p0,
    0x1.0p-2

  },
  { // Entry 346
    0x1.48b5e3c3e8187p0,
    0x1.0000000000001p-2

  },
  { // Entry 347
    0x1.2216045b6f5cdp0,
    0x1.fffffffffffffp-4

  },
  { // Entry 348
    0x1.2216045b6f5cdp0,
    0x1.0p-3

  },
  { // Entry 349
    0x1.2216045b6f5cdp0,
    0x1.0000000000001p-3

  },
  { // Entry 350
    0x1.1082b577d34edp0,
    0x1.fffffffffffffp-5

  },
  { // Entry 351
    0x1.1082b577d34eep0,
    0x1.0p-4

  },
  { // Entry 352
    0x1.1082b577d34eep0,
    0x1.0000000000001p-4

  },
  { // Entry 353
    0x1.08205601127edp0,
    0x1.fffffffffffffp-6

  },
  { // Entry 354
    0x1.08205601127edp0,
    0x1.0p-5

  },
  { // Entry 355
    0x1.08205601127edp0,
    0x1.0000000000001p-5

  },
  { // Entry 356
    0x1.04080ab55de39p0,
    0x1.fffffffffffffp-7

  },
  { // Entry 357
    0x1.04080ab55de39p0,
    0x1.0p-6

  },
  { // Entry 358
    0x1.04080ab55de39p0,
    0x1.0000000000001p-6

  },
  { // Entry 359
    0x1.0202015600446p0,
    0x1.fffffffffffffp-8

  },
  { // Entry 360
    0x1.0202015600446p0,
    0x1.0p-7

  },
  { // Entry 361
    0x1.0202015600446p0,
    0x1.0000000000001p-7

  },
  { // Entry 362
    0x1.0100802ab5577p0,
    0x1.fffffffffffffp-9

  },
  { // Entry 363
    0x1.0100802ab5577p0,
    0x1.0p-8

  },
  { // Entry 364
    0x1.0100802ab5577p0,
    0x1.0000000000001p-8

  },
  { // Entry 365
    0x1.0080200556001p0,
    0x1.fffffffffffffp-10

  },
  { // Entry 366
    0x1.0080200556001p0,
    0x1.0p-9

  },
  { // Entry 367
    0x1.0080200556001p0,
    0x1.0000000000001p-9

  },
  { // Entry 368
    0x1.00400800aab55p0,
    0x1.fffffffffffffp-11

  },
  { // Entry 369
    0x1.00400800aab55p0,
    0x1.0p-10

  },
  { // Entry 370
    0x1.00400800aab55p0,
    0x1.0000000000001p-10

  },
  { // Entry 371
    0x1.0008002000555p0,
    0x1.fffffffffffffp-14

  },
  { // Entry 372
    0x1.0008002000555p0,
    0x1.0p-13

  },
  { // Entry 373
    0x1.0008002000555p0,
    0x1.0000000000001p-13

  },
  { // Entry 374
    0x1.44109edb206a9p-739,
    -0x1.0000000000001p9

  },
  { // Entry 375
    0x1.44109edb20931p-739,
    -0x1.0p9

  },
  { // Entry 376
    0x1.44109edb20a76p-739,
    -0x1.fffffffffffffp8

  },
  { // Entry 377
    0x1.9755956ad4d05p-370,
    -0x1.0000000000001p8

  },
  { // Entry 378
    0x1.9755956ad4e9cp-370,
    -0x1.0p8

  },
  { // Entry 379
    0x1.9755956ad4f68p-370,
    -0x1.fffffffffffffp7

  },
  { // Entry 380
    0x1.42eb9f39afa6ap-185,
    -0x1.0000000000001p7

  },
  { // Entry 381
    0x1.42eb9f39afb0bp-185,
    -0x1.0p7

  },
  { // Entry 382
    0x1.42eb9f39afb5cp-185,
    -0x1.fffffffffffffp6

  },
  { // Entry 383
    0x1.969d47321e466p-93,
    -0x1.0000000000001p6

  },
  { // Entry 384
    0x1.969d47321e4ccp-93,
    -0x1.0p6

  },
  { // Entry 385
    0x1.969d47321e4fep-93,
    -0x1.fffffffffffffp5

  },
  { // Entry 386
    0x1.c8464f761642fp-47,
    -0x1.0000000000001p5

  },
  { // Entry 387
    0x1.c8464f7616468p-47,
    -0x1.0p5

  },
  { // Entry 388
    0x1.c8464f7616485p-47,
    -0x1.fffffffffffffp4

  },
  { // Entry 389
    0x1.e355bbaee85adp-24,
    -0x1.0000000000001p4

  },
  { // Entry 390
    0x1.e355bbaee85cbp-24,
    -0x1.0p4

  },
  { // Entry 391
    0x1.e355bbaee85dap-24,
    -0x1.fffffffffffffp3

  },
  { // Entry 392
    0x1.5fc21041027a2p-12,
    -0x1.0000000000001p3

  },
  { // Entry 393
    0x1.5fc21041027adp-12,
    -0x1.0p3

  },
  { // Entry 394
    0x1.5fc21041027b2p-12,
    -0x1.fffffffffffffp2

  },
  { // Entry 395
    0x1.2c155b8213cfp-6,
    -0x1.0000000000001p2

  },
  { // Entry 396
    0x1.2c155b8213cf4p-6,
    -0x1.0p2

  },
  { // Entry 397
    0x1.2c155b8213cf7p-6,
    -0x1.fffffffffffffp1

  },
  { // Entry 398
    0x1.152aaa3bf81c9p-3,
    -0x1.0000000000001p1

  },
  { // Entry 399
    0x1.152aaa3bf81ccp-3,
    -0x1.0p1

  },
  { // Entry 400
    0x1.152aaa3bf81cdp-3,
    -0x1.fffffffffffffp0

  },
  { // Entry 401
    0x1.78b56362cef36p-2,
    -0x1.0000000000001p0

  },
  { // Entry 402
    0x1.78b56362cef38p-2,
    -0x1.0p0

  },
  { // Entry 403
    0x1.78b56362cef38p-2,
    -0x1.fffffffffffffp-1

  },
  { // Entry 404
    0x1.9476504ba839ap738,
    0x1.fffffffffffffp8

  },
  { // Entry 405
    0x1.9476504ba852ep738,
    0x1.0p9

  },
  { // Entry 406
    0x1.9476504ba8857p738,
    0x1.0000000000001p9

  },
  { // Entry 407
    0x1.41c7a8814be19p369,
    0x1.fffffffffffffp7

  },
  { // Entry 408
    0x1.41c7a8814bebap369,
    0x1.0p8

  },
  { // Entry 409
    0x1.41c7a8814bffcp369,
    0x1.0000000000001p8

  },
  { // Entry 410
    0x1.95e54c5dd41b2p184,
    0x1.fffffffffffffp6

  },
  { // Entry 411
    0x1.95e54c5dd4218p184,
    0x1.0p7

  },
  { // Entry 412
    0x1.95e54c5dd42e2p184,
    0x1.0000000000001p7

  },
  { // Entry 413
    0x1.425982cf597a5p92,
    0x1.fffffffffffffp5

  },
  { // Entry 414
    0x1.425982cf597cdp92,
    0x1.0p6

  },
  { // Entry 415
    0x1.425982cf5981ep92,
    0x1.0000000000001p6

  },
  { // Entry 416
    0x1.1f43fcc4b661bp46,
    0x1.fffffffffffffp4

  },
  { // Entry 417
    0x1.1f43fcc4b662cp46,
    0x1.0p5

  },
  { // Entry 418
    0x1.1f43fcc4b665p46,
    0x1.0000000000001p5

  },
  { // Entry 419
    0x1.0f2ebd0a80018p23,
    0x1.fffffffffffffp3

  },
  { // Entry 420
    0x1.0f2ebd0a8002p23,
    0x1.0p4

  },
  { // Entry 421
    0x1.0f2ebd0a80031p23,
    0x1.0000000000001p4

  },
  { // Entry 422
    0x1.749ea7d470c68p11,
    0x1.fffffffffffffp2

  },
  { // Entry 423
    0x1.749ea7d470c6ep11,
    0x1.0p3

  },
  { // Entry 424
    0x1.749ea7d470c7ap11,
    0x1.0000000000001p3

  },
  { // Entry 425
    0x1.b4c902e273a55p5,
    0x1.fffffffffffffp1

  },
  { // Entry 426
    0x1.b4c902e273a58p5,
    0x1.0p2

  },
  { // Entry 427
    0x1.b4c902e273a5fp5,
    0x1.0000000000001p2

  },
  { // Entry 428
    0x1.d8e64b8d4ddacp2,
    0x1.fffffffffffffp0

  },
  { // Entry 429
    0x1.d8e64b8d4ddaep2,
    0x1.0p1

  },
  { // Entry 430
    0x1.d8e64b8d4ddb2p2,
    0x1.0000000000001p1

  },
  { // Entry 431
    0x1.5bf0a8b145769p1,
    0x1.fffffffffffffp-1

  },
  { // Entry 432
    0x1.5bf0a8b14576ap1,
    0x1.0p0

  },
  { // Entry 433
    0x1.5bf0a8b14576bp1,
    0x1.0000000000001p0

  },
  { // Entry 434
    0x1.ffc045692fc9ep1023,
    0x1.62e41ffffffffp9

  },
  { // Entry 435
    0x1.ffc045693009dp1023,
    0x1.62e42p9

  },
  { // Entry 436
    0x1.ffc045693049dp1023,
    0x1.62e4200000001p9

  },
  { // Entry 437
    0x0.000000000000ep-1022,
    -0x1.72e4200000001p9

  },
  { // Entry 438
    0x0.000000000000ep-1022,
    -0x1.72e42p9

  },
  { // Entry 439
    0x0.000000000000ep-1022,
    -0x1.72e41ffffffffp9

  },
  { // Entry 440
    0x1.724046eb09339p4,
    0x1.921fb54442d18p1

  },
  { // Entry 441
    0x1.33dedc855935fp2,
    0x1.921fb54442d18p0

  },
  { // Entry 442
    0x1.5bf0a8b14576bp1,
    0x1.0000000000001p0

  },
  { // Entry 443
    0x1.5bf0a8b14576ap1,
    0x1.0p0

  },
  { // Entry 444
    0x1.5bf0a8b145769p1,
    0x1.fffffffffffffp-1

  },
  { // Entry 445
    0x1.18bd669471caap1,
    0x1.921fb54442d18p-1

  },
  { // Entry 446
    0x1.0p0,
    0x1.0000000000001p-1022

  },
  { // Entry 447
    0x1.0p0,
    0x1.0p-1022

  },
  { // Entry 448
    0x1.0p0,
    0x0.fffffffffffffp-1022

  },
  { // Entry 449
    0x1.0p0,
    0x0.ffffffffffffep-1022

  },
  { // Entry 450
    0x1.0p0,
    0x0.0000000000002p-1022

  },
  { // Entry 451
    0x1.0p0,
    0x0.0000000000001p-1022

  },
  { // Entry 452
    0x1.0p0,
    0x0.0p0

  },
  { // Entry 453
    0x1.0p0,
    -0x0.0p0

  },
  { // Entry 454
    0x1.0p0,
    -0x0.0000000000001p-1022

  },
  { // Entry 455
    0x1.0p0,
    -0x0.0000000000002p-1022

  },
  { // Entry 456
    0x1.0p0,
    -0x0.ffffffffffffep-1022

  },
  { // Entry 457
    0x1.0p0,
    -0x0.fffffffffffffp-1022

  },
  { // Entry 458
    0x1.0p0,
    -0x1.0p-1022

  },
  { // Entry 459
    0x1.0p0,
    -0x1.0000000000001p-1022

  },
  { // Entry 460
    0x1.d2e171cf04879p-2,
    -0x1.921fb54442d18p-1

  },
  { // Entry 461
    0x1.78b56362cef38p-2,
    -0x1.fffffffffffffp-1

  },
  { // Entry 462
    0x1.78b56362cef38p-2,
    -0x1.0p0

  },
  { // Entry 463
    0x1.78b56362cef36p-2,
    -0x1.0000000000001p0

  },
  { // Entry 464
    0x1.a9bcc46f767ep-3,
    -0x1.921fb54442d18p0

  },
  { // Entry 465
    0x1.620227b598efap-5,
    -0x1.921fb54442d18p1

  },
  { // Entry 466
    0x0.0p0,
    -0x1.ffffffffffffep1023

  },
  { // Entry 467
    0x0.0p0,
    -0x1.fffffffffffffp1023

  },
  { // Entry 468
    0x1.fffffffffff2ap1023,
    0x1.62e42fefa39efp9

  },
  { // Entry 469
    0x1.000000000007cp-1022,
    -0x1.6232bdd7abcd2p9

  },
  { // Entry 470
    0x0.ffffffffffe7cp-1022,
    -0x1.6232bdd7abcd3p9

  },
  { // Entry 471
    0x1.e4efb75e4527ap3,
    0x1.5bf0a8b145769p1
  },
  { // Entry 472
    0x1.0e49787e20548p-4,
    -0x1.5bf0a8b145769p1
  },
  { // Entry 473
    0x1.0p0,
    0x0.0p0
  },
  { // Entry 474
    0x1.0p0,
    -0x0.0p0
  },
  { // Entry 475
    0x1.5bf0a8b14576ap1,
    0x1.0p0
  },
  { // Entry 476
    0x1.78b56362cef38p-2,
    -0x1.0p0
  },
  { // Entry 477
    0x1.368b2fc6f960ap-1,
    -0x1.0p-1
  },
  { // Entry 478
    0x1.a61298e1e069cp0,
    0x1.0p-1
  },
  { // Entry 479
    0x1.e989f5d6dff5ep-10,
    -0x1.921fb54442d18p2
  },
  { // Entry 480
    0x1.265d4e92b6b9cp-7,
    -0x1.2d97c7f3321d2p2
  },
  { // Entry 481
    0x1.620227b598efap-5,
    -0x1.921fb54442d18p1
  },
  { // Entry 482
    0x1.a9bcc46f767ep-3,
    -0x1.921fb54442d18p0
  },
  { // Entry 483
    0x1.0p0,
    0x0.0p0
  },
  { // Entry 484
    0x1.33dedc855935fp2,
    0x1.921fb54442d18p0
  },
  { // Entry 485
    0x1.724046eb09339p4,
    0x1.921fb54442d18p1
  },
  { // Entry 486
    0x1.bd4567b97538p6,
    0x1.2d97c7f3321d2p2
  },
  { // Entry 487
    0x1.0bbeee9177e18p9,
    0x1.921fb54442d18p2
  },
};

static TestCase g_tan_data[] = {
  { // Entry 0
    0x1.5078cebff9c73p-5,
    0x1.50486b2f87014p-5

  },
  { // Entry 1
    -0x1.5078cebff9c73p-5,
    -0x1.50486b2f87014p-5

  },
  { // Entry 2
    0x1.5389e6df41979p-4,
    0x1.52c39ef070cadp-4

  },
  { // Entry 3
    -0x1.5389e6df41979p-4,
    -0x1.52c39ef070cadp-4

  },
  { // Entry 4
    0x1.a933fe176b375p-3,
    0x1.a33f32ac5ceb5p-3

  },
  { // Entry 5
    -0x1.a933fe176b375p-3,
    -0x1.a33f32ac5ceb5p-3

  },
  { // Entry 6
    0x1.fac71cd34eea7p-2,
    0x1.d696bfa988db9p-2

  },
  { // Entry 7
    -0x1.fac71cd34eea7p-2,
    -0x1.d696bfa988db9p-2

  },
  { // Entry 8
    0x1.7ba49f739829fp-1,
    0x1.46ac372243536p-1

  },
  { // Entry 9
    -0x1.7ba49f739829fp-1,
    -0x1.46ac372243536p-1

  },
  { // Entry 10
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 11
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 12
    -0x1.8f048832144b7p0,
    -0x1.0018p0

  },
  { // Entry 13
    0x1.8f048832144b7p0,
    0x1.0018p0

  },
  { // Entry 14
    -0x1.8e884b24313afp0,
    -0x1.090cca18a5565p2

  },
  { // Entry 15
    0x1.8e884b24313afp0,
    0x1.090cca18a5565p2

  },
  { // Entry 16
    -0x1.ca18654b35697p0,
    -0x1.0faa7650df144p0

  },
  { // Entry 17
    0x1.ca18654b35697p0,
    0x1.0faa7650df144p0

  },
  { // Entry 18
    -0x1.e52fafa22ef14p-2,
    -0x1.180000000004p5

  },
  { // Entry 19
    0x1.e52fafa22ef14p-2,
    0x1.180000000004p5

  },
  { // Entry 20
    -0x1.a3ca421dc30f2p-3,
    -0x1.4000527aca388p99

  },
  { // Entry 21
    0x1.a3ca421dc30f2p-3,
    0x1.4000527aca388p99

  },
  { // Entry 22
    0x1.1f3b7d1978609p1,
    -0x1.486c3634751ecp2

  },
  { // Entry 23
    -0x1.1f3b7d1978609p1,
    0x1.486c3634751ecp2

  },
  { // Entry 24
    -0x1.7eb873343fa7ap-1,
    -0x1.48a71800b5713p-1

  },
  { // Entry 25
    0x1.7eb873343fa7ap-1,
    0x1.48a71800b5713p-1

  },
  { // Entry 26
    0x1.be071572f64e8p-1,
    -0x1.49af0314eea3cp299

  },
  { // Entry 27
    -0x1.be071572f64e8p-1,
    0x1.49af0314eea3cp299

  },
  { // Entry 28
    0x1.ffbb2647f57a2p-1,
    -0x1.5fe00c814ffd6p2

  },
  { // Entry 29
    -0x1.ffbb2647f57a2p-1,
    0x1.5fe00c814ffd6p2

  },
  { // Entry 30
    -0x1.a8eb142b2f428p-1,
    -0x1.62ac241f79439p-1

  },
  { // Entry 31
    0x1.a8eb142b2f428p-1,
    0x1.62ac241f79439p-1

  },
  { // Entry 32
    -0x1.7d1d3559ddac9p-4,
    -0x1.7c051b476ca8dp-4

  },
  { // Entry 33
    0x1.7d1d3559ddac9p-4,
    0x1.7c051b476ca8dp-4

  },
  { // Entry 34
    -0x1.0e1d0305b7b72p2,
    -0x1.7e43c880074c6p996

  },
  { // Entry 35
    0x1.0e1d0305b7b72p2,
    0x1.7e43c880074c6p996

  },
  { // Entry 36
    -0x1.812bdfe0246bcp-4,
    -0x1.800ac363398c4p-4

  },
  { // Entry 37
    0x1.812bdfe0246bcp-4,
    0x1.800ac363398c4p-4

  },
  { // Entry 38
    -0x1.850e5544b0c79p-4,
    -0x1.83e46aedbff36p-4

  },
  { // Entry 39
    0x1.850e5544b0c79p-4,
    0x1.83e46aedbff36p-4

  },
  { // Entry 40
    0x1.e6b5d91bba934p-2,
    -0x1.83ecf42e9265ap3

  },
  { // Entry 41
    -0x1.e6b5d91bba934p-2,
    0x1.83ecf42e9265ap3

  },
  { // Entry 42
    -0x1.f3688bc2594e2p-1,
    -0x1.8bcp-1

  },
  { // Entry 43
    0x1.f3688bc2594e2p-1,
    0x1.8bcp-1

  },
  { // Entry 44
    0x1.ec0d0facdd08bp-2,
    -0x1.8d2ffffffffd1p9

  },
  { // Entry 45
    -0x1.ec0d0facdd08bp-2,
    0x1.8d2ffffffffd1p9

  },
  { // Entry 46
    0x1.ec0336d539259p-2,
    -0x1.8d3000fffffd1p9

  },
  { // Entry 47
    -0x1.ec0336d539259p-2,
    0x1.8d3000fffffd1p9

  },
  { // Entry 48
    -0x1.f8093a017021fp-1,
    -0x1.baeee6f6fa538p6

  },
  { // Entry 49
    0x1.f8093a017021fp-1,
    0x1.baeee6f6fa538p6

  },
  { // Entry 50
    0x1.deaf34994b7e7p3,
    -0x1.c6867e07455eap3

  },
  { // Entry 51
    -0x1.deaf34994b7e7p3,
    0x1.c6867e07455eap3

  },
  { // Entry 52
    -0x1.f29aa87d4e1dep-1,
    -0x1.d27ffffffep7

  },
  { // Entry 53
    0x1.f29aa87d4e1dep-1,
    0x1.d27ffffffep7

  },
  { // Entry 54
    0x1.762fb47a1925ap-3,
    -0x1.f0df38029c9efp3

  },
  { // Entry 55
    -0x1.762fb47a1925ap-3,
    0x1.f0df38029c9efp3

  },
  { // Entry 56
    -0x1.8eb23ef2126bcp0,
    -0x1.fffffc0000fffp-1

  },
  { // Entry 57
    0x1.8eb23ef2126bcp0,
    0x1.fffffc0000fffp-1

  },
  { // Entry 58
    -0x1.d299d285bf018p-2,
    -0x1.ffffffffffffcp1023

  },
  { // Entry 59
    0x1.d299d285bf018p-2,
    0x1.ffffffffffffcp1023

  },
  { // Entry 60
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 61
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 62
    -0x1.82bee572e2ac9p-6,
    0x1.0p64

  },
  { // Entry 63
    0x1.82bee572e2ac9p-6,
    -0x1.0p64

  },
  { // Entry 64
    0x1.f53a8d05afcf7p4,
    0x1.0000000000001p51

  },
  { // Entry 65
    -0x1.f53a8d05afcf7p4,
    -0x1.0000000000001p51

  },
  { // Entry 66
    -0x1.6b371df5980cep-1,
    0x1.0000000000001p1017

  },
  { // Entry 67
    0x1.6b371df5980cep-1,
    -0x1.0000000000001p1017

  },
  { // Entry 68
    -0x1.b32e78f49a0c8p2,
    0x1.0000000000003p3

  },
  { // Entry 69
    0x1.b32e78f49a0c8p2,
    -0x1.0000000000003p3

  },
  { // Entry 70
    0x1.98afbd24264bcp-1,
    0x1.0000000000003p21

  },
  { // Entry 71
    -0x1.98afbd24264bcp-1,
    -0x1.0000000000003p21

  },
  { // Entry 72
    0x1.b667a2abe36c3p0,
    0x1.0000000000003p511

  },
  { // Entry 73
    -0x1.b667a2abe36c3p0,
    -0x1.0000000000003p511

  },
  { // Entry 74
    0x1.204c26a427862p-2,
    0x1.0000000000003p716

  },
  { // Entry 75
    -0x1.204c26a427862p-2,
    -0x1.0000000000003p716

  },
  { // Entry 76
    0x1.91c8f2938262dp4,
    0x1.0000000000007p8

  },
  { // Entry 77
    -0x1.91c8f2938262dp4,
    -0x1.0000000000007p8

  },
  { // Entry 78
    -0x1.27f7f0880032p-2,
    0x1.0000000000038p380

  },
  { // Entry 79
    0x1.27f7f0880032p-2,
    -0x1.0000000000038p380

  },
  { // Entry 80
    -0x1.d6890cc32711dp-3,
    0x1.0000000000118p380

  },
  { // Entry 81
    0x1.d6890cc32711dp-3,
    -0x1.0000000000118p380

  },
  { // Entry 82
    0x1.9af0e6f72f912p-3,
    0x1.0000000000908p500

  },
  { // Entry 83
    -0x1.9af0e6f72f912p-3,
    -0x1.0000000000908p500

  },
  { // Entry 84
    0x1.17b4f5bf44098p-1,
    0x1.000000000cp-1

  },
  { // Entry 85
    -0x1.17b4f5bf44098p-1,
    -0x1.000000000cp-1

  },
  { // Entry 86
    -0x1.17eb22e4dba73p0,
    0x1.00000001cp40

  },
  { // Entry 87
    0x1.17eb22e4dba73p0,
    -0x1.00000001cp40

  },
  { // Entry 88
    0x1.f6f03ce5690a7p-1,
    0x1.0000001p250

  },
  { // Entry 89
    -0x1.f6f03ce5690a7p-1,
    -0x1.0000001p250

  },
  { // Entry 90
    0x1.e23b78282a75dp-1,
    0x1.000000988p27

  },
  { // Entry 91
    -0x1.e23b78282a75dp-1,
    -0x1.000000988p27

  },
  { // Entry 92
    -0x1.981b657e1ca27p-3,
    0x1.00000cp429

  },
  { // Entry 93
    0x1.981b657e1ca27p-3,
    -0x1.00000cp429

  },
  { // Entry 94
    -0x1.455a2184f4c3ep-1,
    0x1.00000fcp1000

  },
  { // Entry 95
    0x1.455a2184f4c3ep-1,
    -0x1.00000fcp1000

  },
  { // Entry 96
    0x1.8ee66962f210cp0,
    0x1.000f371b7a006p0

  },
  { // Entry 97
    -0x1.8ee66962f210cp0,
    -0x1.000f371b7a006p0

  },
  { // Entry 98
    -0x1.ecd75cf6d4664p-3,
    0x1.001p15

  },
  { // Entry 99
    0x1.ecd75cf6d4664p-3,
    -0x1.001p15

  },
  { // Entry 100
    0x1.17d42033277ccp-1,
    0x1.0017ffffffffdp-1

  },
  { // Entry 101
    -0x1.17d42033277ccp-1,
    -0x1.0017ffffffffdp-1

  },
  { // Entry 102
    0x1.8f048832144b7p0,
    0x1.0018p0

  },
  { // Entry 103
    -0x1.8f048832144b7p0,
    -0x1.0018p0

  },
  { // Entry 104
    -0x1.18273cc3e7639p-2,
    0x1.001fffep500

  },
  { // Entry 105
    0x1.18273cc3e7639p-2,
    -0x1.001fffep500

  },
  { // Entry 106
    -0x1.d8f90cad30547p-2,
    0x1.018p40

  },
  { // Entry 107
    0x1.d8f90cad30547p-2,
    -0x1.018p40

  },
  { // Entry 108
    0x1.b079ea0d14a4ap-2,
    0x1.01b8a484ac0b6p4

  },
  { // Entry 109
    -0x1.b079ea0d14a4ap-2,
    -0x1.01b8a484ac0b6p4

  },
  { // Entry 110
    -0x1.a40c262f6ab9ap-1,
    0x1.026ac0ef32d4p28

  },
  { // Entry 111
    0x1.a40c262f6ab9ap-1,
    -0x1.026ac0ef32d4p28

  },
  { // Entry 112
    0x1.03b8c1f329665p-4,
    0x1.035fdcd08a596p-4

  },
  { // Entry 113
    -0x1.03b8c1f329665p-4,
    -0x1.035fdcd08a596p-4

  },
  { // Entry 114
    0x1.044979d134ed9p-4,
    0x1.03fp-4

  },
  { // Entry 115
    -0x1.044979d134ed9p-4,
    -0x1.03fp-4

  },
  { // Entry 116
    -0x1.e717de7da2ce8p0,
    0x1.07p1

  },
  { // Entry 117
    0x1.e717de7da2ce8p0,
    -0x1.07p1

  },
  { // Entry 118
    -0x1.8c896f607ff52p-1,
    0x1.07p30

  },
  { // Entry 119
    0x1.8c896f607ff52p-1,
    -0x1.07p30

  },
  { // Entry 120
    0x1.fffffffff5d84p-1,
    0x1.07e4cef4cbb0ep4

  },
  { // Entry 121
    -0x1.fffffffff5d84p-1,
    -0x1.07e4cef4cbb0ep4

  },
  { // Entry 122
    0x1.b476d32c1b745p0,
    0x1.0a53a78b13ab2p0

  },
  { // Entry 123
    -0x1.b476d32c1b745p0,
    -0x1.0a53a78b13ab2p0

  },
  { // Entry 124
    0x1.f2df7c02d20cep-1,
    0x1.0afbc268b9848p6

  },
  { // Entry 125
    -0x1.f2df7c02d20cep-1,
    -0x1.0afbc268b9848p6

  },
  { // Entry 126
    -0x1.b571af562f08ap0,
    0x1.0cd5d435bea6dp1

  },
  { // Entry 127
    0x1.b571af562f08ap0,
    -0x1.0cd5d435bea6dp1

  },
  { // Entry 128
    -0x1.ac73d2920a795p0,
    0x1.0ep1

  },
  { // Entry 129
    0x1.ac73d2920a795p0,
    -0x1.0ep1

  },
  { // Entry 130
    -0x1.126dce8ac7c81p-1,
    0x1.1086210842108p5

  },
  { // Entry 131
    0x1.126dce8ac7c81p-1,
    -0x1.1086210842108p5

  },
  { // Entry 132
    -0x1.9680c02601047p0,
    0x1.11p1

  },
  { // Entry 133
    0x1.9680c02601047p0,
    -0x1.11p1

  },
  { // Entry 134
    0x1.d1e716934469bp0,
    0x1.118p0

  },
  { // Entry 135
    -0x1.d1e716934469bp0,
    -0x1.118p0

  },
  { // Entry 136
    -0x1.6aa7310143084p-1,
    0x1.19df389f39e0ap3

  },
  { // Entry 137
    0x1.6aa7310143084p-1,
    -0x1.19df389f39e0ap3

  },
  { // Entry 138
    0x1.cb9a99227bdc9p1,
    0x1.1c3598211013ap2

  },
  { // Entry 139
    -0x1.cb9a99227bdc9p1,
    -0x1.1c3598211013ap2

  },
  { // Entry 140
    -0x1.bc109c3e61724p7,
    0x1.1d65aa4224c3p118

  },
  { // Entry 141
    0x1.bc109c3e61724p7,
    -0x1.1d65aa4224c3p118

  },
  { // Entry 142
    -0x1.09b393f48b2c6p-1,
    0x1.1e4658272dc6fp3

  },
  { // Entry 143
    0x1.09b393f48b2c6p-1,
    -0x1.1e4658272dc6fp3

  },
  { // Entry 144
    0x1.200000000007ap-22,
    0x1.2p-22

  },
  { // Entry 145
    -0x1.200000000007ap-22,
    -0x1.2p-22

  },
  { // Entry 146
    -0x1.02a335b00707ap0,
    0x1.2127409620cacp95

  },
  { // Entry 147
    0x1.02a335b00707ap0,
    -0x1.2127409620cacp95

  },
  { // Entry 148
    0x1.2508b9c1273acp-4,
    0x1.2489224892248p-4

  },
  { // Entry 149
    -0x1.2508b9c1273acp-4,
    -0x1.2489224892248p-4

  },
  { // Entry 150
    0x1.fded5f53d132dp2,
    0x1.2a52d119da061p237

  },
  { // Entry 151
    -0x1.fded5f53d132dp2,
    -0x1.2a52d119da061p237

  },
  { // Entry 152
    0x1.2de56a6ef9c5ep-4,
    0x1.2d59ebab8dae4p-4

  },
  { // Entry 153
    -0x1.2de56a6ef9c5ep-4,
    -0x1.2d59ebab8dae4p-4

  },
  { // Entry 154
    0x1.31665eb191fbbp-4,
    0x1.30d5f8e54b6d8p-4

  },
  { // Entry 155
    -0x1.31665eb191fbbp-4,
    -0x1.30d5f8e54b6d8p-4

  },
  { // Entry 156
    0x1.3cc1d4d28bfd2p-2,
    0x1.333275d63ec5p-2

  },
  { // Entry 157
    -0x1.3cc1d4d28bfd2p-2,
    -0x1.333275d63ec5p-2

  },
  { // Entry 158
    0x1.3cc237c0c7dccp-2,
    0x1.3332d020b6da9p-2

  },
  { // Entry 159
    -0x1.3cc237c0c7dccp-2,
    -0x1.3332d020b6da9p-2

  },
  { // Entry 160
    0x1.5e472e16999dfp-1,
    0x1.333333401e66bp-1

  },
  { // Entry 161
    -0x1.5e472e16999dfp-1,
    -0x1.333333401e66bp-1

  },
  { // Entry 162
    0x1.b5ed1c2080a98p-1,
    0x1.38f137cb9dbfcp9

  },
  { // Entry 163
    -0x1.b5ed1c2080a98p-1,
    -0x1.38f137cb9dbfcp9

  },
  { // Entry 164
    0x1.01aa22e2133d4p1,
    0x1.39a383f3fa003p85

  },
  { // Entry 165
    -0x1.01aa22e2133d4p1,
    -0x1.39a383f3fa003p85

  },
  { // Entry 166
    0x1.ffffffffff582p-1,
    0x1.3a28c59d54311p4

  },
  { // Entry 167
    -0x1.ffffffffff582p-1,
    -0x1.3a28c59d54311p4

  },
  { // Entry 168
    0x1.7166689d4803fp-1,
    0x1.4000000003fffp-1

  },
  { // Entry 169
    -0x1.7166689d4803fp-1,
    -0x1.4000000003fffp-1

  },
  { // Entry 170
    -0x1.ff7d27b37eba1p-1,
    0x1.40724a44714cfp5

  },
  { // Entry 171
    0x1.ff7d27b37eba1p-1,
    -0x1.40724a44714cfp5

  },
  { // Entry 172
    0x1.453a7d29dadadp-4,
    0x1.448c2d6e1e1afp-4

  },
  { // Entry 173
    -0x1.453a7d29dadadp-4,
    -0x1.448c2d6e1e1afp-4

  },
  { // Entry 174
    -0x1.a50f7601413e5p0,
    0x1.478fc08p43

  },
  { // Entry 175
    0x1.a50f7601413e5p0,
    -0x1.478fc08p43

  },
  { // Entry 176
    -0x1.a9991acb7636cp-4,
    0x1.4e93bee72b565p62

  },
  { // Entry 177
    0x1.a9991acb7636cp-4,
    -0x1.4e93bee72b565p62

  },
  { // Entry 178
    0x1.2952396945948p1,
    0x1.4f0f308p488

  },
  { // Entry 179
    -0x1.2952396945948p1,
    -0x1.4f0f308p488

  },
  { // Entry 180
    0x1.5078cebff9c73p-5,
    0x1.50486b2f87014p-5

  },
  { // Entry 181
    -0x1.5078cebff9c73p-5,
    -0x1.50486b2f87014p-5

  },
  { // Entry 182
    -0x1.1c929b6ede9eep-1,
    0x1.5130d552f1036p1

  },
  { // Entry 183
    0x1.1c929b6ede9eep-1,
    -0x1.5130d552f1036p1

  },
  { // Entry 184
    0x1.2ab3189e2d4aep1,
    0x1.52f00ep793

  },
  { // Entry 185
    -0x1.2ab3189e2d4aep1,
    -0x1.52f00ep793

  },
  { // Entry 186
    -0x1.7d2e63fb98891p0,
    0x1.5371684e5fb34p2

  },
  { // Entry 187
    0x1.7d2e63fb98891p0,
    -0x1.5371684e5fb34p2

  },
  { // Entry 188
    -0x1.f9f4f0da4de54p-1,
    0x1.54ef2208956p239

  },
  { // Entry 189
    0x1.f9f4f0da4de54p-1,
    -0x1.54ef2208956p239

  },
  { // Entry 190
    0x1.1483073142e6p2,
    0x1.57e590af09014p0

  },
  { // Entry 191
    -0x1.1483073142e6p2,
    -0x1.57e590af09014p0

  },
  { // Entry 192
    0x1.9972d4021c972p-1,
    0x1.596p-1

  },
  { // Entry 193
    -0x1.9972d4021c972p-1,
    -0x1.596p-1

  },
  { // Entry 194
    -0x1.e501ffd3a68c4p-2,
    0x1.5981293783e1fp1

  },
  { // Entry 195
    0x1.e501ffd3a68c4p-2,
    -0x1.5981293783e1fp1

  },
  { // Entry 196
    0x1.1604cc3dfc418p-1,
    0x1.5bea01p468

  },
  { // Entry 197
    -0x1.1604cc3dfc418p-1,
    -0x1.5bea01p468

  },
  { // Entry 198
    -0x1.f76ca50bbbaebp-1,
    0x1.60661c1969666p2

  },
  { // Entry 199
    0x1.f76ca50bbbaebp-1,
    -0x1.60661c1969666p2

  },
  { // Entry 200
    0x1.cd8b73c9430ffp0,
    0x1.62c5a850a142ap59

  },
  { // Entry 201
    -0x1.cd8b73c9430ffp0,
    -0x1.62c5a850a142ap59

  },
  { // Entry 202
    0x1.3accfd453ee67p0,
    0x1.64ef438p142

  },
  { // Entry 203
    -0x1.3accfd453ee67p0,
    -0x1.64ef438p142

  },
  { // Entry 204
    -0x1.acd9302d72de4p-1,
    0x1.658p2

  },
  { // Entry 205
    0x1.acd9302d72de4p-1,
    -0x1.658p2

  },
  { // Entry 206
    0x1.f004f875c2e74p-1,
    0x1.6603c65d348d2p5

  },
  { // Entry 207
    -0x1.f004f875c2e74p-1,
    -0x1.6603c65d348d2p5

  },
  { // Entry 208
    0x1.f53496e6d7f72p-1,
    0x1.660e6bf2e092ap5

  },
  { // Entry 209
    -0x1.f53496e6d7f72p-1,
    -0x1.660e6bf2e092ap5

  },
  { // Entry 210
    0x1.b64ee24f0119cp-1,
    0x1.6a8p-1

  },
  { // Entry 211
    -0x1.b64ee24f0119cp-1,
    -0x1.6a8p-1

  },
  { // Entry 212
    -0x1.d9ba9a7975636p60,
    0x1.6ac5b262ca1ffp849

  },
  { // Entry 213
    0x1.d9ba9a7975636p60,
    -0x1.6ac5b262ca1ffp849

  },
  { // Entry 214
    0x1.b6f557b999e23p-1,
    0x1.6aep-1

  },
  { // Entry 215
    -0x1.b6f557b999e23p-1,
    -0x1.6aep-1

  },
  { // Entry 216
    0x1.c1e1d5c4c0f08p-1,
    0x1.6cdb36cdb36c9p239

  },
  { // Entry 217
    -0x1.c1e1d5c4c0f08p-1,
    -0x1.6cdb36cdb36c9p239

  },
  { // Entry 218
    0x1.95bce4f578698p-1,
    0x1.6f1af1612270ap6

  },
  { // Entry 219
    -0x1.95bce4f578698p-1,
    -0x1.6f1af1612270ap6

  },
  { // Entry 220
    0x1.711e8f5fffba2p-4,
    0x1.702p-4

  },
  { // Entry 221
    -0x1.711e8f5fffba2p-4,
    -0x1.702p-4

  },
  { // Entry 222
    0x1.fb5898f29bb26p2,
    0x1.72p0

  },
  { // Entry 223
    -0x1.fb5898f29bb26p2,
    -0x1.72p0

  },
  { // Entry 224
    -0x1.ff9b771284d23p1,
    0x1.7348c347ddc2p239

  },
  { // Entry 225
    0x1.ff9b771284d23p1,
    -0x1.7348c347ddc2p239

  },
  { // Entry 226
    0x1.f72d47a0080e3p-2,
    0x1.739ce739ce738p100

  },
  { // Entry 227
    -0x1.f72d47a0080e3p-2,
    -0x1.739ce739ce738p100

  },
  { // Entry 228
    0x1.76441e7f8ea6p-4,
    0x1.753acc3d3ff35p-4

  },
  { // Entry 229
    -0x1.76441e7f8ea6p-4,
    -0x1.753acc3d3ff35p-4

  },
  { // Entry 230
    0x1.ce3f642e15af4p-1,
    0x1.77fffffffffffp-1

  },
  { // Entry 231
    -0x1.ce3f642e15af4p-1,
    -0x1.77fffffffffffp-1

  },
  { // Entry 232
    0x1.f425002a548ebp42,
    0x1.78fdb9effea26p4

  },
  { // Entry 233
    -0x1.f425002a548ebp42,
    -0x1.78fdb9effea26p4

  },
  { // Entry 234
    -0x1.dbc80de7dd043p-1,
    0x1.7a5f74607e851p19

  },
  { // Entry 235
    0x1.dbc80de7dd043p-1,
    -0x1.7a5f74607e851p19

  },
  { // Entry 236
    0x1.7b3bb3d0b3ca4p42,
    0x1.7f7ef77e83f1ap19

  },
  { // Entry 237
    -0x1.7b3bb3d0b3ca4p42,
    -0x1.7f7ef77e83f1ap19

  },
  { // Entry 238
    0x1.e7f05b71cd2d1p33,
    0x1.7f7f10a07f45ep20

  },
  { // Entry 239
    -0x1.e7f05b71cd2d1p33,
    -0x1.7f7f10a07f45ep20

  },
  { // Entry 240
    0x1.8000000000004p-25,
    0x1.7ffffffffffffp-25

  },
  { // Entry 241
    -0x1.8000000000004p-25,
    -0x1.7ffffffffffffp-25

  },
  { // Entry 242
    0x1.8000000000007p-25,
    0x1.8000000000002p-25

  },
  { // Entry 243
    -0x1.8000000000007p-25,
    -0x1.8000000000002p-25

  },
  { // Entry 244
    0x1.24245af4cd995p-52,
    0x1.81ae0dffa3b33p959

  },
  { // Entry 245
    -0x1.24245af4cd995p-52,
    -0x1.81ae0dffa3b33p959

  },
  { // Entry 246
    0x1.d72261d98e26cp-1,
    0x1.846bd7a4dce55p698

  },
  { // Entry 247
    -0x1.d72261d98e26cp-1,
    -0x1.846bd7a4dce55p698

  },
  { // Entry 248
    0x1.42d8a1ba441adp1,
    0x1.8720588p392

  },
  { // Entry 249
    -0x1.42d8a1ba441adp1,
    -0x1.8720588p392

  },
  { // Entry 250
    0x1.ea7b444cd798ep-1,
    0x1.8722a67ea14acp-1

  },
  { // Entry 251
    -0x1.ea7b444cd798ep-1,
    -0x1.8722a67ea14acp-1

  },
  { // Entry 252
    -0x1.c7dc7f08dbbap-1,
    0x1.89936c8828d38p299

  },
  { // Entry 253
    0x1.c7dc7f08dbbap-1,
    -0x1.89936c8828d38p299

  },
  { // Entry 254
    0x1.569653e319bbap1,
    0x1.8a69106fb9798p6

  },
  { // Entry 255
    -0x1.569653e319bbap1,
    -0x1.8a69106fb9798p6

  },
  { // Entry 256
    0x1.f2db21469f3d6p-1,
    0x1.8b777e1d2308cp-1

  },
  { // Entry 257
    -0x1.f2db21469f3d6p-1,
    -0x1.8b777e1d2308cp-1

  },
  { // Entry 258
    0x1.f3688bc2594e2p-1,
    0x1.8bcp-1

  },
  { // Entry 259
    -0x1.f3688bc2594e2p-1,
    -0x1.8bcp-1

  },
  { // Entry 260
    0x1.8d3a2544566dfp-4,
    0x1.8bfd2274d851ap-4

  },
  { // Entry 261
    -0x1.8d3a2544566dfp-4,
    -0x1.8bfd2274d851ap-4

  },
  { // Entry 262
    0x1.f4575cc4e477fp-1,
    0x1.8c3a450071dd9p-1

  },
  { // Entry 263
    -0x1.f4575cc4e477fp-1,
    -0x1.8c3a450071dd9p-1

  },
  { // Entry 264
    -0x1.1e09f66c4250cp11,
    0x1.8cc0dd2b0f4b8p200

  },
  { // Entry 265
    0x1.1e09f66c4250cp11,
    -0x1.8cc0dd2b0f4b8p200

  },
  { // Entry 266
    0x1.f71496cb921e6p-1,
    0x1.8dap-1

  },
  { // Entry 267
    -0x1.f71496cb921e6p-1,
    -0x1.8dap-1

  },
  { // Entry 268
    0x1.f71b4a659116ap-1,
    0x1.8da368da368d8p-1

  },
  { // Entry 269
    -0x1.f71b4a659116ap-1,
    -0x1.8da368da368d8p-1

  },
  { // Entry 270
    0x1.ff9b68ccadb3p-1,
    0x1.91ed64b977a9ap-1

  },
  { // Entry 271
    -0x1.ff9b68ccadb3p-1,
    -0x1.91ed64b977a9ap-1

  },
  { // Entry 272
    0x1.0000000029048p18,
    0x1.921f754442d19p0

  },
  { // Entry 273
    -0x1.0000000029048p18,
    -0x1.921f754442d19p0

  },
  { // Entry 274
    0x1.eef067afd328fp48,
    0x1.921fb54442d1p0

  },
  { // Entry 275
    -0x1.eef067afd328fp48,
    -0x1.921fb54442d1p0

  },
  { // Entry 276
    0x1.0000000003af3p-17,
    0x1.921ff54442d18p1

  },
  { // Entry 277
    -0x1.0000000003af3p-17,
    -0x1.921ff54442d18p1

  },
  { // Entry 278
    -0x1.b6772cb667dc2p17,
    0x1.922p0

  },
  { // Entry 279
    0x1.b6772cb667dc2p17,
    -0x1.922p0

  },
  { // Entry 280
    -0x1.fffffffceeefep-1,
    0x1.922071c31fc99p20

  },
  { // Entry 281
    0x1.fffffffceeefep-1,
    -0x1.922071c31fc99p20

  },
  { // Entry 282
    0x1.9d7c1354ba6f8p-3,
    0x1.97fffffffffffp-3

  },
  { // Entry 283
    -0x1.9d7c1354ba6f8p-3,
    -0x1.97fffffffffffp-3

  },
  { // Entry 284
    0x1.9af8877bb45e4p-4,
    0x1.999999a10a13cp-4

  },
  { // Entry 285
    -0x1.9af8877bb45e4p-4,
    -0x1.999999a10a13cp-4

  },
  { // Entry 286
    -0x1.b6ce128587cdp4,
    0x1.9b74446ed05dcp0

  },
  { // Entry 287
    0x1.b6ce128587cdp4,
    -0x1.9b74446ed05dcp0

  },
  { // Entry 288
    0x1.ff65aef54c8fcp-1,
    0x1.9eae494d2b275p4

  },
  { // Entry 289
    -0x1.ff65aef54c8fcp-1,
    -0x1.9eae494d2b275p4

  },
  { // Entry 290
    0x1.61776aa407a44p-3,
    0x1.a8p1

  },
  { // Entry 291
    -0x1.61776aa407a44p-3,
    -0x1.a8p1

  },
  { // Entry 292
    0x1.b6001de13ad96p-3,
    0x1.af8p-3

  },
  { // Entry 293
    -0x1.b6001de13ad96p-3,
    -0x1.af8p-3

  },
  { // Entry 294
    0x1.b5a0503ae354bp-4,
    0x1.b3f8ea7b1f91bp-4

  },
  { // Entry 295
    -0x1.b5a0503ae354bp-4,
    -0x1.b3f8ea7b1f91bp-4

  },
  { // Entry 296
    0x1.b5a0503ae4c7bp-4,
    0x1.b3f8ea7b21008p-4

  },
  { // Entry 297
    -0x1.b5a0503ae4c7bp-4,
    -0x1.b3f8ea7b21008p-4

  },
  { // Entry 298
    0x1.057584c429b3ap59,
    0x1.b951f1572eba5p23

  },
  { // Entry 299
    -0x1.057584c429b3ap59,
    -0x1.b951f1572eba5p23

  },
  { // Entry 300
    -0x1.9a282fa1ff7dap2,
    0x1.b9cp0

  },
  { // Entry 301
    0x1.9a282fa1ff7dap2,
    -0x1.b9cp0

  },
  { // Entry 302
    -0x1.027d184afb198p-52,
    0x1.bab62ed655019p970

  },
  { // Entry 303
    0x1.027d184afb198p-52,
    -0x1.bab62ed655019p970

  },
  { // Entry 304
    0x1.ca6efdf845d6dp2,
    0x1.bea1b35f3cb6dp84

  },
  { // Entry 305
    -0x1.ca6efdf845d6dp2,
    -0x1.bea1b35f3cb6dp84

  },
  { // Entry 306
    0x1.fd87b34747b74p42,
    0x1.c463abeccb27bp3

  },
  { // Entry 307
    -0x1.fd87b34747b74p42,
    -0x1.c463abeccb27bp3

  },
  { // Entry 308
    0x1.ffffffffffffbp-1,
    0x1.c463abeccb2bbp2

  },
  { // Entry 309
    -0x1.ffffffffffffbp-1,
    -0x1.c463abeccb2bbp2

  },
  { // Entry 310
    0x1.fb057029acfd2p-1,
    0x1.c6cbe26b7b45fp86

  },
  { // Entry 311
    -0x1.fb057029acfd2p-1,
    -0x1.c6cbe26b7b45fp86

  },
  { // Entry 312
    0x1.c8d5a08be40c2p-117,
    0x1.c8d5a08be40c2p-117

  },
  { // Entry 313
    -0x1.c8d5a08be40c2p-117,
    -0x1.c8d5a08be40c2p-117

  },
  { // Entry 314
    0x1.e5dffd7f06cb3p-2,
    0x1.cad4e9827a2bep1

  },
  { // Entry 315
    -0x1.e5dffd7f06cb3p-2,
    -0x1.cad4e9827a2bep1

  },
  { // Entry 316
    0x1.e6be378b1b4ecp-2,
    0x1.caeb940e4b997p1

  },
  { // Entry 317
    -0x1.e6be378b1b4ecp-2,
    -0x1.caeb940e4b997p1

  },
  { // Entry 318
    0x1.e72bd025a1fd6p-2,
    0x1.caf6c04ecd034p1

  },
  { // Entry 319
    -0x1.e72bd025a1fd6p-2,
    -0x1.caf6c04ecd034p1

  },
  { // Entry 320
    0x1.e844b3d7cbe44p-2,
    0x1.cb135ec1c956ep1

  },
  { // Entry 321
    -0x1.e844b3d7cbe44p-2,
    -0x1.cb135ec1c956ep1

  },
  { // Entry 322
    0x1.dd38a1f1d289bp-54,
    0x1.cb44e86bc192bp648

  },
  { // Entry 323
    -0x1.dd38a1f1d289bp-54,
    -0x1.cb44e86bc192bp648

  },
  { // Entry 324
    0x1.dd38a1f1d289bp-53,
    0x1.cb44e86bc192bp649

  },
  { // Entry 325
    -0x1.dd38a1f1d289bp-53,
    -0x1.cb44e86bc192bp649

  },
  { // Entry 326
    -0x1.fff6e755320edp1,
    0x1.cb61afedb2b3cp119

  },
  { // Entry 327
    0x1.fff6e755320edp1,
    -0x1.cb61afedb2b3cp119

  },
  { // Entry 328
    0x1.ccdf4aa6c229p-7,
    0x1.ccd7834ba3804p-7

  },
  { // Entry 329
    -0x1.ccdf4aa6c229p-7,
    -0x1.ccd7834ba3804p-7

  },
  { // Entry 330
    0x1.cee50016fc2d9p-4,
    0x1.ccf0599da478ep-4

  },
  { // Entry 331
    -0x1.cee50016fc2d9p-4,
    -0x1.ccf0599da478ep-4

  },
  { // Entry 332
    0x1.44cf3ee8a75a8p0,
    0x1.ce8p-1

  },
  { // Entry 333
    -0x1.44cf3ee8a75a8p0,
    -0x1.ce8p-1

  },
  { // Entry 334
    0x1.45aa12ff98152p0,
    0x1.cf276c9cb9afp-1

  },
  { // Entry 335
    -0x1.45aa12ff98152p0,
    -0x1.cf276c9cb9afp-1

  },
  { // Entry 336
    0x1.f9bc744f61e0fp-4,
    0x1.d2e979148a458p61

  },
  { // Entry 337
    -0x1.f9bc744f61e0fp-4,
    -0x1.d2e979148a458p61

  },
  { // Entry 338
    0x1.6e70f9edbd1ap-2,
    0x1.d6b5ad6b5ab68p100

  },
  { // Entry 339
    -0x1.6e70f9edbd1ap-2,
    -0x1.d6b5ad6b5ab68p100

  },
  { // Entry 340
    0x1.13e9c6a348e4ap2,
    0x1.d96e058p488

  },
  { // Entry 341
    -0x1.13e9c6a348e4ap2,
    -0x1.d96e058p488

  },
  { // Entry 342
    -0x1.d355463c23036p-5,
    0x1.dd10f25171bc9p5

  },
  { // Entry 343
    0x1.d355463c23036p-5,
    -0x1.dd10f25171bc9p5

  },
  { // Entry 344
    0x1.ddf21ebf6fc93p-6,
    0x1.ddcf6e56696a4p-6

  },
  { // Entry 345
    -0x1.ddf21ebf6fc93p-6,
    -0x1.ddcf6e56696a4p-6

  },
  { // Entry 346
    0x1.5cb0bfc1558p0,
    0x1.ep-1

  },
  { // Entry 347
    -0x1.5cb0bfc1558p0,
    -0x1.ep-1

  },
  { // Entry 348
    -0x1.97b5e8ae9a21dp1,
    0x1.ep0

  },
  { // Entry 349
    0x1.97b5e8ae9a21dp1,
    -0x1.ep0

  },
  { // Entry 350
    -0x1.f8bf68d0c23f3p0,
    0x1.e779de779de71p118

  },
  { // Entry 351
    0x1.f8bf68d0c23f3p0,
    -0x1.e779de779de71p118

  },
  { // Entry 352
    -0x1.fffffffffd71dp-1,
    0x1.eb0567bed1f71p12

  },
  { // Entry 353
    0x1.fffffffffd71dp-1,
    -0x1.eb0567bed1f71p12

  },
  { // Entry 354
    0x1.ee788d1cc7955p-4,
    0x1.ec16f30a34fbcp-4

  },
  { // Entry 355
    -0x1.ee788d1cc7955p-4,
    -0x1.ec16f30a34fbcp-4

  },
  { // Entry 356
    0x1.efdad938b4b0fp-4,
    0x1.ed7424ba2aa4ep-4

  },
  { // Entry 357
    -0x1.efdad938b4b0fp-4,
    -0x1.ed7424ba2aa4ep-4

  },
  { // Entry 358
    0x1.da347607948f4p-1,
    0x1.ee9eda589039p499

  },
  { // Entry 359
    -0x1.da347607948f4p-1,
    -0x1.ee9eda589039p499

  },
  { // Entry 360
    0x1.f682d27a8be88p0,
    0x1.f4ff0d7b3ac1p100

  },
  { // Entry 361
    -0x1.f682d27a8be88p0,
    -0x1.f4ff0d7b3ac1p100

  },
  { // Entry 362
    -0x1.ffd36a753ced9p-1,
    0x1.f5a814afd69f5p119

  },
  { // Entry 363
    0x1.ffd36a753ced9p-1,
    -0x1.f5a814afd69f5p119

  },
  { // Entry 364
    -0x1.0fa7701d059b5p-9,
    0x1.f8fc824d2693bp61

  },
  { // Entry 365
    0x1.0fa7701d059b5p-9,
    -0x1.f8fc824d2693bp61

  },
  { // Entry 366
    -0x1.afe3875bd3afep-4,
    0x1.f9be6f9be6f98p9

  },
  { // Entry 367
    0x1.afe3875bd3afep-4,
    -0x1.f9be6f9be6f98p9

  },
  { // Entry 368
    0x1.fd1032e82deabp-4,
    0x1.fa775cd264f43p-4

  },
  { // Entry 369
    -0x1.fd1032e82deabp-4,
    -0x1.fa775cd264f43p-4

  },
  { // Entry 370
    0x1.fea8c67dd46cbp-4,
    0x1.fc09b47402d82p-4

  },
  { // Entry 371
    -0x1.fea8c67dd46cbp-4,
    -0x1.fc09b47402d82p-4

  },
  { // Entry 372
    -0x1.9020caf18a874p-1,
    0x1.feeffffffffc6p995

  },
  { // Entry 373
    0x1.9020caf18a874p-1,
    -0x1.feeffffffffc6p995

  },
  { // Entry 374
    0x1.af135beb0f281p0,
    0x1.ff01fffffffffp7

  },
  { // Entry 375
    -0x1.af135beb0f281p0,
    -0x1.ff01fffffffffp7

  },
  { // Entry 376
    -0x1.fd97532efd89bp3,
    0x1.ff8ffffffffffp540

  },
  { // Entry 377
    0x1.fd97532efd89bp3,
    -0x1.ff8ffffffffffp540

  },
  { // Entry 378
    -0x1.cc32cb9338181p-1,
    0x1.ff8ffffffffffp870

  },
  { // Entry 379
    0x1.cc32cb9338181p-1,
    -0x1.ff8ffffffffffp870

  },
  { // Entry 380
    -0x1.8659d3e2b52b8p-1,
    0x1.ffc1p9

  },
  { // Entry 381
    0x1.8659d3e2b52b8p-1,
    -0x1.ffc1p9

  },
  { // Entry 382
    -0x1.b66066fb812eep-2,
    0x1.ffcfff8p19

  },
  { // Entry 383
    0x1.b66066fb812eep-2,
    -0x1.ffcfff8p19

  },
  { // Entry 384
    -0x1.520ebd32e1d8fp1,
    0x1.ffcfff8p365

  },
  { // Entry 385
    0x1.520ebd32e1d8fp1,
    -0x1.ffcfff8p365

  },
  { // Entry 386
    0x1.489813c24d13bp0,
    0x1.ffcffffffff6cp720

  },
  { // Entry 387
    -0x1.489813c24d13bp0,
    -0x1.ffcffffffff6cp720

  },
  { // Entry 388
    0x1.413e63f7dd608p0,
    0x1.ffcfffffffff9p320

  },
  { // Entry 389
    -0x1.413e63f7dd608p0,
    -0x1.ffcfffffffff9p320

  },
  { // Entry 390
    -0x1.fc3928a39b652p-2,
    0x1.ffcffffffffffp990

  },
  { // Entry 391
    0x1.fc3928a39b652p-2,
    -0x1.ffcffffffffffp990

  },
  { // Entry 392
    -0x1.9ad70d284f16ep-1,
    0x1.ffeffffffffccp995

  },
  { // Entry 393
    0x1.9ad70d284f16ep-1,
    -0x1.ffeffffffffccp995

  },
  { // Entry 394
    0x1.aad6effcb6784p-3,
    0x1.ffefffffffffdp366

  },
  { // Entry 395
    -0x1.aad6effcb6784p-3,
    -0x1.ffefffffffffdp366

  },
  { // Entry 396
    0x1.c88645f9d119fp2,
    0x1.ffeffffffffffp180

  },
  { // Entry 397
    -0x1.c88645f9d119fp2,
    -0x1.ffeffffffffffp180

  },
  { // Entry 398
    0x1.131aa7b9d4aap2,
    0x1.ffeffffffffffp231

  },
  { // Entry 399
    -0x1.131aa7b9d4aap2,
    -0x1.ffeffffffffffp231

  },
  { // Entry 400
    0x1.f671719be50d2p-3,
    0x1.ffeffffffffffp1019

  },
  { // Entry 401
    -0x1.f671719be50d2p-3,
    -0x1.ffeffffffffffp1019

  },
  { // Entry 402
    0x1.ff078a2d2d872p-1,
    0x1.fff1fffffffffp40

  },
  { // Entry 403
    -0x1.ff078a2d2d872p-1,
    -0x1.fff1fffffffffp40

  },
  { // Entry 404
    0x1.0784b04fc42a6p9,
    0x1.fff1fffffffffp41

  },
  { // Entry 405
    -0x1.0784b04fc42a6p9,
    -0x1.fff1fffffffffp41

  },
  { // Entry 406
    0x1.8eb22dd167a37p0,
    0x1.fffff1fffffffp-1

  },
  { // Entry 407
    -0x1.8eb22dd167a37p0,
    -0x1.fffff1fffffffp-1

  },
  { // Entry 408
    -0x1.22e7346fd3ddap1,
    0x1.ffffff8p119

  },
  { // Entry 409
    0x1.22e7346fd3ddap1,
    -0x1.ffffff8p119

  },
  { // Entry 410
    -0x1.db0b3b019f176p-3,
    0x1.ffffff8p192

  },
  { // Entry 411
    0x1.db0b3b019f176p-3,
    -0x1.ffffff8p192

  },
  { // Entry 412
    0x1.06b6bede91026p-1,
    0x1.ffffff8p543

  },
  { // Entry 413
    -0x1.06b6bede91026p-1,
    -0x1.ffffff8p543

  },
  { // Entry 414
    -0x1.40f02a15dfa3dp1,
    0x1.ffffffffbbfffp40

  },
  { // Entry 415
    0x1.40f02a15dfa3dp1,
    -0x1.ffffffffbbfffp40

  },
  { // Entry 416
    0x1.ab60112ef4fdep-2,
    0x1.fffffffff7fffp231

  },
  { // Entry 417
    -0x1.ab60112ef4fdep-2,
    -0x1.fffffffff7fffp231

  },
  { // Entry 418
    0x1.35a9929eeafd7p0,
    0x1.fffffffffff78p920

  },
  { // Entry 419
    -0x1.35a9929eeafd7p0,
    -0x1.fffffffffff78p920

  },
  { // Entry 420
    0x1.4630298f3b993p0,
    0x1.fffffffffffd5p995

  },
  { // Entry 421
    -0x1.4630298f3b993p0,
    -0x1.fffffffffffd5p995

  },
  { // Entry 422
    -0x1.9472e045129fep-1,
    0x1.fffffffffffe8p720

  },
  { // Entry 423
    0x1.9472e045129fep-1,
    -0x1.fffffffffffe8p720

  },
  { // Entry 424
    0x1.42e586daa1b42p0,
    0x1.fffffffffffebp920

  },
  { // Entry 425
    -0x1.42e586daa1b42p0,
    -0x1.fffffffffffebp920

  },
  { // Entry 426
    -0x1.bfc436b94374cp-1,
    0x1.ffffffffffff1p245

  },
  { // Entry 427
    0x1.bfc436b94374cp-1,
    -0x1.ffffffffffff1p245

  },
  { // Entry 428
    0x1.35117d4a4f1e5p0,
    0x1.ffffffffffff4p845

  },
  { // Entry 429
    -0x1.35117d4a4f1e5p0,
    -0x1.ffffffffffff4p845

  },
  { // Entry 430
    -0x1.bfdd9292798aap-1,
    0x1.ffffffffffff4p1020

  },
  { // Entry 431
    0x1.bfdd9292798aap-1,
    -0x1.ffffffffffff4p1020

  },
  { // Entry 432
    -0x1.9b768ccdae6ecp9,
    0x1.ffffffffffffcp45

  },
  { // Entry 433
    0x1.9b768ccdae6ecp9,
    -0x1.ffffffffffffcp45

  },
  { // Entry 434
    0x1.feca047f2730fp-1,
    0x1.ffffffffffffcp474

  },
  { // Entry 435
    -0x1.feca047f2730fp-1,
    -0x1.ffffffffffffcp474

  },
  { // Entry 436
    -0x1.449f15cc94559p-2,
    0x1.ffffffffffffcp976

  },
  { // Entry 437
    0x1.449f15cc94559p-2,
    -0x1.ffffffffffffcp976

  },
  { // Entry 438
    0x1.fffc58da07952p-2,
    0x1.ffffffffffffep881

  },
  { // Entry 439
    -0x1.fffc58da07952p-2,
    -0x1.ffffffffffffep881

  },
  { // Entry 440
    -0x1.c1c9195ec23aap-1,
    0x1.ffffffffffffep970

  },
  { // Entry 441
    0x1.c1c9195ec23aap-1,
    -0x1.ffffffffffffep970

  },
  { // Entry 442
    0x1.3cc1ed3906d2fp-2,
    0x1.33328c1b37321p-2

  },
  { // Entry 443
    -0x1.3cc1ed3906d2fp-2,
    -0x1.33328c1b37321p-2

  },
  { // Entry 444
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 445
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 446
    -0x0.0p0,
    -0x0.0p0

  },
  { // Entry 447
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 448
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 449
    -0x1.0000000000001p-1022,
    -0x1.0000000000001p-1022

  },
  { // Entry 450
    0x1.0000000000001p-1022,
    0x1.0000000000001p-1022

  },
  { // Entry 451
    -0x1.0p-1022,
    -0x1.0p-1022

  },
  { // Entry 452
    0x1.0p-1022,
    0x1.0p-1022

  },
  { // Entry 453
    -0x0.fffffffffffffp-1022,
    -0x0.fffffffffffffp-1022

  },
  { // Entry 454
    0x0.fffffffffffffp-1022,
    0x0.fffffffffffffp-1022

  },
  { // Entry 455
    0x0.fffffffffffffp-1022,
    0x0.fffffffffffffp-1022

  },
  { // Entry 456
    -0x0.fffffffffffffp-1022,
    -0x0.fffffffffffffp-1022

  },
  { // Entry 457
    0x1.0p-1022,
    0x1.0p-1022

  },
  { // Entry 458
    -0x1.0p-1022,
    -0x1.0p-1022

  },
  { // Entry 459
    0x1.0000000000001p-1022,
    0x1.0000000000001p-1022

  },
  { // Entry 460
    -0x1.0000000000001p-1022,
    -0x1.0000000000001p-1022

  },
  { // Entry 461
    0x1.999999f0fb38cp-13,
    0x1.999999999999ap-13

  },
  { // Entry 462
    -0x1.999999f0fb38cp-13,
    -0x1.999999999999ap-13

  },
  { // Entry 463
    0x1.99999af720174p-12,
    0x1.999999999999ap-12

  },
  { // Entry 464
    -0x1.99999af720174p-12,
    -0x1.999999999999ap-12

  },
  { // Entry 465
    0x1.33333581062a4p-11,
    0x1.3333333333334p-11

  },
  { // Entry 466
    -0x1.33333581062a4p-11,
    -0x1.3333333333334p-11

  },
  { // Entry 467
    0x1.99999f0fb3a1p-11,
    0x1.999999999999ap-11

  },
  { // Entry 468
    -0x1.99999f0fb3a1p-11,
    -0x1.999999999999ap-11

  },
  { // Entry 469
    0x1.0000055555777p-10,
    0x1.0p-10

  },
  { // Entry 470
    -0x1.0000055555777p-10,
    -0x1.0p-10

  },
  { // Entry 471
    0x1.33333c6a7f4ecp-10,
    0x1.3333333333333p-10

  },
  { // Entry 472
    -0x1.33333c6a7f4ecp-10,
    -0x1.3333333333333p-10

  },
  { // Entry 473
    0x1.66667508e0a1bp-10,
    0x1.6666666666666p-10

  },
  { // Entry 474
    -0x1.66667508e0a1bp-10,
    -0x1.6666666666666p-10

  },
  { // Entry 475
    0x1.9999af7202c36p-10,
    0x1.9999999999999p-10

  },
  { // Entry 476
    -0x1.9999af7202c36p-10,
    -0x1.9999999999999p-10

  },
  { // Entry 477
    0x1.ccccebe76f103p-10,
    0x1.cccccccccccccp-10

  },
  { // Entry 478
    -0x1.ccccebe76f103p-10,
    -0x1.cccccccccccccp-10

  },
  { // Entry 479
    0x1.0667d5fcf3d08p-7,
    0x1.0666666666666p-7

  },
  { // Entry 480
    -0x1.0667d5fcf3d08p-7,
    -0x1.0666666666666p-7

  },
  { // Entry 481
    0x1.ccd4939d0ccd7p-7,
    0x1.cccccccccccccp-7

  },
  { // Entry 482
    -0x1.ccd4939d0ccd7p-7,
    -0x1.cccccccccccccp-7

  },
  { // Entry 483
    0x1.49a4fc02ad194p-6,
    0x1.4999999999999p-6

  },
  { // Entry 484
    -0x1.49a4fc02ad194p-6,
    -0x1.4999999999999p-6

  },
  { // Entry 485
    0x1.ace5ded5f6be7p-6,
    0x1.accccccccccccp-6

  },
  { // Entry 486
    -0x1.ace5ded5f6be7p-6,
    -0x1.accccccccccccp-6

  },
  { // Entry 487
    0x1.081767fd3cb68p-5,
    0x1.08p-5

  },
  { // Entry 488
    -0x1.081767fd3cb68p-5,
    -0x1.08p-5

  },
  { // Entry 489
    0x1.39c0d6dea66fbp-5,
    0x1.399999999999ap-5

  },
  { // Entry 490
    -0x1.39c0d6dea66fbp-5,
    -0x1.399999999999ap-5

  },
  { // Entry 491
    0x1.6b702b954bc1dp-5,
    0x1.6b33333333334p-5

  },
  { // Entry 492
    -0x1.6b702b954bc1dp-5,
    -0x1.6b33333333334p-5

  },
  { // Entry 493
    0x1.9d265618dd0c7p-5,
    0x1.9cccccccccccep-5

  },
  { // Entry 494
    -0x1.9d265618dd0c7p-5,
    -0x1.9cccccccccccep-5

  },
  { // Entry 495
    0x1.cee446e4cfd4cp-5,
    0x1.ce66666666666p-5

  },
  { // Entry 496
    -0x1.cee446e4cfd4cp-5,
    -0x1.ce66666666666p-5

  },
  { // Entry 497
    0x1.a1eaedd5a4314p-1,
    0x1.5e7fc4369bdadp-1

  },
  { // Entry 498
    -0x1.a1eaedd5a4314p-1,
    -0x1.5e7fc4369bdadp-1

  },
  { // Entry 499
    0x1.d93b8aad424dep1,
    0x1.4e7fc4369bdadp0

  },
  { // Entry 500
    -0x1.d93b8aad424dep1,
    -0x1.4e7fc4369bdadp0

  },
  { // Entry 501
    -0x1.563acf158c2ebp1,
    0x1.edbfa651e9c84p0

  },
  { // Entry 502
    0x1.563acf158c2ebp1,
    -0x1.edbfa651e9c84p0

  },
  { // Entry 503
    -0x1.576b77609f089p-1,
    0x1.467fc4369bdadp1

  },
  { // Entry 504
    0x1.576b77609f089p-1,
    -0x1.467fc4369bdadp1

  },
  { // Entry 505
    0x1.00155777aebf7p-5,
    0x1.961fb54442d18p1

  },
  { // Entry 506
    -0x1.00155777aebf7p-5,
    -0x1.961fb54442d18p1

  },
  { // Entry 507
    0x1.87e9966e7d22dp-1,
    0x1.e5bfa651e9c83p1

  },
  { // Entry 508
    -0x1.87e9966e7d22dp-1,
    -0x1.e5bfa651e9c83p1

  },
  { // Entry 509
    0x1.a49e7d8987851p1,
    0x1.1aafcbafc85f7p2

  },
  { // Entry 510
    -0x1.a49e7d8987851p1,
    -0x1.1aafcbafc85f7p2

  },
  { // Entry 511
    -0x1.79ced8156d041p1,
    0x1.427fc4369bdadp2

  },
  { // Entry 512
    0x1.79ced8156d041p1,
    -0x1.427fc4369bdadp2

  },
  { // Entry 513
    -0x1.6f1f65cd1e91bp-1,
    0x1.6a4fbcbd6f562p2

  },
  { // Entry 514
    0x1.6f1f65cd1e91bp-1,
    -0x1.6a4fbcbd6f562p2

  },
  { // Entry 515
    -0x1.67747d5f844e2p-1,
    0x1.6af2eff0a2896p2

  },
  { // Entry 516
    0x1.67747d5f844e2p-1,
    -0x1.6af2eff0a2896p2

  },
  { // Entry 517
    -0x1.626a258815d18p1,
    0x1.43c62a9d02414p2

  },
  { // Entry 518
    0x1.626a258815d18p1,
    -0x1.43c62a9d02414p2

  },
  { // Entry 519
    0x1.d6adaf80f8b05p1,
    0x1.1c99654961f92p2

  },
  { // Entry 520
    -0x1.d6adaf80f8b05p1,
    -0x1.1c99654961f92p2

  },
  { // Entry 521
    0x1.a94d1b21370d5p-1,
    0x1.ead93feb8361fp1

  },
  { // Entry 522
    -0x1.a94d1b21370d5p-1,
    -0x1.ead93feb8361fp1

  },
  { // Entry 523
    0x1.4cba9e7822234p-4,
    0x1.9c7fb54442d1ap1

  },
  { // Entry 524
    -0x1.4cba9e7822234p-4,
    -0x1.9c7fb54442d1ap1

  },
  { // Entry 525
    -0x1.2cb6d02634532p-1,
    0x1.4e262a9d02415p1

  },
  { // Entry 526
    0x1.2cb6d02634532p-1,
    -0x1.4e262a9d02415p1

  },
  { // Entry 527
    -0x1.18d9112308d5bp1,
    0x1.ff993feb8362p0

  },
  { // Entry 528
    0x1.18d9112308d5bp1,
    -0x1.ff993feb8362p0

  },
  { // Entry 529
    0x1.56fe0145cf29p2,
    0x1.62e62a9d02416p0

  },
  { // Entry 530
    -0x1.56fe0145cf29p2,
    -0x1.62e62a9d02416p0

  },
  { // Entry 531
    0x1.f4ad353aca454p-1,
    0x1.8c662a9d02419p-1

  },
  { // Entry 532
    -0x1.f4ad353aca454p-1,
    -0x1.8c662a9d02419p-1

  },
  { // Entry 533
    0x1.6a7e1f6407ee6p3,
    -0x1.a8aa1d11c44ffp0

  },
  { // Entry 534
    -0x1.6a7e1f6407ee6p3,
    0x1.a8aa1d11c44ffp0

  },
  { // Entry 535
    0x1.0d718cfc82464p6,
    -0x1.95ec8b9e03d54p0

  },
  { // Entry 536
    -0x1.0d718cfc82464p6,
    0x1.95ec8b9e03d54p0

  },
  { // Entry 537
    -0x1.11d87146c2d5ap4,
    -0x1.832efa2a435a9p0

  },
  { // Entry 538
    0x1.11d87146c2d5ap4,
    0x1.832efa2a435a9p0

  },
  { // Entry 539
    -0x1.e3a3729b3e86ep2,
    -0x1.707168b682dfep0

  },
  { // Entry 540
    0x1.e3a3729b3e86ep2,
    0x1.707168b682dfep0

  },
  { // Entry 541
    -0x1.3429e61a5d1f3p2,
    -0x1.5db3d742c2653p0

  },
  { // Entry 542
    0x1.3429e61a5d1f3p2,
    0x1.5db3d742c2653p0

  },
  { // Entry 543
    -0x1.c08caec5cf997p1,
    -0x1.4af645cf01ea8p0

  },
  { // Entry 544
    0x1.c08caec5cf997p1,
    0x1.4af645cf01ea8p0

  },
  { // Entry 545
    -0x1.5d603d751767fp1,
    -0x1.3838b45b416fdp0

  },
  { // Entry 546
    0x1.5d603d751767fp1,
    0x1.3838b45b416fdp0

  },
  { // Entry 547
    -0x1.1b48a35b1b278p1,
    -0x1.257b22e780f52p0

  },
  { // Entry 548
    0x1.1b48a35b1b278p1,
    0x1.257b22e780f52p0

  },
  { // Entry 549
    -0x1.d74caf9912dc8p0,
    -0x1.12bd9173c07abp0

  },
  { // Entry 550
    0x1.d74caf9912dc8p0,
    0x1.12bd9173c07abp0

  },
  { // Entry 551
    -0x1.6be702e1f6cd6p0,
    -0x1.ea5c3ed5b385p-1

  },
  { // Entry 552
    0x1.6be702e1f6cd6p0,
    0x1.ea5c3ed5b385p-1

  },
  { // Entry 553
    -0x1.4d0df1fc1d348p0,
    -0x1.d4b87dab670ap-1

  },
  { // Entry 554
    0x1.4d0df1fc1d348p0,
    0x1.d4b87dab670ap-1

  },
  { // Entry 555
    -0x1.316c8b068a7afp0,
    -0x1.bf14bc811a8fp-1

  },
  { // Entry 556
    0x1.316c8b068a7afp0,
    0x1.bf14bc811a8fp-1

  },
  { // Entry 557
    -0x1.1872a1aaa7e27p0,
    -0x1.a970fb56ce14p-1

  },
  { // Entry 558
    0x1.1872a1aaa7e27p0,
    0x1.a970fb56ce14p-1

  },
  { // Entry 559
    -0x1.01aeeed04cbb1p0,
    -0x1.93cd3a2c8199p-1

  },
  { // Entry 560
    0x1.01aeeed04cbb1p0,
    0x1.93cd3a2c8199p-1

  },
  { // Entry 561
    -0x1.d98e408ac2086p-1,
    -0x1.7e297902351ep-1

  },
  { // Entry 562
    0x1.d98e408ac2086p-1,
    0x1.7e297902351ep-1

  },
  { // Entry 563
    -0x1.b2e4750631c54p-1,
    -0x1.6885b7d7e8a3p-1

  },
  { // Entry 564
    0x1.b2e4750631c54p-1,
    0x1.6885b7d7e8a3p-1

  },
  { // Entry 565
    -0x1.8ee916392e046p-1,
    -0x1.52e1f6ad9c28p-1

  },
  { // Entry 566
    0x1.8ee916392e046p-1,
    0x1.52e1f6ad9c28p-1

  },
  { // Entry 567
    -0x1.6d395e495f77ep-1,
    -0x1.3d3e35834fadp-1

  },
  { // Entry 568
    0x1.6d395e495f77ep-1,
    0x1.3d3e35834fadp-1

  },
  { // Entry 569
    -0x1.24e3e017a098fp-1,
    -0x1.0a0b02501c799p-1

  },
  { // Entry 570
    0x1.24e3e017a098fp-1,
    0x1.0a0b02501c799p-1

  },
  { // Entry 571
    -0x1.fdbd5f0596bdcp-2,
    -0x1.d8f7208e6b82cp-2

  },
  { // Entry 572
    0x1.fdbd5f0596bdcp-2,
    0x1.d8f7208e6b82cp-2

  },
  { // Entry 573
    -0x1.b5f3d6afbe6f2p-2,
    -0x1.9dd83c7c9e126p-2

  },
  { // Entry 574
    0x1.b5f3d6afbe6f2p-2,
    0x1.9dd83c7c9e126p-2

  },
  { // Entry 575
    -0x1.71a0f98081eaap-2,
    -0x1.62b9586ad0a2p-2

  },
  { // Entry 576
    0x1.71a0f98081eaap-2,
    0x1.62b9586ad0a2p-2

  },
  { // Entry 577
    -0x1.301909a2c36e9p-2,
    -0x1.279a74590331ap-2

  },
  { // Entry 578
    0x1.301909a2c36e9p-2,
    0x1.279a74590331ap-2

  },
  { // Entry 579
    -0x1.e18e941cc7fd5p-3,
    -0x1.d8f7208e6b829p-3

  },
  { // Entry 580
    0x1.e18e941cc7fd5p-3,
    0x1.d8f7208e6b829p-3

  },
  { // Entry 581
    -0x1.6650784bbdccp-3,
    -0x1.62b9586ad0a1ep-3

  },
  { // Entry 582
    0x1.6650784bbdccp-3,
    0x1.62b9586ad0a1ep-3

  },
  { // Entry 583
    -0x1.db142468cdafcp-4,
    -0x1.d8f7208e6b826p-4

  },
  { // Entry 584
    0x1.db142468cdafcp-4,
    0x1.d8f7208e6b826p-4

  },
  { // Entry 585
    -0x1.d97dd6d2e53f3p-5,
    -0x1.d8f7208e6b82dp-5

  },
  { // Entry 586
    0x1.d97dd6d2e53f3p-5,
    0x1.d8f7208e6b82dp-5

  },
  { // Entry 587
    0x1.d97dd6d2e53f3p-5,
    0x1.d8f7208e6b82dp-5

  },
  { // Entry 588
    -0x1.d97dd6d2e53f3p-5,
    -0x1.d8f7208e6b82dp-5

  },
  { // Entry 589
    0x1.db142468cdb03p-4,
    0x1.d8f7208e6b82dp-4

  },
  { // Entry 590
    -0x1.db142468cdb03p-4,
    -0x1.d8f7208e6b82dp-4

  },
  { // Entry 591
    0x1.6650784bbdcc4p-3,
    0x1.62b9586ad0a22p-3

  },
  { // Entry 592
    -0x1.6650784bbdcc4p-3,
    -0x1.62b9586ad0a22p-3

  },
  { // Entry 593
    0x1.e18e941cc7fd9p-3,
    0x1.d8f7208e6b82dp-3

  },
  { // Entry 594
    -0x1.e18e941cc7fd9p-3,
    -0x1.d8f7208e6b82dp-3

  },
  { // Entry 595
    0x1.301909a2c36ebp-2,
    0x1.279a74590331cp-2

  },
  { // Entry 596
    -0x1.301909a2c36ebp-2,
    -0x1.279a74590331cp-2

  },
  { // Entry 597
    0x1.71a0f98081eacp-2,
    0x1.62b9586ad0a22p-2

  },
  { // Entry 598
    -0x1.71a0f98081eacp-2,
    -0x1.62b9586ad0a22p-2

  },
  { // Entry 599
    0x1.b5f3d6afbe6f5p-2,
    0x1.9dd83c7c9e128p-2

  },
  { // Entry 600
    -0x1.b5f3d6afbe6f5p-2,
    -0x1.9dd83c7c9e128p-2

  },
  { // Entry 601
    0x1.fdbd5f0596bdfp-2,
    0x1.d8f7208e6b82ep-2

  },
  { // Entry 602
    -0x1.fdbd5f0596bdfp-2,
    -0x1.d8f7208e6b82ep-2

  },
  { // Entry 603
    0x1.24e3e017a098fp-1,
    0x1.0a0b02501c799p-1

  },
  { // Entry 604
    -0x1.24e3e017a098fp-1,
    -0x1.0a0b02501c799p-1

  },
  { // Entry 605
    0x1.6d395e495f778p-1,
    0x1.3d3e35834faccp-1

  },
  { // Entry 606
    -0x1.6d395e495f778p-1,
    -0x1.3d3e35834faccp-1

  },
  { // Entry 607
    0x1.8ee916392e03fp-1,
    0x1.52e1f6ad9c27cp-1

  },
  { // Entry 608
    -0x1.8ee916392e03fp-1,
    -0x1.52e1f6ad9c27cp-1

  },
  { // Entry 609
    0x1.b2e4750631c4dp-1,
    0x1.6885b7d7e8a2cp-1

  },
  { // Entry 610
    -0x1.b2e4750631c4dp-1,
    -0x1.6885b7d7e8a2cp-1

  },
  { // Entry 611
    0x1.d98e408ac207ep-1,
    0x1.7e297902351dcp-1

  },
  { // Entry 612
    -0x1.d98e408ac207ep-1,
    -0x1.7e297902351dcp-1

  },
  { // Entry 613
    0x1.01aeeed04cbadp0,
    0x1.93cd3a2c8198cp-1

  },
  { // Entry 614
    -0x1.01aeeed04cbadp0,
    -0x1.93cd3a2c8198cp-1

  },
  { // Entry 615
    0x1.1872a1aaa7e22p0,
    0x1.a970fb56ce13cp-1

  },
  { // Entry 616
    -0x1.1872a1aaa7e22p0,
    -0x1.a970fb56ce13cp-1

  },
  { // Entry 617
    0x1.316c8b068a7aap0,
    0x1.bf14bc811a8ecp-1

  },
  { // Entry 618
    -0x1.316c8b068a7aap0,
    -0x1.bf14bc811a8ecp-1

  },
  { // Entry 619
    0x1.4d0df1fc1d343p0,
    0x1.d4b87dab6709cp-1

  },
  { // Entry 620
    -0x1.4d0df1fc1d343p0,
    -0x1.d4b87dab6709cp-1

  },
  { // Entry 621
    0x1.6be702e1f6cdp0,
    0x1.ea5c3ed5b384cp-1

  },
  { // Entry 622
    -0x1.6be702e1f6cdp0,
    -0x1.ea5c3ed5b384cp-1

  },
  { // Entry 623
    0x1.d74caf9912dc8p0,
    0x1.12bd9173c07abp0

  },
  { // Entry 624
    -0x1.d74caf9912dc8p0,
    -0x1.12bd9173c07abp0

  },
  { // Entry 625
    0x1.1b48a35b1b284p1,
    0x1.257b22e780f56p0

  },
  { // Entry 626
    -0x1.1b48a35b1b284p1,
    -0x1.257b22e780f56p0

  },
  { // Entry 627
    0x1.5d603d751769p1,
    0x1.3838b45b41701p0

  },
  { // Entry 628
    -0x1.5d603d751769p1,
    -0x1.3838b45b41701p0

  },
  { // Entry 629
    0x1.c08caec5cf9b2p1,
    0x1.4af645cf01eacp0

  },
  { // Entry 630
    -0x1.c08caec5cf9b2p1,
    -0x1.4af645cf01eacp0

  },
  { // Entry 631
    0x1.3429e61a5d20bp2,
    0x1.5db3d742c2657p0

  },
  { // Entry 632
    -0x1.3429e61a5d20bp2,
    -0x1.5db3d742c2657p0

  },
  { // Entry 633
    0x1.e3a3729b3e8a8p2,
    0x1.707168b682e02p0

  },
  { // Entry 634
    -0x1.e3a3729b3e8a8p2,
    -0x1.707168b682e02p0

  },
  { // Entry 635
    0x1.11d87146c2da4p4,
    0x1.832efa2a435adp0

  },
  { // Entry 636
    -0x1.11d87146c2da4p4,
    -0x1.832efa2a435adp0

  },
  { // Entry 637
    -0x1.0d718cfc82349p6,
    0x1.95ec8b9e03d58p0

  },
  { // Entry 638
    0x1.0d718cfc82349p6,
    -0x1.95ec8b9e03d58p0

  },
  { // Entry 639
    -0x1.6a7e1f6407ee6p3,
    0x1.a8aa1d11c44ffp0

  },
  { // Entry 640
    0x1.6a7e1f6407ee6p3,
    -0x1.a8aa1d11c44ffp0

  },
  { // Entry 641
    0x1.9f39ea5bbe475p0,
    0x1.04aff6d330942p0

  },
  { // Entry 642
    -0x1.9f39ea5bbe475p0,
    -0x1.04aff6d330942p0

  },
  { // Entry 643
    0x1.9f3c4b8469f85p0,
    0x1.04b09e98dcdb4p0

  },
  { // Entry 644
    -0x1.9f3c4b8469f85p0,
    -0x1.04b09e98dcdb4p0

  },
  { // Entry 645
    0x1.9f3eacb224c21p0,
    0x1.04b1465e89226p0

  },
  { // Entry 646
    -0x1.9f3eacb224c21p0,
    -0x1.04b1465e89226p0

  },
  { // Entry 647
    0x1.9f410de4eeb69p0,
    0x1.04b1ee2435698p0

  },
  { // Entry 648
    -0x1.9f410de4eeb69p0,
    -0x1.04b1ee2435698p0

  },
  { // Entry 649
    0x1.9f436f1cc7e82p0,
    0x1.04b295e9e1b0ap0

  },
  { // Entry 650
    -0x1.9f436f1cc7e82p0,
    -0x1.04b295e9e1b0ap0

  },
  { // Entry 651
    0x1.9f45d059b068fp0,
    0x1.04b33daf8df7cp0

  },
  { // Entry 652
    -0x1.9f45d059b068fp0,
    -0x1.04b33daf8df7cp0

  },
  { // Entry 653
    0x1.9f48319ba84b2p0,
    0x1.04b3e5753a3eep0

  },
  { // Entry 654
    -0x1.9f48319ba84b2p0,
    -0x1.04b3e5753a3eep0

  },
  { // Entry 655
    0x1.9f4a92e2afa0dp0,
    0x1.04b48d3ae686p0

  },
  { // Entry 656
    -0x1.9f4a92e2afa0dp0,
    -0x1.04b48d3ae686p0

  },
  { // Entry 657
    0x1.9f4cf42ec67bbp0,
    0x1.04b5350092ccfp0

  },
  { // Entry 658
    -0x1.9f4cf42ec67bbp0,
    -0x1.04b5350092ccfp0

  },
  { // Entry 659
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 660
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 661
    -0x0.0p0,
    -0x0.0p0

  },
  { // Entry 662
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 663
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 664
    0x1.4d82b68cac19ep-1,
    0x1.279a74590331bp-1

  },
  { // Entry 665
    -0x1.4d82b68cac19ep-1,
    -0x1.279a74590331bp-1

  },
  { // Entry 666
    0x1.4d82b68cac1ap-1,
    0x1.279a74590331cp-1

  },
  { // Entry 667
    -0x1.4d82b68cac1ap-1,
    -0x1.279a74590331cp-1

  },
  { // Entry 668
    0x1.4d82b68cac1a1p-1,
    0x1.279a74590331dp-1

  },
  { // Entry 669
    -0x1.4d82b68cac1a1p-1,
    -0x1.279a74590331dp-1

  },
  { // Entry 670
    -0x1.89712eeca32bfp2,
    0x1.bb67ae8584ca9p0

  },
  { // Entry 671
    0x1.89712eeca32bfp2,
    -0x1.bb67ae8584ca9p0

  },
  { // Entry 672
    -0x1.89712eeca32b5p2,
    0x1.bb67ae8584caap0

  },
  { // Entry 673
    0x1.89712eeca32b5p2,
    -0x1.bb67ae8584caap0

  },
  { // Entry 674
    -0x1.89712eeca32abp2,
    0x1.bb67ae8584cabp0

  },
  { // Entry 675
    0x1.89712eeca32abp2,
    -0x1.bb67ae8584cabp0

  },
  { // Entry 676
    0x1.def49eaab37ap-2,
    0x1.bffffffffffffp-2

  },
  { // Entry 677
    -0x1.def49eaab37ap-2,
    -0x1.bffffffffffffp-2

  },
  { // Entry 678
    0x1.def49eaab37a1p-2,
    0x1.cp-2

  },
  { // Entry 679
    -0x1.def49eaab37a1p-2,
    -0x1.cp-2

  },
  { // Entry 680
    0x1.def49eaab37a3p-2,
    0x1.c000000000001p-2

  },
  { // Entry 681
    -0x1.def49eaab37a3p-2,
    -0x1.c000000000001p-2

  },
  { // Entry 682
    0x1.a46cb2be6a0bp-1,
    0x1.5ffffffffffffp-1

  },
  { // Entry 683
    -0x1.a46cb2be6a0bp-1,
    -0x1.5ffffffffffffp-1

  },
  { // Entry 684
    0x1.a46cb2be6a0b2p-1,
    0x1.6p-1

  },
  { // Entry 685
    -0x1.a46cb2be6a0b2p-1,
    -0x1.6p-1

  },
  { // Entry 686
    0x1.a46cb2be6a0b4p-1,
    0x1.6000000000001p-1

  },
  { // Entry 687
    -0x1.a46cb2be6a0b4p-1,
    -0x1.6000000000001p-1

  },
  { // Entry 688
    0x1.3d6dc956eac7ap1,
    0x1.2ffffffffffffp0

  },
  { // Entry 689
    -0x1.3d6dc956eac7ap1,
    -0x1.2ffffffffffffp0

  },
  { // Entry 690
    0x1.3d6dc956eac7dp1,
    0x1.3p0

  },
  { // Entry 691
    -0x1.3d6dc956eac7dp1,
    -0x1.3p0

  },
  { // Entry 692
    0x1.3d6dc956eac81p1,
    0x1.3000000000001p0

  },
  { // Entry 693
    -0x1.3d6dc956eac81p1,
    -0x1.3000000000001p0

  },
  { // Entry 694
    -0x1.b2d89a9382954p-1,
    0x1.37fffffffffffp1

  },
  { // Entry 695
    0x1.b2d89a9382954p-1,
    -0x1.37fffffffffffp1

  },
  { // Entry 696
    -0x1.b2d89a938294cp-1,
    0x1.38p1

  },
  { // Entry 697
    0x1.b2d89a938294cp-1,
    -0x1.38p1

  },
  { // Entry 698
    -0x1.b2d89a9382946p-1,
    0x1.3800000000001p1

  },
  { // Entry 699
    0x1.b2d89a9382946p-1,
    -0x1.3800000000001p1

  },
  { // Entry 700
    0x1.06f8d014bf084p-4,
    0x1.069c8b46b3792p-4

  },
  { // Entry 701
    -0x1.06f8d014bf084p-4,
    -0x1.069c8b46b3792p-4

  },
  { // Entry 702
    0x1.080f73b07051ep-3,
    0x1.069c8b46b3792p-3

  },
  { // Entry 703
    -0x1.080f73b07051ep-3,
    -0x1.069c8b46b3792p-3

  },
  { // Entry 704
    0x1.8ed9142fc9189p-3,
    0x1.89ead0ea0d35bp-3

  },
  { // Entry 705
    -0x1.8ed9142fc9189p-3,
    -0x1.89ead0ea0d35bp-3

  },
  { // Entry 706
    0x1.0c864083d1e7dp-2,
    0x1.069c8b46b3792p-2

  },
  { // Entry 707
    -0x1.0c864083d1e7dp-2,
    -0x1.069c8b46b3792p-2

  },
  { // Entry 708
    0x1.53fdcdfd37f04p-2,
    0x1.4843ae1860576p-2

  },
  { // Entry 709
    -0x1.53fdcdfd37f04p-2,
    -0x1.4843ae1860576p-2

  },
  { // Entry 710
    0x1.9e9257e6b62a1p-2,
    0x1.89ead0ea0d35ap-2

  },
  { // Entry 711
    -0x1.9e9257e6b62a1p-2,
    -0x1.89ead0ea0d35ap-2

  },
  { // Entry 712
    0x1.ed21e59972b08p-2,
    0x1.cb91f3bbba13ep-2

  },
  { // Entry 713
    -0x1.ed21e59972b08p-2,
    -0x1.cb91f3bbba13ep-2

  },
  { // Entry 714
    0x1.205a6009d5e3ep-1,
    0x1.069c8b46b3791p-1

  },
  { // Entry 715
    -0x1.205a6009d5e3ep-1,
    -0x1.069c8b46b3791p-1

  },
  { // Entry 716
    0x1.4d466accad48ep-1,
    0x1.27701caf89e83p-1

  },
  { // Entry 717
    -0x1.4d466accad48ep-1,
    -0x1.27701caf89e83p-1

  },
  { // Entry 718
    0x1.7e1d936f4d615p-1,
    0x1.4843ae1860575p-1

  },
  { // Entry 719
    -0x1.7e1d936f4d615p-1,
    -0x1.4843ae1860575p-1

  },
  { // Entry 720
    0x1.b3df386f18228p-1,
    0x1.69173f8136c67p-1

  },
  { // Entry 721
    -0x1.b3df386f18228p-1,
    -0x1.69173f8136c67p-1

  },
  { // Entry 722
    0x1.efd82742d778cp-1,
    0x1.89ead0ea0d359p-1

  },
  { // Entry 723
    -0x1.efd82742d778cp-1,
    -0x1.89ead0ea0d359p-1

  },
  { // Entry 724
    0x1.19e26b5ecd5cp0,
    0x1.aabe6252e3a4bp-1

  },
  { // Entry 725
    -0x1.19e26b5ecd5cp0,
    -0x1.aabe6252e3a4bp-1

  },
  { // Entry 726
    0x1.41038b70be0fap0,
    0x1.cb91f3bbba13dp-1

  },
  { // Entry 727
    -0x1.41038b70be0fap0,
    -0x1.cb91f3bbba13dp-1

  },
  { // Entry 728
    0x1.6efec8c1e493bp0,
    0x1.ec6585249082fp-1

  },
  { // Entry 729
    -0x1.6efec8c1e493bp0,
    -0x1.ec6585249082fp-1

  },
  { // Entry 730
    0x1.a64c7c9d65346p0,
    0x1.069c8b46b3791p0

  },
  { // Entry 731
    -0x1.a64c7c9d65346p0,
    -0x1.069c8b46b3791p0

  },
  { // Entry 732
    0x1.eab43e0e5e87cp0,
    0x1.170653fb1eb0ap0

  },
  { // Entry 733
    -0x1.eab43e0e5e87cp0,
    -0x1.170653fb1eb0ap0

  },
  { // Entry 734
    0x1.21277b97c0137p1,
    0x1.27701caf89e83p0

  },
  { // Entry 735
    -0x1.21277b97c0137p1,
    -0x1.27701caf89e83p0

  },
  { // Entry 736
    0x1.5bd13dda077cdp1,
    0x1.37d9e563f51fcp0

  },
  { // Entry 737
    -0x1.5bd13dda077cdp1,
    -0x1.37d9e563f51fcp0

  },
  { // Entry 738
    0x1.af4643e9371f9p1,
    0x1.4843ae1860575p0

  },
  { // Entry 739
    -0x1.af4643e9371f9p1,
    -0x1.4843ae1860575p0

  },
  { // Entry 740
    0x1.1866fe845e75ap2,
    0x1.58ad76cccb8eep0

  },
  { // Entry 741
    -0x1.1866fe845e75ap2,
    -0x1.58ad76cccb8eep0

  },
  { // Entry 742
    0x1.8bdcd54bd5981p2,
    0x1.69173f8136c67p0

  },
  { // Entry 743
    -0x1.8bdcd54bd5981p2,
    -0x1.69173f8136c67p0

  },
  { // Entry 744
    0x1.4bb6a64e0625cp3,
    0x1.79810835a1fep0

  },
  { // Entry 745
    -0x1.4bb6a64e0625cp3,
    -0x1.79810835a1fep0

  },
  { // Entry 746
    0x1.f2f05cfb656a4p4,
    0x1.89ead0ea0d359p0

  },
  { // Entry 747
    -0x1.f2f05cfb656a4p4,
    -0x1.89ead0ea0d359p0

  },
  { // Entry 748
    -0x1.f2f05cfb657f5p4,
    0x1.9a54999e786d2p0

  },
  { // Entry 749
    0x1.f2f05cfb657f5p4,
    -0x1.9a54999e786d2p0

  },
  { // Entry 750
    -0x1.4bb6a64e062a7p3,
    0x1.aabe6252e3a4bp0

  },
  { // Entry 751
    0x1.4bb6a64e062a7p3,
    -0x1.aabe6252e3a4bp0

  },
  { // Entry 752
    -0x1.8bdcd54bd59b7p2,
    0x1.bb282b074edc4p0

  },
  { // Entry 753
    0x1.8bdcd54bd59b7p2,
    -0x1.bb282b074edc4p0

  },
  { // Entry 754
    -0x1.1866fe845e776p2,
    0x1.cb91f3bbba13dp0

  },
  { // Entry 755
    0x1.1866fe845e776p2,
    -0x1.cb91f3bbba13dp0

  },
  { // Entry 756
    -0x1.af4643e93721bp1,
    0x1.dbfbbc70254b6p0

  },
  { // Entry 757
    0x1.af4643e93721bp1,
    -0x1.dbfbbc70254b6p0

  },
  { // Entry 758
    -0x1.5bd13dda077e4p1,
    0x1.ec6585249082fp0

  },
  { // Entry 759
    0x1.5bd13dda077e4p1,
    -0x1.ec6585249082fp0

  },
  { // Entry 760
    -0x1.21277b97c0148p1,
    0x1.fccf4dd8fbba8p0

  },
  { // Entry 761
    0x1.21277b97c0148p1,
    -0x1.fccf4dd8fbba8p0

  },
  { // Entry 762
    -0x1.eab43e0e5e891p0,
    0x1.069c8b46b3791p1

  },
  { // Entry 763
    0x1.eab43e0e5e891p0,
    -0x1.069c8b46b3791p1

  },
  { // Entry 764
    -0x1.a64c7c9d65353p0,
    0x1.0ed16fa0e914ep1

  },
  { // Entry 765
    0x1.a64c7c9d65353p0,
    -0x1.0ed16fa0e914ep1

  },
  { // Entry 766
    -0x1.6efec8c1e4945p0,
    0x1.170653fb1eb0bp1

  },
  { // Entry 767
    0x1.6efec8c1e4945p0,
    -0x1.170653fb1eb0bp1

  },
  { // Entry 768
    -0x1.41038b70be1p0,
    0x1.1f3b3855544c8p1

  },
  { // Entry 769
    0x1.41038b70be1p0,
    -0x1.1f3b3855544c8p1

  },
  { // Entry 770
    -0x1.19e26b5ecd5c3p0,
    0x1.27701caf89e85p1

  },
  { // Entry 771
    0x1.19e26b5ecd5c3p0,
    -0x1.27701caf89e85p1

  },
  { // Entry 772
    -0x1.efd82742d778cp-1,
    0x1.2fa50109bf842p1

  },
  { // Entry 773
    0x1.efd82742d778cp-1,
    -0x1.2fa50109bf842p1

  },
  { // Entry 774
    -0x1.b3df386f18225p-1,
    0x1.37d9e563f51ffp1

  },
  { // Entry 775
    0x1.b3df386f18225p-1,
    -0x1.37d9e563f51ffp1

  },
  { // Entry 776
    -0x1.7e1d936f4d60fp-1,
    0x1.400ec9be2abbcp1

  },
  { // Entry 777
    0x1.7e1d936f4d60fp-1,
    -0x1.400ec9be2abbcp1

  },
  { // Entry 778
    -0x1.4d466accad486p-1,
    0x1.4843ae1860579p1

  },
  { // Entry 779
    0x1.4d466accad486p-1,
    -0x1.4843ae1860579p1

  },
  { // Entry 780
    -0x1.205a6009d5e34p-1,
    0x1.5078927295f36p1

  },
  { // Entry 781
    0x1.205a6009d5e34p-1,
    -0x1.5078927295f36p1

  },
  { // Entry 782
    -0x1.ed21e59972aefp-2,
    0x1.58ad76cccb8f3p1

  },
  { // Entry 783
    0x1.ed21e59972aefp-2,
    -0x1.58ad76cccb8f3p1

  },
  { // Entry 784
    -0x1.9e9257e6b6285p-2,
    0x1.60e25b27012bp1

  },
  { // Entry 785
    0x1.9e9257e6b6285p-2,
    -0x1.60e25b27012bp1

  },
  { // Entry 786
    -0x1.53fdcdfd37ee5p-2,
    0x1.69173f8136c6dp1

  },
  { // Entry 787
    0x1.53fdcdfd37ee5p-2,
    -0x1.69173f8136c6dp1

  },
  { // Entry 788
    -0x1.0c864083d1e5bp-2,
    0x1.714c23db6c62ap1

  },
  { // Entry 789
    0x1.0c864083d1e5bp-2,
    -0x1.714c23db6c62ap1

  },
  { // Entry 790
    -0x1.8ed9142fc913fp-3,
    0x1.79810835a1fe7p1

  },
  { // Entry 791
    0x1.8ed9142fc913fp-3,
    -0x1.79810835a1fe7p1

  },
  { // Entry 792
    -0x1.080f73b0704cfp-3,
    0x1.81b5ec8fd79a4p1

  },
  { // Entry 793
    0x1.080f73b0704cfp-3,
    -0x1.81b5ec8fd79a4p1

  },
  { // Entry 794
    -0x1.06f8d014bf09bp-4,
    0x1.89ead0ea0d35bp1

  },
  { // Entry 795
    0x1.06f8d014bf09bp-4,
    -0x1.89ead0ea0d35bp1

  },
  { // Entry 796
    0x1.0c864083d1e7fp-2,
    -0x1.81b5ec8fd799fp2

  },
  { // Entry 797
    -0x1.0c864083d1e7fp-2,
    0x1.81b5ec8fd799fp2

  },
  { // Entry 798
    0x1.205a6009d5e3fp-1,
    -0x1.714c23db6c626p2

  },
  { // Entry 799
    -0x1.205a6009d5e3fp-1,
    0x1.714c23db6c626p2

  },
  { // Entry 800
    0x1.efd82742d778ep-1,
    -0x1.60e25b27012adp2

  },
  { // Entry 801
    -0x1.efd82742d778ep-1,
    0x1.60e25b27012adp2

  },
  { // Entry 802
    0x1.a64c7c9d65347p0,
    -0x1.5078927295f34p2

  },
  { // Entry 803
    -0x1.a64c7c9d65347p0,
    0x1.5078927295f34p2

  },
  { // Entry 804
    0x1.af4643e9371f9p1,
    -0x1.400ec9be2abbbp2

  },
  { // Entry 805
    -0x1.af4643e9371f9p1,
    0x1.400ec9be2abbbp2

  },
  { // Entry 806
    0x1.f2f05cfb656aap4,
    -0x1.2fa50109bf842p2

  },
  { // Entry 807
    -0x1.f2f05cfb656aap4,
    0x1.2fa50109bf842p2

  },
  { // Entry 808
    -0x1.1866fe845e775p2,
    -0x1.1f3b3855544c9p2

  },
  { // Entry 809
    0x1.1866fe845e775p2,
    0x1.1f3b3855544c9p2

  },
  { // Entry 810
    -0x1.eab43e0e5e896p0,
    -0x1.0ed16fa0e915p2

  },
  { // Entry 811
    0x1.eab43e0e5e896p0,
    0x1.0ed16fa0e915p2

  },
  { // Entry 812
    -0x1.19e26b5ecd5cdp0,
    -0x1.fccf4dd8fbbaep1

  },
  { // Entry 813
    0x1.19e26b5ecd5cdp0,
    0x1.fccf4dd8fbbaep1

  },
  { // Entry 814
    -0x1.4d466accad49fp-1,
    -0x1.dbfbbc70254bcp1

  },
  { // Entry 815
    0x1.4d466accad49fp-1,
    0x1.dbfbbc70254bcp1

  },
  { // Entry 816
    -0x1.53fdcdfd37f1fp-2,
    -0x1.bb282b074edcap1

  },
  { // Entry 817
    0x1.53fdcdfd37f1fp-2,
    0x1.bb282b074edcap1

  },
  { // Entry 818
    -0x1.06f8d014bf0e9p-4,
    -0x1.9a54999e786d8p1

  },
  { // Entry 819
    0x1.06f8d014bf0e9p-4,
    0x1.9a54999e786d8p1

  },
  { // Entry 820
    0x1.8ed9142fc915p-3,
    -0x1.79810835a1fe6p1

  },
  { // Entry 821
    -0x1.8ed9142fc915p-3,
    0x1.79810835a1fe6p1

  },
  { // Entry 822
    0x1.ed21e59972ae6p-2,
    -0x1.58ad76cccb8f4p1

  },
  { // Entry 823
    -0x1.ed21e59972ae6p-2,
    0x1.58ad76cccb8f4p1

  },
  { // Entry 824
    0x1.b3df386f1821p-1,
    -0x1.37d9e563f5202p1

  },
  { // Entry 825
    -0x1.b3df386f1821p-1,
    0x1.37d9e563f5202p1

  },
  { // Entry 826
    0x1.6efec8c1e4926p0,
    -0x1.170653fb1eb1p1

  },
  { // Entry 827
    -0x1.6efec8c1e4926p0,
    0x1.170653fb1eb1p1

  },
  { // Entry 828
    0x1.5bd13dda077aep1,
    -0x1.ec6585249083cp0

  },
  { // Entry 829
    -0x1.5bd13dda077aep1,
    0x1.ec6585249083cp0

  },
  { // Entry 830
    0x1.4bb6a64e061f7p3,
    -0x1.aabe6252e3a58p0

  },
  { // Entry 831
    -0x1.4bb6a64e061f7p3,
    0x1.aabe6252e3a58p0

  },
  { // Entry 832
    -0x1.8bdcd54bd5ap2,
    -0x1.69173f8136c74p0

  },
  { // Entry 833
    0x1.8bdcd54bd5ap2,
    0x1.69173f8136c74p0

  },
  { // Entry 834
    -0x1.21277b97c015fp1,
    -0x1.27701caf89e9p0

  },
  { // Entry 835
    0x1.21277b97c015fp1,
    0x1.27701caf89e9p0

  },
  { // Entry 836
    -0x1.41038b70be11bp0,
    -0x1.cb91f3bbba157p-1

  },
  { // Entry 837
    0x1.41038b70be11bp0,
    0x1.cb91f3bbba157p-1

  },
  { // Entry 838
    -0x1.7e1d936f4d63cp-1,
    -0x1.4843ae186058ep-1

  },
  { // Entry 839
    0x1.7e1d936f4d63cp-1,
    0x1.4843ae186058ep-1

  },
  { // Entry 840
    -0x1.9e9257e6b62d9p-2,
    -0x1.89ead0ea0d38ap-2

  },
  { // Entry 841
    0x1.9e9257e6b62d9p-2,
    0x1.89ead0ea0d38ap-2

  },
  { // Entry 842
    -0x1.080f73b07057ep-3,
    -0x1.069c8b46b37fp-3

  },
  { // Entry 843
    0x1.080f73b07057ep-3,
    0x1.069c8b46b37fp-3

  },
  { // Entry 844
    0x1.080f73b0704bfp-3,
    0x1.069c8b46b3734p-3

  },
  { // Entry 845
    -0x1.080f73b0704bfp-3,
    -0x1.069c8b46b3734p-3

  },
  { // Entry 846
    0x1.9e9257e6b626bp-2,
    0x1.89ead0ea0d32cp-2

  },
  { // Entry 847
    -0x1.9e9257e6b626bp-2,
    -0x1.89ead0ea0d32cp-2

  },
  { // Entry 848
    0x1.7e1d936f4d5f3p-1,
    0x1.4843ae186055fp-1

  },
  { // Entry 849
    -0x1.7e1d936f4d5f3p-1,
    -0x1.4843ae186055fp-1

  },
  { // Entry 850
    0x1.41038b70be0dfp0,
    0x1.cb91f3bbba128p-1

  },
  { // Entry 851
    -0x1.41038b70be0dfp0,
    -0x1.cb91f3bbba128p-1

  },
  { // Entry 852
    0x1.21277b97c0116p1,
    0x1.27701caf89e78p0

  },
  { // Entry 853
    -0x1.21277b97c0116p1,
    -0x1.27701caf89e78p0

  },
  { // Entry 854
    0x1.8bdcd54bd5915p2,
    0x1.69173f8136c5cp0

  },
  { // Entry 855
    -0x1.8bdcd54bd5915p2,
    -0x1.69173f8136c5cp0

  },
  { // Entry 856
    -0x1.4bb6a64e0633cp3,
    0x1.aabe6252e3a4p0

  },
  { // Entry 857
    0x1.4bb6a64e0633cp3,
    -0x1.aabe6252e3a4p0

  },
  { // Entry 858
    -0x1.5bd13dda07812p1,
    0x1.ec65852490824p0

  },
  { // Entry 859
    0x1.5bd13dda07812p1,
    -0x1.ec65852490824p0

  },
  { // Entry 860
    -0x1.6efec8c1e497p0,
    0x1.170653fb1eb04p1

  },
  { // Entry 861
    0x1.6efec8c1e497p0,
    -0x1.170653fb1eb04p1

  },
  { // Entry 862
    -0x1.b3df386f18263p-1,
    0x1.37d9e563f51f6p1

  },
  { // Entry 863
    0x1.b3df386f18263p-1,
    -0x1.37d9e563f51f6p1

  },
  { // Entry 864
    -0x1.ed21e59972b5cp-2,
    0x1.58ad76cccb8e8p1

  },
  { // Entry 865
    0x1.ed21e59972b5cp-2,
    -0x1.58ad76cccb8e8p1

  },
  { // Entry 866
    -0x1.8ed9142fc9217p-3,
    0x1.79810835a1fdap1

  },
  { // Entry 867
    0x1.8ed9142fc9217p-3,
    -0x1.79810835a1fdap1

  },
  { // Entry 868
    0x1.06f8d014bef68p-4,
    0x1.9a54999e786ccp1

  },
  { // Entry 869
    -0x1.06f8d014bef68p-4,
    -0x1.9a54999e786ccp1

  },
  { // Entry 870
    0x1.53fdcdfd37eb4p-2,
    0x1.bb282b074edbep1

  },
  { // Entry 871
    -0x1.53fdcdfd37eb4p-2,
    -0x1.bb282b074edbep1

  },
  { // Entry 872
    0x1.4d466accad45ap-1,
    0x1.dbfbbc70254bp1

  },
  { // Entry 873
    -0x1.4d466accad45ap-1,
    -0x1.dbfbbc70254bp1

  },
  { // Entry 874
    0x1.19e26b5ecd598p0,
    0x1.fccf4dd8fbba2p1

  },
  { // Entry 875
    -0x1.19e26b5ecd598p0,
    -0x1.fccf4dd8fbba2p1

  },
  { // Entry 876
    0x1.eab43e0e5e825p0,
    0x1.0ed16fa0e914ap2

  },
  { // Entry 877
    -0x1.eab43e0e5e825p0,
    -0x1.0ed16fa0e914ap2

  },
  { // Entry 878
    0x1.1866fe845e6fcp2,
    0x1.1f3b3855544c3p2

  },
  { // Entry 879
    -0x1.1866fe845e6fcp2,
    -0x1.1f3b3855544c3p2

  },
  { // Entry 880
    -0x1.f2f05cfb65c5ep4,
    0x1.2fa50109bf83cp2

  },
  { // Entry 881
    0x1.f2f05cfb65c5ep4,
    -0x1.2fa50109bf83cp2

  },
  { // Entry 882
    -0x1.af4643e93728dp1,
    0x1.400ec9be2abb5p2

  },
  { // Entry 883
    0x1.af4643e93728dp1,
    -0x1.400ec9be2abb5p2

  },
  { // Entry 884
    -0x1.a64c7c9d653ap0,
    0x1.5078927295f2ep2

  },
  { // Entry 885
    0x1.a64c7c9d653ap0,
    -0x1.5078927295f2ep2

  },
  { // Entry 886
    -0x1.efd82742d77ebp-1,
    0x1.60e25b27012a7p2

  },
  { // Entry 887
    0x1.efd82742d77ebp-1,
    -0x1.60e25b27012a7p2

  },
  { // Entry 888
    -0x1.205a6009d5e7fp-1,
    0x1.714c23db6c62p2

  },
  { // Entry 889
    0x1.205a6009d5e7fp-1,
    -0x1.714c23db6c62p2

  },
  { // Entry 890
    -0x1.0c864083d1ee6p-2,
    0x1.81b5ec8fd7999p2

  },
  { // Entry 891
    0x1.0c864083d1ee6p-2,
    -0x1.81b5ec8fd7999p2

  },
  { // Entry 892
    0x1.f09b63aa81fc3p-5,
    0x1.effffffffffffp-5

  },
  { // Entry 893
    -0x1.f09b63aa81fc3p-5,
    -0x1.effffffffffffp-5

  },
  { // Entry 894
    0x1.f09b63aa81fc4p-5,
    0x1.fp-5

  },
  { // Entry 895
    -0x1.f09b63aa81fc4p-5,
    -0x1.fp-5

  },
  { // Entry 896
    0x1.f09b63aa81fc5p-5,
    0x1.f000000000001p-5

  },
  { // Entry 897
    -0x1.f09b63aa81fc5p-5,
    -0x1.f000000000001p-5

  },
  { // Entry 898
    0x1.fa8f21c8a33afp-4,
    0x1.f7fffffffffffp-4

  },
  { // Entry 899
    -0x1.fa8f21c8a33afp-4,
    -0x1.f7fffffffffffp-4

  },
  { // Entry 900
    0x1.fa8f21c8a33bp-4,
    0x1.f8p-4

  },
  { // Entry 901
    -0x1.fa8f21c8a33bp-4,
    -0x1.f8p-4

  },
  { // Entry 902
    0x1.fa8f21c8a33b1p-4,
    0x1.f800000000001p-4

  },
  { // Entry 903
    -0x1.fa8f21c8a33b1p-4,
    -0x1.f800000000001p-4

  },
  { // Entry 904
    0x1.4ef06cb4f0a87p-3,
    0x1.4bfffffffffffp-3

  },
  { // Entry 905
    -0x1.4ef06cb4f0a87p-3,
    -0x1.4bfffffffffffp-3

  },
  { // Entry 906
    0x1.4ef06cb4f0a88p-3,
    0x1.4cp-3

  },
  { // Entry 907
    -0x1.4ef06cb4f0a88p-3,
    -0x1.4cp-3

  },
  { // Entry 908
    0x1.4ef06cb4f0a89p-3,
    0x1.4c00000000001p-3

  },
  { // Entry 909
    -0x1.4ef06cb4f0a89p-3,
    -0x1.4c00000000001p-3

  },
  { // Entry 910
    0x1.3cc2a44e29996p-2,
    0x1.3333333333332p-2

  },
  { // Entry 911
    -0x1.3cc2a44e29996p-2,
    -0x1.3333333333332p-2

  },
  { // Entry 912
    0x1.3cc2a44e29998p-2,
    0x1.3333333333333p-2

  },
  { // Entry 913
    -0x1.3cc2a44e29998p-2,
    -0x1.3333333333333p-2

  },
  { // Entry 914
    0x1.3cc2a44e29999p-2,
    0x1.3333333333334p-2

  },
  { // Entry 915
    -0x1.3cc2a44e29999p-2,
    -0x1.3333333333334p-2

  },
  { // Entry 916
    0x1.9943711dc2ce8p-1,
    0x1.594317acc4ef8p-1

  },
  { // Entry 917
    -0x1.9943711dc2ce8p-1,
    -0x1.594317acc4ef8p-1

  },
  { // Entry 918
    0x1.9943711dc2ceap-1,
    0x1.594317acc4ef9p-1

  },
  { // Entry 919
    -0x1.9943711dc2ceap-1,
    -0x1.594317acc4ef9p-1

  },
  { // Entry 920
    0x1.9943711dc2cecp-1,
    0x1.594317acc4efap-1

  },
  { // Entry 921
    -0x1.9943711dc2cecp-1,
    -0x1.594317acc4efap-1

  },
  { // Entry 922
    0x1.fbc511df5917dp-1,
    0x1.8ffffffffffffp-1

  },
  { // Entry 923
    -0x1.fbc511df5917dp-1,
    -0x1.8ffffffffffffp-1

  },
  { // Entry 924
    0x1.fbc511df5917fp-1,
    0x1.9p-1

  },
  { // Entry 925
    -0x1.fbc511df5917fp-1,
    -0x1.9p-1

  },
  { // Entry 926
    0x1.fbc511df59181p-1,
    0x1.9000000000001p-1

  },
  { // Entry 927
    -0x1.fbc511df59181p-1,
    -0x1.9000000000001p-1

  },
  { // Entry 928
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 929
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 930
    -0x0.0p0,
    -0x0.0p0

  },
  { // Entry 931
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 932
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 933
    0x1.927278a3b1161p-5,
    0x1.921fb54442d17p-5

  },
  { // Entry 934
    -0x1.927278a3b1161p-5,
    -0x1.921fb54442d17p-5

  },
  { // Entry 935
    0x1.927278a3b1162p-5,
    0x1.921fb54442d18p-5

  },
  { // Entry 936
    -0x1.927278a3b1162p-5,
    -0x1.921fb54442d18p-5

  },
  { // Entry 937
    0x1.927278a3b1163p-5,
    0x1.921fb54442d19p-5

  },
  { // Entry 938
    -0x1.927278a3b1163p-5,
    -0x1.921fb54442d19p-5

  },
  { // Entry 939
    0x1.936bb8c5b2da1p-4,
    0x1.921fb54442d17p-4

  },
  { // Entry 940
    -0x1.936bb8c5b2da1p-4,
    -0x1.921fb54442d17p-4

  },
  { // Entry 941
    0x1.936bb8c5b2da2p-4,
    0x1.921fb54442d18p-4

  },
  { // Entry 942
    -0x1.936bb8c5b2da2p-4,
    -0x1.921fb54442d18p-4

  },
  { // Entry 943
    0x1.936bb8c5b2da3p-4,
    0x1.921fb54442d19p-4

  },
  { // Entry 944
    -0x1.936bb8c5b2da3p-4,
    -0x1.921fb54442d19p-4

  },
  { // Entry 945
    0x1.975f5e0553157p-3,
    0x1.921fb54442d17p-3

  },
  { // Entry 946
    -0x1.975f5e0553157p-3,
    -0x1.921fb54442d17p-3

  },
  { // Entry 947
    0x1.975f5e0553158p-3,
    0x1.921fb54442d18p-3

  },
  { // Entry 948
    -0x1.975f5e0553158p-3,
    -0x1.921fb54442d18p-3

  },
  { // Entry 949
    0x1.975f5e0553159p-3,
    0x1.921fb54442d19p-3

  },
  { // Entry 950
    -0x1.975f5e0553159p-3,
    -0x1.921fb54442d19p-3

  },
  { // Entry 951
    0x1.a827999fcef31p-2,
    0x1.921fb54442d17p-2

  },
  { // Entry 952
    -0x1.a827999fcef31p-2,
    -0x1.921fb54442d17p-2

  },
  { // Entry 953
    0x1.a827999fcef32p-2,
    0x1.921fb54442d18p-2

  },
  { // Entry 954
    -0x1.a827999fcef32p-2,
    -0x1.921fb54442d18p-2

  },
  { // Entry 955
    0x1.a827999fcef33p-2,
    0x1.921fb54442d19p-2

  },
  { // Entry 956
    -0x1.a827999fcef33p-2,
    -0x1.921fb54442d19p-2

  },
  { // Entry 957
    0x1.ffffffffffffdp-1,
    0x1.921fb54442d17p-1

  },
  { // Entry 958
    -0x1.ffffffffffffdp-1,
    -0x1.921fb54442d17p-1

  },
  { // Entry 959
    0x1.fffffffffffffp-1,
    0x1.921fb54442d18p-1

  },
  { // Entry 960
    -0x1.fffffffffffffp-1,
    -0x1.921fb54442d18p-1

  },
  { // Entry 961
    0x1.0000000000001p0,
    0x1.921fb54442d19p-1

  },
  { // Entry 962
    -0x1.0000000000001p0,
    -0x1.921fb54442d19p-1

  },
  { // Entry 963
    0x1.9153d9443ed0bp51,
    0x1.921fb54442d17p0

  },
  { // Entry 964
    -0x1.9153d9443ed0bp51,
    -0x1.921fb54442d17p0

  },
  { // Entry 965
    0x1.d02967c31cdb5p53,
    0x1.921fb54442d18p0

  },
  { // Entry 966
    -0x1.d02967c31cdb5p53,
    -0x1.921fb54442d18p0

  },
  { // Entry 967
    -0x1.617a15494767ap52,
    0x1.921fb54442d19p0

  },
  { // Entry 968
    0x1.617a15494767ap52,
    -0x1.921fb54442d19p0

  },
  { // Entry 969
    -0x1.469898cc51702p-51,
    0x1.921fb54442d17p1

  },
  { // Entry 970
    0x1.469898cc51702p-51,
    -0x1.921fb54442d17p1

  },
  { // Entry 971
    -0x1.1a62633145c07p-53,
    0x1.921fb54442d18p1

  },
  { // Entry 972
    0x1.1a62633145c07p-53,
    -0x1.921fb54442d18p1

  },
  { // Entry 973
    0x1.72cece675d1fdp-52,
    0x1.921fb54442d19p1

  },
  { // Entry 974
    -0x1.72cece675d1fdp-52,
    -0x1.921fb54442d19p1

  },
  { // Entry 975
    -0x1.469898cc51702p-50,
    0x1.921fb54442d17p2

  },
  { // Entry 976
    0x1.469898cc51702p-50,
    -0x1.921fb54442d17p2

  },
  { // Entry 977
    -0x1.1a62633145c07p-52,
    0x1.921fb54442d18p2

  },
  { // Entry 978
    0x1.1a62633145c07p-52,
    -0x1.921fb54442d18p2

  },
  { // Entry 979
    0x1.72cece675d1fdp-51,
    0x1.921fb54442d19p2

  },
  { // Entry 980
    -0x1.72cece675d1fdp-51,
    -0x1.921fb54442d19p2

  },
  { // Entry 981
    -0x1.469898cc51702p-49,
    0x1.921fb54442d17p3

  },
  { // Entry 982
    0x1.469898cc51702p-49,
    -0x1.921fb54442d17p3

  },
  { // Entry 983
    -0x1.1a62633145c07p-51,
    0x1.921fb54442d18p3

  },
  { // Entry 984
    0x1.1a62633145c07p-51,
    -0x1.921fb54442d18p3

  },
  { // Entry 985
    0x1.72cece675d1fdp-50,
    0x1.921fb54442d19p3

  },
  { // Entry 986
    -0x1.72cece675d1fdp-50,
    -0x1.921fb54442d19p3

  },
  { // Entry 987
    -0x1.469898cc51702p-48,
    0x1.921fb54442d17p4

  },
  { // Entry 988
    0x1.469898cc51702p-48,
    -0x1.921fb54442d17p4

  },
  { // Entry 989
    -0x1.1a62633145c07p-50,
    0x1.921fb54442d18p4

  },
  { // Entry 990
    0x1.1a62633145c07p-50,
    -0x1.921fb54442d18p4

  },
  { // Entry 991
    0x1.72cece675d1fdp-49,
    0x1.921fb54442d19p4

  },
  { // Entry 992
    -0x1.72cece675d1fdp-49,
    -0x1.921fb54442d19p4

  },
  { // Entry 993
    -0x1.469898cc51702p-47,
    0x1.921fb54442d17p5

  },
  { // Entry 994
    0x1.469898cc51702p-47,
    -0x1.921fb54442d17p5

  },
  { // Entry 995
    -0x1.1a62633145c07p-49,
    0x1.921fb54442d18p5

  },
  { // Entry 996
    0x1.1a62633145c07p-49,
    -0x1.921fb54442d18p5

  },
  { // Entry 997
    0x1.72cece675d1fdp-48,
    0x1.921fb54442d19p5

  },
  { // Entry 998
    -0x1.72cece675d1fdp-48,
    -0x1.921fb54442d19p5

  },
  { // Entry 999
    -0x1.469898cc51702p-46,
    0x1.921fb54442d17p6

  },
  { // Entry 1000
    0x1.469898cc51702p-46,
    -0x1.921fb54442d17p6

  },
  { // Entry 1001
    -0x1.1a62633145c07p-48,
    0x1.921fb54442d18p6

  },
  { // Entry 1002
    0x1.1a62633145c07p-48,
    -0x1.921fb54442d18p6

  },
  { // Entry 1003
    0x1.72cece675d1fdp-47,
    0x1.921fb54442d19p6

  },
  { // Entry 1004
    -0x1.72cece675d1fdp-47,
    -0x1.921fb54442d19p6

  },
  { // Entry 1005
    -0x1.469898cc51702p-45,
    0x1.921fb54442d17p7

  },
  { // Entry 1006
    0x1.469898cc51702p-45,
    -0x1.921fb54442d17p7

  },
  { // Entry 1007
    -0x1.1a62633145c07p-47,
    0x1.921fb54442d18p7

  },
  { // Entry 1008
    0x1.1a62633145c07p-47,
    -0x1.921fb54442d18p7

  },
  { // Entry 1009
    0x1.72cece675d1fdp-46,
    0x1.921fb54442d19p7

  },
  { // Entry 1010
    -0x1.72cece675d1fdp-46,
    -0x1.921fb54442d19p7

  },
  { // Entry 1011
    -0x1.0000000000005p0,
    0x1.2d97c7f3321d1p1

  },
  { // Entry 1012
    0x1.0000000000005p0,
    -0x1.2d97c7f3321d1p1

  },
  { // Entry 1013
    -0x1.0000000000001p0,
    0x1.2d97c7f3321d2p1

  },
  { // Entry 1014
    0x1.0000000000001p0,
    -0x1.2d97c7f3321d2p1

  },
  { // Entry 1015
    -0x1.ffffffffffffap-1,
    0x1.2d97c7f3321d3p1

  },
  { // Entry 1016
    0x1.ffffffffffffap-1,
    -0x1.2d97c7f3321d3p1

  },
  { // Entry 1017
    0x1.ffffffffffff5p-1,
    0x1.f6a7a2955385dp1

  },
  { // Entry 1018
    -0x1.ffffffffffff5p-1,
    -0x1.f6a7a2955385dp1

  },
  { // Entry 1019
    0x1.ffffffffffffdp-1,
    0x1.f6a7a2955385ep1

  },
  { // Entry 1020
    -0x1.ffffffffffffdp-1,
    -0x1.f6a7a2955385ep1

  },
  { // Entry 1021
    0x1.0000000000003p0,
    0x1.f6a7a2955385fp1

  },
  { // Entry 1022
    -0x1.0000000000003p0,
    -0x1.f6a7a2955385fp1

  },
  { // Entry 1023
    0x1.a8410087262e4p49,
    0x1.2d97c7f3321d1p2

  },
  { // Entry 1024
    -0x1.a8410087262e4p49,
    -0x1.2d97c7f3321d1p2

  },
  { // Entry 1025
    0x1.3570efd768923p52,
    0x1.2d97c7f3321d2p2

  },
  { // Entry 1026
    -0x1.3570efd768923p52,
    -0x1.2d97c7f3321d2p2

  },
  { // Entry 1027
    -0x1.42c0d64d5de51p50,
    0x1.2d97c7f3321d3p2

  },
  { // Entry 1028
    0x1.42c0d64d5de51p50,
    -0x1.2d97c7f3321d3p2

  },
  { // Entry 1029
    -0x1.000000000000ap0,
    0x1.5fdbbe9bba774p2

  },
  { // Entry 1030
    0x1.000000000000ap0,
    -0x1.5fdbbe9bba774p2

  },
  { // Entry 1031
    -0x1.0000000000002p0,
    0x1.5fdbbe9bba775p2

  },
  { // Entry 1032
    0x1.0000000000002p0,
    -0x1.5fdbbe9bba775p2

  },
  { // Entry 1033
    -0x1.ffffffffffff4p-1,
    0x1.5fdbbe9bba776p2

  },
  { // Entry 1034
    0x1.ffffffffffff4p-1,
    -0x1.5fdbbe9bba776p2

  },
  { // Entry 1035
    0x1.fffffffffffebp-1,
    0x1.c463abeccb2bap2

  },
  { // Entry 1036
    -0x1.fffffffffffebp-1,
    -0x1.c463abeccb2bap2

  },
  { // Entry 1037
    0x1.ffffffffffffbp-1,
    0x1.c463abeccb2bbp2

  },
  { // Entry 1038
    -0x1.ffffffffffffbp-1,
    -0x1.c463abeccb2bbp2

  },
  { // Entry 1039
    0x1.0000000000006p0,
    0x1.c463abeccb2bcp2

  },
  { // Entry 1040
    -0x1.0000000000006p0,
    -0x1.c463abeccb2bcp2

  },
  { // Entry 1041
    0x1.7cc080f895856p49,
    0x1.f6a7a2955385dp2

  },
  { // Entry 1042
    -0x1.7cc080f895856p49,
    -0x1.f6a7a2955385dp2

  },
  { // Entry 1043
    0x1.735453027d7c4p51,
    0x1.f6a7a2955385ep2

  },
  { // Entry 1044
    -0x1.735453027d7c4p51,
    -0x1.f6a7a2955385ep2

  },
  { // Entry 1045
    -0x1.86aa4a3a127a4p50,
    0x1.f6a7a2955385fp2

  },
  { // Entry 1046
    0x1.86aa4a3a127a4p50,
    -0x1.f6a7a2955385fp2

  },
  { // Entry 1047
    -0x1.000000000001bp0,
    0x1.1475cc9eedeffp3

  },
  { // Entry 1048
    0x1.000000000001bp0,
    -0x1.1475cc9eedeffp3

  },
  { // Entry 1049
    -0x1.000000000000bp0,
    0x1.1475cc9eedfp3

  },
  { // Entry 1050
    0x1.000000000000bp0,
    -0x1.1475cc9eedfp3

  },
  { // Entry 1051
    -0x1.ffffffffffff6p-1,
    0x1.1475cc9eedf01p3

  },
  { // Entry 1052
    0x1.ffffffffffff6p-1,
    -0x1.1475cc9eedf01p3

  },
  { // Entry 1053
    -0x1.34f272993d141p-49,
    0x1.2d97c7f3321d1p3

  },
  { // Entry 1054
    0x1.34f272993d141p-49,
    -0x1.2d97c7f3321d1p3

  },
  { // Entry 1055
    -0x1.a79394c9e8a0ap-52,
    0x1.2d97c7f3321d2p3

  },
  { // Entry 1056
    0x1.a79394c9e8a0ap-52,
    -0x1.2d97c7f3321d2p3

  },
  { // Entry 1057
    0x1.961b1acd85d7dp-50,
    0x1.2d97c7f3321d3p3

  },
  { // Entry 1058
    -0x1.961b1acd85d7dp-50,
    -0x1.2d97c7f3321d3p3

  },
  { // Entry 1059
    0x1.fffffffffffc9p-1,
    0x1.46b9c347764a2p3

  },
  { // Entry 1060
    -0x1.fffffffffffc9p-1,
    -0x1.46b9c347764a2p3

  },
  { // Entry 1061
    0x1.fffffffffffe9p-1,
    0x1.46b9c347764a3p3

  },
  { // Entry 1062
    -0x1.fffffffffffe9p-1,
    -0x1.46b9c347764a3p3

  },
  { // Entry 1063
    0x1.0000000000004p0,
    0x1.46b9c347764a4p3

  },
  { // Entry 1064
    -0x1.0000000000004p0,
    -0x1.46b9c347764a4p3

  },
  { // Entry 1065
    0x1.9c78eea78baa7p48,
    0x1.5fdbbe9bba774p3

  },
  { // Entry 1066
    -0x1.9c78eea78baa7p48,
    -0x1.5fdbbe9bba774p3

  },
  { // Entry 1067
    0x1.093c3b4aebeb1p51,
    0x1.5fdbbe9bba775p3

  },
  { // Entry 1068
    -0x1.093c3b4aebeb1p51,
    -0x1.5fdbbe9bba775p3

  },
  { // Entry 1069
    -0x1.516ac96142185p49,
    0x1.5fdbbe9bba776p3

  },
  { // Entry 1070
    0x1.516ac96142185p49,
    -0x1.5fdbbe9bba776p3

  },
  { // Entry 1071
    -0x1.000000000001cp0,
    0x1.78fdb9effea45p3

  },
  { // Entry 1072
    0x1.000000000001cp0,
    -0x1.78fdb9effea45p3

  },
  { // Entry 1073
    -0x1.000000000000cp0,
    0x1.78fdb9effea46p3

  },
  { // Entry 1074
    0x1.000000000000cp0,
    -0x1.78fdb9effea46p3

  },
  { // Entry 1075
    -0x1.ffffffffffff8p-1,
    0x1.78fdb9effea47p3

  },
  { // Entry 1076
    0x1.ffffffffffff8p-1,
    -0x1.78fdb9effea47p3

  },
  { // Entry 1077
    0x1.fffffffffffc7p-1,
    0x1.ab41b09886fe8p3

  },
  { // Entry 1078
    -0x1.fffffffffffc7p-1,
    -0x1.ab41b09886fe8p3

  },
  { // Entry 1079
    0x1.fffffffffffe7p-1,
    0x1.ab41b09886fe9p3

  },
  { // Entry 1080
    -0x1.fffffffffffe7p-1,
    -0x1.ab41b09886fe9p3

  },
  { // Entry 1081
    0x1.0000000000003p0,
    0x1.ab41b09886feap3

  },
  { // Entry 1082
    -0x1.0000000000003p0,
    -0x1.ab41b09886feap3

  },
  { // Entry 1083
    0x1.86c4e333b3c9fp48,
    0x1.c463abeccb2bap3

  },
  { // Entry 1084
    -0x1.86c4e333b3c9fp48,
    -0x1.c463abeccb2bap3

  },
  { // Entry 1085
    0x1.9c96951f36184p50,
    0x1.c463abeccb2bbp3

  },
  { // Entry 1086
    -0x1.9c96951f36184p50,
    -0x1.c463abeccb2bbp3

  },
  { // Entry 1087
    -0x1.73244d369e0b7p49,
    0x1.c463abeccb2bcp3

  },
  { // Entry 1088
    0x1.73244d369e0b7p49,
    -0x1.c463abeccb2bcp3

  },
  { // Entry 1089
    -0x1.000000000001dp0,
    0x1.dd85a7410f58bp3

  },
  { // Entry 1090
    0x1.000000000001dp0,
    -0x1.dd85a7410f58bp3

  },
  { // Entry 1091
    -0x1.000000000000dp0,
    0x1.dd85a7410f58cp3

  },
  { // Entry 1092
    0x1.000000000000dp0,
    -0x1.dd85a7410f58cp3

  },
  { // Entry 1093
    -0x1.ffffffffffffap-1,
    0x1.dd85a7410f58dp3

  },
  { // Entry 1094
    0x1.ffffffffffffap-1,
    -0x1.dd85a7410f58dp3

  },
  { // Entry 1095
    -0x1.583ebeff65cc2p-49,
    0x1.f6a7a2955385dp3

  },
  { // Entry 1096
    0x1.583ebeff65cc2p-49,
    -0x1.f6a7a2955385dp3

  },
  { // Entry 1097
    -0x1.60fafbfd97309p-51,
    0x1.f6a7a2955385ep3

  },
  { // Entry 1098
    0x1.60fafbfd97309p-51,
    -0x1.f6a7a2955385ep3

  },
  { // Entry 1099
    0x1.4f8282013467cp-50,
    0x1.f6a7a2955385fp3

  },
  { // Entry 1100
    -0x1.4f8282013467cp-50,
    -0x1.f6a7a2955385fp3

  },
  { // Entry 1101
    0x1.fffffffffff84p-1,
    0x1.07e4cef4cbd96p4

  },
  { // Entry 1102
    -0x1.fffffffffff84p-1,
    -0x1.07e4cef4cbd96p4

  },
  { // Entry 1103
    0x1.fffffffffffc4p-1,
    0x1.07e4cef4cbd97p4

  },
  { // Entry 1104
    -0x1.fffffffffffc4p-1,
    -0x1.07e4cef4cbd97p4

  },
  { // Entry 1105
    0x1.0000000000002p0,
    0x1.07e4cef4cbd98p4

  },
  { // Entry 1106
    -0x1.0000000000002p0,
    -0x1.07e4cef4cbd98p4

  },
  { // Entry 1107
    0x1.2f0842389e5bcp47,
    0x1.1475cc9eedeffp4

  },
  { // Entry 1108
    -0x1.2f0842389e5bcp47,
    -0x1.1475cc9eedeffp4

  },
  { // Entry 1109
    0x1.733c4e8ef9c51p48,
    0x1.1475cc9eedfp4

  },
  { // Entry 1110
    -0x1.733c4e8ef9c51p48,
    -0x1.1475cc9eedfp4

  },
  { // Entry 1111
    -0x1.9c5b4c7289899p49,
    0x1.1475cc9eedf01p4

  },
  { // Entry 1112
    0x1.9c5b4c7289899p49,
    -0x1.1475cc9eedf01p4

  },
  { // Entry 1113
    -0x1.000000000002ep0,
    0x1.2106ca4910068p4

  },
  { // Entry 1114
    0x1.000000000002ep0,
    -0x1.2106ca4910068p4

  },
  { // Entry 1115
    -0x1.000000000000ep0,
    0x1.2106ca4910069p4

  },
  { // Entry 1116
    0x1.000000000000ep0,
    -0x1.2106ca4910069p4

  },
  { // Entry 1117
    -0x1.fffffffffffddp-1,
    0x1.2106ca491006ap4

  },
  { // Entry 1118
    0x1.fffffffffffddp-1,
    -0x1.2106ca491006ap4

  },
  { // Entry 1119
    -0x1.34f272993d141p-48,
    0x1.2d97c7f3321d1p4

  },
  { // Entry 1120
    0x1.34f272993d141p-48,
    -0x1.2d97c7f3321d1p4

  },
  { // Entry 1121
    -0x1.a79394c9e8a0ap-51,
    0x1.2d97c7f3321d2p4

  },
  { // Entry 1122
    0x1.a79394c9e8a0ap-51,
    -0x1.2d97c7f3321d2p4

  },
  { // Entry 1123
    0x1.961b1acd85d7dp-49,
    0x1.2d97c7f3321d3p4

  },
  { // Entry 1124
    -0x1.961b1acd85d7dp-49,
    -0x1.2d97c7f3321d3p4

  },
  { // Entry 1125
    0x1.fffffffffff82p-1,
    0x1.3a28c59d54339p4

  },
  { // Entry 1126
    -0x1.fffffffffff82p-1,
    -0x1.3a28c59d54339p4

  },
  { // Entry 1127
    0x1.fffffffffffc2p-1,
    0x1.3a28c59d5433ap4

  },
  { // Entry 1128
    -0x1.fffffffffffc2p-1,
    -0x1.3a28c59d5433ap4

  },
  { // Entry 1129
    0x1.0000000000001p0,
    0x1.3a28c59d5433bp4

  },
  { // Entry 1130
    -0x1.0000000000001p0,
    -0x1.3a28c59d5433bp4

  },
  { // Entry 1131
    0x1.28f934315e5ecp47,
    0x1.46b9c347764a2p4

  },
  { // Entry 1132
    -0x1.28f934315e5ecp47,
    -0x1.46b9c347764a2p4

  },
  { // Entry 1133
    0x1.618fdb7f21c14p48,
    0x1.46b9c347764a3p4

  },
  { // Entry 1134
    -0x1.618fdb7f21c14p48,
    -0x1.46b9c347764a3p4

  },
  { // Entry 1135
    -0x1.cfde61218ab9ep49,
    0x1.46b9c347764a4p4

  },
  { // Entry 1136
    0x1.cfde61218ab9ep49,
    -0x1.46b9c347764a4p4

  },
  { // Entry 1137
    -0x1.000000000002fp0,
    0x1.534ac0f19860bp4

  },
  { // Entry 1138
    0x1.000000000002fp0,
    -0x1.534ac0f19860bp4

  },
  { // Entry 1139
    -0x1.000000000000fp0,
    0x1.534ac0f19860cp4

  },
  { // Entry 1140
    0x1.000000000000fp0,
    -0x1.534ac0f19860cp4

  },
  { // Entry 1141
    -0x1.fffffffffffdfp-1,
    0x1.534ac0f19860dp4

  },
  { // Entry 1142
    0x1.fffffffffffdfp-1,
    -0x1.534ac0f19860dp4

  },
  { // Entry 1143
    -0x1.3dc585b2c7422p-48,
    0x1.5fdbbe9bba774p4

  },
  { // Entry 1144
    0x1.3dc585b2c7422p-48,
    -0x1.5fdbbe9bba774p4

  },
  { // Entry 1145
    -0x1.ee2c2d963a10cp-51,
    0x1.5fdbbe9bba775p4

  },
  { // Entry 1146
    0x1.ee2c2d963a10cp-51,
    -0x1.5fdbbe9bba775p4

  },
  { // Entry 1147
    0x1.8474f49a717bdp-49,
    0x1.5fdbbe9bba776p4

  },
  { // Entry 1148
    -0x1.8474f49a717bdp-49,
    -0x1.5fdbbe9bba776p4

  },
  { // Entry 1149
    0x1.fffffffffff8p-1,
    0x1.6c6cbc45dc8dcp4

  },
  { // Entry 1150
    -0x1.fffffffffff8p-1,
    -0x1.6c6cbc45dc8dcp4

  },
  { // Entry 1151
    0x1.fffffffffffcp-1,
    0x1.6c6cbc45dc8ddp4

  },
  { // Entry 1152
    -0x1.fffffffffffcp-1,
    -0x1.6c6cbc45dc8ddp4

  },
  { // Entry 1153
    0x1.0p0,
    0x1.6c6cbc45dc8dep4

  },
  { // Entry 1154
    -0x1.0p0,
    -0x1.6c6cbc45dc8dep4

  },
  { // Entry 1155
    0x1.2326f4e8f2bb2p47,
    0x1.78fdb9effea45p4

  },
  { // Entry 1156
    -0x1.2326f4e8f2bb2p47,
    -0x1.78fdb9effea45p4

  },
  { // Entry 1157
    0x1.517ea08708ba7p48,
    0x1.78fdb9effea46p4

  },
  { // Entry 1158
    -0x1.517ea08708ba7p48,
    -0x1.78fdb9effea46p4

  },
  { // Entry 1159
    -0x1.090b3d5161786p50,
    0x1.78fdb9effea47p4

  },
  { // Entry 1160
    0x1.090b3d5161786p50,
    -0x1.78fdb9effea47p4

  },
  { // Entry 1161
    -0x1.0000000000031p0,
    0x1.858eb79a20baep4

  },
  { // Entry 1162
    0x1.0000000000031p0,
    -0x1.858eb79a20baep4

  },
  { // Entry 1163
    -0x1.0000000000011p0,
    0x1.858eb79a20bafp4

  },
  { // Entry 1164
    0x1.0000000000011p0,
    -0x1.858eb79a20bafp4

  },
  { // Entry 1165
    -0x1.fffffffffffe1p-1,
    0x1.858eb79a20bbp4

  },
  { // Entry 1166
    0x1.fffffffffffe1p-1,
    -0x1.858eb79a20bbp4

  },
  { // Entry 1167
    -0x1.af0792001f856p2,
    0x1.fffffffffffffp62

  },
  { // Entry 1168
    0x1.af0792001f856p2,
    -0x1.fffffffffffffp62

  },
  { // Entry 1169
    0x1.52f50e757941dp6,
    0x1.0p63

  },
  { // Entry 1170
    -0x1.52f50e757941dp6,
    -0x1.0p63

  },
  { // Entry 1171
    0x1.7570667d032edp1,
    0x1.0000000000001p63

  },
  { // Entry 1172
    -0x1.7570667d032edp1,
    -0x1.0000000000001p63

  },
  { // Entry 1173
    -0x1.2e8fc248e7b85p0,
    0x1.fffffffffffffp26

  },
  { // Entry 1174
    0x1.2e8fc248e7b85p0,
    -0x1.fffffffffffffp26

  },
  { // Entry 1175
    -0x1.2e8fc1af81d8cp0,
    0x1.0p27

  },
  { // Entry 1176
    0x1.2e8fc1af81d8cp0,
    -0x1.0p27

  },
  { // Entry 1177
    -0x1.2e8fc07cb61a9p0,
    0x1.0000000000001p27

  },
  { // Entry 1178
    0x1.2e8fc07cb61a9p0,
    -0x1.0000000000001p27

  },
  { // Entry 1179
    -0x1.3ea282860e7fcp0,
    0x1.fffffffffffffp23

  },
  { // Entry 1180
    0x1.3ea282860e7fcp0,
    -0x1.fffffffffffffp23

  },
  { // Entry 1181
    -0x1.3ea28271a9beap0,
    0x1.0p24

  },
  { // Entry 1182
    0x1.3ea28271a9beap0,
    -0x1.0p24

  },
  { // Entry 1183
    -0x1.3ea28248e03c7p0,
    0x1.0000000000001p24

  },
  { // Entry 1184
    0x1.3ea28248e03c7p0,
    -0x1.0000000000001p24

  },
  { // Entry 1185
    0x1.2866f9be4de0fp0,
    0x1.fffffffffffffp1

  },
  { // Entry 1186
    -0x1.2866f9be4de0fp0,
    -0x1.fffffffffffffp1

  },
  { // Entry 1187
    0x1.2866f9be4de14p0,
    0x1.0p2

  },
  { // Entry 1188
    -0x1.2866f9be4de14p0,
    -0x1.0p2

  },
  { // Entry 1189
    0x1.2866f9be4de1dp0,
    0x1.0000000000001p2

  },
  { // Entry 1190
    -0x1.2866f9be4de1dp0,
    -0x1.0000000000001p2

  },
  { // Entry 1191
    -0x1.17af62e0950fbp1,
    0x1.fffffffffffffp0

  },
  { // Entry 1192
    0x1.17af62e0950fbp1,
    -0x1.fffffffffffffp0

  },
  { // Entry 1193
    -0x1.17af62e0950f8p1,
    0x1.0p1

  },
  { // Entry 1194
    0x1.17af62e0950f8p1,
    -0x1.0p1

  },
  { // Entry 1195
    -0x1.17af62e0950f2p1,
    0x1.0000000000001p1

  },
  { // Entry 1196
    0x1.17af62e0950f2p1,
    -0x1.0000000000001p1

  },
  { // Entry 1197
    0x1.8eb245cbee3a4p0,
    0x1.fffffffffffffp-1

  },
  { // Entry 1198
    -0x1.8eb245cbee3a4p0,
    -0x1.fffffffffffffp-1

  },
  { // Entry 1199
    0x1.8eb245cbee3a6p0,
    0x1.0p0

  },
  { // Entry 1200
    -0x1.8eb245cbee3a6p0,
    -0x1.0p0

  },
  { // Entry 1201
    0x1.8eb245cbee3a9p0,
    0x1.0000000000001p0

  },
  { // Entry 1202
    -0x1.8eb245cbee3a9p0,
    -0x1.0000000000001p0

  },
  { // Entry 1203
    0x1.17b4f5bf3474ap-1,
    0x1.fffffffffffffp-2

  },
  { // Entry 1204
    -0x1.17b4f5bf3474ap-1,
    -0x1.fffffffffffffp-2

  },
  { // Entry 1205
    0x1.17b4f5bf3474ap-1,
    0x1.0p-1

  },
  { // Entry 1206
    -0x1.17b4f5bf3474ap-1,
    -0x1.0p-1

  },
  { // Entry 1207
    0x1.17b4f5bf3474cp-1,
    0x1.0000000000001p-1

  },
  { // Entry 1208
    -0x1.17b4f5bf3474cp-1,
    -0x1.0000000000001p-1

  },
  { // Entry 1209
    0x1.05785a43c4c55p-2,
    0x1.fffffffffffffp-3

  },
  { // Entry 1210
    -0x1.05785a43c4c55p-2,
    -0x1.fffffffffffffp-3

  },
  { // Entry 1211
    0x1.05785a43c4c56p-2,
    0x1.0p-2

  },
  { // Entry 1212
    -0x1.05785a43c4c56p-2,
    -0x1.0p-2

  },
  { // Entry 1213
    0x1.05785a43c4c57p-2,
    0x1.0000000000001p-2

  },
  { // Entry 1214
    -0x1.05785a43c4c57p-2,
    -0x1.0000000000001p-2

  },
  { // Entry 1215
    0x1.01577af1511a4p-3,
    0x1.fffffffffffffp-4

  },
  { // Entry 1216
    -0x1.01577af1511a4p-3,
    -0x1.fffffffffffffp-4

  },
  { // Entry 1217
    0x1.01577af1511a5p-3,
    0x1.0p-3

  },
  { // Entry 1218
    -0x1.01577af1511a5p-3,
    -0x1.0p-3

  },
  { // Entry 1219
    0x1.01577af1511a6p-3,
    0x1.0000000000001p-3

  },
  { // Entry 1220
    -0x1.01577af1511a6p-3,
    -0x1.0000000000001p-3

  },
  { // Entry 1221
    0x1.005577854dfp-4,
    0x1.fffffffffffffp-5

  },
  { // Entry 1222
    -0x1.005577854dfp-4,
    -0x1.fffffffffffffp-5

  },
  { // Entry 1223
    0x1.005577854df01p-4,
    0x1.0p-4

  },
  { // Entry 1224
    -0x1.005577854df01p-4,
    -0x1.0p-4

  },
  { // Entry 1225
    0x1.005577854df02p-4,
    0x1.0000000000001p-4

  },
  { // Entry 1226
    -0x1.005577854df02p-4,
    -0x1.0000000000001p-4

  },
  { // Entry 1227
    0x1.00155777aec08p-5,
    0x1.fffffffffffffp-6

  },
  { // Entry 1228
    -0x1.00155777aec08p-5,
    -0x1.fffffffffffffp-6

  },
  { // Entry 1229
    0x1.00155777aec08p-5,
    0x1.0p-5

  },
  { // Entry 1230
    -0x1.00155777aec08p-5,
    -0x1.0p-5

  },
  { // Entry 1231
    0x1.00155777aec09p-5,
    0x1.0000000000001p-5

  },
  { // Entry 1232
    -0x1.00155777aec09p-5,
    -0x1.0000000000001p-5

  },
  { // Entry 1233
    0x1.0005557778548p-6,
    0x1.fffffffffffffp-7

  },
  { // Entry 1234
    -0x1.0005557778548p-6,
    -0x1.fffffffffffffp-7

  },
  { // Entry 1235
    0x1.0005557778549p-6,
    0x1.0p-6

  },
  { // Entry 1236
    -0x1.0005557778549p-6,
    -0x1.0p-6

  },
  { // Entry 1237
    0x1.000555777854ap-6,
    0x1.0000000000001p-6

  },
  { // Entry 1238
    -0x1.000555777854ap-6,
    -0x1.0000000000001p-6

  },
  { // Entry 1239
    0x1.0000000555555p-14,
    0x1.fffffffffffffp-15

  },
  { // Entry 1240
    -0x1.0000000555555p-14,
    -0x1.fffffffffffffp-15

  },
  { // Entry 1241
    0x1.0000000555555p-14,
    0x1.0p-14

  },
  { // Entry 1242
    -0x1.0000000555555p-14,
    -0x1.0p-14

  },
  { // Entry 1243
    0x1.0000000555556p-14,
    0x1.0000000000001p-14

  },
  { // Entry 1244
    -0x1.0000000555556p-14,
    -0x1.0000000000001p-14

  },
  { // Entry 1245
    0x1.fffffffffffffp-28,
    0x1.fffffffffffffp-28

  },
  { // Entry 1246
    -0x1.fffffffffffffp-28,
    -0x1.fffffffffffffp-28

  },
  { // Entry 1247
    0x1.0p-27,
    0x1.0p-27

  },
  { // Entry 1248
    -0x1.0p-27,
    -0x1.0p-27

  },
  { // Entry 1249
    0x1.0000000000001p-27,
    0x1.0000000000001p-27

  },
  { // Entry 1250
    -0x1.0000000000001p-27,
    -0x1.0000000000001p-27

  },
  { // Entry 1251
    0x1.fffffffffffffp-31,
    0x1.fffffffffffffp-31

  },
  { // Entry 1252
    -0x1.fffffffffffffp-31,
    -0x1.fffffffffffffp-31

  },
  { // Entry 1253
    0x1.0p-30,
    0x1.0p-30

  },
  { // Entry 1254
    -0x1.0p-30,
    -0x1.0p-30

  },
  { // Entry 1255
    0x1.0000000000001p-30,
    0x1.0000000000001p-30

  },
  { // Entry 1256
    -0x1.0000000000001p-30,
    -0x1.0000000000001p-30

  },
  { // Entry 1257
    0x1.4530cfe729484p-8,
    -0x1.fffffffffffffp1023

  },
  { // Entry 1258
    -0x1.4530cfe729484p-8,
    0x1.fffffffffffffp1023

  },
  { // Entry 1259
    -0x1.4530cfe729484p-8,
    0x1.fffffffffffffp1023

  },
  { // Entry 1260
    0x1.4530cfe729484p-8,
    -0x1.fffffffffffffp1023

  },
  { // Entry 1261
    -0x1.4530cfe729484p-8,
    0x1.fffffffffffffp1023

  },
  { // Entry 1262
    0x1.4530cfe729484p-8,
    -0x1.fffffffffffffp1023

  },
  { // Entry 1263
    0x1.3c6e9970f78b8p1,
    0x1.ffffffffffffep1023

  },
  { // Entry 1264
    -0x1.3c6e9970f78b8p1,
    -0x1.ffffffffffffep1023

  },
  { // Entry 1265
    -0x1.1a62633145c07p-53,
    0x1.921fb54442d18p1

  },
  { // Entry 1266
    0x1.1a62633145c07p-53,
    -0x1.921fb54442d18p1

  },
  { // Entry 1267
    0x1.d02967c31cdb5p53,
    0x1.921fb54442d18p0

  },
  { // Entry 1268
    -0x1.d02967c31cdb5p53,
    -0x1.921fb54442d18p0

  },
  { // Entry 1269
    0x1.8eb245cbee3a9p0,
    0x1.0000000000001p0

  },
  { // Entry 1270
    -0x1.8eb245cbee3a9p0,
    -0x1.0000000000001p0

  },
  { // Entry 1271
    0x1.8eb245cbee3a6p0,
    0x1.0p0

  },
  { // Entry 1272
    -0x1.8eb245cbee3a6p0,
    -0x1.0p0

  },
  { // Entry 1273
    0x1.8eb245cbee3a4p0,
    0x1.fffffffffffffp-1

  },
  { // Entry 1274
    -0x1.8eb245cbee3a4p0,
    -0x1.fffffffffffffp-1

  },
  { // Entry 1275
    0x1.fffffffffffffp-1,
    0x1.921fb54442d18p-1

  },
  { // Entry 1276
    -0x1.fffffffffffffp-1,
    -0x1.921fb54442d18p-1

  },
  { // Entry 1277
    0x1.0000000000001p-1022,
    0x1.0000000000001p-1022

  },
  { // Entry 1278
    -0x1.0000000000001p-1022,
    -0x1.0000000000001p-1022

  },
  { // Entry 1279
    0x1.0p-1022,
    0x1.0p-1022

  },
  { // Entry 1280
    -0x1.0p-1022,
    -0x1.0p-1022

  },
  { // Entry 1281
    0x0.fffffffffffffp-1022,
    0x0.fffffffffffffp-1022

  },
  { // Entry 1282
    -0x0.fffffffffffffp-1022,
    -0x0.fffffffffffffp-1022

  },
  { // Entry 1283
    0x0.ffffffffffffep-1022,
    0x0.ffffffffffffep-1022

  },
  { // Entry 1284
    -0x0.ffffffffffffep-1022,
    -0x0.ffffffffffffep-1022

  },
  { // Entry 1285
    0x0.0000000000002p-1022,
    0x0.0000000000002p-1022

  },
  { // Entry 1286
    -0x0.0000000000002p-1022,
    -0x0.0000000000002p-1022

  },
  { // Entry 1287
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 1288
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 1289
    0x0.0p0,
    0x0.0p0

  },
  { // Entry 1290
    -0x0.0p0,
    -0x0.0p0

  },
  { // Entry 1291
    -0x1.cd5cdb683b402p-2,
    0x1.5bf0a8b145769p1
  },
  { // Entry 1292
    0x1.cd5cdb683b402p-2,
    -0x1.5bf0a8b145769p1
  },
  { // Entry 1293
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 1294
    -0x0.0p0,
    -0x0.0p0
  },
  { // Entry 1295
    0x1.8eb245cbee3a6p0,
    0x1.0p0
  },
  { // Entry 1296
    -0x1.8eb245cbee3a6p0,
    -0x1.0p0
  },
  { // Entry 1297
    -0x1.17b4f5bf3474ap-1,
    -0x1.0p-1
  },
  { // Entry 1298
    0x1.17b4f5bf3474ap-1,
    0x1.0p-1
  },
  { // Entry 1299
    0x1.1a62633145c07p-52,
    -0x1.921fb54442d18p2
  },
  { // Entry 1300
    -0x1.3570efd768923p52,
    -0x1.2d97c7f3321d2p2
  },
  { // Entry 1301
    0x1.1a62633145c07p-53,
    -0x1.921fb54442d18p1
  },
  { // Entry 1302
    -0x1.d02967c31cdb5p53,
    -0x1.921fb54442d18p0
  },
  { // Entry 1303
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 1304
    0x1.d02967c31cdb5p53,
    0x1.921fb54442d18p0
  },
  { // Entry 1305
    -0x1.1a62633145c07p-53,
    0x1.921fb54442d18p1
  },
  { // Entry 1306
    0x1.3570efd768923p52,
    0x1.2d97c7f3321d2p2
  },
  { // Entry 1307
    -0x1.1a62633145c07p-52,
    0x1.921fb54442d18p2
  },
};

static TestCase g_log10_data[] = {
  { // Entry 0
    0x1.bcb7b1526e50ep-2,
    0x1.5bf0a8b145769p1
  },
  { // Entry 1
    0x0.0p0,
    0x1.0p0
  },
  { // Entry 2
    -0x1.34413509f79ffp-2,
    0x1.0p-1
  },
  { // Entry 3
    0x1.91a74c4f85377p-3,
    0x1.921fb54442d18p0
  },
  { // Entry 4
    0x1.fd14db31ba3bap-2,
    0x1.921fb54442d18p1
  },
  { // Entry 5
    0x1.58b30fc65ffdcp-1,
    0x1.2d97c7f3321d2p2
  },
  { // Entry 6
    0x1.98ab081dd8eddp-1,
    0x1.921fb54442d18p2
  },
};

static TestCase g_sin_data[] = {
  { // Entry 0
    0x1.9259e3708bd3ap-5,
    0x1.9283586503fep-5

  },
  { // Entry 1
    -0x1.9259e3708bd3ap-5,
    -0x1.9283586503fep-5

  },
  { // Entry 2
    0x1.d77b117f230d6p-5,
    0x1.d7bdcd778049fp-5

  },
  { // Entry 3
    -0x1.d77b117f230d6p-5,
    -0x1.d7bdcd778049fp-5

  },
  { // Entry 4
    0x1.a1490c8c06ba7p-4,
    0x1.a202b3fb84788p-4

  },
  { // Entry 5
    -0x1.a1490c8c06ba7p-4,
    -0x1.a202b3fb84788p-4

  },
  { // Entry 6
    0x1.cc40c3805229ap-3,
    0x1.d037cb27ee6dfp-3

  },
  { // Entry 7
    -0x1.cc40c3805229ap-3,
    -0x1.d037cb27ee6dfp-3

  },
  { // Entry 8
    0x1.d0ef799001ba9p-3,
    0x1.d5064e6fe82c5p-3

  },
  { // Entry 9
    -0x1.d0ef799001ba9p-3,
    -0x1.d5064e6fe82c5p-3

  },
  { // Entry 10
    0x1.e9950730c4696p-2,
    0x1.fe767739d0f6dp-2

  },
  { // Entry 11
    -0x1.e9950730c4696p-2,
    -0x1.fe767739d0f6dp-2

  },
  { // Entry 12
    0x1.98dcd09337793p-1,
    0x1.d98c4c612718dp-1

  },
  { // Entry 13
    -0x1.98dcd09337793p-1,
    -0x1.d98c4c612718dp-1

  },
  { // Entry 14
    0x1.0p0,
    0x1.921fb54442d18p0

  },
  { // Entry 15
    -0x1.0p0,
    -0x1.921fb54442d18p0

  },
  { // Entry 16
    0x1.70a9d825b5064p-1,
    -0x1.0000001f8p500

  },
  { // Entry 17
    -0x1.70a9d825b5064p-1,
    0x1.0000001f8p500

  },
  { // Entry 18
    0x1.bf3980c6c1e9fp-1,
    -0x1.00c0bf8p700

  },
  { // Entry 19
    -0x1.bf3980c6c1e9fp-1,
    0x1.00c0bf8p700

  },
  { // Entry 20
    0x1.d62899d48b43ap-4,
    -0x1.13fffffffff8p6

  },
  { // Entry 21
    -0x1.d62899d48b43ap-4,
    0x1.13fffffffff8p6

  },
  { // Entry 22
    -0x1.17b7a60ce1f15p-5,
    -0x1.17c5920767dfcp-5

  },
  { // Entry 23
    0x1.17b7a60ce1f15p-5,
    0x1.17c5920767dfcp-5

  },
  { // Entry 24
    0x1.f0192b794fbbep-1,
    -0x1.1d99be08713ccp2

  },
  { // Entry 25
    -0x1.f0192b794fbbep-1,
    0x1.1d99be08713ccp2

  },
  { // Entry 26
    -0x1.5e61328c0034fp-3,
    -0x1.1ddbfd64fc0d3p81

  },
  { // Entry 27
    0x1.5e61328c0034fp-3,
    0x1.1ddbfd64fc0d3p81

  },
  { // Entry 28
    0x1.fb028c5df1db4p-1,
    -0x1.1e2a1563e068ep7

  },
  { // Entry 29
    -0x1.fb028c5df1db4p-1,
    0x1.1e2a1563e068ep7

  },
  { // Entry 30
    -0x1.2cefb196ba208p-3,
    -0x1.2e07a91314dp-3

  },
  { // Entry 31
    0x1.2cefb196ba208p-3,
    0x1.2e07a91314dp-3

  },
  { // Entry 32
    0x1.b80f489d3edf5p-2,
    -0x1.3bcec270444e2p3

  },
  { // Entry 33
    -0x1.b80f489d3edf5p-2,
    0x1.3bcec270444e2p3

  },
  { // Entry 34
    -0x1.4fffffffffa39p-20,
    -0x1.500000000004p-20

  },
  { // Entry 35
    0x1.4fffffffffa39p-20,
    0x1.500000000004p-20

  },
  { // Entry 36
    -0x1.d29da5b44f51cp-2,
    -0x1.559001a42d90cp1

  },
  { // Entry 37
    0x1.d29da5b44f51cp-2,
    0x1.559001a42d90cp1

  },
  { // Entry 38
    -0x1.f85f526147f78p-1,
    -0x1.597bf3e9776b7p99

  },
  { // Entry 39
    0x1.f85f526147f78p-1,
    0x1.597bf3e9776b7p99

  },
  { // Entry 40
    -0x1.6d61b58c99c43p-59,
    -0x1.6c6cbc45dc8dep7

  },
  { // Entry 41
    0x1.6d61b58c99c43p-59,
    0x1.6c6cbc45dc8dep7

  },
  { // Entry 42
    0x1.e5c3c08a258a8p-1,
    -0x1.73d8d173f90dp4

  },
  { // Entry 43
    -0x1.e5c3c08a258a8p-1,
    0x1.73d8d173f90dp4

  },
  { // Entry 44
    0x1.feb36806ca5fbp-1,
    -0x1.8c202d3a31802p6

  },
  { // Entry 45
    -0x1.feb36806ca5fbp-1,
    0x1.8c202d3a31802p6

  },
  { // Entry 46
    -0x1.7c6c7b01b98dap-1,
    -0x1.acd538b1a6d5dp-1

  },
  { // Entry 47
    0x1.7c6c7b01b98dap-1,
    0x1.acd538b1a6d5dp-1

  },
  { // Entry 48
    -0x1.191be2059dcb6p-1,
    -0x1.b7525ac97e0d2p2

  },
  { // Entry 49
    0x1.191be2059dcb6p-1,
    0x1.b7525ac97e0d2p2

  },
  { // Entry 50
    -0x1.f8305993a212cp-1,
    -0x1.bee5fa8a84b02p0

  },
  { // Entry 51
    0x1.f8305993a212cp-1,
    0x1.bee5fa8a84b02p0

  },
  { // Entry 52
    0x1.ff3b13530fd71p-1,
    -0x1.c393979fe5921p9

  },
  { // Entry 53
    -0x1.ff3b13530fd71p-1,
    0x1.c393979fe5921p9

  },
  { // Entry 54
    -0x1.f119da81a4da6p-1,
    -0x1.c48ffc72563c8p18

  },
  { // Entry 55
    0x1.f119da81a4da6p-1,
    0x1.c48ffc72563c8p18

  },
  { // Entry 56
    -0x1.fd73b81e04cccp-1,
    -0x1.c79548bc31856p3

  },
  { // Entry 57
    0x1.fd73b81e04cccp-1,
    0x1.c79548bc31856p3

  },
  { // Entry 58
    -0x1.c7885aef33a95p-3,
    -0x1.cb6p-3

  },
  { // Entry 59
    0x1.c7885aef33a95p-3,
    0x1.cb6p-3

  },
  { // Entry 60
    -0x1.e6494911eedd1p-7,
    -0x1.e64ddaf7bd72fp-7

  },
  { // Entry 61
    0x1.e6494911eedd1p-7,
    0x1.e64ddaf7bd72fp-7

  },
  { // Entry 62
    0x1.e180eef5b1c88p-1,
    -0x1.ecdd0fbf07942p5

  },
  { // Entry 63
    -0x1.e180eef5b1c88p-1,
    0x1.ecdd0fbf07942p5

  },
  { // Entry 64
    -0x1.fd98d20c1be44p-1,
    -0x1.f073a23292337p2

  },
  { // Entry 65
    0x1.fd98d20c1be44p-1,
    0x1.f073a23292337p2

  },
  { // Entry 66
    -0x1.7268c112297c8p-5,
    -0x1.f5e4c410f4ef8p15

  },
  { // Entry 67
    0x1.7268c112297c8p-5,
    0x1.f5e4c410f4ef8p15

  },
  { // Entry 68
    0x1.420796146070ep-18,
    -0x1.f8000000002p95

  },
  { // Entry 69
    -0x1.420796146070ep-18,
    0x1.f8000000002p95

  },
  { // Entry 70
    -0x1.e4f6dc499d9ccp-2,
    -0x1.f9365d79546e1p-2

  },
  { // Entry 71
    0x1.e4f6dc499d9ccp-2,
    0x1.f9365d79546e1p-2

  },
  { // Entry 72
    0x1.b2ef99b140d65p-14,
    -0x1.ffffffffffe7ep1023

  },
  { // Entry 73
    -0x1.b2ef99b140d65p-14,
    0x1.ffffffffffe7ep1023

  },
  { // Entry 74
    0x1.db0ffc3ecc6e4p-1,
    0x1.0p15

  },
  { // Entry 75
    -0x1.db0ffc3ecc6e4p-1,
    -0x1.0p15

  },
  { // Entry 76
    -0x1.e98f87098b627p-1,
    0x1.0000000000001p13

  },
  { // Entry 77
    0x1.e98f87098b627p-1,
    -0x1.0000000000001p13

  },
  { // Entry 78
    0x1.053c35068e10dp-4,
    0x1.0000000000001p52

  },
  { // Entry 79
    -0x1.053c35068e10dp-4,
    -0x1.0000000000001p52

  },
  { // Entry 80
    0x1.72d421b6884e5p-1,
    0x1.0000000000001p228

  },
  { // Entry 81
    -0x1.72d421b6884e5p-1,
    -0x1.0000000000001p228

  },
  { // Entry 82
    0x1.77fba987c5654p-1,
    0x1.0000000000001p491

  },
  { // Entry 83
    -0x1.77fba987c5654p-1,
    -0x1.0000000000001p491

  },
  { // Entry 84
    -0x1.723b2625331afp-1,
    0x1.0000000000003p215

  },
  { // Entry 85
    0x1.723b2625331afp-1,
    -0x1.0000000000003p215

  },
  { // Entry 86
    0x1.aed548f090cf5p-1,
    0x1.0000000000006p0

  },
  { // Entry 87
    -0x1.aed548f090cf5p-1,
    -0x1.0000000000006p0

  },
  { // Entry 88
    -0x1.ff983208c7dc9p-1,
    0x1.0000000000007p8

  },
  { // Entry 89
    0x1.ff983208c7dc9p-1,
    -0x1.0000000000007p8

  },
  { // Entry 90
    0x1.ffef29dc38453p-1,
    0x1.0000000000007p275

  },
  { // Entry 91
    -0x1.ffef29dc38453p-1,
    -0x1.0000000000007p275

  },
  { // Entry 92
    -0x1.fa88c375723c1p-8,
    0x1.0000000000007p449

  },
  { // Entry 93
    0x1.fa88c375723c1p-8,
    -0x1.0000000000007p449

  },
  { // Entry 94
    0x1.fff5322c94eaep-1,
    0x1.0000000000011p644

  },
  { // Entry 95
    -0x1.fff5322c94eaep-1,
    -0x1.0000000000011p644

  },
  { // Entry 96
    -0x1.a73630af8f15cp-1,
    0x1.000000000001fp164

  },
  { // Entry 97
    0x1.a73630af8f15cp-1,
    -0x1.000000000001fp164

  },
  { // Entry 98
    0x1.1c548f9249e44p-2,
    0x1.0000000000038p380

  },
  { // Entry 99
    -0x1.1c548f9249e44p-2,
    -0x1.0000000000038p380

  },
  { // Entry 100
    0x1.ca965bd2c4dffp-3,
    0x1.0000000000118p380

  },
  { // Entry 101
    -0x1.ca965bd2c4dffp-3,
    -0x1.0000000000118p380

  },
  { // Entry 102
    -0x1.837b9dddc24dp-1,
    0x1.000000000012cp2

  },
  { // Entry 103
    0x1.837b9dddc24dp-1,
    -0x1.000000000012cp2

  },
  { // Entry 104
    0x1.d82c1784c3eccp-2,
    0x1.00000000001f8p700

  },
  { // Entry 105
    -0x1.d82c1784c3eccp-2,
    -0x1.00000000001f8p700

  },
  { // Entry 106
    0x1.fffeaaaaef2eep-8,
    0x1.00000000002p-7

  },
  { // Entry 107
    -0x1.fffeaaaaef2eep-8,
    -0x1.00000000002p-7

  },
  { // Entry 108
    -0x1.0871bddd90fc6p-1,
    0x1.00000000002p40

  },
  { // Entry 109
    0x1.0871bddd90fc6p-1,
    -0x1.00000000002p40

  },
  { // Entry 110
    0x1.fffeaaaaef2fp-8,
    0x1.0000000000201p-7

  },
  { // Entry 111
    -0x1.fffeaaaaef2fp-8,
    -0x1.0000000000201p-7

  },
  { // Entry 112
    0x1.fffeaaaaef33p-8,
    0x1.0000000000221p-7

  },
  { // Entry 113
    -0x1.fffeaaaaef33p-8,
    -0x1.0000000000221p-7

  },
  { // Entry 114
    0x1.fffeaaaaef362p-8,
    0x1.000000000023ap-7

  },
  { // Entry 115
    -0x1.fffeaaaaef362p-8,
    -0x1.000000000023ap-7

  },
  { // Entry 116
    0x1.e0c6edfa93601p-9,
    0x1.0000000004p45

  },
  { // Entry 117
    -0x1.e0c6edfa93601p-9,
    -0x1.0000000004p45

  },
  { // Entry 118
    0x1.ea1f618356db1p-5,
    0x1.0000000cp40

  },
  { // Entry 119
    -0x1.ea1f618356db1p-5,
    -0x1.0000000cp40

  },
  { // Entry 120
    0x1.faaeed7587542p-3,
    0x1.00000013c86f4p-2

  },
  { // Entry 121
    -0x1.faaeed7587542p-3,
    -0x1.00000013c86f4p-2

  },
  { // Entry 122
    0x1.540bc7785680bp-1,
    0x1.001p13

  },
  { // Entry 123
    -0x1.540bc7785680bp-1,
    -0x1.001p13

  },
  { // Entry 124
    -0x1.37a7cb907a2e5p-1,
    0x1.003p699

  },
  { // Entry 125
    0x1.37a7cb907a2e5p-1,
    -0x1.003p699

  },
  { // Entry 126
    -0x1.29e5845fc54b5p-1,
    0x1.0038p40

  },
  { // Entry 127
    0x1.29e5845fc54b5p-1,
    -0x1.0038p40

  },
  { // Entry 128
    0x1.ffe5ca4656491p-1,
    0x1.007p10

  },
  { // Entry 129
    -0x1.ffe5ca4656491p-1,
    -0x1.007p10

  },
  { // Entry 130
    0x1.ea4df82db014bp-1,
    0x1.007p25

  },
  { // Entry 131
    -0x1.ea4df82db014bp-1,
    -0x1.007p25

  },
  { // Entry 132
    0x1.fe757aef1c80cp-1,
    0x1.007p41

  },
  { // Entry 133
    -0x1.fe757aef1c80cp-1,
    -0x1.007p41

  },
  { // Entry 134
    0x1.e9b71805ec068p-7,
    0x1.00cp41

  },
  { // Entry 135
    -0x1.e9b71805ec068p-7,
    -0x1.00cp41

  },
  { // Entry 136
    0x1.b0b6d0a540583p-1,
    0x1.01c00000001p0

  },
  { // Entry 137
    -0x1.b0b6d0a540583p-1,
    -0x1.01c00000001p0

  },
  { // Entry 138
    0x1.fef0092627012p-3,
    0x1.02322e46da919p-2

  },
  { // Entry 139
    -0x1.fef0092627012p-3,
    -0x1.02322e46da919p-2

  },
  { // Entry 140
    0x1.ffc90059804a1p-3,
    0x1.02a236478p-2

  },
  { // Entry 141
    -0x1.ffc90059804a1p-3,
    -0x1.02a236478p-2

  },
  { // Entry 142
    0x1.ffd10a6b5429fp-3,
    0x1.02a65d08ca5e5p-2

  },
  { // Entry 143
    -0x1.ffd10a6b5429fp-3,
    -0x1.02a65d08ca5e5p-2

  },
  { // Entry 144
    0x1.ffd10ab302a3fp-3,
    0x1.02a65d2dce49ap-2

  },
  { // Entry 145
    -0x1.ffd10ab302a3fp-3,
    -0x1.02a65d2dce49ap-2

  },
  { // Entry 146
    0x1.ffe0b1764ca4cp-3,
    0x1.02ae7238ap-2

  },
  { // Entry 147
    -0x1.ffe0b1764ca4cp-3,
    -0x1.02ae7238ap-2

  },
  { // Entry 148
    -0x1.f68f0e26c0f6bp-3,
    0x1.0501d22221dacp621

  },
  { // Entry 149
    0x1.f68f0e26c0f6bp-3,
    -0x1.0501d22221dacp621

  },
  { // Entry 150
    0x1.b63c41f09eb75p-1,
    0x1.06ffffffffff8p0

  },
  { // Entry 151
    -0x1.b63c41f09eb75p-1,
    -0x1.06ffffffffff8p0

  },
  { // Entry 152
    -0x1.ffdc173adabb2p-1,
    0x1.07023d3d44215p12

  },
  { // Entry 153
    0x1.ffdc173adabb2p-1,
    -0x1.07023d3d44215p12

  },
  { // Entry 154
    0x1.0889e11bef135p-5,
    0x1.0895a7a3e8ae6p-5

  },
  { // Entry 155
    -0x1.0889e11bef135p-5,
    -0x1.0895a7a3e8ae6p-5

  },
  { // Entry 156
    0x1.08ca077c76445p-5,
    0x1.08d5d69840601p-5

  },
  { // Entry 157
    -0x1.08ca077c76445p-5,
    -0x1.08d5d69840601p-5

  },
  { // Entry 158
    -0x1.ff7fbe518023fp-1,
    0x1.0ep6

  },
  { // Entry 159
    0x1.ff7fbe518023fp-1,
    -0x1.0ep6

  },
  { // Entry 160
    -0x1.fd6c68b877afep-1,
    0x1.107ba49c346e4p9

  },
  { // Entry 161
    0x1.fd6c68b877afep-1,
    -0x1.107ba49c346e4p9

  },
  { // Entry 162
    -0x1.a2ba6bc70bce4p-1,
    0x1.149154477444p745

  },
  { // Entry 163
    0x1.a2ba6bc70bce4p-1,
    -0x1.149154477444p745

  },
  { // Entry 164
    0x1.165609790f235p-5,
    0x1.1663c0e51818p-5

  },
  { // Entry 165
    -0x1.165609790f235p-5,
    -0x1.1663c0e51818p-5

  },
  { // Entry 166
    -0x1.fc0523ff94e45p-1,
    0x1.1745d1745d176p238

  },
  { // Entry 167
    0x1.fc0523ff94e45p-1,
    -0x1.1745d1745d176p238

  },
  { // Entry 168
    0x1.f34a729c584bdp-1,
    0x1.17472a408a3ep97

  },
  { // Entry 169
    -0x1.f34a729c584bdp-1,
    -0x1.17472a408a3ep97

  },
  { // Entry 170
    0x1.177fae169fdf1p-5,
    0x1.178d91b6b992dp-5

  },
  { // Entry 171
    -0x1.177fae169fdf1p-5,
    -0x1.178d91b6b992dp-5

  },
  { // Entry 172
    0x1.177fae16a120fp-5,
    0x1.178d91b6bad4ep-5

  },
  { // Entry 173
    -0x1.177fae16a120fp-5,
    -0x1.178d91b6bad4ep-5

  },
  { // Entry 174
    0x1.177fae16a1f79p-5,
    0x1.178d91b6bbabap-5

  },
  { // Entry 175
    -0x1.177fae16a1f79p-5,
    -0x1.178d91b6bbabap-5

  },
  { // Entry 176
    0x1.177fae16a40ffp-5,
    0x1.178d91b6bdc45p-5

  },
  { // Entry 177
    -0x1.177fae16a40ffp-5,
    -0x1.178d91b6bdc45p-5

  },
  { // Entry 178
    0x1.297c768f2413p-1,
    0x1.19752dbee5f6ap933

  },
  { // Entry 179
    -0x1.297c768f2413p-1,
    -0x1.19752dbee5f6ap933

  },
  { // Entry 180
    0x1.b826df5cafafap-2,
    0x1.1b3009cfe4dbcp8

  },
  { // Entry 181
    -0x1.b826df5cafafap-2,
    -0x1.1b3009cfe4dbcp8

  },
  { // Entry 182
    0x1.b7a5956250b6bp-2,
    0x1.1f6475d95bf18p3

  },
  { // Entry 183
    -0x1.b7a5956250b6bp-2,
    -0x1.1f6475d95bf18p3

  },
  { // Entry 184
    0x1.4db6566b64548p-1,
    0x1.229148a452291p118

  },
  { // Entry 185
    -0x1.4db6566b64548p-1,
    -0x1.229148a452291p118

  },
  { // Entry 186
    0x1.1686fee2c49a8p-1,
    0x1.268p-1

  },
  { // Entry 187
    -0x1.1686fee2c49a8p-1,
    -0x1.268p-1

  },
  { // Entry 188
    0x1.22eb21a44d627p-2,
    0x1.26fb3844dd19p-2

  },
  { // Entry 189
    -0x1.22eb21a44d627p-2,
    -0x1.26fb3844dd19p-2

  },
  { // Entry 190
    0x1.d4a216d89b2b3p-1,
    0x1.27fffffffe6bp0

  },
  { // Entry 191
    -0x1.d4a216d89b2b3p-1,
    -0x1.27fffffffe6bp0

  },
  { // Entry 192
    -0x1.76c9b0f3a22f8p-1,
    0x1.284b84048d481p204

  },
  { // Entry 193
    0x1.76c9b0f3a22f8p-1,
    -0x1.284b84048d481p204

  },
  { // Entry 194
    -0x1.ff01226f97d33p-1,
    0x1.2999e3109cad4p2

  },
  { // Entry 195
    0x1.ff01226f97d33p-1,
    -0x1.2999e3109cad4p2

  },
  { // Entry 196
    0x1.2a8f11e7ae82cp-5,
    0x1.2aap-5

  },
  { // Entry 197
    -0x1.2a8f11e7ae82cp-5,
    -0x1.2aap-5

  },
  { // Entry 198
    0x1.2b03d1bf773dfp-5,
    0x1.2b14d3be0c23p-5

  },
  { // Entry 199
    -0x1.2b03d1bf773dfp-5,
    -0x1.2b14d3be0c23p-5

  },
  { // Entry 200
    -0x1.ffb90ee641792p-1,
    0x1.2b7cb44849981p2

  },
  { // Entry 201
    0x1.ffb90ee641792p-1,
    -0x1.2b7cb44849981p2

  },
  { // Entry 202
    -0x1.ffffff79e71a4p-1,
    0x1.2becc8685258p200

  },
  { // Entry 203
    0x1.ffffff79e71a4p-1,
    -0x1.2becc8685258p200

  },
  { // Entry 204
    -0x1.fff9edaf85b77p-1,
    0x1.2cfa14ce27cd5p2

  },
  { // Entry 205
    0x1.fff9edaf85b77p-1,
    -0x1.2cfa14ce27cd5p2

  },
  { // Entry 206
    0x1.2cbaaa4cebb52p-4,
    0x1.2dp-4

  },
  { // Entry 207
    -0x1.2cbaaa4cebb52p-4,
    -0x1.2dp-4

  },
  { // Entry 208
    -0x1.ffffbc177e01p-1,
    0x1.2d76d18721be8p2

  },
  { // Entry 209
    0x1.ffffbc177e01p-1,
    -0x1.2d76d18721be8p2

  },
  { // Entry 210
    0x1.745843dfafefdp-18,
    0x1.302a494e0909p97

  },
  { // Entry 211
    -0x1.745843dfafefdp-18,
    -0x1.302a494e0909p97

  },
  { // Entry 212
    0x1.ffcc568d42376p-1,
    0x1.31cc731cc731cp1000

  },
  { // Entry 213
    -0x1.ffcc568d42376p-1,
    -0x1.31cc731cc731cp1000

  },
  { // Entry 214
    0x1.b676077d4faf8p-1,
    0x1.328463d4f8ca6p441

  },
  { // Entry 215
    -0x1.b676077d4faf8p-1,
    -0x1.328463d4f8ca6p441

  },
  { // Entry 216
    -0x1.0p0,
    0x1.32ce90b32171ep18

  },
  { // Entry 217
    0x1.0p0,
    -0x1.32ce90b32171ep18

  },
  { // Entry 218
    0x1.35cbd3240d149p-5,
    0x1.35debd7f020ecp-5

  },
  { // Entry 219
    -0x1.35cbd3240d149p-5,
    -0x1.35debd7f020ecp-5

  },
  { // Entry 220
    0x1.3bb2086559faap-7,
    0x1.3bb3487893405p-7

  },
  { // Entry 221
    -0x1.3bb2086559faap-7,
    -0x1.3bb3487893405p-7

  },
  { // Entry 222
    0x1.3bb2086559facp-7,
    0x1.3bb3487893407p-7

  },
  { // Entry 223
    -0x1.3bb2086559facp-7,
    -0x1.3bb3487893407p-7

  },
  { // Entry 224
    0x1.dff197edc51d2p-16,
    0x1.3bb681d65aa6p100

  },
  { // Entry 225
    -0x1.dff197edc51d2p-16,
    -0x1.3bb681d65aa6p100

  },
  { // Entry 226
    -0x1.5d08d3dbb41bp-3,
    0x1.3f9aa8626042fp83

  },
  { // Entry 227
    0x1.5d08d3dbb41bp-3,
    -0x1.3f9aa8626042fp83

  },
  { // Entry 228
    0x1.fb503983f94bbp-3,
    0x1.3fep19

  },
  { // Entry 229
    -0x1.fb503983f94bbp-3,
    -0x1.3fep19

  },
  { // Entry 230
    -0x1.d3876eacc9ee7p-1,
    0x1.4285478f1e3c8p58

  },
  { // Entry 231
    0x1.d3876eacc9ee7p-1,
    -0x1.4285478f1e3c8p58

  },
  { // Entry 232
    0x1.42b66d54f69c1p-5,
    0x1.42cbcf45a169ep-5

  },
  { // Entry 233
    -0x1.42b66d54f69c1p-5,
    -0x1.42cbcf45a169ep-5

  },
  { // Entry 234
    0x1.b45e9e9427554p-1,
    0x1.43fffffffff6ap557

  },
  { // Entry 235
    -0x1.b45e9e9427554p-1,
    -0x1.43fffffffff6ap557

  },
  { // Entry 236
    0x1.43ffffffea603p-17,
    0x1.44p-17

  },
  { // Entry 237
    -0x1.43ffffffea603p-17,
    -0x1.44p-17

  },
  { // Entry 238
    -0x1.6a4e98d2d8b1cp-1,
    0x1.4748c08dc0976p200

  },
  { // Entry 239
    0x1.6a4e98d2d8b1cp-1,
    -0x1.4748c08dc0976p200

  },
  { // Entry 240
    -0x1.b57ca8aacf2a9p-1,
    0x1.478fc08p43

  },
  { // Entry 241
    0x1.b57ca8aacf2a9p-1,
    -0x1.478fc08p43

  },
  { // Entry 242
    0x1.ffe38008ef6b5p-1,
    0x1.4cf36d17c596ep200

  },
  { // Entry 243
    -0x1.ffe38008ef6b5p-1,
    -0x1.4cf36d17c596ep200

  },
  { // Entry 244
    0x1.d6457a3f12e6cp-1,
    0x1.4f0f308p488

  },
  { // Entry 245
    -0x1.d6457a3f12e6cp-1,
    -0x1.4f0f308p488

  },
  { // Entry 246
    0x1.4fffffffff9f9p-20,
    0x1.5p-20

  },
  { // Entry 247
    -0x1.4fffffffff9f9p-20,
    -0x1.5p-20

  },
  { // Entry 248
    -0x1.cbad095f503a2p-1,
    0x1.5143e25a488f1p3

  },
  { // Entry 249
    0x1.cbad095f503a2p-1,
    -0x1.5143e25a488f1p3

  },
  { // Entry 250
    -0x1.f942d6262e82ep-5,
    0x1.51f0f44da4df4p200

  },
  { // Entry 251
    0x1.f942d6262e82ep-5,
    -0x1.51f0f44da4df4p200

  },
  { // Entry 252
    -0x1.fc466ccaece8p-3,
    0x1.52ad6c5a3602fp16

  },
  { // Entry 253
    0x1.fc466ccaece8p-3,
    -0x1.52ad6c5a3602fp16

  },
  { // Entry 254
    0x1.d69c3cf4eecdep-1,
    0x1.52f00ep793

  },
  { // Entry 255
    -0x1.d69c3cf4eecdep-1,
    -0x1.52f00ep793

  },
  { // Entry 256
    0x1.e120292f3d495p-1,
    0x1.5555555555556p239

  },
  { // Entry 257
    -0x1.e120292f3d495p-1,
    -0x1.5555555555556p239

  },
  { // Entry 258
    -0x1.fd1d85b7ef004p-1,
    0x1.5a0000008p6

  },
  { // Entry 259
    0x1.fd1d85b7ef004p-1,
    -0x1.5a0000008p6

  },
  { // Entry 260
    0x1.5aff9664b07e2p-6,
    0x1.5b063ad2dd08fp-6

  },
  { // Entry 261
    -0x1.5aff9664b07e2p-6,
    -0x1.5b063ad2dd08fp-6

  },
  { // Entry 262
    -0x1.83f8bbb59f2f8p-1,
    0x1.5b179d75fa285p2

  },
  { // Entry 263
    0x1.83f8bbb59f2f8p-1,
    -0x1.5b179d75fa285p2

  },
  { // Entry 264
    0x1.fa865b0d99497p-1,
    0x1.5bb5967402f9cp79

  },
  { // Entry 265
    -0x1.fa865b0d99497p-1,
    -0x1.5bb5967402f9cp79

  },
  { // Entry 266
    0x1.e8a523fce884dp-2,
    0x1.5bea01p468

  },
  { // Entry 267
    -0x1.e8a523fce884dp-2,
    -0x1.5bea01p468

  },
  { // Entry 268
    -0x1.ff2ad941f0a41p-1,
    0x1.5f19fbc507af6p9

  },
  { // Entry 269
    0x1.ff2ad941f0a41p-1,
    -0x1.5f19fbc507af6p9

  },
  { // Entry 270
    -0x1.75ce4a0d0bd03p-1,
    0x1.60a610a658da9p889

  },
  { // Entry 271
    0x1.75ce4a0d0bd03p-1,
    -0x1.60a610a658da9p889

  },
  { // Entry 272
    -0x1.721586594ab48p-1,
    0x1.62ad7ce17143dp62

  },
  { // Entry 273
    0x1.721586594ab48p-1,
    -0x1.62ad7ce17143dp62

  },
  { // Entry 274
    0x1.b8d27019d1b9fp-2,
    0x1.645926cc1132cp9

  },
  { // Entry 275
    -0x1.b8d27019d1b9fp-2,
    -0x1.645926cc1132cp9

  },
  { // Entry 276
    0x1.647e09059c1eap-9,
    0x1.647e25d391f17p-9

  },
  { // Entry 277
    -0x1.647e09059c1eap-9,
    -0x1.647e25d391f17p-9

  },
  { // Entry 278
    -0x1.8d3b53ff85a82p-1,
    0x1.64ef438p142

  },
  { // Entry 279
    0x1.8d3b53ff85a82p-1,
    -0x1.64ef438p142

  },
  { // Entry 280
    -0x1.f7c8630e62a02p-1,
    0x1.6599665996658p3

  },
  { // Entry 281
    0x1.f7c8630e62a02p-1,
    -0x1.6599665996658p3

  },
  { // Entry 282
    0x1.67028e3602035p-5,
    0x1.672p-5

  },
  { // Entry 283
    -0x1.67028e3602035p-5,
    -0x1.672p-5

  },
  { // Entry 284
    0x1.bc60c8c33cb5fp-2,
    0x1.688ae6c138ea8p299

  },
  { // Entry 285
    -0x1.bc60c8c33cb5fp-2,
    -0x1.688ae6c138ea8p299

  },
  { // Entry 286
    -0x1.fc3b4bb8b012ep-1,
    0x1.6aa78p17

  },
  { // Entry 287
    0x1.fc3b4bb8b012ep-1,
    -0x1.6aa78p17

  },
  { // Entry 288
    0x1.0p0,
    0x1.6ac5b262ca1ffp849

  },
  { // Entry 289
    -0x1.0p0,
    -0x1.6ac5b262ca1ffp849

  },
  { // Entry 290
    -0x1.82317836a97c8p-1,
    0x1.6d88083749412p4

  },
  { // Entry 291
    0x1.82317836a97c8p-1,
    -0x1.6d88083749412p4

  },
  { // Entry 292
    0x1.6f781c78cc82bp-6,
    0x1.6f8p-6

  },
  { // Entry 293
    -0x1.6f781c78cc82bp-6,
    -0x1.6f8p-6

  },
  { // Entry 294
    -0x1.fdbe5085494aep-1,
    0x1.729aa6859d1f4p396

  },
  { // Entry 295
    0x1.fdbe5085494aep-1,
    -0x1.729aa6859d1f4p396

  },
  { // Entry 296
    -0x1.fffffae862b5p-1,
    0x1.73e2dbe9a2f8p10

  },
  { // Entry 297
    0x1.fffffae862b5p-1,
    -0x1.73e2dbe9a2f8p10

  },
  { // Entry 298
    0x1.769ac74459b06p-7,
    0x1.769cde0b90b8p-7

  },
  { // Entry 299
    -0x1.769ac74459b06p-7,
    -0x1.769cde0b90b8p-7

  },
  { // Entry 300
    0x1.769e8afb6a4ecp-5,
    0x1.76cp-5

  },
  { // Entry 301
    -0x1.769e8afb6a4ecp-5,
    -0x1.76cp-5

  },
  { // Entry 302
    0x1.fd562611f5bd4p-1,
    0x1.78001p0

  },
  { // Entry 303
    -0x1.fd562611f5bd4p-1,
    -0x1.78001p0

  },
  { // Entry 304
    0x1.fdba784ca00f2p-1,
    0x1.7ap0

  },
  { // Entry 305
    -0x1.fdba784ca00f2p-1,
    -0x1.7ap0

  },
  { // Entry 306
    0x1.f930c222a8683p-5,
    0x1.7abd870381c2dp38

  },
  { // Entry 307
    -0x1.f930c222a8683p-5,
    -0x1.7abd870381c2dp38

  },
  { // Entry 308
    0x1.ffeb2ff2b6923p-1,
    0x1.7dc945c21248p95

  },
  { // Entry 309
    -0x1.ffeb2ff2b6923p-1,
    -0x1.7dc945c21248p95

  },
  { // Entry 310
    0x1.b279153c23fb2p-2,
    0x1.7f73e1594b70cp98

  },
  { // Entry 311
    -0x1.b279153c23fb2p-2,
    -0x1.7f73e1594b70cp98

  },
  { // Entry 312
    -0x1.599fad35cf60bp-41,
    0x1.7f7ef77e83f1ap21

  },
  { // Entry 313
    0x1.599fad35cf60bp-41,
    -0x1.7f7ef77e83f1ap21

  },
  { // Entry 314
    0x1.feb7a9b2c6d8bp-1,
    0x1.8p0

  },
  { // Entry 315
    -0x1.feb7a9b2c6d8bp-1,
    -0x1.8p0

  },
  { // Entry 316
    0x1.f798d01ec615cp-1,
    0x1.8p6

  },
  { // Entry 317
    -0x1.f798d01ec615cp-1,
    -0x1.8p6

  },
  { // Entry 318
    0x1.fee1a2a977bcfp-1,
    0x1.8132ceb1c4f39p0

  },
  { // Entry 319
    -0x1.fee1a2a977bcfp-1,
    -0x1.8132ceb1c4f39p0

  },
  { // Entry 320
    -0x1.24245af4cd995p-52,
    0x1.81ae0dffa3b33p959

  },
  { // Entry 321
    0x1.24245af4cd995p-52,
    -0x1.81ae0dffa3b33p959

  },
  { // Entry 322
    0x1.85d41b0bf3091p-4,
    0x1.85ec5a399a2e6p1

  },
  { // Entry 323
    -0x1.85d41b0bf3091p-4,
    -0x1.85ec5a399a2e6p1

  },
  { // Entry 324
    0x1.1e42ae3cfbdc6p-24,
    0x1.86a0092754022p16

  },
  { // Entry 325
    -0x1.1e42ae3cfbdc6p-24,
    -0x1.86a0092754022p16

  },
  { // Entry 326
    -0x1.dbf4e594cefe1p-1,
    0x1.8720588p392

  },
  { // Entry 327
    0x1.dbf4e594cefe1p-1,
    -0x1.8720588p392

  },
  { // Entry 328
    0x1.44302d6a82d4p-9,
    0x1.8929354ebc6aap43

  },
  { // Entry 329
    -0x1.44302d6a82d4p-9,
    -0x1.8929354ebc6aap43

  },
  { // Entry 330
    0x1.8a52189ec3487p-5,
    0x1.8a791e4791e75p-5

  },
  { // Entry 331
    -0x1.8a52189ec3487p-5,
    -0x1.8a791e4791e75p-5

  },
  { // Entry 332
    -0x1.fe8566e538123p-1,
    0x1.8ba761438f5edp11

  },
  { // Entry 333
    0x1.fe8566e538123p-1,
    -0x1.8ba761438f5edp11

  },
  { // Entry 334
    0x1.fff42aca4cb5ap-1,
    0x1.8eaf16de6392p0

  },
  { // Entry 335
    -0x1.fff42aca4cb5ap-1,
    -0x1.8eaf16de6392p0

  },
  { // Entry 336
    0x1.fffb7d3f3a253p-1,
    0x1.9p0

  },
  { // Entry 337
    -0x1.fffb7d3f3a253p-1,
    -0x1.9p0

  },
  { // Entry 338
    -0x1.e815770667fd9p-4,
    0x1.91a5657fb6a9ap6

  },
  { // Entry 339
    0x1.e815770667fd9p-4,
    -0x1.91a5657fb6a9ap6

  },
  { // Entry 340
    -0x1.fffffffd311dcp-1,
    0x1.921fb54468847p37

  },
  { // Entry 341
    0x1.fffffffd311dcp-1,
    -0x1.921fb54468847p37

  },
  { // Entry 342
    0x1.ffffffff875e6p-17,
    0x1.921ff54442d18p2

  },
  { // Entry 343
    -0x1.ffffffff875e6p-17,
    -0x1.921ff54442d18p2

  },
  { // Entry 344
    0x1.812a5da3777cdp-8,
    0x1.928p2

  },
  { // Entry 345
    -0x1.812a5da3777cdp-8,
    -0x1.928p2

  },
  { // Entry 346
    0x1.fff9be8d82573p-1,
    0x1.94ap0

  },
  { // Entry 347
    -0x1.fff9be8d82573p-1,
    -0x1.94ap0

  },
  { // Entry 348
    0x1.947b0ace235f3p-5,
    0x1.94a5294a51bdep-5

  },
  { // Entry 349
    -0x1.947b0ace235f3p-5,
    -0x1.94a5294a51bdep-5

  },
  { // Entry 350
    0x1.c34f70e55a708p-2,
    0x1.94a5294a52948p100

  },
  { // Entry 351
    -0x1.c34f70e55a708p-2,
    -0x1.94a5294a52948p100

  },
  { // Entry 352
    0x1.950bcfc0f3d51p-5,
    0x1.95361b8f7697dp-5

  },
  { // Entry 353
    -0x1.950bcfc0f3d51p-5,
    -0x1.95361b8f7697dp-5

  },
  { // Entry 354
    0x1.6c548bfcce696p-1,
    0x1.956p-1

  },
  { // Entry 355
    -0x1.6c548bfcce696p-1,
    -0x1.956p-1

  },
  { // Entry 356
    0x1.ffeffdbf67ca6p-1,
    0x1.962p0

  },
  { // Entry 357
    -0x1.ffeffdbf67ca6p-1,
    -0x1.962p0

  },
  { // Entry 358
    0x1.9708213bf67f5p-5,
    0x1.97330d2ea16d9p-5

  },
  { // Entry 359
    -0x1.9708213bf67f5p-5,
    -0x1.97330d2ea16d9p-5

  },
  { // Entry 360
    0x1.972bf92713d51p-5,
    0x1.9756f073b6b61p-5

  },
  { // Entry 361
    -0x1.972bf92713d51p-5,
    -0x1.9756f073b6b61p-5

  },
  { // Entry 362
    0x1.976845ebe7119p-5,
    0x1.97935055cec1bp-5

  },
  { // Entry 363
    -0x1.976845ebe7119p-5,
    -0x1.97935055cec1bp-5

  },
  { // Entry 364
    0x1.97535cee51a43p-4,
    0x1.98p-4

  },
  { // Entry 365
    -0x1.97535cee51a43p-4,
    -0x1.98p-4

  },
  { // Entry 366
    0x1.6f494c3356177p-1,
    0x1.999999a42160cp-1

  },
  { // Entry 367
    -0x1.6f494c3356177p-1,
    -0x1.999999a42160cp-1

  },
  { // Entry 368
    0x1.6f494c37edd6ep-1,
    0x1.999999aab8f5p-1

  },
  { // Entry 369
    -0x1.6f494c37edd6ep-1,
    -0x1.999999aab8f5p-1

  },
  { // Entry 370
    0x1.6fa912bdeaab2p-1,
    0x1.9a2324b9c6326p-1

  },
  { // Entry 371
    -0x1.6fa912bdeaab2p-1,
    -0x1.9a2324b9c6326p-1

  },
  { // Entry 372
    0x1.70c7ef4ef9b34p-1,
    0x1.9bcp-1

  },
  { // Entry 373
    -0x1.70c7ef4ef9b34p-1,
    -0x1.9bcp-1

  },
  { // Entry 374
    0x1.ff28176ad3164p-1,
    0x1.a0d1d817d6c4ap0

  },
  { // Entry 375
    -0x1.ff28176ad3164p-1,
    -0x1.a0d1d817d6c4ap0

  },
  { // Entry 376
    0x1.749468a7248dep-1,
    0x1.a141c9de12fdfp-1

  },
  { // Entry 377
    -0x1.749468a7248dep-1,
    -0x1.a141c9de12fdfp-1

  },
  { // Entry 378
    0x1.754ebb7e73f46p-1,
    0x1.a251bc6766f2p-1

  },
  { // Entry 379
    -0x1.754ebb7e73f46p-1,
    -0x1.a251bc6766f2p-1

  },
  { // Entry 380
    -0x1.7c3bfefa74bd1p-1,
    0x1.a2689ae1b86ddp62

  },
  { // Entry 381
    0x1.7c3bfefa74bd1p-1,
    -0x1.a2689ae1b86ddp62

  },
  { // Entry 382
    -0x1.ffff4f3648e03p-1,
    0x1.a3f66180c455p100

  },
  { // Entry 383
    0x1.ffff4f3648e03p-1,
    -0x1.a3f66180c455p100

  },
  { // Entry 384
    -0x1.1cf463983c0e3p-3,
    0x1.a3fdd2a5286c3p1

  },
  { // Entry 385
    0x1.1cf463983c0e3p-3,
    -0x1.a3fdd2a5286c3p1

  },
  { // Entry 386
    0x1.feb7948d224d8p-1,
    0x1.a44p0

  },
  { // Entry 387
    -0x1.feb7948d224d8p-1,
    -0x1.a44p0

  },
  { // Entry 388
    0x1.78801e3e11665p-1,
    0x1.a701ef3c7d54bp-1

  },
  { // Entry 389
    -0x1.78801e3e11665p-1,
    -0x1.a701ef3c7d54bp-1

  },
  { // Entry 390
    -0x1.fff11e871d59cp-1,
    0x1.a8c01fd43cp537

  },
  { // Entry 391
    0x1.fff11e871d59cp-1,
    -0x1.a8c01fd43cp537

  },
  { // Entry 392
    0x1.fdfa4366eb733p-1,
    0x1.a8e29b7602f3bp0

  },
  { // Entry 393
    -0x1.fdfa4366eb733p-1,
    -0x1.a8e29b7602f3bp0

  },
  { // Entry 394
    0x1.fde98b94e7948p-1,
    0x1.a94p0

  },
  { // Entry 395
    -0x1.fde98b94e7948p-1,
    -0x1.a94p0

  },
  { // Entry 396
    0x1.7931cba100008p-2,
    0x1.aa445fce93b82p2

  },
  { // Entry 397
    -0x1.7931cba100008p-2,
    -0x1.aa445fce93b82p2

  },
  { // Entry 398
    0x1.7af3f76c7a708p-1,
    0x1.aaa3fbc359fbep-1

  },
  { // Entry 399
    -0x1.7af3f76c7a708p-1,
    -0x1.aaa3fbc359fbep-1

  },
  { // Entry 400
    0x1.fd74e53ae32fdp-6,
    0x1.abdd3dbd4d86p119

  },
  { // Entry 401
    -0x1.fd74e53ae32fdp-6,
    -0x1.abdd3dbd4d86p119

  },
  { // Entry 402
    0x1.7d4a7bf183a34p-1,
    0x1.ae2165a0c9f8ep-1

  },
  { // Entry 403
    -0x1.7d4a7bf183a34p-1,
    -0x1.ae2165a0c9f8ep-1

  },
  { // Entry 404
    0x1.b81410edc79e1p-2,
    0x1.ae8dfefcfe13bp2

  },
  { // Entry 405
    -0x1.b81410edc79e1p-2,
    -0x1.ae8dfefcfe13bp2

  },
  { // Entry 406
    -0x1.ff751561dc50ap-2,
    0x1.b5597f950ee8cp29

  },
  { // Entry 407
    0x1.ff751561dc50ap-2,
    -0x1.b5597f950ee8cp29

  },
  { // Entry 408
    0x1.027d184afb198p-52,
    0x1.bab62ed655019p970

  },
  { // Entry 409
    -0x1.027d184afb198p-52,
    -0x1.bab62ed655019p970

  },
  { // Entry 410
    0x1.bc572e5e413e1p-10,
    0x1.bc573c4ffffffp-10

  },
  { // Entry 411
    -0x1.bc572e5e413e1p-10,
    -0x1.bc573c4ffffffp-10

  },
  { // Entry 412
    0x1.fb300f1e39afep-1,
    0x1.bef5cd25ab1adp9

  },
  { // Entry 413
    -0x1.fb300f1e39afep-1,
    -0x1.bef5cd25ab1adp9

  },
  { // Entry 414
    0x1.b1baaf622d3a3p-2,
    0x1.bfdf6df2a24c1p-2

  },
  { // Entry 415
    -0x1.b1baaf622d3a3p-2,
    -0x1.bfdf6df2a24c1p-2

  },
  { // Entry 416
    0x1.88fb762c35ce4p-1,
    0x1.bfffffdffffffp-1

  },
  { // Entry 417
    -0x1.88fb762c35ce4p-1,
    -0x1.bfffffdffffffp-1

  },
  { // Entry 418
    0x1.fe0ebff99ab8dp-1,
    0x1.c2b489520e376p920

  },
  { // Entry 419
    -0x1.fe0ebff99ab8dp-1,
    -0x1.c2b489520e376p920

  },
  { // Entry 420
    0x1.cf7f749f2a836p-4,
    0x1.c54beb008547p5

  },
  { // Entry 421
    -0x1.cf7f749f2a836p-4,
    -0x1.c54beb008547p5

  },
  { // Entry 422
    0x1.b6facf6658915p-2,
    0x1.c5ad34f5f472ap-2

  },
  { // Entry 423
    -0x1.b6facf6658915p-2,
    -0x1.c5ad34f5f472ap-2

  },
  { // Entry 424
    0x1.b851cd9b84ee7p-2,
    0x1.c728fc2f34bd6p-2

  },
  { // Entry 425
    -0x1.b851cd9b84ee7p-2,
    -0x1.c728fc2f34bd6p-2

  },
  { // Entry 426
    0x1.ba21b53cf2ff3p-2,
    0x1.c92b0f6105089p-2

  },
  { // Entry 427
    -0x1.ba21b53cf2ff3p-2,
    -0x1.c92b0f6105089p-2

  },
  { // Entry 428
    0x1.c9a2b68e30ec7p-5,
    0x1.c9dfbbe9ec704p-5

  },
  { // Entry 429
    -0x1.c9a2b68e30ec7p-5,
    -0x1.c9dfbbe9ec704p-5

  },
  { // Entry 430
    0x1.f370115c9ab35p-1,
    0x1.caf31bd7ee217p0

  },
  { // Entry 431
    -0x1.f370115c9ab35p-1,
    -0x1.caf31bd7ee217p0

  },
  { // Entry 432
    -0x1.dd38a1f1d289bp-54,
    0x1.cb44e86bc192bp648

  },
  { // Entry 433
    0x1.dd38a1f1d289bp-54,
    -0x1.cb44e86bc192bp648

  },
  { // Entry 434
    0x1.dd38a1f1d289bp-53,
    0x1.cb44e86bc192bp649

  },
  { // Entry 435
    -0x1.dd38a1f1d289bp-53,
    -0x1.cb44e86bc192bp649

  },
  { // Entry 436
    0x1.c7885aef33a95p-3,
    0x1.cb6p-3

  },
  { // Entry 437
    -0x1.c7885aef33a95p-3,
    -0x1.cb6p-3

  },
  { // Entry 438
    0x1.cd279aa6196b6p-4,
    0x1.ce2271d2f662fp-4

  },
  { // Entry 439
    -0x1.cd279aa6196b6p-4,
    -0x1.ce2271d2f662fp-4

  },
  { // Entry 440
    0x1.930b705f9fad2p-1,
    0x1.d0000000004p-1

  },
  { // Entry 441
    -0x1.930b705f9fad2p-1,
    -0x1.d0000000004p-1

  },
  { // Entry 442
    0x1.7ef24c8e67d9ap-1,
    0x1.d01p199

  },
  { // Entry 443
    -0x1.7ef24c8e67d9ap-1,
    -0x1.d01p199

  },
  { // Entry 444
    0x1.ffff124c001abp-1,
    0x1.d024ba6f953cfp1000

  },
  { // Entry 445
    -0x1.ffff124c001abp-1,
    -0x1.d024ba6f953cfp1000

  },
  { // Entry 446
    -0x1.f83a0983dd15dp-2,
    0x1.d4067c60f471ep1

  },
  { // Entry 447
    0x1.f83a0983dd15dp-2,
    -0x1.d4067c60f471ep1

  },
  { // Entry 448
    0x1.d79b9896ff555p-5,
    0x1.d7de6263bcaabp-5

  },
  { // Entry 449
    -0x1.d79b9896ff555p-5,
    -0x1.d7de6263bcaabp-5

  },
  { // Entry 450
    0x1.ed0b908a2983p-1,
    0x1.d800000002274p0

  },
  { // Entry 451
    -0x1.ed0b908a2983p-1,
    -0x1.d800000002274p0

  },
  { // Entry 452
    -0x1.f2c217cbc7dcdp-1,
    0x1.d96e058p488

  },
  { // Entry 453
    0x1.f2c217cbc7dcdp-1,
    -0x1.d96e058p488

  },
  { // Entry 454
    0x1.98dcd09337793p-1,
    0x1.d98c4c612718dp-1

  },
  { // Entry 455
    -0x1.98dcd09337793p-1,
    -0x1.d98c4c612718dp-1

  },
  { // Entry 456
    0x1.db3ba8775ca26p-5,
    0x1.db8p-5

  },
  { // Entry 457
    -0x1.db3ba8775ca26p-5,
    -0x1.db8p-5

  },
  { // Entry 458
    -0x1.9fee37697d582p-2,
    0x1.de386d6090303p200

  },
  { // Entry 459
    0x1.9fee37697d582p-2,
    -0x1.de386d6090303p200

  },
  { // Entry 460
    -0x1.5361ee6553188p-53,
    0x1.de5e5054e921bp35

  },
  { // Entry 461
    0x1.5361ee6553188p-53,
    -0x1.de5e5054e921bp35

  },
  { // Entry 462
    0x1.fec48d5e769ecp-1,
    0x1.df77ddf77ddf4p10

  },
  { // Entry 463
    -0x1.fec48d5e769ecp-1,
    -0x1.df77ddf77ddf4p10

  },
  { // Entry 464
    -0x1.2902a83d72632p-1,
    0x1.e1562b0448a86p1

  },
  { // Entry 465
    0x1.2902a83d72632p-1,
    -0x1.e1562b0448a86p1

  },
  { // Entry 466
    0x1.9e26c7bc96b69p-1,
    0x1.e2700cdc86635p-1

  },
  { // Entry 467
    -0x1.9e26c7bc96b69p-1,
    -0x1.e2700cdc86635p-1

  },
  { // Entry 468
    0x1.e6494911eedd2p-7,
    0x1.e64ddaf7bd73p-7

  },
  { // Entry 469
    -0x1.e6494911eedd2p-7,
    -0x1.e64ddaf7bd73p-7

  },
  { // Entry 470
    0x1.eb26c690bda25p-5,
    0x1.eb7239bca8afap-5

  },
  { // Entry 471
    -0x1.eb26c690bda25p-5,
    -0x1.eb7239bca8afap-5

  },
  { // Entry 472
    0x1.c73238790a4cfp-3,
    0x1.ef7b83f7bdef4p3

  },
  { // Entry 473
    -0x1.c73238790a4cfp-3,
    -0x1.ef7b83f7bdef4p3

  },
  { // Entry 474
    0x1.ed1b575acb8c8p-3,
    0x1.f20000000109bp-3

  },
  { // Entry 475
    -0x1.ed1b575acb8c8p-3,
    -0x1.f20000000109bp-3

  },
  { // Entry 476
    0x1.c1b50a56c8809p-1,
    0x1.f40ca67a9e8d7p9

  },
  { // Entry 477
    -0x1.c1b50a56c8809p-1,
    -0x1.f40ca67a9e8d7p9

  },
  { // Entry 478
    0x1.e321fea643a96p-2,
    0x1.f7224d2c7754p-2

  },
  { // Entry 479
    -0x1.e321fea643a96p-2,
    -0x1.f7224d2c7754p-2

  },
  { // Entry 480
    0x1.c1269b020a108p-3,
    0x1.f78a0d05e60e2p6

  },
  { // Entry 481
    -0x1.c1269b020a108p-3,
    -0x1.f78a0d05e60e2p6

  },
  { // Entry 482
    0x1.f76cae28a0775p-5,
    0x1.f7bdef7bdf073p-5

  },
  { // Entry 483
    -0x1.f76cae28a0775p-5,
    -0x1.f7bdef7bdf073p-5

  },
  { // Entry 484
    0x1.e42c139dc2054p-2,
    0x1.f8502d5955443p-2

  },
  { // Entry 485
    -0x1.e42c139dc2054p-2,
    -0x1.f8502d5955443p-2

  },
  { // Entry 486
    0x1.0fa749e07f64p-9,
    0x1.f8fc824d2693bp61

  },
  { // Entry 487
    -0x1.0fa749e07f64p-9,
    -0x1.f8fc824d2693bp61

  },
  { // Entry 488
    0x1.ffa80324e2d8fp-1,
    0x1.f8fffffffffffp2

  },
  { // Entry 489
    -0x1.ffa80324e2d8fp-1,
    -0x1.f8fffffffffffp2

  },
  { // Entry 490
    -0x1.7cdf79d5e37b8p-1,
    0x1.fd8p1

  },
  { // Entry 491
    0x1.7cdf79d5e37b8p-1,
    -0x1.fd8p1

  },
  { // Entry 492
    0x1.fd3f48847a1d1p-5,
    0x1.fd9364d936596p-5

  },
  { // Entry 493
    -0x1.fd3f48847a1d1p-5,
    -0x1.fd9364d936596p-5

  },
  { // Entry 494
    0x1.f93ad471d262fp-3,
    0x1.fe8p-3

  },
  { // Entry 495
    -0x1.f93ad471d262fp-3,
    -0x1.fe8p-3

  },
  { // Entry 496
    0x1.83b3062414974p-1,
    0x1.febb646e2ee57p13

  },
  { // Entry 497
    -0x1.83b3062414974p-1,
    -0x1.febb646e2ee57p13

  },
  { // Entry 498
    0x1.3b45bd7449775p-1,
    0x1.feeffffffffc6p995

  },
  { // Entry 499
    -0x1.3b45bd7449775p-1,
    -0x1.feeffffffffc6p995

  },
  { // Entry 500
    -0x1.eefb59d143646p-1,
    0x1.ff8ffffffffffp7

  },
  { // Entry 501
    0x1.eefb59d143646p-1,
    -0x1.ff8ffffffffffp7

  },
  { // Entry 502
    -0x1.56433f0c6bceep-1,
    0x1.ff8ffffffffffp870

  },
  { // Entry 503
    0x1.56433f0c6bceep-1,
    -0x1.ff8ffffffffffp870

  },
  { // Entry 504
    -0x1.930006246a6cp-2,
    0x1.ffcfff8p19

  },
  { // Entry 505
    0x1.930006246a6cp-2,
    -0x1.ffcfff8p19

  },
  { // Entry 506
    0x1.ded37a1f0aa6dp-1,
    0x1.ffcfff8p365

  },
  { // Entry 507
    -0x1.ded37a1f0aa6dp-1,
    -0x1.ffcfff8p365

  },
  { // Entry 508
    -0x1.93e4d96b621ep-1,
    0x1.ffcffffffff6cp720

  },
  { // Entry 509
    0x1.93e4d96b621ep-1,
    -0x1.ffcffffffff6cp720

  },
  { // Entry 510
    0x1.9068b90e42606p-1,
    0x1.ffcfffffffff9p320

  },
  { // Entry 511
    -0x1.9068b90e42606p-1,
    -0x1.ffcfffffffff9p320

  },
  { // Entry 512
    0x1.cf81642e7421cp-1,
    0x1.ffcffffffffffp12

  },
  { // Entry 513
    -0x1.cf81642e7421cp-1,
    -0x1.ffcffffffffffp12

  },
  { // Entry 514
    0x1.ffffffe61fe61p-1,
    0x1.ffcffffffffffp404

  },
  { // Entry 515
    -0x1.ffffffe61fe61p-1,
    -0x1.ffcffffffffffp404

  },
  { // Entry 516
    -0x1.406ee9ae91e17p-1,
    0x1.ffeffffffffccp995

  },
  { // Entry 517
    0x1.406ee9ae91e17p-1,
    -0x1.ffeffffffffccp995

  },
  { // Entry 518
    0x1.fa9f6ca0ec44ep-3,
    0x1.ffeffffffffffp-3

  },
  { // Entry 519
    -0x1.fa9f6ca0ec44ep-3,
    -0x1.ffeffffffffffp-3

  },
  { // Entry 520
    0x1.6b491db8b66d9p-4,
    0x1.ffeffffffffffp55

  },
  { // Entry 521
    -0x1.6b491db8b66d9p-4,
    -0x1.ffeffffffffffp55

  },
  { // Entry 522
    0x1.fb0ab102cb13p-1,
    0x1.ffeffffffffffp180

  },
  { // Entry 523
    -0x1.fb0ab102cb13p-1,
    -0x1.ffeffffffffffp180

  },
  { // Entry 524
    0x1.e4315ec04635dp-3,
    0x1.ffeffffffffffp706

  },
  { // Entry 525
    -0x1.e4315ec04635dp-3,
    -0x1.ffeffffffffffp706

  },
  { // Entry 526
    0x1.ffffc39997ef6p-1,
    0x1.fff1fffffffffp41

  },
  { // Entry 527
    -0x1.ffffc39997ef6p-1,
    -0x1.fff1fffffffffp41

  },
  { // Entry 528
    0x1.fff163992831fp-7,
    0x1.fff6b89ffffffp-7

  },
  { // Entry 529
    -0x1.fff163992831fp-7,
    -0x1.fff6b89ffffffp-7

  },
  { // Entry 530
    -0x1.d9757a05fcc43p-1,
    0x1.fffdffff0001fp105

  },
  { // Entry 531
    0x1.d9757a05fcc43p-1,
    -0x1.fffdffff0001fp105

  },
  { // Entry 532
    -0x1.83791fe63a17ap-1,
    0x1.ffff0c0000002p1

  },
  { // Entry 533
    0x1.83791fe63a17ap-1,
    -0x1.ffff0c0000002p1

  },
  { // Entry 534
    -0x1.d9d3a85acc50dp-1,
    0x1.ffffc00000055p150

  },
  { // Entry 535
    0x1.d9d3a85acc50dp-1,
    -0x1.ffffc00000055p150

  },
  { // Entry 536
    -0x1.f25d858dcdee7p-3,
    0x1.ffffe3fffffffp40

  },
  { // Entry 537
    0x1.f25d858dcdee7p-3,
    -0x1.ffffe3fffffffp40

  },
  { // Entry 538
    0x1.d18f7bfe557ecp-1,
    0x1.ffffefffcffaep0

  },
  { // Entry 539
    -0x1.d18f7bfe557ecp-1,
    -0x1.ffffefffcffaep0

  },
  { // Entry 540
    -0x1.bc14ebf6bfb52p-4,
    0x1.fffffbfffffffp228

  },
  { // Entry 541
    0x1.bc14ebf6bfb52p-4,
    -0x1.fffffbfffffffp228

  },
  { // Entry 542
    0x1.bb887a06f6c51p-3,
    0x1.fffffbfffffffp735

  },
  { // Entry 543
    -0x1.bb887a06f6c51p-3,
    -0x1.fffffbfffffffp735

  },
  { // Entry 544
    0x1.ffaaadef54e2fp-5,
    0x1.fffffefffffffp-5

  },
  { // Entry 545
    -0x1.ffaaadef54e2fp-5,
    -0x1.fffffefffffffp-5

  },
  { // Entry 546
    0x1.d4a3c62c5be09p-1,
    0x1.ffffff8p119

  },
  { // Entry 547
    -0x1.d4a3c62c5be09p-1,
    -0x1.ffffff8p119

  },
  { // Entry 548
    0x1.cec20f197703fp-3,
    0x1.ffffff8p192

  },
  { // Entry 549
    -0x1.cec20f197703fp-3,
    -0x1.ffffff8p192

  },
  { // Entry 550
    0x1.d37aadc7c8662p-2,
    0x1.ffffff8p543

  },
  { // Entry 551
    -0x1.d37aadc7c8662p-2,
    -0x1.ffffff8p543

  },
  { // Entry 552
    0x1.fa8d2a4d0a202p-1,
    0x1.ffffffc0018ffp2

  },
  { // Entry 553
    -0x1.fa8d2a4d0a202p-1,
    -0x1.ffffffc0018ffp2

  },
  { // Entry 554
    0x1.fa8d2a029f978p-1,
    0x1.ffffffffeffffp2

  },
  { // Entry 555
    -0x1.fa8d2a029f978p-1,
    -0x1.ffffffffeffffp2

  },
  { // Entry 556
    -0x1.2763f02a2d1eap-4,
    0x1.fffffffff825p943

  },
  { // Entry 557
    0x1.2763f02a2d1eap-4,
    -0x1.fffffffff825p943

  },
  { // Entry 558
    0x1.fcff128f77ddbp-1,
    0x1.fffffffffe09dp320

  },
  { // Entry 559
    -0x1.fcff128f77ddbp-1,
    -0x1.fffffffffe09dp320

  },
  { // Entry 560
    -0x1.fcc0bfedd84a6p-1,
    0x1.fffffffffe6e3p720

  },
  { // Entry 561
    0x1.fcc0bfedd84a6p-1,
    -0x1.fffffffffe6e3p720

  },
  { // Entry 562
    0x1.aed548f090c1ep-1,
    0x1.ffffffffffe7fp-1

  },
  { // Entry 563
    -0x1.aed548f090c1ep-1,
    -0x1.ffffffffffe7fp-1

  },
  { // Entry 564
    -0x1.f5e11def99d2bp-1,
    0x1.ffffffffffeffp250

  },
  { // Entry 565
    0x1.f5e11def99d2bp-1,
    -0x1.ffffffffffeffp250

  },
  { // Entry 566
    0x1.8a9cbf48fec9fp-1,
    0x1.fffffffffff78p920

  },
  { // Entry 567
    -0x1.8a9cbf48fec9fp-1,
    -0x1.fffffffffff78p920

  },
  { // Entry 568
    -0x1.7eba5894844ccp-3,
    0x1.fffffffffff83p150

  },
  { // Entry 569
    0x1.7eba5894844ccp-3,
    -0x1.fffffffffff83p150

  },
  { // Entry 570
    0x1.92c4f06d2cdd1p-1,
    0x1.fffffffffffd5p995

  },
  { // Entry 571
    -0x1.92c4f06d2cdd1p-1,
    -0x1.fffffffffffd5p995

  },
  { // Entry 572
    -0x1.3d5f7deb1d3bbp-1,
    0x1.fffffffffffe8p720

  },
  { // Entry 573
    0x1.3d5f7deb1d3bbp-1,
    -0x1.fffffffffffe8p720

  },
  { // Entry 574
    -0x1.91349b0ae90e5p-1,
    0x1.fffffffffffebp920

  },
  { // Entry 575
    0x1.91349b0ae90e5p-1,
    -0x1.fffffffffffebp920

  },
  { // Entry 576
    -0x1.837b9dddc1e88p-1,
    0x1.ffffffffffff1p1

  },
  { // Entry 577
    0x1.837b9dddc1e88p-1,
    -0x1.ffffffffffff1p1

  },
  { // Entry 578
    0x1.510e062e7fa2p-1,
    0x1.ffffffffffff1p245

  },
  { // Entry 579
    -0x1.510e062e7fa2p-1,
    -0x1.ffffffffffff1p245

  },
  { // Entry 580
    0x1.eaee8744b05e5p-2,
    0x1.ffffffffffff3p-2

  },
  { // Entry 581
    -0x1.eaee8744b05e5p-2,
    -0x1.ffffffffffff3p-2

  },
  { // Entry 582
    0x1.8a4dee8f40628p-1,
    0x1.ffffffffffff4p845

  },
  { // Entry 583
    -0x1.8a4dee8f40628p-1,
    -0x1.ffffffffffff4p845

  },
  { // Entry 584
    0x1.5118d6bbde07ep-1,
    0x1.ffffffffffff4p1020

  },
  { // Entry 585
    -0x1.5118d6bbde07ep-1,
    -0x1.ffffffffffff4p1020

  },
  { // Entry 586
    -0x1.5cd5c53cf30a9p-1,
    0x1.ffffffffffff8p616

  },
  { // Entry 587
    0x1.5cd5c53cf30a9p-1,
    -0x1.ffffffffffff8p616

  },
  { // Entry 588
    0x1.ffffa1f0d7dafp-1,
    0x1.ffffffffffffcp475

  },
  { // Entry 589
    -0x1.ffffa1f0d7dafp-1,
    -0x1.ffffffffffffcp475

  },
  { // Entry 590
    0x1.51e9d840106d7p-1,
    0x1.ffffffffffffep970

  },
  { // Entry 591
    -0x1.51e9d840106d7p-1,
    -0x1.ffffffffffffep970

  },
  { // Entry 592
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 593
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 594
    -0x0.0p0,
    -0x0.0p0

  },
  { // Entry 595
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 596
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 597
    -0x1.0000000000001p-1022,
    -0x1.0000000000001p-1022

  },
  { // Entry 598
    0x1.0000000000001p-1022,
    0x1.0000000000001p-1022

  },
  { // Entry 599
    -0x1.0p-1022,
    -0x1.0p-1022

  },
  { // Entry 600
    0x1.0p-1022,
    0x1.0p-1022

  },
  { // Entry 601
    -0x0.fffffffffffffp-1022,
    -0x0.fffffffffffffp-1022

  },
  { // Entry 602
    0x0.fffffffffffffp-1022,
    0x0.fffffffffffffp-1022

  },
  { // Entry 603
    0x0.fffffffffffffp-1022,
    0x0.fffffffffffffp-1022

  },
  { // Entry 604
    -0x0.fffffffffffffp-1022,
    -0x0.fffffffffffffp-1022

  },
  { // Entry 605
    0x1.0p-1022,
    0x1.0p-1022

  },
  { // Entry 606
    -0x1.0p-1022,
    -0x1.0p-1022

  },
  { // Entry 607
    0x1.0000000000001p-1022,
    0x1.0000000000001p-1022

  },
  { // Entry 608
    -0x1.0000000000001p-1022,
    -0x1.0000000000001p-1022

  },
  { // Entry 609
    0x1.9999996de8ca2p-13,
    0x1.999999999999ap-13

  },
  { // Entry 610
    -0x1.9999996de8ca2p-13,
    -0x1.999999999999ap-13

  },
  { // Entry 611
    0x1.999998ead65b9p-12,
    0x1.999999999999ap-12

  },
  { // Entry 612
    -0x1.999998ead65b9p-12,
    -0x1.999999999999ap-12

  },
  { // Entry 613
    0x1.3333320c49bacp-11,
    0x1.3333333333334p-11

  },
  { // Entry 614
    -0x1.3333320c49bacp-11,
    -0x1.3333333333334p-11

  },
  { // Entry 615
    0x1.999996de8ca29p-11,
    0x1.999999999999ap-11

  },
  { // Entry 616
    -0x1.999996de8ca29p-11,
    -0x1.999999999999ap-11

  },
  { // Entry 617
    0x1.fffffaaaaaaefp-11,
    0x1.0p-10

  },
  { // Entry 618
    -0x1.fffffaaaaaaefp-11,
    -0x1.0p-10

  },
  { // Entry 619
    0x1.33332e978d553p-10,
    0x1.3333333333333p-10

  },
  { // Entry 620
    -0x1.33332e978d553p-10,
    -0x1.3333333333333p-10

  },
  { // Entry 621
    0x1.66665f1529bp-10,
    0x1.6666666666666p-10

  },
  { // Entry 622
    -0x1.66665f1529bp-10,
    -0x1.6666666666666p-10

  },
  { // Entry 623
    0x1.99998ead65cep-10,
    0x1.9999999999999p-10

  },
  { // Entry 624
    -0x1.99998ead65cep-10,
    -0x1.9999999999999p-10

  },
  { // Entry 625
    0x1.ccccbd3f7d15dp-10,
    0x1.cccccccccccccp-10

  },
  { // Entry 626
    -0x1.ccccbd3f7d15dp-10,
    -0x1.cccccccccccccp-10

  },
  { // Entry 627
    0x1.0665ae9c7b44fp-7,
    0x1.0666666666666p-7

  },
  { // Entry 628
    -0x1.0665ae9c7b44fp-7,
    -0x1.0666666666666p-7

  },
  { // Entry 629
    0x1.ccc8e97b59f62p-7,
    0x1.cccccccccccccp-7

  },
  { // Entry 630
    -0x1.ccc8e97b59f62p-7,
    -0x1.cccccccccccccp-7

  },
  { // Entry 631
    0x1.4993e8a8ff79bp-6,
    0x1.4999999999999p-6

  },
  { // Entry 632
    -0x1.4993e8a8ff79bp-6,
    -0x1.4999999999999p-6

  },
  { // Entry 633
    0x1.acc044c56db0ep-6,
    0x1.accccccccccccp-6

  },
  { // Entry 634
    -0x1.acc044c56db0ep-6,
    -0x1.accccccccccccp-6

  },
  { // Entry 635
    0x1.07f44d67cf41bp-5,
    0x1.08p-5

  },
  { // Entry 636
    -0x1.07f44d67cf41bp-5,
    -0x1.08p-5

  },
  { // Entry 637
    0x1.3985fe46f1c87p-5,
    0x1.399999999999ap-5

  },
  { // Entry 638
    -0x1.3985fe46f1c87p-5,
    -0x1.399999999999ap-5

  },
  { // Entry 639
    0x1.6b14bde93ac5fp-5,
    0x1.6b33333333334p-5

  },
  { // Entry 640
    -0x1.6b14bde93ac5fp-5,
    -0x1.6b33333333334p-5

  },
  { // Entry 641
    0x1.9ca0153ed8397p-5,
    0x1.9cccccccccccep-5

  },
  { // Entry 642
    -0x1.9ca0153ed8397p-5,
    -0x1.9cccccccccccep-5

  },
  { // Entry 643
    0x1.ce278d4027d34p-5,
    0x1.ce66666666666p-5

  },
  { // Entry 644
    -0x1.ce278d4027d34p-5,
    -0x1.ce66666666666p-5

  },
  { // Entry 645
    0x1.43c1e9c171a66p-1,
    0x1.5e7fc4369bdadp-1

  },
  { // Entry 646
    -0x1.43c1e9c171a66p-1,
    -0x1.5e7fc4369bdadp-1

  },
  { // Entry 647
    0x1.ee3d6bcea09cap-1,
    0x1.4e7fc4369bdadp0

  },
  { // Entry 648
    -0x1.ee3d6bcea09cap-1,
    -0x1.4e7fc4369bdadp0

  },
  { // Entry 649
    0x1.df8e22ea809d6p-1,
    0x1.edbfa651e9c84p0

  },
  { // Entry 650
    -0x1.df8e22ea809d6p-1,
    -0x1.edbfa651e9c84p0

  },
  { // Entry 651
    0x1.1d3479eac7ae3p-1,
    0x1.467fc4369bdadp1

  },
  { // Entry 652
    -0x1.1d3479eac7ae3p-1,
    -0x1.467fc4369bdadp1

  },
  { // Entry 653
    -0x1.ffeaaaeeee84bp-6,
    0x1.961fb54442d18p1

  },
  { // Entry 654
    0x1.ffeaaaeeee84bp-6,
    -0x1.961fb54442d18p1

  },
  { // Entry 655
    -0x1.3734d32d49bd1p-1,
    0x1.e5bfa651e9c83p1

  },
  { // Entry 656
    0x1.3734d32d49bd1p-1,
    -0x1.e5bfa651e9c83p1

  },
  { // Entry 657
    -0x1.e9d25d19911e2p-1,
    0x1.1aafcbafc85f7p2

  },
  { // Entry 658
    0x1.e9d25d19911e2p-1,
    -0x1.1aafcbafc85f7p2

  },
  { // Entry 659
    -0x1.e4ecdc5a4e466p-1,
    0x1.427fc4369bdadp2

  },
  { // Entry 660
    0x1.e4ecdc5a4e466p-1,
    -0x1.427fc4369bdadp2

  },
  { // Entry 661
    -0x1.2a59f10344262p-1,
    0x1.6a4fbcbd6f562p2

  },
  { // Entry 662
    0x1.2a59f10344262p-1,
    -0x1.6a4fbcbd6f562p2

  },
  { // Entry 663
    -0x1.26312443bd35fp-1,
    0x1.6af2eff0a2896p2

  },
  { // Entry 664
    0x1.26312443bd35fp-1,
    -0x1.6af2eff0a2896p2

  },
  { // Entry 665
    -0x1.e18e660a5e2fbp-1,
    0x1.43c62a9d02414p2

  },
  { // Entry 666
    0x1.e18e660a5e2fbp-1,
    -0x1.43c62a9d02414p2

  },
  { // Entry 667
    -0x1.ee0e83a0198b7p-1,
    0x1.1c99654961f92p2

  },
  { // Entry 668
    0x1.ee0e83a0198b7p-1,
    -0x1.1c99654961f92p2

  },
  { // Entry 669
    -0x1.4727747338e46p-1,
    0x1.ead93feb8361fp1

  },
  { // Entry 670
    0x1.4727747338e46p-1,
    -0x1.ead93feb8361fp1

  },
  { // Entry 671
    -0x1.4ba2f75dda5fep-4,
    0x1.9c7fb54442d1ap1

  },
  { // Entry 672
    0x1.4ba2f75dda5fep-4,
    -0x1.9c7fb54442d1ap1

  },
  { // Entry 673
    0x1.034c4d633b4efp-1,
    0x1.4e262a9d02415p1

  },
  { // Entry 674
    -0x1.034c4d633b4efp-1,
    -0x1.4e262a9d02415p1

  },
  { // Entry 675
    0x1.d1e4cde2f3945p-1,
    0x1.ff993feb8362p0

  },
  { // Entry 676
    -0x1.d1e4cde2f3945p-1,
    -0x1.ff993feb8362p0

  },
  { // Entry 677
    0x1.f750235c94992p-1,
    0x1.62e62a9d02416p0

  },
  { // Entry 678
    -0x1.f750235c94992p-1,
    -0x1.62e62a9d02416p0

  },
  { // Entry 679
    0x1.65f7d571279b1p-1,
    0x1.8c662a9d02419p-1

  },
  { // Entry 680
    -0x1.65f7d571279b1p-1,
    -0x1.8c662a9d02419p-1

  },
  { // Entry 681
    -0x1.fe043f57369d7p-1,
    -0x1.a8aa1d11c44ffp0

  },
  { // Entry 682
    0x1.fe043f57369d7p-1,
    0x1.a8aa1d11c44ffp0

  },
  { // Entry 683
    -0x1.fff18f24f3e4cp-1,
    -0x1.95ec8b9e03d54p0

  },
  { // Entry 684
    0x1.fff18f24f3e4cp-1,
    0x1.95ec8b9e03d54p0

  },
  { // Entry 685
    -0x1.ff20d961624e7p-1,
    -0x1.832efa2a435a9p0

  },
  { // Entry 686
    0x1.ff20d961624e7p-1,
    0x1.832efa2a435a9p0

  },
  { // Entry 687
    -0x1.fb933c40107fdp-1,
    -0x1.707168b682dfep0

  },
  { // Entry 688
    0x1.fb933c40107fdp-1,
    0x1.707168b682dfep0

  },
  { // Entry 689
    -0x1.f54d971881ad7p-1,
    -0x1.5db3d742c2653p0

  },
  { // Entry 690
    0x1.f54d971881ad7p-1,
    0x1.5db3d742c2653p0

  },
  { // Entry 691
    -0x1.ec5883b7b6cf5p-1,
    -0x1.4af645cf01ea8p0

  },
  { // Entry 692
    0x1.ec5883b7b6cf5p-1,
    0x1.4af645cf01ea8p0

  },
  { // Entry 693
    -0x1.e0c04a94e1731p-1,
    -0x1.3838b45b416fdp0

  },
  { // Entry 694
    0x1.e0c04a94e1731p-1,
    0x1.3838b45b416fdp0

  },
  { // Entry 695
    -0x1.d294d1f96c7ecp-1,
    -0x1.257b22e780f52p0

  },
  { // Entry 696
    0x1.d294d1f96c7ecp-1,
    0x1.257b22e780f52p0

  },
  { // Entry 697
    -0x1.c1e9883373d7fp-1,
    -0x1.12bd9173c07abp0

  },
  { // Entry 698
    0x1.c1e9883373d7fp-1,
    0x1.12bd9173c07abp0

  },
  { // Entry 699
    -0x1.a2c289d9d055bp-1,
    -0x1.ea5c3ed5b385p-1

  },
  { // Entry 700
    0x1.a2c289d9d055bp-1,
    0x1.ea5c3ed5b385p-1

  },
  { // Entry 701
    -0x1.95f05257dbcb6p-1,
    -0x1.d4b87dab670ap-1

  },
  { // Entry 702
    0x1.95f05257dbcb6p-1,
    0x1.d4b87dab670ap-1

  },
  { // Entry 703
    -0x1.88647f26a6e0fp-1,
    -0x1.bf14bc811a8fp-1

  },
  { // Entry 704
    0x1.88647f26a6e0fp-1,
    0x1.bf14bc811a8fp-1

  },
  { // Entry 705
    -0x1.7a2541dfd4e75p-1,
    -0x1.a970fb56ce14p-1

  },
  { // Entry 706
    0x1.7a2541dfd4e75p-1,
    0x1.a970fb56ce14p-1

  },
  { // Entry 707
    -0x1.6b391e25bc26dp-1,
    -0x1.93cd3a2c8199p-1

  },
  { // Entry 708
    0x1.6b391e25bc26dp-1,
    0x1.93cd3a2c8199p-1

  },
  { // Entry 709
    -0x1.5ba6e6a8e7065p-1,
    -0x1.7e297902351ep-1

  },
  { // Entry 710
    0x1.5ba6e6a8e7065p-1,
    0x1.7e297902351ep-1

  },
  { // Entry 711
    -0x1.4b75ba096fa55p-1,
    -0x1.6885b7d7e8a3p-1

  },
  { // Entry 712
    0x1.4b75ba096fa55p-1,
    0x1.6885b7d7e8a3p-1

  },
  { // Entry 713
    -0x1.3aacff95a3123p-1,
    -0x1.52e1f6ad9c28p-1

  },
  { // Entry 714
    0x1.3aacff95a3123p-1,
    0x1.52e1f6ad9c28p-1

  },
  { // Entry 715
    -0x1.295463e769285p-1,
    -0x1.3d3e35834fadp-1

  },
  { // Entry 716
    0x1.295463e769285p-1,
    0x1.3d3e35834fadp-1

  },
  { // Entry 717
    -0x1.fc769b77e5885p-2,
    -0x1.0a0b02501c799p-1

  },
  { // Entry 718
    0x1.fc769b77e5885p-2,
    0x1.0a0b02501c799p-1

  },
  { // Entry 719
    -0x1.c853c78462de4p-2,
    -0x1.d8f7208e6b82cp-2

  },
  { // Entry 720
    0x1.c853c78462de4p-2,
    0x1.d8f7208e6b82cp-2

  },
  { // Entry 721
    -0x1.92aba90aaf272p-2,
    -0x1.9dd83c7c9e126p-2

  },
  { // Entry 722
    0x1.92aba90aaf272p-2,
    0x1.9dd83c7c9e126p-2

  },
  { // Entry 723
    -0x1.5bac064658f39p-2,
    -0x1.62b9586ad0a2p-2

  },
  { // Entry 724
    0x1.5bac064658f39p-2,
    0x1.62b9586ad0a2p-2

  },
  { // Entry 725
    -0x1.2383ca8078e58p-2,
    -0x1.279a74590331ap-2

  },
  { // Entry 726
    0x1.2383ca8078e58p-2,
    0x1.279a74590331ap-2

  },
  { // Entry 727
    -0x1.d4c5bc11d2372p-3,
    -0x1.d8f7208e6b829p-3

  },
  { // Entry 728
    0x1.d4c5bc11d2372p-3,
    0x1.d8f7208e6b829p-3

  },
  { // Entry 729
    -0x1.60f3faaf43024p-3,
    -0x1.62b9586ad0a1ep-3

  },
  { // Entry 730
    0x1.60f3faaf43024p-3,
    0x1.62b9586ad0a1ep-3

  },
  { // Entry 731
    -0x1.d7ea3de45a9d6p-4,
    -0x1.d8f7208e6b826p-4

  },
  { // Entry 732
    0x1.d7ea3de45a9d6p-4,
    0x1.d8f7208e6b826p-4

  },
  { // Entry 733
    -0x1.d8b3df489987ap-5,
    -0x1.d8f7208e6b82dp-5

  },
  { // Entry 734
    0x1.d8b3df489987ap-5,
    0x1.d8f7208e6b82dp-5

  },
  { // Entry 735
    0x1.d8b3df489987ap-5,
    0x1.d8f7208e6b82dp-5

  },
  { // Entry 736
    -0x1.d8b3df489987ap-5,
    -0x1.d8f7208e6b82dp-5

  },
  { // Entry 737
    0x1.d7ea3de45a9ddp-4,
    0x1.d8f7208e6b82dp-4

  },
  { // Entry 738
    -0x1.d7ea3de45a9ddp-4,
    -0x1.d8f7208e6b82dp-4

  },
  { // Entry 739
    0x1.60f3faaf43028p-3,
    0x1.62b9586ad0a22p-3

  },
  { // Entry 740
    -0x1.60f3faaf43028p-3,
    -0x1.62b9586ad0a22p-3

  },
  { // Entry 741
    0x1.d4c5bc11d2376p-3,
    0x1.d8f7208e6b82dp-3

  },
  { // Entry 742
    -0x1.d4c5bc11d2376p-3,
    -0x1.d8f7208e6b82dp-3

  },
  { // Entry 743
    0x1.2383ca8078e5ap-2,
    0x1.279a74590331cp-2

  },
  { // Entry 744
    -0x1.2383ca8078e5ap-2,
    -0x1.279a74590331cp-2

  },
  { // Entry 745
    0x1.5bac064658f3bp-2,
    0x1.62b9586ad0a22p-2

  },
  { // Entry 746
    -0x1.5bac064658f3bp-2,
    -0x1.62b9586ad0a22p-2

  },
  { // Entry 747
    0x1.92aba90aaf274p-2,
    0x1.9dd83c7c9e128p-2

  },
  { // Entry 748
    -0x1.92aba90aaf274p-2,
    -0x1.9dd83c7c9e128p-2

  },
  { // Entry 749
    0x1.c853c78462de6p-2,
    0x1.d8f7208e6b82ep-2

  },
  { // Entry 750
    -0x1.c853c78462de6p-2,
    -0x1.d8f7208e6b82ep-2

  },
  { // Entry 751
    0x1.fc769b77e5885p-2,
    0x1.0a0b02501c799p-1

  },
  { // Entry 752
    -0x1.fc769b77e5885p-2,
    -0x1.0a0b02501c799p-1

  },
  { // Entry 753
    0x1.295463e769281p-1,
    0x1.3d3e35834faccp-1

  },
  { // Entry 754
    -0x1.295463e769281p-1,
    -0x1.3d3e35834faccp-1

  },
  { // Entry 755
    0x1.3aacff95a312p-1,
    0x1.52e1f6ad9c27cp-1

  },
  { // Entry 756
    -0x1.3aacff95a312p-1,
    -0x1.52e1f6ad9c27cp-1

  },
  { // Entry 757
    0x1.4b75ba096fa52p-1,
    0x1.6885b7d7e8a2cp-1

  },
  { // Entry 758
    -0x1.4b75ba096fa52p-1,
    -0x1.6885b7d7e8a2cp-1

  },
  { // Entry 759
    0x1.5ba6e6a8e7062p-1,
    0x1.7e297902351dcp-1

  },
  { // Entry 760
    -0x1.5ba6e6a8e7062p-1,
    -0x1.7e297902351dcp-1

  },
  { // Entry 761
    0x1.6b391e25bc26ap-1,
    0x1.93cd3a2c8198cp-1

  },
  { // Entry 762
    -0x1.6b391e25bc26ap-1,
    -0x1.93cd3a2c8198cp-1

  },
  { // Entry 763
    0x1.7a2541dfd4e73p-1,
    0x1.a970fb56ce13cp-1

  },
  { // Entry 764
    -0x1.7a2541dfd4e73p-1,
    -0x1.a970fb56ce13cp-1

  },
  { // Entry 765
    0x1.88647f26a6e0dp-1,
    0x1.bf14bc811a8ecp-1

  },
  { // Entry 766
    -0x1.88647f26a6e0dp-1,
    -0x1.bf14bc811a8ecp-1

  },
  { // Entry 767
    0x1.95f05257dbcb4p-1,
    0x1.d4b87dab6709cp-1

  },
  { // Entry 768
    -0x1.95f05257dbcb4p-1,
    -0x1.d4b87dab6709cp-1

  },
  { // Entry 769
    0x1.a2c289d9d0558p-1,
    0x1.ea5c3ed5b384cp-1

  },
  { // Entry 770
    -0x1.a2c289d9d0558p-1,
    -0x1.ea5c3ed5b384cp-1

  },
  { // Entry 771
    0x1.c1e9883373d7fp-1,
    0x1.12bd9173c07abp0

  },
  { // Entry 772
    -0x1.c1e9883373d7fp-1,
    -0x1.12bd9173c07abp0

  },
  { // Entry 773
    0x1.d294d1f96c7efp-1,
    0x1.257b22e780f56p0

  },
  { // Entry 774
    -0x1.d294d1f96c7efp-1,
    -0x1.257b22e780f56p0

  },
  { // Entry 775
    0x1.e0c04a94e1733p-1,
    0x1.3838b45b41701p0

  },
  { // Entry 776
    -0x1.e0c04a94e1733p-1,
    -0x1.3838b45b41701p0

  },
  { // Entry 777
    0x1.ec5883b7b6cf7p-1,
    0x1.4af645cf01eacp0

  },
  { // Entry 778
    -0x1.ec5883b7b6cf7p-1,
    -0x1.4af645cf01eacp0

  },
  { // Entry 779
    0x1.f54d971881ad8p-1,
    0x1.5db3d742c2657p0

  },
  { // Entry 780
    -0x1.f54d971881ad8p-1,
    -0x1.5db3d742c2657p0

  },
  { // Entry 781
    0x1.fb933c40107fep-1,
    0x1.707168b682e02p0

  },
  { // Entry 782
    -0x1.fb933c40107fep-1,
    -0x1.707168b682e02p0

  },
  { // Entry 783
    0x1.ff20d961624e7p-1,
    0x1.832efa2a435adp0

  },
  { // Entry 784
    -0x1.ff20d961624e7p-1,
    -0x1.832efa2a435adp0

  },
  { // Entry 785
    0x1.fff18f24f3e4bp-1,
    0x1.95ec8b9e03d58p0

  },
  { // Entry 786
    -0x1.fff18f24f3e4bp-1,
    -0x1.95ec8b9e03d58p0

  },
  { // Entry 787
    0x1.fe043f57369d7p-1,
    0x1.a8aa1d11c44ffp0

  },
  { // Entry 788
    -0x1.fe043f57369d7p-1,
    -0x1.a8aa1d11c44ffp0

  },
  { // Entry 789
    0x1.b3d3695acc413p-1,
    0x1.04aff6d330942p0

  },
  { // Entry 790
    -0x1.b3d3695acc413p-1,
    -0x1.04aff6d330942p0

  },
  { // Entry 791
    0x1.b3d41972dc806p-1,
    0x1.04b09e98dcdb4p0

  },
  { // Entry 792
    -0x1.b3d41972dc806p-1,
    -0x1.04b09e98dcdb4p0

  },
  { // Entry 793
    0x1.b3d4c98a318fbp-1,
    0x1.04b1465e89226p0

  },
  { // Entry 794
    -0x1.b3d4c98a318fbp-1,
    -0x1.04b1465e89226p0

  },
  { // Entry 795
    0x1.b3d579a0cb6eep-1,
    0x1.04b1ee2435698p0

  },
  { // Entry 796
    -0x1.b3d579a0cb6eep-1,
    -0x1.04b1ee2435698p0

  },
  { // Entry 797
    0x1.b3d629b6aa1dap-1,
    0x1.04b295e9e1b0ap0

  },
  { // Entry 798
    -0x1.b3d629b6aa1dap-1,
    -0x1.04b295e9e1b0ap0

  },
  { // Entry 799
    0x1.b3d6d9cbcd9bap-1,
    0x1.04b33daf8df7cp0

  },
  { // Entry 800
    -0x1.b3d6d9cbcd9bap-1,
    -0x1.04b33daf8df7cp0

  },
  { // Entry 801
    0x1.b3d789e035e89p-1,
    0x1.04b3e5753a3eep0

  },
  { // Entry 802
    -0x1.b3d789e035e89p-1,
    -0x1.04b3e5753a3eep0

  },
  { // Entry 803
    0x1.b3d839f3e3043p-1,
    0x1.04b48d3ae686p0

  },
  { // Entry 804
    -0x1.b3d839f3e3043p-1,
    -0x1.04b48d3ae686p0

  },
  { // Entry 805
    0x1.b3d8ea06d4eep-1,
    0x1.04b5350092ccfp0

  },
  { // Entry 806
    -0x1.b3d8ea06d4eep-1,
    -0x1.04b5350092ccfp0

  },
  { // Entry 807
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 808
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 809
    -0x0.0p0,
    -0x0.0p0

  },
  { // Entry 810
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 811
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 812
    0x1.1773d561fd506p-1,
    0x1.279a74590331bp-1

  },
  { // Entry 813
    -0x1.1773d561fd506p-1,
    -0x1.279a74590331bp-1

  },
  { // Entry 814
    0x1.1773d561fd507p-1,
    0x1.279a74590331cp-1

  },
  { // Entry 815
    -0x1.1773d561fd507p-1,
    -0x1.279a74590331cp-1

  },
  { // Entry 816
    0x1.1773d561fd508p-1,
    0x1.279a74590331dp-1

  },
  { // Entry 817
    -0x1.1773d561fd508p-1,
    -0x1.279a74590331dp-1

  },
  { // Entry 818
    0x1.f95b8e7107419p-1,
    0x1.bb67ae8584ca9p0

  },
  { // Entry 819
    -0x1.f95b8e7107419p-1,
    -0x1.bb67ae8584ca9p0

  },
  { // Entry 820
    0x1.f95b8e7107418p-1,
    0x1.bb67ae8584caap0

  },
  { // Entry 821
    -0x1.f95b8e7107418p-1,
    -0x1.bb67ae8584caap0

  },
  { // Entry 822
    0x1.f95b8e7107418p-1,
    0x1.bb67ae8584cabp0

  },
  { // Entry 823
    -0x1.f95b8e7107418p-1,
    -0x1.bb67ae8584cabp0

  },
  { // Entry 824
    0x1.b1d8305321616p-2,
    0x1.bffffffffffffp-2

  },
  { // Entry 825
    -0x1.b1d8305321616p-2,
    -0x1.bffffffffffffp-2

  },
  { // Entry 826
    0x1.b1d8305321617p-2,
    0x1.cp-2

  },
  { // Entry 827
    -0x1.b1d8305321617p-2,
    -0x1.cp-2

  },
  { // Entry 828
    0x1.b1d8305321617p-2,
    0x1.c000000000001p-2

  },
  { // Entry 829
    -0x1.b1d8305321617p-2,
    -0x1.c000000000001p-2

  },
  { // Entry 830
    0x1.44eb381cf386ap-1,
    0x1.5ffffffffffffp-1

  },
  { // Entry 831
    -0x1.44eb381cf386ap-1,
    -0x1.5ffffffffffffp-1

  },
  { // Entry 832
    0x1.44eb381cf386bp-1,
    0x1.6p-1

  },
  { // Entry 833
    -0x1.44eb381cf386bp-1,
    -0x1.6p-1

  },
  { // Entry 834
    0x1.44eb381cf386cp-1,
    0x1.6000000000001p-1

  },
  { // Entry 835
    -0x1.44eb381cf386cp-1,
    -0x1.6000000000001p-1

  },
  { // Entry 836
    0x1.dad902fa8ac86p-1,
    0x1.2ffffffffffffp0

  },
  { // Entry 837
    -0x1.dad902fa8ac86p-1,
    -0x1.2ffffffffffffp0

  },
  { // Entry 838
    0x1.dad902fa8ac87p-1,
    0x1.3p0

  },
  { // Entry 839
    -0x1.dad902fa8ac87p-1,
    -0x1.3p0

  },
  { // Entry 840
    0x1.dad902fa8ac88p-1,
    0x1.3000000000001p0

  },
  { // Entry 841
    -0x1.dad902fa8ac88p-1,
    -0x1.3000000000001p0

  },
  { // Entry 842
    0x1.4b707a7acdedp-1,
    0x1.37fffffffffffp1

  },
  { // Entry 843
    -0x1.4b707a7acdedp-1,
    -0x1.37fffffffffffp1

  },
  { // Entry 844
    0x1.4b707a7acdecdp-1,
    0x1.38p1

  },
  { // Entry 845
    -0x1.4b707a7acdecdp-1,
    -0x1.38p1

  },
  { // Entry 846
    0x1.4b707a7acdecap-1,
    0x1.3800000000001p1

  },
  { // Entry 847
    -0x1.4b707a7acdecap-1,
    -0x1.3800000000001p1

  },
  { // Entry 848
    0x1.066e7eb76f5c6p-4,
    0x1.069c8b46b3792p-4

  },
  { // Entry 849
    -0x1.066e7eb76f5c6p-4,
    -0x1.069c8b46b3792p-4

  },
  { // Entry 850
    0x1.05e4761ab8d8fp-3,
    0x1.069c8b46b3792p-3

  },
  { // Entry 851
    -0x1.05e4761ab8d8fp-3,
    -0x1.069c8b46b3792p-3

  },
  { // Entry 852
    0x1.877e2cd4f6fdap-3,
    0x1.89ead0ea0d35bp-3

  },
  { // Entry 853
    -0x1.877e2cd4f6fdap-3,
    -0x1.89ead0ea0d35bp-3

  },
  { // Entry 854
    0x1.03be06f97cbeep-2,
    0x1.069c8b46b3792p-2

  },
  { // Entry 855
    -0x1.03be06f97cbeep-2,
    -0x1.069c8b46b3792p-2

  },
  { // Entry 856
    0x1.42abba8c72fbcp-2,
    0x1.4843ae1860576p-2

  },
  { // Entry 857
    -0x1.42abba8c72fbcp-2,
    -0x1.4843ae1860576p-2

  },
  { // Entry 858
    0x1.8045fe64e62dcp-2,
    0x1.89ead0ea0d35ap-2

  },
  { // Entry 859
    -0x1.8045fe64e62dcp-2,
    -0x1.89ead0ea0d35ap-2

  },
  { // Entry 860
    0x1.bc4c04d71abbfp-2,
    0x1.cb91f3bbba13ep-2

  },
  { // Entry 861
    -0x1.bc4c04d71abbfp-2,
    -0x1.cb91f3bbba13ep-2

  },
  { // Entry 862
    0x1.f67ea975b86ap-2,
    0x1.069c8b46b3791p-1

  },
  { // Entry 863
    -0x1.f67ea975b86ap-2,
    -0x1.069c8b46b3791p-1

  },
  { // Entry 864
    0x1.175059bf0d425p-1,
    0x1.27701caf89e83p-1

  },
  { // Entry 865
    -0x1.175059bf0d425p-1,
    -0x1.27701caf89e83p-1

  },
  { // Entry 866
    0x1.323b8b1fb4ba2p-1,
    0x1.4843ae1860575p-1

  },
  { // Entry 867
    -0x1.323b8b1fb4ba2p-1,
    -0x1.4843ae1860575p-1

  },
  { // Entry 868
    0x1.4be4979c5efb3p-1,
    0x1.69173f8136c67p-1

  },
  { // Entry 869
    -0x1.4be4979c5efb3p-1,
    -0x1.69173f8136c67p-1

  },
  { // Entry 870
    0x1.643080d67acc1p-1,
    0x1.89ead0ea0d359p-1

  },
  { // Entry 871
    -0x1.643080d67acc1p-1,
    -0x1.89ead0ea0d359p-1

  },
  { // Entry 872
    0x1.7b05b7b6c612ep-1,
    0x1.aabe6252e3a4bp-1

  },
  { // Entry 873
    -0x1.7b05b7b6c612ep-1,
    -0x1.aabe6252e3a4bp-1

  },
  { // Entry 874
    0x1.904c37505de49p-1,
    0x1.cb91f3bbba13dp-1

  },
  { // Entry 875
    -0x1.904c37505de49p-1,
    -0x1.cb91f3bbba13dp-1

  },
  { // Entry 876
    0x1.a3ed9e252938ap-1,
    0x1.ec6585249082fp-1

  },
  { // Entry 877
    -0x1.a3ed9e252938ap-1,
    -0x1.ec6585249082fp-1

  },
  { // Entry 878
    0x1.b5d545b109bf9p-1,
    0x1.069c8b46b3791p0

  },
  { // Entry 879
    -0x1.b5d545b109bf9p-1,
    -0x1.069c8b46b3791p0

  },
  { // Entry 880
    0x1.c5f058230e7fdp-1,
    0x1.170653fb1eb0ap0

  },
  { // Entry 881
    -0x1.c5f058230e7fdp-1,
    -0x1.170653fb1eb0ap0

  },
  { // Entry 882
    0x1.d42de42dce134p-1,
    0x1.27701caf89e83p0

  },
  { // Entry 883
    -0x1.d42de42dce134p-1,
    -0x1.27701caf89e83p0

  },
  { // Entry 884
    0x1.e07eeeda109cbp-1,
    0x1.37d9e563f51fcp0

  },
  { // Entry 885
    -0x1.e07eeeda109cbp-1,
    -0x1.37d9e563f51fcp0

  },
  { // Entry 886
    0x1.ead6834909b93p-1,
    0x1.4843ae1860575p0

  },
  { // Entry 887
    -0x1.ead6834909b93p-1,
    -0x1.4843ae1860575p0

  },
  { // Entry 888
    0x1.f329c0558e968p-1,
    0x1.58ad76cccb8eep0

  },
  { // Entry 889
    -0x1.f329c0558e968p-1,
    -0x1.58ad76cccb8eep0

  },
  { // Entry 890
    0x1.f96fe405f1ac6p-1,
    0x1.69173f8136c67p0

  },
  { // Entry 891
    -0x1.f96fe405f1ac6p-1,
    -0x1.69173f8136c67p0

  },
  { // Entry 892
    0x1.fda254c27a01fp-1,
    0x1.79810835a1fep0

  },
  { // Entry 893
    -0x1.fda254c27a01fp-1,
    -0x1.79810835a1fep0

  },
  { // Entry 894
    0x1.ffbca846c4fcap-1,
    0x1.89ead0ea0d359p0

  },
  { // Entry 895
    -0x1.ffbca846c4fcap-1,
    -0x1.89ead0ea0d359p0

  },
  { // Entry 896
    0x1.ffbca846c4fcap-1,
    0x1.9a54999e786d2p0

  },
  { // Entry 897
    -0x1.ffbca846c4fcap-1,
    -0x1.9a54999e786d2p0

  },
  { // Entry 898
    0x1.fda254c27a02p-1,
    0x1.aabe6252e3a4bp0

  },
  { // Entry 899
    -0x1.fda254c27a02p-1,
    -0x1.aabe6252e3a4bp0

  },
  { // Entry 900
    0x1.f96fe405f1ac8p-1,
    0x1.bb282b074edc4p0

  },
  { // Entry 901
    -0x1.f96fe405f1ac8p-1,
    -0x1.bb282b074edc4p0

  },
  { // Entry 902
    0x1.f329c0558e96ap-1,
    0x1.cb91f3bbba13dp0

  },
  { // Entry 903
    -0x1.f329c0558e96ap-1,
    -0x1.cb91f3bbba13dp0

  },
  { // Entry 904
    0x1.ead6834909b96p-1,
    0x1.dbfbbc70254b6p0

  },
  { // Entry 905
    -0x1.ead6834909b96p-1,
    -0x1.dbfbbc70254b6p0

  },
  { // Entry 906
    0x1.e07eeeda109cfp-1,
    0x1.ec6585249082fp0

  },
  { // Entry 907
    -0x1.e07eeeda109cfp-1,
    -0x1.ec6585249082fp0

  },
  { // Entry 908
    0x1.d42de42dce139p-1,
    0x1.fccf4dd8fbba8p0

  },
  { // Entry 909
    -0x1.d42de42dce139p-1,
    -0x1.fccf4dd8fbba8p0

  },
  { // Entry 910
    0x1.c5f058230e801p-1,
    0x1.069c8b46b3791p1

  },
  { // Entry 911
    -0x1.c5f058230e801p-1,
    -0x1.069c8b46b3791p1

  },
  { // Entry 912
    0x1.b5d545b109bfdp-1,
    0x1.0ed16fa0e914ep1

  },
  { // Entry 913
    -0x1.b5d545b109bfdp-1,
    -0x1.0ed16fa0e914ep1

  },
  { // Entry 914
    0x1.a3ed9e252938dp-1,
    0x1.170653fb1eb0bp1

  },
  { // Entry 915
    -0x1.a3ed9e252938dp-1,
    -0x1.170653fb1eb0bp1

  },
  { // Entry 916
    0x1.904c37505de4cp-1,
    0x1.1f3b3855544c8p1

  },
  { // Entry 917
    -0x1.904c37505de4cp-1,
    -0x1.1f3b3855544c8p1

  },
  { // Entry 918
    0x1.7b05b7b6c613p-1,
    0x1.27701caf89e85p1

  },
  { // Entry 919
    -0x1.7b05b7b6c613p-1,
    -0x1.27701caf89e85p1

  },
  { // Entry 920
    0x1.643080d67acc1p-1,
    0x1.2fa50109bf842p1

  },
  { // Entry 921
    -0x1.643080d67acc1p-1,
    -0x1.2fa50109bf842p1

  },
  { // Entry 922
    0x1.4be4979c5efb2p-1,
    0x1.37d9e563f51ffp1

  },
  { // Entry 923
    -0x1.4be4979c5efb2p-1,
    -0x1.37d9e563f51ffp1

  },
  { // Entry 924
    0x1.323b8b1fb4b9fp-1,
    0x1.400ec9be2abbcp1

  },
  { // Entry 925
    -0x1.323b8b1fb4b9fp-1,
    -0x1.400ec9be2abbcp1

  },
  { // Entry 926
    0x1.175059bf0d42p-1,
    0x1.4843ae1860579p1

  },
  { // Entry 927
    -0x1.175059bf0d42p-1,
    -0x1.4843ae1860579p1

  },
  { // Entry 928
    0x1.f67ea975b8692p-2,
    0x1.5078927295f36p1

  },
  { // Entry 929
    -0x1.f67ea975b8692p-2,
    -0x1.5078927295f36p1

  },
  { // Entry 930
    0x1.bc4c04d71abadp-2,
    0x1.58ad76cccb8f3p1

  },
  { // Entry 931
    -0x1.bc4c04d71abadp-2,
    -0x1.58ad76cccb8f3p1

  },
  { // Entry 932
    0x1.8045fe64e62c6p-2,
    0x1.60e25b27012bp1

  },
  { // Entry 933
    -0x1.8045fe64e62c6p-2,
    -0x1.60e25b27012bp1

  },
  { // Entry 934
    0x1.42abba8c72fa1p-2,
    0x1.69173f8136c6dp1

  },
  { // Entry 935
    -0x1.42abba8c72fa1p-2,
    -0x1.69173f8136c6dp1

  },
  { // Entry 936
    0x1.03be06f97cbdp-2,
    0x1.714c23db6c62ap1

  },
  { // Entry 937
    -0x1.03be06f97cbdp-2,
    -0x1.714c23db6c62ap1

  },
  { // Entry 938
    0x1.877e2cd4f6f94p-3,
    0x1.79810835a1fe7p1

  },
  { // Entry 939
    -0x1.877e2cd4f6f94p-3,
    -0x1.79810835a1fe7p1

  },
  { // Entry 940
    0x1.05e4761ab8d42p-3,
    0x1.81b5ec8fd79a4p1

  },
  { // Entry 941
    -0x1.05e4761ab8d42p-3,
    -0x1.81b5ec8fd79a4p1

  },
  { // Entry 942
    0x1.066e7eb76f5ddp-4,
    0x1.89ead0ea0d35bp1

  },
  { // Entry 943
    -0x1.066e7eb76f5ddp-4,
    -0x1.89ead0ea0d35bp1

  },
  { // Entry 944
    0x1.03be06f97cbf1p-2,
    -0x1.81b5ec8fd799fp2

  },
  { // Entry 945
    -0x1.03be06f97cbf1p-2,
    0x1.81b5ec8fd799fp2

  },
  { // Entry 946
    0x1.f67ea975b86a2p-2,
    -0x1.714c23db6c626p2

  },
  { // Entry 947
    -0x1.f67ea975b86a2p-2,
    0x1.714c23db6c626p2

  },
  { // Entry 948
    0x1.643080d67acc2p-1,
    -0x1.60e25b27012adp2

  },
  { // Entry 949
    -0x1.643080d67acc2p-1,
    0x1.60e25b27012adp2

  },
  { // Entry 950
    0x1.b5d545b109bf9p-1,
    -0x1.5078927295f34p2

  },
  { // Entry 951
    -0x1.b5d545b109bf9p-1,
    0x1.5078927295f34p2

  },
  { // Entry 952
    0x1.ead6834909b93p-1,
    -0x1.400ec9be2abbbp2

  },
  { // Entry 953
    -0x1.ead6834909b93p-1,
    0x1.400ec9be2abbbp2

  },
  { // Entry 954
    0x1.ffbca846c4fcap-1,
    -0x1.2fa50109bf842p2

  },
  { // Entry 955
    -0x1.ffbca846c4fcap-1,
    0x1.2fa50109bf842p2

  },
  { // Entry 956
    0x1.f329c0558e96ap-1,
    -0x1.1f3b3855544c9p2

  },
  { // Entry 957
    -0x1.f329c0558e96ap-1,
    0x1.1f3b3855544c9p2

  },
  { // Entry 958
    0x1.c5f058230e802p-1,
    -0x1.0ed16fa0e915p2

  },
  { // Entry 959
    -0x1.c5f058230e802p-1,
    0x1.0ed16fa0e915p2

  },
  { // Entry 960
    0x1.7b05b7b6c6136p-1,
    -0x1.fccf4dd8fbbaep1

  },
  { // Entry 961
    -0x1.7b05b7b6c6136p-1,
    0x1.fccf4dd8fbbaep1

  },
  { // Entry 962
    0x1.175059bf0d42fp-1,
    -0x1.dbfbbc70254bcp1

  },
  { // Entry 963
    -0x1.175059bf0d42fp-1,
    0x1.dbfbbc70254bcp1

  },
  { // Entry 964
    0x1.42abba8c72fd2p-2,
    -0x1.bb282b074edcap1

  },
  { // Entry 965
    -0x1.42abba8c72fd2p-2,
    0x1.bb282b074edcap1

  },
  { // Entry 966
    0x1.066e7eb76f62bp-4,
    -0x1.9a54999e786d8p1

  },
  { // Entry 967
    -0x1.066e7eb76f62bp-4,
    0x1.9a54999e786d8p1

  },
  { // Entry 968
    -0x1.877e2cd4f6fa4p-3,
    -0x1.79810835a1fe6p1

  },
  { // Entry 969
    0x1.877e2cd4f6fa4p-3,
    0x1.79810835a1fe6p1

  },
  { // Entry 970
    -0x1.bc4c04d71aba6p-2,
    -0x1.58ad76cccb8f4p1

  },
  { // Entry 971
    0x1.bc4c04d71aba6p-2,
    0x1.58ad76cccb8f4p1

  },
  { // Entry 972
    -0x1.4be4979c5efa8p-1,
    -0x1.37d9e563f5202p1

  },
  { // Entry 973
    0x1.4be4979c5efa8p-1,
    0x1.37d9e563f5202p1

  },
  { // Entry 974
    -0x1.a3ed9e2529382p-1,
    -0x1.170653fb1eb1p1

  },
  { // Entry 975
    0x1.a3ed9e2529382p-1,
    0x1.170653fb1eb1p1

  },
  { // Entry 976
    -0x1.e07eeeda109c6p-1,
    -0x1.ec6585249083cp0

  },
  { // Entry 977
    0x1.e07eeeda109c6p-1,
    0x1.ec6585249083cp0

  },
  { // Entry 978
    -0x1.fda254c27a01ep-1,
    -0x1.aabe6252e3a58p0

  },
  { // Entry 979
    0x1.fda254c27a01ep-1,
    0x1.aabe6252e3a58p0

  },
  { // Entry 980
    -0x1.f96fe405f1acap-1,
    -0x1.69173f8136c74p0

  },
  { // Entry 981
    0x1.f96fe405f1acap-1,
    0x1.69173f8136c74p0

  },
  { // Entry 982
    -0x1.d42de42dce13fp-1,
    -0x1.27701caf89e9p0

  },
  { // Entry 983
    0x1.d42de42dce13fp-1,
    0x1.27701caf89e9p0

  },
  { // Entry 984
    -0x1.904c37505de59p-1,
    -0x1.cb91f3bbba157p-1

  },
  { // Entry 985
    0x1.904c37505de59p-1,
    0x1.cb91f3bbba157p-1

  },
  { // Entry 986
    -0x1.323b8b1fb4bb6p-1,
    -0x1.4843ae186058ep-1

  },
  { // Entry 987
    0x1.323b8b1fb4bb6p-1,
    0x1.4843ae186058ep-1

  },
  { // Entry 988
    -0x1.8045fe64e6309p-2,
    -0x1.89ead0ea0d38ap-2

  },
  { // Entry 989
    0x1.8045fe64e6309p-2,
    0x1.89ead0ea0d38ap-2

  },
  { // Entry 990
    -0x1.05e4761ab8decp-3,
    -0x1.069c8b46b37fp-3

  },
  { // Entry 991
    0x1.05e4761ab8decp-3,
    0x1.069c8b46b37fp-3

  },
  { // Entry 992
    0x1.05e4761ab8d32p-3,
    0x1.069c8b46b3734p-3

  },
  { // Entry 993
    -0x1.05e4761ab8d32p-3,
    -0x1.069c8b46b3734p-3

  },
  { // Entry 994
    0x1.8045fe64e62b2p-2,
    0x1.89ead0ea0d32cp-2

  },
  { // Entry 995
    -0x1.8045fe64e62b2p-2,
    -0x1.89ead0ea0d32cp-2

  },
  { // Entry 996
    0x1.323b8b1fb4b9p-1,
    0x1.4843ae186055fp-1

  },
  { // Entry 997
    -0x1.323b8b1fb4b9p-1,
    -0x1.4843ae186055fp-1

  },
  { // Entry 998
    0x1.904c37505de3cp-1,
    0x1.cb91f3bbba128p-1

  },
  { // Entry 999
    -0x1.904c37505de3cp-1,
    -0x1.cb91f3bbba128p-1

  },
  { // Entry 1000
    0x1.d42de42dce12bp-1,
    0x1.27701caf89e78p0

  },
  { // Entry 1001
    -0x1.d42de42dce12bp-1,
    -0x1.27701caf89e78p0

  },
  { // Entry 1002
    0x1.f96fe405f1ac2p-1,
    0x1.69173f8136c5cp0

  },
  { // Entry 1003
    -0x1.f96fe405f1ac2p-1,
    -0x1.69173f8136c5cp0

  },
  { // Entry 1004
    0x1.fda254c27a022p-1,
    0x1.aabe6252e3a4p0

  },
  { // Entry 1005
    -0x1.fda254c27a022p-1,
    -0x1.aabe6252e3a4p0

  },
  { // Entry 1006
    0x1.e07eeeda109d7p-1,
    0x1.ec65852490824p0

  },
  { // Entry 1007
    -0x1.e07eeeda109d7p-1,
    -0x1.ec65852490824p0

  },
  { // Entry 1008
    0x1.a3ed9e252939ep-1,
    0x1.170653fb1eb04p1

  },
  { // Entry 1009
    -0x1.a3ed9e252939ep-1,
    -0x1.170653fb1eb04p1

  },
  { // Entry 1010
    0x1.4be4979c5efcdp-1,
    0x1.37d9e563f51f6p1

  },
  { // Entry 1011
    -0x1.4be4979c5efcdp-1,
    -0x1.37d9e563f51f6p1

  },
  { // Entry 1012
    0x1.bc4c04d71abfcp-2,
    0x1.58ad76cccb8e8p1

  },
  { // Entry 1013
    -0x1.bc4c04d71abfcp-2,
    -0x1.58ad76cccb8e8p1

  },
  { // Entry 1014
    0x1.877e2cd4f7061p-3,
    0x1.79810835a1fdap1

  },
  { // Entry 1015
    -0x1.877e2cd4f7061p-3,
    -0x1.79810835a1fdap1

  },
  { // Entry 1016
    -0x1.066e7eb76f4acp-4,
    0x1.9a54999e786ccp1

  },
  { // Entry 1017
    0x1.066e7eb76f4acp-4,
    -0x1.9a54999e786ccp1

  },
  { // Entry 1018
    -0x1.42abba8c72f77p-2,
    0x1.bb282b074edbep1

  },
  { // Entry 1019
    0x1.42abba8c72f77p-2,
    -0x1.bb282b074edbep1

  },
  { // Entry 1020
    -0x1.175059bf0d407p-1,
    0x1.dbfbbc70254bp1

  },
  { // Entry 1021
    0x1.175059bf0d407p-1,
    -0x1.dbfbbc70254bp1

  },
  { // Entry 1022
    -0x1.7b05b7b6c6116p-1,
    0x1.fccf4dd8fbba2p1

  },
  { // Entry 1023
    0x1.7b05b7b6c6116p-1,
    -0x1.fccf4dd8fbba2p1

  },
  { // Entry 1024
    -0x1.c5f058230e7ecp-1,
    0x1.0ed16fa0e914ap2

  },
  { // Entry 1025
    0x1.c5f058230e7ecp-1,
    -0x1.0ed16fa0e914ap2

  },
  { // Entry 1026
    -0x1.f329c0558e96p-1,
    0x1.1f3b3855544c3p2

  },
  { // Entry 1027
    0x1.f329c0558e96p-1,
    -0x1.1f3b3855544c3p2

  },
  { // Entry 1028
    -0x1.ffbca846c4fcbp-1,
    0x1.2fa50109bf83cp2

  },
  { // Entry 1029
    0x1.ffbca846c4fcbp-1,
    -0x1.2fa50109bf83cp2

  },
  { // Entry 1030
    -0x1.ead6834909ba1p-1,
    0x1.400ec9be2abb5p2

  },
  { // Entry 1031
    0x1.ead6834909ba1p-1,
    -0x1.400ec9be2abb5p2

  },
  { // Entry 1032
    -0x1.b5d545b109c12p-1,
    0x1.5078927295f2ep2

  },
  { // Entry 1033
    0x1.b5d545b109c12p-1,
    -0x1.5078927295f2ep2

  },
  { // Entry 1034
    -0x1.643080d67ace4p-1,
    0x1.60e25b27012a7p2

  },
  { // Entry 1035
    0x1.643080d67ace4p-1,
    -0x1.60e25b27012a7p2

  },
  { // Entry 1036
    -0x1.f67ea975b86f6p-2,
    0x1.714c23db6c62p2

  },
  { // Entry 1037
    0x1.f67ea975b86f6p-2,
    -0x1.714c23db6c62p2

  },
  { // Entry 1038
    -0x1.03be06f97cc4dp-2,
    0x1.81b5ec8fd7999p2

  },
  { // Entry 1039
    0x1.03be06f97cc4dp-2,
    -0x1.81b5ec8fd7999p2

  },
  { // Entry 1040
    0x1.efb26ef930c4cp-5,
    0x1.effffffffffffp-5

  },
  { // Entry 1041
    -0x1.efb26ef930c4cp-5,
    -0x1.effffffffffffp-5

  },
  { // Entry 1042
    0x1.efb26ef930c4dp-5,
    0x1.fp-5

  },
  { // Entry 1043
    -0x1.efb26ef930c4dp-5,
    -0x1.fp-5

  },
  { // Entry 1044
    0x1.efb26ef930c4ep-5,
    0x1.f000000000001p-5

  },
  { // Entry 1045
    -0x1.efb26ef930c4ep-5,
    -0x1.f000000000001p-5

  },
  { // Entry 1046
    0x1.f6baaa131de63p-4,
    0x1.f7fffffffffffp-4

  },
  { // Entry 1047
    -0x1.f6baaa131de63p-4,
    -0x1.f7fffffffffffp-4

  },
  { // Entry 1048
    0x1.f6baaa131de64p-4,
    0x1.f8p-4

  },
  { // Entry 1049
    -0x1.f6baaa131de64p-4,
    -0x1.f8p-4

  },
  { // Entry 1050
    0x1.f6baaa131de65p-4,
    0x1.f800000000001p-4

  },
  { // Entry 1051
    -0x1.f6baaa131de65p-4,
    -0x1.f800000000001p-4

  },
  { // Entry 1052
    0x1.4a8c3b4e9c7ffp-3,
    0x1.4bfffffffffffp-3

  },
  { // Entry 1053
    -0x1.4a8c3b4e9c7ffp-3,
    -0x1.4bfffffffffffp-3

  },
  { // Entry 1054
    0x1.4a8c3b4e9c8p-3,
    0x1.4cp-3

  },
  { // Entry 1055
    -0x1.4a8c3b4e9c8p-3,
    -0x1.4cp-3

  },
  { // Entry 1056
    0x1.4a8c3b4e9c801p-3,
    0x1.4c00000000001p-3

  },
  { // Entry 1057
    -0x1.4a8c3b4e9c801p-3,
    -0x1.4c00000000001p-3

  },
  { // Entry 1058
    0x1.2e9cd95baba32p-2,
    0x1.3333333333332p-2

  },
  { // Entry 1059
    -0x1.2e9cd95baba32p-2,
    -0x1.3333333333332p-2

  },
  { // Entry 1060
    0x1.2e9cd95baba33p-2,
    0x1.3333333333333p-2

  },
  { // Entry 1061
    -0x1.2e9cd95baba33p-2,
    -0x1.3333333333333p-2

  },
  { // Entry 1062
    0x1.2e9cd95baba34p-2,
    0x1.3333333333334p-2

  },
  { // Entry 1063
    -0x1.2e9cd95baba34p-2,
    -0x1.3333333333334p-2

  },
  { // Entry 1064
    0x1.3faefc7a5466fp-1,
    0x1.594317acc4ef8p-1

  },
  { // Entry 1065
    -0x1.3faefc7a5466fp-1,
    -0x1.594317acc4ef8p-1

  },
  { // Entry 1066
    0x1.3faefc7a5467p-1,
    0x1.594317acc4ef9p-1

  },
  { // Entry 1067
    -0x1.3faefc7a5467p-1,
    -0x1.594317acc4ef9p-1

  },
  { // Entry 1068
    0x1.3faefc7a5467p-1,
    0x1.594317acc4efap-1

  },
  { // Entry 1069
    -0x1.3faefc7a5467p-1,
    -0x1.594317acc4efap-1

  },
  { // Entry 1070
    0x1.6888a4e134b2ep-1,
    0x1.8ffffffffffffp-1

  },
  { // Entry 1071
    -0x1.6888a4e134b2ep-1,
    -0x1.8ffffffffffffp-1

  },
  { // Entry 1072
    0x1.6888a4e134b2fp-1,
    0x1.9p-1

  },
  { // Entry 1073
    -0x1.6888a4e134b2fp-1,
    -0x1.9p-1

  },
  { // Entry 1074
    0x1.6888a4e134b2fp-1,
    0x1.9000000000001p-1

  },
  { // Entry 1075
    -0x1.6888a4e134b2fp-1,
    -0x1.9000000000001p-1

  },
  { // Entry 1076
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 1077
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 1078
    -0x0.0p0,
    -0x0.0p0

  },
  { // Entry 1079
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 1080
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 1081
    0x1.91f65f10dd813p-5,
    0x1.921fb54442d17p-5

  },
  { // Entry 1082
    -0x1.91f65f10dd813p-5,
    -0x1.921fb54442d17p-5

  },
  { // Entry 1083
    0x1.91f65f10dd814p-5,
    0x1.921fb54442d18p-5

  },
  { // Entry 1084
    -0x1.91f65f10dd814p-5,
    -0x1.921fb54442d18p-5

  },
  { // Entry 1085
    0x1.91f65f10dd815p-5,
    0x1.921fb54442d19p-5

  },
  { // Entry 1086
    -0x1.91f65f10dd815p-5,
    -0x1.921fb54442d19p-5

  },
  { // Entry 1087
    0x1.917a6bc29b42bp-4,
    0x1.921fb54442d17p-4

  },
  { // Entry 1088
    -0x1.917a6bc29b42bp-4,
    -0x1.921fb54442d17p-4

  },
  { // Entry 1089
    0x1.917a6bc29b42cp-4,
    0x1.921fb54442d18p-4

  },
  { // Entry 1090
    -0x1.917a6bc29b42cp-4,
    -0x1.921fb54442d18p-4

  },
  { // Entry 1091
    0x1.917a6bc29b42dp-4,
    0x1.921fb54442d19p-4

  },
  { // Entry 1092
    -0x1.917a6bc29b42dp-4,
    -0x1.921fb54442d19p-4

  },
  { // Entry 1093
    0x1.8f8b83c69a609p-3,
    0x1.921fb54442d17p-3

  },
  { // Entry 1094
    -0x1.8f8b83c69a609p-3,
    -0x1.921fb54442d17p-3

  },
  { // Entry 1095
    0x1.8f8b83c69a60ap-3,
    0x1.921fb54442d18p-3

  },
  { // Entry 1096
    -0x1.8f8b83c69a60ap-3,
    -0x1.921fb54442d18p-3

  },
  { // Entry 1097
    0x1.8f8b83c69a60bp-3,
    0x1.921fb54442d19p-3

  },
  { // Entry 1098
    -0x1.8f8b83c69a60bp-3,
    -0x1.921fb54442d19p-3

  },
  { // Entry 1099
    0x1.87de2a6aea962p-2,
    0x1.921fb54442d17p-2

  },
  { // Entry 1100
    -0x1.87de2a6aea962p-2,
    -0x1.921fb54442d17p-2

  },
  { // Entry 1101
    0x1.87de2a6aea963p-2,
    0x1.921fb54442d18p-2

  },
  { // Entry 1102
    -0x1.87de2a6aea963p-2,
    -0x1.921fb54442d18p-2

  },
  { // Entry 1103
    0x1.87de2a6aea964p-2,
    0x1.921fb54442d19p-2

  },
  { // Entry 1104
    -0x1.87de2a6aea964p-2,
    -0x1.921fb54442d19p-2

  },
  { // Entry 1105
    0x1.6a09e667f3bccp-1,
    0x1.921fb54442d17p-1

  },
  { // Entry 1106
    -0x1.6a09e667f3bccp-1,
    -0x1.921fb54442d17p-1

  },
  { // Entry 1107
    0x1.6a09e667f3bccp-1,
    0x1.921fb54442d18p-1

  },
  { // Entry 1108
    -0x1.6a09e667f3bccp-1,
    -0x1.921fb54442d18p-1

  },
  { // Entry 1109
    0x1.6a09e667f3bcdp-1,
    0x1.921fb54442d19p-1

  },
  { // Entry 1110
    -0x1.6a09e667f3bcdp-1,
    -0x1.921fb54442d19p-1

  },
  { // Entry 1111
    0x1.0p0,
    0x1.921fb54442d17p0

  },
  { // Entry 1112
    -0x1.0p0,
    -0x1.921fb54442d17p0

  },
  { // Entry 1113
    0x1.0p0,
    0x1.921fb54442d18p0

  },
  { // Entry 1114
    -0x1.0p0,
    -0x1.921fb54442d18p0

  },
  { // Entry 1115
    0x1.0p0,
    0x1.921fb54442d19p0

  },
  { // Entry 1116
    -0x1.0p0,
    -0x1.921fb54442d19p0

  },
  { // Entry 1117
    0x1.469898cc51702p-51,
    0x1.921fb54442d17p1

  },
  { // Entry 1118
    -0x1.469898cc51702p-51,
    -0x1.921fb54442d17p1

  },
  { // Entry 1119
    0x1.1a62633145c07p-53,
    0x1.921fb54442d18p1

  },
  { // Entry 1120
    -0x1.1a62633145c07p-53,
    -0x1.921fb54442d18p1

  },
  { // Entry 1121
    -0x1.72cece675d1fdp-52,
    0x1.921fb54442d19p1

  },
  { // Entry 1122
    0x1.72cece675d1fdp-52,
    -0x1.921fb54442d19p1

  },
  { // Entry 1123
    -0x1.469898cc51702p-50,
    0x1.921fb54442d17p2

  },
  { // Entry 1124
    0x1.469898cc51702p-50,
    -0x1.921fb54442d17p2

  },
  { // Entry 1125
    -0x1.1a62633145c07p-52,
    0x1.921fb54442d18p2

  },
  { // Entry 1126
    0x1.1a62633145c07p-52,
    -0x1.921fb54442d18p2

  },
  { // Entry 1127
    0x1.72cece675d1fdp-51,
    0x1.921fb54442d19p2

  },
  { // Entry 1128
    -0x1.72cece675d1fdp-51,
    -0x1.921fb54442d19p2

  },
  { // Entry 1129
    -0x1.469898cc51702p-49,
    0x1.921fb54442d17p3

  },
  { // Entry 1130
    0x1.469898cc51702p-49,
    -0x1.921fb54442d17p3

  },
  { // Entry 1131
    -0x1.1a62633145c07p-51,
    0x1.921fb54442d18p3

  },
  { // Entry 1132
    0x1.1a62633145c07p-51,
    -0x1.921fb54442d18p3

  },
  { // Entry 1133
    0x1.72cece675d1fdp-50,
    0x1.921fb54442d19p3

  },
  { // Entry 1134
    -0x1.72cece675d1fdp-50,
    -0x1.921fb54442d19p3

  },
  { // Entry 1135
    -0x1.469898cc51702p-48,
    0x1.921fb54442d17p4

  },
  { // Entry 1136
    0x1.469898cc51702p-48,
    -0x1.921fb54442d17p4

  },
  { // Entry 1137
    -0x1.1a62633145c07p-50,
    0x1.921fb54442d18p4

  },
  { // Entry 1138
    0x1.1a62633145c07p-50,
    -0x1.921fb54442d18p4

  },
  { // Entry 1139
    0x1.72cece675d1fdp-49,
    0x1.921fb54442d19p4

  },
  { // Entry 1140
    -0x1.72cece675d1fdp-49,
    -0x1.921fb54442d19p4

  },
  { // Entry 1141
    -0x1.469898cc51702p-47,
    0x1.921fb54442d17p5

  },
  { // Entry 1142
    0x1.469898cc51702p-47,
    -0x1.921fb54442d17p5

  },
  { // Entry 1143
    -0x1.1a62633145c07p-49,
    0x1.921fb54442d18p5

  },
  { // Entry 1144
    0x1.1a62633145c07p-49,
    -0x1.921fb54442d18p5

  },
  { // Entry 1145
    0x1.72cece675d1fdp-48,
    0x1.921fb54442d19p5

  },
  { // Entry 1146
    -0x1.72cece675d1fdp-48,
    -0x1.921fb54442d19p5

  },
  { // Entry 1147
    -0x1.469898cc51702p-46,
    0x1.921fb54442d17p6

  },
  { // Entry 1148
    0x1.469898cc51702p-46,
    -0x1.921fb54442d17p6

  },
  { // Entry 1149
    -0x1.1a62633145c07p-48,
    0x1.921fb54442d18p6

  },
  { // Entry 1150
    0x1.1a62633145c07p-48,
    -0x1.921fb54442d18p6

  },
  { // Entry 1151
    0x1.72cece675d1fdp-47,
    0x1.921fb54442d19p6

  },
  { // Entry 1152
    -0x1.72cece675d1fdp-47,
    -0x1.921fb54442d19p6

  },
  { // Entry 1153
    -0x1.469898cc51702p-45,
    0x1.921fb54442d17p7

  },
  { // Entry 1154
    0x1.469898cc51702p-45,
    -0x1.921fb54442d17p7

  },
  { // Entry 1155
    -0x1.1a62633145c07p-47,
    0x1.921fb54442d18p7

  },
  { // Entry 1156
    0x1.1a62633145c07p-47,
    -0x1.921fb54442d18p7

  },
  { // Entry 1157
    0x1.72cece675d1fdp-46,
    0x1.921fb54442d19p7

  },
  { // Entry 1158
    -0x1.72cece675d1fdp-46,
    -0x1.921fb54442d19p7

  },
  { // Entry 1159
    0x1.6a09e667f3bdp-1,
    0x1.2d97c7f3321d1p1

  },
  { // Entry 1160
    -0x1.6a09e667f3bdp-1,
    -0x1.2d97c7f3321d1p1

  },
  { // Entry 1161
    0x1.6a09e667f3bcdp-1,
    0x1.2d97c7f3321d2p1

  },
  { // Entry 1162
    -0x1.6a09e667f3bcdp-1,
    -0x1.2d97c7f3321d2p1

  },
  { // Entry 1163
    0x1.6a09e667f3bcap-1,
    0x1.2d97c7f3321d3p1

  },
  { // Entry 1164
    -0x1.6a09e667f3bcap-1,
    -0x1.2d97c7f3321d3p1

  },
  { // Entry 1165
    -0x1.6a09e667f3bc9p-1,
    0x1.f6a7a2955385dp1

  },
  { // Entry 1166
    0x1.6a09e667f3bc9p-1,
    -0x1.f6a7a2955385dp1

  },
  { // Entry 1167
    -0x1.6a09e667f3bccp-1,
    0x1.f6a7a2955385ep1

  },
  { // Entry 1168
    0x1.6a09e667f3bccp-1,
    -0x1.f6a7a2955385ep1

  },
  { // Entry 1169
    -0x1.6a09e667f3bcep-1,
    0x1.f6a7a2955385fp1

  },
  { // Entry 1170
    0x1.6a09e667f3bcep-1,
    -0x1.f6a7a2955385fp1

  },
  { // Entry 1171
    -0x1.0p0,
    0x1.2d97c7f3321d1p2

  },
  { // Entry 1172
    0x1.0p0,
    -0x1.2d97c7f3321d1p2

  },
  { // Entry 1173
    -0x1.0p0,
    0x1.2d97c7f3321d2p2

  },
  { // Entry 1174
    0x1.0p0,
    -0x1.2d97c7f3321d2p2

  },
  { // Entry 1175
    -0x1.0p0,
    0x1.2d97c7f3321d3p2

  },
  { // Entry 1176
    0x1.0p0,
    -0x1.2d97c7f3321d3p2

  },
  { // Entry 1177
    -0x1.6a09e667f3bd4p-1,
    0x1.5fdbbe9bba774p2

  },
  { // Entry 1178
    0x1.6a09e667f3bd4p-1,
    -0x1.5fdbbe9bba774p2

  },
  { // Entry 1179
    -0x1.6a09e667f3bcep-1,
    0x1.5fdbbe9bba775p2

  },
  { // Entry 1180
    0x1.6a09e667f3bcep-1,
    -0x1.5fdbbe9bba775p2

  },
  { // Entry 1181
    -0x1.6a09e667f3bc8p-1,
    0x1.5fdbbe9bba776p2

  },
  { // Entry 1182
    0x1.6a09e667f3bc8p-1,
    -0x1.5fdbbe9bba776p2

  },
  { // Entry 1183
    0x1.6a09e667f3bc5p-1,
    0x1.c463abeccb2bap2

  },
  { // Entry 1184
    -0x1.6a09e667f3bc5p-1,
    -0x1.c463abeccb2bap2

  },
  { // Entry 1185
    0x1.6a09e667f3bcbp-1,
    0x1.c463abeccb2bbp2

  },
  { // Entry 1186
    -0x1.6a09e667f3bcbp-1,
    -0x1.c463abeccb2bbp2

  },
  { // Entry 1187
    0x1.6a09e667f3bd1p-1,
    0x1.c463abeccb2bcp2

  },
  { // Entry 1188
    -0x1.6a09e667f3bd1p-1,
    -0x1.c463abeccb2bcp2

  },
  { // Entry 1189
    0x1.0p0,
    0x1.f6a7a2955385dp2

  },
  { // Entry 1190
    -0x1.0p0,
    -0x1.f6a7a2955385dp2

  },
  { // Entry 1191
    0x1.0p0,
    0x1.f6a7a2955385ep2

  },
  { // Entry 1192
    -0x1.0p0,
    -0x1.f6a7a2955385ep2

  },
  { // Entry 1193
    0x1.0p0,
    0x1.f6a7a2955385fp2

  },
  { // Entry 1194
    -0x1.0p0,
    -0x1.f6a7a2955385fp2

  },
  { // Entry 1195
    0x1.6a09e667f3bdfp-1,
    0x1.1475cc9eedeffp3

  },
  { // Entry 1196
    -0x1.6a09e667f3bdfp-1,
    -0x1.1475cc9eedeffp3

  },
  { // Entry 1197
    0x1.6a09e667f3bd4p-1,
    0x1.1475cc9eedfp3

  },
  { // Entry 1198
    -0x1.6a09e667f3bd4p-1,
    -0x1.1475cc9eedfp3

  },
  { // Entry 1199
    0x1.6a09e667f3bc9p-1,
    0x1.1475cc9eedf01p3

  },
  { // Entry 1200
    -0x1.6a09e667f3bc9p-1,
    -0x1.1475cc9eedf01p3

  },
  { // Entry 1201
    0x1.34f272993d141p-49,
    0x1.2d97c7f3321d1p3

  },
  { // Entry 1202
    -0x1.34f272993d141p-49,
    -0x1.2d97c7f3321d1p3

  },
  { // Entry 1203
    0x1.a79394c9e8a0ap-52,
    0x1.2d97c7f3321d2p3

  },
  { // Entry 1204
    -0x1.a79394c9e8a0ap-52,
    -0x1.2d97c7f3321d2p3

  },
  { // Entry 1205
    -0x1.961b1acd85d7dp-50,
    0x1.2d97c7f3321d3p3

  },
  { // Entry 1206
    0x1.961b1acd85d7dp-50,
    -0x1.2d97c7f3321d3p3

  },
  { // Entry 1207
    -0x1.6a09e667f3bb9p-1,
    0x1.46b9c347764a2p3

  },
  { // Entry 1208
    0x1.6a09e667f3bb9p-1,
    -0x1.46b9c347764a2p3

  },
  { // Entry 1209
    -0x1.6a09e667f3bc4p-1,
    0x1.46b9c347764a3p3

  },
  { // Entry 1210
    0x1.6a09e667f3bc4p-1,
    -0x1.46b9c347764a3p3

  },
  { // Entry 1211
    -0x1.6a09e667f3bdp-1,
    0x1.46b9c347764a4p3

  },
  { // Entry 1212
    0x1.6a09e667f3bdp-1,
    -0x1.46b9c347764a4p3

  },
  { // Entry 1213
    -0x1.0p0,
    0x1.5fdbbe9bba774p3

  },
  { // Entry 1214
    0x1.0p0,
    -0x1.5fdbbe9bba774p3

  },
  { // Entry 1215
    -0x1.0p0,
    0x1.5fdbbe9bba775p3

  },
  { // Entry 1216
    0x1.0p0,
    -0x1.5fdbbe9bba775p3

  },
  { // Entry 1217
    -0x1.0p0,
    0x1.5fdbbe9bba776p3

  },
  { // Entry 1218
    0x1.0p0,
    -0x1.5fdbbe9bba776p3

  },
  { // Entry 1219
    -0x1.6a09e667f3bep-1,
    0x1.78fdb9effea45p3

  },
  { // Entry 1220
    0x1.6a09e667f3bep-1,
    -0x1.78fdb9effea45p3

  },
  { // Entry 1221
    -0x1.6a09e667f3bd5p-1,
    0x1.78fdb9effea46p3

  },
  { // Entry 1222
    0x1.6a09e667f3bd5p-1,
    -0x1.78fdb9effea46p3

  },
  { // Entry 1223
    -0x1.6a09e667f3bcap-1,
    0x1.78fdb9effea47p3

  },
  { // Entry 1224
    0x1.6a09e667f3bcap-1,
    -0x1.78fdb9effea47p3

  },
  { // Entry 1225
    0x1.6a09e667f3bb8p-1,
    0x1.ab41b09886fe8p3

  },
  { // Entry 1226
    -0x1.6a09e667f3bb8p-1,
    -0x1.ab41b09886fe8p3

  },
  { // Entry 1227
    0x1.6a09e667f3bc4p-1,
    0x1.ab41b09886fe9p3

  },
  { // Entry 1228
    -0x1.6a09e667f3bc4p-1,
    -0x1.ab41b09886fe9p3

  },
  { // Entry 1229
    0x1.6a09e667f3bcfp-1,
    0x1.ab41b09886feap3

  },
  { // Entry 1230
    -0x1.6a09e667f3bcfp-1,
    -0x1.ab41b09886feap3

  },
  { // Entry 1231
    0x1.0p0,
    0x1.c463abeccb2bap3

  },
  { // Entry 1232
    -0x1.0p0,
    -0x1.c463abeccb2bap3

  },
  { // Entry 1233
    0x1.0p0,
    0x1.c463abeccb2bbp3

  },
  { // Entry 1234
    -0x1.0p0,
    -0x1.c463abeccb2bbp3

  },
  { // Entry 1235
    0x1.0p0,
    0x1.c463abeccb2bcp3

  },
  { // Entry 1236
    -0x1.0p0,
    -0x1.c463abeccb2bcp3

  },
  { // Entry 1237
    0x1.6a09e667f3be1p-1,
    0x1.dd85a7410f58bp3

  },
  { // Entry 1238
    -0x1.6a09e667f3be1p-1,
    -0x1.dd85a7410f58bp3

  },
  { // Entry 1239
    0x1.6a09e667f3bd6p-1,
    0x1.dd85a7410f58cp3

  },
  { // Entry 1240
    -0x1.6a09e667f3bd6p-1,
    -0x1.dd85a7410f58cp3

  },
  { // Entry 1241
    0x1.6a09e667f3bcbp-1,
    0x1.dd85a7410f58dp3

  },
  { // Entry 1242
    -0x1.6a09e667f3bcbp-1,
    -0x1.dd85a7410f58dp3

  },
  { // Entry 1243
    0x1.583ebeff65cc2p-49,
    0x1.f6a7a2955385dp3

  },
  { // Entry 1244
    -0x1.583ebeff65cc2p-49,
    -0x1.f6a7a2955385dp3

  },
  { // Entry 1245
    0x1.60fafbfd97309p-51,
    0x1.f6a7a2955385ep3

  },
  { // Entry 1246
    -0x1.60fafbfd97309p-51,
    -0x1.f6a7a2955385ep3

  },
  { // Entry 1247
    -0x1.4f8282013467cp-50,
    0x1.f6a7a2955385fp3

  },
  { // Entry 1248
    0x1.4f8282013467cp-50,
    -0x1.f6a7a2955385fp3

  },
  { // Entry 1249
    -0x1.6a09e667f3ba1p-1,
    0x1.07e4cef4cbd96p4

  },
  { // Entry 1250
    0x1.6a09e667f3ba1p-1,
    -0x1.07e4cef4cbd96p4

  },
  { // Entry 1251
    -0x1.6a09e667f3bb8p-1,
    0x1.07e4cef4cbd97p4

  },
  { // Entry 1252
    0x1.6a09e667f3bb8p-1,
    -0x1.07e4cef4cbd97p4

  },
  { // Entry 1253
    -0x1.6a09e667f3bcep-1,
    0x1.07e4cef4cbd98p4

  },
  { // Entry 1254
    0x1.6a09e667f3bcep-1,
    -0x1.07e4cef4cbd98p4

  },
  { // Entry 1255
    -0x1.0p0,
    0x1.1475cc9eedeffp4

  },
  { // Entry 1256
    0x1.0p0,
    -0x1.1475cc9eedeffp4

  },
  { // Entry 1257
    -0x1.0p0,
    0x1.1475cc9eedfp4

  },
  { // Entry 1258
    0x1.0p0,
    -0x1.1475cc9eedfp4

  },
  { // Entry 1259
    -0x1.0p0,
    0x1.1475cc9eedf01p4

  },
  { // Entry 1260
    0x1.0p0,
    -0x1.1475cc9eedf01p4

  },
  { // Entry 1261
    -0x1.6a09e667f3bedp-1,
    0x1.2106ca4910068p4

  },
  { // Entry 1262
    0x1.6a09e667f3bedp-1,
    -0x1.2106ca4910068p4

  },
  { // Entry 1263
    -0x1.6a09e667f3bd7p-1,
    0x1.2106ca4910069p4

  },
  { // Entry 1264
    0x1.6a09e667f3bd7p-1,
    -0x1.2106ca4910069p4

  },
  { // Entry 1265
    -0x1.6a09e667f3bcp-1,
    0x1.2106ca491006ap4

  },
  { // Entry 1266
    0x1.6a09e667f3bcp-1,
    -0x1.2106ca491006ap4

  },
  { // Entry 1267
    -0x1.34f272993d141p-48,
    0x1.2d97c7f3321d1p4

  },
  { // Entry 1268
    0x1.34f272993d141p-48,
    -0x1.2d97c7f3321d1p4

  },
  { // Entry 1269
    -0x1.a79394c9e8a0ap-51,
    0x1.2d97c7f3321d2p4

  },
  { // Entry 1270
    0x1.a79394c9e8a0ap-51,
    -0x1.2d97c7f3321d2p4

  },
  { // Entry 1271
    0x1.961b1acd85d7dp-49,
    0x1.2d97c7f3321d3p4

  },
  { // Entry 1272
    -0x1.961b1acd85d7dp-49,
    -0x1.2d97c7f3321d3p4

  },
  { // Entry 1273
    0x1.6a09e667f3bap-1,
    0x1.3a28c59d54339p4

  },
  { // Entry 1274
    -0x1.6a09e667f3bap-1,
    -0x1.3a28c59d54339p4

  },
  { // Entry 1275
    0x1.6a09e667f3bb7p-1,
    0x1.3a28c59d5433ap4

  },
  { // Entry 1276
    -0x1.6a09e667f3bb7p-1,
    -0x1.3a28c59d5433ap4

  },
  { // Entry 1277
    0x1.6a09e667f3bcep-1,
    0x1.3a28c59d5433bp4

  },
  { // Entry 1278
    -0x1.6a09e667f3bcep-1,
    -0x1.3a28c59d5433bp4

  },
  { // Entry 1279
    0x1.0p0,
    0x1.46b9c347764a2p4

  },
  { // Entry 1280
    -0x1.0p0,
    -0x1.46b9c347764a2p4

  },
  { // Entry 1281
    0x1.0p0,
    0x1.46b9c347764a3p4

  },
  { // Entry 1282
    -0x1.0p0,
    -0x1.46b9c347764a3p4

  },
  { // Entry 1283
    0x1.0p0,
    0x1.46b9c347764a4p4

  },
  { // Entry 1284
    -0x1.0p0,
    -0x1.46b9c347764a4p4

  },
  { // Entry 1285
    0x1.6a09e667f3beep-1,
    0x1.534ac0f19860bp4

  },
  { // Entry 1286
    -0x1.6a09e667f3beep-1,
    -0x1.534ac0f19860bp4

  },
  { // Entry 1287
    0x1.6a09e667f3bd8p-1,
    0x1.534ac0f19860cp4

  },
  { // Entry 1288
    -0x1.6a09e667f3bd8p-1,
    -0x1.534ac0f19860cp4

  },
  { // Entry 1289
    0x1.6a09e667f3bc1p-1,
    0x1.534ac0f19860dp4

  },
  { // Entry 1290
    -0x1.6a09e667f3bc1p-1,
    -0x1.534ac0f19860dp4

  },
  { // Entry 1291
    0x1.3dc585b2c7422p-48,
    0x1.5fdbbe9bba774p4

  },
  { // Entry 1292
    -0x1.3dc585b2c7422p-48,
    -0x1.5fdbbe9bba774p4

  },
  { // Entry 1293
    0x1.ee2c2d963a10cp-51,
    0x1.5fdbbe9bba775p4

  },
  { // Entry 1294
    -0x1.ee2c2d963a10cp-51,
    -0x1.5fdbbe9bba775p4

  },
  { // Entry 1295
    -0x1.8474f49a717bdp-49,
    0x1.5fdbbe9bba776p4

  },
  { // Entry 1296
    0x1.8474f49a717bdp-49,
    -0x1.5fdbbe9bba776p4

  },
  { // Entry 1297
    -0x1.6a09e667f3b9fp-1,
    0x1.6c6cbc45dc8dcp4

  },
  { // Entry 1298
    0x1.6a09e667f3b9fp-1,
    -0x1.6c6cbc45dc8dcp4

  },
  { // Entry 1299
    -0x1.6a09e667f3bb6p-1,
    0x1.6c6cbc45dc8ddp4

  },
  { // Entry 1300
    0x1.6a09e667f3bb6p-1,
    -0x1.6c6cbc45dc8ddp4

  },
  { // Entry 1301
    -0x1.6a09e667f3bcdp-1,
    0x1.6c6cbc45dc8dep4

  },
  { // Entry 1302
    0x1.6a09e667f3bcdp-1,
    -0x1.6c6cbc45dc8dep4

  },
  { // Entry 1303
    -0x1.0p0,
    0x1.78fdb9effea45p4

  },
  { // Entry 1304
    0x1.0p0,
    -0x1.78fdb9effea45p4

  },
  { // Entry 1305
    -0x1.0p0,
    0x1.78fdb9effea46p4

  },
  { // Entry 1306
    0x1.0p0,
    -0x1.78fdb9effea46p4

  },
  { // Entry 1307
    -0x1.0p0,
    0x1.78fdb9effea47p4

  },
  { // Entry 1308
    0x1.0p0,
    -0x1.78fdb9effea47p4

  },
  { // Entry 1309
    -0x1.6a09e667f3befp-1,
    0x1.858eb79a20baep4

  },
  { // Entry 1310
    0x1.6a09e667f3befp-1,
    -0x1.858eb79a20baep4

  },
  { // Entry 1311
    -0x1.6a09e667f3bd8p-1,
    0x1.858eb79a20bafp4

  },
  { // Entry 1312
    0x1.6a09e667f3bd8p-1,
    -0x1.858eb79a20bafp4

  },
  { // Entry 1313
    -0x1.6a09e667f3bc2p-1,
    0x1.858eb79a20bbp4

  },
  { // Entry 1314
    0x1.6a09e667f3bc2p-1,
    -0x1.858eb79a20bbp4

  },
  { // Entry 1315
    0x1.fa7299b17573dp-1,
    0x1.fffffffffffffp62

  },
  { // Entry 1316
    -0x1.fa7299b17573dp-1,
    -0x1.fffffffffffffp62

  },
  { // Entry 1317
    0x1.fff6dfd42dc54p-1,
    0x1.0p63

  },
  { // Entry 1318
    -0x1.fff6dfd42dc54p-1,
    -0x1.0p63

  },
  { // Entry 1319
    0x1.e456b818e7397p-1,
    0x1.0000000000001p63

  },
  { // Entry 1320
    -0x1.e456b818e7397p-1,
    -0x1.0000000000001p63

  },
  { // Entry 1321
    -0x1.86dcca0d689e8p-1,
    0x1.fffffffffffffp26

  },
  { // Entry 1322
    0x1.86dcca0d689e8p-1,
    -0x1.fffffffffffffp26

  },
  { // Entry 1323
    -0x1.86dcc9babb0a4p-1,
    0x1.0p27

  },
  { // Entry 1324
    0x1.86dcc9babb0a4p-1,
    -0x1.0p27

  },
  { // Entry 1325
    -0x1.86dcc9155fe18p-1,
    0x1.0000000000001p27

  },
  { // Entry 1326
    0x1.86dcc9155fe18p-1,
    -0x1.0000000000001p27

  },
  { // Entry 1327
    -0x1.8f22f84d42da2p-1,
    0x1.fffffffffffffp23

  },
  { // Entry 1328
    0x1.8f22f84d42da2p-1,
    -0x1.fffffffffffffp23

  },
  { // Entry 1329
    -0x1.8f22f8433d6eep-1,
    0x1.0p24

  },
  { // Entry 1330
    0x1.8f22f8433d6eep-1,
    -0x1.0p24

  },
  { // Entry 1331
    -0x1.8f22f82f32986p-1,
    0x1.0000000000001p24

  },
  { // Entry 1332
    0x1.8f22f82f32986p-1,
    -0x1.0000000000001p24

  },
  { // Entry 1333
    -0x1.837b9dddc1eacp-1,
    0x1.fffffffffffffp1

  },
  { // Entry 1334
    0x1.837b9dddc1eacp-1,
    -0x1.fffffffffffffp1

  },
  { // Entry 1335
    -0x1.837b9dddc1eaep-1,
    0x1.0p2

  },
  { // Entry 1336
    0x1.837b9dddc1eaep-1,
    -0x1.0p2

  },
  { // Entry 1337
    -0x1.837b9dddc1eb4p-1,
    0x1.0000000000001p2

  },
  { // Entry 1338
    0x1.837b9dddc1eb4p-1,
    -0x1.0000000000001p2

  },
  { // Entry 1339
    0x1.d18f6ead1b447p-1,
    0x1.fffffffffffffp0

  },
  { // Entry 1340
    -0x1.d18f6ead1b447p-1,
    -0x1.fffffffffffffp0

  },
  { // Entry 1341
    0x1.d18f6ead1b446p-1,
    0x1.0p1

  },
  { // Entry 1342
    -0x1.d18f6ead1b446p-1,
    -0x1.0p1

  },
  { // Entry 1343
    0x1.d18f6ead1b444p-1,
    0x1.0000000000001p1

  },
  { // Entry 1344
    -0x1.d18f6ead1b444p-1,
    -0x1.0000000000001p1

  },
  { // Entry 1345
    0x1.aed548f090cedp-1,
    0x1.fffffffffffffp-1

  },
  { // Entry 1346
    -0x1.aed548f090cedp-1,
    -0x1.fffffffffffffp-1

  },
  { // Entry 1347
    0x1.aed548f090ceep-1,
    0x1.0p0

  },
  { // Entry 1348
    -0x1.aed548f090ceep-1,
    -0x1.0p0

  },
  { // Entry 1349
    0x1.aed548f090cefp-1,
    0x1.0000000000001p0

  },
  { // Entry 1350
    -0x1.aed548f090cefp-1,
    -0x1.0000000000001p0

  },
  { // Entry 1351
    0x1.eaee8744b05efp-2,
    0x1.fffffffffffffp-2

  },
  { // Entry 1352
    -0x1.eaee8744b05efp-2,
    -0x1.fffffffffffffp-2

  },
  { // Entry 1353
    0x1.eaee8744b05fp-2,
    0x1.0p-1

  },
  { // Entry 1354
    -0x1.eaee8744b05fp-2,
    -0x1.0p-1

  },
  { // Entry 1355
    0x1.eaee8744b05f2p-2,
    0x1.0000000000001p-1

  },
  { // Entry 1356
    -0x1.eaee8744b05f2p-2,
    -0x1.0000000000001p-1

  },
  { // Entry 1357
    0x1.faaeed4f31576p-3,
    0x1.fffffffffffffp-3

  },
  { // Entry 1358
    -0x1.faaeed4f31576p-3,
    -0x1.fffffffffffffp-3

  },
  { // Entry 1359
    0x1.faaeed4f31577p-3,
    0x1.0p-2

  },
  { // Entry 1360
    -0x1.faaeed4f31577p-3,
    -0x1.0p-2

  },
  { // Entry 1361
    0x1.faaeed4f31579p-3,
    0x1.0000000000001p-2

  },
  { // Entry 1362
    -0x1.faaeed4f31579p-3,
    -0x1.0000000000001p-2

  },
  { // Entry 1363
    0x1.feaaeee86ee35p-4,
    0x1.fffffffffffffp-4

  },
  { // Entry 1364
    -0x1.feaaeee86ee35p-4,
    -0x1.fffffffffffffp-4

  },
  { // Entry 1365
    0x1.feaaeee86ee36p-4,
    0x1.0p-3

  },
  { // Entry 1366
    -0x1.feaaeee86ee36p-4,
    -0x1.0p-3

  },
  { // Entry 1367
    0x1.feaaeee86ee38p-4,
    0x1.0000000000001p-3

  },
  { // Entry 1368
    -0x1.feaaeee86ee38p-4,
    -0x1.0000000000001p-3

  },
  { // Entry 1369
    0x1.ffaaaeeed4edap-5,
    0x1.fffffffffffffp-5

  },
  { // Entry 1370
    -0x1.ffaaaeeed4edap-5,
    -0x1.fffffffffffffp-5

  },
  { // Entry 1371
    0x1.ffaaaeeed4edbp-5,
    0x1.0p-4

  },
  { // Entry 1372
    -0x1.ffaaaeeed4edbp-5,
    -0x1.0p-4

  },
  { // Entry 1373
    0x1.ffaaaeeed4eddp-5,
    0x1.0000000000001p-4

  },
  { // Entry 1374
    -0x1.ffaaaeeed4eddp-5,
    -0x1.0000000000001p-4

  },
  { // Entry 1375
    0x1.ffeaaaeeee86ep-6,
    0x1.fffffffffffffp-6

  },
  { // Entry 1376
    -0x1.ffeaaaeeee86ep-6,
    -0x1.fffffffffffffp-6

  },
  { // Entry 1377
    0x1.ffeaaaeeee86fp-6,
    0x1.0p-5

  },
  { // Entry 1378
    -0x1.ffeaaaeeee86fp-6,
    -0x1.0p-5

  },
  { // Entry 1379
    0x1.ffeaaaeeee871p-6,
    0x1.0000000000001p-5

  },
  { // Entry 1380
    -0x1.ffeaaaeeee871p-6,
    -0x1.0000000000001p-5

  },
  { // Entry 1381
    0x1.fffaaaaeeeed4p-7,
    0x1.fffffffffffffp-7

  },
  { // Entry 1382
    -0x1.fffaaaaeeeed4p-7,
    -0x1.fffffffffffffp-7

  },
  { // Entry 1383
    0x1.fffaaaaeeeed5p-7,
    0x1.0p-6

  },
  { // Entry 1384
    -0x1.fffaaaaeeeed5p-7,
    -0x1.0p-6

  },
  { // Entry 1385
    0x1.fffaaaaeeeed7p-7,
    0x1.0000000000001p-6

  },
  { // Entry 1386
    -0x1.fffaaaaeeeed7p-7,
    -0x1.0000000000001p-6

  },
  { // Entry 1387
    0x1.fffffffaaaaaap-15,
    0x1.fffffffffffffp-15

  },
  { // Entry 1388
    -0x1.fffffffaaaaaap-15,
    -0x1.fffffffffffffp-15

  },
  { // Entry 1389
    0x1.fffffffaaaaabp-15,
    0x1.0p-14

  },
  { // Entry 1390
    -0x1.fffffffaaaaabp-15,
    -0x1.0p-14

  },
  { // Entry 1391
    0x1.fffffffaaaaadp-15,
    0x1.0000000000001p-14

  },
  { // Entry 1392
    -0x1.fffffffaaaaadp-15,
    -0x1.0000000000001p-14

  },
  { // Entry 1393
    0x1.fffffffffffffp-28,
    0x1.fffffffffffffp-28

  },
  { // Entry 1394
    -0x1.fffffffffffffp-28,
    -0x1.fffffffffffffp-28

  },
  { // Entry 1395
    0x1.0p-27,
    0x1.0p-27

  },
  { // Entry 1396
    -0x1.0p-27,
    -0x1.0p-27

  },
  { // Entry 1397
    0x1.0000000000001p-27,
    0x1.0000000000001p-27

  },
  { // Entry 1398
    -0x1.0000000000001p-27,
    -0x1.0000000000001p-27

  },
  { // Entry 1399
    0x1.fffffffffffffp-31,
    0x1.fffffffffffffp-31

  },
  { // Entry 1400
    -0x1.fffffffffffffp-31,
    -0x1.fffffffffffffp-31

  },
  { // Entry 1401
    0x1.0p-30,
    0x1.0p-30

  },
  { // Entry 1402
    -0x1.0p-30,
    -0x1.0p-30

  },
  { // Entry 1403
    0x1.0000000000001p-30,
    0x1.0000000000001p-30

  },
  { // Entry 1404
    -0x1.0000000000001p-30,
    -0x1.0000000000001p-30

  },
  { // Entry 1405
    -0x1.452fc98b34e97p-8,
    -0x1.fffffffffffffp1023

  },
  { // Entry 1406
    0x1.452fc98b34e97p-8,
    0x1.fffffffffffffp1023

  },
  { // Entry 1407
    0x1.452fc98b34e97p-8,
    0x1.fffffffffffffp1023

  },
  { // Entry 1408
    -0x1.452fc98b34e97p-8,
    -0x1.fffffffffffffp1023

  },
  { // Entry 1409
    0x1.452fc98b34e97p-8,
    0x1.fffffffffffffp1023

  },
  { // Entry 1410
    -0x1.452fc98b34e97p-8,
    -0x1.fffffffffffffp1023

  },
  { // Entry 1411
    0x1.daa3677c6ee8ap-1,
    0x1.ffffffffffffep1023

  },
  { // Entry 1412
    -0x1.daa3677c6ee8ap-1,
    -0x1.ffffffffffffep1023

  },
  { // Entry 1413
    0x1.1a62633145c07p-53,
    0x1.921fb54442d18p1

  },
  { // Entry 1414
    -0x1.1a62633145c07p-53,
    -0x1.921fb54442d18p1

  },
  { // Entry 1415
    0x1.0p0,
    0x1.921fb54442d18p0

  },
  { // Entry 1416
    -0x1.0p0,
    -0x1.921fb54442d18p0

  },
  { // Entry 1417
    0x1.aed548f090cefp-1,
    0x1.0000000000001p0

  },
  { // Entry 1418
    -0x1.aed548f090cefp-1,
    -0x1.0000000000001p0

  },
  { // Entry 1419
    0x1.aed548f090ceep-1,
    0x1.0p0

  },
  { // Entry 1420
    -0x1.aed548f090ceep-1,
    -0x1.0p0

  },
  { // Entry 1421
    0x1.aed548f090cedp-1,
    0x1.fffffffffffffp-1

  },
  { // Entry 1422
    -0x1.aed548f090cedp-1,
    -0x1.fffffffffffffp-1

  },
  { // Entry 1423
    0x1.6a09e667f3bccp-1,
    0x1.921fb54442d18p-1

  },
  { // Entry 1424
    -0x1.6a09e667f3bccp-1,
    -0x1.921fb54442d18p-1

  },
  { // Entry 1425
    0x1.0000000000001p-1022,
    0x1.0000000000001p-1022

  },
  { // Entry 1426
    -0x1.0000000000001p-1022,
    -0x1.0000000000001p-1022

  },
  { // Entry 1427
    0x1.0p-1022,
    0x1.0p-1022

  },
  { // Entry 1428
    -0x1.0p-1022,
    -0x1.0p-1022

  },
  { // Entry 1429
    0x0.fffffffffffffp-1022,
    0x0.fffffffffffffp-1022

  },
  { // Entry 1430
    -0x0.fffffffffffffp-1022,
    -0x0.fffffffffffffp-1022

  },
  { // Entry 1431
    0x0.ffffffffffffep-1022,
    0x0.ffffffffffffep-1022

  },
  { // Entry 1432
    -0x0.ffffffffffffep-1022,
    -0x0.ffffffffffffep-1022

  },
  { // Entry 1433
    0x0.0000000000002p-1022,
    0x0.0000000000002p-1022

  },
  { // Entry 1434
    -0x0.0000000000002p-1022,
    -0x0.0000000000002p-1022

  },
  { // Entry 1435
    0x0.0000000000001p-1022,
    0x0.0000000000001p-1022

  },
  { // Entry 1436
    -0x0.0000000000001p-1022,
    -0x0.0000000000001p-1022

  },
  { // Entry 1437
    0x0.0p0,
    0x0.0p0

  },
  { // Entry 1438
    0x1.a4a3d9c2131dfp-2,
    0x1.5bf0a8b145769p1
  },
  { // Entry 1439
    -0x1.a4a3d9c2131dfp-2,
    -0x1.5bf0a8b145769p1
  },
  { // Entry 1440
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 1441
    -0x0.0p0,
    -0x0.0p0
  },
  { // Entry 1442
    0x1.aed548f090ceep-1,
    0x1.0p0
  },
  { // Entry 1443
    -0x1.aed548f090ceep-1,
    -0x1.0p0
  },
  { // Entry 1444
    -0x1.eaee8744b05fp-2,
    -0x1.0p-1
  },
  { // Entry 1445
    0x1.eaee8744b05fp-2,
    0x1.0p-1
  },
  { // Entry 1446
    0x1.1a62633145c07p-52,
    -0x1.921fb54442d18p2
  },
  { // Entry 1447
    0x1.0p0,
    -0x1.2d97c7f3321d2p2
  },
  { // Entry 1448
    -0x1.1a62633145c07p-53,
    -0x1.921fb54442d18p1
  },
  { // Entry 1449
    -0x1.0p0,
    -0x1.921fb54442d18p0
  },
  { // Entry 1450
    0x0.0p0,
    0x0.0p0
  },
  { // Entry 1451
    0x1.0p0,
    0x1.921fb54442d18p0
  },
  { // Entry 1452
    0x1.1a62633145c07p-53,
    0x1.921fb54442d18p1
  },
  { // Entry 1453
    -0x1.0p0,
    0x1.2d97c7f3321d2p2
  },
  { // Entry 1454
    -0x1.1a62633145c07p-52,
    0x1.921fb54442d18p2
  },
};

#endif // __BIONIC__

TEST(math_csv, csv_values) {
#if defined(__BIONIC__)
  for (size_t i = 0; i < sizeof(g_sinh_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_sinh_data[i].expected, sinh(g_sinh_data[i].call_data)) << "Failed on element " << i << " of function sinh";
  }
  for (size_t i = 0; i < sizeof(g_asin_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_asin_data[i].expected, asin(g_asin_data[i].call_data)) << "Failed on element " << i << " of function asin";
  }
  for (size_t i = 0; i < sizeof(g_cos_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_cos_data[i].expected, cos(g_cos_data[i].call_data)) << "Failed on element " << i << " of function cos";
  }
  for (size_t i = 0; i < sizeof(g_log_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_log_data[i].expected, log(g_log_data[i].call_data)) << "Failed on element " << i << " of function log";
  }
  for (size_t i = 0; i < sizeof(g_atan_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_atan_data[i].expected, atan(g_atan_data[i].call_data)) << "Failed on element " << i << " of function atan";
  }
  for (size_t i = 0; i < sizeof(g_tanh_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_tanh_data[i].expected, tanh(g_tanh_data[i].call_data)) << "Failed on element " << i << " of function tanh";
  }
  for (size_t i = 0; i < sizeof(g_acos_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_acos_data[i].expected, acos(g_acos_data[i].call_data)) << "Failed on element " << i << " of function acos";
  }
  for (size_t i = 0; i < sizeof(g_sqrt_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_sqrt_data[i].expected, sqrt(g_sqrt_data[i].call_data)) << "Failed on element " << i << " of function sqrt";
  }
  for (size_t i = 0; i < sizeof(g_cosh_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_cosh_data[i].expected, cosh(g_cosh_data[i].call_data)) << "Failed on element " << i << " of function cosh";
  }
  for (size_t i = 0; i < sizeof(g_abs_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_abs_data[i].expected, abs(g_abs_data[i].call_data)) << "Failed on element " << i << " of function abs";
  }
  for (size_t i = 0; i < sizeof(g_exp_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_exp_data[i].expected, exp(g_exp_data[i].call_data)) << "Failed on element " << i << " of function exp";
  }
  for (size_t i = 0; i < sizeof(g_tan_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_tan_data[i].expected, tan(g_tan_data[i].call_data)) << "Failed on element " << i << " of function tan";
  }
  for (size_t i = 0; i < sizeof(g_log10_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_log10_data[i].expected, log10(g_log10_data[i].call_data)) << "Failed on element " << i << " of function log10";
  }
  for (size_t i = 0; i < sizeof(g_sin_data)/sizeof(TestCase); i++) {
    EXPECT_DOUBLE_EQ(g_sin_data[i].expected, sin(g_sin_data[i].call_data)) << "Failed on element " << i << " of function sin";
  }
#else // __BIONIC__
  GTEST_LOG_(INFO) << "This test does nothing.";
#endif // __BIONIC__
}