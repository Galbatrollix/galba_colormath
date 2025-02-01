#include "tests.h"

#include "galba_colormath.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>



static double round_to_n_digits(double x, unsigned int digits) {
    double fac = pow(10, digits);
    return round(x* fac)/ fac;
}
static void print_lab(lab_t p){
    printf("lab(%lf %lf %lf)", p.l, p.a, p.b);
}


#define CIEDE2000_TEST_COUNT 40

const lab_t CIEDE2000_TEST_INPUTS[CIEDE2000_TEST_COUNT][2] = {
	// Test values taken from:
	// 1 . https://hajim.rochester.edu/ece/sites/gsharma/ciede2000/
	{{50.0000, 2.6772, -79.7751}, {50.0000, 0.0000, -82.7485}},
	{{50.0000, 3.1571, -77.2803}, {50.0000, 0.0000, -82.7485}},
	{{50.0000, 2.8361, -74.0200}, {50.0000, 0.0000, -82.7485}},
	{{50.0000, -1.3802, -84.2814}, {50.0000, 0.0000, -82.7485}},
	{{50.0000, -1.1848, -84.8006}, {50.0000, 0.0000, -82.7485}},
	{{50.0000, -0.9009, -85.5211}, {50.0000, 0.0000, -82.7485}},
	{{50.0000, 0.0000, 0.0000}, {50.0000, -1.0000, 2.0000}},
	{{50.0000, -1.0000, 2.0000}, {50.0000, 0.0000, 0.0000}},
	{{50.0000, 2.4900, -0.0010}, {50.0000, -2.4900, 0.0009}},
	{{50.0000, 2.4900, -0.0010}, { 50.0000, -2.4900, 0.0010}},
	{{50.0000, 2.4900, -0.0010}, { 50.0000, -2.4900, 0.0011}},
	{{50.0000, 2.4900, -0.0010}, { 50.0000, -2.4900, 0.0012}},
	{{50.0000, -0.0010, 2.4900}, { 50.0000, 0.0009, -2.4900}},
	{{50.0000, -0.0010, 2.4900}, { 50.0000, 0.0010, -2.4900}},
	{{50.0000, -0.0010, 2.4900}, { 50.0000, 0.0011, -2.4900}},
	{{50.0000, 2.5000, 0.0000}, { 50.0000, 0.0000, -2.5000}},
	{{50.0000, 2.5000, 0.0000}, { 73.0000, 25.0000, -18.0000}},
	{{50.0000, 2.5000, 0.0000}, { 61.0000, -5.0000, 29.0000}},
	{{50.0000, 2.5000, 0.0000}, { 56.0000, -27.0000, -3.0000}},
	{{50.0000, 2.5000, 0.0000}, { 58.0000, 24.0000, 15.0000}},
	{{50.0000, 2.5000, 0.0000}, { 50.0000, 3.1736, 0.5854}},
	{{50.0000, 2.5000, 0.0000}, { 50.0000, 3.2972, 0.0000}},
	{{50.0000, 2.5000, 0.0000}, { 50.0000, 1.8634, 0.5757}},
	{{50.0000, 2.5000, 0.0000}, { 50.0000, 3.2592, 0.3350}},
	{{60.2574, -34.0099, 36.2677}, { 60.4626, -34.1751, 39.4387}},
	{{63.0109, -31.0961, -5.8663}, { 62.8187, -29.7946, -4.0864}},
	{{61.2901, 3.7196, -5.3901}, { 61.4292, 2.2480, -4.9620}},
	{{35.0831, -44.1164, 3.7933}, { 35.0232, -40.0716, 1.5901}},
	{{22.7233, 20.0904, -46.6940}, { 23.0331, 14.9730, -42.5619}},
	{{36.4612, 47.8580, 18.3852}, { 36.2715, 50.5065, 21.2231}},
	{{90.8027, -2.0831, 1.4410}, { 91.1528, -1.6435, 0.0447}},
	{{90.9257, -0.5406, -0.9208}, { 88.6381, -0.8985, -0.7239}},
	{{6.7747, -0.2908, -2.4247}, { 5.8714, -0.0985, -2.2286}},
	{{2.0776, 0.0795, -1.1350}, { 0.9033, -0.0636, -0.5514}},

	// 2. hand picked and cross referenced with websites, including color.js implementation
	{{0,0,0},{0,0,0}},
	{{99.9751541, -0.1694255, 0.465877863506425},{ 100, 0.005260499, -0.0104081845252}},
	{{100.0, -125.0, 125.0},{ 100.0, 125.0, 125.0}},

							// slightly out of bounds inputs
	{{100.01, -125.01, 125.01},{ 100.01, 125.01, 125.01}},      
	{{0.0 ,-125, 125},{100.000001,125.000001,125.000001}},
	{{-0.000001,-125.000001,125.000001},{100,125,125}},                               
};

const double CIEDE2000_TEST_EXPECTED[CIEDE2000_TEST_COUNT] = {
	// 1. 
	2.0425, 2.8615, 3.4412, 1.0000, 1.0000, 1.0000, 2.3669, 2.3669, 
	7.1792, 7.1792, 7.2195, 7.2195, 4.8045, 4.8045, 4.7461, 4.3065, 
	27.1492, 22.8977, 31.9030, 19.4535, 1.0000, 1.0000, 1.0000, 1.0000, 
	1.2644, 1.2630, 1.8731, 1.8645, 2.0373, 1.4146, 1.4441, 1.5381, 0.6377, 0.9082, 
	// 2.
	0.0, 0.537679, 94.77616, 94.7790, 137.77707 ,137.77707

};




