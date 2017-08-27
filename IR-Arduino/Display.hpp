#ifndef IR_THERMO_DISPLAY_H
#define IR_THERMO_DISPLAY_H

#define FLT_MAX     3.40282347E+38F

#include "U8g2lib.h"
#include "my_config.h"

// "test" defines :(
#define TESTING_DISABLE_HORIZONTAL_MARKING
#define TESTING_DISABLE_VERTICAL_MARKING
#define TESTING_DISABLE_FRAME
#define TESTING_DISABLE_GRAPH
//#define TESTING_DISABLE_AVG_MEASUREMENT
//#define TESTING_DISABLE_LAST_MEASUREMENT
//#define TESTING_DISABLE_LASER_INDICATOR
//#define TESTING_DISABLE_CONT_SCAN_INDICATOR
//#define TESTING_DISABLE_BACKGROUND_LIGHT_INDICATOR
//#define TESTING_DISABLE_MEASUREMENT_DIVIDERS

// bitmaps so we can avoid loading the entire UTF8 symbol font
static const uint8_t CONT_SCAN_BITMAP[8] U8X8_PROGMEM = {0xdf, 0x91, 0x89, 0x85, 0xa1, 0x91, 0x89, 0xfb};
static const uint8_t LASER_BITMAP[10] U8X8_PROGMEM = {0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x3c, 0x18, 0x3c, 0x18};
static const uint8_t BG_LIGHT_BITMAP[7] U8X8_PROGMEM = {0x55, 0x3e, 0x63, 0x22, 0x63, 0x3e, 0x55};

class Display {
private: // constants
    static const uint8_t FONT_HEIGHT = 12;

    static const uint8_t DISPLAY_WIDTH = 84, DISPLAY_HEIGHT = 48, DISPLAY_BOTTOM = DISPLAY_HEIGHT;

    static const uint8_t LAST_MEASUREMENT_X = 0, LAST_MEASUREMENT_Y = DISPLAY_BOTTOM - FONT_HEIGHT / 4;
    static const uint8_t AVG_MEASUREMENT_X = 28, AVG_MEASUREMENT_Y = DISPLAY_BOTTOM - FONT_HEIGHT / 4;

    static const uint8_t GRAPH_HEIGHT = 36;

    // definitions for the graph decorations
    static const uint8_t
            GRAPH_LEGEND_WIDTH = 8, GRAPH_LEGEND_HEIGHT = 8,
            GRAPH_LEGEND_X = 0, GRAPH_LEGEND_Y = GRAPH_HEIGHT + GRAPH_LEGEND_HEIGHT,
            GRAPH_LEGEND_RIGHT = GRAPH_LEGEND_X + GRAPH_LEGEND_WIDTH,
            GRAPH_LEGEND_TOP = GRAPH_LEGEND_HEIGHT + GRAPH_LEGEND_Y;

    // definitions for the graph itself
    static const uint8_t
            GRAPH_X = GRAPH_LEGEND_X + GRAPH_LEGEND_WIDTH,
            GRAPH_Y = GRAPH_LEGEND_Y - GRAPH_LEGEND_HEIGHT,
            GRAPH_WIDTH = DISPLAY_WIDTH - GRAPH_LEGEND_WIDTH,
            GRAPH_RIGHT = GRAPH_X + GRAPH_WIDTH,
            GRAPH_TOP = GRAPH_Y + GRAPH_HEIGHT;

    static const uint8_t
            BITMAP_WIDTH = 8,
            BITMAP_CONT_SCAN_HEIGHT = 8,
            BITMAP_LASER_HEIGHT = 10,
            BITMAP_BG_LIGHT_HEIGHT = 7;

    static const uint8_t CONT_SCAN_IND_X = 56, CONT_SCAN_IND_Y = DISPLAY_BOTTOM - BITMAP_CONT_SCAN_HEIGHT;
    static const uint8_t LASER_IND_X = 63, LASER_IND_Y = DISPLAY_BOTTOM - BITMAP_LASER_HEIGHT;
    static const uint8_t BG_IND_X = 70, BG_IND_Y = DISPLAY_BOTTOM - BITMAP_BG_LIGHT_HEIGHT;

private:
    U8G2_PCD8544_84X48_F_4W_HW_SPI *display;

    void renderBottomMeasurementDividers() {
        uint8_t dividerTop = DISPLAY_BOTTOM - FONT_HEIGHT;
        display->drawHLine(0, dividerTop, DISPLAY_WIDTH);
        display->drawVLine(AVG_MEASUREMENT_X - 2, dividerTop, FONT_HEIGHT);
        display->drawVLine(CONT_SCAN_IND_X - 2, dividerTop, FONT_HEIGHT);
    }

    void renderGraph(const float *input, size_t count, float min, float max) {
        // calculate vertical scale
        float verticalScale = GRAPH_HEIGHT / (max - min);

        for (size_t i = 0; i < count; i++) {
            float val = input[count - 1 - i];
            uint8_t x = static_cast<uint8_t >(GRAPH_RIGHT - i);
            uint8_t y = static_cast<uint8_t>(GRAPH_Y + (val - min) * verticalScale);
            display->drawPixel(x, y);
        }
    }

