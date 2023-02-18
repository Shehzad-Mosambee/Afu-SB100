#ifndef __MERCURY_GNSS_h__
#define __MERCURY_GNSS_h__

#define GPS_ERR_SUCCESS             0
#define GPS_ERR_INVALID_PARAMS      501
#define GPS_ERR_NO_SUPPORT          502
#define GPS_ERR_SUBSYSTEM_BUSY      503
#define GPS_ERR_SESSION_ONGOING     504
#define GPS_ERR_SESSION_NO_ACTIVE   505
#define GPS_ERR_TIMEOUT             506
#define GPS_ERR_NO_ENABLDE          507
#define GPS_ERR_INFORMATION_ERR     508
#define GPS_ERR_VALIDITY_OUT_OF_RANGE     512
#define GPS_ERR_RESOURCE_ERR        513
#define GPS_ERR_NO_FIXED            516
#define GPS_ERR_UNKONW_ERR          549

typedef int(*gnss_nmea_cb)(char *p_data, int len);

typedef struct
{
    char utctime[12+1];     /**< UTC of position (just time) */
    char date[6+1];         /**< ddmmyy */
    char status[1+1];       /**< Status (A = active or V = void) */
	char lat[16+1];         /**< Latitude in NDEG - [degree][min].[sec/60] */
    char ns[1+1];           /**< [N]orth or [S]outh */
	char lon[16+1];         /**< Longitude in NDEG - [degree][min].[sec/60] */
    char ew[1+1];           /**< [E]ast or [W]est */
    char mode[1+1];         /**< Mode indicator of fix type (A = autonomous, D = differential, E = estimated, N = not valid, S = simulator) */
    double speed;           /**< Speed over the ground in knots */
    double hdop;           /**< Horizontal dilution of precision */
    double sog;             /**< Ground speed, knots */
	double kph;             /**< Ground speed, kilometers per hour */
} Gnss_CurrentInfo;

typedef struct
{
    uint16 NoSv;          /**< Number of satellites in view) */
	uint16  satinuse;     /**< Number of satellites in use (not those in view) */
    uint16 cn[4];         //保存最大的4个SNR值
} Gnss_CurParamInfo;


/****************************************
GNSS START
param[in] cfun : nmea data callback, if NULL, get info/nmeadata by Gnss_GetCurInfo/Gnss_GetCurNmea &&
param[in] freq, nmeadata callback freq;
          if cfun is not NULL, freq min value is 1
          if cfun is NULL , freq no care     
*****************************************/
int Gnss_Start(gnss_nmea_cb cfun, uint8_t freq);

/****************************************
GNSS STOP
*****************************************/
int Gnss_Stop(void);

/****************************************
GNSS RESET
param[in] mode: 0: hot start, 1,warm start, 2 clod start
*****************************************/
int Gnss_Reset(uint8_t mode);

/****************************************
Get currten info
param[out] curinfo: current info 
*****************************************/
int Gnss_GetCurInfo(Gnss_CurrentInfo *curinfo);

/****************************************
Get nmea data
param[in] nmeatype: "RMC", "GGA", "GSV","GSA", "VTG" ,"ALL"
param[out] data: nmea data buffer
*****************************************/
int Gnss_GetCurNmea(char *nmeatype, char* data);

/****************************************
gnss config
param[in] item: "gpsnmeatye" 31 ALL, 1 GGA, 2 RMC, 4 GSV, 8 GSA 16 VTG
                "baud"  9600(default)/115200/230400
param[in] value
*****************************************/
int Gnss_Config(char *item, uint32_t value);

/****************************************
Get param info
param[out] curinfo: 
*****************************************/
int Gnss_GetParamCurInfo(Gnss_CurParamInfo *curinfo);

#endif

