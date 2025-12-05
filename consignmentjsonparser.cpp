#include "consignmentjsonparser.h"

#include <QJsonObject>
#include <QJsonArray>

ConsignmentJsonParser::ConsignmentJsonParser() {}

ConsignmentParty ConsignmentJsonParser::parseCompanyInfoFromJson(const QJsonObject &json)
{
    ConsignmentParty company;

    company.name = json.value("name").toString("");
    company.inn = json.value("inn").toString("");
    company.kpp = json.value("kpp").toString("");
    company.address = json.value("address").toString("");

    return company;
}

ConsignmentItem ConsignmentJsonParser::parseItemFromJson(const QJsonObject &json)
{
    ConsignmentItem item;

    item.name = json.value("name").toString("");
    item.unit = json.value("unit").toString("");
    item.quantity = json.value("quantity").toDouble(0.0);
    item.price = json.value("price").toDouble(0.0);
    item.grossWeight = json.value("grossWeight").toDouble(0.0);
    item.netWeight = json.value("netWeight").toDouble(0.0);
    item.vatRate = json.value("vatRate").toDouble(0.0);

    // Если не заданы явно, вычисляем по умолчанию
    if (json.value("grossWeight").isUndefined()) {
        item.grossWeight = item.netWeight;
    }

    if (json.value("vatRate").isUndefined()) {
        item.vatRate = 20.0; // Стандартная ставка НДС
    }

    return item;
}

ConsignmentOptions ConsignmentJsonParser::fromJson(const QJsonObject &json)
{
    ConsignmentOptions options;

    // Основные поля
    options.invoiceNumber = json.value("invoice_number").toString("");
    options.invoiceDate = json.value("invoice_date").toString("");
    options.basis = json.value("basis").toString("");

    // Парсинг компаний
    if (json.contains("shipper") && json["shipper"].isObject()) {
        options.shipper = parseCompanyInfoFromJson(json["shipper"].toObject());
    }

    if (json.contains("consignee") && json["consignee"].isObject()) {
        options.consignee = parseCompanyInfoFromJson(json["consignee"].toObject());
    }

    if (json.contains("supplier") && json["supplier"].isObject()) {
        options.supplier = parseCompanyInfoFromJson(json["supplier"].toObject());
    }

    // Парсинг товаров
    if (json.contains("items") && json["items"].isArray()) {
        QJsonArray itemsArray = json["items"].toArray();
        options.items.clear();

        for (const QJsonValue &itemValue : itemsArray) {
            if (itemValue.isObject()) {
                options.items.append(parseItemFromJson(itemValue.toObject()));
            }
        }
    }

    return options;
}
