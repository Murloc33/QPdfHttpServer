#ifndef INVOICEGENERATOR_H
#define INVOICEGENERATOR_H

#include "default.h"

class InvoiceGenerator {
public:
    static QByteArray generateConsignmentPDF(const InvoiceOptions &options);
};

#endif // INVOICEGENERATOR_H
