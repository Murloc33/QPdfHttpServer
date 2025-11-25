#include "invoicejsonparser.h"

#include <QJsonArray>

InvoiceOptions InvoiceJsonParser::fromJson(const QJsonObject& json) {
    InvoiceOptions options;
    try {
        if (json.contains("invoice") && json["invoice"].isObject()) {
            QJsonObject invoiceObj = json["invoice"].toObject();
            options.invoice_number = invoiceObj["invoice_number"].toString();
            options.invoice_date = invoiceObj["invoice_date"].toString();
            options.payment_purpose = invoiceObj["payment_purpose"].toString();
            options.valid_period = invoiceObj["valid_period"].toString();
            options.file_name = invoiceObj["file_name"].toString();
        }

        if (json.contains("companies") && json["companies"].isObject()) {
            QJsonObject companiesObj = json["companies"].toObject();
            if (companiesObj.contains("supplier")) {
                options.supplier = parseCompany(companiesObj["supplier"].toObject());
            }
            if (companiesObj.contains("customer")) {
                options.customer = parseCompany(companiesObj["customer"].toObject());
            }
        }

        if (json.contains("bank_details") && json["bank_details"].isObject()) {
            options.bank = parseBankDetails(json["bank_details"].toObject());
        }

        if (json.contains("payment_details") && json["payment_details"].isObject()) {
            options.payment = parsePaymentDetails(json["payment_details"].toObject());
        }

        if (json.contains("manager") && json["manager"].isObject()) {
            QJsonObject managerObj = json["manager"].toObject();
            options.manager_name = managerObj["manager_name"].toString();
            options.manager_phone = managerObj["manager_phone"].toString();
            options.manager_email = managerObj["manager_email"].toString();
        }

        if (json.contains("items") && json["items"].isArray()) {
            options.items = parseItems(json["items"].toArray());
        }

        if (json.contains("totals") && json["totals"].isObject()) {
            QJsonObject totalsObj = json["totals"].toObject();
            options.total_weight = totalsObj["total_weight"].toDouble(0);
            options.total_amount = totalsObj["total_amount"].toDouble(0);
            options.total_coast = totalsObj["total_coast"].toDouble(0);
            options.VAT = totalsObj["VAT"].toDouble(0);
        }

    } catch (const std::exception& e) {
        qWarning() << "Error parsing JSON:" << e.what();
    }

    return options;
}

InvoiceOptions InvoiceJsonParser::fromJsonString(const QString& jsonString) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Invalid JSON string";
        return InvoiceOptions();
    }
    return fromJson(doc.object());
}

QJsonObject InvoiceJsonParser::toJson(const InvoiceOptions& options) {
    QJsonObject json;

    QJsonObject invoiceObj;
    invoiceObj["invoice_number"] = options.invoice_number;
    invoiceObj["invoice_date"] = options.invoice_date;
    invoiceObj["payment_purpose"] = options.payment_purpose;
    invoiceObj["valid_period"] = options.valid_period;
    invoiceObj["file_name"] = options.file_name;
    json["invoice"] = invoiceObj;

    QJsonObject companiesObj;
    companiesObj["supplier"] = companyToJson(options.supplier);
    companiesObj["customer"] = companyToJson(options.customer);
    json["companies"] = companiesObj;

    json["bank_details"] = bankDetailsToJson(options.bank);

    json["payment_details"] = paymentDetailsToJson(options.payment);

    QJsonObject managerObj;
    managerObj["manager_name"] = options.manager_name;
    managerObj["manager_phone"] = options.manager_phone;
    managerObj["manager_email"] = options.manager_email;
    json["manager"] = managerObj;

    json["items"] = itemsToJson(options.items);

    QJsonObject totalsObj;
    totalsObj["total_weight"] = options.total_weight;
    totalsObj["total_amount"] = options.total_amount;
    totalsObj["total_coast"] = options.total_coast;
    totalsObj["VAT"] = options.VAT;
    json["totals"] = totalsObj;

    return json;
}

QString InvoiceJsonParser::toJsonString(const InvoiceOptions& options) {
    QJsonDocument doc(toJson(options));
    return doc.toJson(QJsonDocument::Indented);
}

