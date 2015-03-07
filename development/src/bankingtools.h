/* header defining the interface of the source. */
#ifndef BANKINGTOOLS_H
#define BANKINGTOOLS_H

/* include some QT libraries. */
#include <QString>

/* credit card types enumeration data type. */
typedef enum creditCardType {
    creditCardError = -1, /* exists for error checking. */
    creditCardVisa,
    creditCardMasterCard
} creditCardType;

/* get the type of the credit card. */
creditCardType getCreditCardType(const QString cardNumber);

/* try to charge the credit card. */
bool chargeCreditCard(const QString cardNumber, const double charge);

#endif // BANKINGTOOLS_H
