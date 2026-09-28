#include "kv_cache.h"
#include <cassert>

int main() {
    KvCacheTracker cache(1000);
    assert(cache.reserve(1, 400));
    assert(cache.reserve(2, 500));
    assert(!cache.reserve(3, 200));
    cache.release(1);
    assert(cache.reserve(3, 200));
}
