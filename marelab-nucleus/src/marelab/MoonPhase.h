/*
 * MoonPhase.h
 *
 *  Created on: 17.03.2012
 *      Author: ths
 */

#ifndef MOONPHASE_H_
#define MOONPHASE_H_

typedef struct {
    int year,month,day;
    double hour;
} TimePlace;

#define		PI	3.1415926535897932384626433832795
#define		RAD	(PI/180.0)
#define     SMALL_FLOAT	(1e-12)

class MoonPhase {
public:
	MoonPhase();
	virtual ~MoonPhase();
	void JulianToDate(TimePlace* now, double jd);
	double Julian(int year,int month,double day);
	double sun_position(double j);
	double moon_position(double j, double ls);
	double moon_phase(int year,int month,int day, double hour, int* ip);
	void nextDay(int* y, int* m, int* d, double dd);
	//double MoonPhase::SN(double x);
	//double MoonPhase::CS(double x);
	//double MoonPhase::Lightning(double JDE);
	//double MoonPhase::BeleuchtungZeile(int zeit);
	//long MoonPhase::Java_JD(long time_in_ms);
};

#endif /* MOONPHASE_H_ */
