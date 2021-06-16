#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <conio.h> 
#include <ctype.h>
#include <cairo.h>
#include <math.h>
#include <gtk/gtk.h>

#include <curl/curl.h>
#include "solpos00.h"

#define INPUT_LENGTH 15
#define BUFFSIZ 100
#define END_YEAR 2006
#define STRING_SIZE 100
#define NUM_STRINGS 10

// w = 1000, h = 800

GtkWidget *printVal4, *printVal5;
GtkWidget *printVal6, *printVal7, *printVal8, *printVal9, *printVal10, *printVal11;
GtkWidget *printVal12, *printVal13, *printVal14, *printVal15, *printVal16, *printVal17;
GtkWidget *printVal18, *printVal19, *printVal20, *printVal21, *printVal22, *printVal23, *printVal24;
GtkWidget *printVal25, *printVal26, *printVal27, *printVal28, *printVal29, *printVal30, *printVal70;
GtkWidget *printVal31, *printVal32, *printVal33, *printVal34, *printVal35, *printVal36, *printVal37;
GtkWidget *printVal38, *printVal39, *printVal40, *printVal41, *printVal42, *printVal43, *printVal44;
GtkWidget *printVal45, *printVal46, *printVal47, *printVal48, *printVal49, *printVal50, *printVal51;
GtkWidget *printVal52, *printVal53, *printVal54, *printVal55, *printVal56, *printVal57, *printVal58;

GtkWidget *printVal71, *printVal72, *printVal73, *printVal74, *printVal75, *printVal76, *printVal77;
GtkWidget *printVal78, *printVal79, *printVal80, *printVal81, *printVal82, *printVal83, *printVal84;
GtkWidget *printVal93, *printVal94, *printVal95, *printVal96, *printVal97, *printVal98, *printVal99;
GtkWidget *printVal200, *printVal201, *printVal202, *printVal203, *printVal204, *printVal205, *printVal206;
GtkWidget *printVal207, *printVal208;

GtkWidget *printValue1, *printValue2, *printValue3, *printValue4, *printValue5, *printValue6, *printValue7;
GtkWidget *printValue8, *printValue9, *printValue10, *printValue11, *printValue12, *printValue13, *printValue14;
GtkWidget *printValue15;

GtkWidget *prinOp1, *prinOp2,*prinOp3, *prinOp4, *prinOp5, *prinOp6, *prinOp7, *prinOp8, *prinOp9, *prinOp10;
GtkWidget *prinOp11, *prinOp12, *prinOp13, *prinOp14, *prinOp15, *prinOp16, *prinOp17, *prinOp18, *prinOp19, *prinOp20; 
GtkWidget *prinOp21, *prinOp22, *prinOp23, *prinOp24, *prinOp25, *prinOp26, *prinOp27, *prinOp28, *prinOp29, *prinOp30;
GtkWidget *prinOp31, *prinOp32, *prinOp33, *prinOp34, *prinOp35, *prinOp36, *prinOp37, *prinOp38, *prinOp39, *prinOp40;

GtkWidget *prinAb1, *prinAb2, *prinAb3, *prinAb4, *prinAb5, *prinAb6, *prinAb7, *prinAb8, *prinAb9, *prinAb10;
GtkWidget *prinAb11, *prinAb12, *prinAb13, *prinAb14, *prinAb15, *prinAb16, *prinAb17, *prinAb18, *prinAb19, *prinAb20;
GtkWidget *prinAb21, *prinAb22, *prinAb23, *prinAb24, *prinAb25, *prinAb26, *prinAb27, *prinAb28, *prinAb29, *prinAb30;
GtkWidget *prinAb31, *prinAb32, *prinAb33, *prinAb34, *prinAb35, *prinAb36, *prinAb37, *prinAb38, *prinAb39, *prinAb40;

GtkWidget *entry1, *entry2, *entry3, *entry4, *entry5, *entry6, *entry7, *entry8, *entry9, *entry10, *saveEntry;
GtkWidget *entry11, *entry12, *entry13, *entry14, *entry15, *entry16, *entry17;

GtkWidget *window, *window1, *window3, *window4, *save_window, *dialog;
GtkNotebook *note1, *note2;
GtkFileChooser *chooser;

GtkWidget *da;
GtkWidget *g_lbl_download;
GtkWidget *g_lbl_PV, *g_lbl_Bat, *g_lbl_Inv, *g_lbl_ChaCon;

char pv_selection_entry[8];             // number entered by the user to select PV module
char tilt_entry[BUFFSIZ];               // tilt angle entered by the user
char pv_per_string_entry[BUFFSIZ];      // number of modules per string entered by the user
char num_pv_strings_entry[BUFFSIZ];     // number of pv strings entered by user
char battery_selection_entry[BUFFSIZ];  // battery model selected by user
char bat_per_string_entry[BUFFSIZ];     // number of batteries per string entered by user
char num_bat_strings_entry[BUFFSIZ];    // number of battery strings entered by user
char chrg_controller_entry[BUFFSIZ];    // charge controller model entered by user
char inverter_entry[BUFFSIZ];           // inverter model nubmer entered by user
char num_inverters_entry[BUFFSIZ];      // number of inverters entered by user
char azimuth_angle_entry[BUFFSIZ];      // pv azimuth angle entered by user
char reliability_entry[BUFFSIZ];        // user-specified desired minimum reliability
char numCC[BUFFSIZ];

char reliability[10];
char lon[INPUT_LENGTH];    // should be -180 to 180
char lat[INPUT_LENGTH];    // should be -90 to 90
char url[1000] = "https://power.larc.nasa.gov/cgi-bin/v1/DataAccess.py?&request=execute&identifier=SinglePoint&parameters=KT,T2M_MAX,T2M_MIN,T2M&startDate=19840101&endDate=20051231&userCommunity=SSE&tempAverage=DAILY&outputList=CSV&lat=";
static const char* pagefilename = "src/Weather Data.txt";
FILE* pagefile;
CURL *curl_handle;

char BOScost[BUFFSIZ];

float totalCost;
char dollarSignedCost[BUFFSIZ];
char taxPrice[BUFFSIZ];
char *savedLocation;
char locationFN[100];
char savedFN[100];
char *appendingFile;
char appendingLocation[100];

float dailyKTData[NUM_YEARS][NUM_DAYS];
float avgTempData[NUM_YEARS][NUM_DAYS];
float maxTempData[NUM_YEARS][NUM_DAYS];
float minTempData[NUM_YEARS][NUM_DAYS];
float avgDailyInsolation;

CURL *curl;
char TZurl[200] = "https://dev.virtualearth.net/REST/v1/timezone/";

struct posdata pd, *pdat;
struct DataValues d, *data;
struct PVData pnl, *panel;
struct BatteryData bat, *battery;
struct ChargeControllerData chrgcntrllr, *cController;
struct InverterData invrtr, *inverter;

// Reverses a string 'str' of length 'len' 
void reverse(char* str, int len) 
{ 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 
  
// Converts a given integer x to string str[].  
// d is the number of digits required in the output.  
// If d is more than the number of digits in x,  
// then 0s are added at the beginning. 
int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 

void ftoa(float n, char* res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter  
        // is needed to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
} 

int isfloat(char string[]) {
    int i = 0;
    int yesFloat = 1;
    while (string[i] != '\0') {
        if (!(isdigit(string[i]) || string[i] == '.'))
            yesFloat = 0;
        i++;
    }
    return yesFloat;
}

static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
    return written;
}