bool test_CIEDE2000_premade_data(void){

	for(int i=0; i<CIEDE2000_TEST_COUNT; i++){
		double expected = CIEDE2000_TEST_EXPECTED[i];
		double calculated = delta_CIEDE2000(CIEDE2000_TEST_INPUTS[i][0], CIEDE2000_TEST_INPUTS[i][1]);
		double calculated_swap = delta_CIEDE2000(CIEDE2000_TEST_INPUTS[i][1], CIEDE2000_TEST_INPUTS[i][0]);

		expected = round_to_n_digits(expected, 4);
		calculated = round_to_n_digits(calculated, 4);
		calculated_swap = round_to_n_digits(calculated_swap, 4);


		// yes, comparing floats for equality IS DEFINED and PERFECTLY fine. 
		// In this case numbers are rounded in the same way so they are eihter identical or the function returned incorrectly
		bool correct =  expected == calculated && calculated == calculated_swap;
		if(! correct){
			printf("Test test_CIEDE2000_premade_data FAILED at:\n");
			print_lab(CIEDE2000_TEST_INPUTS[i][0]);
			printf(", ");
			print_lab(CIEDE2000_TEST_INPUTS[i][1]);
			printf("\n");

			printf("Expected: %.4lf, calculated: %.4lf, calculated with arguments swapped: %.4lf\n", 
														expected, calculated, calculated_swap);
			return false;
		}

	}

	puts("SUCCESS");
	return true;


}


#define CIEDE2000_TEST_COUNT_NONDEFAULT 51
#define CIEDE2000_PARAMS_NONDEFAULT ((const CIEDE2000_params){.KL = 1.3, .KC = 1.5, .KH = 0.9})


// Used colorjs implementation as a reference for these values
const lab_t CIEDE2000_TEST_INPUTS_NONDEFAULT[CIEDE2000_TEST_COUNT_NONDEFAULT][2] = {
	{{0,0,0}, {0,0,0}},
	{{19.074259708006924, 1.961511990585862, -12.62824008474858}, {41.18203772557023, -85.66476680604316, 13.28278275039093}},
	{{86.96849938157338, 11.732725030909705, -80.141939388551}, {59.660931407443485, -94.88031231734968, -97.4949004103237}},
	{{95.49241851009332, 86.15954362497132, 9.03652044019290}, {12.272447057256585, 66.30476531815876, -97.0518986437556}},
	{{8.874243281424587, -88.02778963753899, 10.98650839657443}, {98.4597146597517, -31.87601857599556, 97.2277146509022}},
	{{97.24330824953475, -53.48255235230381, -121.6977955216243}, {54.60476878881316, -109.11190101526266, 2.102149589313654}},
	{{29.269244050056066, -68.46000302117605, -53.7090871623873}, {50.535891499450905, -91.66940371914089, -10.83708942493815}},
	{{4.053110683746485, 46.76291952887837, -10.2974212680940}, {47.758717173327334, -88.42087379638019, -97.5238031480408}},
	{{75.63369634672684, 3.7744826120633945, -2.32683586900694}, {34.28452009911891, -74.35741643948853, 85.2575083151657}},
	{{76.57489827836899, -93.96579599384697, -42.9919178551268}, {41.55391214814526, 41.33745062809186, 3.442572080293871}},
	{{26.97951281486435, -57.924389712402856, 11.3254670687}, {28.620942229799528, -97.07307461800914, -3.811597997365055}},
	{{24.543271030270684, 66.10771820485243, -45.73834230556120}, {24.377487865384296, 83.84652525357541, -66.3058794249391}},
	{{94.14821821202666, 77.57960626738415, 67.9548063841644}, {58.294365100900805, -96.51009009310724, 62.0725644870190}},
	{{38.25635365207516, -64.26358792571163, -43.51363102875579}, {75.38098501624731, -61.548507431090165, 85.1826448952169}},
	{{22.5590373264838, 27.78326934251288, 53.4053501974406}, {90.12981181851573, 118.40658938109965, -1.66200924786730}},
	{{68.05181652566291, -25.42506196571354, -37.848783506147}, {47.89901819798851, 15.694015679652239, 114.1173268816944}},
	{{42.803648848231504, -59.424699739424625, -91.1862875806500}, {82.71077514932664, 118.46441031672887, -11.73356981302431}},
	{{47.95493608450075, -60.3206888069858, -75.2259328892227}, {49.74243758364276, 57.03350447603367, -88.6049182949232}},
	{{24.25952239902123, -51.32940136204353, -39.444690028699}, {38.563651409881274, 107.8783815211024, -93.9398819718997}},
	{{87.58085022324322, 97.15829020711124, 83.6819255808225}, {15.6698972273006, 98.95470748137038, 122.9927884272891}},
	{{9.274659954087404, -60.03300365361747, 75.2973813900416}, {32.620052328824144, 48.753019653305415, -79.2551318623355}},
	{{85.5414296241987, 87.89973014909987, -58.1566227146646}, {92.03090320260739, -71.20205971979945, -121.9430492634366}},
	{{43.241763731815965, -77.01501823768803, -105.5447059065166}, {69.19416574446731, -85.95032642892068, -119.604144859384}},
	{{96.55047241636353, -117.44848903160074, -69.2142437264166}, {97.66485989030632, -40.368091586364486, 28.009099166490}},
	{{90.62523024733864, 14.430172961794625, -10.67781448882912}, {64.7915872869482, -79.12201445560754, 60.9684353141272}},
	{{30.295524680567098, 83.34820127012281, -40.70398891202755}, {7.901312233387658, -107.75313312868923, -91.5681072780951}},
	{{25.036124919339077, -7.562601853093852, 88.5431745654273}, {56.405013744363075, 40.78397484822406, 27.95705285474181}},
	{{4.780743748284233, -14.185187917688907, -81.1569304922229}, {78.07473085204747, 108.92403242323456, -59.0554351606465}},
	{{30.070354310839964, -25.065905190819336, -122.5539487754832}, {77.24765791480377, -25.991481738599447, -3.80587833968002}},
	{{17.968995744891725, 27.69930191289353, -111.479565240305}, {86.1022931469558, -47.083045590791514, 117.9646410255389}},
	{{52.38247028678493, -112.38237232558329, 12.8690547444801}, {78.10643143575535, -117.97703326592412, 20.14878350506941}},
	{{9.35463151619218, 2.3398216501176705, -113.2715632387381}, {16.473498352808168, -89.15466533688723, -113.6097684981163}},
	{{80.85436266156034, -122.02684068520786, 44.50384341315026}, {84.46779323179585, -5.387394694797678, 19.82142342873649}},
	{{88.73876819749913, 110.45857747440502, 69.4811914018477}, {52.99218129044203, 102.64504129597228, -58.3339870300628}},
	{{29.579628249903756, -64.80667805584503, 94.2609584892787}, {90.02173146583179, -98.33034062715407, -97.395092928647}},
	{{77.01230014315263, 53.018126679862064, 123.624961807909}, {18.081956043174085, 18.42867843109738, 92.1801788813726}},
	{{70.45663267352118, -27.443341521105864, -54.99578761098524}, {12.17152307115149, -0.37003530982818233, -121.8679637341669}},
	{{81.41289408050235, 23.385114720586927, -36.40353064776813}, {99.27010393072356, -15.46632501871234, 18.14965705535746}},
	{{88.30895572311717, 34.32299526814293, 95.5089056528551}, {89.19290025814726, 102.25501412817854, -45.20081720820884}},
	{{35.749890143532824, 70.76727375234083, -46.1464261827290}, {26.603886031408365, -109.6220006936395, 113.3380605852002}},
	{{92.61337830983464, 51.66403581072407, -113.9262777222185}, {95.75984658759793, -12.325601760341755, -72.0811343204047}},
	{{19.010009544454043, -7.892944270444573, 113.9294917535887}, {71.25715551004807, 95.65330427449081, 29.71466247228542}},
	{{93.07413446952575, 80.6592569504217, -80.9634823721437}, {88.84631640290004, -115.92271129456591, 104.9102362581210}},
	{{45.347654476762855, 106.7386902258936, 1.684905340219145}, {47.168963639976624, 124.06428133453778, -107.4560284234142}},
	{{56.872607669164225, -0.5252013683366101, 74.0842905704614}, {48.018232608751546, -34.98865299936027, -46.9436481815662}},
	{{82.97831485518981, -111.18446252136785, 72.566003506806}, {25.259830266528294, -18.787156389410896, 109.1093783637807}},
	{{75.59044111102538, -99.9053130868266, 29.30848846585678}, {57.381099807995675, -94.4824047903452, -120.6265768697670}},
	{{47.62154598005761, 40.229894736440656, 120.7313306542937}, {8.749343337750215, -41.89573570101463, 88.1095643727403}},
	{{89.84322300957567, -13.792322718593766, 63.8488425048231}, {88.36861596118001, 124.58125630118968, 98.8190422905426}},
	{{38.014579825704075, -20.55966039351425, -89.6370678637382}, {68.60246382789741, 49.504731858182964, -55.42414856682684}},
	{{99.40319140154949, 33.65668148405442, 19.22922551937608}, {15.638910934930639, -54.52713136510987, -123.0114701971984}},


};

