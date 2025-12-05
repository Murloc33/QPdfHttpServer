#include <QApplication>
#include <QDebug>
#include <QHttpServer>

#include "server.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    InvoiceServer server;

    qDebug() << "Готово!";

    return app.exec();
}
