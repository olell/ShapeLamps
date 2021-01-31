#pragma once

// Loglevels
#define LOGLEVEL_DEBUG 0
#define LOGLEVEL_INFO  1
#define LOGLEVEL_WARN  2
#define LOGLEVEL_ERROR 3
#define LOGLEVEL_FATAL 4
#define LOGLEVEL_OFF   5

// WiFi behaviors
// Just opens an access point
#define WIFI_AP_ONLY     0
// Opens an access point for configuration
#define WIFI_AP_CONFIG   1
// Connects to a configurated access point
#define WIFI_CONFIG_CRED 2

// Network stuff
#define DNS_PORT 53
#define HTTP_PORT 80
#define UDP_PORT 1234

// Math stuff
#ifndef M_PI
#define M_PI 3.1415926
#endif

// Macros
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define random_float() (random(0, 1000) / 1000.0)

// Gamma correction based on adafruits quickfix
const double gamma8[] = {0.0000000000,0.0000001827,0.0000012722,0.0000039594,0.0000088604,0.0000165502,0.0000275747,0.0000424582,0.0000617077,0.0000858157,0.0001152624,0.0001505175,0.0001920415,0.0002402862,0.0002956964,0.0003587096,0.0004297575,0.0005092658,0.0005976549,0.0006953406,0.0008027337,0.0009202409,0.0010482648,0.0011872042,0.0013374544,0.0014994069,0.0016734505,0.0018599704,0.0020593491,0.0022719663,0.0024981989,0.0027384211,0.0029930049,0.0032623198,0.0035467327,0.0038466088,0.0041623107,0.0044941992,0.0048426331,0.0052079690,0.0055905620,0.0059907651,0.0064089297,0.0068454054,0.0073005401,0.0077746802,0.0082681705,0.0087813542,0.0093145732,0.0098681676,0.0104424765,0.0110378373,0.0116545862,0.0122930580,0.0129535863,0.0136365034,0.0143421404,0.0150708272,0.0158228925,0.0165986638,0.0173984675,0.0182226290,0.0190714725,0.0199453211,0.0208444971,0.0217693215,0.0227201144,0.0236971950,0.0247008814,0.0257314908,0.0267893395,0.0278747430,0.0289880155,0.0301294708,0.0312994215,0.0324981794,0.0337260557,0.0349833605,0.0362704031,0.0375874921,0.0389349354,0.0403130398,0.0417221117,0.0431624565,0.0446343789,0.0461381829,0.0476741719,0.0492426484,0.0508439142,0.0524782705,0.0541460177,0.0558474558,0.0575828839,0.0593526003,0.0611569030,0.0629960892,0.0648704555,0.0667802977,0.0687259112,0.0707075908,0.0727256305,0.0747803238,0.0768719638,0.0790008427,0.0811672524,0.0833714840,0.0856138283,0.0878945754,0.0902140147,0.0925724353,0.0949701258,0.0974073740,0.0998844674,0.1024016929,0.1049593370,0.1075576854,0.1101970236,0.1128776366,0.1155998087,0.1183638238,0.1211699654,0.1240185164,0.1269097594,0.1298439763,0.1328214488,0.1358424580,0.1389072844,0.1420162084,0.1451695096,0.1483674675,0.1516103608,0.1548984681,0.1582320673,0.1616114361,0.1650368517,0.1685085907,0.1720269297,0.1755921445,0.1792045106,0.1828643032,0.1865717971,0.1903272665,0.1941309854,0.1979832275,0.2018842657,0.2058343731,0.2098338219,0.2138828843,0.2179818318,0.2221309359,0.2263304673,0.2305806968,0.2348818944,0.2392343301,0.2436382734,0.2480939934,0.2526017588,0.2571618382,0.2617744997,0.2664400110,0.2711586395,0.2759306523,0.2807563163,0.2856358977,0.2905696627,0.2955578772,0.3006008064,0.3056987156,0.3108518695,0.3160605327,0.3213249692,0.3266454431,0.3320222178,0.3374555565,0.3429457222,0.3484929776,0.3540975849,0.3597598062,0.3654799032,0.3712581374,0.3770947699,0.3829900616,0.3889442730,0.3949576644,0.4010304958,0.4071630269,0.4133555171,0.4196082256,0.4259214112,0.4322953325,0.4387302478,0.4452264152,0.4517840925,0.4584035371,0.4650850062,0.4718287569,0.4786350458,0.4855041293,0.4924362637,0.4994317049,0.5064907084,0.5136135298,0.5208004241,0.5280516463,0.5353674509,0.5427480924,0.5501938250,0.5577049024,0.5652815783,0.5729241062,0.5806327392,0.5884077302,0.5962493319,0.6041577966,0.6121333767,0.6201763241,0.6282868904,0.6364653273,0.6447118858,0.6530268171,0.6614103720,0.6698628010,0.6783843544,0.6869752825,0.6956358349,0.7043662615,0.7131668117,0.7220377346,0.7309792793,0.7399916945,0.7490752288,0.7582301306,0.7674566480,0.7767550289,0.7861255209,0.7955683717,0.8050838284,0.8146721381,0.8243335476,0.8340683037,0.8438766528,0.8537588410,0.8637151144,0.8737457189,0.8838509001,0.8940309033,0.9042859739,0.9146163567,0.9250222967,0.9355040385,0.9460618264,0.9566959048,0.9674065176,0.9781939086,0.9890583216,1.0000000000};