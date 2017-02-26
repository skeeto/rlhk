#ifndef RLHK_RAND_H
#define RLHK_RAND_H

#ifndef RLHK_RAND_API
#  define RLHK_RAND_API
#endif

/**
 * Fill a buffer with true entropy from the operating system.
 *
 * This shouldn't be used directly to generate numbers, but rather to
 * seed a PRNG.
 *
 * Returns 1 on success, 0 on failure (leaving buf untouched).
 */
RLHK_RAND_API
int rlhk_rand_entropy(void *buf, unsigned len);

/**
 * Generate a uniform 32-bit integer from a 32-bit state.
 */
RLHK_RAND_API
unsigned long rlhk_rand_32(unsigned long *);

/* Implementation */
#ifdef RLHK_RAND_IMPLEMENTATION
#include <math.h>

/* System entropy. */
#if (defined(__unix__) || defined(__APPLE__)) && !defined(__DJGPP__)
#include <fcntl.h>
#include <unistd.h>

RLHK_RAND_API
int
rlhk_rand_entropy(void *buf, unsigned len)
{
    int fd = open("/dev/urandom", O_RDONLY);
    int success = 0;
    if (fd != -1) {
        success = read(fd, buf, len) == len;
        close(fd);
    }
    return success;
}

#elif defined(_WIN32)
#include <windows.h>

RLHK_RAND_API
int
rlhk_rand_entropy(void *buf, unsigned len)
{
    HCRYPTPROV h = 0;
    DWORD type = PROV_RSA_FULL;
    DWORD flags = CRYPT_VERIFYCONTEXT | CRYPT_SILENT;
    int success =
        CryptAcquireContext(&h, 0, 0, type, flags) &&
        CryptGenRandom(h, len, buf);
    if (h)
        CryptReleaseContext(h, 0);
    return success;
}
#elif defined(__MSDOS__)
#include <time.h>

RLHK_RAND_API
int
rlhk_rand_entropy(void *buf, unsigned len)
{
    /* Best option is the clock. */
    unsigned long t = time(0);
    char *e = (char *)buf + len;
    char *p = buf;
    while (p != e) {
        unsigned long v = rlhk_rand_32(&t);
        int z = e - p > 4 ? 4 : e - p;
        memcpy(p, &v, z);
        p += z;
    }
    return 1;
}

#endif /* __MSDOS__ */

RLHK_RAND_API
unsigned long
rlhk_rand_32(unsigned long *state)
{
    unsigned long x = *state;
    x ^= x << 13;
    x ^= (x & 0xffffffffUL) >> 17;
    x ^= x << 5;
    return (*state = x) & 0xffffffffUL;
}

RLHK_RAND_API
double
rlhk_rand_uniform(unsigned long *s)
{
    return rlhk_rand_32(s) / (double)0xffffffffUL;
}

RLHK_RAND_API
void
rlhk_rand_norm(unsigned long *s, double *n0, double *n1)
{
    double x0, x1, w;
    do {
        x0 = 2 * rlhk_rand_uniform(s) - 1;
        x1 = 2 * rlhk_rand_uniform(s) - 1;
        w = x0 * x0 + x1 * x1;
    } while (w >= 1);
    w = sqrt((-2.0 * log(w)) / w);
    *n0 = x0 * w;
    *n1 = x1 * w;
}

#endif /* RLHK_RAND_IMPLEMENTATION */
#endif /* RLHK_RAND_H */
