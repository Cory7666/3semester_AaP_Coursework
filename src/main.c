#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include "date.h"
#include "lists.h"
#include "files.h"

void print (list_elem_t * e, wchar_t * text)
{
    setlocale(LC_ALL, "");

    wchar_t date_wcs[1024];
    date2wcs(&e->data.date, date_wcs);
    swprintf(text, 2048, L"%lu%c%lu%c%ls%c%ls%c%ls%c%ls%c%.3Lf%c%.2Lf%c%ls",
            0, CSV_DELIMITER,
            e->data.cage_num, CSV_DELIMITER,
            e->data.animal.name, CSV_DELIMITER,
            e->data.animal.breed, CSV_DELIMITER,
            e->data.animal.areal, CSV_DELIMITER,
            e->data.products.type, CSV_DELIMITER,
            e->data.products.weight, CSV_DELIMITER,
            e->data.products.cost, CSV_DELIMITER,
            date_wcs
        );
}

int main ()
{
    wchar_t text[2048];

    list_obj_t * l = lists_CreateNewListObject();

    read_from_csv(l, L"file.csv");

    lists_SortListByField(l, SORT_DESCENDING_ORDER, LIST_ANIMAL_NAME);

    lists_DeleteListObject(&l);
    return 0;
}