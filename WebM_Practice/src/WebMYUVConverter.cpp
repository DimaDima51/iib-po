#include "WebMYUVConverter.h"

#include <emmintrin.h>

namespace xgen
{
    namespace video
    {
        namespace
        {
            inline int clampToByte(int value)
            {
                return (value < 0) ? 0 : ((value > 255) ? 255 : value);
            }
        }

        void yuv420ToRGBA(uint width, uint height,
                          const uint8_t *y, const uint8_t *u, const uint8_t *v,
                          unsigned int ystride, unsigned int ustride, unsigned int vstride,
                          unsigned char *out)
        {
            for (uint h = 0; h < height; h++)
            {
                const uint y_offset = h * ystride;
                const uint u_offset = (h / 2) * ustride;
                const uint v_offset = (h / 2) * vstride;

                uint x = 0;
                for (; x + 3 < width; x += 4)
                {
                    int y_vals[4];
                    int u_vals[4];
                    int v_vals[4];

                    for (int i = 0; i < 4; ++i)
                    {
                        const uint pixel_x = x + i;
                        y_vals[i] = y[y_offset + pixel_x];
                        const uint sample_x = pixel_x >> 1;
                        u_vals[i] = u[u_offset + sample_x];
                        v_vals[i] = v[v_offset + sample_x];
                    }

                    __m128i u_centered_vec = _mm_sub_epi32(_mm_setr_epi32(u_vals[0], u_vals[1], u_vals[2], u_vals[3]), _mm_set1_epi32(128));
                    __m128i v_centered_vec = _mm_sub_epi32(_mm_setr_epi32(v_vals[0], v_vals[1], v_vals[2], v_vals[3]), _mm_set1_epi32(128));

                    int u_centered[4];
                    int v_centered[4];
                    _mm_storeu_si128(reinterpret_cast<__m128i *>(u_centered), u_centered_vec);
                    _mm_storeu_si128(reinterpret_cast<__m128i *>(v_centered), v_centered_vec);

                    int r[4];
                    int g[4];
                    int b[4];

                    for (int i = 0; i < 4; ++i)
                    {
                        const int u_c = u_centered[i];
                        const int v_c = v_centered[i];

                        r[i] = clampToByte(static_cast<int>(y_vals[i]) + (113983 * v_c) / 100000);
                        g[i] = clampToByte(static_cast<int>(y_vals[i]) - (39465 * u_c) / 100000 - (58060 * v_c) / 100000);
                        b[i] = clampToByte(static_cast<int>(y_vals[i]) + (203211 * u_c) / 100000);
                    }

                    for (int i = 0; i < 4; ++i)
                    {
                        const uint pixel_x = x + i;
                        const uint out_offset = (h * width + pixel_x) * 4;
                        out[out_offset + 0] = static_cast<unsigned char>(r[i]);
                        out[out_offset + 1] = static_cast<unsigned char>(g[i]);
                        out[out_offset + 2] = static_cast<unsigned char>(b[i]);
                        out[out_offset + 3] = 255;
                    }
                }

                for (; x < width; x++)
                {
                    const uint y_val = y[y_offset + x];
                    const uint u_val = u[u_offset + (x / 2)];
                    const uint v_val = v[v_offset + (x / 2)];

                    const int u_centered = static_cast<int>(u_val) - 128;
                    const int v_centered = static_cast<int>(v_val) - 128;

                    const int r_val = static_cast<int>(y_val) + (113983 * v_centered) / 100000;
                    const int g_val = static_cast<int>(y_val) - (39465 * u_centered) / 100000 - (58060 * v_centered) / 100000;
                    const int b_val = static_cast<int>(y_val) + (203211 * u_centered) / 100000;

                    const uint out_offset = (h * width + x) * 4;
                    out[out_offset + 0] = static_cast<unsigned char>(clampToByte(r_val));
                    out[out_offset + 1] = static_cast<unsigned char>(clampToByte(g_val));
                    out[out_offset + 2] = static_cast<unsigned char>(clampToByte(b_val));
                    out[out_offset + 3] = 255;
                }
            }
        }

