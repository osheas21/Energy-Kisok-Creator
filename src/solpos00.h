 /*============================================================================
*
*    NAME:  solpos00.h
*
*    Contains:
*        S_solpos     (computes the solar position and intensity
*                      from time and place)
*            INPUTS:     (from posdata)
*                          year, month, day, hour, minute, second,
*                          latitude, longitude, timezone, interval
*            OPTIONAL:   (from posdata; defaults from S_init function)
*                            press   DEFAULT 1013.0 (standard pressure)
*                            temp    DEFAULT   10.0 (standard temperature)
*                            tilt    DEFAULT    0.0 (horizontal panel)
*                            aspect  DEFAULT  180.0 (South-facing panel)
*                            sbwid   DEFAULT    7.6 (shadowband width)
*                            sbrad   DEFAULT   31.7 (shadowband radius)
*                            sbsky   DEFAULT   0.04 (shadowband sky factor)
*
*            OUTPUTS:    (posdata) daynum, amass, ampress, azim, cosinc,
*                        elevref, etr, etrn, etrtilt, prime,
*                        sbcf, sretr, ssetr, unprime, zenref
*
*            RETURNS:   Long int status code (defined in solpos.h)
*
*    Usage:
*         In calling program, along with other 'includes', insert:
*
*              #include "solpos.h"
*
*    Martin Rymes
*    National Renewable Energy Laboratory
*    25 March 1998
*----------------------------------------------------------------------------*/

/*============================================================================
*
*     Define the function codes
*
*----------------------------------------------------------------------------*/
#define L_DOY    0x0001
#define L_GEOM   0x0002
#define L_ZENETR 0x0004
#define L_SSHA   0x0008
#define L_SBCF   0x0010
#define L_TST    0x0020
#define L_SRSS   0x0040
#define L_SOLAZM 0x0080
#define L_REFRAC 0x0100
#define L_AMASS  0x0200
#define L_PRIME  0x0400
#define L_TILT   0x0800
#define L_ETR    0x1000
#define L_ALL    0xFFFF

/*============================================================================
*
*     Define the bit-wise masks for each function
*
*----------------------------------------------------------------------------*/
#define S_DOY    ( L_DOY                          )
#define S_GEOM   ( L_GEOM   | S_DOY               )
#define S_ZENETR ( L_ZENETR | S_GEOM              )
#define S_SSHA   ( L_SSHA   | S_GEOM              )
#define S_SBCF   ( L_SBCF   | S_SSHA              )
#define S_TST    ( L_TST    | S_GEOM              )
#define S_SRSS   ( L_SRSS   | S_SSHA   | S_TST    )
#define S_SOLAZM ( L_SOLAZM | S_ZENETR            )
#define S_REFRAC ( L_REFRAC | S_ZENETR            )
#define S_AMASS  ( L_AMASS  | S_REFRAC            )
#define S_PRIME  ( L_PRIME  | S_AMASS             )
#define S_TILT   ( L_TILT   | S_SOLAZM | S_REFRAC )
#define S_ETR    ( L_ETR    | S_REFRAC            )
#define S_ALL    ( L_ALL                          )


/*============================================================================
*
*     Enumerate the error codes
*     (Bit positions are from least significant to most significant)
*
*----------------------------------------------------------------------------*/
/*          Code          Bit       Parameter            Range
      ===============     ===  ===================  =============   */
enum {S_YEAR_ERROR,    /*  0   year                  1950 -  2050   */
      S_MONTH_ERROR,   /*  1   month                    1 -    12   */
      S_DAY_ERROR,     /*  2   day-of-month             1 -    31   */
      S_DOY_ERROR,     /*  3   day-of-year              1 -   366   */
      S_HOUR_ERROR,    /*  4   hour                     0 -    24   */
      S_MINUTE_ERROR,  /*  5   minute                   0 -    59   */
      S_SECOND_ERROR,  /*  6   second                   0 -    59   */
      S_TZONE_ERROR,   /*  7   time zone              -12 -    12   */
      S_INTRVL_ERROR,  /*  8   interval (seconds)       0 - 28800   */
      S_LAT_ERROR,     /*  9   latitude               -90 -    90   */
      S_LON_ERROR,     /* 10   longitude             -180 -   180   */
      S_TEMP_ERROR,    /* 11   temperature (deg. C)  -100 -   100   */
      S_PRESS_ERROR,   /* 12   pressure (millibars)     0 -  2000   */
      S_TILT_ERROR,    /* 13   tilt                   -90 -    90   */
      S_ASPECT_ERROR,  /* 14   aspect                -360 -   360   */
      S_SBWID_ERROR,   /* 15   shadow band width (cm)   1 -   100   */
      S_SBRAD_ERROR,   /* 16   shadow band radius (cm)  1 -   100   */
      S_SBSKY_ERROR};  /* 17   shadow band sky factor  -1 -     1   */

