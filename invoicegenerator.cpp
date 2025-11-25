#include "invoicegenerator.h"

#include <QTextDocument>
#include <QPrinter>
#include <QPainter>
#include <QPdfWriter>
#include <QDate>
#include <QBuffer>
#include <QFile>

#include <QTextDocument>

QString InvoiceGenerator::numberToWords(double num) {
    int integerPart = static_cast<int>(num);

    if (integerPart == 0) return "ноль";

    auto convert = [&](int n) -> QString {
        if (n == 0) return "ноль";


        int billions = n / 1000000000;
        int millions = (n % 1000000000) / 1000000;
        int thousands = (n % 1000000) / 1000;
        int remainder = n % 1000;

        QString result;

        if (billions > 0) {
            result += convertLessThanOneThousand(billions) + " миллиард" + getPluralForm(billions, "а", "ов", "ов") + " ";
        }

        if (millions > 0) {
            result += convertLessThanOneThousand(millions) + " миллион" + getPluralForm(millions, "", "а", "ов") + " ";
        }

        if (thousands > 0) {
            QString thousandWord = convertLessThanOneThousand(thousands);
            thousandWord = thousandWord.replace("один", "одна").replace("два", "две");
            result += thousandWord + " тысяч" + getPluralForm(thousands, "а", "и", "") + " ";
        }

        if (remainder > 0) {
            result += convertLessThanOneThousand(remainder);
        }

        return result.trimmed();
    };

    return convert(integerPart);
}

QString InvoiceGenerator::getPluralForm(int n, const QString &one, const QString &two, const QString &five) {
    int mod10 = n % 10;
    int mod100 = n % 100;

    if (mod100 >= 11 && mod100 <= 19) return five;
    if (mod10 == 1) return one;
    if (mod10 >= 2 && mod10 <= 4) return two;
    return five;
}

