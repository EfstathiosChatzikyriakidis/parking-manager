/*
 *  This file implements the customer gui form and data model.
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
#include "customerform.h"
#include "paywizard.h"
#include "globaldeclarations.h"

/* creates the application's customer gui form and data model. */
CustomerForm::CustomerForm(const int id, QWidget *parent) : QDialog(parent) {
    /* create the appropriate customer line edits, labels and set buddies. */
    nameEdit = new QLineEdit;
    nameLabel = new QLabel(nameLabelStr);
    nameLabel->setBuddy(nameEdit);

    addressEdit = new QLineEdit;
    addressLabel = new QLabel(addressLabelStr);
    addressLabel->setBuddy(addressEdit);

    cityEdit = new QLineEdit;
    cityLabel = new QLabel(cityLabelStr);
    cityLabel->setBuddy(cityEdit);

    stateEdit = new QLineEdit;
    stateLabel = new QLabel(stateLabelStr);
    stateLabel->setBuddy(stateEdit);

    phoneEdit = new QLineEdit;
    phoneLabel = new QLabel(phoneLabelStr);
    phoneLabel->setBuddy(phoneEdit);

    emailEdit = new QLineEdit;
    emailLabel = new QLabel(emailLabelStr);
    emailLabel->setBuddy(emailEdit);

    cardDateEdit = new EmptyDateEdit(QDate()); /* Null Date. */
    cardDateLabel = new QLabel(cardDateLabelStr);
    cardDateLabel->setBuddy(cardDateEdit);

    cardMoneyEdit = new QLineEdit;
    cardMoneyLabel = new QLabel(cardMoneyLabelStr);
    cardMoneyLabel->setBuddy(cardMoneyEdit);

    cardComboBox = new QComboBox;
    cardTypeLabel = new QLabel(cardTypeLabelStr);
    cardTypeLabel->setBuddy(cardComboBox);

    /* create the management buttons. */
    cardButton = new QPushButton(cardButtonStr);
    addButton = new QPushButton(addButtonStr);
    deleteButton = new QPushButton(deleteButtonStr);
    closeButton = new QPushButton(closeButtonStr);

    /* add the buttons in a button box dialog. */
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::AcceptRole);

    /* set the signals/slots for the buttons' events. */
    connect(cardButton, SIGNAL(clicked()), this, SLOT(addEditCard()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addCustomer()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteCustomer()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cardComboBox, SIGNAL(activated(const int)), this, SLOT(handleCardType(const int)));

    /* create the relational table model for the customer. */
    tableModel = new QSqlRelationalTableModel(this);

    /* set the table to select. */
    tableModel->setTable("customer");

    /* get either a selected customer or none. */
    tableModel->setFilter(QString("customer.id = %1").arg(id));

    /* set a relation for the card type of the customer. */
    tableModel->setRelation(Customer_CardId, QSqlRelation("cardtype", "id", "title"));

    /* select the customer model in order to show the data. */
    tableModel->select();

    /* create the table model of the cards type for the customer. */
    QSqlTableModel *relationModel = tableModel->relationModel(Customer_CardId);

    /* set the card type table model to the card type combobox. */
    cardComboBox->setModel(relationModel);

    /* select the field to show in the combobox. */
    cardComboBox->setModelColumn(relationModel->fieldIndex("title"));

    /* create a data widget mapper for the customer fields. */
    mapper = new QDataWidgetMapper(this);

    /* set some operative options in the mapper. */
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));

    /* add to the map the customer GUI objects which manage the fields. */
    mapper->addMapping(nameEdit, Customer_Name);
    mapper->addMapping(addressEdit, Customer_Address);
    mapper->addMapping(cityEdit, Customer_City);
    mapper->addMapping(stateEdit, Customer_State);
    mapper->addMapping(phoneEdit, Customer_Phone);
    mapper->addMapping(emailEdit, Customer_Email);
    mapper->addMapping(cardDateEdit, Customer_CardDate);
    mapper->addMapping(cardMoneyEdit, Customer_CardMoney);
    mapper->addMapping(cardComboBox, Customer_CardId);

    /* lock all card related gui objects. */
    cardMoneyEdit->setReadOnly(true);
    cardDateEdit->setReadOnly(true);
    cardComboBox->setDisabled(true);

    /* if there is no customer selected. */
    if (!tableModel->rowCount()) {
        /* lock the rest gui objects (readonly, disabled). */
        lockGUI();

        /* none option index from the cardtype combobox. */
        previousCardType = -1;

        /* focus the close button. */
        closeButton->setFocus();
    }
    else {
        /* try to select the first one in the model. */
        mapper->toFirst();

        /* selected option index in the card combobox. */
        previousCardType = cardComboBox->currentIndex();
    }

    /* create a table grid. */
    QGridLayout *mainLayout = new QGridLayout;

    /* add the following objects in the appropriate position in the grid. */
    mainLayout->addWidget(nameLabel, 0, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(nameEdit, 0, 1, 1, 2);
    mainLayout->addWidget(cardTypeLabel, 1, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(cardComboBox, 1, 1);
    mainLayout->addWidget(cardButton, 1, 2);
    mainLayout->addWidget(cardDateLabel, 2, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(cardDateEdit, 2, 1, 1, 2);
    mainLayout->addWidget(cardMoneyLabel, 3, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(cardMoneyEdit, 3, 1, 1, 2);
    mainLayout->addWidget(addressLabel, 4, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(addressEdit, 4, 1, 1, 2);
    mainLayout->addWidget(cityLabel, 5, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(cityEdit, 5, 1, 1, 2);
    mainLayout->addWidget(stateLabel, 6, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(stateEdit, 6, 1, 1, 2);
    mainLayout->addWidget(phoneLabel, 7, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(phoneEdit, 7, 1, 1, 2);
    mainLayout->addWidget(emailLabel, 8, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(emailEdit, 8, 1, 1, 2);
    mainLayout->addWidget(buttonBox, 9, 0, 1, 3);

    /* set some GUI options for the table grid. */
    mainLayout->setRowMinimumHeight(9, 35);
    mainLayout->setRowStretch(9, 1);
    mainLayout->setColumnStretch(2, 1);

    /* do not allow to resize the form. */
    mainLayout->setSizeConstraint (QLayout::SetFixedSize);

    /* set the layout for the customer form. */
    setLayout(mainLayout);

    /* set the text of the form's window. */
    setWindowTitle(custWinTitleStr);
}

/* update any changes and close the form. */
void
CustomerForm::done(const int result) {
    /* check characters for sql injection issue. */
    if(nameEdit->text().contains(QRegExp(sqlInjectionRegExpStr))) {
        /* show a message. */
        QMessageBox::warning(this, infoMsgTitleStr, sqlInjectCustStr);

        /* do nothing. */
        return;
    }

    /* update any changes. */
    mapper->submit();

    /* return from the form. */
    QDialog::done(result);
}

/* ask the user if he/she wants to add / edit a card type. */
void
CustomerForm::addEditCard() {
    /* check if the are any rows in the model. */
    if (!tableModel->rowCount()) return;

    /* disable the card type combobox. */
    cardComboBox->setDisabled(true);

    /* ask him/her if he/she wants to add / edit a card type. */
    int r = QMessageBox::question(this, infoMsgTitleStr, cardCreationStr, QMessageBox::Yes | QMessageBox::No);

    /* if he/she don't want it just return and do nothing. */
    if (r == QMessageBox::No) return;

    /* enable the card type combobox and open the list. */
    cardComboBox->setDisabled(false);
    cardComboBox->showPopup();
}

/* adds a new customer in the database. */
void
CustomerForm::addCustomer() {
    /* check characters for sql injection issue. */
    if(nameEdit->text().contains(QRegExp(sqlInjectionRegExpStr))) {
        /* show a message. */
        QMessageBox::warning(this, infoMsgTitleStr, sqlInjectCustStr);

        /* do nothing. */
        return;
    }

    /* disable the add and delete button. */
    deleteButton->setDisabled(true);
    addButton->setDisabled(true);

    /* unlock the gui objects. */
    unlockGUI();

    /* get the selected customer. */
    int row = mapper->currentIndex();

    /* check if the are any rows in the model. */
    if (!tableModel->rowCount()) row = 0;

    /* perform any changes to the current customer. */
    mapper->submit();

    /* disable the card type combobox. */
    cardComboBox->setDisabled(true);

    /* clear the card date. */
    cardDateEdit->clear();

    /* create a new customer record. */
    tableModel->insertRow(row);

    /* select the new customer record. */
    mapper->setCurrentIndex(row);
}

/* deletes the current selected customer from the database. */
void
CustomerForm::deleteCustomer() {
    /* check if the are any rows in the model. */
    if (!tableModel->rowCount()) return;

    /* ask him/her if he/she wants to delete the customer. */
    int r = QMessageBox::question(this, infoMsgTitleStr, deleteCustomerStr, QMessageBox::Yes | QMessageBox::No);

    /* if he/she don't want it just return and do nothing. */
    if (r == QMessageBox::No) return;

    /* get the row of the current customer selected. */
    const int row = mapper->currentIndex();

    /* get the record of the specific customer. */
    QSqlRecord record = tableModel->record(row);

    /* get the id of the customer. */
    const int id = record.value(Customer_Id).toInt();

    /* declare a sql query object. */
    QSqlQuery query;

    /* execute queries to reparent the vehicles (if any) to simple guest. */
    query.exec(QString("UPDATE vehicle SET cust_id = 1 WHERE cust_id = %1").arg(id));
    query.exec(QString("UPDATE transacts SET cust_id = 1 WHERE cust_id = %1").arg(id));

    /* now remove also the customer. */
    tableModel->removeRow(row);

    /* apply any changes. */
    mapper->submit();

    /* disable the card type combobox. */
    cardComboBox->setDisabled(true);

    clearGUI(); /* clear the data from the gui objects .*/
    lockGUI();  /* lock all gui objects (readonly, disabled). */

    /* select the appropriate customer. */
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}

/* handle the card type selection. */
void
CustomerForm::handleCardType (const int index) {
    /* set editable the date card and card money. */
    cardMoneyEdit->setReadOnly(false);
    cardDateEdit->setReadOnly(false);

    /* switch according to the card type. */
    switch(index) {
        case ErrorCardType: /* no selection, empty list. */
        case NoCardType:
        case SimpleCardType:
            {
                /* clear both date and money of the card. */
                cardDateEdit->clear();
                cardMoneyEdit->clear();
                break;
            }
        case MonthCardType:
        case YearCardType:
            {
                /* create the payment wizard. */
                PayWizard payform (nameEdit->text(),
                                   index == MonthCardType ? MONTH_CARD_FEE : YEAR_CARD_FEE,
                                   this);

                /* execute the wizard form and return payment completion status. */
                if(payform.exec() == QDialog::Accepted) {
                    /* set the date of the card as current. */
                    cardDateEdit->setDate(QDate::currentDate());

                    /* clear the card money. */
                    cardMoneyEdit->clear();
                }
                else {
                    /* set the previous card type index from the combobox. */
                    cardComboBox->setCurrentIndex(previousCardType);
                }
                break;
            }
        case CreditCardType:
            {
                /* assume no money first. */
                double money = 0;

                /* assume that user didn't press ok from the dialog. */
                bool ok = false;

                /* get money the client wants. */
                money = QInputDialog::getDouble(this,
                                                infoMsgTitleStr,
                                                cardInputMoneyStr,
                                                0, 1, 1000, 2, &ok);

                /* if ok button is pressed from the dialog. */
                if(ok) {
                    /* create the payment wizard. */
                    PayWizard payform (nameEdit->text(), money, this);

                    /* execute the wizard form and return payment completion status. */
                    if(payform.exec() == QDialog::Accepted) {
                        /* clear the date of the card. */
                        cardDateEdit->clear();

                        /* set the new money to the edit (if changes happen). */
                        cardMoneyEdit->setText(QString("%1").arg(money));
                    }
                    else {
                        /* set the previous card type index from the combobox. */
                        cardComboBox->setCurrentIndex(previousCardType);
                    }
                }
                else {
                    /* set the previous card type index from the combobox. */
                    cardComboBox->setCurrentIndex(previousCardType);
                }
                break;
            }
        default: /* this should never happen. */
            break;
    }

    /* set readonly the date card and card money. */
    cardMoneyEdit->setReadOnly(true);
    cardDateEdit->setReadOnly(true);

    /* disable the card type combobox. */
    cardComboBox->setDisabled(true);
}

/* lock (readonly, disable) the gui objects. */
void
CustomerForm::lockGUI() {
    nameEdit->setReadOnly(true);
    addressEdit->setReadOnly(true);
    cityEdit->setReadOnly(true);
    stateEdit->setReadOnly(true);
    phoneEdit->setReadOnly(true);
    emailEdit->setReadOnly(true);
}

/* unlock (editable, enable) the gui objects. */
void
CustomerForm::unlockGUI() {
    nameEdit->setReadOnly(false);
    addressEdit->setReadOnly(false);
    cityEdit->setReadOnly(false);
    stateEdit->setReadOnly(false);
    phoneEdit->setReadOnly(false);
    emailEdit->setReadOnly(false);
}

/* clear the contents of the gui objects. */
void
CustomerForm::clearGUI() {
    nameEdit->clear();
    addressEdit->clear();
    cityEdit->clear();
    stateEdit->clear();
    phoneEdit->clear();
    emailEdit->clear();
    cardMoneyEdit->clear();
    cardDateEdit->clear();
}