void create_library() {
// pv library reads
	FILE *pv_file = openReadFile("Library/PV Modules.csv");
    struct PVData pnl, *panel;
    panel = &pnl;

    char PVModelNames[NUM_STRINGS][STRING_SIZE];
    readModelNames(pv_file, &PVModelNames[0][0], STRING_SIZE, NUM_STRINGS);

// for opening window
    gtk_label_set_text(GTK_LABEL(prinOp1), PVModelNames[0]);
    gtk_label_set_text(GTK_LABEL(prinOp2), PVModelNames[1]);
    gtk_label_set_text(GTK_LABEL(prinOp3), PVModelNames[2]);
    gtk_label_set_text(GTK_LABEL(prinOp4), PVModelNames[3]);
    gtk_label_set_text(GTK_LABEL(prinOp5), PVModelNames[4]);
    gtk_label_set_text(GTK_LABEL(prinOp6), PVModelNames[5]);
    gtk_label_set_text(GTK_LABEL(prinOp7), PVModelNames[6]);
    gtk_label_set_text(GTK_LABEL(prinOp8), PVModelNames[7]);
    gtk_label_set_text(GTK_LABEL(prinOp9), PVModelNames[8]);
    gtk_label_set_text(GTK_LABEL(prinOp10), PVModelNames[9]);

// for about window
    gtk_label_set_text(GTK_LABEL(prinAb1), PVModelNames[0]);
    gtk_label_set_text(GTK_LABEL(prinAb2), PVModelNames[1]);
    gtk_label_set_text(GTK_LABEL(prinAb3), PVModelNames[2]);
    gtk_label_set_text(GTK_LABEL(prinAb4), PVModelNames[3]);
    gtk_label_set_text(GTK_LABEL(prinAb5), PVModelNames[4]);
    gtk_label_set_text(GTK_LABEL(prinAb6), PVModelNames[5]);
    gtk_label_set_text(GTK_LABEL(prinAb7), PVModelNames[6]);
    gtk_label_set_text(GTK_LABEL(prinAb8), PVModelNames[7]);
    gtk_label_set_text(GTK_LABEL(prinAb9), PVModelNames[8]);
    gtk_label_set_text(GTK_LABEL(prinAb10), PVModelNames[9]);

// battery library reads
    FILE *battery_file = openReadFile("Library/Batteries.csv");
    struct BatteryData bat, *battery;
    battery = &bat;

    char batteryModelNames[NUM_STRINGS][STRING_SIZE];
    readModelNames(battery_file, &batteryModelNames[0][0], STRING_SIZE, NUM_STRINGS);

    gtk_label_set_text(GTK_LABEL(prinOp11), batteryModelNames[0]);
    gtk_label_set_text(GTK_LABEL(prinOp12), batteryModelNames[1]);
    gtk_label_set_text(GTK_LABEL(prinOp13), batteryModelNames[2]);
    gtk_label_set_text(GTK_LABEL(prinOp14), batteryModelNames[3]);
    gtk_label_set_text(GTK_LABEL(prinOp15), batteryModelNames[4]);
    gtk_label_set_text(GTK_LABEL(prinOp16), batteryModelNames[5]);
    gtk_label_set_text(GTK_LABEL(prinOp17), batteryModelNames[6]);
    gtk_label_set_text(GTK_LABEL(prinOp18), batteryModelNames[7]);
    gtk_label_set_text(GTK_LABEL(prinOp19), batteryModelNames[8]);
    gtk_label_set_text(GTK_LABEL(prinOp20), batteryModelNames[9]);

    gtk_label_set_text(GTK_LABEL(prinAb11), batteryModelNames[0]);
    gtk_label_set_text(GTK_LABEL(prinAb12), batteryModelNames[1]);
    gtk_label_set_text(GTK_LABEL(prinAb13), batteryModelNames[2]);
    gtk_label_set_text(GTK_LABEL(prinAb14), batteryModelNames[3]);
    gtk_label_set_text(GTK_LABEL(prinAb15), batteryModelNames[4]);
    gtk_label_set_text(GTK_LABEL(prinAb16), batteryModelNames[5]);
    gtk_label_set_text(GTK_LABEL(prinAb17), batteryModelNames[6]);
    gtk_label_set_text(GTK_LABEL(prinAb18), batteryModelNames[7]);
    gtk_label_set_text(GTK_LABEL(prinAb19), batteryModelNames[8]);
    gtk_label_set_text(GTK_LABEL(prinAb20), batteryModelNames[9]);

// inverter library reads
    FILE *inverter_file = openReadFile("Library/Inverters.csv");
    struct InverterData inverter, *inverter1;
    inverter1 = &inverter;

    char inverterModelNames[NUM_STRINGS][STRING_SIZE];
    readModelNames(inverter_file, &inverterModelNames[0][0], STRING_SIZE, NUM_STRINGS);
   
// for opening window
    gtk_label_set_text(GTK_LABEL(prinOp21), inverterModelNames[0]);

// for about window
    gtk_label_set_text(GTK_LABEL(prinAb21), inverterModelNames[0]);

// charge controller library
    FILE *cc_file = openReadFile("Library/Charge Controllers.csv");
    struct ChargeControllerData chargecontrol, *controlcharg;
    controlcharg = &chargecontrol;

    char chargeControllerModels[NUM_STRINGS][STRING_SIZE];
    readModelNames(cc_file, &chargeControllerModels[0][0], STRING_SIZE, NUM_STRINGS);
   
// for opening window
    gtk_label_set_text(GTK_LABEL(prinOp31), chargeControllerModels[0]);

// for about window
    gtk_label_set_text(GTK_LABEL(prinAb31), chargeControllerModels[0]);
}

static gboolean
on_draw (GtkWidget *widget, cairo_t *cr, gpointer user_data)
{

    GdkRectangle da;            /* GtkDrawingArea size */
    // gdouble dx = 5.0, dy = 5.0; /* Pixels between each point */
    // gdouble i, clip_x1 = 0.0, clip_y1 = 0.0, clip_x2 = 0.0, clip_y2 = 0.0;
    GdkWindow *window = gtk_widget_get_window(widget);

    /* Determine GtkDrawingArea dimensions */
    gdk_window_get_geometry (window,
            &da.x,
            &da.y,
            &da.width,
            &da.height);

    /* Draw on a neutral background */
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1); 
	// cairo_set_source_rgb (cr, 128, 128, 128);
	cairo_stroke(cr);



    if (avgDailyInsolation > 5) {
        // green circle
        // gtk_label_set_text(GTK_LABEL(printVal29), "GOOD LOCATION");     // this isn't working for some reason
        cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
        cairo_arc(cr, 100, 60, 40, 0, 2*M_PI);
        cairo_stroke_preserve(cr);
        cairo_fill(cr);
    }
    else if (avgDailyInsolation > 4) {
        // yellow circle
        cairo_set_source_rgb(cr, 1.0, 1.0, 0.0);
        cairo_arc(cr, 100, 60, 40, 0, 2*M_PI);
        cairo_stroke_preserve(cr);
        cairo_fill(cr);
        // gtk_label_set_text(GTK_LABEL(printVal29), "MEDIOCRE LOCATION");
    }
    else {
        // red circle
        cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
        cairo_arc(cr, 100, 60, 40, 0, 2*M_PI);
        cairo_stroke_preserve(cr);
        cairo_fill(cr);
        // gtk_label_set_text(GTK_LABEL(printVal29), "BAD LOCATION");   
    }



    return TRUE;
}

