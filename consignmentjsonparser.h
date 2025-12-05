#ifndef CONSIGNMENTJSONPARSER_H
#define CONSIGNMENTJSONPARSER_H

#include "default.h"

#include <QJsonObject>

class ConsignmentJsonParser
{
public:
    ConsignmentJsonParser();

    static ConsignmentParty parseCompanyInfoFromJson(const QJsonObject &json);
    static ConsignmentItem parseItemFromJson(const QJsonObject &json);
    static ConsignmentOptions fromJson(const QJsonObject &json);
};

#endif // CONSIGNMENTJSONPARSER_H
