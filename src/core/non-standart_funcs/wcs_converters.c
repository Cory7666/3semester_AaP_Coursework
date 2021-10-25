#include "wcs_conv.h"

long long wcs2int (const wchar_t * string)
{
    /* Немедленно выйти из подпрограммы, если строка не была передана */
    if (string == NULL) return 0;

    size_t offset = 0;  // Отступ от начала строки
    int    sign   = 1;  // Окончательный знак числа
    long long n   = 0;  // Полученное число

    /* Пропустить пробелы перед числом */
    while (iswspace(string[offset])) offset++;

    /* Определиться со знаком числа */
    switch (string[offset])
    {
        case '-': sign = -1;
        case '+': offset++;  break;
    }

    /* Преобразовать оставшёюся часть строки в число */
    while (iswdigit(string[offset]))
        n = n * 10 + (string[offset++] - L'0');
    
    return (n * sign);
}

long double wcs2float (const wchar_t * string)
{
    /* Немедленно выйти из подпрограммы, если не была передана строка */
    if (string == NULL) return 0;

    size_t offset = 0;  // Отступ от начала строки
    int    sign   = 1;  // Итоговый знак числа
    long double n = 0;  // Полученное число

    /* Пропуск пробелов перед числом */
    while (iswspace(string[offset])) offset++;

    /* Определиться со знаком числа */
    switch (string[offset])
    {
        case '-': sign = -1;
        case '+': offset++;  break;
    }

    /* Преобразовать целую часть числа */
    for (; iswdigit(string[offset]); offset++)
        n = n * 10 + ((long double) (string[offset] - L'0'));
    
    /* Встречено не-число */
    switch (string[offset])
    {
        // Продолжить выполнение, если встреченное не-число является
        //   разделителем
        case '.': case ',':
            offset++;
            break;
        // Иначе вернуть полученную целую часть числа
        default:
            return (n * sign);
            break;
    }
    
    int d = 1;

    /* Преобразовать дробную часть числа */
    while (iswdigit(string[offset]))
        n += ((long double) (string[offset++] - '0')) / (d *= 10);
    
    return (n * sign);
}

void int2wcs (const long long number, wchar_t * string)
{
    swprintf(string, 20, L"%lld", number);
}

void float2wcs (const long double number, wchar_t * string)
{
    swprintf(string, 20, L"%Lf", number);
}
