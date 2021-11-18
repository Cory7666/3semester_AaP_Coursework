#include "checker.h"

int check_wchar (const wint_t ch, const checker_mode_t mode)
{
    if ((mode & VMASK_LOWER_CHARS) && iswlower(ch)) return VMASK_LOWER_CHARS;
    if ((mode & VMASK_UPPER_CHARS) && iswupper(ch)) return VMASK_UPPER_CHARS;
    if ((mode & VMASK_DIGITS)      && iswdigit(ch)) return VMASK_DIGITS;
    if ((mode & VMASK_SPACES)      && (ch == L' ')) return VMASK_SPACES;
    if ((mode & VMASK_PUNCTS)      && iswpunct(ch)) return VMASK_PUNCTS;

    return 0;
}