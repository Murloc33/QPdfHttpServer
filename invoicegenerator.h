#ifndef INVOICEGENERATOR_H
#define INVOICEGENERATOR_H

#include "default.h"

class InvoiceGenerator {
public:
    static QByteArray generateInvoicePDF(const InvoiceOptions &options);

private:
    static QString numberToWords(double num);
    static QString getPluralForm(int n, const QString &one, const QString &two, const QString &five);
    static QString convertLessThanOneThousand(int n);
};

#endif // INVOICEGENERATOR_H
