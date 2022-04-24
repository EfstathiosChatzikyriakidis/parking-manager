/*
 *  This file implements a new time edit that can have empty value.
 *
 *  Copyright (C) 2010  Efstathios Chatzikyriakidis (stathis.chatzikyriakidis@gmail.com)
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
#include "emptytimeedit.h"

/* create a time edit gui object with special value text. */
EmptyTimeEdit::EmptyTimeEdit(const QTime &time, QWidget *parent) : QTimeEdit(time, parent) {
    this->setSpecialValueText(emptyTimeText);
}

/* create a time edit gui object with special value text. */
EmptyTimeEdit::EmptyTimeEdit(QWidget *parent) : QTimeEdit(parent) {
    this->setSpecialValueText(emptyTimeText);
}

/* deconstructor of empty time edit gui object. */
EmptyTimeEdit::~EmptyTimeEdit() {}

/* redefine the clear method to set the minimum time. */
void
EmptyTimeEdit::clear() {
    this->setTime(this->minimumTime());
}

/* check the time with the minimum time. */
QTime
EmptyTimeEdit::emptyTime() const {
    if (time() == this->minimumTime())
        return QTime();

    return time();
}

/* redefine the set method, and if the
   time is null then set it to minimum. */
void
EmptyTimeEdit::setTime(const QTime &time) {
    if (time.isNull())
        QTimeEdit::setTime(this->minimumTime());

    QTimeEdit::setTime(time);
}