const double CIEDE2000_TEST_EXPECTED_NONDEFAULT[CIEDE2000_TEST_COUNT_NONDEFAULT] = {
	0,
	34.839852625998056, 39.3642719410122, 72.10234674904575, 76.73378728873477, 50.85100584467276, 
	26.371659800875953, 113.1059594956131, 47.735576633715226, 110.98643756282158, 10.533837935261694, 
	4.31040703632919, 95.55534254093203, 61.4903950834801, 68.21205679756577, 64.14425250519767, 
	131.63061164051803, 57.61265869722471, 67.97746235829895, 56.1721030314811, 74.01590718523583, 
	108.96356140093961, 18.62129067815126, 42.53766336466819, 63.536443405422006, 116.34860764810941, 
	50.8959315554567, 93.98831064489167, 46.76514099407472, 108.27161441428743, 16.429146734833903, 
	28.4927146356742, 27.105797891403878, 46.43937918714207, 83.04692980497722, 46.602380726950926, 
	42.50801525247751, 43.55757030840121, 70.88460753748487, 117.22051879190725, 29.99620063955653, 
	82.63613815522058, 72.8621483562153, 27.550562502913227, 62.605669959276995, 54.95555451855282, 
	55.136017713331164, 49.83807124531308, 61.11270949756064, 62.87507930371434, 91.93108660576064 
};

bool test_CIEDE2000_params_nondefault(void){
	for(int i=0; i<CIEDE2000_TEST_COUNT_NONDEFAULT; i++){
		double expected = CIEDE2000_TEST_EXPECTED_NONDEFAULT[i];

		double calculated = delta_CIEDE2000_full(
			CIEDE2000_TEST_INPUTS_NONDEFAULT[i][0],
			CIEDE2000_TEST_INPUTS_NONDEFAULT[i][1],
			CIEDE2000_PARAMS_NONDEFAULT
		);
		double calculated_swap = delta_CIEDE2000_full(
			CIEDE2000_TEST_INPUTS_NONDEFAULT[i][1],
			CIEDE2000_TEST_INPUTS_NONDEFAULT[i][0],
			CIEDE2000_PARAMS_NONDEFAULT
		);

		expected = round_to_n_digits(expected, 4);
		calculated = round_to_n_digits(calculated, 4);
		calculated_swap = round_to_n_digits(calculated_swap, 4);

		bool correct =  expected == calculated && calculated == calculated_swap;
		if(! correct){
			printf("Test test_CIEDE2000_params_nondefault FAILED at:\n");
			print_lab(CIEDE2000_TEST_INPUTS_NONDEFAULT[i][0]);
			printf(", ");
			print_lab(CIEDE2000_TEST_INPUTS_NONDEFAULT[i][1]);
			printf("\n");

			printf("Expected: %.4lf, calculated: %.4lf, calculated with arguments swapped: %.4lf\n", 
														expected, calculated, calculated_swap);
			return false;
		}

	}

	puts("SUCCESS");
	return true;


}


