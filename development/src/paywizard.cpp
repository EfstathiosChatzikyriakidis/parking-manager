/*
 *  This file implements the payment non-linear wizard state machine.
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

/* include headers defining the interface of the sources. */
#include "paywizard.h"
#include "arithmetictools.h"
#include "globaldeclarations.h"
#include "bankingtools.h"

/* create the transaction payment wizard (as non-linear state machine). */
PayWizard::PayWizard(const QString custName, const double charge, QWidget *parent) : QWizard(parent) {
    /* set the pages/states of the wizard/machine. */
    setPage(Page_SelectPayWay, new SelectPayWayPage (custName, charge));
    setPage(Page_InsertCash, new InsertCashPage);
    setPage(Page_InsertCard, new InsertCardPage);
    setPage(Page_Complete, new CompletePage);

    /* set the initial page/state. */
    setStartId(Page_SelectPayWay);

    /* set the title of the wizard. */
    setWindowTitle(wizardWinTitleStr);
}

/* handle and ignore the close event of the wizard. */
void
PayWizard::closeEvent(QCloseEvent *event) {
    event->ignore();
}

/* create the payment way selection wizard page. */
SelectPayWayPage::SelectPayWayPage(const QString custName, const double charge, QWidget *parent) : QWizardPage(parent) {
    /* set the title of the page. */
    setTitle(payWayTitleStr);

    /* set the watermark of the page. */
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/graphics/paywizard/watermark"));

    /* set the top label of the page. */
    topLabel = new QLabel(payWayTopLabelStr);

    /* create the payment way radio buttons. */
    cashRadioButton = new QRadioButton(payWayCashLabelStr);
    cardRadioButton = new QRadioButton(payWayCardLabelStr);

    /* check the cash radio button. */
    cashRadioButton->setChecked(true);

    /* register fields containing customer name and payment charge. */
    registerField("customer.name", new QLineEdit(custName));
    registerField("payment.charge", new QLineEdit(QString("%1").arg(charge)));

    /* create the layout of the page. */
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(cashRadioButton);
    layout->addWidget(cardRadioButton);

    /* set the layout of the page. */
    setLayout(layout);
}

/* set the next page for the next button. */
int
SelectPayWayPage::nextId() const {
    if (cashRadioButton->isChecked()) {
        return PayWizard::Page_InsertCash;
    } else {
        return PayWizard::Page_InsertCard;
    }
}

/* create the cash insertion/manipulation wizard page. */
InsertCashPage::InsertCashPage(QWidget *parent) : QWizardPage(parent) {
    /* set the title of the page. */
    setTitle(inputCashTitleStr);

    /* set the watermark of the page. */
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/graphics/paywizard/watermark"));

    /* create the top label of the page. */
    topLabel = new QLabel;

    /* payment money label, edit and buddy. */
    moneyLabel = new QLabel(inputCashLabelStr);
    moneyEdit = new QLineEdit;
    moneyLabel->setBuddy(moneyEdit);

    /* accept only numbers in the money edit. */
    moneyEdit->setValidator(new QRegExpValidator(QRegExp(numberRegExpStr), this));

    /* register a mandatory field for the money edit. */
    registerField("insertcash.money*", moneyEdit);

    /* create the layout of the page. */
    QHBoxLayout *layoutH = new QHBoxLayout;
    layoutH->addWidget(moneyLabel);
    layoutH->addWidget(moneyEdit);
    layoutH->addStretch();
    QVBoxLayout *layoutV = new QVBoxLayout;
    layoutV->addWidget(topLabel);
    layoutV->addLayout(layoutH);

    /* set the layout of the page. */
    setLayout(layoutV);
}

/* initialization code for the page. */
void
InsertCashPage::initializePage() {
    topLabel->setText(payChargeLabelStr + field("payment.charge").toString());
}

/* logic code for page validation. */
bool
InsertCashPage::validatePage() {
    /* check if the customer's money is less than the charge. */
    if(isLessThan(moneyEdit->text().toDouble(), field("payment.charge").toDouble())) {
        /* show a message. */
        QMessageBox::warning(this, infoMsgTitleStr, notManyMoneyStr);

        /* stay in the same page. */
        return false;
    }

    /* move to the next. */
    return true;
}

/* set the next page for the next button. */
int
InsertCashPage::nextId() const {
    return PayWizard::Page_Complete;
}

