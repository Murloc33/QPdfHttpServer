#include "server.h"

#include "invoicejsonparser.h"
#include "invoicegenerator.h"

#include "consignmentgenerator.h"
#include "consignmentjsonparser.h"

InvoiceServer::InvoiceServer() {
    setupRoutes();

    auto tcpserver = new QTcpServer();
    if (!tcpserver->listen(QHostAddress::Any, 8080) || !server.bind(tcpserver)) {
        delete tcpserver;
    }
}

void InvoiceServer::setupRoutes() {
    server.route("/health", QHttpServerRequest::Method::Post,
                 []() {
                     QJsonObject response;
                     response["status"] = "ok";
                     response["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
                     return QHttpServerResponse(response);
                 });

    server.route("/generate-invoice", QHttpServerRequest::Method::Post,
                 [this](const QHttpServerRequest &request) {
                     return handleGenerateInvoice(request);
                 });

    server.route("/", QHttpServerRequest::Method::Get,
                 []() {
                     QJsonObject response;
                     response["service"] = "Invoice PDF Generator";
                     response["version"] = "1.0";
                     response["endpoints"] = QJsonArray::fromStringList({
                         "POST /generate-invoice",
                         "GET /health"
                     });
                     return QHttpServerResponse(response);
                 });

    server.route("/generate-consignment", QHttpServerRequest::Method::Post,
                 [this](const QHttpServerRequest &request) {
                     return handleGenerateConsignment(request);
                 });
}

QHttpServerResponse InvoiceServer::handleGenerateInvoice(const QHttpServerRequest &request) {
    try {

        QJsonDocument doc = QJsonDocument::fromJson(request.body());
        if (doc.isNull() || !doc.isObject()) {
            return createErrorResponse("Неверный JSON формат");
        }

        QJsonObject jsonData = doc.object();

        InvoiceOptions options = InvoiceJsonParser::fromJson(jsonData);

        QByteArray pdfData = InvoiceGenerator::generateConsignmentPDF(options);

        QHttpServerResponse response(pdfData);

        qInfo() << "Успешно сгенерирован счет №" << options.invoice_number;

        return response;

    } catch (const std::exception& e) {
        qCritical() << "Ошибка при генерации счета:" << e.what();
        return createErrorResponse(QString("Внутренняя ошибка сервера: %1").arg(e.what()));
    }
}

QHttpServerResponse InvoiceServer::handleGenerateConsignment(const QHttpServerRequest &request)
{
    try {

        QJsonDocument doc = QJsonDocument::fromJson(request.body());
        if (doc.isNull() || !doc.isObject()) {
            return createErrorResponse("Неверный JSON формат");
        }

        QJsonObject jsonData = doc.object();

        ConsignmentOptions options = ConsignmentJsonParser::fromJson(jsonData);

        QByteArray pdfData = ConsignmentGenerator::generateConsignmentPDF(options);

        QHttpServerResponse response(pdfData);

        return response;

    } catch (const std::exception& e) {
        qCritical() << "Ошибка при генерации счета:" << e.what();
        return createErrorResponse(QString("Внутренняя ошибка сервера: %1").arg(e.what()));
    }
}

QHttpServerResponse InvoiceServer::createErrorResponse(const QString &message) {
    QJsonObject error;
    error["error"] = true;
    error["message"] = message;
    error["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QHttpServerResponse response(error);

    return response;
}
