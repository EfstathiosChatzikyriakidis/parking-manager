/*
 *  This file implements the main gui form.
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
#include <QIcon>

/* include headers defining the interface of the sources. */
#include "vehicleform.h"
#include "customerform.h"
#include "transactionform.h"
#include "reportform.h"
#include "settingsform.h"
#include "mainform.h"
#include "appsettings.h"
#include "arithmetictools.h"

/* creates the application's main gui form. */
MainForm::MainForm() {
    /* try to establish app's settings. */
    establishSettings();

    /* create the panels for the customers and vehicles. */
    createCustomerPanel();
    createVehiclePanel();

    /* split horizontally the two panels with a splitter. */
    splitter = new QSplitter(Qt::Horizontal);
    splitter->setFrameStyle(QFrame::StyledPanel);
    splitter->addWidget(customerPanel);
    splitter->addWidget(vehiclePanel);

    /* create the buttons of the main form. */
    editButtonVehicle = new QPushButton(vehiclesButtonStr);
    editButtonCustomer = new QPushButton(customersButtonStr);
    editButtonTransaction = new QPushButton(transactsButtonStr);
    buttonReportTransaction = new QPushButton(reportButtonStr);
    buttonGuestVehicles = new QPushButton(guestVehiclesStr);
    quitButton = new QPushButton(quitButtonStr);
    settingsButton = new QPushButton(settingsButtonStr);

    /* create the about toolbutton. */
    aboutButton = new QToolButton;
    aboutButton->setIcon(QIcon(":/graphics/window/about-icon"));

    /* add the buttons in a button box dialog. */
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(buttonGuestVehicles, QDialogButtonBox::ActionRole);
    buttonBox->addButton(buttonReportTransaction, QDialogButtonBox::ActionRole);
    buttonBox->addButton(editButtonVehicle, QDialogButtonBox::ActionRole);
    buttonBox->addButton(editButtonCustomer, QDialogButtonBox::ActionRole);
    buttonBox->addButton(editButtonTransaction, QDialogButtonBox::ActionRole);

    /* set the following button as checkable (as toogle button). */
    buttonGuestVehicles->setCheckable(true);

    /* set the signals/slots for the buttons' events. */
    connect(customerView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editCustomers()));
    connect(vehicleView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editVehicles()));
    connect(buttonReportTransaction, SIGNAL(clicked()), this, SLOT(reportTransactions()));
    connect(editButtonTransaction, SIGNAL(clicked()), this, SLOT(editTransactions()));
    connect(editButtonCustomer, SIGNAL(clicked()), this, SLOT(editCustomers()));
    connect(editButtonVehicle, SIGNAL(clicked()), this, SLOT(editVehicles()));
    connect(buttonGuestVehicles, SIGNAL(toggled(const bool)), this, SLOT(handleGuestVehicles(const bool)));
    connect(settingsButton, SIGNAL(clicked()), this, SLOT(editSettings()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(aboutButton, SIGNAL(clicked()), this, SLOT(handleAbout()));

    /* create a vertical and a horizontal layout. */
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    QHBoxLayout *mainHLayout = new QHBoxLayout;

    /* add the splitter to the vertical layout. */
    mainVLayout->addWidget(splitter);

    /* set the settings button and the button box to the horizontal layout. */
    mainHLayout->addWidget(aboutButton);
    mainHLayout->addWidget(settingsButton);
    mainHLayout->addStretch();
    mainHLayout->addWidget(buttonBox, 0, Qt::AlignHCenter);
    mainHLayout->addStretch();
    mainHLayout->addWidget(quitButton);

    /* add the buttons bar to the vertical layout. */
    mainVLayout->addLayout(mainHLayout);

    /* set the layout to the main form. */
    setLayout(mainVLayout);

    /* set the window title of the main form. */
    setWindowTitle(appName);

    /* set the icon of the window. */
    setWindowIcon(QIcon(":/graphics/window/window-icon"));

    /* resize the form to dekstop size and also maximize the window. */
    resize(QApplication::desktop()->size());
    showMaximized();

    /* show/hide the header of the vehicles view according of the records count. */
    vehicleView->horizontalHeader()->setVisible(vehicleModel->rowCount() > 0);

    /* show/hide the header of the customers view according of the records count. */
    customerView->horizontalHeader()->setVisible(customerModel->rowCount() > 0);

    /* select the first customer (if any) in the view. */
    if(customerModel->rowCount() > 0)
      customerView->setCurrentIndex(customerModel->index(0, 0));
}

/* updates the view with the vehicles data. */
void
MainForm::updateVehicleView() {
    /* get the current selected customer from the view. */
    const QModelIndex index = customerView->currentIndex();

    /* if the customer is valid. */
    if (index.isValid()) {
        /* get the record of the customer. */
        const QSqlRecord record = customerModel->record(index.row());

        /* get the id of the customer. */
        const int id = record.value(CustomerForm::Customer_Id).toInt();

        /* filter vehicles depending on the customer's id. */
        vehicleModel->setFilter(QString("cust_id = %1").arg(id));

        /* show the appropriate message for this selection. */
        vehicleLabel->setText(vehiclesOfStr.arg(record.value(CustomerForm::Customer_Name).toString()));
    } else {
        /* do not show any vehicles. */
        vehicleModel->setFilter("cust_id = -1");

        /* show the appropriate message for this selection. */
        vehicleLabel->setText(vehiclesStr);
    }

    /* select the vehicle model in order to apply the new filtering. */
    vehicleModel->select();

    /* show/hide the header of the view according of the records count. */
    vehicleView->horizontalHeader()->setVisible(vehicleModel->rowCount() > 0);

    /* perform some operations with columns' width. */
    vehicleView->resizeColumnsToContents();
    vehicleView->horizontalHeader()->setStretchLastSection(true);
}

/* updates the view with the customers data. */
void
MainForm::updateCustomerView() {
    /* select the customers model in order to show the new changes. */
    customerModel->select();

    /* show/hide the header of the view according of the records count. */
    customerView->horizontalHeader()->setVisible(customerModel->rowCount() > 0);

    /* perform some operations with columns' width. */
    customerView->resizeColumnsToContents();
    customerView->horizontalHeader()->setStretchLastSection(true);
}

/* opens a form to manager application's settings. */
void
MainForm::editSettings() {
    /* declare the form which manages settings. */
    SettingsForm form(sets, this);

    /* execute the form. */
    form.exec();

    /* try to establish again any changes. */
    establishSettings();
}

/* opens a form to manage vehicles. */
void
MainForm::editVehicles() {
    /* assume that no vehicle is selected from the view. */
    int vehicleId = -1;

    /* try to get the selected vehicle from the view. */
    const QModelIndex index = vehicleView->currentIndex();

    /* if the selected vehicle is valid. */
    if (index.isValid()) {
        /* get the record of the vehicle. */
        const QSqlRecord record = vehicleModel->record(index.row());

        /* get the id of the vehicle. */
        vehicleId = record.value(VehicleForm::Vehicle_Id).toInt();
    }

    /* declare the form which manages vehicles. */
    VehicleForm form(sets.parkingCapacity, vehicleId, this);

    /* execute the form. */
    form.exec();

    /* update the view of the vehicles because changes may happen. */
    updateVehicleView();
}

/* opens a form to manage customers. */
void
MainForm::editCustomers() {
    /* assume that no customer is selected from the view. */
    int customerId = -1;

    /* try to get the selected customer from the view. */
    const QModelIndex index = customerView->currentIndex();

    /* if the selected customer is valid. */
    if (index.isValid()) {
        /* get the record of the customer. */
        const QSqlRecord record = customerModel->record(index.row());

        /* get the id of the customer. */
        customerId = record.value(CustomerForm::Customer_Id).toInt();
    }

    /* declare the form which manages customers. */
    CustomerForm form(customerId, this);

    /* execute the form. */
    form.exec();

    /* update the view of the customers because changes may happen. */
    updateCustomerView();
}

/* opens a form to manage transactions. */
void
MainForm::editTransactions() {
    /* declare the form which manages transactions. */
    TransactionForm form(sets, this);

    /* execute the form. */
    form.exec();
}

/* opens a form to report transactions. */
void
MainForm::reportTransactions() {
    /* declare the form which reports transactions. */
    ReportForm form(this);

    /* execute the form. */
    form.exec();
}

/* opens an about dialog. */
void
MainForm::handleAbout() {
    QMessageBox::about(this, appName,
                       "<h2>" + appName + " " + appVersion + "</h2>"
                       "<p>" + appCopyright + "</p>"
                       "<p align='justify'>" + appLicense  + "</p>");
}


/* show/hide guest's vehicles. */
void
MainForm::handleGuestVehicles(const bool buttonPressed) {
    /* if the button is pressed. */
    if (buttonPressed) {
        /* show guest's vehicles. */
        vehicleModel->setFilter("cust_id = 1");

        /* show the appropriate message for this selection. */
        vehicleLabel->setText(vehiclesStr);

        /* perform some operations with columns' width. */
        vehicleView->resizeColumnsToContents();
        vehicleView->horizontalHeader()->setStretchLastSection(true);

        /* show/hide the header of the view according of the records count. */
        vehicleView->horizontalHeader()->setVisible(vehicleModel->rowCount() > 0);
    }
    else {
        /* if the button is not pressed update the vehicle view. */
        updateVehicleView();
    }
}

/* establish the settings of the program.
   also check, create them if not exist. */
void MainForm::establishSettings() {
    QSettings s(setsAppOrg, setsAppName);

    s.beginGroup("parking");
    if (s.value("capacity").isNull()
        || s.value("capacity").toInt() < MIN_PARKING_CAPACITY
        || s.value("capacity").toInt() > MAX_PARKING_CAPACITY) {
        s.setValue("capacity", DEF_PARKING_CAPACITY);
    } else {
        sets.parkingCapacity = s.value("capacity").toInt();
    }
    s.endGroup();

    s.beginGroup("payment");
    if (s.value("timeslice").isNull()
        || s.value("timeslice").toInt() < MIN_TIMESLICE
        || s.value("timeslice").toInt() > MAX_TIMESLICE) {
        s.setValue("timeslice", DEF_TIMESLICE);
    } else {
        sets.timeslice = s.value("timeslice").toInt();
    }

    if (s.value("charge_per_timeslice").isNull()
        || isLessThan(s.value("charge_per_timeslice").toDouble(), MIN_CHARGE_PER_TIMESLICE)
        || isGreaterThan(s.value("charge_per_timeslice").toDouble(), MAX_CHARGE_PER_TIMESLICE)) {
        s.setValue("charge_per_timeslice", DEF_CHARGE_PER_TIMESLICE);
    } else {
        sets.chargePerTimeslice = s.value("charge_per_timeslice").toDouble();
    }

    if (s.value("charge_precision").isNull()
        || s.value("charge_precision").toInt() < MIN_CHARGE_PRECISION
        || s.value("charge_precision").toInt() > MAX_CHARGE_PRECISION) {
        s.setValue("charge_precision", DEF_CHARGE_PRECISION);
    } else {
        sets.chargePrecision = s.value("charge_precision").toInt();
    }

    s.endGroup();

    /* try to write down the settings. */
    s.sync();

    /* if the app cannot write the settings. */
    if (s.status() != QSettings::NoError) {
        /* show a message. */
        QMessageBox::critical(this, infoMsgTitleStr, cantWriteSetsStr);

        /* terminate the program. */
        exit(EXIT_FAILURE);
    }
}

/* creates the panel for the customers. */
void
MainForm::createCustomerPanel() {
    /* create the panel for the customers. */
    customerPanel = new QWidget;

    /* create the model for the customers. */
    customerModel = new QSqlRelationalTableModel(this);

    /* set the table to select. */
    customerModel->setTable("customer");

    /* get only the real customers. */
    customerModel->setFilter("customer.id != 1");

    /* set a relation for the card type of the customer. */
    customerModel->setRelation(CustomerForm::Customer_CardId, QSqlRelation("cardtype", "id", "title"));

    /* sort in ascending order by customer name. */
    customerModel->setSort(CustomerForm::Customer_Name, Qt::AscendingOrder);

    /* set the headers text. */
    customerModel->setHeaderData(CustomerForm::Customer_Name, Qt::Horizontal, nameStr);
    customerModel->setHeaderData(CustomerForm::Customer_CardId, Qt::Horizontal, cardStr);
    customerModel->setHeaderData(CustomerForm::Customer_Phone, Qt::Horizontal, phoneStr);

    /* select the model in order to apply the changes. */
    customerModel->select();

    /* create the table view of the customers. */
    customerView = new QTableView;

    /* assign the model to the table view. */
    customerView->setModel(customerModel);

    /* set some operative options in the table view. */
    customerView->setItemDelegate(new QSqlRelationalDelegate(this));
    customerView->setSelectionMode(QAbstractItemView::SingleSelection);
    customerView->setSelectionBehavior(QAbstractItemView::SelectRows);
    customerView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* hide the following columns. */
    customerView->setColumnHidden(CustomerForm::Customer_Id, true);
    customerView->setColumnHidden(CustomerForm::Customer_City, true);
    customerView->setColumnHidden(CustomerForm::Customer_State, true);
    customerView->setColumnHidden(CustomerForm::Customer_Address, true);
    customerView->setColumnHidden(CustomerForm::Customer_Email, true);
    customerView->setColumnHidden(CustomerForm::Customer_CardDate, true);
    customerView->setColumnHidden(CustomerForm::Customer_CardMoney, true);

    /* perform some operations with columns' width. */
    customerView->resizeColumnsToContents();
    customerView->horizontalHeader()->setStretchLastSection(true);

    /* signal/slot assignment in order to filter the vehicles when a customer is changed. */
    connect(customerView->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(updateVehicleView()));

    /* create the label before the panel. */
    customerLabel = new QLabel(customersStr);
    customerLabel->setBuddy(customerView);

    /* create a vertical layout. */
    QVBoxLayout *layout = new QVBoxLayout;

    /* add to the layout the label describing the table view. */
    layout->addWidget(customerLabel);

    /* add to the layout the table view. */
    layout->addWidget(customerView);

    /* add the layout to the panel. */
    customerPanel->setLayout(layout);
}

/* creates the panel for the vehicles. */
void
MainForm::createVehiclePanel() {
    /* create the panel for the vehicles. */
    vehiclePanel = new QWidget;

    /* create the model for the vehicles. */
    vehicleModel = new QSqlRelationalTableModel(this);

    /* set the table to select. */
    vehicleModel->setTable("vehicle");

    /* set a relation for the customer of the vehicle. */
    vehicleModel->setRelation(VehicleForm::Vehicle_CustomerId, QSqlRelation("customer", "id", "name"));

    /* sort in ascending order by vehicle name. */
    vehicleModel->setSort(VehicleForm::Vehicle_Name, Qt::AscendingOrder);

    /* set the headers text. */
    vehicleModel->setHeaderData(VehicleForm::Vehicle_Name, Qt::Horizontal, vehicleStr);
    vehicleModel->setHeaderData(VehicleForm::Vehicle_Description, Qt::Horizontal, descStr);

    /* select the model in order to apply the changes. */
    vehicleModel->select();

    /* create the table view of the vehicles. */
    vehicleView = new QTableView;

    /* assign the model to the table view. */
    vehicleView->setModel(vehicleModel);

    /* set some operative options in the table view. */
    vehicleView->setSelectionMode(QAbstractItemView::SingleSelection);
    vehicleView->setSelectionBehavior(QAbstractItemView::SelectRows);
    vehicleView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    vehicleView->horizontalHeader()->setStretchLastSection(true);

    /* hide the following columns. */
    vehicleView->setColumnHidden(VehicleForm::Vehicle_Id, true);
    vehicleView->setColumnHidden(VehicleForm::Vehicle_CustomerId, true);

    /* perform some operations with columns' width. */
    vehicleView->resizeColumnsToContents();
    vehicleView->horizontalHeader()->setStretchLastSection(true);

    /* create the label before the panel. */
    vehicleLabel = new QLabel(vehiclesStr);
    vehicleLabel->setBuddy(vehicleView);

    /* create a vertical layout. */
    QVBoxLayout *layout = new QVBoxLayout;

    /* add to the layout the label describing the table view. */
    layout->addWidget(vehicleLabel);

    /* add to the layout the table view. */
    layout->addWidget(vehicleView);

    /* add the layout to the panel. */
    vehiclePanel->setLayout(layout);
}
