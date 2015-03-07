/* header defining the interface of the source. */
#ifndef PAYWIZARD_H
#define PAYWIZARD_H

/* include some QT libraries. */
#include <QWizard>

/* use these classes. */
class QRadioButton;
class QLineEdit;
class QLabel;

/* GUI string messages. */
static const QString wizardWinTitleStr   = QObject::tr("Payment Transaction");
static const QString payWayTitleStr      = QObject::tr("Payment Way");
static const QString inputCardTitleStr   = QObject::tr("Credit Card Payment Way");
static const QString inputCashTitleStr   = QObject::tr("Cash Payment Way");
static const QString completeTitleStr    = QObject::tr("Charge Completed");

static const QString custChangeLabelStr  = QObject::tr("Customer's change : ");
static const QString payChargeLabelStr   = QObject::tr("Money you will be charged : ");

static const QString payWayCashLabelStr  = QObject::tr("&Cash");
static const QString payWayCardLabelStr  = QObject::tr("&Credit Card");
static const QString inputCashLabelStr   = QObject::tr("&Input Money :");
static const QString inputCardLabelStr   = QObject::tr("&Credit Card :");

static const QString notManyMoneyStr     = QObject::tr("Please input enough money for the charge.");
static const QString cardNotChargedStr   = QObject::tr("Unfortunately, credit card cannot be charged.");
static const QString cardChargedLabelStr = QObject::tr("The credit card has been charged.");
static const QString payWayTopLabelStr   = QObject::tr("Please select a payment way in order to continue.");

static const QString payReceiptHtmlStr   = QObject::tr(
        "<h2>%1 %2</h2>"
        "<p>Receipt for transaction payment.</p>"
        "<ul><li>Customer Name : <b>%3</b></li>"
        "<li>Date & Time : <b>%4</b></li>"
        "<li>Payment Charge : <b>%5</b></li></ul>");

/* class which implements the transaction payment wizard (as non-linear state machine). */
class PayWizard : public QWizard
{
    Q_OBJECT

    public:
        /* payment wizard states' indexes enumeration data type. */
        typedef enum payWizardState {
            Page_SelectPayWay = 0,
            Page_InsertCash,
            Page_InsertCard,
            Page_Complete
        } payWizardState;

        PayWizard(const QString custName, const double charge, QWidget *parent = 0);

    protected:
        void closeEvent(QCloseEvent *event);
};

/* class which implements the payment way selection wizard page. */
class SelectPayWayPage : public QWizardPage
{
    Q_OBJECT

    public:
        SelectPayWayPage(const QString custName, const double charge, QWidget *parent = 0);
        int nextId() const;

    private:
        QRadioButton *cashRadioButton;
        QRadioButton *cardRadioButton;
        QLabel *topLabel;
};

/* class which implements the cash insertion/manipulation wizard page. */
class InsertCashPage : public QWizardPage
{
    Q_OBJECT

    public:
        InsertCashPage(QWidget *parent = 0);
        void initializePage();
        bool validatePage();
        int nextId() const;

    private:
        QLabel *moneyLabel;
        QLineEdit *moneyEdit;
        QLabel *topLabel;
};

/* class which implements the card insertion/manipulation wizard page. */
class InsertCardPage : public QWizardPage
{
    Q_OBJECT

    public:
        InsertCardPage(QWidget *parent = 0);
        void initializePage();
        bool validatePage();
        int nextId() const;

    private:
        QLabel *cardLabel;
        QLineEdit *cardEdit;
        QLabel *topLabel;
};

/* class which implements the payment completion/receipt wizard page. */
class CompletePage : public QWizardPage
{
    Q_OBJECT

    public:
        CompletePage(QWidget *parent = 0);
        void setVisible(const bool visible);
        void initializePage();
        int nextId() const;

    private slots:
        void printButtonClicked();

    private:
        QLabel *topLabel;
};

#endif // PAYWIZARD_H