#define CMC_TEST_COUNT_2_1 50

const lab_t CMC_TEST_INPUTS_2_1[CMC_TEST_COUNT_2_1][2] = {
	{{74.58598894266014, 74.7508454554301, -39.0357893276908}, {95.50546391827288, 89.83733171690977, -121.9687302740398}},
	{{55.560285526327505, 110.64656963863362, 115.6457591745203}, {22.973725496283937, 100.32815546422603, 116.2919803432478}},
	{{73.13545385913352, -36.28037398420011, -53.3415544505332}, {59.16658352868273, 92.67351855447683, -3.61748982442209}},
	{{34.11877374471328, -82.71969065131032, 4.87266173139357}, {61.67935385323127, -3.5565711831159206, 20.4396310345815}},
	{{87.024122681321, -34.71280227003021, -121.3397227931685}, {4.640835409287325, -17.807071750871955, 98.0743670983053}},
	{{53.27692929241853, 80.65746298429215, 124.2006837290610}, {80.36956008281774, -51.99900023561361, 19.93930408768923}},
	{{36.07748313744881, 76.73187305710289, 80.604453159826}, {11.633924080435133, 109.12956208335922, 40.1678065284298}},
	{{20.318129578554277, -6.9155297472331085, -119.0830237028242}, {87.41232918146873, -102.61950148535672, 20.01564007886045}},
	{{64.84440561239332, 34.003141159263265, 120.405630080118}, {30.623993672078754, 73.6780916064539, -72.3531225689074}},
	{{91.9366825534645, 69.91730482355246, 100.4807616579149}, {62.51624632075848, -103.13428886747661, 79.3848739681852}},
	{{21.245687518206545, 30.762249485195895, 58.0955745789444}, {90.81757090554433, -83.63334804445947, -57.65487092217918}},
	{{58.22875990032658, -124.30010355239743, -124.0434017581350}, {37.03687508122051, 2.227699304542952, 85.5199893245467}},
	{{64.02177082388157, 112.14309312706774, 111.1482206687690}, {20.874924729818854, -35.232245154126176, 36.34639579661387}},
	{{24.585073238373113, -15.478853378637467, 37.3826447573743}, {56.32788300304252, 95.75510007361913, -2.63130990707830}},
	{{60.276077794307106, 124.97746412072937, 5.85116746799320}, {70.69005627078792, 69.35779062709418, 104.2753685202320}},
	{{40.9514146914015, -66.5257804209797, 52.986009877575}, {82.46937040144996, 3.213033098917606, 4.75830304853710}},
	{{50.32616860438674, 80.99919493725704, -42.4526135660087}, {48.41508384094683, -11.697613063278624, -47.3582708572083}},
	{{62.13161343309837, -16.518463517543935, -49.73242566436475}, {3.4465775929292897, 116.43136390726738, -111.9908417908247}},
	{{88.16434824096771, 41.088684809768836, -26.26143360678689}, {21.94657863438192, -33.831360293929436, 49.43848005748793}},
	{{25.255254533593718, -17.985633151448766, 123.9125949674910}, {24.696912317381525, 124.05051195869876, 25.51731130487647}},
	{{7.9209368122081125, 45.6627308825139, -121.1141128150258}, {33.17054342004384, 14.053069087533316, -102.4804359460399}},
	{{28.672266831548875, -38.29726207416536, -61.76311852622015}, {27.705691800801034, 97.01437886307903, -3.237778993426388}},
	{{22.603834200247796, -91.89087435599578, 91.1250036956963}, {57.99725935289932, 115.3927402130534, 22.37865258919299}},
	{{47.551853758356955, 88.2620762954721, 114.759260612570}, {20.782672218488674, 107.8666289180363, 81.8363568017620}},
	{{29.69015346632773, 108.29593345820953, 110.3458023754803}, {46.66206867394857, 91.15610378234672, -27.75442775063032}},
	{{74.64310371177551, -21.494598580999565, -45.41371001854608}, {84.0954976853373, -69.0094177939462, -52.63234696890053}},
	{{26.32767272952995, -6.56123264422898, 110.1193198456893}, {78.81373081873771, 27.77683198210019, 34.5410678892590}},
	{{74.05858899235723, 68.25779007399316, 77.468451155079}, {67.0528857891856, 59.187538203683005, 73.3704484565766}},
	{{21.6602584912876, 65.6849334932559, -38.4257954778078}, {20.862913987340292, -97.8025891136019, 87.453433850792}},
	{{38.191496494895794, 58.67606319221315, -9.22189921947985}, {81.64876063977353, -11.224380215810982, 8.91891256802125}},
	{{90.05092801224455, -12.94813114526319, 100.4135046896738}, {38.030093217886495, 51.88633801373783, 65.5098931193857}},
	{{94.68084785705445, 109.08612880881796, 43.4814808360944}, {14.005877557907265, 57.94745914883916, 98.0665033104785}},
	{{24.589656792866833, -115.43044535984009, 80.2136491614248}, {79.95437347763492, -19.217013510069762, 10.89320415647662}},
	{{15.502038087469906, 82.65084819119, 98.2293628933249}, {34.01360248260934, 92.76036416844937, -43.66409709914178}},
	{{0.6833552389811981, 43.195874079154606, 37.3122834566288}, {27.181578914237026, -91.72451209395832, 117.6385508282748}},
	{{92.6936761706512, 52.027605400932316, -79.4157567137981}, {22.808944264308884, -83.96113216850986, 81.5810978629342}},
	{{30.673247315798836, 21.946563363316614, 91.5684728443562}, {81.21854440155114, 52.76763993410208, -99.4863678690167}},
	{{70.24216025182002, -116.88135658110593, 74.2822399199684}, {29.45893124783411, 93.121566789778, 74.9370240080181}},
	{{52.97996462289065, -30.8389196699613, 12.53857802747904}, {48.94838505682606, -111.68774204454272, -57.1230559099005}},
	{{46.59847482966717, 115.67875348390783, 93.513778802545}, {80.25226495670701, -85.47308770422453, -46.1647333856474}},
	{{19.367503117667617, 40.74234609060349, -17.80747745158758}, {29.330226628725953, 18.16209854920632, 114.760601929549}},
	{{51.56129186893891, 59.94740063085419, -107.3183828432035}, {42.5330066452399, 114.24560773768715, 52.52874380929523}},
	{{72.55881589495526, -82.65545381137227, -109.9910635503530}, {7.525713131452894, 53.40933152126544, -107.7548768489864}},
	{{7.744877939301409, -46.13590584866117, 70.6741409304997}, {2.6520083817354934, 20.842612534861814, -69.4082378385110}},
	{{42.83203204717564, -72.4718378903563, -113.3072580998598}, {16.78027491985299, -14.043024808593131, 69.8831179685951}},
	{{49.60473907843183, -30.418980700708303, 123.8082645106100}, {23.05421135703113, -65.4565778791125, 11.81669780132094}},
	{{51.675708885161875, -18.993250992230088, 48.04490189441770}, {13.398607065717927, -115.45118880090604, -53.9623893534076}},
	{{31.955840637244858, 55.97822994002473, 65.840043345875}, {80.85594369860158, -52.428995866177715, 49.78118079924}},
	{{47.89746321015455, 5.895144657184034, -3.84546003330848}, {69.21356874417032, 108.30052312015397, -83.1638373690900}},
	{{76.72442015696058, -11.427516482643256, -6.71129415880039}, {84.08419138288718, -69.19057176775512, 20.41650317195390}},
};

