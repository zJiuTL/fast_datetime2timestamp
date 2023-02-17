#ifndef __TIME_STAMP__H
#define __TIME_STAMP__H

#include <stdint.h>


#define BCD2HEX(value)    ((((value & 0xF0) >> 4) * 10) + (value & 0x0F))
#define HEX2BCD(value)    ((((value & 0xFF) / 10) << 4) | ((value & 0xFF) % 10))


typedef struct{
    uint8_t sec ,min ,hour ,week ,day ,mon ,year;
}datetime_t;

/*---------------------------------------------------------------------------------*/
uint32_t datetime2timestamp(const datetime_t *datetime);
datetime_t timestamp2datetime(uint32_t timestamp);

uint32_t zdatetime2timestamp(const datetime_t *datetime);
datetime_t ztimestamp2datetime(uint32_t timestamp);


#endif //end of __TIME_STAMP__H
