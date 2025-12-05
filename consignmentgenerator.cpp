#include "consignmentgenerator.h"

#include <QBuffer>
#include <QTextDocument>
#include <QPdfWriter>

ConsignmentGenerator::ConsignmentGenerator() {}

QByteArray ConsignmentGenerator::generateConsignmentPDF(const ConsignmentOptions &options)
{
    QByteArray pdfData;
    QBuffer buffer(&pdfData);

    QString html;

    html += "<style>"
            "body { font-family: 'Arial', sans-serif; font-size: 9pt; margin: 0; padding: 0; line-height: 1.1; }"
            ".header { text-align: center; font-size: 8pt; margin-bottom: 15px; }"
            ".title { font-size: 10pt; font-weight: bold; text-align: center; margin: 10px 0; }"
            ".form-info { margin: 10px 0; }"
            ".company-info { margin: 15px 0; line-height: 1.2; }"
            ".section { margin: 20px 0; }"
            ".table { border-collapse: collapse; width: 100%; margin: 10px 0; }"
            ".table th { border: 1px solid #000; padding: 3px; text-align: center; font-weight: bold; font-size: 8pt; background-color: #f0f0f0; }"
            ".table td { border: 1px solid #000; padding: 3px; font-size: 8pt; }"
            ".no-border { border: none !important; }"
            ".signature-table { width: 100%; margin-top: 30px; }"
            ".signature-cell { vertical-align: top; padding: 10px 20px; }"
            ".dotted-line { border-bottom: 1px dotted #000; margin-top: 40px; }"
            ".center { text-align: center; }"
            ".right { text-align: right; }"
            ".left { text-align: left; }"
            ".bold { font-weight: bold; }"
            ".small { font-size: 8pt; }"
            ".very-small { font-size: 7pt; }"
            ".stamp { margin-top: 50px; text-align: center; }"
            ".page-break { page-break-before: always; }"
            "</style>";

    html += "<div class='header'>Создано при помощи controlata.ru</div>";

    html += "<div class='title'>Унифицированная форма ТОРГ-12, утверждена постановлением Госкомитета России от 25.12.98 № 132</div>";

    html += "<div class='form-info'>"
            "<table width='100%'>"
            "<tr>"
            "<td width='50%'>Форма по ОКУД: 0330212</td>"
            "<td width='50%' class='right'>Код</td>"
            "</tr>"
            "<tr>"
            "<td>по ОКПО:</td>"
            "<td></td>"
            "</tr>"
            "</table>"
            "</div>";

    html += "<div class='company-info'>";
    html += "<div><strong>Грузоотправитель:</strong> " + options.shipper.name + ", ИНН " + options.shipper.inn + ", КПП " + options.shipper.kpp + ", " + options.shipper.address + "</div>";
    html += "<div class='small'>Вид деятельности по ОКДП:</div>";
    html += "<div class='small'>по ОКПО:</div>";
    html += "</div>";

    html += "<div class='company-info'>";
    html += "<div><strong>Грузополучатель:</strong> " + options.consignee.name + ", ИНН " + options.consignee.inn + ", КПП " + options.consignee.kpp + ", " + options.consignee.address + "</div>";
    html += "</div>";

    html += "<div class='company-info'>";
    html += "<div><strong>Поставщик:</strong> " + options.supplier.name + ", ИНН " + options.supplier.inn + ", КПП " + options.supplier.kpp + ", " + options.supplier.address + "</div>";
    html += "</div>";

    html += "<div class='company-info'>";
    html += "<div><strong>Основание:</strong> " + options.basis + "</div>";
    html += "</div>";

    html += "<div class='section'>";
    html += "<table class='table'>"
            "<tr>"
            "<th>Номер документа</th>"
            "<th>Дата составления</th>"
            "<th>Транспортная накладная</th>"
            "<th>Вид операции</th>"
            "</tr>"
            "<tr>"
            "<td class='center bold'>ТОВАРНАЯ НАКЛАДНАЯ</td>"
            "<td class='center'>" + options.invoiceNumber + "</td>"
                                      "<td class='center'>" + options.invoiceDate + "</td>"
                                    "<td class='center'></td>"
                                    "</tr>"
                                    "</table>";
    html += "</div>";

    html += "<div class='section'>";
    html += "<table class='table'>"
            "<tr>"
            "<th rowspan='2'>№</th>"
            "<th rowspan='2' colspan='3'>Товар</th>"
            "<th rowspan='2'>Единица измерения</th>"
            "<th rowspan='2'>Вид упаковки</th>"
            "<th colspan='2'>Количество</th>"
            "<th rowspan='2'>Масса брутто</th>"
            "<th rowspan='2'>Количество (масса нетто)</th>"
            "<th rowspan='2'>Цена, руб. коп.</th>"
            "<th rowspan='2'>Сумма без учета НДС, руб. коп.</th>"
            "<th rowspan='2'>НДС ставка, %</th>"
            "<th rowspan='2'>Сумма с учетом НДС, руб. коп.</th>"
            "</tr>"
            "<tr>"
            "<th>в одном месте</th>"
            "<th>мест, штук</th>"
            "</tr>";

    double totalAmount = 0.0;
    for (int i = 0; i < options.items.size(); ++i) {
        const auto &item = options.items[i];
        double itemTotal = item.quantity * item.price;
        totalAmount += itemTotal;

        html += "<tr>"
                "<td class='center'>" + QString::number(i + 1) + "</td>"
                                           "<td colspan='3'>" + item.name + "</td>"
                              "<td class='center'>" + item.unit + "</td>"
                              "<td class='center'></td>"
                              "<td class='center'></td>"
                              "<td class='center'>" + QString::number(item.quantity) + "</td>"
                                                   "<td class='center'>" + (item.grossWeight > 0 ? QString::number(item.grossWeight) : "") + "</td>"
                                                                                    "<td class='center'>" + (item.netWeight > 0 ? QString::number(item.netWeight) : "") + "</td>"
                                                                                "<td class='right'>" + QString::number(item.price, 'f', 2).replace('.', ',') + "</td>"
                                                                          "<td class='right'>" + QString::number(itemTotal, 'f', 2).replace('.', ',') + "</td>"
                                                                         "<td class='center'>" + QString::number(item.vatRate) + "</td>"
                                                  "<td class='right'>" + QString::number(itemTotal * (1 + item.vatRate / 100.0), 'f', 2).replace('.', ',') + "</td>"
                                                                                                      "</tr>";
    }

    for (int i = options.items.size(); i < 10; ++i) {
        html += "<tr>"
                "<td class='center'>" + QString::number(i + 1) + "</td>"
                                           "<td colspan='3'></td>"
                                           "<td class='center'></td>"
                                           "<td class='center'></td>"
                                           "<td class='center'></td>"
                                           "<td class='center'></td>"
                                           "<td class='center'></td>"
                                           "<td class='center'></td>"
                                           "<td class='center'></td>"
                                           "<td class='center'></td>"
                                           "<td class='center'></td>"
                                           "<td class='center'></td>"
                                           "</tr>";
    }

    html += "<tr>"
            "<td colspan='10' class='right bold'>Итого:</td>"
            "<td class='right'>" + QString::number(totalAmount, 'f', 2).replace('.', ',') + "</td>"
                                                                       "<td></td>"
                                                                       "<td class='right'>" + QString::number(totalAmount, 'f', 2).replace('.', ',') + "</td>"
                                                                       "</tr>";

    html += "</table>";
    html += "</div>";

    html += "<div class='section'>";
    html += "<table width='100%' class='no-border'>"
            "<tr>"
            "<td>Tоварная накладная имеет приложение на ______ листах и содержит ______ порядковых номеров записей</td>"
            "</tr>"
            "<tr>"
            "<td>Всего мест: ______ Масса груза (нетто) ______ Масса груза (брутто) ______</td>"
            "</tr>"
            "<tr>"
            "<td>Приложение (паспорта, сертификаты и т.п.) на ______ листах</td>"
            "</tr>"
            "</table>";
    html += "</div>";

    html += "<div class='bold'>Всего отпущено на сумму " + numberToWords(totalAmount) + " рублей 00 копеек</div>";

    html += "<div class='section'>";
    html += "<table class='signature-table'>"
            "<tr>"
            "<td width='33%' class='signature-cell'>"
            "<div>Отпуск разрешил:</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(должность)</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(подпись)</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(расшифровка подписи)</div>"
            "</td>"
            "<td width='33%' class='signature-cell'>"
            "<div>Главный (старший) бухгалтер:</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(подпись)</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(расшифровка подписи)</div>"
            "</td>"
            "<td width='33%' class='signature-cell'>"
            "<div>Груз принял:</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(должность)</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(подпись)</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(расшифровка подписи)</div>"
            "</td>"
            "</tr>"
            "</table>";
    html += "</div>";

    html += "<div class='page-break'></div>";

    html += "<div class='section'>";
    html += "<table class='signature-table'>"
            "<tr>"
            "<td width='50%' class='signature-cell'>"
            "<div>Отпуск произвел:</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(должность)</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(подпись)</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(расшифровка подписи)</div>"
            "</td>"
            "<td width='50%' class='signature-cell'>"
            "<div>Груз получил:</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(должность)</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(подпись)</div>"
            "<div class='dotted-line'></div>"
            "<div class='very-small center'>(расшифровка подписи)</div>"
            "</td>"
            "</tr>"
            "</table>";
    html += "</div>";


    html += "<div class='stamp'>";
    html += "<div class='dotted-line' style='width: 150px; margin: 0 auto;'></div>"
            "<div class='very-small'>М.П. (место печати)</div>";
    html += "</div>";

    html += "<div class='stamp'>";
    html += "<div class='dotted-line' style='width: 150px; margin: 0 auto;'></div>"
            "<div class='very-small'>М.П. (место печати)</div>";
    html += "</div>";

    QTextDocument document;
    document.setHtml(html);

    buffer.open(QIODevice::WriteOnly);
    QPdfWriter pdfWriter(&buffer);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setTitle("Товарная накладная" + options.invoiceNumber);

    document.print(&pdfWriter);
    buffer.close();

    return pdfData;
}

