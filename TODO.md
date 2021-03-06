- Change `cstdio.hpp` to use our portable version of printf/scanf when in use with MSVC.
- Fix the formatting of floats in `printf` / `fast_printf` variants.
- Implement `scanf` / `fast_scanf` variants.
- Implement `memset_pattern2` and `memset_pattern16` and update `uninitialized_fill_n`.
- Add implementations for wide-character versions of `printf`/`scanf` functions.
- Optimize and test the s-expression parser/lexer more thoroughly.
- Implement `memccpy`, `stpcpy`, `stpncpy`, and better implement `mempcpy`.
- Implement `wmemccpy`, `wcpcpy`, `wcpncpy`, and better implement `wmempcpy`.
- Implement filesystem `copy` and `copy_directory` functions.
- Test filesystem more thoroughly on Windows.
- Finish implementing transcedental functions for vector math library.
- Revamp the generic C++ port of the vector math library, and get it working again.
- Add ARM NEON port of the vector math library.
- Add perfect hashing library which can auto-generate an optimized C++ hashing function source file for a given key set.
- Improve the benchmarking code in uptest to output more useful metrics, and get rid of the useless ones like the iteration/size count--maybe make it customizable via a format string or flags.
- Add a command line arguments parsing library to upcore.
- Make sure function attributes in `cuchar.hpp` match the ones in `cstring.hpp`.
- Audit Windows implementation of thread-local locales for memory leaks and proper use of LC_GLOBAL_LOCALE.

