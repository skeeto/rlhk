/* Roguelike Header Kit : Algorithms
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Provides a portable set of general roguelike algorithms, largely
 * for 2D grid-based maps.
 *
 * For all functions involving gradients or directions, the 8 possible
 * directions are represented using the integers 0 through 7. Counting
 * up, the directions are clockwise with "0" at twelve o'clock (north)
 * and "4" at six o'clock (south). Use the RLHK_ALGO_DX() and
 * RLHK_ALGO_DY() to decode the integers 0 through 7 into direction
 * deltas for each axis (-1, 0, +1).
 *
 * Map coordinates are always expected to fit in 16-bit integers
 * Negative coordinates are allowed. Path lengths are always expected
 * to fit in 32-bit integers. To save on memory, you may not actually
 * want to use "long" (possibly 64 bits) in your map representation
 * and instead try to specifically use a 32-bit integer.
 *
 * Functions:
 *   - rlhk_algo_shortest
 *   - rlhk_algo_dijkstra
 */
#ifndef RLHK_ALGO_H
#define RLHK_ALGO_H

#ifndef RLHK_ALGO_API
#  ifdef RLHK_API
#    define RLHK_ALGO_API RLHK_API
#  else
#    define RLHK_ALGO_API
#  endif
#endif

#define RLHK_ALGO_DX(i) ((int)((0x0489a621UL >> (4 * (i) + 0)) & 3) - 1)
#define RLHK_ALGO_DY(i) ((int)((0x0489a621UL >> (4 * (i) + 2)) & 3) - 1)

enum rlhk_algo_map_method {
    /**
     * Asks if the tile at (x, y) is passable coming from the
     * direction indicated via "data". Return non-zero for true, 0 for
     * false.
     *
     * To get strict 4-directional NSEW movement, return 0 for any
     * diagonal "data" direction. For bishop movement, return 0 for
     * any NSEW movement.
     */
    RLHK_ALGO_MAP_GET_PASSABLE,

    /**
     * Set every tile's 32-bit distance value to -1. The return value
     * is ignored.
     */
    RLHK_ALGO_MAP_CLEAR_DISTANCE,

    /**
     * Set a 32-bit distance value for (x, y). The return value is
     * ignored.
     */
    RLHK_ALGO_MAP_SET_DISTANCE,

    /**
     * Return the previously-set 32-bit distance value at (x, y). The
     * "data" parameter is unused.
     */
    RLHK_ALGO_MAP_GET_DISTANCE,

    /**
     * Set a 32-bit heuristic score for (x, y). The return value is
     * ignored.
     */
    RLHK_ALGO_MAP_SET_HEURISTIC,

    /**
     * Return the previously-set 32-bit heuristic value at (x, y). The
     * "data" parameter is unused.
     */
    RLHK_ALGO_MAP_GET_HEURISTIC,

    /**
     * Set an unsigned 3-bit directional value (0-7) at (x, y). A
     * special direction of "-1" is sometimes used to indicate a lack
     * of direction / gradient. You are not expected to return this -1
     * later and may either discard/ignore it or use the information
     * as you wish.
     *
     * The return value is ignored.
     */
    RLHK_ALGO_MAP_SET_GRADIENT,

    /**
     * Indicates that a tile lies along a shortest path. You *must*
     * return the previous RLHK_ALGO_MAP_SET_GRADIENT data value,
     * unless that value was -1 in which case the return value is
     * ignored. The "data" value is the 32-bit distance from the goal
     * (x1, y1).
     */
    RLHK_ALGO_MAP_MARK_SHORTEST,

    /**
     * Indicates that the tile at (x, y) is visible. The return value
     * must indicate if this tile is transparent: non-zero for true, 0
     * for false.
     *
     * The "data" parameter is unused.
     */
    RLHK_ALGO_MAP_MARK_VISIBLE
};

/**
 * Generic map interface provided to RLHK.
 *
 * It is *your* job to implement this function. You must implement
 * each of the methods above that may be used the map functions you
 * call. Your map representation will provide some of the per-tile
 * storage needed by the functions below.
 *
 * You must also typedef "rlhk_algo_map" to your own map type
 * (typically as a pointer type) before including this header.
 */
RLHK_ALGO_API
long rlhk_algo_map_call(rlhk_algo_map map,
                        enum rlhk_algo_map_method method,
                        int x, int y, long data);

