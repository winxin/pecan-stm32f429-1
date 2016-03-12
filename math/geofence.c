/**
  * Geofencing algorithm and geofencing data
  */

#include "ch.h"
#include "hal.h"
#include "debug.h"
#include "geofence.h"

const coord_t america[] = {
	// Latitude  Longitude (in deg*10000000)
	{ 602803500,-1800000000},
	{ 527878300,-1800000000},
	{ 475767600,-1798696200},
	{ -14012100,-1800000000},
	{ -11792700,-1627114200},
	{  67268500,-1627024600},
	{  65637300,-1560138200},
	{ -79800100,-1502085700},
	{-109216900,-1502040900},
	{-110023000,-1659320300},
	{-155292800,-1725193600},
	{-444551600,-1725356300},
	{-510311600,-1795750100},
	{-624631500,-1800000000},
	{-622917200, -805076100},
	{-593631200, -532159100},
	{-613268000, -216598500},
	{-523586500, -216531900},
	{-158237300, -216398800},
	{-261370000, -256562300},
	{ 116521800, -379076800},
	{ 250055500, -471176200},
	{ 437701700, -474320100},
	{ 529477400, -475106100},
	{ 592874700, -556751500},
	{ 649970500, -579417200},
	{ 677211100, -595630600},
	{ 703992100, -625906400},
	{ 748557600, -742708000},
	{ 900000000, -749253300},
	{ 900000000,-1800000000},
	{ 751492400,-1800000000},
	{ 684708700,-1696228400},
	{ 655163500,-1697060800},
	{ 641285100,-1733928400}
};

const coord_t china[] = {
	// Latitude  Longitude (in deg*10000000)
	{451738100, 825642300},
	{452352200, 819625200},
	{453544900, 817013800},
	{451756000, 809593900},
	{449717800, 799587900},
	{446579400, 803864200},
	{440643900, 804054400},
	{434967200, 806881200},
	{431645300, 806192500},
	{428305300, 803306500},
	{421086600, 801075700},
	{410720900, 778157100},
	{410197600, 769014100},
	{405847000, 766682700},
	{403815200, 762593500},
	{404379000, 749752300},
	{395074200, 736814000},
	{371897300, 748730300},
	{315530900, 791019900},
	{301780600, 820775400},
	{283590400, 859320000},
	{279363900, 892463900},
	{284084300, 897228300},
	{279579500, 916425000},
	{286829100, 946908200},
	{278888600, 977112800},
	{240215100, 974360200},
	{212269300,1010279300},
	{226236400,1052672100},
	{207971800,1073531600},
	{174983800,1071305300},
	{161966200,1121345000},
	{203309900,1169129400},
	{209227300,1244251400},
	{236120900,1277185900},
	{261685400,1256067700},
	{326694600,1240792500},
	{349869300,1233594400},
	{373804200,1239579800},
	{385913100,1232100700},
	{390679600,1240433600},
	{397587600,1312690000},
	{423981400,1305152200},
	{427504600,1304090500},
	{428757700,1310060100},
	{431897400,1311892000},
	{440344300,1312479400},
	{447908400,1309259800},
	{449651300,1313950300},
	{452936700,1317762000},
	{450233100,1329138800},
	{462123900,1336656800},
	{473018100,1341318300},
	{477423200,1346395600},
	{484421800,1344441700},
	{482578200,1337457700},
	{481039700,1329297000},
	{477062600,1324401400},
	{476285700,1316887800},
	{477875100,1309374300},
	{488034100,1304674300},
	{495930500,1287364200},
	{495253500,1279267800},
	{498558300,1274247600},
	{504821100,1272556800},
	{520086700,1263022800},
	{529189300,1257138500},
	{535240400,1233065100},
	{533144800,1208991700},
	{527595400,1199859800},
	{520423200,1205267200},
	{510068000,1196984600},
	{500606200,1193096500},
	{495287200,1176531300},
	{498308400,1166490900},
	{479328400,1157076200},
	{480844300,1180933300},
	{472134400,1196377400},
	{467122600,1196628800},
	{463993300,1169068900},
	{455875700,1158663400},
	{453936600,1146868800},
	{448262800,1138589800},
	{450878400,1119834600},
	{445395200,1115141900},
	{436638100,1117473200},
	{426459800,1096952900},
	{422650000,1069401300},
	{416851000,1049759900},
	{420205800,1025418600},
	{426135800,1010745200},
	{424921600, 990358900},
	{428878200, 963380800},
	{442724800, 951612100},
	{450616000, 931665300},
	{450499500, 918176200},
	{452861800, 909960600},
	{454215300, 906749700},
	{458940100, 907493900},
	{465557200, 910300600},
	{474407800, 904927700},
	{479146500, 898286200},
	{481795100, 888129100},
	{485449300, 880388900},
	{491957400, 877043300},
	{491705700, 873129800},
	{491552500, 870409600},
	{490895800, 868238800},
	{489435600, 867192900},
	{488602800, 868287300},
	{485646500, 866003400},
	{485116800, 864543400},
	{484311000, 862090400},
	{484087600, 858099400},
	{481666000, 855665300},
	{475765800, 856453300},
	{472641400, 857286700},
	{470697000, 855483500},
	{470438500, 852250700},
	{468830000, 849457300},
	{469921200, 848366900},
	{470261800, 845298900},
	{469969200, 839931900},
	{470275800, 837201600},
	{472396400, 830235700},
	{462430400, 825576900},
	{459932800, 824555100},
	{459642900, 823423500},
	{456150100, 822588400},
	{455358200, 822665200},
	{454102700, 825928100}
};

// http://stackoverflowcom/questions/924171/geo-fencing-point-inside-outside-polygon
/**
  * Determines is location is located in polygon
  * @param poly Polygon
  * @param lat Latitude
  * @param lat Longitude
  */
bool isPointInPolygon(const coord_t *poly, uint32_t size, int32_t lat, int32_t lon) {
	bool c = false;
	uint32_t j = size-1;

	for(uint32_t i=0; i<size; i++) {
		if((((poly[i].lat <= lat) && (lat < poly[j].lat)) || ((poly[j].lat <= lat) && (lat < poly[i].lat))) && (lon < (poly[j].lon - poly[i].lon) * (lat - poly[i].lat) / (poly[j].lat - poly[i].lat) + poly[i].lon))
			c = !c;
		j = i;
	}

	return c;
}

/**
  * Determines if point is located in America
  * @param lat Latitude in deg*10000000
  * @param lat Longitude in deg*10000000
  */
bool isPointInAmerica(int32_t lat, int32_t lon) {
	isPointInPolygon(america, sizeof(america)/sizeof(america[0]), lat, lon);
	return false;
}

/**
  * Determines if point is located in China
  * @param lat Latitude in deg*10000000
  * @param lat Longitude in deg*10000000
  */
bool isPointInChina(int32_t lat, int32_t lon) {
	isPointInPolygon(china, sizeof(america)/sizeof(america[0]), lat, lon);
	return false;
}
