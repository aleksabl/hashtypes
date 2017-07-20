
extern unsigned char *cstring_to_hexarr(const char *arg, Size length, const char *hashname);

extern unsigned char *text_to_hexarr(const text *arg, Size length, const char *hashname);

extern char *hexarr_to_cstring(const unsigned char *value, Size length);

extern int hexarr_cmp_int(const unsigned char *a, const unsigned char *b, Size length);

extern bytea *hexarr_to_bytea(const unsigned char *value, Size length);
