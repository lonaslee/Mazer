#ifndef DESTROY_H
#define DESTROY_H

#define DESTROY(type, fn)               \
    do {                                \
        va_list vargs;                  \
        va_start(vargs, num);           \
        for (int i = 0; i < num; i++) { \
            fn(va_arg(vargs, type *));  \
        }                               \
        va_end(vargs);                  \
        return;                         \
    } while (0);

/**
 * @brief Destroy a number of resources of the same type
 *
 * @param type type of the resource being destroyed, is the string full name of the resource
 * @param num  how many items are being passed
 * @param ...  va args of resource pointers to be destroyed
 */
void destroy_resources(char *type, int num, ...);

#endif /* DESTROY_H */