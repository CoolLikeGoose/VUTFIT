/**
 * IZG 23/24L - Computer Graphics Principles - BUT FIT
 * Lab 01 - Color Space Reduction
 *
 * Copyright (c) 2022-23 by Michal Vlnas, ivlnas@fit.vutbr.cz
 *               2023-24 by Tibor Kubik, ikubik@fit.vut.cz
 *
 * This template is for study purposes only, distribution of the code is prohibited without the author's knowledge.
 */

#ifndef _BASE_H__
#define _BASE_H__

#include <random>
#include <cmath>
#include <iostream>
#include <SDL.h>
#include <string>

template<typename ...Args>
inline void IZG_INFO(Args && ...args)
{
    std::cout << "Info: ";
    (std::cout << ... << args);
    std::cout << std::endl;
}

/// Frame buffer element
struct RGB
{
    RGB() = default;
    RGB(uint8_t a) : r(a), g(a), b(a) { }
    RGB(uint8_t a, uint8_t b, uint8_t c) : r(a), g(b), b(c) { }

    uint8_t r;
    uint8_t g;
    uint8_t b;

    friend std::ostream& operator<<(std::ostream& os, RGB const& a)
    {
        std::cout << "[" << std::to_string(a.r)
                  << ", " << std::to_string(a.g) << ", "
                  << std::to_string(a.b) << "]";
        return os;
    }
};

/// Constant colors
const RGB COLOR_WHITE = RGB { 255, 255, 255 };
const RGB COLOR_BLACK = RGB { 0, 0, 0 };
const RGB COLOR_RED = RGB { 255, 0, 0 };
const RGB COLOR_GREEN = RGB { 0, 255, 0 };
const RGB COLOR_BLUE = RGB { 0, 0, 255 };

/// Matrix size constant for ordered dithering
const uint32_t m_side = 4;

/// Global config
struct Config
{
    /// Frame buffer width
    unsigned w;
    /// Frame buffer height
    unsigned h;
    bool running;
};

/// Image transforming class
class ImageTransform
{
public:
    explicit ImageTransform(Config* _cfg) : gen(rd()),
                                   distribution(std::uniform_int_distribution<uint16_t>(0, 255)),
                                   cfg(_cfg)
    {
        frame_buffer = new RGB[cfg->w * cfg->h];
        for (uint32_t i = 0; i < cfg->w * cfg->h; i++)
            frame_buffer[i] = COLOR_WHITE;

        read_only_frame_buffer = new RGB[cfg->w * cfg->h];
        for (uint32_t i = 0; i < cfg->w * cfg->h; i++)
            read_only_frame_buffer[i] = COLOR_WHITE;
    }

    ~ImageTransform() { delete [] frame_buffer; delete [] read_only_frame_buffer; }

    RGB getPixel(uint32_t x, uint32_t y)
    {
        return frame_buffer[y * cfg->w + x];
    }

    // Use ONLY in _evaluateThreshold_ method!
    uint8_t getPixelGrayScale(uint32_t x, uint32_t y)
    {
        return read_only_frame_buffer[y * cfg->w + x].r;
    }

    void setPixel(uint32_t x, uint32_t y, RGB color)
    {
        frame_buffer[y * cfg->w + x] = color;
    }

    RGB* getFrameBuffer() { return frame_buffer; }

    bool loadImage(std::string const& file);
    bool saveImage(std::string const& file);

    /// Student defined methods: to grayscale
    void grayscale();

    /// Student defined methods: to black&white, pixel-based
    void threshold();
    void randomModulation();

    /// Student defined methods: to black&white, neighborhood-based
    void errorDistribution();
    void toneDependentErrorDistribution();

    /// Already implemented example
    void randomDithering();

protected:
    int getRandomFromRange(int lowerBound, int upperBound)
    {   
        std::uniform_int_distribution<int> distr(lowerBound, upperBound);
        return distr(gen);
    }

    uint16_t getRandom()
    {
        return distribution(gen);
    }

    void updatePixelWithError(uint32_t x, uint32_t y, float err);

private:
    RGB* frame_buffer;
    RGB* read_only_frame_buffer;

    /// Random number generators
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<uint16_t> distribution;

public:
    Config* cfg;
};

#endif // _BASE_H__
