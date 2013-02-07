//
//  Upcaste Performance Libraries
//  Copyright (C) 2012-2013 Jesse W. Towner
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <up/cinttypes.hpp>
#include <up/cstdlib.hpp>
#include <up/cstdio.hpp>
#include <up/cstring.hpp>

bool option_help = false;
bool option_key_value_mode = false;
bool option_verbose = false;

char const** strings = nullptr;
size_t strings_size = 0;
size_t strings_capacity = 0;

int main(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        char const* arg = argv[i];
        if (!arg) {
            continue;
        }

        if (arg[0] == '-') {
            if (arg[1] == '\0') {
                up::fputs("fatal error: invalid option. terminating...\n", stderr);
                up::exit(EXIT_FAILURE);
            }

            if (arg[1] == '-') {
                if (!up::strcmp(&arg[2], "help")) {
                    option_help = true;
                }
                else if (!up::strcmp(&arg[2], "key-value")) {
                    option_key_value_mode = true;
                }
                else if (!up::strcmp(&arg[2], "verbose")) {
                    option_verbose = true;
                }
                else {
                    up::fprintf(stderr, "fatal error: invalid option '%s'. terminating...\n", arg);
                    up::exit(EXIT_FAILURE);
                }
                continue;
            }
            
            for (size_t j = 1; arg[j] != '\0'; ++j) {
                if (arg[j] == 'h') {
                    option_help = true;
                }
                else if (arg[j] == 'k') {
                    option_key_value_mode = true;
                }
                else if (arg[j] == 'v') {
                    option_verbose = true;
                }
                else {
                    char option[3];
                    option[0] = '-';
                    option[1] = arg[j];
                    option[2] = '\0';
                    up::fprintf(stderr, "fatal error: invalid option '%s'. terminating...\n", option);
                    up::exit(EXIT_FAILURE);
                }
            }

            continue;
        }

        if (strings_size >= strings_capacity) {
            strings_capacity = (strings_capacity >= 8) ? (strings_capacity * 2) : 8;
            strings = static_cast<char const**>(up::realloc(strings, strings_capacity * sizeof(char*)));
            if (!strings) {
                up::fputs("fatal error: out of memory. terminating...\n", stderr);
                up::exit(EXIT_FAILURE);
            }
        }

        strings[strings_size++] = arg;
    }

    if (option_help) {
        up::puts(
            "Usage: upcaste-hash [options] key...\n"
            "Options:\n"
            "\t-h,--help\tPrints this help message and exits.\n"
            "\t-k,--key-value\tOutputs hashcode results in key-value pairs.\n"
            "\t-v,--verbose\tLogs additional diagnostic messages to stderr.\n"
        );
        up::exit(EXIT_SUCCESS);
    }

    if (!strings_size) {
        up::fputs("fatal error: no input. terminating...\n", stderr);
        up::exit(EXIT_FAILURE);
    }

    if (option_verbose) {
        up::fputs("hashing keys passed on command line...\n", stderr);
    }

    for (size_t i = 0; i < strings_size; ++i) {
        if (option_verbose) {
            up::fprintf(stderr, "[%" PRIuMAX "] hashing \"%s\" ...\n", static_cast<uintmax_t>(i + 1), strings[i]);
        }

        uint_least64_t hashcode = up::strhash(strings[i]);

        if (option_key_value_mode) {
            up::printf("\"%s\"=%#.8" PRIxLEAST64 "\n", strings[i], hashcode);
        }
        else {
            up::printf("%#.8" PRIxLEAST64 "\n", hashcode);
        }
    }

    if (option_verbose) {
        up::fputs("done.\n", stderr);
    }

    return 0;
}