int main(int argc, char *argv[])
{
    pdat = &pd;
    S_init(pdat);

    GtkBuilder      *builder, *builder1, *builder2, *builder3, *builder4; 
 
    curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade/window_main.glade");
    builder1 = gtk_builder_new_from_file("glade/window_main.glade");
    builder2 = gtk_builder_new_from_file("glade/window_main.glade");
    builder3 = gtk_builder_new_from_file("glade/window_main.glade");
    builder4 = gtk_builder_new_from_file("glade/window_main.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "inputWin"));   // user input window
    window3 = GTK_WIDGET(gtk_builder_get_object(builder1, "openWin"));  // about window
    window1 = GTK_WIDGET(gtk_builder_get_object(builder2, "window_main"));  // output window
    window4 = GTK_WIDGET(gtk_builder_get_object(builder3, "aboutWin"));  // output window
    save_window = GTK_WIDGET(gtk_builder_get_object(builder4, "save_file"));  // output window

    printValue1 = GTK_WIDGET(gtk_builder_get_object(builder, "input1"));
    printValue2 = GTK_WIDGET(gtk_builder_get_object(builder, "input2"));
    printValue3 = GTK_WIDGET(gtk_builder_get_object(builder, "input3"));
    printValue4 = GTK_WIDGET(gtk_builder_get_object(builder, "input4"));
    printValue5 = GTK_WIDGET(gtk_builder_get_object(builder, "input5"));
    printValue6 = GTK_WIDGET(gtk_builder_get_object(builder, "input6"));
    printValue7 = GTK_WIDGET(gtk_builder_get_object(builder, "input7"));
    printValue8 = GTK_WIDGET(gtk_builder_get_object(builder, "input8"));
    printValue9 = GTK_WIDGET(gtk_builder_get_object(builder, "input9"));
    printValue10 = GTK_WIDGET(gtk_builder_get_object(builder, "input10"));
    printValue11 = GTK_WIDGET(gtk_builder_get_object(builder, "input11"));
    printValue12 = GTK_WIDGET(gtk_builder_get_object(builder, "input12"));
    printValue13 = GTK_WIDGET(gtk_builder_get_object(builder, "input13"));
    printValue14 = GTK_WIDGET(gtk_builder_get_object(builder, "input14"));
    printValue15 = GTK_WIDGET(gtk_builder_get_object(builder, "input15"));

    printVal4 = GTK_WIDGET(gtk_builder_get_object(builder2, "label4"));
    printVal5 = GTK_WIDGET(gtk_builder_get_object(builder2, "label5"));
    printVal6 = GTK_WIDGET(gtk_builder_get_object(builder2, "label6"));
    printVal7 = GTK_WIDGET(gtk_builder_get_object(builder2, "label7"));
    printVal8 = GTK_WIDGET(gtk_builder_get_object(builder2, "label8"));
    printVal9 = GTK_WIDGET(gtk_builder_get_object(builder2, "label9"));
    printVal10 = GTK_WIDGET(gtk_builder_get_object(builder2, "label10"));
    printVal11 = GTK_WIDGET(gtk_builder_get_object(builder2, "label11"));
    printVal12 = GTK_WIDGET(gtk_builder_get_object(builder2, "label12"));
    printVal13 = GTK_WIDGET(gtk_builder_get_object(builder2, "label13"));
    printVal14 = GTK_WIDGET(gtk_builder_get_object(builder2, "label14"));
    printVal15 = GTK_WIDGET(gtk_builder_get_object(builder2, "label15"));
    printVal16 = GTK_WIDGET(gtk_builder_get_object(builder2, "label16"));
    printVal17 = GTK_WIDGET(gtk_builder_get_object(builder2, "label17"));
    printVal18 = GTK_WIDGET(gtk_builder_get_object(builder2, "label18"));
    printVal19 = GTK_WIDGET(gtk_builder_get_object(builder2, "label19"));
    printVal20 = GTK_WIDGET(gtk_builder_get_object(builder2, "label20"));
    printVal21 = GTK_WIDGET(gtk_builder_get_object(builder2, "label21"));
    printVal22 = GTK_WIDGET(gtk_builder_get_object(builder2, "label22"));
    printVal23 = GTK_WIDGET(gtk_builder_get_object(builder2, "label23"));
    printVal24 = GTK_WIDGET(gtk_builder_get_object(builder2, "label24"));
    printVal25 = GTK_WIDGET(gtk_builder_get_object(builder2, "label25"));
    
    printVal26 = GTK_WIDGET(gtk_builder_get_object(builder2, "label26"));
    printVal27 = GTK_WIDGET(gtk_builder_get_object(builder2, "label27"));
    printVal28 = GTK_WIDGET(gtk_builder_get_object(builder2, "label28"));
    printVal29 = GTK_WIDGET(gtk_builder_get_object(builder, "label29"));
    printVal30 = GTK_WIDGET(gtk_builder_get_object(builder2, "label30"));
    printVal31 = GTK_WIDGET(gtk_builder_get_object(builder2, "label31"));
    printVal32 = GTK_WIDGET(gtk_builder_get_object(builder2, "label32"));
    printVal33 = GTK_WIDGET(gtk_builder_get_object(builder2, "label33"));
    printVal34 = GTK_WIDGET(gtk_builder_get_object(builder2, "label34"));
    printVal35 = GTK_WIDGET(gtk_builder_get_object(builder2, "label35"));
    printVal36 = GTK_WIDGET(gtk_builder_get_object(builder2, "label36"));
    printVal37 = GTK_WIDGET(gtk_builder_get_object(builder2, "label37"));
    printVal38 = GTK_WIDGET(gtk_builder_get_object(builder2, "label38"));
    printVal39 = GTK_WIDGET(gtk_builder_get_object(builder2, "label39"));
    printVal40 = GTK_WIDGET(gtk_builder_get_object(builder2, "label40"));
    printVal41 = GTK_WIDGET(gtk_builder_get_object(builder2, "label41"));
    printVal42 = GTK_WIDGET(gtk_builder_get_object(builder2, "label42"));
    printVal43 = GTK_WIDGET(gtk_builder_get_object(builder2, "label43"));
    printVal44 = GTK_WIDGET(gtk_builder_get_object(builder2, "label44"));
    printVal45 = GTK_WIDGET(gtk_builder_get_object(builder2, "label45"));
    printVal46 = GTK_WIDGET(gtk_builder_get_object(builder2, "label46"));
    printVal47 = GTK_WIDGET(gtk_builder_get_object(builder2, "label47"));
    printVal48 = GTK_WIDGET(gtk_builder_get_object(builder2, "label48"));
    printVal49 = GTK_WIDGET(gtk_builder_get_object(builder2, "label49"));
    printVal50 = GTK_WIDGET(gtk_builder_get_object(builder2, "label50"));
    printVal51 = GTK_WIDGET(gtk_builder_get_object(builder2, "label51"));
    printVal52 = GTK_WIDGET(gtk_builder_get_object(builder2, "label52"));
    printVal53 = GTK_WIDGET(gtk_builder_get_object(builder2, "label53"));
    printVal54 = GTK_WIDGET(gtk_builder_get_object(builder2, "label54"));
    printVal55 = GTK_WIDGET(gtk_builder_get_object(builder2, "label55"));
    printVal56 = GTK_WIDGET(gtk_builder_get_object(builder2, "label56"));
    printVal57 = GTK_WIDGET(gtk_builder_get_object(builder2, "label57"));
    printVal58 = GTK_WIDGET(gtk_builder_get_object(builder2, "label58"));
    printVal70 = GTK_WIDGET(gtk_builder_get_object(builder2, "label70"));

    printVal71 = GTK_WIDGET(gtk_builder_get_object(builder2, "label71"));
    printVal72 = GTK_WIDGET(gtk_builder_get_object(builder2, "label72"));
    printVal73 = GTK_WIDGET(gtk_builder_get_object(builder2, "label73"));
    printVal74 = GTK_WIDGET(gtk_builder_get_object(builder2, "label74"));
    printVal75 = GTK_WIDGET(gtk_builder_get_object(builder2, "label75"));
    printVal76 = GTK_WIDGET(gtk_builder_get_object(builder2, "label76"));    // validate pv specifiations
    printVal77 = GTK_WIDGET(gtk_builder_get_object(builder2, "label77"));
    printVal78 = GTK_WIDGET(gtk_builder_get_object(builder2, "label78"));

    // labels for about window
    prinOp1 = GTK_WIDGET(gtk_builder_get_object(builder1, "open1")); 
    prinOp2 = GTK_WIDGET(gtk_builder_get_object(builder1, "open2")); 
    prinOp3 = GTK_WIDGET(gtk_builder_get_object(builder1, "open3")); 
    prinOp4 = GTK_WIDGET(gtk_builder_get_object(builder1, "open4")); 
    prinOp5 = GTK_WIDGET(gtk_builder_get_object(builder1, "open5")); 
    prinOp6 = GTK_WIDGET(gtk_builder_get_object(builder1, "open6")); 
    prinOp7 = GTK_WIDGET(gtk_builder_get_object(builder1, "open7")); 
    prinOp8 = GTK_WIDGET(gtk_builder_get_object(builder1, "open8")); 
    prinOp9 = GTK_WIDGET(gtk_builder_get_object(builder1, "open9")); 
    prinOp10 = GTK_WIDGET(gtk_builder_get_object(builder1, "open10")); 
    prinOp11 = GTK_WIDGET(gtk_builder_get_object(builder1, "open11")); 
    prinOp12 = GTK_WIDGET(gtk_builder_get_object(builder1, "open12")); 
    prinOp13 = GTK_WIDGET(gtk_builder_get_object(builder1, "open13")); 
    prinOp14 = GTK_WIDGET(gtk_builder_get_object(builder1, "open14")); 
    prinOp15 = GTK_WIDGET(gtk_builder_get_object(builder1, "open15")); 
    prinOp16 = GTK_WIDGET(gtk_builder_get_object(builder1, "open16")); 
    prinOp17 = GTK_WIDGET(gtk_builder_get_object(builder1, "open17")); 
    prinOp18 = GTK_WIDGET(gtk_builder_get_object(builder1, "open18")); 
    prinOp19 = GTK_WIDGET(gtk_builder_get_object(builder1, "open19")); 
    prinOp20 = GTK_WIDGET(gtk_builder_get_object(builder1, "open20")); 
    prinOp21 = GTK_WIDGET(gtk_builder_get_object(builder1, "open21")); 
    prinOp31 = GTK_WIDGET(gtk_builder_get_object(builder1, "open31")); 

    prinAb1 = GTK_WIDGET(gtk_builder_get_object(builder3, "about1")); 
    prinAb2 = GTK_WIDGET(gtk_builder_get_object(builder3, "about2")); 
    prinAb3 = GTK_WIDGET(gtk_builder_get_object(builder3, "about3")); 
    prinAb4 = GTK_WIDGET(gtk_builder_get_object(builder3, "about4")); 
    prinAb5 = GTK_WIDGET(gtk_builder_get_object(builder3, "about5")); 
    prinAb6 = GTK_WIDGET(gtk_builder_get_object(builder3, "about6")); 
    prinAb7 = GTK_WIDGET(gtk_builder_get_object(builder3, "about7")); 
    prinAb8 = GTK_WIDGET(gtk_builder_get_object(builder3, "about8")); 
    prinAb9 = GTK_WIDGET(gtk_builder_get_object(builder3, "about9")); 
    prinAb10 = GTK_WIDGET(gtk_builder_get_object(builder3, "about10")); 
    prinAb11 = GTK_WIDGET(gtk_builder_get_object(builder3, "about11")); 
    prinAb12 = GTK_WIDGET(gtk_builder_get_object(builder3, "about12")); 
    prinAb13 = GTK_WIDGET(gtk_builder_get_object(builder3, "about13")); 
    prinAb14 = GTK_WIDGET(gtk_builder_get_object(builder3, "about14")); 
    prinAb15 = GTK_WIDGET(gtk_builder_get_object(builder3, "about15")); 
    prinAb16 = GTK_WIDGET(gtk_builder_get_object(builder3, "about16")); 
    prinAb17 = GTK_WIDGET(gtk_builder_get_object(builder3, "about17")); 
    prinAb18 = GTK_WIDGET(gtk_builder_get_object(builder3, "about18")); 
    prinAb19 = GTK_WIDGET(gtk_builder_get_object(builder3, "about19")); 
    prinAb20 = GTK_WIDGET(gtk_builder_get_object(builder3, "about20")); 
    prinAb21 = GTK_WIDGET(gtk_builder_get_object(builder3, "about21")); 
    prinAb31 = GTK_WIDGET(gtk_builder_get_object(builder3, "about31")); 

    printVal93 = GTK_WIDGET(gtk_builder_get_object(builder2, "label93")); 
    printVal94 = GTK_WIDGET(gtk_builder_get_object(builder2, "label94"));   // warning or loading label

    printVal95 = GTK_WIDGET(gtk_builder_get_object(builder, "label95"));
    printVal96 = GTK_WIDGET(gtk_builder_get_object(builder2, "label96"));
    printVal97 = GTK_WIDGET(gtk_builder_get_object(builder2, "label97"));
    printVal99 = GTK_WIDGET(gtk_builder_get_object(builder2, "label99"));

    printVal200 = GTK_WIDGET(gtk_builder_get_object(builder2, "label200"));
    printVal201 = GTK_WIDGET(gtk_builder_get_object(builder2, "label201"));
    printVal202 = GTK_WIDGET(gtk_builder_get_object(builder2, "label202"));
    printVal203 = GTK_WIDGET(gtk_builder_get_object(builder2, "label203"));
    printVal204 = GTK_WIDGET(gtk_builder_get_object(builder2, "label204"));
    printVal205 = GTK_WIDGET(gtk_builder_get_object(builder2, "label205"));
    printVal206 = GTK_WIDGET(gtk_builder_get_object(builder2, "label206"));
    printVal207 = GTK_WIDGET(gtk_builder_get_object(builder2, "label207"));
    printVal208 = GTK_WIDGET(gtk_builder_get_object(builder2, "label208"));

    da = GTK_WIDGET(gtk_builder_get_object(builder, "draw1"));
    entry1 = GTK_WIDGET(gtk_builder_get_object(builder, "entry1"));
    entry2 = GTK_WIDGET(gtk_builder_get_object(builder, "entry2"));
    entry3 = GTK_WIDGET(gtk_builder_get_object(builder, "entry3"));
    entry4 = GTK_WIDGET(gtk_builder_get_object(builder, "entry4"));
    entry5 = GTK_WIDGET(gtk_builder_get_object(builder, "entry5"));
    entry6 = GTK_WIDGET(gtk_builder_get_object(builder, "entry6"));
    entry7 = GTK_WIDGET(gtk_builder_get_object(builder, "entry7"));
    entry8 = GTK_WIDGET(gtk_builder_get_object(builder, "entry8"));
    entry9 = GTK_WIDGET(gtk_builder_get_object(builder, "entry9"));
    entry10 = GTK_WIDGET(gtk_builder_get_object(builder, "entry10"));
    entry11 = GTK_WIDGET(gtk_builder_get_object(builder, "entry11"));
    entry12 = GTK_WIDGET(gtk_builder_get_object(builder, "entry12"));
    entry13 = GTK_WIDGET(gtk_builder_get_object(builder, "entry13"));
    entry14 = GTK_WIDGET(gtk_builder_get_object(builder, "entry14"));
    entry15 = GTK_WIDGET(gtk_builder_get_object(builder, "entry15"));
    entry16 = GTK_WIDGET(gtk_builder_get_object(builder2, "entry16"));
    entry17 = GTK_WIDGET(gtk_builder_get_object(builder2, "entry17"));
    saveEntry = GTK_WIDGET(gtk_builder_get_object(builder4, "saveEntry"));

    dialog = GTK_WIDGET(gtk_builder_get_object(builder4, "save_file"));
    chooser = GTK_FILE_CHOOSER(dialog);

    note1 = GTK_NOTEBOOK(gtk_builder_get_object(builder, "notebook1"));
    note2 = GTK_NOTEBOOK(gtk_builder_get_object(builder1, "notebook2"));

    g_lbl_download = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_download"));
    g_lbl_PV = GTK_WIDGET(gtk_builder_get_object(builder2 , "lbl_PV"));
    g_lbl_Bat = GTK_WIDGET(gtk_builder_get_object(builder2, "lbl_Bat"));
    g_lbl_Inv = GTK_WIDGET(gtk_builder_get_object(builder2, "lbl_Inv"));
    g_lbl_ChaCon = GTK_WIDGET(gtk_builder_get_object(builder2, "lbl_ChaCon"));

    gtk_builder_connect_signals(builder, NULL);
    gtk_builder_connect_signals(builder1, NULL);
    gtk_builder_connect_signals(builder2, NULL);
    gtk_builder_connect_signals(builder3, NULL);
    gtk_builder_connect_signals(builder4, NULL);


    // g_signal_connect (G_OBJECT (da),
    //         "draw",
    //         G_CALLBACK (on_draw),
    //         NULL);

    g_object_unref(builder);
    g_object_unref(builder1);
    g_object_unref(builder2);
    g_object_unref(builder3);
    g_object_unref(builder4);

    create_library();

    gtk_widget_show(window3);
    gtk_widget_show(window); 
    gtk_widget_hide(window1);
    gtk_widget_hide(window4);
    gtk_widget_hide(save_window);
    gtk_main();


    return 0;
}

void figure_out_price(struct PVData *panel, struct BatteryData *battery, struct InverterData *inverter, struct ChargeControllerData *controlcharg) {

    strcpy(dollarSignedCost, "$ ");

    float panelCost = panel->cellPrice;
    float battCost = battery->cellPrice;
    float invtCost = inverter->price;
    float ccCost = controlcharg->price;


    totalCost = (strtof(pv_per_string_entry, NULL)*strtof(num_pv_strings_entry, NULL))*panelCost + 
                        (strtof(bat_per_string_entry, NULL)*strtof(num_bat_strings_entry, NULL))* battCost +
                        strtof(num_inverters_entry, NULL) * invtCost +
                        strtof(numCC, NULL) * ccCost;

    char buffer[BUFFSIZ];
    ftoa(totalCost, buffer, 2);
    strcat(dollarSignedCost, buffer);

    gtk_label_set_text(GTK_LABEL(printVal38), dollarSignedCost);

    char buffer1[BUFFSIZ];
    char buffer2[BUFFSIZ];

    // char completeCost[BUFFSIZ];

    // ftoa(totalCostWithTaxAndBOS, completeCost, 2);
    // strcat(dollarSignedCost, completeCost);

    // pv component capital cost
    float totalPV = (strtof(pv_per_string_entry, NULL)*strtof(num_pv_strings_entry, NULL));
    ftoa(totalPV, buffer1, 0);
    gtk_label_set_text(GTK_LABEL(printVal30), buffer1);    

    char dollarSignedPVCost[BUFFSIZ];
    strcpy(dollarSignedPVCost, "$ ");

    float PVtotalCost = totalPV * panelCost;
    ftoa(PVtotalCost, buffer2, 2);
    strcat(dollarSignedPVCost, buffer2);
    gtk_label_set_text(GTK_LABEL(printVal31), dollarSignedPVCost);

    // battery component capital cost
    float totalBatteries = (strtof(bat_per_string_entry, NULL)*strtof(num_bat_strings_entry, NULL));
    ftoa(totalBatteries, buffer1, 0);
    gtk_label_set_text(GTK_LABEL(printVal32), buffer1);

    char dollarSignedBatteryCost[BUFFSIZ];
    strcpy(dollarSignedBatteryCost, "$ ");

    float BatteriesTotalCost = totalBatteries * battCost;
    ftoa(BatteriesTotalCost, buffer2, 2);
    strcat(dollarSignedBatteryCost, buffer2);
    gtk_label_set_text(GTK_LABEL(printVal33), dollarSignedBatteryCost); 

    // inverter component cost
    float totalInverters = strtof(num_inverters_entry, NULL);
    gtk_label_set_text(GTK_LABEL(printVal34), num_inverters_entry);

    char dollarSignedInverterCost[BUFFSIZ];
    strcpy(dollarSignedInverterCost, "$ ");

    float InverterTotalCost = totalInverters * invtCost;
    ftoa(InverterTotalCost, buffer2, 2);
    strcat(dollarSignedInverterCost, buffer2);
    gtk_label_set_text(GTK_LABEL(printVal35), dollarSignedInverterCost);

    // charge controler component capital cost
    float totalCC = strtof(numCC, NULL);
    gtk_label_set_text(GTK_LABEL(printVal36), numCC);

    char dollarSignedCCcost[BUFFSIZ];
    strcpy(dollarSignedCCcost, "$ ");

    float CCtotalCost = totalCC * ccCost;
    ftoa(CCtotalCost, buffer2, 2);
    strcat(dollarSignedCCcost, buffer2);
    gtk_label_set_text(GTK_LABEL(printVal37), dollarSignedCCcost); 

}

void sun() {
    // struct posdata pd, *pdat; /* declare a posdata struct and a pointer for
    //                                 it (if desired, the structure could be
    //                                 allocated dynamically with malloc) */
    long retval;              /* to capture S_solpos return codes */

    pdat = &pd;
    S_init (pdat);        

// // ---------------------- ECE 21.3 CODE BEGIN ------------------------------------
   
    float lon1;
    lon1 = strtof(lon, '\0');

    float lat1;
    lat1 = strtof(lat, '\0');

    pdat->longitude = lon1;  /* Note that latitude and longitude are  */
    pdat->latitude  =  lat1;  /*   in DECIMAL DEGREES, not Deg/Min/Sec */
    // pdat->longitude = LUSAKA_LON;  /* Note that latitude and longitude are  */
    // pdat->latitude  = LUSAKA_LAT;  /*   in DECIMAL DEGREES, not Deg/Min/Sec */


    pdat->tilt = strtof(tilt_entry, '\0');
    pdat->aspect = strtof(azimuth_angle_entry, '\0');

    pdat->year      = 1984;    /* The year is 1984. */
    pdat->daynum    =  1;    /* July 22nd, the 203'rd day of the year (the
                                algorithm will compensate for leap year, so
                                you just count days). S_solpos can be
                                configured to accept month-day dates; see
                                examples below.) */

    /* The time of day (STANDARD time) */

    pdat->hour      = 0;
    pdat->minute    = 0;
    pdat->second    = 0;


    FILE *weatherData;
    char FILE_NAME[100] = "src/Weather Data.txt";

    weatherData = fopen( FILE_NAME, "r");
    readAllData(pdat, weatherData, dailyKTData, avgTempData, maxTempData, minTempData);



    data = &d;

	FILE *timezone_file = fopen("src/Time Zone Data.txt", "r");
	importTimeZone(timezone_file, pdat);
	// pdat->timezone = 8;
	char tzString[2];
	sprintf(tzString, "%.0f", pdat->timezone);
	gtk_label_set_text(GTK_LABEL(printVal73), tzString);

	int pv_selection = atoi(pv_selection_entry);
    FILE *pv_file = openReadFile("Library/PV Modules.csv");
    panel = &pnl;
	importPVLibrary(pv_file, panel, pv_selection);
	panel->stringSize = strtol(num_pv_strings_entry, '\0', 10);
	panel->numStrings = strtol(pv_per_string_entry, '\0', 10);
	panel->arrayMaxPower = panel->stringSize * panel->numStrings * panel->cellMaxPower;
	panel->arrayVoc = panel->cellVoc * panel->stringSize;

	char pvArrayV_str[7];
	sprintf(pvArrayV_str, "%.1f", panel->arrayVoc);
    gtk_label_set_text(GTK_LABEL(printVal74), pvArrayV_str);


    int bat_selection = atoi(battery_selection_entry);
	FILE *battery_file = openReadFile("Library/Batteries.csv");
    battery = &bat;
    battery->numStrings = strtol(bat_per_string_entry, '\0', 10);
    battery->stringSize = strtol(num_bat_strings_entry, '\0', 10);
	initializeBattery(battery);
	importBatteryLibrary(battery_file, battery, bat_selection);
    setBatteryArray(battery);

	char batArrayV_str[7];
	sprintf(batArrayV_str, "%.1f", battery->arrayVoltage);
    gtk_label_set_text(GTK_LABEL(printVal75), batArrayV_str);


    int chargeController_selection = atoi(chrg_controller_entry);
    FILE *chargeController_file = openReadFile("Library/Charge Controllers.csv");
    cController = &chrgcntrllr;
    importChargeControllerLibrary(chargeController_file, cController, chargeController_selection);


    // gtk_label_set_text(GTK_LABEL(printVal76), cController->model);

    int inverter_selection = atoi(inverter_entry);
    FILE *inverter_file = openReadFile("Library/Inverters.csv");
    inverter = &invrtr;
    importInverterLibrary(inverter_file, inverter, inverter_selection);


    // gtk_label_set_text(GTK_LABEL(printVal75), inverter->model);

    gtk_label_set_text(GTK_LABEL(g_lbl_PV), panel->model);        // insted of 'printVal 39' use what you have
    // gtk_label_set_text(GTK_LABEL(printVal73), pv_selection_entry);

    gtk_label_set_text(GTK_LABEL(g_lbl_Bat), battery->model);      // insted of 'printVal 39' use what you have
    // gtk_label_set_text(GTK_LABEL(printVal74), battery_selection_entry);
    
    gtk_label_set_text(GTK_LABEL(g_lbl_Inv), inverter->model);
    // gtk_label_set_text(GTK_LABEL(printVal75), inverter_entry);

    gtk_label_set_text(GTK_LABEL(g_lbl_ChaCon), cController->model);
    // gtk_label_set_text(GTK_LABEL(printVal76), chrg_controller_entry);

    // figure_out_price(panel, battery);
    figure_out_price(panel, battery, inverter, cController);

    float dailyPowerData[NUM_YEARS][NUM_DAYS] = {0};

    float ETinsolation = 0;
    float globInsolation = 0;
    float dailyPower = 0;
    float SoC = (1 + battery->minSoC) / 2;  // start halfway between fully charged and minimum state of charge
    float loadCurrent;      // negative is current into the battery, positive is current out of the battery

    FILE *load_profile_file = fopen("Library/Load Profile.csv", "r");
    readLoadProfile(load_profile_file, data->loadProfile);




    float ***hourlyTemp	= (float ***)malloc(sizeof(float **) * NUM_YEARS);
    float ***cellTemp	= (float ***)malloc(sizeof(float **) * NUM_YEARS);
    float ***pvPower	= (float ***)malloc(sizeof(float **) * NUM_YEARS);
    for (int i = 0; i < NUM_YEARS; i++) {
        hourlyTemp[i]	= (float **)malloc(sizeof(float *) * NUM_DAYS);
        cellTemp[i]		= (float **)malloc(sizeof(float *) * NUM_DAYS);
        pvPower[i]		= (float **)malloc(sizeof(float *) * NUM_DAYS);

        for (int j = 0; j < NUM_DAYS; j++) {
            hourlyTemp[i][j]	= (float *)malloc(sizeof(float) * NUM_HOURS);
            cellTemp[i][j] 		= (float *)malloc(sizeof(float) * NUM_HOURS);
            pvPower[i][j] 		= (float *)malloc(sizeof(float) * NUM_HOURS);
        }
    }

    for (pdat->year = START_YEAR; pdat->year < START_YEAR + NUM_YEARS; pdat->year++) {
        for (pdat->daynum = 1; pdat->daynum < 366 || (pdat->daynum < 367 && pdat->year % 4 == 0); pdat->daynum++) {   // while the day is less than 12/31 on a leap-year, and it is not the 366th day of the year on a non-leap-year
            for (pdat->hour = 0; pdat->hour < NUM_HOURS; pdat->hour++) {

                retval = S_solpos (pdat);  /* S_solpos function call */
                S_decode(retval, pdat);    /* ALWAYS look at the return code! */

				data->insolation[pdat->year-START_YEAR][pdat->daynum][pdat->hour] = calcAvgIrradianceTilt(pdat) * dailyKTData[pdat->year-START_YEAR][pdat->daynum];

                ETinsolation += pdat->etr;
                globInsolation += data->insolation[pdat->year-START_YEAR][pdat->daynum][pdat->hour];

                hourlyTemp[pdat->year-START_YEAR][pdat->daynum][pdat->hour] = calcAmbientTemp(pdat->hour, pdat->ssetr,
                    minTempData[pdat->year-START_YEAR][pdat->daynum], maxTempData[pdat->year-START_YEAR][pdat->daynum],
                    avgTempData[pdat->year-START_YEAR][pdat->daynum]);

                cellTemp[pdat->year-START_YEAR][pdat->daynum][pdat->hour] = calcCellTemp(hourlyTemp[pdat->year-START_YEAR][pdat->daynum][pdat->hour],
                    panel->noct, data->insolation[pdat->year-START_YEAR][pdat->daynum][pdat->hour]);

                pvPower[pdat->year-START_YEAR][pdat->daynum][pdat->hour] = calcPVPower(panel->arrayMaxPower, data->insolation[pdat->year-START_YEAR][pdat->daynum][pdat->hour],
                    panel->alphaP, cellTemp[pdat->year-START_YEAR][pdat->daynum][pdat->hour]);

                dailyPower += pvPower[pdat->year-START_YEAR][pdat->daynum][pdat->hour];
                loadCurrent = (data->loadProfile[pdat->hour] - pvPower[pdat->year-START_YEAR][pdat->daynum][pdat->hour]) / battery->arrayVoltage;


                data->hourlySoC[pdat->year-START_YEAR][pdat->daynum][pdat->hour] = calcBatteryCharge(battery, inverter, SoC, loadCurrent);
				SoC = data->hourlySoC[pdat->year-START_YEAR][pdat->daynum][pdat->hour];
            }
            dailyPowerData[pdat->year-START_YEAR][pdat->daynum] = dailyPower;
            dailyPower = 0;
        }
    }


    storeData(pdat, data, battery, pvPower);
    // gtk_label_set_text(GTK_LABEL(printVal4), "running");

// grid output
    char avgBuffer[BUFFSIZ];
    float avgOutput = data->avgMonthPVOutput[1];
    ftoa(avgOutput, avgBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal4), avgBuffer);

    avgOutput = data->avgMonthPVOutput[2];
    ftoa(avgOutput, avgBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal5), avgBuffer);

    avgOutput = data->avgMonthPVOutput[3];
    ftoa(avgOutput, avgBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal6), avgBuffer);

    avgOutput = data->avgMonthPVOutput[4];
    ftoa(avgOutput, avgBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal7), avgBuffer);

    avgOutput = data->avgMonthPVOutput[5];
    ftoa(avgOutput, avgBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal8), avgBuffer);

    avgOutput = data->avgMonthPVOutput[6];
    ftoa(avgOutput, avgBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal9), avgBuffer);

    avgOutput = data->avgMonthPVOutput[7];
    ftoa(avgOutput, avgBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal10), avgBuffer);

    avgOutput = data->avgMonthPVOutput[8];
    ftoa(avgOutput, avgBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal11), avgBuffer);

    avgOutput = data->avgMonthPVOutput[9];
    ftoa(avgOutput, avgBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal12), avgBuffer);

    avgOutput = data->avgMonthPVOutput[10];
    ftoa(avgOutput, avgBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal13), avgBuffer);

    avgOutput = data->avgMonthPVOutput[11];
    ftoa(avgOutput, avgBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal14), avgBuffer);

    avgOutput = data->avgMonthPVOutput[12];
    ftoa(avgOutput, avgBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal15), avgBuffer);


    // printing the WORST average for the month of a year
    char worstBuffer[BUFFSIZ];
    int yearOfWorst = data->monthMinPVOutput[1];
    float monthOfWorst = data->eachMonthAvgPVOutput[yearOfWorst-START_YEAR][1];
    ftoa(monthOfWorst, worstBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal16), worstBuffer);

    yearOfWorst = data->monthMinPVOutput[2];
    monthOfWorst = data->eachMonthAvgPVOutput[yearOfWorst-START_YEAR][2];
    ftoa(monthOfWorst, worstBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal17), worstBuffer);

    yearOfWorst = data->monthMinPVOutput[3];
    monthOfWorst = data->eachMonthAvgPVOutput[yearOfWorst-START_YEAR][3];
    ftoa(monthOfWorst, worstBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal18), worstBuffer);

    yearOfWorst = data->monthMinPVOutput[4];
    monthOfWorst = data->eachMonthAvgPVOutput[yearOfWorst-START_YEAR][4];
    ftoa(monthOfWorst, worstBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal19), worstBuffer);

    yearOfWorst = data->monthMinPVOutput[5];
    monthOfWorst = data->eachMonthAvgPVOutput[yearOfWorst-START_YEAR][5];
    ftoa(monthOfWorst, worstBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal20), worstBuffer);

    yearOfWorst = data->monthMinPVOutput[6];
    monthOfWorst = data->eachMonthAvgPVOutput[yearOfWorst-START_YEAR][6];
    ftoa(monthOfWorst, worstBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal21), worstBuffer);

    yearOfWorst = data->monthMinPVOutput[7];
    monthOfWorst = data->eachMonthAvgPVOutput[yearOfWorst-START_YEAR][7];
    ftoa(monthOfWorst, worstBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal22), worstBuffer);

    yearOfWorst = data->monthMinPVOutput[8];
    monthOfWorst = data->eachMonthAvgPVOutput[yearOfWorst-START_YEAR][8];
    ftoa(monthOfWorst, worstBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal23), worstBuffer);

    yearOfWorst = data->monthMinPVOutput[9];
    monthOfWorst = data->eachMonthAvgPVOutput[yearOfWorst-START_YEAR][9];
    ftoa(monthOfWorst, worstBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal24), worstBuffer);

    yearOfWorst = data->monthMinPVOutput[10];
    monthOfWorst = data->eachMonthAvgPVOutput[yearOfWorst-START_YEAR][10];
    ftoa(monthOfWorst, worstBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal25), worstBuffer);

    yearOfWorst = data->monthMinPVOutput[11];
    monthOfWorst = data->eachMonthAvgPVOutput[yearOfWorst-START_YEAR][11];
    ftoa(monthOfWorst, worstBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal26), worstBuffer);

    yearOfWorst = data->monthMinPVOutput[12];
    monthOfWorst = data->eachMonthAvgPVOutput[yearOfWorst-START_YEAR][12];
    ftoa(monthOfWorst, worstBuffer, 2);
    gtk_label_set_text(GTK_LABEL(printVal27), worstBuffer);


	/***** STATE OF CHARGE TAB **************************************/
    // MAX SoC
    char maxSoCText[7];
    sprintf (maxSoCText, "%.2f", data->allMaxSoC * 100);
    strcat(maxSoCText, "%");
    gtk_label_set_text(GTK_LABEL(printVal205), maxSoCText);

    // MIN SoC
    char minSoCText[7];
    sprintf (minSoCText, "%.2f", data->allMinSoC * 100);
    strcat(minSoCText, "%");
    gtk_label_set_text(GTK_LABEL(printVal206), minSoCText);

	// SoC histogram ranges
	char SoCRanges_str[6][20];
	char temp[20];
	for (int i = 0; i < 5; i++) {
		sprintf (SoCRanges_str[i], "%.1f", data->SoCIntervalRanges[i] * 100);
		strcat(SoCRanges_str[i], " - ");
		sprintf(temp, "%.1f", data->SoCIntervalRanges[i+1] * 100);
		strcat(SoCRanges_str[i], temp);
	}
	sprintf (SoCRanges_str[5], "%.1f", data->SoCIntervalRanges[5] * 100);
	strcat(SoCRanges_str[5], " - ");
	strcat(SoCRanges_str[5], "100");

	gtk_label_set_text(GTK_LABEL(printVal41), strcat(SoCRanges_str[0], " %"));
	gtk_label_set_text(GTK_LABEL(printVal42), strcat(SoCRanges_str[1], " %"));
	gtk_label_set_text(GTK_LABEL(printVal43), strcat(SoCRanges_str[2], " %"));
	gtk_label_set_text(GTK_LABEL(printVal44), strcat(SoCRanges_str[3], " %"));
	gtk_label_set_text(GTK_LABEL(printVal45), strcat(SoCRanges_str[4], " %"));
	gtk_label_set_text(GTK_LABEL(printVal46), strcat(SoCRanges_str[5], " %"));

	// SoC histogram time in PERCENTAGE
	char SoCProportion_str[6][20];
	for (int i = 0; i < 6; i++)
		sprintf (SoCProportion_str[i], "%.1f", data->SoCIntervalProportion[i] * 100);

	gtk_label_set_text(GTK_LABEL(printVal47), SoCProportion_str[0]);
	gtk_label_set_text(GTK_LABEL(printVal48), SoCProportion_str[1]);
	gtk_label_set_text(GTK_LABEL(printVal49), SoCProportion_str[2]);
	gtk_label_set_text(GTK_LABEL(printVal50), SoCProportion_str[3]);
	gtk_label_set_text(GTK_LABEL(printVal51), SoCProportion_str[4]);
	gtk_label_set_text(GTK_LABEL(printVal52), SoCProportion_str[5]);

	// SoC histogram time in HOURS PER YEAR
	char SoCTime_str[6][20];
	for (int i = 0; i < 6; i++)
		sprintf (SoCTime_str[i], "%i", data->SoCIntervalTime[i]);

	gtk_label_set_text(GTK_LABEL(printVal53), SoCTime_str[0]);
	gtk_label_set_text(GTK_LABEL(printVal54), SoCTime_str[1]);
	gtk_label_set_text(GTK_LABEL(printVal55), SoCTime_str[2]);
	gtk_label_set_text(GTK_LABEL(printVal56), SoCTime_str[3]);
	gtk_label_set_text(GTK_LABEL(printVal57), SoCTime_str[4]);
	gtk_label_set_text(GTK_LABEL(printVal58), SoCTime_str[5]);
	/*****************************************************************/

        
    // reliability print
	char reliability[7];
    sprintf (reliability, "%.2f", calcReliability(battery, data->hourlySoC) * 100);
    strcat(reliability, "%");
    gtk_label_set_text(GTK_LABEL(printVal99), reliability);

    // total PV output average
    char pvOutput[BUFFSIZ];
    sprintf (pvOutput, "%.2f", data->allAvgPVOutput);
    gtk_label_set_text(GTK_LABEL(printVal28), pvOutput);
    

    // gtk_label_set_text(GTK_LABEL(printVal71), "deallocating memory...");
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
}