const double CMC_TEST_EXPECTED_2_1[CMC_TEST_COUNT_2_1] = {
	31.680360811008686, 15.07926986757705, 67.88237007974074, 33.123960594981305, 95.85326077887696, 
	101.62071230460249, 37.40093633490103, 93.81253470281355, 109.62500962686208, 132.96964871883662, 
	145.43192307476716, 81.2066458090543, 81.14638360061346, 72.2939302398125, 45.827338769658844, 
	34.91031508268723, 37.39750417230786, 74.3634708841243, 64.04598738475217, 74.37548542632501, 
	28.288908032966308, 69.55969873530962, 82.62415147818484, 28.734166474213932, 78.07960208052724, 
	23.071569931999914, 46.64440134099411, 4.84002709503579, 93.26461788732149, 40.0961663379767, 
	38.28736011819605, 45.41708707413017, 50.3032199667374, 97.7668285933205, 110.56894904685117, 
	102.42219481498768, 118.49893175613171, 74.76216524828044, 55.198441885902554, 133.55740196526685, 
	71.66237445433946, 74.96626764709453, 56.39283464194246, 66.55015365971123, 72.43917732637402, 
	46.03169469670125, 73.654928609151, 89.80515896927801, 123.88305700061926, 49.5830286122819, 
};


bool test_CMC_params_2_1(void){
	for(int i=0; i<CMC_TEST_COUNT_2_1; i++){
		double expected = CMC_TEST_EXPECTED_2_1[i];
		double calculated = delta_CMC_a(CMC_TEST_INPUTS_2_1[i][0], CMC_TEST_INPUTS_2_1[i][1]);

		expected = round_to_n_digits(expected, 4);
		calculated = round_to_n_digits(calculated, 4);



		// yes, comparing floats for equality IS DEFINED and PERFECTLY fine. 
		// In this case numbers are rounded in the same way so they are eihter identical or the function returned incorrectly
		bool correct =  expected == calculated;
		if(! correct){
			printf("Test test_CMC_params_2_1 FAILED at:\n");
			print_lab(CMC_TEST_INPUTS_2_1[i][0]);
			printf(", ");
			print_lab(CMC_TEST_INPUTS_2_1[i][1]);
			printf("\n");

			printf("Expected: %.4lf, calculated: %.4lf\n", expected, calculated);
			return false;
		}

	}

	puts("SUCCESS");
	return true;
}


#define CMC_TEST_COUNT_1_1 50

