/*
 * ledStrip.h
 *
 *  Created on: Jul 15, 2022
 *      Author: Sebastian Mueller
 */

#ifndef INC_LEDSTRIP_H_
#define INC_LEDSTRIP_H_

#include "main.h"
#include <string.h>
#include <stdint.h>


/*================================================================================================
 * CONFIGURATION OPTIONS
 *================================================================================================*/
#define NUM_PIXELS 300
#define NUM_PIXELS_REAR 150
#define USING_BRIGHTNESS_FUNCTIONALITY true

/*================================================================================================
 * DEFINES
 *================================================================================================*/
#define BITS_PER_PIXEL 24
// (24 +) - When DMA is started the first couple values can be
//			unreliable. Because of this we pad the beginning of the
//			array with 0s to ensure the actual data transfer is stable
// (+1)	  - For reset frame. By setting equal to 0 ensures that once
//		    DMA transfer has completed the bus is in a low state.
#define ARRAY_SIZE 24 + (NUM_PIXELS * BITS_PER_PIXEL) + 1
// Using Timer3 - APB1 @96MHz input. Need 800kHz for leds.Prescaler of 120 gives 96MHz/120 = 800kHz
//				  Need ~25% duty cycle for 0 and ~75% duty cycle for 1. Therefore
//				  0.25*120 = 30 for logical 0	0.75*120 = 90 for logical 1
//WS2815
#define ONE_HIGH_TIME 84
#define ZERO_HIGH_TIME 28
//WS2812b
//#define ONE_HIGH_TIME 76
//#define ZERO_HIGH_TIME 36
#define RED_INDEX 0
#define GREEN_INDEX 1
#define BLUE_INDEX 2
#define BRIGHTNESS_INDEX 3

/*================================================================================================
 * EXTERNALS
 *================================================================================================*/
// DMA array
extern uint16_t pwmData[];

/*	Contains raw color data
 *  [0] = RED
 *  [1] = GREEN
 *  [2] = BLUE
 *  [3] = BRIGHTNESS
 */
extern uint8_t ledData[][4];

/*================================================================================================
 * ENUMERATED TYPES
 *================================================================================================*/
typedef enum
{
	RED_TO_RED_GREEN = 0,
	RED_GREEN_TO_GREEN,
	GREEN_TO_GREEN_BLUE,
	GREEN_BLUE_TO_BLUE,
	BLUE_TO_BLUE_RED,
	BLUE_RED_TO_RED,
	NUM_COLOR_PHASES
} colorPhase_E;

/*================================================================================================
 * FUNCTIONS
 *================================================================================================*/
/*
 * @brief Resets the LED strip to {0, 0, 0} for all colors.
 */
void clear();

/*
 * @brief Update the led strip with the new colors in the array.
 */
void write();

/*
 * @brief Sets a specific pixel color but leaves brightness unaffected.
 *
 * @param ledNum 		Which LED's color to set. Indexed starting at 0.
 * @param red			What value to assign to the red channel. 			Range of 0-255.
 * @param green			What value to assign to the green channel. 			Range of 0-255.
 * @param blue			What value to assign to the blue channel. 			Range of 0-255.
 */
void setPixelColor(uint32_t ledNum, uint8_t red, uint8_t green, uint8_t blue);

/*
 * @brief Sets a specific pixel color and brightness.
 *
 * @param ledNum 		Which LED's color to set. Indexed starting at 0.
 * @param red			What value to assign to the red channel. 			Range of 0-255.
 * @param green			What value to assign to the green channel. 			Range of 0-255.
 * @param blue			What value to assign to the blue channel. 			Range of 0-255.
 * @param brightness	What value to assign to the brightness channel. 	Range of 0-255.
 */
void setPixel(uint32_t ledNum, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness);

/*
 * @brief Sets the strip to a specific color but leaves brightness unaffected.
 *
 * @param red			What value to assign to the red channel. 			Range of 0-255.
 * @param green			What value to assign to the green channel. 			Range of 0-255.
 * @param blue			What value to assign to the blue channel. 			Range of 0-255.
 */
void setStripColor(uint8_t red, uint8_t green, uint8_t blue);

/*
 * @brief Sets the strips color and brightness
 *
 * @param red			What value to assign to the red channel. 			Range of 0-255.
 * @param green			What value to assign to the green channel. 			Range of 0-255.
 * @param blue			What value to assign to the blue channel. 			Range of 0-255.
 * @param brightness	What value to assign to the brightness channel. 	Range of 0-255.
 */
void setStrip(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness);

/*
 * @brief Update the brightness of the entire led strip.
 *
 * @param brightness	What value to assign to the brightness channel. 	Range of 0-255.
 */
void setStripBrightness(uint32_t brightness);

/*
 * @brief Update the brightness of an individual LED.
 *
 * @param ledNum		What LED's brightness channel to change.
 * @param brightness	What value to assign to the brightness channel. 	Range of 0-255.
 */
void setPixelBrightness(uint32_t ledNum, uint32_t brightness);

/*
 * @brief Map an input from one range to another.
 *
 * @param x				Input value to be mapped.
 * @param inputMin		Minimum value of input range.
 * @param inputMax		Maximum value of input range.
 * @param outputMin		Minimum value of output range.
 * @param outputMax		Maximum value of output range.
 *
 * @return 				Returns x mapped from input range to output range.
 */
int32_t map(int32_t x, int32_t inputMin, int32_t inputMax, int32_t outputMin, int32_t outputMax);

/*
 * @brief Show a rainbow pattern along the entire strip.
 *
 * @param patternStartIndex		Which led to start the rainbow pattern on. Starts on red
 */
void generateRainbow(uint32_t rainbowArr[NUM_PIXELS][3]);
void runRainbow(uint32_t rainbowArr[NUM_PIXELS][3]);
void shiftRainbowUP(uint32_t rainbowArr[NUM_PIXELS][3]);
void shiftRainbowDOWN(uint32_t rainbowArr[NUM_PIXELS][3]);

void rainbow(uint32_t patternStartIndex);

/*
 * @brief Light up a percent of the led strip with a certain color
 *
 * @param percent		How much of the strip should be colored. 0-100
 * @param red			What value to assign to the red channel. 0-255.
 * @param green			What value to assign to the green channel. 0-255.
 * @param blue			What value to assign to the blue channel. 0-255.
 */
void drawProgressBar(uint32_t percent, uint8_t red, uint8_t green, uint8_t blue);
void drawColorProgressBar(uint32_t percent, uint8_t redStart, uint8_t greenStart, uint8_t blueStart,
											uint8_t redMid1, uint8_t greenMid1, uint8_t blueMid1,
											uint8_t redMid2, uint8_t greenMid2, uint8_t blueMid2,
											uint8_t redEnd, uint8_t greenEnd, uint8_t blueEnd);

void rpmProgressBar(uint32_t percent, uint8_t redStart, uint8_t greenStart, uint8_t blueStart,
											uint8_t redEnd, uint8_t greenEnd, uint8_t blueEnd);
void rpmProgressBarRear(uint32_t percent, uint8_t redStart, uint8_t greenStart, uint8_t blueStart,
											uint8_t redEnd, uint8_t greenEnd, uint8_t blueEnd);


//void drawColorProgressBar(uint32_t percent, uint8_t redStart, uint8_t greenStart, uint8_t blueStart,
//											uint8_t redEnd, uint8_t greenEnd, uint8_t blueEnd);
void drawSmoothRainbow(uint32_t start);

void dottedLine(uint32_t stepSize, uint32_t offset, uint8_t red, uint8_t green, uint8_t blue);

#endif /* INC_LEDSTRIP_H_ */