/**
 * Find a shortest route between two points using A*.
 *
 * Ultimately the RLHK_ALGO_MAP_SET_RESULT method will be called for
 * each tile on the shortest route, starting from (x1, y1) and ending
 * at (x0, y1).
 *
 * You must provide some workspace memory (buf) and its size in bytes
 * (buflen) to be used as a priority queue. The memory need not be
 * initialized. A buflen of "sizeof(short) * numtiles * 2" will always
 * be sufficient (one pair of shorts per map tile).
 *
 * To perform a small search with early bailout, just provide a small
 * work buffer and allow the function to (safely) run out of memory.
 *
 * If successful, it returns the length of the path. On failure it
 * returns -1 if no path could be found or -2 if it ran out of
 * workspace before finding a solution.
 *
 * Methods used:
 *   - RLHK_ALGO_MAP_GET_PASSABLE
 *   - RLHK_ALGO_MAP_CLEAR_DISTANCE
 *   - RLHK_ALGO_MAP_SET_DISTANCE
 *   - RLHK_ALGO_MAP_GET_DISTANCE
 *   - RLHK_ALGO_MAP_SET_HEURISTIC
 *   - RLHK_ALGO_MAP_GET_HEURISTIC
 *   - RLHK_ALGO_MAP_SET_GRADIENT
 *   - RLHK_ALGO_MAP_MARK_SHORTEST
 */
RLHK_ALGO_API
long rlhk_algo_shortest(rlhk_algo_map map, int x0, int y0, int x1, int y1,
                        short *buf, long buflen);

/**
 * Add an (x, y) coordinate to the buffer (buf).
 *
 * The buffer (buf) need not be initialized, but the index (i) must be
 * 0 on the first call to this function. The buffer size (buflen) is
 * in bytes.
 *
 * Returns the new value for i, or -1 if the buffer is full.
 */
RLHK_ALGO_API
long rlhk_algo_buf_push(short *buf, long buflen, long i, int x, int y);

/**
 * Create a Dijkstra map: a map-wide per-tile distance from a set of
 * tiles.
 *
 * Basically this is a breadth-first flood-fill of distance and
 * gradient. Results are delivered via RLHK_ALGO_MAP_SET_DISTANCE.
 *
 * Use rlhk_algo_buf_push() to add the points of interest to the
 * buffer (buf) before calling this function. The contents of the
 * buffer will be destroyed by this function.
 *
 * To get an early bailout and only fill the local area to the points
 * of interest, provide only a small buffer and let this function
 * (safely) run out of memory.
 *
 * Returns 1 on success or 0 if it ran out of buffer memory.
 *
 * Methods used:
 *   RLHK_ALGO_MAP_GET_PASSABLE
 *   RLHK_ALGO_MAP_SET_DISTANCE
 *   RLHK_ALGO_MAP_GET_DISTANCE
 */
RLHK_ALGO_API
int rlhk_algo_dijkstra(rlhk_algo_map map, short *buf, long buflen, long i);

/**
 * Compute the field-of-view from a given tile.
 *
 * This uses a brute force raycast, which has a lot of overlap and is
 * relatively more expensive. But it's precise and doesn't require a
 * work buffer.
 *
 * Methods used:
 *   RLHK_ALGO_MAP_MARK_VISIBLE
 */
RLHK_ALGO_API
void rlhk_algo_fov(rlhk_algo_map map, int x, int y, int radius);

/* Implementation */
#if defined(RLHK_IMPLEMENTATION) || defined(RLHK_ALGO_IMPLEMENTATION)
#include <stdlib.h>
#include <limits.h>

