#ifndef DATA_H
#define DATA_H

class Data
{
public:
    static int getDay();
    static int getHour();
    static int getMinutes();
    static float getTemp();
    static String getDate();
    static void currTemp();
    static void initSensors();
};

#endif // DATA_H