void curly(char *u, FILE *current_file) {
    if(curl_handle) {
        /* set URL to get here */
        curl_easy_setopt(curl_handle, CURLOPT_URL, u);

        /* Switch on full protocol/debug output while testing */
        curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

        /* disable progress meter, set to 0L to enable it */
        curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

        /* send all data to this function  */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

        /* open the file */
        if (current_file) {

            /* write the page body to this file handle */
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, current_file);

            /* get it! */
            curl_easy_perform(curl_handle);

            /* Downloading Completely */
            gtk_label_set_text(GTK_LABEL(g_lbl_download), "Downloaded Completely");

            /* close the header file */
            fclose(current_file);
        }

        /* cleanup curl stuff */
        curl_easy_cleanup(curl_handle);
        curl_global_cleanup();
    }
}

void concat(char *la, char *lo) {
    strcat(url, la);
    strcat(url, "&lon=");
    strcat(url, lon);
    pagefile = fopen(pagefilename, "w");
    curly(url, pagefile);
}

void curlToFile (char *url, FILE *current_file)
{
    CURLcode res;
    
    curl = curl_easy_init();
    if(curl) {
        /* set URL to get here */
        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* Switch on full protocol/debug output while testing */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* disable progress meter, set to 0L to enable it */
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

        if (current_file) {

            /* write the page body to this file handle */
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, current_file);

            /* get it! */
            curl_easy_perform(curl);

            /* close the header file */
            fclose(current_file);
        }

        /* cleanup curl stuff */
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
}

