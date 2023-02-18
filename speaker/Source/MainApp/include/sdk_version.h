/*****
 * 2020.12.30, wangjianyu@qiyinyi.com
 * 	Init;
 */
#ifndef _QYY_SDK_VERSION_H_
#define _QYY_SDK_VERSION_H_


/* VERSION */
#if 1				/* OLD VERSION */
/****
 * Update PAC only;
 */
#define MERCURY_VERSION_4G		"MercuryPlat_V2.0.4"
#define QYY_SDK_VERSION_IN		"QYY_SDK_4G_v2.2.5"

#endif/*_QYY_SDK_VERSION_H_*/


/*****
 * AFFACT
 */
#ifdef MERCURY_DEBUG_VARIANT
#define QYY_SDK_VERSION                 QYY_SDK_VERSION_IN"(debug)""("__DATE__" "__TIME__")"
#else
#define QYY_SDK_VERSION                 QYY_SDK_VERSION_IN"(release)""("__DATE__" "__TIME__")"
#endif


#endif
