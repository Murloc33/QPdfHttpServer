#ifndef CONSIGNMENTGENERATOR_H
#define CONSIGNMENTGENERATOR_H

#include <QByteArray>

#include <QString>
#include <QVector>

#include "invoicegenerator.h".h"

class ConsignmentGenerator
{
public:
    ConsignmentGenerator();

     static QByteArray generateConsignmentPDF(const ConsignmentOptions &opt);
};

#endif // CONSIGNMENTGENERATOR_H