void createTZLink (char *lat, char *lon)
{
    char linkEnd[10] = "?key=";
    // char startLink[50] = "https://dev.virtualearth.net/REST/v1/timezone/";
    char apikey[150] = "haxVStwCxUhIzGxjli0z~zsJv0puaIIPHTnzn6KG9tg~ApvTi-AZv9wuqhTJuuQr0y2aByjzFzOqreedOKadhVUyLn9DREaqOJsPZfFwhKst";

    // strcat(TZurl, startLink);
    strcat(TZurl, lat);
    strcat(TZurl, ",");
    strcat(TZurl, lon);
    strcat(TZurl, linkEnd);
    strcat(TZurl, apikey);

    FILE *tzFile = fopen("src/Time Zone Data.txt", "w");

    curlToFile(TZurl, tzFile);
}

// called when window is closed
G_MODULE_EXPORT void on_window_main_destroy(GtkWidget *w)
{
    gtk_main_quit();
    // g_free(bwidget);
    // g_free(pvwidget);
    g_free(window);
    g_free(window1);
    g_free(window3);
    g_free(window4);
}

// called when window is closed
G_MODULE_EXPORT void on_inputWin_destroy(GtkWidget *w)
{
    gtk_widget_show(window1);
    gtk_widget_hide(window);
}

