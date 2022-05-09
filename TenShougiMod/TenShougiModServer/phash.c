#include "shogi.h"

static unsigned short tab[] = {
5866,14081,15663,4534,14969,3034,8274,7570,
14571,15428,2921,37,2534,4727,660,249,
8308,3662,15794,10079,13857,13019,6070,9141,
3942,14629,6891,11561,12156,4793,11132,12989,
10044,16304,9389,2228,5053,3789,5988,4479,
8719,13901,15524,3098,7570,12949,1074,0,
10079,16233,7531,14642,13600,8087,0,11690,
8579,15413,6522,1076,2677,6914,10881,13803,
13806,15789,14642,2135,15427,14023,14079,7708,
3602,0,4976,10910,4821,4543,5455,9368,
4201,9970,10647,15130,10079,15020,5558,15902,
7858,2962,10386,2155,14641,12308,8021,8170,
2490,10528,15427,14629,10528,15136,999,1883,
14912,10788,6360,3677,14960,12257,2294,12614,
8698,12433,9867,12663,16016,11332,8568,8348,
0,3228,8594,11249,6674,3098,9297,10077,
10340,5803,5420,6070,660,5676,1,9497,
8700,14264,5846,12433,16282,12561,12032,13732,
8239,10731,7236,10286,2678,7321,13368,6594,
9047,5291,8293,2809,3268,6485,10509,2277,
10471,595,8887,15919,14413,15881,8827,13726,
13975,14092,7634,1652,8753,6801,6568,15853,
1022,5664,0,9303,15719,6128,6674,7433,
157,9360,6555,12432,9593,9660,14081,13927,
8702,13702,8663,5629,3691,16298,1573,902,
1272,2376,1084,14447,14666,1692,15316,15129,
3833,920,8348,15839,15902,15635,10340,6603,
1764,9634,3833,2470,4704,9646,385,13200,
3009,1562,6846,6598,15861,8924,11762,3766,
9095,431,6599,1987,15902,11064,4972,12175,
14642,13857,6130,5833,13857,12112,8170,9867,
2569,9648,1883,10205,12365,6801,5222,14506,
1819,5142,2172,5291,15542,12112,2092,5560,
4538,5461,15059,11773,15884,2099,6876,11914,
3833,4972,8164,6240,12487,1341,10731,11762,
14548,4471,5044,7576,2392,16129,8175,7559,
12559,15861,6396,2926,7618,8417,15759,0,
9095,3510,7247,1712,16345,2209,8055,0,
319,12209,13144,15159,8238,7822,5014,10077,
2284,7177,6555,15861,14479,2510,8170,11806,
12149,12440,14185,11762,11552,8260,11864,11728,
15046,11561,4707,13420,3615,2233,2277,10668,
2228,8016,1813,2677,5240,6594,6636,9911,
1737,4396,12500,9682,7176,2027,12500,5244,
12112,1768,14571,12663,6729,3662,8170,15641,
15316,9648,5373,5475,6522,14571,3662,15902,
1652,9648,2228,8348,902,5988,9150,13250,
13411,4675,431,5507,10616,13987,15316,1378,
13832,13356,10087,9543,9093,8065,10497,8228,
11762,616,12968,5411,9648,12487,9463,15597,
11292,13985,14482,14600,1705,4678,12020,8828,
6210,9481,14980,5738,5990,14447,1569,8618,
2273,8543,1819,16158,12447,12470,10160,4725,
10778,1412,8040,4300,9550,568,7394,592,
9481,15680,3476,16128,7660,1214,359,6181,
8618,4067,14699,13368,514,11355,8914,3801,
14520,8885,10434,8274,7709,4312,7243,12633,
4867,5981,4538,2490,3034,5558,6801,7512,
14971,4315,4905,11682,8845,3732,1475,11661,
10189,1741,11667,5605,2656,9573,10898,2796,
1817,15216,11152,470,5988,6164,4972,10386,
13128,7247,3268,6729,4300,13101,8568,12663,
13849,3600,9718,11203,10528,13732,5251,14148,
8568,13128,10205,11332,1712,12821,9557,12753,
10505,5276,109,10394,10122,9809,3513,16093,
2490,9943,14798,15084,4598,5174,15648,13109,
0,6971,15426,14500,14571,6831,2623,6801,
6522,15388,3228,11053,0,2902,4285,13853,
14830,5373,15137,10087,3268,2569,6284,2902,
1483,3838,12159,4764,15130,6846,6957,12499,
4604,2739,14629,996,4867,13420,7791,5263,
8119,10088,4128,7950,11875,16244,1147,3473,
3268,15869,12821,11152,9922,1987,8238,15271,
852,6921,4155,7857,11187,2275,14224,15725,
9967,4479,6522,11152,2457,5962,13732,10088,
11226,11359,5053,13420,3727,12487,3823,13412,
13732,5962,11005,11173,8719,4354,6821,11421,
15640,4769,14868,9369,215,8176,2363,1930,
4124,11338,5373,2135,5253,10648,6220,5853,
2741,12020,14070,14571,7476,9011,11415,1306,
5318,13418,7293,15364,8898,3402,11976,9325,
1562,7366,3395,10762,7144,2336,13458,4538,
1159,1987,11652,12964,13513,541,13792,9741,
10840,16129,10917,4534,13600,5462,2357,3890,
6299,11315,13376,13820,13369,4450,14282,3510,
10008,2381,9301,10165,4832,13042,3789,11751,
8021,14286,9341,436,9171,8623,12745,12738,
567,13996,3654,9495,844,8260,15108,2851,
10351,2756,8572,3694,8851,5833,4280,12168,
9550,9095,1652,6594,2534,3662,12009,6827,
7447,2228,6555,15919,2294,6522,6674,0,
13128,15478,3098,14571,1573,13117,16336,3422,
12751,4635,9808,13171,11875,14148,14759,15680,
610,780,14127,15067,12717,11768,2223,4745,
2111,4363,6821,12230,13200,16093,410,15205,
3838,15020,12230,3473,7951,2926,981,13414,
3034,9835,3875,7247,15776,11053,10533,6393,
6729,1712,12233,10088,5152,1378,6043,3871,
6043,12487,15527,11769,12308,5962,11864,10804,
3268,14005,13662,10647,4972,15137,14485,7784,
7275,410,6555,2470,7074,359,12751,12054,
8851,2993,9487,13270,9267,2370,8119,14112,
6639,157,10245,16121,9967,14912,10307,8011,
12094,2470,10322,3570,6154,1453,15084,14801,
3823,7259,780,4972,13047,14500,2569,5833,
56,14601,2690,5036,788,46,2738,9922,
11332,10619,8011,13128,3268,431,13412,8851,
14748,5036,12185,13042,10030,11716,100,16205,
7951,5738,8417,10757,6463,7114,1183,14798,
10205,9721,15359,10074,8727,4587,11875,14615,
10340,9171,9171,4686,7731,4269,13042,8574,
16298,7024,10309,2858,4904,7223,8238,7512,
4556,3532,15222,2161,1162,6476,3221,10290,
11229,15137,14921,1319,4925,1991,7817,7427,
15089,9366,920,3875,4641,11787,14255,4915,
7588,15555,13106,16014,12882,10324,341,1331,
10685,7950,12292,10230,15336,7431,7576,6158,
5058,12149,2777,13434,5263,15769,9877,2704,
10300,867,1443,5285,2155,4474,935,12586,
12601,158,16191,8119,3726,3554,10398,13418,
8170,4867,14642,15478,1712,10136,6674,0,
46,9867,3766,12433,6057,3972,2156,852,
2144,9856,14761,11491,46,363,10561,4484,
999,9557,2902,7323,15084,8274,16087,6679,
6867,6971,2914,586,6240,5988,10386,8304,
3662,10088,5988,12821,7409,8308,15931,14376,
14700,15998,4484,1766,15794,3833,2464,4342,
1358,319,10099,15776,15839,1530,8538,7800,
3641,5569,7469,2104,8314,8976,5962,11857,
10330,8293,12356,1399,1991,5415,10497,15478,
15931,8308,11608,10167,4185,9369,1658,6193,
5962,5132,12322,319,7802,4707,3034,11308,
9559,10788,2228,8339,13513,5222,1712,3098,
1987,2719,1548,15427,13361,13736,5139,1255,
11064,8011,985,6135,788,2761,6522,5362,
13128,3476,6164,12032,2902,15881,11053,14538,
12964,14730,3751,12890,396,1132,6164,7938,
3165,7677,12218,7007,4824,6266,4198,9127,
12821,6463,199,6522,5462,1084,7232,15717,
3354,2788,6294,11857,14174,2235,6135,7808,
15931,10377,1168,9162,7176,3595,10046,1960,
3726,1723,6779,10955,11201,4721,661,7247,
5988,11291,9095,8555,7017,5975,14500,15822,
10887,7999,9648,15908,2938,8293,14629,4233,
8419,5141,12427,9360,2948,9321,13473,1134,
87,4180,2284,8822,3893,15653,15668,12175,
7951,12668,13119,7839,4793,4686,914,9481,
11912,13196,4235,15293,5704,9063,1605,15640,
13270,1192,9967,8698,5576,3059,4067,11809,
5420,7691,16087,764,12487,2273,14224,13412,
5373,12663,788,12433,8870,15902,14019,3966,
1104,11153,13117,8913,46,9922,15240,8260,
9874,1712,9096,7708,4544,15601,0,10529,
7419,9404,653,4067,10717,4901,10009,7229,
9867,7620,1412,14284,9931,8845,13975,8238,
10941,10205,678,11308,14985,12099,3268,11120,
14148,7900,5934,10099,12903,6274,9648,2851,
972,5149,9506,12102,723,15873,14497,5447,
13505,13658,13128,4699,11152,0,0,7808,
2623,6674,9557,7920,10004,14081,0,15478,
12498,7162,13662,14438,6939,10841,6599,7074,
2129,9303,3614,5121,13626,14806,13096,5275,
12032,11431,12265,242,12651,6465,11153,15635,
6154,7864,3263,7708,13216,9466,4929,4763,
16121,12949,6801,14244,6289,3766,8913,3228,
10099,4198,11895,286,13119,10778,12230,7238,
3614,14081,7323,4050,6846,14629,13420,9427,
5460,5130,10528,15705,6369,8361,1212,8866,
12821,15427,7323,2777,9648,8744,5386,8241,
11875,14112,14770,15369,9813,6750,13732,6594,
2464,11682,2031,15129,3532,12112,4831,4354,
2275,10376,11226,11611,8313,9590,14500,8598,
13403,4316,15388,13458,961,208,5995,1225,
6455,5932,11987,2902,13128,1224,1003,7409,
3028,650,13436,8485,6653,911,9425,11421,
9554,10993,15293,514,6670,14578,4972,9874,
8483,2534,14969,1934,14154,8926,11830,8784,
2987,9634,6670,13501,13957,4260,5761,15347,
7991,13414,7159,7233,14206,11665,6715,11079,
3545,5185,15639,14307,9338,8971,15994,5645,
3098,14081,788,8170,10188,8016,5988,15919,
12788,14960,10214,12606,11152,2470,13420,2927,
5240,7370,9648,10099,2039,4846,10610,11823,
13270,14761,5962,7512,10079,4867,8260,2135,
1052,10823,5729,10395,14361,13382,12200,8162,
5117,7531,13361,14520,10602,8685,3654,9777,
16263,6135,6389,11745,10383,8550,12618,10340,
4128,15459,7062,5135,9047,9346,5897,15479,
7268,5576,0,14937,12736,5545,10079,6507,
3238,3204,9865,5843,12487,198,8251,13195,
158,8913,6598,4373,15991,13857,3615,11106,
3244,10654,6394,6183,11633,3098,12257,15427,
1768,7209,11987,963,13857,7247,16046,7239,
13200,11773,2914,12927,11669,13011,1905,9089,
16176,3743,158,5222,12717,5825,16087,9519,
9227,15563,1712,8511,11005,4867,13200,13106,
10788,3606,8680,3447,5833,3709,4526,10847,
2871,6000,2204,15769,2969,7617,15976,7618,
4067,10757,15316,6476,10509,8772,9095,0,
8996,410,14742,7239,10532,9406,7419,15903,
5437,5560,12608,9141,3986,14358,7409,3992,
3221,6170,11821,508,6434,1442,4260,3813,
8308,6456,3534,431,4538,3406,12435,5643,
7144,8870,9186,3775,1817,9905,7951,4336,
2273,12264,6513,2993,16270,5541,5077,6263,
1259,2559,1675,16218,1790,12804,15098,4445,
10336,15216,12144,8618,4573,13679,15000,10189,
2209,4583,5353,13784,2623,7720,5833,11064,
10088,10077,10610,6786,10939,10867,1768,8119,
7394,12056,3789,1074,15413,10527,7247,12487,
8568,10136,2228,13412,4840,1652,6740,11322,
5037,2910,5174,10340,2135,8011,11895,11895,
109,11957,14336,13234,8225,14343,9063,9819,
13517,11635,13549,14936,988,3809,5833,7857,
13462,297,7668,14482,4316,10848,2569,13931,
5593,12056,11103,14170,448,902,1768,11552,
6043,0,0,8646,10391,0,15316,3737,
10594,11026,13280,14667,5713,4057,2787,4191,
3838,7247,8772,14185,15641,8772,0,14081,
0,9429,17,613,7428,15131,15651,14350,
4365,15856,9521,3228,7950,3395,0,6594,
11078,14975,6889,10768,4288,6707,7175,2235,
10896,911,7707,7592,12916,15570,15644,6997,
12523,14757,11895,7109,16279,12969,11690,0,
8854,16304,8568,11334,15641,15478,2275,6193,
7323,5230,9835,12821,1652,12477,5044,359,
12487,13881,10731,8246,14642,14885,6555,15919,
13857,1982,7646,12528,12009,7310,12310,3193,
359,1483,5322,10476,46,3751,3766,6729,
4512,13853,5558,12736,1733,2718,9956,7857,
6519,2187,12964,12949,750,8877,15136,3742,
4495,13851,3811,906,11592,9943,920,10806,
10442,12561,5035,988,15861,8024,10272,16372,
13353,15946,7330,4834,9414,8267,1668,12498,
4530,9162,8648,3883,9451,3900,2028,12427,
11203,3071,5645,10228,11875,8198,4563,215,
3614,5717,11332,1617,11382,13987,12626,11857,
12156,6513,13496,6997,16158,9093,6725,13737,
5890,10008,14447,5475,12215,141,16192,10200,
4450,6688,11090,16109,12256,9027,15355,3422,
10079,14571,13101,9967,12296,3460,8348,15413,
5222,11762,3823,13412,6729,6599,10528,9922,
11152,9171,5362,5373,750,8969,1652,6220,
5373,2569,3742,15119,10340,999,10088,10731,
1028,3228,7369,15256,11822,12904,8121,11641,
11158,5016,10725,8412,1183,9674,11806,12112,
2623,15516,4226,10021,13316,12075,792,4972,
8506,8921,6133,14718,567,4349,13420,5953,
2265,11902,10019,16265,16312,0,9922,3009,
8170,3473,10757,3567,2825,6915,5174,4299,
0,8274,6292,295,6522,1475,359,4538,
5358,5174,3008,6317,12688,4165,752,12052,
10008,3912,7431,13364,9894,13524,284,16013,
9820,11295,1144,2845,3538,10678,13806,1469,
14210,7870,3766,10528,13642,10823,4200,8031,
4671,14093,5541,1214,4115,3286,8217,8018 };


unsigned int CONV phash( unsigned int move, int turn )
{
  unsigned int a, b;
  
  move |= ( (unsigned int)turn << 31 );
  move += 0x69fe378e;
  move ^= ( move >> 16 );
  move += ( move << 8 );
  move ^= ( move >> 4 );
  b     = ( move >> 7 ) & 0x7ff;
  a     = move >> 18;
  
  return a ^ tab[b];
}