    void renderFrame() {
        // line from top to bottom
        display->drawVLine(GRAPH_X, GRAPH_Y, GRAPH_HEIGHT);

        // draw line for left to right
        display->drawHLine(GRAPH_X, GRAPH_Y, GRAPH_WIDTH);
    }

    void renderYMarks(float min, float max) {
        // print max value at the top
        display->setCursor(GRAPH_LEGEND_X, GRAPH_LEGEND_TOP - FONT_HEIGHT);
        display->print(ceil(max));

        // avg
        display->setCursor(GRAPH_LEGEND_X, GRAPH_LEGEND_Y + GRAPH_LEGEND_HEIGHT / 2);
        display->print(lround(min + (max - min) / 2));

        // min
        display->setCursor(GRAPH_LEGEND_X, GRAPH_LEGEND_Y);
        display->print(floor(min));
    }

    void renderXMarks(size_t count) {
        // calculate for how many minutes/seconds the logger was running
        uint32_t logDuration = count * CONTINUOUS_MEASUREMENT_INTERVAL_SECS;
        uint32_t logMinutes = logDuration / 60;
        uint32_t logSeconds = logDuration % 60;

        // draw full log duration on left
        display->setCursor(GRAPH_LEGEND_X, GRAPH_LEGEND_Y);
        display->print(logMinutes);
        display->print(":");
        display->print(logSeconds);

        // draw "now" on the right
        display->setCursor(GRAPH_LEGEND_RIGHT - 8 * 5 /*subtract approx width of characters*/, GRAPH_LEGEND_Y);
        display->print("00:00");
    }

    void renderContinuousScanIndicator(bool enabled) {
        if (!enabled)
            display->drawXBMP(
                    CONT_SCAN_IND_X,
                    CONT_SCAN_IND_Y,
                    BITMAP_WIDTH,
                    BITMAP_CONT_SCAN_HEIGHT,
                    CONT_SCAN_BITMAP);
    }

    void renderLaserIndicator(bool enabled) {
        if (!enabled)
            display->drawXBMP(
                    LASER_IND_X,
                    LASER_IND_Y,
                    BITMAP_WIDTH,
                    BITMAP_LASER_HEIGHT,
                    LASER_BITMAP);
    }

    void renderBackgroundLightIndicator(bool enabled) {
        if (!enabled)
            display->drawXBMP(
                    BG_IND_X,
                    BG_IND_Y,
                    BITMAP_WIDTH,
                    BITMAP_BG_LIGHT_HEIGHT,
                    BG_LIGHT_BITMAP);
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
            const float *input,
            size_t count,
            bool continuousScanEnabled,
            bool backgroundLightEnabled,
            bool laserEnabled) {

        // calculate the average once
        float sum = 0, max = -FLT_MAX, min = FLT_MAX;
        for (size_t i = 0; i < count; i++) {
            max = max >= input[i] ? max : input[i];
            min = min <= input[i] ? min : input[i];
            sum += input[i];
        }

        display->firstPage();
        do {
#ifndef TESTING_DISABLE_HORIZONTAL_MARKING
            renderXMarks(count);
#endif
#ifndef TESTING_DISABLE_VERTICAL_MARKING
            renderYMarks(min, max);
#endif
#ifndef TESTING_DISABLE_FRAME
            renderFrame();
#endif
#ifndef TESTING_DISABLE_GRAPH
            renderGraph(input, count, max, min);
#endif
#ifndef TESTING_DISABLE_AVG_MEASUREMENT
            renderAverageMeasurement(sum / count);
#endif
#ifndef TESTING_DISABLE_LAST_MEASUREMENT
            renderLastMeasurement(input[count - 1]);
#endif
#ifndef TESTING_DISABLE_LASER_INDICATOR
            renderLaserIndicator(laserEnabled);
#endif
#ifndef TESTING_DISABLE_CONT_SCAN_INDICATOR
            renderContinuousScanIndicator(continuousScanEnabled);
#endif
#ifndef TESTING_DISABLE_BACKGROUND_LIGHT_INDICATOR
            renderBackgroundLightIndicator(backgroundLightEnabled);
#endif
#ifndef TESTING_DISABLE_MEASUREMENT_DIVIDERS
            renderBottomMeasurementDividers();
#endif
        } while ((bool) display->nextPage());
    }

    /// Basically find out how many points we can display
    size_t getGraphHorizontalResolution() {
        return GRAPH_WIDTH;
    }

    Display() {
        display = new U8G2_PCD8544_84X48_F_4W_HW_SPI(U8G2_R0, SPI_CS_DISPLAY, SPI_DC, SPI_RESET);
        display->begin();
        display->enableUTF8Print();
        //display->setFont(u8g2_font_7x14_tr);
        display->setFont(u8g2_font_micro_tr);
    }
};

#endif