// called when window is closed
G_MODULE_EXPORT void on_aboutWin_destroy(GtkWidget *w)
{
    gtk_widget_hide(window4);
}

// pv csv choice
G_MODULE_EXPORT void on_button1_clicked(GtkButton *b, gpointer *data) {
    int length = strlen(pv_selection_entry);
    int num;
    
    sprintf(pv_selection_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = atoi(pv_selection_entry);
    if (num < 0 || isalpha(pv_selection_entry[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry1), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue1), "NOT VALID");
        sprintf(pv_selection_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue1), "Value Stored");
        gtk_label_set_text(GTK_LABEL(printVal73), pv_selection_entry);
        //gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
        // gtk_notebook_next_page(note1);      // change page automatically
    }
}

// number of pvs
G_MODULE_EXPORT void on_button2_clicked(GtkButton *b, gpointer *data) {
    int length = strlen(pv_per_string_entry);
    int num;
    
    sprintf(pv_per_string_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = atoi(pv_per_string_entry);
    if (num < 0 || isalpha(pv_per_string_entry[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry2), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue2), "NOT VALID");
        sprintf(pv_per_string_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue2), "Value Stored");
        gtk_label_set_text(GTK_LABEL(printVal201), pv_per_string_entry);
        //gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
        // gtk_notebook_next_page(note1);      // change page automatically
    }
}