bool InvoiceJsonParser::validateJson(const QJsonObject& json) {
    if (!json.contains("invoice") || !json["invoice"].isObject()) return false;
    if (!json.contains("companies") || !json["companies"].isObject()) return false;
    if (!json.contains("bank_details") || !json["bank_details"].isObject()) return false;

    QJsonObject invoiceObj = json["invoice"].toObject();
    if (!invoiceObj.contains("invoice_number") || !invoiceObj.contains("invoice_date")) {
        return false;
    }

    return true;
}

Company InvoiceJsonParser::parseCompany(const QJsonObject& json) {
    Company company;
    company.name = json["name"].toString();
    company.INN = json["INN"].toString();
    company.KPP = json["KPP"].toString();
    company.address = json["address"].toString();
    return company;
}

BankDetails InvoiceJsonParser::parseBankDetails(const QJsonObject& json) {
    BankDetails bank;
    bank.bank_name = json["bank_name"].toString();
    bank.BIC = json["BIC"].toString();
    bank.account_number = json["account_number"].toString();
    bank.correspondent_account = json["correspondent_account"].toString();
    bank.recipient = json["recipient"].toString();
    bank.recipient_INN = json["recipient_INN"].toString();
    bank.recipient_KPP = json["recipient_KPP"].toString();
    return bank;
}

PaymentDetails InvoiceJsonParser::parsePaymentDetails(const QJsonObject& json) {
    PaymentDetails payment;
    payment.type = json["type"].toString("01");
    payment.purpose = json["purpose"].toString();
    payment.code = json["code"].toString();
    payment.term = json["term"].toString();
    payment.queue = json["queue"].toString();
    payment.reserve_field = json["reserve_field"].toString();
    return payment;
}

InvoiceItem InvoiceJsonParser::parseInvoiceItem(const QJsonObject& json) {
    InvoiceItem item;
    item.name = json["name"].toString();
    item.quantity = json["quantity"].toDouble(0);
    item.unit = json["unit"].toString("шт.");
    item.price = json["price"].toDouble(0);
    item.total = json["total"].toDouble(0);
    return item;
}

QList<InvoiceItem> InvoiceJsonParser::parseItems(const QJsonArray& jsonArray) {
    QList<InvoiceItem> items;
    for (const QJsonValue& value : jsonArray) {
        if (value.isObject()) {
            items.append(parseInvoiceItem(value.toObject()));
        }
    }
    return items;
}

// Вспомогательные методы сериализации
QJsonObject InvoiceJsonParser::companyToJson(const Company& company) {
    QJsonObject json;
    json["name"] = company.name;
    json["INN"] = company.INN;
    json["KPP"] = company.KPP;
    json["address"] = company.address;
    return json;
}

QJsonObject InvoiceJsonParser::bankDetailsToJson(const BankDetails& bank) {
    QJsonObject json;
    json["bank_name"] = bank.bank_name;
    json["BIC"] = bank.BIC;
    json["account_number"] = bank.account_number;
    json["correspondent_account"] = bank.correspondent_account;
    json["recipient"] = bank.recipient;
    json["recipient_INN"] = bank.recipient_INN;
    json["recipient_KPP"] = bank.recipient_KPP;
    return json;
}

QJsonObject InvoiceJsonParser::paymentDetailsToJson(const PaymentDetails& payment) {
    QJsonObject json;
    json["type"] = payment.type;
    json["purpose"] = payment.purpose;
    json["code"] = payment.code;
    json["term"] = payment.term;
    json["queue"] = payment.queue;
    json["reserve_field"] = payment.reserve_field;
    return json;
}

QJsonObject InvoiceJsonParser::invoiceItemToJson(const InvoiceItem& item) {
    QJsonObject json;
    json["name"] = item.name;
    json["quantity"] = item.quantity;
    json["unit"] = item.unit;
    json["price"] = item.price;
    json["total"] = item.total;
    return json;
}

QJsonArray InvoiceJsonParser::itemsToJson(const QList<InvoiceItem>& items) {
    QJsonArray array;
    for (const InvoiceItem& item : items) {
        array.append(invoiceItemToJson(item));
    }
    return array;
}