struct posdata
{
  /***** ALPHABETICAL LIST OF COMMON VARIABLES *****/
                           /* Each comment begins with a 1-column letter code:
                              I:  INPUT variable
                              O:  OUTPUT variable
                              T:  TRANSITIONAL variable used in the algorithm,
                                  of interest only to the solar radiation
                                  modelers, and available to you because you
                                  may be one of them.

                              The FUNCTION column indicates which sub-function
                              within solpos must be switched on using the
                              "function" parameter to calculate the desired
                              output variable.  All function codes are
                              defined in the solpos.h file.  The default
                              S_ALL switch calculates all output variables.
                              Multiple functions may be or'd to create a
                              composite function switch.  For example,
                              (S_TST | S_SBCF). Specifying only the functions
                              for required output variables may allow solpos
                              to execute more quickly.

                              The S_DOY mask works as a toggle between the
                              input date represented as a day number (daynum)
                              or as month and day.  To set the switch (to
                              use daynum input), the function is or'd; to
                              clear the switch (to use month and day input),
                              the function is inverted and and'd.

                              For example:
                                  pdat->function |= S_DOY (sets daynum input)
                                  pdat->function &= ~S_DOY (sets month and day input)

                              Whichever date form is used, S_solpos will
                              calculate and return the variables(s) of the
                              other form.  See the soltest.c program for
                              other examples. */

  /* VARIABLE        I/O  Function    Description */
  /* -------------  ----  ----------  ---------------------------------------*/

  int   day;       /* I/O: S_DOY      Day of month (May 27 = 27, etc.)
                                        solpos will CALCULATE this by default,
                                        or will optionally require it as input
                                        depending on the setting of the S_DOY
                                        function switch. */
  int   daynum;    /* I/O: S_DOY      Day number (day of year; Feb 1 = 32 )
                                        solpos REQUIRES this by default, but
                                        will optionally calculate it from
                                        month and day depending on the setting
                                        of the S_DOY function switch. */
  int   function;  /* I:              Switch to choose functions for desired
                                        output. */
  int   hour;      /* I:              Hour of day, 0 - 23, DEFAULT = 12 */
  int   interval;  /* I:              Interval of a measurement period in
                                        seconds.  Forces solpos to use the
                                        time and date from the interval
                                        midpoint. The INPUT time (hour,
                                        minute, and second) is assumed to
                                        be the END of the measurement
                                        interval. */
  int   minute;    /* I:              Minute of hour, 0 - 59, DEFAULT = 0 */
  int   month;     /* I/O: S_DOY      Month number (Jan = 1, Feb = 2, etc.)
                                        solpos will CALCULATE this by default,
                                        or will optionally require it as input
                                        depending on the setting of the S_DOY
                                        function switch. */
  int   second;    /* I:              Second of minute, 0 - 59, DEFAULT = 0 */
  int   year;      /* I:              4-digit year (2-digit year is NOT
                                       allowed */

  /***** FLOATS *****/

