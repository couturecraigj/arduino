#include <Adafruit_NeoPixel.h>

/**
 * Make it so you can adjust color brightness per pixel
 */
class NeoPixelColor {
private:
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    // Helper function to scale color values by a brightness factor
    uint8_t scaleColor(uint8_t color, float factor) {
        return static_cast<uint8_t>(color * factor);
    }

public:
    // Constructor: initializes the color with red, green, and blue components
    NeoPixelColor(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {}

    // Adjust brightness and return the color as a uint32_t
    uint32_t adjustBrightness(uint8_t brightnessPercentage) {
        if (brightnessPercentage > 100) {
            brightnessPercentage = 100; // Cap the brightness to 100%
        }

        float brightnessFactor = brightnessPercentage / 100.0f;

        uint8_t adjRed = scaleColor(red, brightnessFactor);
        uint8_t adjGreen = scaleColor(green, brightnessFactor);
        uint8_t adjBlue = scaleColor(blue, brightnessFactor);

        return Adafruit_NeoPixel::Color(adjRed, adjGreen, adjBlue);
    }
};
