#ifndef WCONFIG_H
#define WCONFIG_H

// Version defines
#define WT_SERIES 3
#define WT_MAJOR  3
#define WT_MINOR  4

/*! \brief A constant that encodes the library version of %Wt
 *
 * You may use this constant to check for the version of %Wt at build-time.
 */
#define WT_VERSION (((WT_SERIES & 0xff) << 24) | ((WT_MAJOR & 0xff) << 16) | ((WT_MINOR & 0xff) << 8))
#define WT_VERSION_STR "3.3.4"
#define WT_CLASS       "Wt3_3_4"
#define WT_INCLUDED_VERSION Wt_3_3_4

#define RUNDIR "/var/run/wt"
#define WT_CONFIG_XML "/home/irina/projects/lughos/externalLibraries/etc/wt_config.xml"
#define WTHTTP_CONFIGURATION "/home/irina/projects/lughos/externalLibraries/etc/wthttpd"


/* #undef WT_STATIC */
/* #undef WTDBO_STATIC */
/* #undef WTDBOPOSTGRES_STATIC */
/* #undef WTDBOSQLITE3_STATIC */
/* #undef WTDBOFIREBIRD_STATIC */
/* #undef WTDBOMYSQL_STATIC */
/* #undef WTHTTP_STATIC */
/* #undef WT_EXT_STATIC */
/* #undef WT_EXT_STATIC */

/* #undef WT_HAS_WRASTERIMAGE */
/* #undef WT_HAS_WPDFIMAGE */
#define WT_WITH_SSL

/* #undef WT_NO_BOOST_INTRUSIVE */
/* #undef WT_NO_BOOST_RANDOM */
/* #undef WT_NO_STD_LOCALE */
/* #undef WT_NO_STD_WSTRING */
/* #undef WT_USE_OPENGL */
/* #undef WT_DEBUG_ENABLED */

/* #undef WT_USE_BOOST_SIGNALS */
#define WT_USE_BOOST_SIGNALS2

// our win32: WIN32 (gcc) or _WIN32 (MSC)
#if defined(WIN32) || defined(_WIN32)
#define WT_WIN32 1
#endif

#endif
