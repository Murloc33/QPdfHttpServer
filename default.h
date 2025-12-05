#ifndef DEFAULT_H
#define DEFAULT_H

#include <QString>
#include <QList>

struct Company {
    QString name;
    QString INN;
    QString KPP;
    QString address;
};

struct BankDetails {
    QString bank_name;
    QString BIC;
    QString account_number;
    QString correspondent_account;
    QString recipient;
    QString recipient_INN;
    QString recipient_KPP;
};

struct PaymentDetails {
    QString type;
    QString purpose;
    QString code;
    QString term;
    QString queue;
    QString reserve_field;
};

struct InvoiceItem {
    QString name;
    double quantity = 0;
    QString unit = "шт.";
    double price = 0;
    double total = 0;
};

struct InvoiceOptions {
    QString invoice_number;
    QString invoice_date;
    QString payment_purpose;

    Company supplier;
    Company customer;
    BankDetails bank;
    PaymentDetails payment;

    QString manager_name;
    QString manager_phone;
    QString manager_email;

    QList<InvoiceItem> items;
    QString file_name;

    double total_weight = 0;
    double total_amount = 0;
    double total_coast = 0;
    double VAT = 0;

    QString valid_period;
};

struct ConsignmentParty {
    QString name;
    QString inn;
    QString kpp;
    QString address;
};

struct ConsignmentItem {
    QString name;
    QString unit;
    double quantity;
    double price;
    double grossWeight;
    double netWeight;
    double vatRate;
};

struct ConsignmentOptions {
    QString invoiceNumber;
    QString invoiceDate;
    ConsignmentParty shipper;
    ConsignmentParty consignee;
    ConsignmentParty supplier;
    QString basis;
    QVector<ConsignmentItem> items;
};

QString numberToWords(double num);
QString getPluralForm(int n, const QString &one, const QString &two, const QString &five);
QString convertLessThanOneThousand(int n);

#endif // DEFAULT_H
