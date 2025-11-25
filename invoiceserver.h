#ifndef INVOICESERVER_H
#define INVOICESERVER_H

#include <QCoreApplication>
#include <QHttpServer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QBuffer>
#include <QByteArray>
#include <QJsonArray>
#include <QDebug>
#include <QTcpServer>
#include <QDateTime>

class InvoiceServer {
public:
    InvoiceServer();

private:
    void setupRoutes();

    QHttpServerResponse handleGenerateInvoice(const QHttpServerRequest &request);

    QHttpServerResponse createErrorResponse(const QString& message);

private:
    QHttpServer server;
};

#endif // INVOICESERVER_H
