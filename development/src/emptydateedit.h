/* header defining the interface of the source. */
#ifndef EMPTYDATEEDIT_H
#define EMPTYDATEEDIT_H

/* include some QT libraries. */
#include <QDateEdit>

/* GUI string messages. */
static const QString emptyDateText = "None";

/* class which implements a date edit that can have empty value. */
class EmptyDateEdit : public QDateEdit
{
    Q_OBJECT

    Q_PROPERTY(QDate emptyDate READ emptyDate WRITE setDate USER true)

    public:
        EmptyDateEdit(const QDate &date, QWidget *parent = 0);
        EmptyDateEdit(QWidget *parent = 0);
        ~EmptyDateEdit();
        QDate emptyDate() const;

    public slots:
        void clear();
        void setDate(const QDate &date);
};

#endif // EMPTYDATEEDIT_H