  float amass;      /* O:  S_AMASS    Relative optical airmass */
  float ampress;    /* O:  S_AMASS    Pressure-corrected airmass */
  float aspect;     /* I:             Azimuth of panel surface (direction it
                                        faces) N=0, E=90, S=180, W=270,
                                        DEFAULT = 180 */
  float azim;       /* O:  S_SOLAZM   Solar azimuth angle:  N=0, E=90, S=180,
                                        W=270 */
  float cosinc;     /* O:  S_TILT     Cosine of solar incidence angle on
                                        panel */
  float coszen;     /* O:  S_REFRAC   Cosine of refraction corrected solar
                                        zenith angle */
  float dayang;     /* T:  S_GEOM     Day angle (daynum*360/year-length)
                                        degrees */
  float declin;     /* T:  S_GEOM     Declination--zenith angle of solar noon
                                        at equator, degrees NORTH */
  float eclong;     /* T:  S_GEOM     Ecliptic longitude, degrees */
  float ecobli;     /* T:  S_GEOM     Obliquity of ecliptic */
  float ectime;     /* T:  S_GEOM     Time of ecliptic calculations */
  float elevetr;    /* O:  S_ZENETR   Solar elevation, no atmospheric
                                        correction (= ETR) */
  float elevref;    /* O:  S_REFRAC   Solar elevation angle,
                                        deg. from horizon, refracted */
  float eqntim;     /* T:  S_TST      Equation of time (TST - LMT), minutes */
  float erv;        /* T:  S_GEOM     Earth radius vector
                                        (multiplied to solar constant) */
  float etr;        /* O:  S_ETR      Extraterrestrial (top-of-atmosphere)
                                        W/sq m global horizontal solar
                                        irradiance */
  float etrn;       /* O:  S_ETR      Extraterrestrial (top-of-atmosphere)
                                        W/sq m direct normal solar
                                        irradiance */
  float etrtilt;    /* O:  S_TILT     Extraterrestrial (top-of-atmosphere)
                                        W/sq m global irradiance on a tilted
                                        surface */
  float gmst;       /* T:  S_GEOM     Greenwich mean sidereal time, hours */
  float hrang;      /* T:  S_GEOM     Hour angle--hour of sun from solar noon,
                                        degrees WEST */
  float julday;     /* T:  S_GEOM     Julian Day of 1 JAN 2000 minus
                                        2,400,000 days (in order to regain
                                        single precision) */
  float latitude;   /* I:             Latitude, degrees north (south negative) */
  float longitude;  /* I:             Longitude, degrees east (west negative) */
  float lmst;       /* T:  S_GEOM     Local mean sidereal time, degrees */
  float mnanom;     /* T:  S_GEOM     Mean anomaly, degrees */
  float mnlong;     /* T:  S_GEOM     Mean longitude, degrees */
  float rascen;     /* T:  S_GEOM     Right ascension, degrees */
  float press;      /* I:             Surface pressure, millibars, used for
                                        refraction correction and ampress */
  float prime;      /* O:  S_PRIME    Factor that normalizes Kt, Kn, etc. */
  float sbcf;       /* O:  S_SBCF     Shadow-band correction factor */
  float sbwid;      /* I:             Shadow-band width (cm) */
  float sbrad;      /* I:             Shadow-band radius (cm) */
  float sbsky;      /* I:             Shadow-band sky factor */
  float solcon;     /* I:             Solar constant (NREL uses 1367 W/sq m) */
  float ssha;       /* T:  S_SRHA     Sunset(/rise) hour angle, degrees */
  float sretr;      /* O:  S_SRSS     Sunrise time, minutes from midnight,
                                        local, WITHOUT refraction */
  float ssetr;      /* O:  S_SRSS     Sunset time, minutes from midnight,
                                        local, WITHOUT refraction */
  float temp;       /* I:             Ambient dry-bulb temperature, degrees C,
                                        used for refraction correction */
  float tilt;       /* I:             Degrees tilt from horizontal of panel */
  float timezone;   /* I:             Time zone, east (west negative).
                                      USA:  Mountain = -7, Central = -6, etc. */
  float tst;        /* T:  S_TST      True solar time, minutes from midnight */
  float tstfix;     /* T:  S_TST      True solar time - local standard time */
  float unprime;    /* O:  S_PRIME    Factor that denormalizes Kt', Kn', etc. */
  float utime;      /* T:  S_GEOM     Universal (Greenwich) standard time */
  float zenetr;     /* T:  S_ZENETR   Solar zenith angle, no atmospheric
                                        correction (= ETR) */
  float zenref;     /* O:  S_REFRAC   Solar zenith angle, deg. from zenith,
                                        refracted */
};