#define RLHK_ALGO_CALL(m, method, x, y, d) \
    rlhk_algo_map_call((m), RLHK_ALGO_MAP_##method, (x), (y), (d))

struct rlhk_algo_heap {
    short *coords;
    long count;
    long size;
};

static void
rlhk_algo_heap_swap(struct rlhk_algo_heap *heap, long a, long b)
{
    short ax = heap->coords[a * 2 + 0];
    short ay = heap->coords[a * 2 + 1];
    heap->coords[a * 2 + 0] = heap->coords[b * 2 + 0];
    heap->coords[a * 2 + 1] = heap->coords[b * 2 + 1];
    heap->coords[b * 2 + 0] = ax;
    heap->coords[b * 2 + 1] = ay;
}

static int
rlhk_algo_heap_push(struct rlhk_algo_heap *heap,
                    rlhk_algo_map map, int x, int y)
{
    long f, n;
    if (heap->count == heap->size)
        return 0;

    n = heap->count++;
    heap->coords[n * 2 + 0] = x;
    heap->coords[n * 2 + 1] = y;
    f = RLHK_ALGO_CALL(map, GET_HEURISTIC, x, y, 0);
    while (n > 0) {
        long p = (n - 1) / 2;
        int px = heap->coords[p * 2 + 0];
        int py = heap->coords[p * 2 + 1];
        long pf = RLHK_ALGO_CALL(map, GET_HEURISTIC, px, py, 0);
        if (f < pf) {
            rlhk_algo_heap_swap(heap, n, p);
            n = p;
        } else {
            break;
        }
    }
    return 1;
}

static void
rlhk_algo_heap_pop(struct rlhk_algo_heap *heap, rlhk_algo_map map)
{
    long n = 0;
    long d = --heap->count;
    int x = heap->coords[0] = heap->coords[d * 2 + 0];
    int y = heap->coords[1] = heap->coords[d * 2 + 1];
    long f = RLHK_ALGO_CALL(map, GET_HEURISTIC, x, y, 0);
    while (n < heap->count) {
        long an, af, bn, bf;
        an = 2 * n + 1;
        if (an < heap->count) {
            int ax = heap->coords[an * 2 + 0];
            int ay = heap->coords[an * 2 + 1];
            af = RLHK_ALGO_CALL(map, GET_HEURISTIC, ax, ay, 0);
        } else {
            af = LONG_MAX;
        }
        bn = 2 * n + 2;
        if (bn < heap->count) {
            int bx = heap->coords[bn * 2 + 0];
            int by = heap->coords[bn * 2 + 1];
            bf = RLHK_ALGO_CALL(map, GET_HEURISTIC, bx, by, 0);
        } else {
            bf = LONG_MAX;
        }
        if (af < f && af <= bf) {
            rlhk_algo_heap_swap(heap, n, an);
            n = an;
        } else if (bf < f && bf < af) {
            rlhk_algo_heap_swap(heap, n, bn);
            n = bn;
        } else {
            break;
        }
    }
}

#define RLHK_ALGO_MAX(a, b) ((b) > (a) ? (b) : (a))

RLHK_ALGO_API
long
rlhk_algo_shortest(rlhk_algo_map m, int x0, int y0, int x1, int y1,
                   short *buf, long buflen)
{
    struct rlhk_algo_heap heap[1];
    long length = -1;
    int origin_heuristic = RLHK_ALGO_MAX(abs(x0 - x1), abs(y0 - y1));

    heap->coords = buf;
    heap->count = 0;
    heap->size = buflen / (sizeof(heap->coords[0]) * 2);

    RLHK_ALGO_CALL(m, CLEAR_DISTANCE, 0, 0, 0);
    RLHK_ALGO_CALL(m, SET_DISTANCE, x0, y0, 0);
    RLHK_ALGO_CALL(m, SET_HEURISTIC, x0, y0, origin_heuristic);
    RLHK_ALGO_CALL(m, SET_GRADIENT, x0, y0, -1);
    rlhk_algo_heap_push(heap, m, x0, y0);

    while (heap->count) {
        int d;
        int x = heap->coords[0];
        int y = heap->coords[1];
        long g, tg;
        if (x == x1 && y == y1) {
            length = 0;
            break;
        }
        rlhk_algo_heap_pop(heap, m);
        g = RLHK_ALGO_CALL(m, GET_DISTANCE, x, y, 0);
        for (d = 0; d < 8; d++) {
            long tentative = g + 1;
            int tx = x + RLHK_ALGO_DX(d);
            int ty = y + RLHK_ALGO_DY(d);
            int passable =
                RLHK_ALGO_CALL(m, GET_PASSABLE, tx, ty, (d + 4) % 8);
            if (!passable)
                continue;
            tg = RLHK_ALGO_CALL(m, GET_DISTANCE, tx, ty, 0);
            if (tg == -1 || tentative < tg) {
                int h = RLHK_ALGO_MAX(abs(tx - x1), abs(ty - y1));
                RLHK_ALGO_CALL(m, SET_GRADIENT, tx, ty, (d + 4) % 8);
                RLHK_ALGO_CALL(m, SET_DISTANCE, tx, ty, tentative);
                RLHK_ALGO_CALL(m, SET_HEURISTIC, tx, ty, tentative + h);
                if (!rlhk_algo_heap_push(heap, m, tx, ty))
                    return -2; /* out of memory */
            }
        }
    }

    /* Reconstruct shortest route. */
    if (length == 0) {
        int x = x1;
        int y = y1;
        int d;
        while (x != x0 || y != y0) {
            d = RLHK_ALGO_CALL(m, MARK_SHORTEST, x, y, length);
            x += RLHK_ALGO_DX(d);
            y += RLHK_ALGO_DY(d);
            length++;
        }
        RLHK_ALGO_CALL(m, MARK_SHORTEST, x, y, length);
    }

    return length;
}

RLHK_ALGO_API
long
rlhk_algo_buf_push(short *buf, long buflen, long i, int x, int y)
{
    long size = buflen / (sizeof(*buf) * 2);
    if (i == size) {
        return -1;
    } else {
        buf[i * 2 + 0] = x;
        buf[i * 2 + 1] = y;
        return i + 1;
    }
}

RLHK_ALGO_API
int rlhk_algo_dijkstra(rlhk_algo_map m, short *buf, long buflen, long head)
{
    long size = buflen / (sizeof(*buf) * 2);
    long tail = 0;
    long i;

    /* Initialize distances. */
    RLHK_ALGO_CALL(m, CLEAR_DISTANCE, 0, 0, 0);
    for (i = 0; i < head; i++) {
        int x = buf[i * 2 + 0];
        int y = buf[i * 2 + 1];
        RLHK_ALGO_CALL(m, SET_DISTANCE, x, y, 0);
    }

    /* Breadth-first search. */
    while (tail != head) {
        int d;
        int x = buf[tail * 2 + 0];
        int y = buf[tail * 2 + 1];
        long v = RLHK_ALGO_CALL(m, GET_DISTANCE, x, y, 0);
        tail = (tail + 1) % size;
        for (d = 0; d < 8; d++) {
            int cx = x + RLHK_ALGO_DX(d);
            int cy = y + RLHK_ALGO_DY(d);
            int p = RLHK_ALGO_CALL(m, GET_PASSABLE, cx, cy, (d + 4) % 8);
            if (p) {
                long cv = RLHK_ALGO_CALL(m, GET_DISTANCE, cx, cy, 0);
                if (cv == -1) {
                    long next = (head + 1) % size;
                    RLHK_ALGO_CALL(m, SET_DISTANCE, cx, cy, v + 1);
                    if (next == tail)
                        return 0; /* out of memory */
                    buf[head * 2 + 0] = cx;
                    buf[head * 2 + 1] = cy;
                    head = next;

                }
            }
        }
    }
    return 1;
}

static void
rlhk_algo_raycast(rlhk_algo_map map, int x0, int y0, int x1, int y1)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x1 < x0 ? -1 : 1;
    int sy = y1 < y0 ? -1 : 1;

    if (dx > dy) {
        int d = 2 * dy - dx;
        int x, y = y0;
        for (x = x0; x != x1; x += sx) {
            if (!RLHK_ALGO_CALL(map, MARK_VISIBLE, x, y, 0))
                return;
            if (d > 0) {
                y += sy;
                d -= 2 * dx;
            }
            d += 2 * dy;
        }
    } else {
        int d = 2 * dx - dy;
        int y, x = x0;
        for (y = y0; y != y1; y += sy) {
            if (!RLHK_ALGO_CALL(map, MARK_VISIBLE, x, y, 0))
                return;
            if (d > 0) {
                x += sx;
                d -= 2 * dy;
            }
            d += 2 * dx;
        }
    }
    RLHK_ALGO_CALL(map, MARK_VISIBLE, x1, y1, 0);
}

