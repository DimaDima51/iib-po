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
            double U = uRow[i / 2] - 128;
            double V = vRow[i / 2] - 128;

            double R = Y + 113983 * V / 100000;
            double G = Y - 39465 * U / 100000 - 58060 * V / 100000;
            double B = Y + 203211 * U / 100000;

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
            double U = uRow[i / 2] - 128;
            double V = vRow[i / 2] - 128;

            double R = Y + 113983 * V / 100000;
            double G = Y - 39465 * U / 100000 - 58060 * V / 100000;
            double B = Y + 203211 * U / 100000;

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
            double U = uRow[i / 2] - 128;
            double V = vRow[i / 2] - 128;

            double R = Y + 113983 * V / 100000;
            double G = Y - 39465 * U / 100000 - 58060 * V / 100000;
            double B = Y + 203211 * U / 100000;

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

R = Y + 113983 * (V - 128) / 100000;
G = Y - 39465 * (U - 128) / 100000 - 58060 * (V - 128) / 100000;
B = Y + 203211 * (U - 128) / 100000;
Y = 299 * R / 1000 + 587 * G / 1000 + 114 * B / 1000;
U = -14713 * R / 100000 - 28886 * G / 100000 + 436 * B / 1000 + 128;
V = 615 * R / 1000 - 51499 * G / 100000 - 10001 * B / 100000 + 128

Для вычислений лучше домножить на 100000 и потом делить на 100000, чтобы не было потери точности!!!
*/