// number of strings in pv
G_MODULE_EXPORT void on_button3_clicked(GtkButton *b, gpointer *data) {
    int length = strlen(num_pv_strings_entry);
    int num;
    
    sprintf(num_pv_strings_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = atoi(num_pv_strings_entry);
    if (num < 0 || isalpha(num_pv_strings_entry[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry3), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue3), "NOT VALID");
        sprintf(num_pv_strings_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue3), "Value Stored");
        gtk_label_set_text(GTK_LABEL(printVal202), num_pv_strings_entry);
        //gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
        // gtk_notebook_next_page(note1);      // change page automatically
    }
}

// the tilt input
G_MODULE_EXPORT void on_button4_clicked(GtkButton *b, gpointer data) {
    int length = strlen(tilt_entry);
    float num;

    sprintf(tilt_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = strtof(tilt_entry, '\0');
    if (num < 0 || num > 90 || isalpha(tilt_entry[length]) != 0) {
        gtk_editable_delete_text(GTK_EDITABLE(entry4), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue4), "NOT VALID");
        sprintf(tilt_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));        
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue4), "Value Stored");
        gtk_label_set_text(GTK_LABEL(printVal76), tilt_entry);
        // gtk_notebook_next_page(note1);
    }
}

// azimuth angle
G_MODULE_EXPORT void on_button5_clicked(GtkButton *b, gpointer data) {
    int length = strlen(azimuth_angle_entry);
    float num;

    sprintf(azimuth_angle_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = strtof(azimuth_angle_entry, '\0');
    if (num < 0 || num > 360 || isalpha(azimuth_angle_entry[length]) != 0) {
        gtk_editable_delete_text(GTK_EDITABLE(entry5), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue5), "NOT VALID");
        sprintf(azimuth_angle_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));        
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue5), "Value Stored");
        gtk_label_set_text(GTK_LABEL(printVal77), azimuth_angle_entry);
        gtk_notebook_next_page(note1);
    }
}

// longitude input
G_MODULE_EXPORT void on_button6_clicked(GtkButton *b, gpointer data) {
    int length = strlen(lon);
    float num;
    int count;
    
    sprintf(lon, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = strtof(lon, '\0');
    if (num < -180 || num > 180 || isalpha(lon[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry6), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue6), "NOT VALID");
        sprintf(lon, "%s", gtk_entry_get_text(GTK_ENTRY(data)));

        /* Downloading */
        gtk_label_set_text(GTK_LABEL(g_lbl_download), "Input A Valid Number");  
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue6), "Value Stored");
        gtk_label_set_text(GTK_LABEL(printVal71), lon);
        //gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
        // gtk_notebook_next_page(note1);      // change page automatically

        /* Downloading */
        gtk_label_set_text(GTK_LABEL(g_lbl_download), "   ");  
    }
}

// latitude input
G_MODULE_EXPORT void on_button7_pressed(GtkButton *b, gpointer data) {
        /* Downloading */
        gtk_label_set_text(GTK_LABEL(printValue7), " ");
        gtk_label_set_text(GTK_LABEL(g_lbl_download), "Downloading .... ");
}

// latitude input
G_MODULE_EXPORT void on_button7_released(GtkButton *b, gpointer data) {
    int length = strlen(lat);
    float num;

    sprintf(lat, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = strtof(lat, '\0');    

    if (num < -90 || num > 90 || isalpha(lat[length]) != 0) {
        gtk_editable_delete_text(GTK_EDITABLE(entry7), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue7), "NOT VALID");
        sprintf(lat, "%s", gtk_entry_get_text(GTK_ENTRY(data)));    
        /* Input Again */
        gtk_label_set_text(GTK_LABEL(g_lbl_download), "Input A Valid Number");    
    }
    else {
        gtk_label_set_text(GTK_LABEL(printVal72), lat);
        //gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);

        gtk_label_set_text(GTK_LABEL(printValue7), "Value Stored");
        createTZLink(lat, lon);

        concat(lat, lon);
        // gtk_notebook_next_page(note1);
    }

}

// battery choice
G_MODULE_EXPORT void on_button8_clicked(GtkButton *b, gpointer data) {
    int length = strlen(battery_selection_entry);
    int num;
    
    sprintf(battery_selection_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = atoi(battery_selection_entry);
    if (num < 0 || isalpha(battery_selection_entry[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry8), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue8), "NOT VALID");
        sprintf(battery_selection_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue8), "Value Stored");
        // gtk_label_set_text(GTK_LABEL(printVal74), battery_selection_entry);
        //gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
        // gtk_notebook_next_page(note1);      // change page automatically
    }
}

// number of batteries using
G_MODULE_EXPORT void on_button9_clicked(GtkButton *b, gpointer data) {
    int length = strlen(bat_per_string_entry);
    int num;
    
    sprintf(bat_per_string_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = atoi(bat_per_string_entry);
    if (num < 0 || isalpha(bat_per_string_entry[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry9), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue9), "NOT VALID");
        sprintf(bat_per_string_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue9), "Value Stored");
        gtk_label_set_text(GTK_LABEL(printVal203), bat_per_string_entry);
        //gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
        // gtk_notebook_next_page(note1);      // change page automatically
    }
}

// number strings for batteries using
G_MODULE_EXPORT void on_button10_clicked(GtkButton *b, gpointer data) {
    int length = strlen(num_bat_strings_entry);
    int num;
    
    sprintf(num_bat_strings_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = atoi(num_bat_strings_entry);
    if (num < 0 || isalpha(num_bat_strings_entry[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry10), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue10), "NOT VALID");
        sprintf(num_bat_strings_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue10), "Value Stored");
        gtk_label_set_text(GTK_LABEL(printVal204), num_bat_strings_entry);
        //gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
        gtk_notebook_next_page(note1);      // change page automatically
    }
}

// inverter choice
G_MODULE_EXPORT void on_button11_clicked(GtkButton *b, gpointer data) {
    int length = strlen(inverter_entry);
    int num;
    
    sprintf(inverter_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = atoi(inverter_entry);
    if (num < 0 || isalpha(inverter_entry[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry11), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue11), "NOT VALID");
        sprintf(inverter_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue11), "Value Stored");
        // gtk_label_set_text(GTK_LABEL(printVal75), inverter_entry);
    }  
}

// number of inverter
G_MODULE_EXPORT void on_button12_clicked(GtkButton *b, gpointer data) {
    int length = strlen(num_inverters_entry);
    int num;
    
    sprintf(num_inverters_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = atoi(num_inverters_entry);
    if (num < 0 || isalpha(inverter_entry[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry12), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue12), "NOT VALID");
        sprintf(num_inverters_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue12), "");
        gtk_label_set_text(GTK_LABEL(printVal207), num_inverters_entry);
        //gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
        gtk_notebook_next_page(note1);      // change page automatically
    }  
}

// charge controller choice
G_MODULE_EXPORT void on_button13_clicked(GtkButton *b, gpointer data) {
    int length = strlen(chrg_controller_entry);
    int num;
    
    sprintf(chrg_controller_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = atoi(chrg_controller_entry);
    if (num < 0 || isalpha(chrg_controller_entry[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry13), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue13), "NOT VALID");
        sprintf(chrg_controller_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue13), "Value Stored");
        // gtk_label_set_text(GTK_LABEL(printVal76), chrg_controller_entry);
        //gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
    }
}

// number of charge controller
G_MODULE_EXPORT void on_button14_clicked(GtkButton *b, gpointer data) {
    int length = strlen(numCC);
    int num;
    
    sprintf(numCC, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = atoi(numCC);
    if (num < 0 || isalpha(numCC[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry14), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue14), "NOT VALID");
        sprintf(numCC, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue14), "Value Stored");
        gtk_label_set_text(GTK_LABEL(printVal208), numCC);
        //gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
        gtk_notebook_next_page(note1);      // change page automatically
    }
}