/* For users that wish to access individual functions, the following table
lists all output and transition variables, the L_ mask for the function
that calculates them, and all the input variables required by that function.
The function variable is set to the L_ mask, which will force S_solpos to
only call the required function.  L_ masks may be ORed as desired.

VARIABLE      Mask       Required Variables
---------  ----------  ---------------------------------------
 amass      L_AMASS    zenref, press
 ampress    L_AMASS    zenref, press
 azim       L_SOLAZM   elevetr, declin, latitude, hrang
 cosinc     L_TILT     azim, aspect, tilt, zenref, coszen,etrn
 coszen     L_REFRAC   elevetr, press, temp
 dayang     L_GEOM     All date, time, and location inputs
 declin     L_GEOM     All date, time, and location inputs
 eclong     L_GEOM     All date, time, and location inputs
 ecobli     L_GEOM     All date, time, and location inputs
 ectime     L_GEOM     All date, time, and location inputs
 elevetr    L_ZENETR   declin, latitude, hrang
 elevref    L_REFRAC   elevetr, press, temp
 eqntim     L_TST      hrang, hour, minute, second, interval
 erv        L_GEOM     All date, time, and location inputs
 etr        L_ETR      coszen, solcon, erv
 etrn       L_ETR      coszen, solcon, erv
 etrtilt    L_TILT     azim, aspect, tilt, zenref, coszen, etrn
 gmst       L_GEOM     All date, time, and location inputs
 hrang      L_GEOM     All date, time, and location inputs
 julday     L_GEOM     All date, time, and location inputs
 lmst       L_GEOM     All date, time, and location inputs
 mnanom     L_GEOM     All date, time, and location inputs
 mnlong     L_GEOM     All date, time, and location inputs
 rascen     L_GEOM     All date, time, and location inputs
 prime      L_PRIME    amass
 sbcf       L_SBCF     latitude, declin, ssha, sbwid, sbrad, sbsky
 ssha       L_SRHA     latitude, declin
 sretr      L_SRSS     ssha, tstfix
 ssetr      L_SRSS     ssha, tstfix
 tst        L_TST      hrang, hour, minute, second, interval
 tstfix     L_TST      hrang, hour, minute, second, interval
 unprime    L_PRIME    amass
 utime      L_GEOM     All date, time, and location inputs
 zenetr     L_ZENETR   declination, latitude, hrang
 zenref     L_REFRAC   elevetr, press, temp
 */


/*============================================================================
*    Long int function S_solpos, adapted from the NREL VAX solar libraries
*
*    This function calculates the apparent solar position and intensity
*    (theoretical maximum solar energy) based on the date, time, and
*    location on Earth. (DEFAULT values are from the optional S_posinit
*    function.)
*
*    Requires:
*        Date and time:
*            year
*            month  (optional without daynum)
*            day    (optional without daynum)
*            daynum
*            hour
*            minute
*            second
*        Location:
*            latitude
*            longitude
*        Location/time adjuster:
*            timezone
*        Atmospheric pressure and temperature:
*            press     DEFAULT 1013.0 mb
*            temp      DEFAULT 10.0 degrees C
*        Tilt of flat surface that receives solar energy:
*            aspect    DEFAULT 180 (South)
*            tilt      DEFAULT 0 (Horizontal)
*        Shadow band parameters:
*            sbwid     DEFAULT 7.6 cm
*            sbrad     DEFAULT 31.7 cm
*            sbsky     DEFAULT 0.04
*        Functionality
*            function  DEFAULT S_ALL (all output parameters computed)
*
*    Returns:
*        everything defined at the top of this listing.
*----------------------------------------------------------------------------*/
long S_solpos (struct posdata *pdat);

/*============================================================================
*    Void function S_init
*
*    This function initiates all of the input functions to S_Solpos().
*    NOTE: This function is optional if you initialize all input parameters
*          in your calling code.
*
*    Requires: Pointer to a posdata structure, members of which are
*           initialized.
*
*    Returns: Void
*
*----------------------------------------------------------------------------*/
void S_init(struct posdata *pdat);


/*============================================================================
*    Void function S_decode
*
*    This function decodes the error codes from S_solpos return value
*
*    INPUTS: Long integer S_solpos return value, struct posdata*
*
*    OUTPUTS: Descriptive text of errors to stderr
*----------------------------------------------------------------------------*/
void S_decode(long code, struct posdata *pdat);





#define NUM_YEARS 22			// number of years we are using
#define NUM_MONTHS 13			// number of months ignoring 0th index
#define NUM_DAYS 367			// number of days we are using (367 accounts for leap years and allows us to ignore 0th index)
#define NUM_HOURS 24			// number of hours in a day
#define TOTAL_NUM_DAYS 8036		// total number of days during 1984 through 2005

/* Starting on January 1st, 1984 */
#define START_YEAR 1984
#define START_DAY 1

struct PVData
{
    char model[100];
    float cellMaxPower;		// maximum power at STC for individual module
	float arrayMaxPower;	// maximum power at STC for entire array
    float cellVoc;			// open-circuit voltage of individual module
    float arrayVoc;			// open-circuit voltage of the PV array
    float noct;				// nominal operating cell temperature
    float alphaP;			// maximum power temperature coefficient
	int stringSize;			// number of modules per string
	int numStrings;			// number of strings per array
	float cellPrice;		// price of solar panel
};