        void yuv420AToRGBA(uint width, uint height,
                           const uint8_t *y, const uint8_t *u, const uint8_t *v, const uint8_t *a,
                           unsigned int ystride, unsigned int ustride, unsigned int vstride, unsigned int astride,
                           unsigned char *out)
        {
            for (uint h = 0; h < height; h++)
            {
                const uint y_offset = h * ystride;
                const uint u_offset = (h / 2) * ustride;
                const uint v_offset = (h / 2) * vstride;
                const uint a_offset = h * astride;

                uint x = 0;
                for (; x + 3 < width; x += 4)
                {
                    int y_vals[4];
                    int u_vals[4];
                    int v_vals[4];
                    int a_vals[4];

                    for (int i = 0; i < 4; ++i)
                    {
                        const uint pixel_x = x + i;
                        y_vals[i] = y[y_offset + pixel_x];
                        const uint sample_x = pixel_x >> 1;
                        u_vals[i] = u[u_offset + sample_x];
                        v_vals[i] = v[v_offset + sample_x];
                        a_vals[i] = a[a_offset + pixel_x];
                    }

                    __m128i u_centered_vec = _mm_sub_epi32(_mm_setr_epi32(u_vals[0], u_vals[1], u_vals[2], u_vals[3]), _mm_set1_epi32(128));
                    __m128i v_centered_vec = _mm_sub_epi32(_mm_setr_epi32(v_vals[0], v_vals[1], v_vals[2], v_vals[3]), _mm_set1_epi32(128));

                    int u_centered[4];
                    int v_centered[4];
                    _mm_storeu_si128(reinterpret_cast<__m128i *>(u_centered), u_centered_vec);
                    _mm_storeu_si128(reinterpret_cast<__m128i *>(v_centered), v_centered_vec);

                    int r[4];
                    int g[4];
                    int b[4];

                    for (int i = 0; i < 4; ++i)
                    {
                        const int u_c = u_centered[i];
                        const int v_c = v_centered[i];

                        r[i] = clampToByte(static_cast<int>(y_vals[i]) + (113983 * v_c) / 100000);
                        g[i] = clampToByte(static_cast<int>(y_vals[i]) - (39465 * u_c) / 100000 - (58060 * v_c) / 100000);
                        b[i] = clampToByte(static_cast<int>(y_vals[i]) + (203211 * u_c) / 100000);
                    }

                    for (int i = 0; i < 4; ++i)
                    {
                        const uint pixel_x = x + i;
                        const uint out_offset = (h * width + pixel_x) * 4;
                        out[out_offset + 0] = static_cast<unsigned char>(r[i]);
                        out[out_offset + 1] = static_cast<unsigned char>(g[i]);
                        out[out_offset + 2] = static_cast<unsigned char>(b[i]);
                        out[out_offset + 3] = static_cast<unsigned char>(a_vals[i]);
                    }
                }

                for (; x < width; x++)
                {
                    const uint y_val = y[y_offset + x];
                    const uint u_val = u[u_offset + (x / 2)];
                    const uint v_val = v[v_offset + (x / 2)];
                    const uint a_val = a[a_offset + x];

                    const int u_centered = static_cast<int>(u_val) - 128;
                    const int v_centered = static_cast<int>(v_val) - 128;

                    const int r_val = static_cast<int>(y_val) + (113983 * v_centered) / 100000;
                    const int g_val = static_cast<int>(y_val) - (39465 * u_centered) / 100000 - (58060 * v_centered) / 100000;
                    const int b_val = static_cast<int>(y_val) + (203211 * u_centered) / 100000;

                    const uint out_offset = (h * width + x) * 4;
                    out[out_offset + 0] = static_cast<unsigned char>(clampToByte(r_val));
                    out[out_offset + 1] = static_cast<unsigned char>(clampToByte(g_val));
                    out[out_offset + 2] = static_cast<unsigned char>(clampToByte(b_val));
                    out[out_offset + 3] = static_cast<unsigned char>(a_val);
                }
            }
        }

