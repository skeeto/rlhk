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
     * Set a 32-bit "fscore" for (x, y). The return value is ignored.
     */
    RLHK_ALGO_MAP_SET_FSCORE,

    /**
     * Return the previously-set 32-bit "fscore" at (x, y). The "data"
     * parameter is unused.
     */
    RLHK_ALGO_MAP_GET_FSCORE,

    /**
     * Set every tile's 32-bit "gscore" to the maximum value. The
     * "data" parameter will be LONG_MAX for your convenience. The
     * return value is ignored.
     */
    RLHK_ALGO_MAP_CLEAR_GSCORE,

    /**
     * Set a 32-bit "gscore" for (x, y). The return value is ignored.
     */
    RLHK_ALGO_MAP_SET_GSCORE,

    /**
     * Return the previously-set 32-bit "fscore" at (x, y). The "data"
     * parameter is unused.
     */
    RLHK_ALGO_MAP_GET_GSCORE,

    /**
     * Set an unsigned 3-bit directional value (0-7) at (x, y). A
     * special direction of "-1" is sometimes used to indicate a lack
     * of direction or gradient. You are not expected to return this
     * -1 later and may either discard/ignore it or use the
     * information as you wish.
     *
     * The return value is ignored.
     */
    RLHK_ALGO_MAP_SET_ROUTE,

    /**
     * Sets the path distance to the goal for (x, y). The return value
     * is ignored. This is used to build a Dijkstra map.
     */
    RLHK_ALGO_MAP_SET_DISTANCE,

    /**
     * Indicates that a tile lies along a shortest path. You *must*
     * return the previous RLHK_ALGO_MAP_SET_ROUTE data value, unless
     * that value was -1 in which case the return value is ignored.
     * The "data" value is the 32-bit distance from the goal (x1, y1).
     */
    RLHK_ALGO_MAP_SET_RESULT
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
 *   RLHK_ALGO_MAP_GET_PASSABLE
 *   RLHK_ALGO_MAP_SET_FSCORE
 *   RLHK_ALGO_MAP_GET_FSCORE
 *   RLHK_ALGO_MAP_CLEAR_GSCORE
 *   RLHK_ALGO_MAP_SET_GSCORE
 *   RLHK_ALGO_MAP_GET_GSCORE
 *   RLHK_ALGO_MAP_SET_ROUTE
 *   RLHK_ALGO_MAP_SET_RESULT
 */
RLHK_ALGO_API
long rlhk_algo_shortest(rlhk_algo_map map, int x0, int y0, int x1, int y1,
                        short *buf, long buflen);

/**
 * Create a Dijkstra map: a map-wide per-tile distance from some (x, y).
 *
 * Basically this is a breadth-first flood-fill of distance and
 * gradient. Results are delivered via RLHK_ALGO_MAP_SET_ROUTE.
 *
 * You must provide some workspace memory (buf) and its size in bytes
 * (buflen) to be used as a queue. The memory need not be initialized.
 *
 * To get an early bailout and only fill the local area, provide a
 * small workspace and let the function (safely) run out of memory.
 *
 * Returns 1 on success or 0 if it ran out of workspace memory.
 *
 * Methods used:
 *   RLHK_ALGO_MAP_GET_PASSABLE
 *   RLHK_ALGO_MAP_SET_ROUTE
 */
RLHK_ALGO_API
int rlhk_algo_dijkstra(rlhk_algo_map map, int x, int y,
                       short *buf, long buflen);

/* Implementation */
#if defined(RLHK_IMPLEMENTATION) || defined(RLHK_ALGO_IMPLEMENTATION)
#include <stdlib.h>
#include <limits.h>

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
    f = rlhk_algo_map_call(map, RLHK_ALGO_MAP_GET_FSCORE, x, y, 0);
    while (n > 0) {
        long p = (n - 1) / 2;
        int px = heap->coords[p * 2 + 0];
        int py = heap->coords[p * 2 + 1];
        long pf = rlhk_algo_map_call(map, RLHK_ALGO_MAP_GET_FSCORE, px, py, 0);
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
    long f = rlhk_algo_map_call(map, RLHK_ALGO_MAP_GET_FSCORE, x, y, 0);
    while (n < heap->count) {
        long an, af, bn, bf;
        an = 2 * n + 1;
        if (an < heap->count) {
            int ax = heap->coords[an * 2 + 0];
            int ay = heap->coords[an * 2 + 1];
            af = rlhk_algo_map_call(map, RLHK_ALGO_MAP_GET_FSCORE, ax, ay, 0);
        } else {
            af = LONG_MAX;
        }
        bn = 2 * n + 2;
        if (bn < heap->count) {
            int bx = heap->coords[bn * 2 + 0];
            int by = heap->coords[bn * 2 + 1];
            bf = rlhk_algo_map_call(map, RLHK_ALGO_MAP_GET_FSCORE, bx, by, 0);
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

    heap->coords = buf;
    heap->count = 0;
    heap->size = buflen / (sizeof(heap->coords[0]) * 2);

    rlhk_algo_map_call(m, RLHK_ALGO_MAP_CLEAR_GSCORE, 0, 0, LONG_MAX);
    rlhk_algo_map_call(m, RLHK_ALGO_MAP_SET_GSCORE, x0, y0, LONG_MAX);
    rlhk_algo_map_call(m, RLHK_ALGO_MAP_SET_FSCORE, x0, y0,
                       RLHK_ALGO_MAX(abs(x0 - x1), abs(y0 - y1)));
    rlhk_algo_map_call(m, RLHK_ALGO_MAP_SET_ROUTE, x0, y0, -1);
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
        g = rlhk_algo_map_call(m, RLHK_ALGO_MAP_GET_GSCORE, x, y, 0);
        for (d = 0; d < 8; d++) {
            long tentative = g + 1;
            int tx = x + RLHK_ALGO_DX(d);
            int ty = y + RLHK_ALGO_DY(d);
            int passable =
                rlhk_algo_map_call(m, RLHK_ALGO_MAP_GET_PASSABLE, tx, ty,
                                   (d + 4) % 8);
            if (!passable)
                continue;
            tg = rlhk_algo_map_call(m, RLHK_ALGO_MAP_GET_GSCORE, tx, ty, 0);
            if (tentative < tg) {
                int heuristic = RLHK_ALGO_MAX(abs(tx - x1), abs(ty - y1));
                rlhk_algo_map_call(m, RLHK_ALGO_MAP_SET_ROUTE, tx, ty,
                                   (d + 4) % 8);
                rlhk_algo_map_call(m, RLHK_ALGO_MAP_SET_GSCORE,
                                   tx, ty, tentative);
                rlhk_algo_map_call(m, RLHK_ALGO_MAP_SET_FSCORE,
                                   tx, ty, tentative + heuristic);
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
        do {
            d = rlhk_algo_map_call(m, RLHK_ALGO_MAP_SET_RESULT, x, y, length);
            x += RLHK_ALGO_DX(d);
            y += RLHK_ALGO_DY(d);
            length++;
        } while (x != x0 || y != y0);
        rlhk_algo_map_call(m, RLHK_ALGO_MAP_SET_RESULT, x, y, length);
    }

    return length;
}

#endif /* RLHK_ALGO_IMPLEMENTATION */
#endif /* RLHK_ALGO_H */
