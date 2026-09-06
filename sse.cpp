// clang++ -std=c++17 -msse2 -O0 -o sse_test main.cpp

#include <emmintrin.h>
#include <xmmintrin.h>
#include <mm_malloc.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <cstdio>

// Cache warm-up, без него SSE проигрывает
#define WARMUP_ITERATIONS 1000
#define TEST_ITERATIONS 10000
#define FLOATS_COUNT 1024

int main()
{
    // Аллокация (16 байт)
    float *a = (float *)_mm_malloc(FLOATS_COUNT * sizeof(float), 16);
    float *b = (float *)_mm_malloc(FLOATS_COUNT * sizeof(float), 16);
    float *c = (float *)_mm_malloc(FLOATS_COUNT * sizeof(float), 16);

    ////////////////////////////////////////////////////////////

    // Заполнить случайными значениями

    srand(time(nullptr));
    for (size_t i = 0; i < FLOATS_COUNT; ++i)
    {
        a[i] = (float)rand() / (float)RAND_MAX;
        b[i] = (float)rand() / (float)RAND_MAX;
    }

    // Cache warm-up
    for (int i = 0; i < WARMUP_ITERATIONS; ++i)
    {
        __m128 *a_sse = (__m128 *)a;
        __m128 *b_sse = (__m128 *)b;
        __m128 *c_sse = (__m128 *)c;
        const size_t sse_count = FLOATS_COUNT / 4;

        for (size_t i = 0; i < sse_count; ++i)
        {
            *c_sse = _mm_add_ps(*a_sse, *b_sse);
            ++a_sse;
            ++b_sse;
            ++c_sse;
        }

        float *a_ptr = a;
        float *b_ptr = b;
        float *c_ptr = c;

        for (size_t i = 0; i < FLOATS_COUNT; ++i)
        {
            *c_ptr = *a_ptr + *b_ptr;
            ++a_ptr;
            ++b_ptr;
            ++c_ptr;
        }
    }

    // SIMD

    // Интерпретация как 128-битное число
    __m128 *a_sse = (__m128 *)a;
    __m128 *b_sse = (__m128 *)b;
    __m128 *c_sse = (__m128 *)c;

    // Начало замера

    auto start_timer_sse = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < FLOATS_COUNT; i += 4)
    {
        // Сложение 4-х float разом
        *c_sse = _mm_add_ps(*a_sse, *b_sse);

        // Смещение на 4 float
        ++a_sse;
        ++b_sse;
        ++c_sse;
    }

    // Конец замера времени

    auto end_timer_sse = std::chrono::high_resolution_clock::now();
    auto time_sse = std::chrono::duration_cast<std::chrono::nanoseconds>(end_timer_sse - start_timer_sse).count();

    ////////////////////////////////////////////////////////////

    // Без SSE

    // Массив решил не создавать снова, сделал сдвиг на начала созданных
    float *a_prt = a;
    float *b_prt = b;
    float *c_prt = c;

    // Начало замера
    auto start_timer_nosse = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < FLOATS_COUNT; ++i)
    {
        // Сложение 1-го float
        *c_prt = *a_prt + *b_prt;

        // Смещение на 1 float
        ++a_prt;
        ++b_prt;
        ++c_prt;
    }

    // Конец замера времени
    auto end_timer_nosse = std::chrono::high_resolution_clock::now();
    auto time_nosse = std::chrono::duration_cast<std::chrono::nanoseconds>(end_timer_nosse - start_timer_nosse).count();

    std::cout << "SSE: " << time_sse << " нс" << std::endl;
    std::cout << "Обычный: " << time_nosse << " нс" << std::endl;
    printf("Разница в: %.2f раз\n", (double)time_nosse / time_sse);
}