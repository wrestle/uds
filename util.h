#ifndef UTIL_UDATASTRUCT
#define UTIL_UDATASTRUCT
#include <stdint.h>
#include <assert.h>

#ifdef typeof
#undef typeof
#define typeof(type) __typeof__(type)
#endif

#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define likely(x)    __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)

static inline int atomic32_cmpset(volatile uint32_t *dst, uint32_t exp, uint32_t src) {
    uint8_t ret;
    __asm__ __volatile__(
                "LOCK ;"
                "cmpxchgl %[src] %[dst]"
                "sete %[ret]"
                : [ret] "=a" (ret), [dst] "=m" (*dst)
                : [src] "r" (src), "a" (exp), "m" (*dst)
                : "memory");
    return ret;
}

#endif // UTIL

