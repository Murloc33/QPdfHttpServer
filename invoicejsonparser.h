#ifndef INVOICEJSONPARSER_H
#define INVOICEJSONPARSER_H

#include <QJsonObject>

#include "default.h"

class InvoiceJsonParser {

public:
    static InvoiceOptions fromJson(const QJsonObject& json);
    static InvoiceOptions fromJsonString(const QString& jsonString);

    static QJsonObject toJson(const InvoiceOptions& options);
    static QString toJsonString(const InvoiceOptions& options);

    static bool validateJson(const QJsonObject& json);

private:
    static Company parseCompany(const QJsonObject& json);
    static BankDetails parseBankDetails(const QJsonObject& json);
    static PaymentDetails parsePaymentDetails(const QJsonObject& json);
    static InvoiceItem parseInvoiceItem(const QJsonObject& json);
    static QList<InvoiceItem> parseItems(const QJsonArray& jsonArray);

    static QJsonObject companyToJson(const Company& company);
    static QJsonObject bankDetailsToJson(const BankDetails& bank);
    static QJsonObject paymentDetailsToJson(const PaymentDetails& payment);
    static QJsonObject invoiceItemToJson(const InvoiceItem& item);
    static QJsonArray itemsToJson(const QList<InvoiceItem>& items);
};

#endif // INVOICEJSONPARSER_H
