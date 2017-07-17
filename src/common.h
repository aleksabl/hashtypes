
extern unsigned char *cstring_to_hexarr(const char *arg, int length, const char *hashname);
extern unsigned char *text_to_hexarr(text *arg, int length, const char *hashname);
extern char *hexarr_to_cstring(const unsigned char *value, int length);
extern int hexarr_cmp_int(const unsigned char *a, const unsigned char *b, int length);
extern bytea *hexarr_to_bytea(const unsigned char *value, int length);
