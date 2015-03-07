/*
 *  This file implements the settings gui form.
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

/* include headers defining the interface of the sources. */
#include "settingsform.h"
#include "globaldeclarations.h"
#include "appsettings.h"

/* creates the application's settings gui form. */
SettingsForm::SettingsForm(const appSettings sets, QWidget *parent) : QDialog(parent) {
    /* store the application's settings. */
    this->sets = sets;

    /* create the appropriate settings line edits, labels and set buddies. */
    parkingCapacitySpin = new QSpinBox;
    parkingCapacityLabel = new QLabel(parkingCapacityLabelStr);
    parkingCapacityLabel->setBuddy(parkingCapacitySpin);

    chargeTimesliceSpin = new QSpinBox;
    chargeTimesliceLabel = new QLabel(chargeTimesliceLabelStr);
    chargeTimesliceLabel->setBuddy(chargeTimesliceSpin);

    chargePerSliceSpin = new QDoubleSpinBox;
    chargePerSliceLabel = new QLabel(chargePerSliceLabelStr);
    chargePerSliceLabel->setBuddy(chargePerSliceSpin);

    chargePrecisionSpin = new QSpinBox;
    chargePrecisionLabel = new QLabel(chargePrecisionLabelStr);
    chargePrecisionLabel->setBuddy(chargePrecisionSpin);

    /* create the management buttons. */
    saveButton = new QPushButton(saveButtonStr);
    closeButton = new QPushButton(closeButtonStr);

    /* add the buttons in a button box dialog. */
    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(saveButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::AcceptRole);

    /* set up some ranges for spinboxes. */
    parkingCapacitySpin->setRange(MIN_PARKING_CAPACITY, MAX_PARKING_CAPACITY);
    parkingCapacitySpin->setValue(sets.parkingCapacity);

    chargeTimesliceSpin->setRange(MIN_TIMESLICE, MAX_TIMESLICE);
    chargeTimesliceSpin->setValue(sets.timeslice);
    chargeTimesliceSpin->setSuffix(" sec(s)");

    chargePerSliceSpin->setRange(MIN_CHARGE_PER_TIMESLICE, MAX_CHARGE_PER_TIMESLICE);
    chargePerSliceSpin->setSingleStep(DEF_CHARGE_SPINBOX_STEP);
    chargePerSliceSpin->setValue(sets.chargePerTimeslice);

    chargePrecisionSpin->setRange(MIN_CHARGE_PRECISION, MAX_CHARGE_PRECISION);
    chargePrecisionSpin->setValue(sets.chargePrecision);

    /* focus the close button. */
    closeButton->setFocus();

    /* set the signals/slots for the buttons' events. */
    connect(saveButton, SIGNAL(clicked()), this, SLOT(writeSettings()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));

    /* create a table grid. */
    QGridLayout *mainLayout = new QGridLayout;

    /* add the following objects in the appropriate position in the grid. */
    mainLayout->addWidget(parkingCapacityLabel, 0, 0);
    mainLayout->addWidget(parkingCapacitySpin, 1, 0);
    mainLayout->addWidget(chargeTimesliceLabel, 2, 0);
    mainLayout->addWidget(chargeTimesliceSpin, 3, 0);
    mainLayout->addWidget(chargePerSliceLabel, 4, 0);
    mainLayout->addWidget(chargePerSliceSpin, 5, 0);
    mainLayout->addWidget(chargePrecisionLabel, 6, 0);
    mainLayout->addWidget(chargePrecisionSpin, 7, 0);
    mainLayout->addWidget(buttonBox, 8, 0);

    /* set some GUI options for the table grid. */
    mainLayout->setRowMinimumHeight(8, 35);
    mainLayout->setRowStretch(8, 1);
    mainLayout->setColumnStretch(2, 1);

    /* do not allow to resize the form. */
    mainLayout->setSizeConstraint (QLayout::SetFixedSize);

    /* set the layout for the settings form. */
    setLayout(mainLayout);

    /* set the text of the form's window. */
    setWindowTitle(setsWinTitleStr);
}

/* close the form. */
void
SettingsForm::done(const int result) {
    /* return from the form. */
    QDialog::done(result);
}

/* write the settings of the program. */
void SettingsForm::writeSettings() {
    QSettings s(setsAppOrg, setsAppName);

    s.beginGroup("parking");
    s.setValue("capacity", parkingCapacitySpin->value());
    s.endGroup();

    s.beginGroup("payment");
    s.setValue("timeslice", chargeTimesliceSpin->value());
    s.setValue("charge_per_timeslice", chargePerSliceSpin->value());
    s.setValue("charge_precision", chargePrecisionSpin->value());
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