struct ChargeControllerData
{
    char model[100];		// model name
    float capacity;			// nominal capacity
    float maxVoltage;		// maximum volatage
    float efficiency;		// rated efficiency
    float price;			// price per module
};


struct InverterData
{
    char model[100];			// inverter model name 
    float capacity;				// nominal inverter capacity
    float maxVoltage;			// maximum input voltage
    float maxEfficiency;		// maximum efficiency
    float zeroLoadPower;		// zero load power consumption
    float fullLoadEfficiency;	// 
    float price;
};

struct BatteryData
{
    char model[100];					// name of the model of the battery
	float dischargeTime[5];				// hour-rate of the corresponding charge capacity [1/h]
    float cellEffectiveCapacity[5];		// discharge rating corresponding to discharge rate of individual battery [Ah]
    float cellDischargeCurrent[5];		// discharge rate corresponding to discharge time of indivudual battery [A]
    float arrayEffectiveCapacity[5];	// discharge rating corresponding to discharge rate of entire array [Ah]
    float arrayDischargeCurrent[5];		// discharge rate corresponding to discharge time of entire array [A]
	float cellVoltage;					// voltage of individual battery cell
	float arrayVoltage;					// voltage of entire array
	float cellNominalCapacity;			// capacity of invividual battery cell
	float arrayNominalCapacity;			// capacity of entire array
	int stringSize;						// number of cells per string
	int numStrings;						// number of strings in array
	float minSoC;						// minimum state of charge
	float cellPrice;					// price of individual battery
};


struct DataValues
{
    int loadProfile[NUM_HOURS];								// hourly load profile
    float insolation[NUM_YEARS][NUM_DAYS][NUM_HOURS];		// hourly all-sky insolation
    float hourlySoC[NUM_YEARS][NUM_DAYS][NUM_HOURS];		// hourly state of charge
    float allMaxSoC;										// maximum state of charge over all years
    float allMinSoC;										// minimum state of charge over all years
    float allAvgSoC;										// average state of charge over all years
    float yearlyMaxSoC[NUM_YEARS];							// maximum state of charge for each year
    float yearlyMinSoC[NUM_YEARS];							// minimum state of charge for each year
    float monthlyMaxSoC[NUM_YEARS][NUM_MONTHS];				// maximum state of charge for each month
    float monthlyMinSoC[NUM_YEARS][NUM_MONTHS];				// minimum state of charge for each month
    float yearlyAvgSoC[NUM_YEARS];							// average state of charge for each year

	float SoCIntervalRanges[6];								// contains the lower end of each range. upper end of each range will be lower end of next range; ends at 1 (100% charged).
	float SoCIntervalProportion[6];							// contains the percent of time that the battery's state of charge is withing each range
	int SoCIntervalTime[6];									// continas the number of hours per year that the battery is within each range

    float reliability;										// reliability over all years
    float allAvgPVOutput;									// average daily PV ouput over all years
    float eachYearAvgPVOutput[NUM_YEARS];					// average daily PV output for each year
    float eachMonthAvgPVOutput[NUM_YEARS][NUM_MONTHS];		// average daily PV output for each month in each year
    float avgMonthPVOutput[NUM_MONTHS];						// average daily PV output for each month over all years
    int yearMinPVOutput;									/* stores the year with the lowest PV production, 
																should be used to index yearlyAvgPVOutput, if the power
																output levels are what you are interested in */
    int monthMinPVOutput[NUM_MONTHS];						/* stores the year for the worst PV output for each month,
																should be used to index monthlyAvgPVOutput, if the power
																output levels are what you are interested in.
																e.g. monthMinPVOutput[4] returns the year that had the 
																worst April. */
};


// takes in the name/location of the file to be read and returns the FILE struct
FILE *openReadFile (char file_name[]);

// initializes cell/arrayEffectiveCapacity, dischargeTime, and cell/arrayDischargeCurrent arrays to 0
void initializeBattery(struct BatteryData *battery);

// calculates the arrayEffectiveCapacity, cellDischargeCurrent, arrayDischargeCurrent, arrayNominalCapacity
// and arrayVoltage using the parameters for the size of the array and parameters of indivudual battery cells
void setBatteryArray(struct BatteryData *battery);

