/*
 *  This file implements the vehicle gui form and data model.
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

/* include some QT libraries. */
#include <QtGui>
#include <QtSql>

/* include headers defining the interface of the sources. */
#include "vehicleform.h"
#include "globaldeclarations.h"

/* creates the application's vehicles gui form and data model. */
VehicleForm::VehicleForm(const int capacity, int id, QWidget *parent) : QDialog(parent) {
    /* store the parking capacity value. */
    this->capacity = capacity;

    /* create the appropriate vehicles line edits, labels and set buddies. */
    nameEdit = new QLineEdit;
    nameLabel = new QLabel(nameLabelStr);
    nameLabel->setBuddy(nameEdit);

    descEdit = new QLineEdit;
    descLabel = new QLabel(descLabelStr);
    descLabel->setBuddy(descEdit);

    customerComboBox = new QComboBox;
    customerLabel = new QLabel(customerLabelStr);
    customerLabel->setBuddy(customerComboBox);

    /* create the management buttons. */
    addButton = new QPushButton(addButtonStr);
    deleteButton = new QPushButton(deleteButtonStr);
    transactionButton = new QPushButton(transactButtonStr);
    closeButton = new QPushButton(closeButtonStr);

    /* add the buttons in a button box dialog. */
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(transactionButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::AcceptRole);

    /* set the signals/slots for the buttons' events. */
    connect(addButton, SIGNAL(clicked()), this, SLOT(addVehicle()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteVehicle()));
    connect(transactionButton, SIGNAL(clicked()), this, SLOT(transactionVehicle()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));

    /* create the relational table model for the vehicles. */
    tableModel = new QSqlRelationalTableModel(this);

    /* set the table to select. */
    tableModel->setTable("vehicle");

    /* get either a selected vehicle or none. */
    tableModel->setFilter(QString("vehicle.id = %1").arg(id));

    /* set a relation for the customer of the vehicle. */
    tableModel->setRelation(Vehicle_CustomerId, QSqlRelation("customer", "id", "name"));

    /* select the vehicles model in order to show the data. */
    tableModel->select();

    /* create the table model of the customer for the vehicle. */
    QSqlTableModel *relationModel = tableModel->relationModel(Vehicle_CustomerId);

    /* set the vehicle table model to the customer combobox. */
    customerComboBox->setModel(relationModel);

    /* select the field to show in the combobox. */
    customerComboBox->setModelColumn(relationModel->fieldIndex("name"));

    /* create a data widget mapper for the vehicles fields. */
    mapper = new QDataWidgetMapper(this);

    /* set some operative options in the mapper. */
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));

    /* add to the map the vehicle GUI objects which manage the fields. */
    mapper->addMapping(nameEdit, Vehicle_Name);
    mapper->addMapping(descEdit, Vehicle_Description);
    mapper->addMapping(customerComboBox, Vehicle_CustomerId);

    /* if there are no vehicles in the database. */
    if (!tableModel->rowCount()) {
        /* lock all gui objects (readonly, disabled). */
        lockGUI();

        /* focus the close button. */
        closeButton->setFocus();
    }
    else {
        /* try to select the first one in the model. */
        mapper->toFirst();
    }

    /* create a table grid. */
    QGridLayout *mainLayout = new QGridLayout;

    /* add the following objects in the appropriate position in the grid. */
    mainLayout->addWidget(nameLabel, 0, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(nameEdit, 0, 1);
    mainLayout->addWidget(customerLabel, 2, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(customerComboBox, 2, 1);
    mainLayout->addWidget(descLabel, 3, 0, 1, 1, Qt::AlignRight);
    mainLayout->addWidget(descEdit, 3, 1);
    mainLayout->addWidget(buttonBox, 4, 0, 1, 2);

    /* set some GUI options for the table grid. */
    mainLayout->setRowMinimumHeight(4, 35);
    mainLayout->setRowStretch(4, 1);
    mainLayout->setColumnStretch(2, 1);

    /* do not allow to resize the form. */
    mainLayout->setSizeConstraint (QLayout::SetFixedSize);

    /* set the layout for the vehicles form. */
    setLayout(mainLayout);

    /* set the text of the form's window. */
    setWindowTitle(vehiWinTitleStr);
}

/* update any changes and close the form. */
void
VehicleForm::done(const int result) {
    /* check characters for sql injection issue. */
    if(nameEdit->text().contains(QRegExp(sqlInjectionRegExpStr))) {
        /* show a message. */
        QMessageBox::warning(this, infoMsgTitleStr, sqlInjectVehiStr);

        /* do nothing. */
        return;
    }

    /* update any changes. */
    mapper->submit();

    /* return from the form. */
    QDialog::done(result);
}

/* adds a new vehicle in the database. */
void
VehicleForm::addVehicle() {
    /* check characters for sql injection issue. */
    if(nameEdit->text().contains(QRegExp(sqlInjectionRegExpStr))) {
        /* show a message. */
        QMessageBox::warning(this, infoMsgTitleStr, sqlInjectVehiStr);

        /* do nothing. */
        return;
    }

    /* disable the add and delete button. */
    deleteButton->setDisabled(true);
    addButton->setDisabled(true);

    /* unlock the gui objects. */
    unlockGUI();

    /* get the selected vehicle. */
    int row = mapper->currentIndex();

    /* check if the are any rows in the model. */
    if (!tableModel->rowCount()) row = 0;

    /* perform any changes to it. */
    mapper->submit();

    /* create a new vehicle record. */
    tableModel->insertRow(row);

    /* select the new vehicle record. */
    mapper->setCurrentIndex(row);
}

/* deletes the current selected vehicle from the database. */
void
VehicleForm::deleteVehicle() {
    /* check if the are any rows in the model. */
    if (!tableModel->rowCount()) return;

    /* ask him/her if he/she wants to delete the vehicle. */
    int r = QMessageBox::question(this, infoMsgTitleStr, deleteVehicleStr, QMessageBox::Yes | QMessageBox::No);

    /* if he/she don't want it just return and do nothing. */
    if (r == QMessageBox::No) return;

    /* get the row of the current vehicle selected. */
    const int row = mapper->currentIndex();

    /* get the record of the specific vehicle. */
    const QSqlRecord record = tableModel->record(row);

    /* get the id of the vehicle. */
    const int id = record.value(Vehicle_Id).toInt();

    /* declare a sql query object. */
    QSqlQuery query;

    /* check if the vehicle is already in the parking. */
    query.exec(QString("SELECT * FROM transacts WHERE vehi_id = %1").arg(id));

    /* try to get the first record. */
    if (query.next()) {
        /* show a message. */
        QMessageBox::information(this, infoMsgTitleStr, vehicleReservedStr);

        /* do nothing more and return. */
        return;
    }

    /* remove the vehicle. */
    tableModel->removeRow(row);

    /* apply any changes. */
    mapper->submit();

    clearGUI(); /* clear the data from the gui objects .*/
    lockGUI();  /* lock all gui objects (readonly, disabled). */

    /* select the appropriate vehicle. */
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}

/* starts a vehicle transaction in the database. */
void
VehicleForm::transactionVehicle() {
    /* check if the are any rows in the model. */
    if (!tableModel->rowCount()) return;

    /* get the row of the current vehicle selected. */
    const int row = mapper->currentIndex();

    /* get the record of the specific vehicle. */
    const QSqlRecord record = tableModel->record(row);

    /* get the id of the vehicle. */
    const int vehiId = record.value(Vehicle_Id).toInt();

    /* declare a sql query object. */
    QSqlQuery query;

    /* check if the vehicle is already in the parking. */
    query.exec(QString("SELECT * FROM transacts WHERE vehi_id = %1").arg(vehiId));

    /* try to get the first record. */
    if (query.next()) {
        /* show a message. */
        QMessageBox::information(this, infoMsgTitleStr, vehicleReservedStr);

        /* do nothing more and return. */
        return;
    }

    /* execute a query to find the number of transactions. */
    query.exec("SELECT COUNT(*) FROM transacts");

    /* assume zero transactions. */
    int numTransacts = 0;

    /* try to get the first record. */
    if (query.next()) {
        /* get the result from count function. */
        numTransacts = query.value(0).toInt();
    }

    /* check if there is some vehicles capacity left. */
    if (numTransacts >= capacity) {
        /* show a message. */
        QMessageBox::information(this, infoMsgTitleStr, noCapacityInDBStr);

        /* do nothing more and return. */
        return;
    }

    /* find the id of the vehicle's customer. */

    /* prepare a sql query with place holders. */
    query.prepare("SELECT c.id FROM customer AS c INNER JOIN vehicle AS v ON c.id = v.cust_id WHERE v.id = :vehi_id");

    /* bind values to the query placeholders. */
    query.bindValue(":vehi_id", vehiId);

    /* execute the query. */
    query.exec();

    /* the vehicle's customer id (assume not found). */
    int custId = -1;

    /* try to get the first record. */
    if (query.next())
        /* get the id of the vehicle's customer. */
        custId = query.value(0).toInt();
    else
        /* ignore the process and do nothing. */
        return;

    /* start a transaction. */

    /* prepare a sql query with place holders. */
    query.prepare("INSERT INTO transacts (vehi_id, cust_id, start_date, start_time) VALUES (:vehi_id, :cust_id, :start_date, :start_time)");

    /* bind values to the query placeholders. */
    query.bindValue(":vehi_id", vehiId);
    query.bindValue(":cust_id", custId);
    query.bindValue(":start_date", QDate::currentDate());
    query.bindValue(":start_time", QTime::currentTime());

    /* execute the query. */
    query.exec();
}

/* lock (readonly, disable) the gui objects. */
void
VehicleForm::lockGUI() {
    descEdit->setReadOnly(true);
    nameEdit->setReadOnly(true);
    customerComboBox->setEnabled(false);
}

/* unlock (editable, enable) the gui objects. */
void
VehicleForm::unlockGUI() {
    descEdit->setReadOnly(false);
    nameEdit->setReadOnly(false);
    customerComboBox->setEnabled(true);
}

/* clear the contents of the gui objects. */
void
VehicleForm::clearGUI() {
    descEdit->clear();
    nameEdit->clear();
    customerComboBox->setEnabled(false);
}
