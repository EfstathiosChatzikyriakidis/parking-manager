/* header defining some global declarations. */
#ifndef GLOBALDECLARATIONS_H
#define GLOBALDECLARATIONS_H

/* include some QT libraries. */
#include <QtGui>

/* card type indexes enumeration data type. */
typedef enum cardMemberType {
    ErrorCardType = -1, /* exists for error checking. */
    NoCardType,
    SimpleCardType,
    MonthCardType,
    YearCardType,
    CreditCardType
} cardMemberType;

/* month/year member card fee. */
static const double MONTH_CARD_FEE = 100;
static const double YEAR_CARD_FEE = 1000;

/* dangerous characters for sql injection. */
static const QString sqlInjectionRegExpStr = "[" + QRegExp::escape("\'\"$*+?[]^{|};\\") + "]+";

/* regular expression for numbers (integer/real). */
static const QString numberRegExpStr = "^[0-9]+(,[0-9]+){0,1}$";

/* mask for a credit card number. */
static const QString creditCardNumberMaskStr = "0000-0000-0000-0000;_";

/* format of a datetime value. */
static const QString dateTimeFormatStr = "dd-MM-yyyy, hh:mm:ss";

/* GUI string messages. */
static const QString infoMsgTitleStr   = QObject::tr("Application's Message");

static const QString nameLabelStr      = QObject::tr("&Name :");
static const QString customerLabelStr  = QObject::tr("&Customer :");
static const QString vehicleLabelStr   = QObject::tr("&Vehicle :");

static const QString customerStr       = QObject::tr("Customer");
static const QString vehicleStr        = QObject::tr("Vehicle");

static const QString firstButtonStr    = QObject::tr("<< &First");
static const QString previousButtonStr = QObject::tr("< &Previous");
static const QString nextButtonStr     = QObject::tr("&Next >");
static const QString lastButtonStr     = QObject::tr("&Last >>");

static const QString addButtonStr      = QObject::tr("&Add");
static const QString deleteButtonStr   = QObject::tr("&Delete");
static const QString closeButtonStr    = QObject::tr("&Close");

static const QString printButtonStr    = QObject::tr("&Print");

static const QString cantWriteSetsStr  = QObject::tr("Application Cannot Use Settings!");
static const QString sqlInjectCustStr  = QObject::tr("Please remove any character symbols from the customer name.");
static const QString sqlInjectVehiStr  = QObject::tr("Please remove any character symbols from the vehicle name.");

static const QString appCopyright = QObject::tr("Copyright (C) 2010  Efstathios Chatzikyriakidis (contact@efxa.org)");
static const QString appName      = QObject::tr("Parking Manager");
static const QString appVersion   = QObject::tr("1.0");

static const QString appLicense   = QObject::tr("This program is free software: you can redistribute it and/or modify "
                                                "it under the terms of the GNU General Public License as published by the "
                                                "Free Software Foundation, either version 3 of the License, or (at your "
                                                "option) any later version.");

#endif // GLOBALDECLARATIONS_H
