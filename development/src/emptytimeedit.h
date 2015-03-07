/* header defining the interface of the source. */
#ifndef EMPTYTIMEEDIT_H
#define EMPTYTIMEEDIT_H

/* include some QT libraries. */
#include <QTimeEdit>

/* GUI string messages. */
static const QString emptyTimeText = "None";

/* class which implements a time edit that can have empty value. */
class EmptyTimeEdit : public QTimeEdit
{
    Q_OBJECT

    Q_PROPERTY(QTime emptyTime READ emptyTime WRITE setTime USER true)

    public:
        EmptyTimeEdit(const QTime &time, QWidget *parent = 0);
        EmptyTimeEdit(QWidget *parent = 0);
        ~EmptyTimeEdit();
        QTime emptyTime() const;

    public slots:
        void clear();
        void setTime(const QTime &time);
};

#endif // EMPTYTIMEEDIT_H