// reliability input
G_MODULE_EXPORT void on_button15_clicked(GtkButton *b, gpointer data) {
    int length = strlen(reliability_entry);
    char *t;
    int num;
    
    sprintf(reliability_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = atoi(reliability_entry)/100;
    if (num < 0 || num > 100 || isalpha(reliability_entry[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry15), 0, -1);
        gtk_label_set_text(GTK_LABEL(printValue15), "NOT VALID");
        sprintf(reliability_entry, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    }
    else {
        gtk_label_set_text(GTK_LABEL(printValue15), "Value Stored");
        gtk_label_set_text(GTK_LABEL(printVal78), reliability_entry);
        sun();       // BE SURE TO UNCOMMENT
        //gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
        gtk_widget_show(window1);
        gtk_widget_hide(window);
    }    
}

// tax input
G_MODULE_EXPORT void on_button18_clicked(GtkButton *b, gpointer data) {
    int length = strlen(taxPrice);
    char *t;
    int num;
    
    sprintf(taxPrice, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = atoi(taxPrice);
    if (num < 0 || isalpha(taxPrice[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry16), 0, -1);
        gtk_label_set_text(GTK_LABEL(printVal39), "NOT VALID");
        sprintf(taxPrice, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    }
    else {
        // gtk_editable_delete_text(GTK_EDITABLE(entry16), 0, -1);
        float taxDecimal = strtof(taxPrice, NULL);
        float total = totalCost * taxDecimal;
        float totalCostWithTax = totalCost + total;
        char completeCost[BUFFSIZ];
        char dollarSignedCostBeforeTax[BUFFSIZ];
        strcpy(dollarSignedCostBeforeTax, "$ ");
        ftoa(totalCostWithTax, completeCost, 2);
        strcat(dollarSignedCostBeforeTax, completeCost);
        gtk_label_set_text(GTK_LABEL(printVal39), dollarSignedCostBeforeTax);
    }
}

G_MODULE_EXPORT void on_button19_clicked(GtkButton *b) {
    pdat = &pd;
    S_init(pdat);
    FILE *weatherData;
    char FILE_NAME[100] = "src/Weather Data.txt";

    weatherData = fopen( FILE_NAME, "r");
    readAllData(pdat, weatherData, dailyKTData, avgTempData, maxTempData, minTempData);
    char testValue[50];

    // pdat->year = 1984;
    // pdat->daynum = 1;
    // pdat->hour = 1;

    float lon1;
    lon1 = strtof(lon, '\0');

    float lat1;
    lat1 = strtof(lat, '\0');

    pdat->longitude = lon1;  /* Note that latitude and longitude are  */
    pdat->latitude  =  lat1;  /*   in DECIMAL DEGREES, not Deg/Min/Sec */

    avgDailyInsolation = insolationQuickLook(pdat, dailyKTData);

    ftoa(avgDailyInsolation, testValue, 2);
    gtk_label_set_text(GTK_LABEL(printVal29), testValue);

    g_signal_connect (G_OBJECT (da),
        "draw",
        G_CALLBACK (on_draw),
        NULL);
}

G_MODULE_EXPORT void on_button20_clicked(GtkButton *b, gpointer data) {
    int length = strlen(BOScost);
    char *t;
    int num;
    
    sprintf(BOScost, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    num = atoi(BOScost);
    if (num < 0 || isalpha(BOScost[length]) != 0){       // verify if input is floating point
        gtk_editable_delete_text(GTK_EDITABLE(entry17), 0, -1);
        gtk_label_set_text(GTK_LABEL(printVal40), "NOT VALID");
        sprintf(BOScost, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    }
    else {
        // gtk_editable_delete_text(GTK_EDITABLE(entry16), 0, -1);
        float taxDecimal = strtof(taxPrice, NULL);
        float total = totalCost * taxDecimal;
        float totalCostWithTaxAndBOS = totalCost + total + strtof(BOScost, NULL);
        char completeCost[BUFFSIZ];
        char dollarSignedCost[BUFFSIZ];
        strcpy(dollarSignedCost, "$ ");
        ftoa(totalCostWithTaxAndBOS, completeCost, 2);
        strcat(dollarSignedCost, completeCost);
        gtk_label_set_text(GTK_LABEL(printVal40), dollarSignedCost);
    }
}

// append project to a file
G_MODULE_EXPORT void on_menuitm_save_activate(GtkMenuItem *menuitem) {
	gtk_widget_show(save_window);

    if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        appendingFile = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));
        strcpy(appendingLocation, appendingFile);
        // gtk_label_set_text(GTK_LABEL(test2), appendingLocation);

        FILE *appender = fopen(appendingLocation, "a+");

        if (!appender) {
        	// Error in file opening
        	gtk_label_set_text(GTK_LABEL(printVal93), "Can't open file");
        } 
        else {
            fprintf(appender, "\n");
            fprintf(appender, "\n");
            fprintf(appender, "Project Loaded");
            fprintf(appender,"Longitude, %s,\n", lon);
            fprintf(appender, "Latitude, %s,\n", lat);
            fprintf(appender, "\n");
            fprintf(appender, "PV Specification, %s,\n", panel->model);
            fprintf(appender, "Number of PV Modules, %s,\n", pv_per_string_entry);
            fprintf(appender, "Number of String for Modules, %s,\n", num_pv_strings_entry);
            fprintf(appender, "Tilt, %s,\n", tilt_entry);
            fprintf(appender, "Azimuth Angle, %s,\n", azimuth_angle_entry);
            fprintf(appender, "\n");
            fprintf(appender, "Battery Specification, %s,\n", battery->model);
            fprintf(appender, "Number of Batteries, %s,\n", bat_per_string_entry);
            fprintf(appender, "Number of Strings for Batteries, %s,\n", num_bat_strings_entry);
            fprintf(appender, "\n");
            fprintf(appender, "Charge Controller Specification, %s,\n", cController->model);
            fprintf(appender, "Number of Charge Controllers, %s,\n", numCC);
            fprintf(appender, "\n");
            fprintf(appender, "Inverter Specification, %s,\n", inverter->model);
            fprintf(appender, "Number of Inverters, %s,\n", num_inverters_entry);
            fprintf(appender, "\n");
            fprintf(appender, "Desired Reliability of System, %s,\n", reliability_entry);
            fprintf(appender, "Calculated Reliability of System, %s,\n", reliability);
            fprintf(appender, "\n");
            fprintf(appender, "Total Cost before Tax and BOS, %s,\n", dollarSignedCost);
            gtk_label_set_text(GTK_LABEL(printVal94), "file saved.. check location\n");
            fclose(appender);
        }
    }
}

// save project to new file
G_MODULE_EXPORT void on_menuitm_saveas_activate(GtkMenuItem *menuitem) {
	gtk_widget_show(save_window);

    if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        savedLocation = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(chooser));
        strcpy(locationFN, savedLocation);
        gtk_label_set_text(GTK_LABEL(printVal93), savedFN);
        strcat(locationFN, "\\");
        strcat(locationFN, savedFN);
        strcat(locationFN, ".csv");
        // gtk_label_set_text(GTK_LABEL(printVal93), locationFN);

        FILE *save = fopen(locationFN, "a+");

        if (!save) {
        	// Error in file opening
        	gtk_label_set_text(GTK_LABEL(printVal93), "Can't open file");
        } 
        else {
            fprintf(save, "Project Loaded");
            fprintf(save,"Longitude, %s,\n", lon);
            fprintf(save, "Latitude, %s,\n", lat);
            fprintf(save, "\n");
            fprintf(save, "PV Specification, %s,\n", panel->model);
            fprintf(save, "Number of PV Modules, %s,\n", pv_per_string_entry);
            fprintf(save, "Number of String for Modules, %s,\n", num_pv_strings_entry);
            fprintf(save, "Tilt, %s,\n", tilt_entry);
            fprintf(save, "Azimuth Angle, %s,\n", azimuth_angle_entry);
            fprintf(save, "\n");
            fprintf(save, "Battery Specification, %s,\n", battery->model);
            fprintf(save, "Number of Batteries, %s,\n", bat_per_string_entry);
            fprintf(save, "Number of Strings for Batteries, %s,\n", num_bat_strings_entry);
            fprintf(save, "\n");
            fprintf(save, "Charge Controller Specification, %s,\n", cController->model);
            fprintf(save, "Number of Charge Controllers, %s,\n", numCC);
            fprintf(save, "\n");
            fprintf(save, "Inverter Specification, %s,\n", inverter->model);
            fprintf(save, "Number of Inverters, %s,\n", num_inverters_entry);
            fprintf(save, "\n");
            fprintf(save, "Reliability of System, %s,\n", reliability_entry);
            fprintf(save, "Calculated Reliability of System, %s,\n", reliability);
            fprintf(save, "\n");
            fprintf(save, "Total Cost before Tax and BOS, %s,\n", dollarSignedCost);
            gtk_label_set_text(GTK_LABEL(printVal94), "file saved.. check location\n");
            fclose(save);
        }
    }
}

G_MODULE_EXPORT void on_button16_clicked(GtkButton *b, gpointer *data) {
    sprintf(savedFN, "%s", gtk_entry_get_text(GTK_ENTRY(data)));
    // strcat(locationFN, savedFN);
    // printf("%s", locationFN);

    // gtk_label_set_text(GTK_LABEL(test2), savedFN);
    gtk_widget_hide(save_window);
}

// extra about window for when the one that is open is shut
G_MODULE_EXPORT void on_menuitm_about_activate(GtkMenuItem *menuitem) {
    gtk_widget_show(window4);
}

// drop down file menu quit ( exiting the entire application)
G_MODULE_EXPORT void on_menuitm_quit_activate(GtkMenuItem *menuitem)
{
    gtk_main_quit();
    g_free(window);
    g_free(window1);
    g_free(window3);
}
