#include "timestamp.h"
#include <time.h>

#define DATETIME_OFFSET      2000

/*
UTC: 1970-01-1 00:0:0
*/

/*--------------------------------------------------------------------------
* @brief      datetime to timestamp
* @param   datetime
* @return   utc timestamp
*/
uint32_t datetime2timestamp(const datetime_t *datetime)
{
    /* get current timeinfo and modify it to the user's choice */
    time_t rawtime = 0;
    struct tm * tim;
    tim = localtime ( &rawtime );
    tim->tm_year/*years since 1900*/ = datetime->year + DATETIME_OFFSET - 1900;
    tim->tm_mon/*[0, 11]*/ = datetime->mon - 1;
    tim->tm_mday/*[1, 31]*/ = datetime->day;
    tim->tm_hour/*[0, 23]*/ = datetime->hour;
    tim->tm_min/*[0, 59]*/ = datetime->min;
    tim->tm_sec/*[0, 61]*/ = datetime->sec;
    /* call mktime: timeinfo->tm_wday will be set */
    rawtime = mktime(tim);
    return (rawtime);
}


/*--------------------------------------------------------------------------
* @brief      timestamp to datetime
* @param   timestamp : utc timestamp
* @return   datetime
*/
datetime_t timestamp2datetime(uint32_t timestamp)
{
    datetime_t datetime;
    time_t rawtime = timestamp;
    
    struct tm *tim = NULL;
    tim = gmtime (&rawtime);
    datetime.year = tim->tm_year + 1900 - DATETIME_OFFSET; /*pTim->tm_year + 1970 - 2000*/
    datetime.mon = tim->tm_mon + 1;
    datetime.day = tim->tm_mday;
    datetime.hour = tim->tm_hour;
    datetime.min = tim->tm_min;
    datetime.week = tim->tm_wday;
    datetime.sec = tim->tm_sec;
    
    return datetime;
}

/*--------------------------------------------------------------------------
* @brief      timestamp to datetime
* @param   timestamp : utc timestamp
* @return   datetime
*/
datetime_t ztimestamp2datetime(uint32_t timestamp)
{
    datetime_t datetime;
    int C ,E;
    uint32_t jd;
    
    /*1. calculate hour minute second */
    jd = timestamp % 3600;
    datetime.min = jd / 60;
    datetime.sec = jd % 60;
    jd = timestamp / 3600;
    datetime.hour = jd % 24;
    /*2.calculatie year month day */
    jd = (timestamp / 3600 / 24 * 10) + 24405875/*2440587.5f*/;
    
    /* get julian day */
    datetime.week = ((jd + 15) / 10) % 7; 
    
    jd = (jd + 5) / 10;
    
    if (jd >= 2299161){
        C = ((jd * 100 - 186721625) / 3652425);
        jd = (jd + 1 + C - (C / 4));
    }
    
    jd += 1524;
    C = ((jd*100 - 12210) / 36525);
    E = ((jd - (36525 * C / 100)) * 10000 / 306001);
    
    /* get the day */
    datetime.day = jd - (36525 * C / 100) - (306001 * E / 10000);
    
    /* get the month */
    if (E < 14){
        datetime.mon = E - 1;
    }else{
        datetime.mon = E - 13;
    }
    
    /* get the year */
    if (datetime.mon > 2){
        datetime.year = C - 4716 - DATETIME_OFFSET;
    }else{
        datetime.year = C - 4715 - DATETIME_OFFSET;
    }
    
    return datetime;
}

/*--------------------------------------------------------------------------
* @brief      datetime to timestamp
* @param   datetime
* @return   utc timestamp
*/
uint32_t zdatetime2timestamp(const datetime_t *datetime)
{
    uint32_t a = 0 ,b = 0 ,temp = 0;
    int nYear = datetime->year + DATETIME_OFFSET;
    int nMonth = datetime->mon;
    int nDay = datetime->day;
    
    /*Cal JD*/
    if(nMonth <= 2){
        nMonth += 12;
        nYear -= 1;
    }
    
    a = nYear / 100;
    b = 2 - a + (a / 4);
    
    temp = ((36525*(nYear + 4716))/100) + (306001*(nMonth + 1)/10000) + nDay + b;
    temp = ((temp * 100)-152450);
    
    temp = temp - 244058750; /*from UTC: 1970-01-1 00:0:0*/
    temp = (temp /100) *24*3600 + datetime->hour * 3600 + datetime->min * 60 + datetime->sec;
    
    return temp;
}