QString InvoiceGenerator::convertLessThanOneThousand(int n) {
    if (n == 0) return "";

    QStringList ones = {"", "один", "два", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять"};
    QStringList teens = {"десять", "одиннадцать", "двенадцать", "тринадцать", "четырнадцать",
                         "пятнадцать", "шестнадцать", "семнадцать", "восемнадцать", "девятнадцать"};
    QStringList tens = {"", "", "двадцать", "тридцать", "сорок", "пятьдесят",
                        "шестьдесят", "семьдесят", "восемьдесят", "девяносто"};
    QStringList hundreds = {"", "сто", "двести", "триста", "четыреста", "пятьсот",
                            "шестьсот", "семьсот", "восемьсот", "девятьсот"};

    if (n < 10) return ones[n];
    if (n < 20) return teens[n - 10];
    if (n < 100) {
        return tens[n / 10] + (n % 10 != 0 ? " " + ones[n % 10] : "");
    }
    return hundreds[n / 100] + (n % 100 != 0 ? " " + convertLessThanOneThousand(n % 100) : "");
}

QByteArray InvoiceGenerator::generateInvoicePDF(const InvoiceOptions &options) {
    QByteArray pdfData;
    QBuffer buffer(&pdfData);

    QString html;

    html += "<style>"
            "body { font-family: 'Times New Roman', serif; font-size: 12pt; margin: 0; padding: 0; line-height: 1.1; }"
            "h3 { font-size: 10pt; font-weight: bold; margin: 3px 0; }"
            "table { border-collapse: collapse; width: 100%; margin: 3px 0; }"
            "th { font-weight: bold; padding: 2px; border: 1px solid #000; text-align: center; font-size: 9pt; background-color: #f0f0f0; }"
            "td { padding: 2px; border: 1px solid #000; font-size: 9pt; }"
            ".bank-table { border: 1px solid #000; font-size: 9pt; width: 100%; }"
            ".bank-table td { border: 1px solid #000; padding: 1px 3px; }"
            ".no-border-table { border: none; }"
            ".no-border-table td { border: none; padding: 1px 3px; vertical-align: top; }"
            ".right-align { text-align: right; }"
            ".center-align { text-align: center; }"
            ".left-align { text-align: left; }"
            ".bold { font-weight: bold; }"
            ".small-text { font-size: 9pt; }"
            ".very-small-text { font-size: 8pt; }"
            ".warning-block { background-color: #fffacd; padding: 5px; margin: 5px 0; border: 1px solid #ffd700; font-size: 9pt; }"
            ".signature-block { margin-top: 30px; }"
            ".payment-purpose { text-align: center; font-weight: bold; margin: 5px 0; font-size: 10pt; }"
            ".invoice-title-line { border-bottom: 1px solid #000; margin: 0 0 8px 0; }"
            ".dotted-line { border-bottom: 1px dotted #000; margin-top: 15px; }"
            ".result-table { border: none; }"
            "</style>";

    html += "<h3 style='text-align: center;'>ВНИМАНИЕ, ИЗМЕНИЛИСЬ РЕКВИЗИТЫ</h3>"
            "<h3 style='text-align: center;'>Образец заполнения платежного поручения</h3>";

    html += "<table class='bank-table'>";
    html += "<tr>"
            "<td rowspan='2' colspan='2' width='55%'>" + options.bank.bank_name + "<br><div style='font-size: 8pt;'>Банк получателя</div></td>"
                                       "<td width='10%'>БИК</td>"
                                       "<td rowspan='2' colspan='3' width='35%'>" + options.bank.BIC + "<br>" + options.bank.correspondent_account +"</td>"
                                                                               "</tr>";

    html += "<tr>"
            "<td>Сч. №</td>"
            "</tr>";

    html += "<tr>"
            "<td>ИНН " + options.bank.recipient_INN + "</td>"
                                           "<td>КПП " + options.bank.recipient_KPP + "</td>"
                                           "<td width='10%'>Сч. №</td>"
                                           "<td width='45%' colspan='3'>" + options.bank.account_number + "</td>"
                                            "</tr>";

    html += "<tr>"
            "<td rowspan='3' colspan='2'>" + options.bank.recipient + "<br><br><div style='font-size: 8pt;'>Получатель</div></td>"
                                       "<td>Вид оп.</td>"
                                       "<td rowspan='3'>" + options.payment.type + "<br>" + options.payment.purpose + "<br>" + options.payment.code + "</td>"
                                                                                                        "<td>Срок плат.</td>"
                                                                                                        "<td rowspan='3'><br>" + options.payment.term + "<br>" + options.payment.queue + "<br>" + options.payment.reserve_field + "</td>"
                                                                                                               "</tr>";

    html += "<tr>"
            "<td>Наз. пл.</td>"
            "<td>Очер. плат.</td>"
            "</tr>";

    html += "<tr>"
            "<td>Код</td>"
            "<td>Рез. поле</td>"
            "</tr>";

    html += "<tr><td colspan='6'>" + options.payment_purpose + "<br><div style='font-size: 8pt;'>Назначение платежа</div></td></tr>";

    html += "</table>";

    html += "<br><br><br>";
    html += "<div style='font-size: 12pt; font-weight: bold;'>Счет на оплату № " + options.invoice_number + " от " + options.invoice_date + "</div>"
                                                                                                                                            "<hr>";

    html += "<table class='no-border-table'>";
    html += "<tr>"
            "<td width='80'>Поставщик:</td>"
            "<td>" + options.supplier.name + ", ИНН " + options.supplier.INN + ", КПП " + options.supplier.KPP + ", " + options.supplier.address + "</td>"
                                                                                                                                            "</tr>";
    html += "<tr>"
            "<td>Покупатель:</td>"
            "<td>" + options.customer.name + ", ИНН " + options.customer.INN + ", КПП " + options.customer.KPP + ", " + options.customer.address + "</td>"
                                                                                                                                            "</tr>";
    html += "</table>";

    html += "<br><br>";

    html += "<div>Менеджер: " + options.manager_name + ", " + options.manager_phone + ", " + options.manager_email + "</div>";

    html += "<table style='margin: 8px 0;'>";
    html += "<tr>"
            "<th width='5%'>№</th>"
            "<th width='50%'>Товары (работы, услуги)</th>"
            "<th width='10%' colspan='2'>Колличество</th>"
            "<th width='10%'>Цена</th>"
            "<th width='10%'>Сумма</th>"
            "<th width='15%'>Комментарий</th>"
            "</tr>";

    for (int i = 0; i < options.items.size(); ++i) {
        const InvoiceItem &item = options.items[i];
        html += "<tr>"
                "<td class='center-align'><font size='2'>" + QString::number(i + 1) + "</font></td>"
                                           "<td class='center-align'><font size='2'>" + item.name + "</font></td>"
                              "<td class='center-align'><font size='2'>" + QString::number(item.quantity, 'f', 0) + "</font></td>"
                                                           "<td class='center-align'><font size='2'>" + item.unit + "</font></td>"
                              "<td class='right-align'><font size='2'>" + QString::number(item.price, 'f', 2).replace('.', ',') + "</font></td>"
                                                                          "<td class='right-align'><font size='2'>" + QString::number(item.total, 'f', 2).replace('.', ',') + "</font></td>"
                                                                          "<td class='center-align'></td>"
                                                                          "</tr>";
    }
    html += "</table>";

    html += "<table class='result-table'>"
            "<tr>"
            "<td width='65%' style='font-weight: bold;'></td>"
            "<td width='20%' style='font-weight: bold; text-align: right;'>Итого:</td>"
            "<td width='15%' style='font-weight: bold; text-align: right;'>" + QString::number(options.total_coast, 'f', 2) + "</td>"
                                                             "</tr>"
                                                             "<tr>"
                                                             "<td width='65%' style='text-align: left;'>Общий вес: " + QString::number(options.total_weight, 'f', 3) + " кг.</td>"
                                                              "<td width='20%' style='font-weight: bold; text-align: right;'>В т.ч. НДС (20%):</td>"
                                                              "<td width='15%' style='font-weight: bold; text-align: right;'>" + QString::number(options.VAT, 'f', 2) + "</td>"
                                                     "</tr>"
                                                     "<tr>"
                                                     "<td width='70%' style='font-weight: bold;'></td>"
                                                     "<td width='20%' style='font-weight: bold; text-align: right;'>Итого с НДС:</td>"
                                                     "<td width='15%' style='font-weight: bold; text-align: right;'>" + QString::number(options.total_coast, 'f', 2) + "</td>"
                                                             "</tr>"
                                                             "<tr>"
                                                             "<td width='65%' style='text-align: left;'>Всего наименований " + QString::number(options.items.size()) + ", на сумму "
            + QString::number(options.total_amount, 'f', 2).replace('.', ',')
            + " руб</td>"
              "<td width='35%' style='text-align: right; font-size: 7pt;' colspan='2'>Если не проходите по цене даём доп. скидку*</td>"
              "</tr>"

              "</table>";

    int kopecks = static_cast<int>((options.total_amount - static_cast<int>(options.total_amount)) * 100);

    QString amountInWords = numberToWords(options.total_amount);
    if (!amountInWords.isEmpty()) {
        amountInWords[0] = amountInWords[0].toUpper();
    }
    html += "<div><strong>" + amountInWords + " рублей " + QString("%1").arg(kopecks, 2, 10, QChar('0')) + " копеек</strong></div>";

    html += "<div>"
            "<p style='text-align: center; font-size: 10pt;'>Счёт действителен в течении " + options.valid_period +
            "<br>ТОВАР РЕЗЕРВИРУЕТСЯ ПОСЛЕ УВЕДОМЛЕНИЯ ПОКУПАТЕЛЕМ ОБ ОПЛАТЕ"
            "<br><strong><font size='2'>Уважаемые Покупатели! Напоминаем Вам, что согласно Закона Российской Федерации \"О защите прав потребителей\", "
            "товар, произведенный и привезенный под заказ, а также отпускаемый на метраж, обмену и возврату не подлежит!</font></strong></p>"
            "</div>";

    html += "<hr>";

    html += "<table class='no-border-table signature-block'>";
    html += "<tr>"
            "<td width='45%'>"
            "<div class='bold'>Руководитель:</div>"
            "<div class='dotted-line'></div>"
            "<div>Коваленко К. Ф.</div>"
            "<div class='very-small-text'>(расшифровка подписи)</div>"
            "</td>"
            "<td width='10%'></td>"
            "<td width='45%'>"
            "<div class='bold'>Бухгалтер:</div>"
            "<div class='dotted-line'></div>"
            "<div>Коваленко К. Ф.</div>"
            "<div class='very-small-text'>(расшифровка подписи)</div>"
            "</td>"
            "</tr>";
    html += "</table>";

    html += "<p class='center-align very-small-text' style='margin-top: 15px;'>"
            "*При предоставлении конкурентного счета с более низкой ценой, рассматриваем дополнительную скидку"
            "</p>";

    QTextDocument document;
    document.setHtml(html);
    buffer.open(QIODeviceBase::WriteOnly);
    QPdfWriter pdfWriter(&buffer);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setTitle("Счет на оплату № " + options.invoice_number);
    pdfWriter.setCreator("Invoice PDF Generator");

    document.print(&pdfWriter);
    buffer.close();

    return buffer.data();
}