const lab_t CMC_TEST_INPUTS_1_1[CMC_TEST_COUNT_1_1][2] = {
	{{51.63923832038837, 71.9703561924367, -15.90966089837861}, {12.438059888704933, -104.91382798545787, -122.270623251367}},
	{{89.60686338406673, -15.349109451194224, -119.6642311583448}, {91.5324235452831, -100.17871604132932, 98.697964413246}},
	{{94.48269922097376, -77.50975018589118, -122.9734850005873}, {48.55562128883093, -117.09752753512146, 78.4442307106417}},
	{{80.40750544261421, -39.41446079178698, -99.2382591626931}, {86.13654263521906, -89.01562015292527, 109.022799966876}},
	{{37.67864490248017, 15.164025898575971, -36.74361778158582}, {8.190503080828648, -107.63954664203214, 71.228309778262}},
	{{99.40163886104801, 56.373450273125115, -16.4271799088507}, {73.41935096595303, -7.415135066159593, -76.213514361380}},
	{{37.12107592430627, 120.92360204738281, -84.6885540169457}, {96.3260638721167, 124.87916772570264, -95.6482697402070}},
	{{32.40619326808548, -100.32823168711799, -61.2193569483757}, {48.051501926020144, 118.12838638110495, -33.3507583202963}},
	{{48.58749096334688, 90.02265616441937, -53.9186314557518}, {39.933647652974756, -82.81577122465356, 124.0520158924629}},
	{{98.89923112811141, -81.04645544978342, 93.307134465099}, {47.76102118847798, 60.30460507407105, 103.5690420573347}},
	{{44.87636815954767, -80.74518267534324, -41.5527052219627}, {39.91634912140823, 43.69734487817573, -39.82472924196015}},
	{{50.98463554395221, -43.025137367122994, -104.9354522847509}, {74.31891740147536, -89.03438157268154, 123.5486044855343}},
	{{99.51810975214684, 65.22867517297098, -86.4577084269330}, {4.34330273058211, -44.603752903546535, -87.4002921377028}},
	{{36.367483835041234, -92.60446241967391, -34.10963217494483}, {8.853560070149346, -26.420242637404996, 28.90509756006221}},
	{{41.53380503857347, 65.2904113416813, 47.506075898397}, {12.132585453200662, -119.4685464859146, 88.3894965604306}},
	{{60.449503648042544, 24.16030811439353, -84.5768940735943}, {97.19474969694082, 36.07513127699076, -69.7530267285126}},
	{{10.568899439774748, -75.00596070275179, 50.7310592554867}, {14.734873480628973, -3.146911455913056, 12.9424558775765}},
	{{75.67684166085454, 111.01724766530756, 68.8214502237976}, {14.540620030694262, 104.84157378401028, -88.2947650422692}},
	{{69.4944455102651, 59.268223823358625, -16.38446825971496}, {23.070155411356453, -43.9537660787564, 124.072594745059}},
	{{32.134564175549244, -58.148153911973594, -45.9943754316750}, {80.84284128859854, 33.42444411065412, 0.184479761500185}},
	{{63.476893384001556, -58.16674633019147, -29.93141286082206}, {4.714488914244019, -70.84793219155253, -78.4382995730177}},
	{{85.19249135659159, -113.13686493651903, -49.3692920595188}, {46.323623332534304, 58.244596523856046, -107.7201381057785}},
	{{68.0757375581185, 6.893137747141765, -107.4192528391970}, {23.179565177244356, -88.03404897748365, 67.7216420446077}},
	{{10.460621990207098, -107.16170739493386, -19.77098571322524}, {56.61537080704636, 112.68152168211364, 55.2219503137339}},
	{{23.615051532851417, 78.69728264788196, 13.12279267432330}, {34.78644026538748, 37.36457335817448, -16.1840367204195}},
	{{87.06701243798322, -39.70388281339035, 87.1213497982351}, {23.283166117477016, 119.02655032705758, -72.2650139449416}},
	{{51.53062067728252, 77.48561180194793, -110.8170613835653}, {69.07451103920981, -107.86192426644664, 7.59225283045782}},
	{{11.595826261867238, 87.73397808068168, -90.0966882876282}, {51.52921583110193, 26.956994729825112, -120.24650899729}},
	{{78.91231515484674, -11.067201422702567, 82.1919660718597}, {71.51215476036191, 32.372361373670174, -43.3972493297216}},
	{{83.8394793226935, 28.28004925556064, 1.303312692065688}, {86.4973130052652, -17.970593490592577, -113.0968998096102}},
	{{80.4881504070296, 4.83651745740508, -45.1433112380563}, {32.88098625668966, 62.38592975629393, -33.34213425585683}},
	{{54.22650617135333, 14.430146541476063, 86.870353914653}, {15.437133042709728, -15.147859394936901, 26.83400268557403}},
	{{30.096364496521556, 120.51634900041478, 29.14212976714310}, {70.41501429063632, 112.91163215019219, 36.77234737479102}},
	{{10.980695222744263, -1.819992722486731, 105.3503272519237}, {35.52090339387743, -69.44924466355195, 116.3416778656274}},
	{{64.5340656617646, 22.99335463238299, 85.8525216540198}, {30.100414957714626, 56.209089749100144, -95.7991697606336}},
	{{55.948896193827544, -85.27867856950571, -76.624077086456}, {88.92884151040438, -41.94634183087986, -31.38394245896894}},
	{{42.25152047835928, 65.24248319483095, 101.4844585515549}, {73.22253136324277, -79.96879131643847, 17.771435335399}},
	{{3.5286804840061303, 85.55691442938667, -45.5747403003219}, {52.683661207383224, 39.931709981883756, 43.3299287277524}},
	{{56.41632803584523, 121.57625119829925, 68.8183552434534}, {62.58480080038291, 73.67460619741385, -0.717771984923430}},
	{{13.931832661004051, -73.84404098717143, 59.9354398848815}, {89.2252993657767, 69.97393424553155, -41.83754927200608}},
	{{17.494578389566474, -22.343920880816682, 16.88082893319747}, {50.716246629288364, -45.78522102667965, 44.891698027079}},
	{{21.984867706627288, -97.13566536104406, -59.4881596356079}, {26.099077109588144, 100.87512987359455, 109.9927821220021}},
	{{72.20813616331188, 19.174291088613757, 24.19159424595571}, {68.66894377166265, 65.77117893668759, 19.5688553770483}},
	{{48.70303380129708, -112.9812354699754, 87.1669275946000}, {22.811892448925274, -94.07878017933862, 40.9103554370833}},
	{{51.21344126470859, -90.57950918617416, 20.1461180107044}, {46.31461771545761, -1.8163658426986444, 68.2918879166142}},
	{{41.30191977432425, -88.10735712422202, 30.87111874723700}, {5.036158388222967, -9.579424235815125, 97.288816950547}},
	{{77.71243519807643, 96.5389014092174, 75.3427643453813}, {90.89719432952414, 66.49932956416069, -111.2396037563179}},
	{{2.109433412664208, 45.56012430903894, 16.88722476234008}, {98.4208764927709, -87.80760376805293, 45.2527319932940}},
	{{77.61030089520989, 77.87974392527207, 1.06729858279113}, {13.994226511324703, -115.84901571707861, -73.1479675375367}},
	{{13.208132562574326, -79.06108517594663, -69.9151922175390}, {85.51548530849836, -13.68119871475237, 59.7304902406594}},
};

