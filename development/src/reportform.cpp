/*
 *  This file implements the report gui form and data model.
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
#include "reportform.h"
#include "globaldeclarations.h"

/* creates the application's report gui form and data model. */
ReportForm::ReportForm(QWidget *parent) : QDialog(parent) {
    /* create the panel for the transactions report. */
    createReportPanel();

    /* create the buttons of the report form. */
    closeButton = new QPushButton(closeButtonStr);
    clearButton = new QPushButton(clearButtonStr);
    showAllButton = new QPushButton(showAllButtonStr);

    /* add the buttons in a button box dialog. */
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(closeButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(showAllButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(clearButton, QDialogButtonBox::AcceptRole);

    /* set the signals/slots for the buttons' events. */
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(showAllButton, SIGNAL(clicked()), this, SLOT(showAllReport()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearReport()));

    /* set the report panel and the button box vertically in a layout. */
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(reportPanel);
    mainLayout->addWidget(buttonBox, 0, Qt::AlignHCenter);

    /* set the layout to the report form. */
    setLayout(mainLayout);

    /* set the window title of the report form. */
    setWindowTitle(repoWinTitleStr);

    /* show/hide the header of the report view according of the records count. */
    reportView->horizontalHeader()->setVisible(tableModel->rowCount() > 0);

    /* resize the form to dekstop size and also maximize the window. */
    resize(QApplication::desktop()->size());
    showMaximized();
}

/* close the form and return. */
void
ReportForm::done(const int result) {
    /* return from the form. */
    QDialog::done(result);
}

/* creates the panel for the transactions report. */
void
ReportForm::createReportPanel() {
    /* create the panel for the report. */
    reportPanel = new QWidget;

    /* create the model for the report. */
    tableModel = new QSqlTableModel(this);

    /* set the table to select. */
    tableModel->setTable("report");

    /* sort in ascending order by transaction start date. */
    tableModel->setSort(Report_StartDate, Qt::AscendingOrder);

    /* set the headers text. */
    tableModel->setHeaderData(Report_Vehicle, Qt::Horizontal, vehicleStr);
    tableModel->setHeaderData(Report_Customer, Qt::Horizontal, customerStr);
    tableModel->setHeaderData(Report_StartDate, Qt::Horizontal, startDateStr);
    tableModel->setHeaderData(Report_EndDate, Qt::Horizontal, endDateStr);
    tableModel->setHeaderData(Report_StartTime, Qt::Horizontal, startTimeStr);
    tableModel->setHeaderData(Report_EndTime, Qt::Horizontal, endTimeStr);
    tableModel->setHeaderData(Report_Charge, Qt::Horizontal, chargeStr);

    /* select the model in order to apply the changes. */
    tableModel->select();

    /* create the table view of the report. */
    reportView = new QTableView;

    /* assign the model to the table view. */
    reportView->setModel(tableModel);

    /* set some operative options in the table view. */
    reportView->setItemDelegate(new QSqlRelationalDelegate(this));
    reportView->setSelectionMode(QAbstractItemView::SingleSelection);
    reportView->setSelectionBehavior(QAbstractItemView::SelectRows);
    reportView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* hide the following columns. */
    reportView->setColumnHidden(Report_Id, true);

    /* perform some operations with columns' width. */
    reportView->resizeColumnsToContents();
    reportView->horizontalHeader()-> setStretchLastSection(true);

    /* create the buttons of the filtering options. */
    dateShowButton = new QPushButton(showButtonStr);
    vehiShowButton = new QPushButton(showButtonStr);
    custShowButton = new QPushButton(showButtonStr);

    /* create line edits and buddies objects for report filtering. */
    vehiFilterEdit = new QLineEdit;
    vehiFilterLabel = new QLabel(vehicleLabelStr);
    vehiFilterLabel->setBuddy(vehiFilterEdit);

    custFilterEdit = new QLineEdit;
    custFilterLabel = new QLabel(customerLabelStr);
    custFilterLabel->setBuddy(custFilterEdit);

    fromDateFilterEdit = new QDateEdit;
    fromDateFilterLabel = new QLabel(fromDateLabelStr);
    fromDateFilterLabel->setBuddy(fromDateFilterEdit);

    toDateFilterEdit = new QDateEdit;
    toDateFilterLabel = new QLabel(toDateLabelStr);
    toDateFilterLabel->setBuddy(toDateFilterEdit);

    /* set some operative options for the date edits objects. */
    fromDateFilterEdit->setDisplayFormat("yyyy-MM-dd");
    fromDateFilterEdit->setCalendarPopup(true);
    fromDateFilterEdit->setDate(QDate::currentDate());

    toDateFilterEdit->setDisplayFormat("yyyy-MM-dd");
    toDateFilterEdit->setCalendarPopup(true);
    toDateFilterEdit->setDate(QDate::currentDate());

    /* set the signals/slots for the buttons' events. */
    connect(dateShowButton, SIGNAL(clicked()), this, SLOT(dateShowReport()));
    connect(vehiShowButton, SIGNAL(clicked()), this, SLOT(vehiShowReport()));
    connect(custShowButton, SIGNAL(clicked()), this, SLOT(custShowReport()));

    /* create a table grid. */
    QGridLayout *filterLayout = new QGridLayout;

    /* add the following objects in the appropriate position in the grid. */
    filterLayout->addWidget(fromDateFilterLabel, 0, 0, 1, 1, Qt::AlignRight);
    filterLayout->addWidget(fromDateFilterEdit, 0, 1);
    filterLayout->addWidget(toDateFilterLabel, 0, 2, 1, 1, Qt::AlignRight);
    filterLayout->addWidget(toDateFilterEdit, 0, 3);
    filterLayout->addWidget(dateShowButton, 0, 4);

    filterLayout->addWidget(custFilterLabel, 0, 5, 1, 1, Qt::AlignRight);
    filterLayout->addWidget(custFilterEdit, 0, 6);
    filterLayout->addWidget(custShowButton, 0, 7);

    filterLayout->addWidget(vehiFilterLabel, 0, 8, 1, 1, Qt::AlignRight);
    filterLayout->addWidget(vehiFilterEdit, 0, 9);
    filterLayout->addWidget(vehiShowButton, 0, 10);

    /* set some GUI options for the table grid. */
    filterLayout->setRowMinimumHeight(0, 40);
    filterLayout->setRowStretch(0, 1);

    /* create a vertical layout. */
    QVBoxLayout *layoutV = new QVBoxLayout;

    /* add the horizontal layout. */
    layoutV->addLayout(filterLayout);

    /* add to the layout the table view. */
    layoutV->addWidget(reportView);

    /* add the layout to the panel. */
    reportPanel->setLayout(layoutV);
}

/* renew the filtering of the report table view (depends on date). */
void
ReportForm::dateShowReport() {
    /* check if the 'To Date' is less than 'From Date'. */
    if(toDateFilterEdit->text() < fromDateFilterEdit->text()) {
        /* show a message. */
        QMessageBox::warning(this, infoMsgTitleStr, datesRelationStr);

        /* just return, do nothing. */
        return;
    }

    /* set the filter again, which performs changes in the report view. */
    tableModel->setFilter(QString("start_date BETWEEN '%1' AND '%2'").arg(fromDateFilterEdit->text(),
                                                                          toDateFilterEdit->text()));

    /* fix size related issues of the report view. */
    fixReportSize();
}

/* renew the filtering of the report table view (depends on customer). */
void
ReportForm::custShowReport() {
    /* if the customer field is empty do not filter. */
    if(custFilterEdit->text().trimmed().isEmpty())
        return;

    /* check characters for sql injection issue. */
    if(custFilterEdit->text().contains(QRegExp(sqlInjectionRegExpStr))) {
        QMessageBox::warning(this, infoMsgTitleStr, sqlInjectCustStr);
        return;
    }

    /* set the filter again, which performs changes in the report view. */
    tableModel->setFilter(QString("customer LIKE '%%1%'").arg(custFilterEdit->text()));

    /* fix size related issues of the report view. */
    fixReportSize();
}

/* renew the filtering of the report table view (depends on vehicle). */
void
ReportForm::vehiShowReport() {
    /* if the vehicle field is empty do not filter. */
    if(vehiFilterEdit->text().trimmed().isEmpty())
        return;

    /* check characters for sql injection issue. */
    if(vehiFilterEdit->text().contains(QRegExp(sqlInjectionRegExpStr))) {
        QMessageBox::warning(this, infoMsgTitleStr, sqlInjectVehiStr);
        return;
    }

    /* set the filter again, which performs changes in the report view. */
    tableModel->setFilter(QString("vehicle LIKE '%%1%'").arg(vehiFilterEdit->text()));

    /* fix size related issues of the report view. */
    fixReportSize();
}

/* show all the transactions. */
void
ReportForm::showAllReport() {
    /* set a filter to get all the records. */
    tableModel->setFilter("");

    /* fix size related issues of the report view. */
    fixReportSize();
}

/* try to clear the report. */
void
ReportForm::clearReport() {
    /* set a filter to get all the records. */
    tableModel->setFilter("");

    /* if there are records in the report. */
    if (tableModel->rowCount()) {
        /* ask him/her if he/she wants to clear the report. */
        int r = QMessageBox::question(this, infoMsgTitleStr, clearReportStr, QMessageBox::Yes | QMessageBox::No);

        /* if he/she don't want it just return and do nothing. */
        if (r == QMessageBox::No) return;

        /* remove all records from the report. */
        tableModel->removeRows(0, tableModel->rowCount());

        /* apply the changes. */
        tableModel->submitAll();
    }
}

/* apply some fix size related issues for the report view. */
void
ReportForm::fixReportSize() {
    /* show/hide the header of the report view according of the records count. */
    reportView->horizontalHeader()->setVisible(tableModel->rowCount() > 0);

    /* perform some operations with columns' width. */
    reportView->resizeColumnsToContents();
    reportView->horizontalHeader()-> setStretchLastSection(true);
}
