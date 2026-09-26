#include "WebMYUVConverter.h"

namespace xgen
{
    namespace video
    {
        void yuv420ToRGBA(uint width, uint height,
                          const uint8_t *y, const uint8_t *u, const uint8_t *v,
                          unsigned int ystride, unsigned int ustride, unsigned int vstride,
                          unsigned char *out)
        {
            for (uint h = 0; h < height; h++)
            {
                uint y_offset = h * ystride;
                uint u_offset = (h / 2) * ustride;
                uint v_offset = (h / 2) * vstride;
                uint out_offset = h * width * 4;

                for (uint w = 0; w < width; w++)
                {
                    uint y_val = y[y_offset + w];
                    uint u_val = u[u_offset + w / 2];
                    uint v_val = v[v_offset + w / 2];

                    int u_centered = (int)u_val - 128;
                    int v_centered = (int)v_val - 128;

                    int r = (int)y_val + (113983 * v_centered) / 100000;
                    int g = (int)y_val - (39465 * u_centered) / 100000 - (58060 * v_centered) / 100000;
                    int b = (int)y_val + (203211 * u_centered) / 100000;

                    r = (r < 0) ? 0 : (r > 255) ? 255
                                                : r;
                    g = (g < 0) ? 0 : (g > 255) ? 255
                                                : g;
                    b = (b < 0) ? 0 : (b > 255) ? 255
                                                : b;

                    out[out_offset + w * 4 + 0] = (unsigned char)r;
                    out[out_offset + w * 4 + 1] = (unsigned char)g;
                    out[out_offset + w * 4 + 2] = (unsigned char)b;
                    out[out_offset + w * 4 + 3] = 255;
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
                uint y_offset = h * ystride;
                uint u_offset = (h / 2) * ustride;
                uint v_offset = (h / 2) * vstride;
                uint a_offset = h * astride;
                uint out_offset = h * width * 4;

                for (uint w = 0; w < width; w++)
                {
                    uint y_val = y[y_offset + w];
                    uint u_val = u[u_offset + w / 2];
                    uint v_val = v[v_offset + w / 2];
                    uint a_val = a[a_offset + w];

                    int u_centered = (int)u_val - 128;
                    int v_centered = (int)v_val - 128;

                    int r = (int)y_val + (113983 * v_centered) / 100000;
                    int g = (int)y_val - (39465 * u_centered) / 100000 - (58060 * v_centered) / 100000;
                    int b = (int)y_val + (203211 * u_centered) / 100000;

                    r = (r < 0) ? 0 : (r > 255) ? 255
                                                : r;
                    g = (g < 0) ? 0 : (g > 255) ? 255
                                                : g;
                    b = (b < 0) ? 0 : (b > 255) ? 255
                                                : b;

                    out[out_offset + w * 4 + 0] = (unsigned char)r;
                    out[out_offset + w * 4 + 1] = (unsigned char)g;
                    out[out_offset + w * 4 + 2] = (unsigned char)b;
                    out[out_offset + w * 4 + 3] = (unsigned char)a_val;
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
                uint y_offset = h * ystride;
                uint u_offset = (h / 2) * ustride;
                uint v_offset = (h / 2) * vstride;
                uint out_offset = h * width * 3;

                for (uint w = 0; w < width; w++)
                {
                    uint y_val = y[y_offset + w];
                    uint u_val = u[u_offset + w / 2];
                    uint v_val = v[v_offset + w / 2];

                    int u_centered = (int)u_val - 128;
                    int v_centered = (int)v_val - 128;

                    int r = (int)y_val + (113983 * v_centered) / 100000;
                    int g = (int)y_val - (39465 * u_centered) / 100000 - (58060 * v_centered) / 100000;
                    int b = (int)y_val + (203211 * u_centered) / 100000;

                    r = (r < 0) ? 0 : (r > 255) ? 255
                                                : r;
                    g = (g < 0) ? 0 : (g > 255) ? 255
                                                : g;
                    b = (b < 0) ? 0 : (b > 255) ? 255
                                                : b;

                    out[out_offset + w * 3 + 0] = (unsigned char)r;
                    out[out_offset + w * 3 + 1] = (unsigned char)g;
                    out[out_offset + w * 3 + 2] = (unsigned char)b;
                }
            }
        }

        // Y в альфу компоненту буфферов
        void yToA(uint width, uint height, const uint8_t *a, unsigned int ystride, unsigned char *out)
        {
            for (uint h = 0; h < height; h++)
            {
                uint a_offset = h * ystride;
                uint out_offset = h * width * 4;

                for (uint w = 0; w < width; w++)
                {
                    uint a_val = a[a_offset + w];
                    out[out_offset + w * 4 + 3] = (unsigned char)a_val;
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