const double CMC_TEST_EXPECTED_1_1[CMC_TEST_COUNT_1_1] = {
	96.12508368228045, 103.10816549843298, 82.86662182237505, 91.21544159094104, 111.51410049211461, 
	46.15887006554565, 64.52128474676758, 83.98460625532718, 101.08364313079548, 62.41422174642722, 
	49.85353332489601, 99.17815380339522, 80.6495864342474, 44.263049080760624, 120.55095515823855, 
	32.052140772647256, 26.862308574020133, 93.8628803440001, 88.69019649137722, 72.78601245962514, 
	51.75399688577124, 71.3365157201608, 103.2481780261354, 125.5397980631633, 26.062214284269114, 
	104.77761212670323, 92.85527855836226, 82.92096073456295, 65.625883490536, 79.94594743994794, 
	52.0672857992989, 43.36039060800434, 50.25187818926742, 57.220759930884135, 116.82430400706897, 
	33.617704382597374, 124.30811615787735, 104.24155379112881, 31.84964132559316, 162.8788301226981, 
	63.54007391151801, 97.78060029023234, 39.96922553870218, 28.264364987716856, 40.81393138547167, 
	55.7017049200561, 109.24922625983511, 207.78931093794426, 106.80653923482055, 152.12859857985524,
};

bool test_CMC_params_1_1(void){
	for(int i=0; i<CMC_TEST_COUNT_1_1; i++){
		double expected = CMC_TEST_EXPECTED_1_1[i];
		double calculated = delta_CMC_p(CMC_TEST_INPUTS_1_1[i][0], CMC_TEST_INPUTS_1_1[i][1]);

		expected = round_to_n_digits(expected, 4);
		calculated = round_to_n_digits(calculated, 4);


		// yes, comparing floats for equality IS DEFINED and PERFECTLY fine. 
		// In this case numbers are rounded in the same way so they are eihter identical or the function returned incorrectly
		bool correct =  expected == calculated;
		if(! correct){
			printf("Test test_CMC_params_1_1 FAILED at:\n");
			print_lab(CMC_TEST_INPUTS_1_1[i][0]);
			printf(", ");
			print_lab(CMC_TEST_INPUTS_1_1[i][1]);
			printf("\n");

			printf("Expected: %.4lf, calculated: %.4lf", expected, calculated);
			return false;
		}

	}

	puts("SUCCESS");
	return true;
}




#define CIE94_TEST_COUNT 51

const lab_t CIE94_TEST_INPUTS[CIE94_TEST_COUNT][2] = {
	{{0,0,0}, {0,0,0}},
	{{19.074259708006924, 1.961511990585862, -12.62824008474858}, {41.18203772557023, -85.66476680604316, 13.28278275039093}},
	{{86.96849938157338, 11.732725030909705, -80.141939388551}, {59.660931407443485, -94.88031231734968, -97.4949004103237}},
	{{95.49241851009332, 86.15954362497132, 9.03652044019290}, {12.272447057256585, 66.30476531815876, -97.0518986437556}},
	{{8.874243281424587, -88.02778963753899, 10.98650839657443}, {98.4597146597517, -31.87601857599556, 97.2277146509022}},
	{{97.24330824953475, -53.48255235230381, -121.6977955216243}, {54.60476878881316, -109.11190101526266, 2.102149589313654}},
	{{29.269244050056066, -68.46000302117605, -53.7090871623873}, {50.535891499450905, -91.66940371914089, -10.83708942493815}},
	{{4.053110683746485, 46.76291952887837, -10.2974212680940}, {47.758717173327334, -88.42087379638019, -97.5238031480408}},
	{{75.63369634672684, 3.7744826120633945, -2.32683586900694}, {34.28452009911891, -74.35741643948853, 85.2575083151657}},
	{{76.57489827836899, -93.96579599384697, -42.9919178551268}, {41.55391214814526, 41.33745062809186, 3.442572080293871}},
	{{26.97951281486435, -57.924389712402856, 11.3254670687}, {28.620942229799528, -97.07307461800914, -3.811597997365055}},
	{{24.543271030270684, 66.10771820485243, -45.73834230556120}, {24.377487865384296, 83.84652525357541, -66.3058794249391}},
	{{94.14821821202666, 77.57960626738415, 67.9548063841644}, {58.294365100900805, -96.51009009310724, 62.0725644870190}},
	{{38.25635365207516, -64.26358792571163, -43.51363102875579}, {75.38098501624731, -61.548507431090165, 85.1826448952169}},
	{{22.5590373264838, 27.78326934251288, 53.4053501974406}, {90.12981181851573, 118.40658938109965, -1.66200924786730}},
	{{68.05181652566291, -25.42506196571354, -37.848783506147}, {47.89901819798851, 15.694015679652239, 114.1173268816944}},
	{{42.803648848231504, -59.424699739424625, -91.1862875806500}, {82.71077514932664, 118.46441031672887, -11.73356981302431}},
	{{47.95493608450075, -60.3206888069858, -75.2259328892227}, {49.74243758364276, 57.03350447603367, -88.6049182949232}},
	{{24.25952239902123, -51.32940136204353, -39.444690028699}, {38.563651409881274, 107.8783815211024, -93.9398819718997}},
	{{87.58085022324322, 97.15829020711124, 83.6819255808225}, {15.6698972273006, 98.95470748137038, 122.9927884272891}},
	{{9.274659954087404, -60.03300365361747, 75.2973813900416}, {32.620052328824144, 48.753019653305415, -79.2551318623355}},
	{{85.5414296241987, 87.89973014909987, -58.1566227146646}, {92.03090320260739, -71.20205971979945, -121.9430492634366}},
	{{43.241763731815965, -77.01501823768803, -105.5447059065166}, {69.19416574446731, -85.95032642892068, -119.604144859384}},
	{{96.55047241636353, -117.44848903160074, -69.2142437264166}, {97.66485989030632, -40.368091586364486, 28.009099166490}},
	{{90.62523024733864, 14.430172961794625, -10.67781448882912}, {64.7915872869482, -79.12201445560754, 60.9684353141272}},
	{{30.295524680567098, 83.34820127012281, -40.70398891202755}, {7.901312233387658, -107.75313312868923, -91.5681072780951}},
	{{25.036124919339077, -7.562601853093852, 88.5431745654273}, {56.405013744363075, 40.78397484822406, 27.95705285474181}},
	{{4.780743748284233, -14.185187917688907, -81.1569304922229}, {78.07473085204747, 108.92403242323456, -59.0554351606465}},
	{{30.070354310839964, -25.065905190819336, -122.5539487754832}, {77.24765791480377, -25.991481738599447, -3.80587833968002}},
	{{17.968995744891725, 27.69930191289353, -111.479565240305}, {86.1022931469558, -47.083045590791514, 117.9646410255389}},
	{{52.38247028678493, -112.38237232558329, 12.8690547444801}, {78.10643143575535, -117.97703326592412, 20.14878350506941}},
	{{9.35463151619218, 2.3398216501176705, -113.2715632387381}, {16.473498352808168, -89.15466533688723, -113.6097684981163}},
	{{80.85436266156034, -122.02684068520786, 44.50384341315026}, {84.46779323179585, -5.387394694797678, 19.82142342873649}},
	{{88.73876819749913, 110.45857747440502, 69.4811914018477}, {52.99218129044203, 102.64504129597228, -58.3339870300628}},
	{{29.579628249903756, -64.80667805584503, 94.2609584892787}, {90.02173146583179, -98.33034062715407, -97.395092928647}},
	{{77.01230014315263, 53.018126679862064, 123.624961807909}, {18.081956043174085, 18.42867843109738, 92.1801788813726}},
	{{70.45663267352118, -27.443341521105864, -54.99578761098524}, {12.17152307115149, -0.37003530982818233, -121.8679637341669}},
	{{81.41289408050235, 23.385114720586927, -36.40353064776813}, {99.27010393072356, -15.46632501871234, 18.14965705535746}},
	{{88.30895572311717, 34.32299526814293, 95.5089056528551}, {89.19290025814726, 102.25501412817854, -45.20081720820884}},
	{{35.749890143532824, 70.76727375234083, -46.1464261827290}, {26.603886031408365, -109.6220006936395, 113.3380605852002}},
	{{92.61337830983464, 51.66403581072407, -113.9262777222185}, {95.75984658759793, -12.325601760341755, -72.0811343204047}},
	{{19.010009544454043, -7.892944270444573, 113.9294917535887}, {71.25715551004807, 95.65330427449081, 29.71466247228542}},
	{{93.07413446952575, 80.6592569504217, -80.9634823721437}, {88.84631640290004, -115.92271129456591, 104.9102362581210}},
	{{45.347654476762855, 106.7386902258936, 1.684905340219145}, {47.168963639976624, 124.06428133453778, -107.4560284234142}},
	{{56.872607669164225, -0.5252013683366101, 74.0842905704614}, {48.018232608751546, -34.98865299936027, -46.9436481815662}},
	{{82.97831485518981, -111.18446252136785, 72.566003506806}, {25.259830266528294, -18.787156389410896, 109.1093783637807}},
	{{75.59044111102538, -99.9053130868266, 29.30848846585678}, {57.381099807995675, -94.4824047903452, -120.6265768697670}},
	{{47.62154598005761, 40.229894736440656, 120.7313306542937}, {8.749343337750215, -41.89573570101463, 88.1095643727403}},
	{{89.84322300957567, -13.792322718593766, 63.8488425048231}, {88.36861596118001, 124.58125630118968, 98.8190422905426}},
	{{38.014579825704075, -20.55966039351425, -89.6370678637382}, {68.60246382789741, 49.504731858182964, -55.42414856682684}},
	{{99.40319140154949, 33.65668148405442, 19.22922551937608}, {15.638910934930639, -54.52713136510987, -123.0114701971984}},
};

