
#define LONGITUDE	28		// Location's latitudinal coordinate
#define LATITUDE	-35		// Location's logitudinal coordinate
#define TIMEZONE	+2.0	// Location's time zone
#define PV_TILT		35		// tilt of the solar panel in degrees
#define AZIMUTH		0.0		// azimuth angle of array. 0 = North, 90 = West, 180 = South, 270 = East

#define pv_selection 1		// solar panel model, selected from the "PV Modules.csv" file in the "Library" folder
#define pv_stringSize 3		// number of solar panels per string
#define pv_numStrings 2		// number of strings per array

#define battery_selection 1		// battery model, selected from the "Batteries.csv" file in the "Library" folder
#define battery_stringSize 2	// number of batteries per string
#define battery_numStrings 1	// number of strings per array

#define inverter_selection 1	// inverter model, selected from the "Inverters.csv" file in the "Library" folder
#define chargeController_selection 1	// charge controller model, selected from the "Charge Controllers.csv" file in the "Library" folder