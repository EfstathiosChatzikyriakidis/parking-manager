/*
 *  This file implements banking tools (modules).
 *
 *  Copyright (C) 2010  Efstathios Chatzikyriakidis (contact@efxa.org)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* include header defining the interface of the source. */
#include "bankingtools.h"

/* get the type of the credit card. */
creditCardType
getCreditCardType(const QString cardNumber) {
    /* here goes code for card type recognition (visa, mastercard). */

    /* assume that card type was Visa. */
    return creditCardVisa;
}

/* we need here an OO factory pattern. */

/* try to charge the credit card. */
bool
chargeCreditCard(const QString cardNumber, const double charge) {
    const creditCardType cardtype = getCreditCardType(cardNumber);

    /* if the credit card type is Visa. */
    if(cardtype == creditCardVisa) {
        /* here goes code for charging Visa credit card. */

        /* assume that credit card was charged. */
        return true;
    }

    /* in any other case the card was not charged. */
    return false;
}
