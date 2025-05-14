#ifndef DATA_H
#define DATA_H

class Data
{
public:
	int getDay();
	int getHour();
	int getMinutes();
	float getIntTemp();
	String getExterTemp();
	String getExterHum();
	String getDate();
	void currState();
	String createCurrJson();
	int getLight();
	int getWater();
	void initSensors(int tempPin, int DOUT_SCALE, int CLK_SCALE);
	void weightTare();
	void weightRead();
	int getWeight();
	void set_oldDay(int day);
	void set_oldHour(int hour);
	void set_oldMinutes(int minutes);
	void setAvg(int avg);
	float getAvg();
	int get_oldDay();
	int get_oldHour();
	int get_oldMinutes();
	void update_oldDay();
	void update_oldHour();
	void update_oldMinutes();
	void updateTime();
	void initTime();
};

#endif // DATA_H
