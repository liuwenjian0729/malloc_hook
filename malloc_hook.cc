#define _GNU_SOURCE

#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <execinfo.h>
#include <iostream>

extern "C" {

int inside_hook = 0;  // 用于避免递归
std::mutex hook_mutex; 

void *(*original_malloc)(size_t) = NULL;
void (*original_free)(void *) = NULL;
void *(*original_calloc)(size_t, size_t) = NULL;
void *(*original_realloc)(void *, size_t) = NULL;

void *malloc(size_t size) {
    if (!original_malloc) preeny_mallocwatch_orig();
    void *r = original_malloc(size);
    {
        std::lock_guard<std::mutex> guard(hook_mutex);
        if (!inside_hook) {
            inside_hook = 1;  // 进入钩子函数，设置标志位
            void* callstack[128];
            int frames = backtrace(callstack, 128);
            char** strs = backtrace_symbols(callstack, frames);
            for (int i = 0; i < frames; ++i) {
                printf("%s\n", strs[i]);
            }
            free(strs);
            inside_hook = 0;  // 离开钩子函数，清除标志位
        }
    }
    return r;
}

void free(void *ptr) {
    if (!original_free) preeny_mallocwatch_orig();
    original_free(ptr);
    {
        std::lock_guard<std::mutex> guard(hook_mutex);
        if (!inside_hook) {
            inside_hook = 1;  // 进入钩子函数，设置标志位
            printf("free(%p)\n", ptr);
            inside_hook = 0;  // 离开钩子函数，清除标志位
        }
    }
}

void *calloc(size_t nmemb, size_t size) {
    if (!original_calloc) preeny_mallocwatch_orig();
    void *r = original_calloc(nmemb, size);
    {
        std::lock_guard<std::mutex> guard(hook_mutex);
        if (!inside_hook) {
            inside_hook = 1;  // 进入钩子函数，设置标志位
            printf("calloc(%zu, %zu) == %p\n", nmemb, size, r);
            inside_hook = 0;  // 离开钩子函数，清除标志位
        }
    }
    return r;
}

void *realloc(void *ptr, size_t size) {
    if (!original_realloc) preeny_mallocwatch_orig();
    void *r = original_realloc(ptr, size);
    {
        std::lock_guard<std::mutex> guard(hook_mutex);
        if (!inside_hook) {
            inside_hook = 1;  // 进入钩子函数，设置标志位
            printf("realloc(%p, %zu) == %p\n", ptr, size, r);
            inside_hook = 0;  // 离开钩子函数，清除标志位
        }
    }
    return r;
}

__attribute__((constructor)) void preeny_mallocwatch_orig() {
    original_malloc = (void *(*)(size_t))dlsym(RTLD_NEXT, "malloc");
    original_free = (void (*)(void *))dlsym(RTLD_NEXT, "free");
    original_calloc = (void *(*)(size_t, size_t))dlsym(RTLD_NEXT, "calloc");
    original_realloc = (void *(*)(void *, size_t))dlsym(RTLD_NEXT, "realloc");
}

} // extern "C"