#define panic(format, ...) panic_("%s (%s:%zu): " format ".\n", __FILE__, __func__, __LINE__, ## __VA_ARGS__)

static int panic_(const char * format, ...) {
    va_list vararg;
    va_start(vararg, format);
    vfprintf(stderr, format, vararg);
    va_end(vararg);
    return 1;
}
