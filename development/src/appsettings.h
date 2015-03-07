/* header defining application's settings related data. */
#ifndef APPSETTINGS_H
#define APPSETTINGS_H

/* include some QT libraries. */
#include <QString>

/* application's settings structure data type. */
typedef struct appSettings {
    int parkingCapacity;
    int timeslice;
    double chargePerTimeslice;
    int chargePrecision;
} appSettings;

/* the default, maximum, minimum parking capacity. */
static const int DEF_PARKING_CAPACITY = 100;
static const int MAX_PARKING_CAPACITY = 5000;
static const int MIN_PARKING_CAPACITY = 1;

/* the default, maximum, minimum timeslice (in secs) for the charge of the transaction. */
static const int DEF_TIMESLICE = 3600;   /* 1 hour. */
static const int MAX_TIMESLICE = 172800; /* 2 days. */
static const int MIN_TIMESLICE = 1;      /* 1 second. */

/* the default, maximum, minimum charge per timeslice for the charge of the transaction. */
static const double DEF_CHARGE_PER_TIMESLICE = 2;
static const double MAX_CHARGE_PER_TIMESLICE = 5000;
static const double MIN_CHARGE_PER_TIMESLICE = 0.1;

/* the default, maximum, minimum precision in digits after the decimal point for charge. */
static const int DEF_CHARGE_PRECISION = 3;
static const int MAX_CHARGE_PRECISION = 10;
static const int MIN_CHARGE_PRECISION = 1;

/* the default increment/decrement step for charge spinbox. */
static const double DEF_CHARGE_SPINBOX_STEP = 0.1;

/* setting's organization and application name. */
static const QString setsAppOrg  = "FreeSoftwareStudios";
static const QString setsAppName = "ParkingManager";

#endif // APPSETTINGS_H
