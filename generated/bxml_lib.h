#define BXML_ARRAY(T) \
    struct            \
    {                 \
        T *items;     \
        size_t count; \
    }