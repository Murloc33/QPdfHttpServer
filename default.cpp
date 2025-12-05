#include "default.h"

QString numberToWords(double num)
{
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

QString getPluralForm(int n, const QString &one, const QString &two, const QString &five)
{
    int mod10 = n % 10;
    int mod100 = n % 100;

    if (mod100 >= 11 && mod100 <= 19) return five;
    if (mod10 == 1) return one;
    if (mod10 >= 2 && mod10 <= 4) return two;
    return five;
}

QString convertLessThanOneThousand(int n)
{
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