static void
rlhk_algo_cast8(rlhk_algo_map map, int x0, int y0, int x, int y)
{
    rlhk_algo_raycast(map, x0, y0, x0 + x, y0 + y);
    rlhk_algo_raycast(map, x0, y0, x0 + y, y0 + x);
    rlhk_algo_raycast(map, x0, y0, x0 - y, y0 + x);
    rlhk_algo_raycast(map, x0, y0, x0 - x, y0 + y);
    rlhk_algo_raycast(map, x0, y0, x0 - x, y0 - y);
    rlhk_algo_raycast(map, x0, y0, x0 - y, y0 - x);
    rlhk_algo_raycast(map, x0, y0, x0 + y, y0 - x);
    rlhk_algo_raycast(map, x0, y0, x0 + x, y0 - y);
}

RLHK_ALGO_API
void
rlhk_algo_fov(rlhk_algo_map map, int x0, int y0, int r)
{
    int x = r;
    int y = 0;
    int e = 0;
    while (x >= y) {
        rlhk_algo_cast8(map, x0, y0, x, y);
        if (e <= 0) {
            y++;
            e += 2 * y + 1;
            rlhk_algo_cast8(map, x0, y0, x, y);
        }
        if (e > 0) {
            x--;
            e -= 2 * x + 1;
            rlhk_algo_cast8(map, x0, y0, x, y);
        }
    }
}

#endif /* RLHK_ALGO_IMPLEMENTATION */
#endif /* RLHK_ALGO_H */