// takes in a line from a csv in the line field, parses out data, and places data in the array
// array can be an array or a single value. single values must be passed by reference
void loadValues(char line[], float *array);

// loads a string into the model name of the BatteryData struct
void loadModelName (char line[], char *modelName);

// imports the time zone from the data in Time Zone Data.txt from Bing Maps API
void importTimeZone (FILE *timezone_file, struct posdata *pdat);

// loads the hourly load into *load array for each hour of the day
void readLoadProfile (FILE *load_profile, int *load);

// takes in the file, charge controller struct, and the selection number that corresponds to the charge controller that will be used
// loads the values from the csv into the chargeController struct
void importChargeControllerLibrary (FILE *chargeController_file, struct ChargeControllerData *chargeController, int selection);

// takes in the file, inverter struct, and the selection number that correstponds to the inverter model that will be used
// loads the values from the inverter csv into the inverter struct
void importInverterLibrary (FILE *inverter_file, struct InverterData *inverter, int selection);

// takes the input of a file in the specific format of the battery library and the battery struct
// fills out the battery struct with the data entered in the csv library for battery cells
void importBatteryLibrary (FILE *battery_file, struct BatteryData *battery, int selection);

// takes the input of a file in the specific format of the PV library and the PV struct
// fills out the PV struct with the data entered in the csv library for PV modules
void importPVLibrary (FILE *pv_file, struct PVData *panel, int selection);

// reads the data at the current line
float readPOWERData(char line[]);

// reads the year the data was taken in from POWER dataset
// returns 0 if there is no year to be read
int readPOWERYear(char line[]);

// calculated average extraterrestrial irradiance on a tilted surface over the course of an hour
float calcAvgIrradianceTilt (struct posdata *pdat);

// calculates extraterrestrial insolation on a flat surface over the course of an hour
float calcAvgInsolation (struct posdata *pdat);

// calculates the temperature for the specified hour given the sunset, min, max, and avg temp
float calcAmbientTemp (int hour, float sunset, float minTemp, float maxTemp, float avgTemp);

// calculated the temperature of the PV module for the specified time
float calcCellTemp (float ambientTemp, float noct, float irradiance);

// calculate the power produced by a PV module using the temperature of the cell
// irradiance, and its characteristics
float calcPVPower (float stcPower, float irradiance, float alphaP, float cellTemp);

// calculated the peukert exponent for a specified battery at a specified current
float calcPeukert (struct BatteryData *battery, float currentOut);

// calculate the SoC of the battery over the course of an hour
float calcBatteryCharge (struct BatteryData *battery, struct InverterData *inverter, float startSoC, float currentOut);

// calculates the reliability of the system (hours with power/total hours)
float calcReliability (struct BatteryData *battery, float SoC[NUM_YEARS][NUM_DAYS][NUM_HOURS]);

// calculates fullMinSoC, fullMaxSoC, yearlyMinSoC, yearlyMaxSoC
float calcDataValues (struct DataValues *data, float SoC[NUM_YEARS][NUM_DAYS][NUM_HOURS]);

// calculates and stores the maximum state of charge for each month, each year, and all years of each year into
// the corresponding DataValues
void calcMaxSoC (struct posdata *pdat, struct DataValues *data);

// calculates and stores the minimum state of charge for each month, each year, and all years of each year into
// the corresponding DataValues
void calcMinSoC (struct posdata *pdat, struct DataValues *data);

void calcSoCIntervals (struct posdata *pdat, struct DataValues *data, float minSoC);

// calculates the daily average PV array output for each month, year, and all the years
// stores the data in the DataValues struct
void calcAvgPVOutput (struct posdata *pdat, struct DataValues *data, float ***pvPower);

// calculates the month and year
void lowestPVOutput (struct posdata *pdat, struct DataValues *data, float ***pvPower);

void readModelNames (FILE *current_file, char *modelNames, int string_size, int num_strings);

void storeData (struct posdata *pdat, struct DataValues *data, struct BatteryData *battery, float ***pvPower);


float insolationQuickLook (struct posdata *pdat, float dailyKTData[NUM_YEARS][NUM_DAYS]);

void readAllData (struct posdata *pdat, FILE *allData, float dailyKTData[NUM_YEARS][NUM_DAYS], float avgTempData[NUM_YEARS][NUM_DAYS],
            float maxTempData[NUM_YEARS][NUM_DAYS], float minTempData[NUM_YEARS][NUM_DAYS]);