        void yuv420ToRGB(uint width, uint height,
                         const uint8_t *y, const uint8_t *u, const uint8_t *v,
                         unsigned int ystride, unsigned int ustride, unsigned int vstride,
                         unsigned char *out)
        {
            for (uint h = 0; h < height; h++)
            {
                const uint y_offset = h * ystride;
                const uint u_offset = (h / 2) * ustride;
                const uint v_offset = (h / 2) * vstride;

                uint x = 0;
                for (; x + 3 < width; x += 4)
                {
                    int y_vals[4];
                    int u_vals[4];
                    int v_vals[4];

                    for (int i = 0; i < 4; ++i)
                    {
                        const uint pixel_x = x + i;
                        y_vals[i] = y[y_offset + pixel_x];
                        const uint sample_x = pixel_x >> 1;
                        u_vals[i] = u[u_offset + sample_x];
                        v_vals[i] = v[v_offset + sample_x];
                    }

                    __m128i u_centered_vec = _mm_sub_epi32(_mm_setr_epi32(u_vals[0], u_vals[1], u_vals[2], u_vals[3]), _mm_set1_epi32(128));
                    __m128i v_centered_vec = _mm_sub_epi32(_mm_setr_epi32(v_vals[0], v_vals[1], v_vals[2], v_vals[3]), _mm_set1_epi32(128));

                    int u_centered[4];
                    int v_centered[4];
                    _mm_storeu_si128(reinterpret_cast<__m128i *>(u_centered), u_centered_vec);
                    _mm_storeu_si128(reinterpret_cast<__m128i *>(v_centered), v_centered_vec);

                    int r[4];
                    int g[4];
                    int b[4];

                    for (int i = 0; i < 4; ++i)
                    {
                        const int u_c = u_centered[i];
                        const int v_c = v_centered[i];

                        r[i] = clampToByte(static_cast<int>(y_vals[i]) + (113983 * v_c) / 100000);
                        g[i] = clampToByte(static_cast<int>(y_vals[i]) - (39465 * u_c) / 100000 - (58060 * v_c) / 100000);
                        b[i] = clampToByte(static_cast<int>(y_vals[i]) + (203211 * u_c) / 100000);
                    }

                    for (int i = 0; i < 4; ++i)
                    {
                        const uint pixel_x = x + i;
                        const uint out_offset = (h * width + pixel_x) * 3;
                        out[out_offset + 0] = static_cast<unsigned char>(r[i]);
                        out[out_offset + 1] = static_cast<unsigned char>(g[i]);
                        out[out_offset + 2] = static_cast<unsigned char>(b[i]);
                    }
                }

                for (; x < width; x++)
                {
                    const uint y_val = y[y_offset + x];
                    const uint u_val = u[u_offset + (x / 2)];
                    const uint v_val = v[v_offset + (x / 2)];

                    const int u_centered = static_cast<int>(u_val) - 128;
                    const int v_centered = static_cast<int>(v_val) - 128;

                    const int r_val = static_cast<int>(y_val) + (113983 * v_centered) / 100000;
                    const int g_val = static_cast<int>(y_val) - (39465 * u_centered) / 100000 - (58060 * v_centered) / 100000;
                    const int b_val = static_cast<int>(y_val) + (203211 * u_centered) / 100000;

                    const uint out_offset = (h * width + x) * 3;
                    out[out_offset + 0] = static_cast<unsigned char>(clampToByte(r_val));
                    out[out_offset + 1] = static_cast<unsigned char>(clampToByte(g_val));
                    out[out_offset + 2] = static_cast<unsigned char>(clampToByte(b_val));
                }
            }
        }

        // Y в альфу компоненту буфферов
        void yToA(uint width, uint height, const uint8_t *a, unsigned int ystride, unsigned char *out)
        {
            for (uint h = 0; h < height; h++)
            {
                const uint a_offset = h * ystride;

                uint x = 0;
                for (; x + 3 < width; x += 4)
                {
                    const __m128i src = _mm_loadu_si128(reinterpret_cast<const __m128i *>(a + a_offset + x));
                    const __m128i alpha_values = _mm_unpacklo_epi8(src, _mm_setzero_si128());

                    unsigned char tmp[16];
                    _mm_storeu_si128(reinterpret_cast<__m128i *>(tmp), alpha_values);

                    const uint out_offset = (h * width + x) * 4;
                    for (int i = 0; i < 4; ++i)
                    {
                        out[out_offset + i * 4 + 3] = tmp[i * 2];
                    }
                }

                for (; x < width; x++)
                {
                    const uint a_val = a[a_offset + x];
                    const uint out_offset = (h * width + x) * 4;
                    out[out_offset + 3] = static_cast<unsigned char>(a_val);
                }
            }
        }

    }
}

/*

R = Y + 1,13983 * (V - 128);
G = Y - 0,39465 * (U - 128) - 0,58060 * (V - 128);
B = Y + 2,03211 * (U - 128);
Y = 0,299 * R + 0,587 * G + 0,114 * B;
U = -0,14713 * R - 0,28886 * G + 0,436 * B + 128;
V = 0,615 * R - 0,51499 * G - 0,10001 * B + 128

Для вычислений лучше домножить на 100000 и потом делить на 100000, чтобы не было потери точности!!!
*/