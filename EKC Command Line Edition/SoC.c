/*============================================================================
*
*    NAME:  stest00.c
*
*    PURPOSE:  Exercises the solar position algorithms in 'solpos.c'.
*
*        S_solpos
*            INPUTS:     year, daynum, hour, minute, second, latitude,
*                        longitude, timezone
*
*            OPTIONAL:   press   DEFAULT 1013.0 (standard pressure)
*                        temp    DEFAULT   10.0 (standard temperature)
*                        tilt    DEFAULT    0.0 (horizontal panel)
*                        aspect  DEFAULT  180.0 (South-facing panel)
*                        month   (if the S_DOY function is turned off)
*                        day     ( "             "             "     )
*
*            OUTPUTS:    amass, ampress, azim, cosinc, coszen, day, daynum,
*                        elevref, etr, etrn, etrtilt, month, prime,
*                        sbcf, sretr, ssetr, unprime, zenref
*
*       S_init        (optional initialization for all input parameters in
*                      the posdata struct)
*           INPUTS:     struct posdata*
*           OUTPUTS:    struct posdata*
*
*                     (Note: initializes the required S_solpos INPUTS above
*                      to out-of-bounds conditions, forcing the user to
*                      supply the parameters; initializes the OPTIONAL
*                      S_solpos inputs above to nominal values.)
*
*      S_decode       (optional utility for decoding the S_solpos return code)
*           INPUTS:     long int S_solpos return value, struct posdata*
*           OUTPUTS:    text to stderr
*
*
*        All variables are defined as members of the struct posdata
*        in 'solpos00.h'.
*
*    Usage:
*         In calling program, along with other 'includes', insert:
*
*              #include "solpos00.h"
*
*    Martin Rymes
*    National Renewable Energy Laboratory
*    25 March 1998
*
*    28 March 2001 REVISION:  SMW changed benchmark numbers to reflect the
*                             February 2001 changes to solpos00.c
*
*----------------------------------------------------------------------------*/
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "solpos00.h"     /* <-- There is the 'include' I was talking about */
#include "Entries.h"


