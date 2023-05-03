#ifndef INC__BASE__H
#define INC__BASE__H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

typedef enum _print_level_e {PL_NONE, PL_LOW, PL_MEDIUM, PL_HIGH, PL_DEBUG, PL_WARNING, PL_ERROR, PL_FATAL} print_level_e;
typedef enum __print_mode_e {PM_NEWLINE, PM_CONTINUE} print_mode_e;

static print_level_e RUN_PRINT_LEVEL = PL_LOW;

inline void GetTime(double *t, char *t_str){
    timeval tv;
    gettimeofday(&tv, NULL);
    *t = tv.tv_sec + tv.tv_usec * 1e-6;
    struct tm *t_info;
    t_info = gmtime((time_t *)&tv.tv_sec);
    strftime(t_str, 100, "%Y-%m-%d %T", t_info);
}

inline void HwPrint(print_level_e pl, char *dbg_info, char *msg) {
    const int MAX_INFO_LEN = 1000;
    char info[MAX_INFO_LEN] = {'\0'};
    char time_str[100];
    double time_val;
    GetTime(&time_val, time_str);
    if (pl == PL_FATAL) {
      snprintf(info, MAX_INFO_LEN, "MSG-F @ %s %s:", time_str, dbg_info);
    } else if (pl == PL_ERROR) {
      snprintf(info, MAX_INFO_LEN, "MSG-E @ %s %s:", time_str, dbg_info);
    } else if (pl == PL_WARNING) {
      snprintf(info, MAX_INFO_LEN, "MSG-W @ %s %s:", time_str, dbg_info);
    } else if (pl <= RUN_PRINT_LEVEL) {
      snprintf(info, MAX_INFO_LEN, "MSG-I @ %s %s:", time_str, dbg_info);
    }
    if (info[0]) {
        printf("%s %s", info, msg);
    }
    if (strstr(info, "MSG-F @ ")) { 
        exit(1);
    }
}

#define USE_HWPRINT(pl, pm, ...) \
  { \
    char dbg_info[1000] = {'\0'}, info[1000]; \
    snprintf(info, 1000, __VA_ARGS__); \
    if (pm == PM_NEWLINE) { \
        snprintf(dbg_info, 1000, "%s(Line %0d)[%s]", __FILE__, __LINE__, __FUNCTION__); \
        if (info[strlen(info)-1] != '\n') { \
            strncat(&info[strlen(info)], "\n", 2); \
        } \
    } \
    HwPrint(pl, dbg_info, info); \
  }

#define HWPRINT(pl, ...) \
    USE_HWPRINT(pl, PM_NEWLINE, __VA_ARGS__);
#define HWPRINT_CONTINUE(pl, ...) \
    USE_HWPRINT(pl, PM_CONTINUE, __VA_ARGS__);

#endif /*INC__BASE__H*/