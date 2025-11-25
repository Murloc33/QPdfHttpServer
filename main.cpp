#include <QApplication>
#include <QDebug>
#include <QHttpServer>

#include "invoiceserver.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    InvoiceServer server;

    qDebug() << "Готово!";

    return app.exec();
}