int main ( )
{
    struct posdata pd, *pdat; /* declare a posdata struct and a pointer for
                                 it (if desired, the structure could be
                                 allocated dynamically with malloc) */
    long retval;              /* to capture S_solpos return codes */



    pdat = &pd; /* point to the structure for convenience */

    /* Initialize structure to default values. (Optional only if ALL input
       parameters are initialized in the calling code, which they are not
       in this example.) */

    S_init (pdat);



    pdat->temp      =   27.0;
    pdat->press     = 1006.0;



    /* I use Lusaka, Zambia for this example */
    // #define LUSAKA_LAT	-35	// Lusaka's latitudinal coordinate
    // #define LUSAKA_LON	 28	// Lusaka's logitudinal coordinate
    // #define LUSAKA_TMZ  +2.0		// Lusaka's time zone

    pdat->longitude = LONGITUDE;  /* Note that latitude and longitude are  */
    pdat->latitude  = LATITUDE;  /*   in DECIMAL DEGREES, not Deg/Min/Sec */
    pdat->timezone  = TIMEZONE;   /* Eastern time zone, even though longitude would
                                  suggest Central.  We use what they use.
                                  DO NOT ADJUST FOR DAYLIGHT SAVINGS TIME. */


    /* The time of day (STANDARD time) */

    pdat->hour      = 0;
    pdat->minute    = 0;
    pdat->second    = 0;

	pdat->tilt		=	PV_TILT;
	pdat->aspect	= 	AZIMUTH;


    /* READ IN KT VALUES */

    printf ("\nopening file...");
    FILE *allData;
    char FILE_NAME[100] = "Weather Data.txt";

    allData = fopen( FILE_NAME, "r");
    if(allData == NULL)
        printf("Error: file not opened\n\n");
    else
        printf ("done\n\n");



    printf ("initializing reading parameters...");


    float dailyKTData[NUM_YEARS][NUM_DAYS];		// stores daily clearness index data
    float avgTempData[NUM_YEARS][NUM_DAYS];		// stores daily average temperature data
    float maxTempData[NUM_YEARS][NUM_DAYS];		// stores daily maximum temperature data
    float minTempData[NUM_YEARS][NUM_DAYS];		// stores daily minimum temperature data
    printf ("done\n");

    readAllData(pdat, allData, dailyKTData, avgTempData, maxTempData, minTempData);
    




    pdat->year      = 1984;    /* The year is 1999. */
    pdat->daynum    =    1;    /* July 22nd, the 203'rd day of the year (the
                                  algorithm will compensate for leap year, so
                                  you just count days). S_solpos can be
                                  configured to accept month-day dates; see
                                  examples below.) */

    /* The time of day (STANDARD time) */

    pdat->hour      = 0;
    pdat->minute    = 0;
    pdat->second    = 0;


    struct DataValues d, *data;
    data = &d;


	FILE *pv_file = openReadFile("Library/PV Modules.csv");
    struct PVData pnl, *panel;
    panel = &pnl;

	importPVLibrary(pv_file, panel, pv_selection);
    printf ("here\n");
	panel->stringSize = pv_stringSize;
	panel->numStrings = pv_numStrings;
	panel->arrayMaxPower = panel->stringSize * panel->numStrings * panel->moduleMaxPower;
	printf ("model: %s\tmaxpower: %.2f\tnoct: %.2f\talpha: %.3f\nVoc: %.2f\n", panel->model, panel->moduleMaxPower, panel->noct, panel->alphaP, panel->moduleVoc);


	FILE *battery_file = openReadFile("Library/Batteries.csv");
    struct BatteryData bat, *battery;
    battery = &bat;
	initializeBattery(battery);
    battery->stringSize = battery_stringSize;
    battery->numStrings = battery_numStrings;

	importBatteryLibrary(battery_file, battery, battery_selection);
    setBatteryArray(battery);
    printf ("model: %s\tmodule voltage: %.2f\tarray voltage: %.2f\teffective capacity[2]: %.3f\n", battery->model, battery->moduleVoltage, battery->arrayVoltage, battery->moduleEffectiveCapacity[2]);
    printf ("minSoC: %.2f\n", battery->minSoC);


    FILE *chargeController_file = openReadFile("Library/Charge Controllers.csv");
    struct ChargeControllerData chrgcntrllr, *cController;
    cController = &chrgcntrllr;
    importChargeControllerLibrary(chargeController_file, cController, chargeController_selection);
    printf ("model: %s\t capacity: %.1f\t maxVoltage: %.1f\t efficiency: %.1f\t price: %.2f\n\n", 
            cController->model, cController->capacity, cController->maxVoltage, cController->efficiency, cController->price);


    FILE *inverter_file = openReadFile("Library/Inverters.csv");
    struct InverterData invrtr, *inverter;
    inverter = &invrtr;
    importInverterLibrary(inverter_file, inverter, inverter_selection);
    printf ("model: %s\tcapacity: %.2f\t max voltage: %.2f\t max eff: %.2f\t zero load power: %.2f\t price: %.2f\n",
            inverter->model, inverter->capacity, inverter->maxVoltage, inverter->maxEfficiency, inverter->zeroLoadPower, inverter->price);





	float SoC = (1 + battery->minSoC) / 2;  // start state of charge halfway between fully charged and minimum state of charge
	float loadCurrent;      // negative is current into the battery, positive is current out of the battery

	FILE *load_profile_file = openReadFile("Library/Load Profile.csv");
	importLoadProfile(load_profile_file, data->loadProfile);



	// for some reason these have to be dynamically allocated, possibly not enough stack memory?
	float ***hourlyTemp						= (float ***)malloc(sizeof(float **) * NUM_YEARS);	// ambient temperature at each hour of the day
	float ***cellTemp						= (float ***)malloc(sizeof(float **) * NUM_YEARS);	// PV module temperature at each hour of the day
	float ***pvPower						= (float ***)malloc(sizeof(float **) * NUM_YEARS);	// power produced by the PV array at each hour of the day
	float ***hourlyCControllerOutput		= (float ***)malloc(sizeof(float **) * NUM_YEARS);	// power out of the charge controller at each hour of the day
	for (int i = 0; i < NUM_YEARS; i++) {
	    hourlyTemp[i]						= (float **)malloc(sizeof(float *) * NUM_DAYS);
	    cellTemp[i]							= (float **)malloc(sizeof(float *) * NUM_DAYS);
	    pvPower[i]							= (float **)malloc(sizeof(float *) * NUM_DAYS);
		hourlyCControllerOutput[i]			= (float **)malloc(sizeof(float *) * NUM_DAYS);

	    for (int j = 0; j < NUM_DAYS; j++) {
	        hourlyTemp[i][j]				= (float *)malloc(sizeof(float) * NUM_HOURS);
	        cellTemp[i][j] 					= (float *)malloc(sizeof(float) * NUM_HOURS);
	        pvPower[i][j] 					= (float *)malloc(sizeof(float) * NUM_HOURS);
			hourlyCControllerOutput[i][j]	= (float *)malloc(sizeof(float) * NUM_HOURS);
	    }
	}

    printf ("average daily insolation (kWh/m^2/day): %.3f\n\n", insolationQuickLook(pdat, dailyKTData));
    
	for (pdat->year = START_YEAR; pdat->year < START_YEAR + NUM_YEARS; pdat->year++) {
		for (pdat->daynum = 1; pdat->daynum < 366 || (pdat->daynum < 367 && pdat->year % 4 == 0); pdat->daynum++) {   // while the day is less than 12/31 on a leap-year, and it is not the 366th day of the year on a non-leap-year
			for (pdat->hour = 0; pdat->hour < NUM_HOURS; pdat->hour++) {

				pdat->temp = hourlyTemp[pdat->year-START_YEAR][pdat->daynum][pdat->hour] = calcAmbientTemp(pdat->hour, pdat->ssetr,
					minTempData[pdat->year-START_YEAR][pdat->daynum], maxTempData[pdat->year-START_YEAR][pdat->daynum],
					avgTempData[pdat->year-START_YEAR][pdat->daynum]);

				retval = S_solpos (pdat);  /* S_solpos function call */
				S_decode(retval, pdat);    /* ALWAYS look at the return code! */

				data->insolation[pdat->year-START_YEAR][pdat->daynum][pdat->hour] = calcAvgIrradianceTilt(pdat) * dailyKTData[pdat->year-START_YEAR][pdat->daynum];

				cellTemp[pdat->year-START_YEAR][pdat->daynum][pdat->hour] = calcCellTemp(hourlyTemp[pdat->year-START_YEAR][pdat->daynum][pdat->hour],
					panel->noct, data->insolation[pdat->year-START_YEAR][pdat->daynum][pdat->hour]);

				pvPower[pdat->year-START_YEAR][pdat->daynum][pdat->hour] = calcPVPower(panel->arrayMaxPower, data->insolation[pdat->year-START_YEAR][pdat->daynum][pdat->hour],
					panel->alphaP, cellTemp[pdat->year-START_YEAR][pdat->daynum][pdat->hour]);

				hourlyCControllerOutput[pdat->year-START_YEAR][pdat->daynum][pdat->hour] = calcCControllerOutput(data, cController, pvPower[pdat->year-START_YEAR][pdat->daynum][pdat->hour]);

				loadCurrent = (data->loadProfile[pdat->hour] - hourlyCControllerOutput[pdat->year-START_YEAR][pdat->daynum][pdat->hour]) / battery->arrayVoltage;

				SoC = data->hourlySoC[pdat->year-START_YEAR][pdat->daynum][pdat->hour] = calcBatteryCharge(battery, inverter, SoC, loadCurrent);

				calcChargeCycles(pdat, data, data->hourlySoC[pdat->year-START_YEAR][pdat->daynum][pdat->hour-1], data->hourlySoC[pdat->year-START_YEAR][pdat->daynum][pdat->hour]);
                    
                // printf ("Year: %i\tDay: %i\tHour: %i \tGHI: %.3f\tPV Power: %.3f\tState of Charge: %.2f\n", 
                //     pdat->year, pdat->daynum, pdat->hour, data->insolation[pdat->year-START_YEAR][pdat->daynum][pdat->hour], 
                //     pvPower[pdat->year-START_YEAR][pdat->daynum][pdat->hour], data->hourlySoC[pdat->year-START_YEAR][pdat->daynum][pdat->hour]*100);
                // printf ("etrtilt: %.3f\tambient: %.2f\n", pdat->etrtilt, hourlyTemp[pdat->year-START_YEAR][pdat->daynum][pdat->hour]);
                // printf ("min: %.2f\tmax: %.2f\tavg: %.2f\tkt: %.2f\n", minTempData[pdat->year-START_YEAR][pdat->daynum], maxTempData[pdat->year-START_YEAR][pdat->daynum],
                //     avgTempData[pdat->year-START_YEAR][pdat->daynum], dailyKTData[pdat->year-START_YEAR][pdat->daynum]);
                // printf ("Year: %i\tDay: %i\tHour: %i\t load: %i\n", pdat->hour, pdat->daynum, pdat->hour, data->loadProfile[pdat->hour]);
				// getc(stdin);
            }
            // printf ("Year: %i\tDay: %i\tKT: %.2f\tMax Temp: %.3f\tGlobal Insolation: %.3f\tTotal Power: %.2f\n\n",
            //     pdat->year, pdat->daynum, dailyKTData[pdat->year-START_YEAR][pdat->daynum], maxTempData[pdat->year-START_YEAR][pdat->daynum], globInsolation, dailyPower);
        }
        printf ("calculating...%.0f%%\n", (pdat->year-(float)START_YEAR)/NUM_YEARS*100);
    }


	printf ("Reliability: %.4f\n", calcReliability(battery, data->hourlySoC));


    storeData(pdat, data, battery, pvPower);

	printf ("cc: %i\n", data->numChargeCycles);

    // /***** HOW TO USE MIN AND MAX SOC *****/
	// printf ("\n");

    // for (pdat->year = START_YEAR; pdat->year < START_YEAR + NUM_YEARS; pdat->year++) {
    //     for (pdat->month = 1; pdat->month < NUM_MONTHS; pdat->month++) {
    //         printf ("year: %i\t month: %i\t minSoC: %.2f\t maxSoC: %.2f\n", pdat->year, pdat->month, data->monthlyMinSoC[pdat->year-START_YEAR][pdat->month], data->monthlyMaxSoC[pdat->year-START_YEAR][pdat->month]);
    //     }
    //      printf ("year: %i\t yearly min SoC: %.2f\t yearly max SoC: %.2f\n\n", pdat->year, data->yearlyMinSoC[pdat->year-START_YEAR], data->yearlyMaxSoC[pdat->year-START_YEAR]);
    // }

	// printf ("\n");
    // /**************************************/


    // /***** HOW TO USE MONTHLY AND YEARLY PV OUTPUT ARRAYS *****/
	// printf ("\n");

    // for (pdat->year = START_YEAR; pdat->year < START_YEAR + NUM_YEARS; pdat->year++) {
    //     for (pdat->month = 1; pdat->month < NUM_MONTHS; pdat->month++) {
    //         printf ("year: %i\t month: %i\t avg PV output: %.2f\n", pdat->year, pdat->month, data->eachMonthAvgPVOutput[pdat->year-START_YEAR][pdat->month]);
    //     }
    //     printf ("year: %i\t avg yearly power: %.2f\n\n", pdat->year, data->eachYearAvgPVOutput[pdat->year-START_YEAR]);
    // }
    // printf ("\nall average power: %.2f\n", data->allAvgPVOutput);

	// printf ("\n");
    // /**********************************************************/


    /***** HOW TO FIND THE WORST YEAR FOR EACH MONTH AND THE AVERAGE DAILY POWER FOR THAT MONTH *****/
	printf ("\n");
    for (pdat->month = 1; pdat->month < NUM_MONTHS; pdat->month++) {
        printf ("month: %i\t worst year for month: %i\t avg power for that month in that year: %.2f\n",
            pdat->month, data->monthMinPVOutput[pdat->month], data->eachMonthAvgPVOutput[data->monthMinPVOutput[pdat->month]-START_YEAR][pdat->month]);
    }

	printf ("\n");
    /************************************************************************************************/


    // /***** HOW TO FIND THE AVERAGE DAILY ENERGY FOR EACH MONTH OVER THE AVERAGE OF ALL YEARS *****/
    // printf ("\n");
	
	// for (pdat->month = 1; pdat->month < NUM_MONTHS; pdat->month++) {
    //     printf ("month: %i\t avg daily power (Wh): %.1f\n", pdat->month, data->avgMonthPVOutput[pdat->month]);
    // }

	// printf ("\n");
    // /*********************************************************************************************/

	// FILE *timezone_file = openReadFile ("Time Zone Data.txt");
	// importTimeZone(timezone_file, pdat);

	// printf ("%.2f\n", pdat->timezone);


	/***** HOW TO FIND AMOUNT OF TIME AT RANGES OF STATE OF CHARGE *************************************/
	printf ("\n");

	printf ("\nInterval:\t");
	for (int i = 0; i < 5; i++)
		printf (" | %4.1f%% - %4.1f%% | ", data->SoCIntervalRanges[i] * 100, data->SoCIntervalRanges[i+1] * 100);
	printf (" | %4.1f%% - 100%% |\n", data->SoCIntervalRanges[5] * 100);

	printf ("Proportion:\t");
	for (int i = 0; i < 6; i++)
		printf (" | %8.1f%%     | ", data->SoCIntervalProportion[i] * 100);
	printf ("\n");
	
	printf ("Time (h):\t");
	for (int i = 0; i < 6; i++)
		printf (" | %8i      | ", data->SoCIntervalTime[i]);
	printf ("\n");

	printf ("\n");
	/****************************************************************************************************/

	FILE *savefile = fopen("Recent Save.csv", "w");
	saveData(pdat, data, pvPower, hourlyTemp, savefile);

    printf ("\ndeallocating memory...");
    for (int i = 0; i < NUM_YEARS; i++) {
        for (int j = 0; j < NUM_DAYS; j++) {
            free(hourlyTemp[i][j]);
            free(cellTemp[i][j]);
            free(pvPower[i][j]);
        }
        free(hourlyTemp[i]);
        free(cellTemp[i]);
        free(pvPower[i]);
    }
    free(hourlyTemp);
    free(cellTemp);
    free(pvPower);


    printf ("done.\n\n");

    return 0;
}