const double CIE94_TEST_EXPECTED[CIE94_TEST_COUNT] = {
	0.000000000000000,  68.728855146125298,  51.446440001939195,  94.803927994314819,  99.742484231454299, 
	61.818392210697652,  29.923345554457651,  94.898841897919141,  107.911238614610410,  62.489222252883330,  
	14.075994449079944,  6.338959148711031,  77.111136007391337,  69.226876028666553,  83.554829770935399, 
	89.210108893438203,  84.007755827121457,  48.205287420538269,  79.469582434001154,  72.586544137822486, 
 	80.755386898331992,  65.569158833860385,  26.066933919432703,  31.479352362628795,  84.694656746969017,  
  	83.753582991054756,  43.180435283635589,  90.744637158833129,  54.516265847544027,  111.714569516912150, 
    25.856213030872951,  33.055030087442034,  22.975805795158095,  56.020064880701028,  93.395746316989573, 
    59.712038157605221,  63.841137876458987,  43.279639869120139,  61.845504767481579,  102.723422139037280, 
    21.248445294361591,  71.612838387272518,  98.790577230754380,  37.672703385692813,  59.913264709571735, 
    66.258914917206056,  58.896488702227039,  48.468441706550053,  59.372718353600035,  44.341403793922630, 
    125.558998599555340, 
};


bool test_CIE94_base(void){
	for(int i=0; i<CIE94_TEST_COUNT; i++){
		double expected = CIE94_TEST_EXPECTED[i];
		double calculated = delta_CIE94_g(CIE94_TEST_INPUTS[i][0], CIE94_TEST_INPUTS[i][1]);

		expected = round_to_n_digits(expected, 4);
		calculated = round_to_n_digits(calculated, 4);


		bool correct =  expected == calculated;


		if(! correct){
			printf("Test test_CIE94_base FAILED at:\n");
			print_lab(CIE94_TEST_INPUTS[i][0]);
			printf(", ");
			print_lab(CIE94_TEST_INPUTS[i][1]);
			printf("\n");

			printf("Expected: %.4lf, calculated: %.4lf\n", expected, calculated);
			return false;
		}

	}

	puts("SUCCESS");
	return true;
}



















