#include "WebMYUVConverter.h"

#if defined(__SSE2__) || defined(_M_X64) || \
    (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#include <emmintrin.h>
#define WEBM_YUV_HAS_SSE2
#endif

namespace
{

    void convertRow(uint width,
                    const uint8_t *yRow, const uint8_t *uRow, const uint8_t *vRow,
                    const uint8_t *aRow, unsigned char *outRow)
    {
        uint i = 0;

#ifdef WEBM_YUV_HAS_SSE2
        for (; i + 1 < width; i += 2)
        {
            const double u = static_cast<double>(uRow[i / 2] - 128);
            const double v = static_cast<double>(vRow[i / 2] - 128);
            const __m128d y = _mm_set_pd(
                static_cast<double>(yRow[i + 1]),
                static_cast<double>(yRow[i]));
            const __m128d uVector = _mm_set1_pd(u);
            const __m128d vVector = _mm_set1_pd(v);

            const __m128d r = _mm_add_pd(
                y, _mm_div_pd(_mm_mul_pd(vVector, _mm_set1_pd(113983.0)),
                              _mm_set1_pd(100000.0)));
            const __m128d g = _mm_sub_pd(
                _mm_sub_pd(
                    y, _mm_div_pd(_mm_mul_pd(uVector, _mm_set1_pd(39465.0)),
                                  _mm_set1_pd(100000.0))),
                _mm_div_pd(_mm_mul_pd(vVector, _mm_set1_pd(58060.0)),
                           _mm_set1_pd(100000.0)));
            const __m128d b = _mm_add_pd(
                y, _mm_div_pd(_mm_mul_pd(uVector, _mm_set1_pd(203211.0)),
                              _mm_set1_pd(100000.0)));

            double red[2];
            double green[2];
            double blue[2];
            _mm_storeu_pd(red, r);
            _mm_storeu_pd(green, g);
            _mm_storeu_pd(blue, b);

            for (uint lane = 0; lane < 2; ++lane)
            {
                const uint pixel = i + lane;
                outRow[pixel * 4 + 0] = static_cast<unsigned char>(red[lane]);
                outRow[pixel * 4 + 1] = static_cast<unsigned char>(green[lane]);
                outRow[pixel * 4 + 2] = static_cast<unsigned char>(blue[lane]);
                outRow[pixel * 4 + 3] = aRow ? aRow[pixel] : 255;
            }
        }
#endif

        for (; i < width; ++i)
        {
            const double y = yRow[i];
            const double u = uRow[i / 2] - 128;
            const double v = vRow[i / 2] - 128;

            const double r = y + 113983 * v / 100000;
            const double g = y - 39465 * u / 100000 - 58060 * v / 100000;
            const double b = y + 203211 * u / 100000;

            outRow[i * 4 + 0] = static_cast<unsigned char>(r);
            outRow[i * 4 + 1] = static_cast<unsigned char>(g);
            outRow[i * 4 + 2] = static_cast<unsigned char>(b);
            outRow[i * 4 + 3] = aRow ? aRow[i] : 255;
        }
    }

} // namespace

void yuv420ToRGBA(uint width, uint height,
                  const uint8_t *y, const uint8_t *u, const uint8_t *v,
                  unsigned int ystride, unsigned int ustride, unsigned int vstride,
                  unsigned char *out)
{
    for (uint j = 0; j < height; ++j)
    {
        const uint8_t *yRow = y + j * ystride;
        const uint8_t *uRow = u + (j / 2) * ustride;
        const uint8_t *vRow = v + (j / 2) * vstride;
        unsigned char *outRow = out + j * width * 4;
        convertRow(width, yRow, uRow, vRow, nullptr, outRow);
    }
}

void yuv420AToRGBA(uint width, uint height,
                   const uint8_t *y, const uint8_t *u, const uint8_t *v, const uint8_t *a,
                   unsigned int ystride, unsigned int ustride, unsigned int vstride, unsigned int astride,
                   unsigned char *out)
{
    for (uint j = 0; j < height; ++j)
    {
        const uint8_t *yRow = y + j * ystride;
        const uint8_t *uRow = u + (j / 2) * ustride;
        const uint8_t *vRow = v + (j / 2) * vstride;
        const uint8_t *aRow = a + j * astride;
        unsigned char *outRow = out + j * width * 4;
        convertRow(width, yRow, uRow, vRow, aRow, outRow);
    }
}

void yuv420ToRGB(uint width, uint height,
                 const uint8_t *y, const uint8_t *u, const uint8_t *v,
                 unsigned int ystride, unsigned int ustride, unsigned int vstride,
                 unsigned char *out)
{
    for (uint j = 0; j < height; ++j)
    {
        const uint8_t *yRow = y + j * ystride;
        const uint8_t *uRow = u + (j / 2) * ustride;
        const uint8_t *vRow = v + (j / 2) * vstride;
        unsigned char *outRow = out + j * width * 4;
        convertRow(width, yRow, uRow, vRow, nullptr, outRow);
    }
}

// Y в альфу компоненту буфферов
void yToA(uint width, uint height,
          const uint8_t *a,
          unsigned int ystride,
          unsigned char *out)
{
    // CODE HERE!!!
}

/*

R = Y + 113983 * (V - 128) / 100000;
G = Y - 39465 * (U - 128) / 100000 - 58060 * (V - 128) / 100000;
B = Y + 203211 * (U - 128) / 100000;
Y = 299 * R / 1000 + 587 * G / 1000 + 114 * B / 1000;
U = -14713 * R / 100000 - 28886 * G / 100000 + 436 * B / 1000 + 128;
V = 615 * R / 1000 - 51499 * G / 100000 - 10001 * B / 100000 + 128

Для вычислений лучше домножить на 100000 и потом делить на 100000, чтобы не было потери точности!!!
*/
