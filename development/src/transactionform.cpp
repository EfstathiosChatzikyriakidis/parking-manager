/*
 *  This file implements the transaction gui form and data model.
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

/* include some QT libraries. */
#include <QtGui>
#include <QtSql>

/* include headers defining the interface of the sources. */
#include "transactionform.h"
#include "paywizard.h"
#include "globaldeclarations.h"
#include "appsettings.h"
#include "arithmetictools.h"

/* creates the application's transactions gui form and data model. */
TransactionForm::TransactionForm(const appSettings sets, QWidget *parent) : QDialog(parent) {
    /* store the application's settings. */
    this->sets = sets;

    /* create line edits and buddies objects for transaction fields. */
    vehicleEdit = new QLineEdit;
    vehicleLabel = new QLabel(vehicleLabelStr);
    vehicleLabel->setBuddy(vehicleEdit);

    customerEdit = new QLineEdit;
    customerLabel = new QLabel(customerLabelStr);
    customerLabel->setBuddy(customerEdit);

    startDateEdit = new EmptyDateEdit(QDate()); /* Null Date. */
    startDateLabel = new QLabel(startDateLabelStr);
    startDateLabel->setBuddy(startDateEdit);

    startTimeEdit = new EmptyTimeEdit(QTime()); /* Null Time. */
    startTimeLabel = new QLabel(startTimeLabelStr);
    startTimeLabel->setBuddy(startTimeEdit);

    /* create the navigation buttons. */
    firstButton = new QPushButton(firstButtonStr);
    previousButton = new QPushButton(previousButtonStr);
    nextButton = new QPushButton(nextButtonStr);
    lastButton = new QPushButton(lastButtonStr);

    /* create the management buttons. */
    completeButton = new QPushButton(completeButtonStr);
    deleteButton = new QPushButton(deleteButtonStr);
    closeButton = new QPushButton(closeButtonStr);

    /* add the buttons in a button box dialog. */
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(completeButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::AcceptRole);

    /* create the relational table model for the transactions. */
    tableModel = new QSqlRelationalTableModel(this);

    /* set the table to select. */
    tableModel->setTable("transacts");

    /* set a relation for the customer of the transaction. */
    tableModel->setRelation(Transaction_CustomerId, QSqlRelation("customer", "id", "name"));

    /* set a relation for the vehicle of the transaction. */
    tableModel->setRelation(Transaction_VehicleId, QSqlRelation("vehicle", "id", "reg_num"));

    /* select the transactions model in order to show the data. */
    tableModel->select();

    /* create a data widget mapper for the transactions fields. */
    mapper = new QDataWidgetMapper(this);

    /* set some operative options in the mapper. */
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));

    /* add to the map the transaction GUI objects which manage the fields. */
    mapper->addMapping(vehicleEdit, Transaction_VehicleId);
    mapper->addMapping(customerEdit, Transaction_CustomerId);
    mapper->addMapping(startDateEdit, Transaction_StartDate);
    mapper->addMapping(startTimeEdit, Transaction_StartTime);

    /* set the signals/slots for the buttons' events. */
    connect(firstButton, SIGNAL(clicked()), mapper, SLOT(toFirst()));
    connect(previousButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    connect(lastButton, SIGNAL(clicked()), mapper, SLOT(toLast()));
    connect(completeButton, SIGNAL(clicked()), this, SLOT(completeTransaction()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteTransaction()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));

    /* lock all gui objects (readonly, disabled). */
    lockGUI();

    /* if there are no transactions. */
    if (!tableModel->rowCount()) {
        /* focus the close button. */
        closeButton->setFocus();
    }
    else {
        /* select the first transaction in the model. */
        mapper->toFirst();

        /* focus the next button. */
        nextButton->setFocus();
    }

    /* create a horizontal layout for the navigation buttons. */
    QHBoxLayout *topButtonLayout = new QHBoxLayout;

    /* set some geometry for the layout. */
    topButtonLayout->setContentsMargins(20, 0, 20, 5);

    /* add the buttons to the layout. */
    topButtonLayout->addWidget(firstButton);
    topButtonLayout->addWidget(previousButton);
    topButtonLayout->addWidget(nextButton);
    topButtonLayout->addWidget(lastButton);

    /* create a table grid. */
    QGridLayout *mainLayout = new QGridLayout;

    /* add the following objects in the appropriate position in the grid. */
    mainLayout->addLayout(topButtonLayout, 0, 0, 1, 4);
    mainLayout->addWidget(vehicleLabel, 1, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(vehicleEdit, 1, 1, 1, 3);
    mainLayout->addWidget(customerLabel, 2, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(customerEdit, 2, 1, 1, 3);
    mainLayout->addWidget(startDateLabel, 3, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(startDateEdit, 3, 1);
    mainLayout->addWidget(startTimeLabel, 3, 2, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(startTimeEdit, 3, 3);
    mainLayout->addWidget(buttonBox, 4, 0, 1, 4);

    /* set some GUI options for the table grid. */
    mainLayout->setRowMinimumHeight(4, 35);
    mainLayout->setRowStretch(4, 1);
    mainLayout->setColumnStretch(2, 1);

    /* do not allow to resize the form. */
    mainLayout->setSizeConstraint (QLayout::SetFixedSize);

    /* set the layout for the transactions form. */
    setLayout(mainLayout);

    /* set the text of the form's window. */
    setWindowTitle(tranWinTitleStr);
}

/* update any changes and close the form. */
void
TransactionForm::done(const int result) {
    /* update any changes. */
    mapper->submit();

    /* return from the form. */
    QDialog::done(result);
}

/* deletes the current selected transaction from the database. */
void
TransactionForm::deleteTransaction() {
    /* check if the are any rows in the model. */
    if (!tableModel->rowCount()) return;

    /* ask him/her if he/she wants to delete the transaction. */
    int r = QMessageBox::question(this, infoMsgTitleStr, deleteTransactStr, QMessageBox::Yes | QMessageBox::No);

    /* if he/she don't want it just return and do nothing. */
    if (r == QMessageBox::No) return;

    /* get the index of the current transaction selected. */
    const int row = mapper->currentIndex();

    /* remove the transaction. */
    tableModel->removeRow(row);

    /* apply any changes. */
    mapper->submit();

    /* if it was the last transaction in the database. */
    if (!tableModel->rowCount()) {
        clearGUI(); /* clear the data from the gui objects .*/
        lockGUI();  /* lock all gui objects (readonly, disabled). */
        return;
    }

    /* select the appropriate transaction. */
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}

/* try to complete the transaction. */
void
TransactionForm::completeTransaction() {
    /* check if the are any rows in the model. */
    if (!tableModel->rowCount()) return;

    /* ask him/her if he/she wants to complete the transaction. */
    int r = QMessageBox::question(this, infoMsgTitleStr, completeTransactStr, QMessageBox::Yes | QMessageBox::No);

    /* if he/she don't want it just return and do nothing. */
    if (r == QMessageBox::No) return;

    /* get the row of the current transaction selected. */
    const int row = mapper->currentIndex();

    /* get the record of the specific transaction. */
    const QSqlRecord record = tableModel->record(row);

    /* get the id of the transaction. */
    const int tran_id = record.value(Transaction_Id).toInt();

    /* try to get the id of the customer. */
    const int cust_id = getCustomerId (tran_id);

    /* if it was impossible to get the id of the customer ignore transaction. */
    if (cust_id < 0) return;

    /* try to check the card type of the customer and fetch it. */
    const int card_type = checkCardType (cust_id);

    /* if card is expired or detection error occured ignore transaction. */
    if (card_type == ErrorCardType) return;

    /* get the start-end date related data. */
    const QDate start_date(record.value(Transaction_StartDate).toDate());
    const QTime start_time(record.value(Transaction_StartTime).toTime());
    const QDate end_date(QDate::currentDate());
    const QTime end_time(QTime::currentTime());

    /* calculate time the vehicle exists in the parking. */
    const int time = calculateTime (start_date, end_date, start_time, end_time);

    /* calculate the charge of the transaction. */
    const double charge = calculateCharge (card_type, time);

    /* get both customer and vehicle names. */
    const QString cust_name(record.value(Transaction_CustomerId).toString());
    const QString vehi_name(record.value(Transaction_VehicleId).toString());

    /* try to perform the payment. */
    if (!completePayment(card_type, cust_id, cust_name, charge))
        return; /* in case of ignore transaction. */

    /* transaction has been completed.
       save in report the transaction. */

    /* show a message. */
    QMessageBox::information(this, infoMsgTitleStr, transactSuccessStr);

    /* store the transaction in the report for future reference. */
    storeInReport (cust_name, vehi_name,
                   start_date, end_date,
                   start_time, end_time,
                   charge);

    /* remove the transaction. */
    tableModel->removeRow(row);

    /* apply any changes. */
    mapper->submit();

    /* if it was the last transaction in the database. */
    if (!tableModel->rowCount()) {
        clearGUI(); /* clear the data from the gui objects .*/
        lockGUI();  /* lock all gui objects (readonly, disabled). */
        return;
    }

    /* select the appropriate transaction. */
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}

/* get the id of the customer. */
int
TransactionForm::getCustomerId (const int tran_id) {
    /* declare a sql query object. */
    QSqlQuery query;

    /* prepare a sql query with place holders. */
    query.prepare("SELECT cust.id FROM customer AS cust INNER JOIN transacts AS tran ON cust.id = tran.cust_id WHERE tran.id = :tran_id");

    /* bind values to the query placeholders. */
    query.bindValue(":tran_id", tran_id);

    /* execute the query. */
    query.exec();

    /* the customer's id (assume not found). */
    int cust_id = -1;

    /* try to get the first record. */
    if (query.next()) {
        /* get the id of the customer. */
        cust_id = query.value(0).toInt();
    }

    /* return the id of the customer. */
    return cust_id;
}

/* get the money from the card of the customer. */
double
TransactionForm::getCardMoney (const int cust_id) {
    /* declare a sql query object. */
    QSqlQuery query;

    /* prepare a sql query with place holders. */
    query.prepare("SELECT cust.card_money FROM customer AS cust WHERE cust.id = :cust_id");

    /* bind values to the query placeholders. */
    query.bindValue(":cust_id", cust_id);

    /* execute the query. */
    query.exec();

    /* the customer's card money (assume not found). */
    double card_money = -1;

    /* try to get the first record. */
    if (query.next())
        /* if the money card is not empty. */
        if(!query.value(0).toString().isEmpty())
            /* get the card money of the customer. */
            card_money = query.value(0).toDouble();

    /* return the card money of the customer. */
    return card_money;
}

/* charge the card of the customer. */
bool
TransactionForm::chargeCustomerCard(const int cust_id, const double charge) {
    /* declare a sql query object. */
    QSqlQuery query;

    /* prepare a sql query with place holders. */
    query.prepare("UPDATE customer SET card_money = card_money - :charge WHERE customer.id = :cust_id");

    /* bind values to the query placeholders. */
    query.bindValue(":cust_id", cust_id);
    query.bindValue(":charge", charge);

    /* execute the query (return true/false for success/failure). */
    return query.exec();
}

/* check and return the card type of the customer. */
int
TransactionForm::checkCardType (const int cust_id) {
    /* declare a sql query object. */
    QSqlQuery query;

    /* prepare a sql query with place holders. */
    query.prepare("SELECT cust.card_id, cust.card_date FROM customer AS cust WHERE cust.id = :cust_id");

    /* bind values to the query placeholders. */
    query.bindValue(":cust_id", cust_id);

    /* execute the query. */
    query.exec();

    /* the customer's card type (assume not found). */
    int card_type = ErrorCardType;

    /* try to get the first record. */
    if (query.next()) {
        /* get the card type of the customer. */
        card_type = query.value(0).toInt() - 1; /* for fixing with indexes. */

        /* get the possible card date. */
        const QDate date = query.value(1).toDate();

        /* if the card has date check if it is expired. */
        if (card_type == MonthCardType) {
            if ((date.daysTo(QDate::currentDate()) + 1) > 30) { /* plus the card creation day. */
                /* show a message. */
                QMessageBox::warning(this, infoMsgTitleStr, cardExpiredStr);

                /* card is expired, stop transaction. */
                card_type = ErrorCardType;
            }
        }
        else if(card_type == YearCardType) {
            if (date.daysTo(QDate::currentDate()) > date.daysInYear()) {
                /* show a message. */
                QMessageBox::warning(this, infoMsgTitleStr, cardExpiredStr);

                /* card is expired, stop transaction. */
                card_type = ErrorCardType;
            }
        }
    }

    /* return the card type of the customer. */
    return card_type;
}

/* calculate time elapsed between start-end dates and times. */
int
TransactionForm::calculateTime (const QDate start_date, const QDate end_date,
                                const QTime start_time, const QTime end_time) {
    /* create datetime objects. */
    QDateTime startDateTime, endDateTime;

    /* set to datetime objects the appropriate values. */
    startDateTime.setDate(start_date);
    startDateTime.setTime(start_time);
    endDateTime.setDate(end_date);
    endDateTime.setTime(end_time);

    /* return the number of timeslices elapsed. */
    return startDateTime.secsTo(endDateTime);
}

/* return the charge for the transaction. */
double
TransactionForm::calculateCharge (const int card_type, const int time) {
    /* calculate the base charge. */
    const double base = (time * sets.chargePerTimeslice) / sets.timeslice;

    /* assume no charge first. */
    double charge = 0;

    /* according to the card type of the customer. */
    switch (card_type) {
        case NoCardType:
            {
                /* charge him/her the base charge. */
                charge = base;
                break;
            }
        case SimpleCardType:
            {
                /* charge him/her the base charge with discount. */
                charge = base - base*0.1;
                break;
            }
        case MonthCardType:
        case YearCardType:
            /* no charge. */
            break;
        case CreditCardType:
            {
                /* charge him/her the base charge. */
                charge = base;
                break;
            }
        case ErrorCardType: /* in case of an error card type. */
        default:            /* this should never happen. */
            break;
    }

    /* format the charge of the transaction and return it. */
    return QString::number(charge, 'f', sets.chargePrecision).toDouble();
}

/* try to perform the payment of the transaction. */
bool
TransactionForm::completePayment(const int card_type, const int cust_id, const QString cust_name, const double charge) {
    /* according to the card type of the customer. */
    switch (card_type) {
        case NoCardType:
        case SimpleCardType:
            {
                /* create the payment wizard. */
                PayWizard payform (cust_name, charge, this);

                /* execute the wizard form and return transaction completion status. */
                return payform.exec() == QDialog::Accepted;
                break; /* save for future bug. */
            }
        case MonthCardType:
        case YearCardType:
            {
                /* accept payment because the cards have been tested before. */
                return true;
                break; /* save for future bug. */
            }
        case CreditCardType:
            {
                /* get the money from the card of the customer. */
                const double card_money = getCardMoney(cust_id);

                /* if it was impossible to get card's money ignore transaction. */
                if (card_money < 0) return false;

                /* check if the customer can pay for the transaction. */
                if(isLessThan(card_money, charge)) {
                    /* show a message. */
                    QMessageBox::warning(this, infoMsgTitleStr, notManyCardMoneyStr + QString("%1").arg(charge));

                    /* ignorn the transaction. */
                    return false;
                }

                /* substracts the money from the customer's card
                   and accept or ignore the transaction payment. */
                return chargeCustomerCard(cust_id, charge);
                break; /* save for future bug. */
            }
        case ErrorCardType: /* in case of an error card type. */
        default:            /* this should never happen. */
            break;
    }

    /* ignore the transaction in any other case. */
    return false;
}

/* store in report the transaction. */
void
TransactionForm::storeInReport(const QString cust_name, const QString vehi_name,
                               const QDate start_date, const QDate end_date,
                               const QTime start_time, const QTime end_time,
                               const double charge) {
    /* declare a sql query object. */
    QSqlQuery query;

    /* prepare a sql query with place holders. */
    query.prepare("INSERT INTO report (vehicle, start_date, end_date, start_time, end_time, charge, customer) VALUES (:vehi_name, :start_date, :end_date, :start_time, :end_time, :charge, :cust_name)");

    /* bind values to the query placeholders. */
    query.bindValue(":vehicle", vehi_name);
    query.bindValue(":start_date", start_date);
    query.bindValue(":end_date", end_date);
    query.bindValue(":start_time", start_time);
    query.bindValue(":end_time", end_time);
    query.bindValue(":charge", charge);
    query.bindValue(":customer", cust_name);

    /* execute the query. */
    query.exec();
}

/* lock (readonly, disable) the gui objects. */
void
TransactionForm::lockGUI() {
    customerEdit->setReadOnly(true);
    vehicleEdit->setReadOnly(true);
    startDateEdit->setReadOnly(true);
    startTimeEdit->setReadOnly(true);
}

/* unlock (editable, enable) the gui objects. */
void
TransactionForm::unlockGUI() {
    customerEdit->setReadOnly(false);
    vehicleEdit->setReadOnly(false);
    startDateEdit->setReadOnly(false);
    startTimeEdit->setReadOnly(false);
}

/* clear the contents of the gui objects. */
void
TransactionForm::clearGUI() {
    customerEdit->clear();
    vehicleEdit->clear();
    startDateEdit->clear();
    startTimeEdit->clear();
}
