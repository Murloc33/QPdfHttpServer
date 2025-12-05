#ifndef SERVER_H
#define SERVER_H

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
    QHttpServerResponse handleGenerateConsignment(const QHttpServerRequest &request);

    QHttpServerResponse createErrorResponse(const QString& message);

private:
    QHttpServer server;
};

#endif // SERVER_H