/* create the card insertion/manipulation wizard page. */
InsertCardPage::InsertCardPage(QWidget *parent) : QWizardPage(parent) {
    /* set the title of the page. */
    setTitle(inputCardTitleStr);

    /* set the watermark of the page. */
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/graphics/paywizard/watermark"));

    /* create the top label of the page. */
    topLabel = new QLabel;

    /* card number label, edit and buddy. */
    cardLabel = new QLabel(inputCardLabelStr);
    cardEdit = new QLineEdit;
    cardLabel->setBuddy(cardEdit);

    /* set a mask for the credit card number. */
    cardEdit->setInputMask(creditCardNumberMaskStr);

    /* register a mandatory field for the card number. */
    registerField("insertcard.number*", cardEdit);

    /* create the layout of the page. */
    QHBoxLayout *layoutH = new QHBoxLayout;
    layoutH->addWidget(cardLabel);
    layoutH->addWidget(cardEdit);
    layoutH->addStretch();
    QVBoxLayout *layoutV = new QVBoxLayout;
    layoutV->addWidget(topLabel);
    layoutV->addLayout(layoutH);

    /* set the layout of the page. */
    setLayout(layoutV);
}

/* initialization code for the page. */
void
InsertCardPage::initializePage() {
    topLabel->setText(payChargeLabelStr + field("payment.charge").toString());
}

/* logic code for page validation. */
bool
InsertCardPage::validatePage() {
    /* check if the credit card has been charged. */
    if(chargeCreditCard(cardEdit->text(), field("payment.charge").toDouble())) {
        /* move to the next page. */
        return true;
    }

    /* show a message. */
    QMessageBox::warning(this, infoMsgTitleStr, cardNotChargedStr);

    /* stay in the same page. */
    return false;
}

/* set the next page for the next button. */
int
InsertCardPage::nextId() const {
    return PayWizard::Page_Complete;
}

/* create the payment completion/receipt wizard page. */
CompletePage::CompletePage(QWidget *parent) : QWizardPage(parent) {
    /* set the title of the page. */
    setTitle(completeTitleStr);

    /* set the watermark of the page. */
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/graphics/paywizard/watermark"));

    /* create the top label of the page. */
    topLabel = new QLabel;

    /* create the layout of the page. */
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);

    /* set the layout of the page. */
    setLayout(layout);
}

/* initialization code for the page. */
void
CompletePage::initializePage() {
    /* the reply message of the payment. */
    QString message;

    /* if the wizard has visited the cash insertion page. */
    if (wizard()->hasVisitedPage(PayWizard::Page_InsertCash)) {
        /* calculate the change of the customer from the payment. */
        double change = field("insertcash.money").toDouble() - field("payment.charge").toDouble();

        /* inform the customer for his/her payment change. */
        message = custChangeLabelStr + QString("%1").arg(change);

        /* disable, hide the cancel, back buttons. */
        wizard()->setOption(QWizard::DisabledBackButtonOnLastPage, true);
        wizard()->setOption(QWizard::NoCancelButton, true);
    } else {
        /* inform the customer for the credit card charge. */
        message = cardChargedLabelStr;

        /* disable, hide the cancel, back buttons. */
        wizard()->setOption(QWizard::DisabledBackButtonOnLastPage, true);
        wizard()->setOption(QWizard::NoCancelButton, true);
    }

    /* set the appropriate value to the top label. */
    topLabel->setText(message);
}

/* set the next page for the next button. */
int
CompletePage::nextId() const {
    /* the end of the state machine. */
    return -1;
}

/* show/hide the print receipt button. */
void
CompletePage::setVisible(bool visible) {
    QWizardPage::setVisible(visible);

    /* show/hide the print button and connect/disconnect the signal/slot entry. */
    if (visible) {
        wizard()->setButtonText(QWizard::CustomButton1, printButtonStr);
        wizard()->setOption(QWizard::HaveCustomButton1, true);
        connect(wizard(), SIGNAL(customButtonClicked(const int)), this, SLOT(printButtonClicked()));
    } else {
        wizard()->setOption(QWizard::HaveCustomButton1, false);
        disconnect(wizard(), SIGNAL(customButtonClicked(int)), this, SLOT(printButtonClicked()));
    }
}

/* print a simple receipt for the payment transaction. */
void
CompletePage::printButtonClicked() {
    /* create a printer object. */
    QPrinter printer;

    /* create a print dialog and assign it with the printer. */
    QPrintDialog dialog(&printer, this);

    /* open the dialog and handle printing properties. */
    if (dialog.exec()) {
        /* create a html text document. */
        QTextDocument textDocument;

        /* create html for the text document. */
        QString html = payReceiptHtmlStr.arg(appName,
                                             appVersion,
                                             field("customer.name").toString(),
                                             QDateTime::currentDateTime().toString(dateTimeFormatStr),
                                             field("payment.charge").toString());

        /* print the document .*/
        textDocument.setHtml(html);
        textDocument.print(&printer);
    }
}
