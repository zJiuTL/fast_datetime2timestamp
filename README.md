# fast_datetime2timestamp
Faster datetime timestamps. It takes up less memory than C library functions and is faster.


Example of calling a functio:


uint32_t timestamp1 = zdatetime2timestamp(&datetime);

datetime1 = ztimestamp2datetime(settimestamp);

We tested on STM32F103, the results show that it is 2253 bytes smaller than C library TIME function, enjoy it！
