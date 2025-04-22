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
    static void set_oldDay(int day);
    static void set_oldHour(int hour);
    static void set_oldMinutes(int minutes);
    static int get_oldDay();
    static int get_oldHour();
    static int get_oldMinutes();
    static void update_oldDay();
    static void update_oldHour();
    static void update_oldMinutes();
    static void updateTime();
    static void initTime();
};

#endif // DATA_H
