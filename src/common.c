/*
 * SHA-1 type definition
 *
 * This type is a pass-by-reference type with no varlena overhead
 * to store SHA-1 digests.
 *
 * Portions Copyright (c) 2001-2008, PostgreSQL Global Development Group
 */
#include "postgres.h"

#include "common.h"
#include "fmgr.h"

PG_MODULE_MAGIC;

#ifndef SET_VARSIZE
#define SET_VARSIZE(PTR, len) (VARATT_SIZEP(PTR) = len)
#endif

static const int8 hexlookup[128] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

static char
get_hex(char c) {
    int res = -1;

    if (c > 0 && c < 127)
        res = hexlookup[(unsigned char) c];

    if (res < 0)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                        errmsg("invalid hexadecimal digit: \"%c\"", c)));

    return (char) res;
}

/*
 * non-SQL-callable function to take a cstring and return a parsed array of hex
 * bytes
 */
unsigned char *
cstring_to_hexarr(const char *arg, Size length, const char *hashname) {
    const char *a,
            *argend;
    char v1,
            v2;
    unsigned char *output;
    unsigned char *p;
    int filled;

    output = palloc0(length);

    argend = arg + strlen(arg);
    a = arg;
    p = output;
    filled = 0;
    while (a < argend) {
        if (*a == ' ' || *a == '\n' || *a == '\t' || *a == '\r') {
            a++;
            continue;
        }
        v1 = get_hex(*a++) << 4;
        if (a >= argend)
            ereport(ERROR,
                    (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                            errmsg("invalid %s data: odd number of digits", hashname)));

        v2 = get_hex(*a++);
        filled++;
        if (filled > length)
            ereport(ERROR,
                    (errmsg("invalid %s data: too many digits (expected %lu)",
                            hashname, length * 2)));
        *p++ = (unsigned char) (v1 | v2);
    }

    if (filled != length)
        ereport(ERROR,
                (errmsg("invalid %s data: not enough digits (got %d, expected %lu)", hashname,
                        filled * 2, length * 2)));

    return output;
}

unsigned char *
text_to_hexarr(const text *arg, Size length, const char *hashname) {
    const Size txtsz = VARSIZE(arg) - VARHDRSZ;
    const char *in = VARDATA(arg); //Not null-terminated!
    unsigned char *output = palloc0(length);

    const char *inend = in + txtsz;
    char v1,
            v2;
    unsigned char *p;
    int filled;

    p = output;
    filled = 0;
    while (in < inend) {
        if (*in == ' ' || *in == '\n' || *in == '\t' || *in == '\r') {
            in++;
            continue;
        }
        v1 = get_hex(*in++) << 4;
        if (in >= inend)
            ereport(ERROR,
                    (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                            errmsg("invalid %s data: odd number of digits", hashname)));

        v2 = get_hex(*in++);
        filled++;
        if (filled > length)
            ereport(ERROR,
                    (errmsg("invalid %s data: too many digits (expected %lu)",
                            hashname, length * 2)));
        *p++ = (unsigned char) (v1 | v2);
    }

    if (filled != length)
        ereport(ERROR,
                (errmsg("invalid %s data: not enough digits (got %d, expected %lu)", hashname,
                        filled * 2, length * 2)));

    return output;
}

/* non-SQL-callable function for converting any digest value into a C-String */
char *
hexarr_to_cstring(const unsigned char *value, Size length) {
    char *output;
    Size outlen = length * 2 + 1;
    int c;
    int offset;

    output = palloc(outlen + 1);

    c = 0;
    offset = 0;
    /* we know all lengths are multiples of 4 */
    for (;;) {
        snprintf(output + offset, 9, "%02x%02x%02x%02x",
                 value[c + 0], value[c + 1], value[c + 2], value[c + 3]);
        c += 4;
        if (c >= length)
            break;
        offset += 8;
    }

    return output;
}

/* Non-SQL-callable comparison function */
int
hexarr_cmp_int(const unsigned char *a, const unsigned char *b, Size length) {
    int i;

    for (i = 0; i < length; i++) {
        if (a[i] > b[i])
            return 1;
        if (a[i] < b[i])
            return -1;
    }

    return 0;
}

bytea *
hexarr_to_bytea(const unsigned char *value, Size length) {
    bytea *output;

    output = palloc(length + VARHDRSZ);
    memcpy(VARDATA(output), value, length);
    SET_VARSIZE(output, length + VARHDRSZ);
    return output;
}
