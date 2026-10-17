#include "WebMYUVConverter.h"

void yuv420ToRGBA(uint width, uint height,
                  const uint8_t* y, const uint8_t* u, const uint8_t* v,
                  unsigned int ystride, unsigned int ustride, unsigned int vstride,
                  unsigned char* out) {
    #pragma clang loop vectorize(enable)
    for (uint j = 0; j < height; ++j) {
        const uint8_t* yRow = y + j * ystride;
        const uint8_t* uRow = u + (j / 2) * ustride;
        const uint8_t* vRow = v + (j / 2) * vstride;
        unsigned char* outRow = out + j * width * 4;
        #pragma clang loop vectorize(enable)
        for (uint i = 0; i < width; ++i) {
            double Y = yRow[i];
            double U = uRow[i / 2] - 128.0;
            double V = vRow[i / 2] - 128.0;

            double R = Y + 1.13983 * V;
            double G = Y - 0.39465 * U - 0.58060 * V;
            double B = Y + 2.03211 * U;

            outRow[i * 4 + 0] = (unsigned char)R;
            outRow[i * 4 + 1] = (unsigned char)G;
            outRow[i * 4 + 2] = (unsigned char)B;
            outRow[i * 4 + 3] = 255;
        }
    }
}

void yuv420AToRGBA(uint width, uint height,
                   const uint8_t* y, const uint8_t* u, const uint8_t* v, const uint8_t* a,
                   unsigned int ystride, unsigned int ustride, unsigned int vstride, unsigned int astride,
                   unsigned char* out) {
    #pragma clang loop vectorize(enable)
    for (uint j = 0; j < height; ++j) {
        const uint8_t* yRow = y + j * ystride;
        const uint8_t* uRow = u + (j / 2) * ustride;
        const uint8_t* vRow = v + (j / 2) * vstride;
        const uint8_t* aRow = a + j * astride;
        unsigned char* outRow = out + j * width * 4;
        #pragma clang loop vectorize(enable)
        for (uint i = 0; i < width; ++i) {
            double Y = yRow[i];
            double U = uRow[i / 2] - 128.0;
            double V = vRow[i / 2] - 128.0;

            double R = Y + 1.13983 * V;
            double G = Y - 0.39465 * U - 0.58060 * V;
            double B = Y + 2.03211 * U;

            outRow[i * 4 + 0] = (unsigned char)R;
            outRow[i * 4 + 1] = (unsigned char)G;
            outRow[i * 4 + 2] = (unsigned char)B;
            outRow[i * 4 + 3] = aRow[i];
        }
    }
}

void yuv420ToRGB(uint width, uint height,
                 const uint8_t* y, const uint8_t* u, const uint8_t* v,
                 unsigned int ystride, unsigned int ustride, unsigned int vstride,
                 unsigned char* out) {
    #pragma clang loop vectorize(enable)
    for (uint j = 0; j < height; ++j) {
        const uint8_t* yRow = y + j * ystride;
        const uint8_t* uRow = u + (j / 2) * ustride;
        const uint8_t* vRow = v + (j / 2) * vstride;
        unsigned char* outRow = out + j * width * 4;
        #pragma clang loop vectorize(enable)
        for (uint i = 0; i < width; ++i) {
            double Y = yRow[i];
            double U = uRow[i / 2] - 128.0;
            double V = vRow[i / 2] - 128.0;

            double R = Y + 1.13983 * V;
            double G = Y - 0.39465 * U - 0.58060 * V;
            double B = Y + 2.03211 * U;

            outRow[i * 4 + 0] = (unsigned char)R;
            outRow[i * 4 + 1] = (unsigned char)G;
            outRow[i * 4 + 2] = (unsigned char)B;
            outRow[i * 4 + 3] = 255;
        }
    }
}

// Y в альфу компоненту буфферов
void yToA(uint width, uint height,
          const uint8_t* a,
          unsigned int ystride,
          unsigned char* out){
    // CODE HERE!!!
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

// Уже сделано : flamegraph без целых чисел
// Потом flamegraph с целыми числами
// Потом с SIMD приколами