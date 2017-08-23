#ifndef IR_THERMO_DISPLAY_H
#define IR_THERMO_DISPLAY_H

#define FLT_MAX     3.40282347E+38F

#include "U8g2lib.h"
#include "my_config.h"

static const uint8_t AVERAGE_BITMAP[8] U8X8_PROGMEM = {0xbc, 0x42, 0xa1, 0x91, 0x89, 0x85, 0x42, 0x3d};
static const uint8_t CONT_SCAN_BITMAP[8] U8X8_PROGMEM = {0xbc, 0x42, 0xa1, 0x91, 0x89, 0x85, 0x42, 0x3d};
static const uint8_t LASER_BITMAP[10] U8X8_PROGMEM = {0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x3c, 0x18, 0x3c, 0x18};
static const uint8_t BG_LIGHT_BITMAP[7] U8X8_PROGMEM = {0x08, 0x08, 0x08, 0x49, 0x2a, 0x1c, 0x3e};

class Display {
private: // constants
    static const uint8_t FONT_HEIGHT = 7;
    static const uint8_t WIDTH = 84, HEIGHT = 48;
    static const uint8_t LAST_MEASUREMENT_X = 0, LAST_MEASUREMENT_Y = HEIGHT;
    static const uint8_t AVG_MEASUREMENT_X = 28, AVG_MEASUREMENT_Y = HEIGHT;

    static const uint8_t CONT_SCAN_IND_X = 56, CONT_SCAN_IND_Y = HEIGHT;
    static const uint8_t LASER_IND_X = 63, LASER_IND_Y = HEIGHT;
    static const uint8_t BG_IND_X = 70, BG_IND_Y = HEIGHT;

    static const uint8_t GRAPH_X = 0, GRAPH_WIDTH = 84, GRAPH_HEIGHT = 36, GRAPH_Y = HEIGHT - GRAPH_HEIGHT;
    static const uint8_t GRAPH_BOTTOM_MARGIN = 8, GRAPH_LEFT_MARGIN = 8;
    static const uint8_t GRAPH_LEFT_START = GRAPH_X + GRAPH_LEFT_MARGIN;


    static const uint8_t
            BITMAP_WIDTH = 8,
            BITMAP_AVERAGE_HEIGHT = 8,
            BITMAP_CONT_SCAN_HEIGHT = 8,
            BITMAP_LASER_HEIGHT = 10,
            BITMAP_BG_LIGHT_HEIGHT = 7;

private:
    U8G2_PCD8544_84X48_1_4W_HW_SPI *display;

    void renderGraph(float *input, uint32_t count, float max, float min) {
        // build x-axis
        uint32_t logDuration = count * CONTINUOUS_MEASUREMENT_INTERVAL_SECS;
        uint32_t logMinutes = logDuration / 60;
        uint32_t logSeconds = logDuration % 60;
        display->drawHLine(GRAPH_LEFT_START, GRAPH_Y + FONT_HEIGHT, GRAPH_WIDTH - GRAPH_LEFT_MARGIN);
        display->setCursor(GRAPH_LEFT_START, GRAPH_Y);
        display->print(logMinutes);
        display->print(":");
        display->print(logSeconds);

        display->setCursor(GRAPH_X + GRAPH_WIDTH - 8 * 5, GRAPH_Y);
        display->print("00:00");

        // build y-axis
        display->setCursor(GRAPH_X, 7);
        display->print(ceil(max));

        display->setCursor(GRAPH_X, (GRAPH_Y - GRAPH_BOTTOM_MARGIN) / 2);
        display->print(lround(min + (max - min) / 2));

        display->setCursor(GRAPH_X, GRAPH_Y - GRAPH_BOTTOM_MARGIN);
        display->print(floor(min));
    }

    void renderContinuousScanIndicator(bool enabled) {
        if (enabled)
            display->drawXBMP(CONT_SCAN_IND_X, CONT_SCAN_IND_Y, BITMAP_WIDTH, BITMAP_CONT_SCAN_HEIGHT, CONT_SCAN_BITMAP);
    }

    void renderLaserIndicator(bool enabled) {
        if (!enabled)
            display->drawXBMP(LASER_IND_X, LASER_IND_Y, BITMAP_WIDTH, BITMAP_LASER_HEIGHT, LASER_BITMAP);
    }

    void renderBackgroundLightIndicator(bool enabled) {
        if (!enabled)
            display->drawXBMP(BG_IND_X, BG_IND_Y, BITMAP_WIDTH, BITMAP_BG_LIGHT_HEIGHT, BG_LIGHT_BITMAP);
    }

    void renderLastMeasurement(double measurementValue) {
        display->setCursor(LAST_MEASUREMENT_X, LAST_MEASUREMENT_Y);
        display->print(measurementValue);
    }

    void renderAverageMeasurement(double averageValue) {

        display->setCursor(AVG_MEASUREMENT_X, AVG_MEASUREMENT_Y);
        display->print("Ø");
        display->print(averageValue);
    }

public:
    void update(
            float *input,
            size_t count,
            bool continuousScanEnabled,
            bool backgroundLightEnabled,
            bool laserEnabled) {

        // calculate the average once
        float sum = 0, max = -FLT_MAX, min = FLT_MAX;
        for (int i = 0; i < count; i++) {
            max = max >= input[i] ? max : input[i];
            min = min <= input[i] ? min : input[i];
            sum += input[i];
        }

        display->firstPage();
        do {
            renderGraph(input, count, max, min);
            renderAverageMeasurement(sum / count);
            renderLastMeasurement(input[count - 1]);
            renderLaserIndicator(laserEnabled);
            renderContinuousScanIndicator(continuousScanEnabled);
            renderBackgroundLightIndicator(backgroundLightEnabled);
        } while ((bool) display->nextPage());
    }

    /// Basically find out how many points we can display
    size_t getGraphHorizontalResolution() {
        return WIDTH - GRAPH_LEFT_MARGIN;
    }

    Display() {
        display = new U8G2_PCD8544_84X48_1_4W_HW_SPI(U8G2_R2, SPI_CS_DISPLAY, SPI_DC, SPI_RESET);
        display->begin();
        display->enableUTF8Print();
        display->setFont(u8g2_font_profont22_mn);
    }
};

#endif