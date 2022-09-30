/*
 * ledStrip.c
 *
 *  Created on: Jul 15, 2022
 *      Author: Sebastian Mueller
 */

#include "ledStrip.h"

extern TIM_HandleTypeDef htim3;


// DMA array
uint16_t pwmData[ARRAY_SIZE]	= {0};

/*	Contains raw color data
 *  [0] = RED
 *  [1] = GREEN
 *  [2] = BLUE
 *  [3] = BRIGHTNESS
 */
uint8_t  ledData[NUM_PIXELS][4]	= {0};


void clear()
{
	memset(ledData, 0, sizeof(ledData[0][0]) * NUM_PIXELS * 4);
	for (int i = 0; i < NUM_PIXELS; i++)
	{
		ledData[i][BRIGHTNESS_INDEX] = 255;
	}
}

void write()
{
	for (int colorPhasePixelIndex = 0; colorPhasePixelIndex < NUM_PIXELS; colorPhasePixelIndex++)
	{
		uint32_t red  = 	(ledData[colorPhasePixelIndex][RED_INDEX] *
							 ledData[colorPhasePixelIndex][BRIGHTNESS_INDEX]) / 255;
		uint32_t green = 	(ledData[colorPhasePixelIndex][GREEN_INDEX] *
				 	 	 	 ledData[colorPhasePixelIndex][BRIGHTNESS_INDEX]) / 255;
		uint32_t blue  = 	(ledData[colorPhasePixelIndex][BLUE_INDEX] *
	 	 	 	 	 	 	 ledData[colorPhasePixelIndex][BRIGHTNESS_INDEX]) / 255;
		uint32_t color = 	(green << 16) | (red <<  8) | (blue);
		uint32_t mask  = 	 0x00000001 << (BITS_PER_PIXEL - 1);

		for (int i = 0; i < BITS_PER_PIXEL; i++)
		{
			// Where in the pwm array we are. +24 due to front DMA padding
			uint32_t pwmIndex = colorPhasePixelIndex * BITS_PER_PIXEL + i + 24;
			if ((mask >> i) & color)
			{
				// Write 1
				pwmData[pwmIndex] = ONE_HIGH_TIME;
			}
			else
			{
				// Write 0
				pwmData[pwmIndex] = ZERO_HIGH_TIME;
			}
		}
	}
	HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_4, (uint32_t*)pwmData, ARRAY_SIZE);
}

void setPixelColor(uint32_t ledNum, uint8_t red, uint8_t green, uint8_t blue)
{
	if (ledNum >= NUM_PIXELS) return;
	ledData[ledNum][0] = red;
	ledData[ledNum][1] = green;
	ledData[ledNum][2] = blue;
}

void setPixel(uint32_t ledNum, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness)
{
	if (ledNum >= NUM_PIXELS) return;
	ledData[ledNum][0] = red;
	ledData[ledNum][1] = green;
	ledData[ledNum][2] = blue;
	ledData[ledNum][3] = brightness;
}

void setStripColor(uint8_t red, uint8_t green, uint8_t blue)
{
	for (int i = 0; i < NUM_PIXELS; i++)
	{
		setPixelColor(i, red, green, blue);
	}
}

void setStrip(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness)
{
	for (int i = 0; i < NUM_PIXELS; i++)
	{
		setPixel(i, red, green, blue, brightness);
	}
}

void setStripBrightness(uint32_t brightness)
{
	if (brightness > 255) brightness = 255;
	for (int i = 0; i < NUM_PIXELS; i++)
	{
		ledData[i][BRIGHTNESS_INDEX] = brightness;
	}
}

void setPixelBrightness(uint32_t ledNum, uint32_t brightness)
{
	if (ledNum < NUM_PIXELS)
	{
		ledData[ledNum][BRIGHTNESS_INDEX] = brightness;
	}
}

int32_t map(int32_t x, int32_t inputMin, int32_t inputMax, int32_t outputMin, int32_t outputMax)
{
  return (x - inputMin) * (outputMax - outputMin) / (inputMax - inputMin) + outputMin;
}

void generateRainbow(uint32_t rainbowArr[NUM_PIXELS][3])
{
	uint32_t colorPhaseStartIndex = 0;
	for (colorPhase_E colorPhase = RED_TO_RED_GREEN; colorPhase <= NUM_COLOR_PHASES; colorPhase++)
	{
		uint32_t numPhasePixels = (NUM_PIXELS / NUM_COLOR_PHASES) +
									((colorPhase < (NUM_PIXELS % NUM_COLOR_PHASES)) ? 1 : 0);
		uint32_t colorPhasePixelIndex = 0;
		while (colorPhasePixelIndex < numPhasePixels)
		{
			uint32_t pixelIndex = (colorPhaseStartIndex + colorPhasePixelIndex) % NUM_PIXELS;
			switch (colorPhase)
			{
			case RED_TO_RED_GREEN:
				rainbowArr[pixelIndex][0] = 255;
				rainbowArr[pixelIndex][1] = map(colorPhasePixelIndex, 0, numPhasePixels, 0, 255);
				rainbowArr[pixelIndex][2] = 0;
				break;

			case RED_GREEN_TO_GREEN:
				rainbowArr[pixelIndex][0] = map(colorPhasePixelIndex, 0, numPhasePixels, 255, 0);
				rainbowArr[pixelIndex][1] = 255;
				rainbowArr[pixelIndex][2] = 0;
				break;

			case GREEN_TO_GREEN_BLUE:
				rainbowArr[pixelIndex][0] = 0;
				rainbowArr[pixelIndex][1] = 255;
				rainbowArr[pixelIndex][2] = map(colorPhasePixelIndex, 0, numPhasePixels, 0, 255);
				break;

			case GREEN_BLUE_TO_BLUE:
				rainbowArr[pixelIndex][0] = 0;
				rainbowArr[pixelIndex][1] = map(colorPhasePixelIndex, 0, numPhasePixels, 255, 0);
				rainbowArr[pixelIndex][2] = 255;
				break;

			case BLUE_TO_BLUE_RED:
				rainbowArr[pixelIndex][0] = map(colorPhasePixelIndex, 0, numPhasePixels, 0, 255);
				rainbowArr[pixelIndex][1] = 0;
				rainbowArr[pixelIndex][2] = 255;
				break;

			case BLUE_RED_TO_RED:
				rainbowArr[pixelIndex][0] = 255;
				rainbowArr[pixelIndex][1] = 0;
				rainbowArr[pixelIndex][2] = map(colorPhasePixelIndex, 0, numPhasePixels, 255, 0);
				break;

			default:
				break;
			}
			colorPhasePixelIndex++;
		}

		colorPhaseStartIndex += numPhasePixels;
	}
}
void runRainbow(uint32_t rainbowArr[NUM_PIXELS][3])
{
	for(int i = 0; i < NUM_PIXELS; i++)
	{
		setPixelColor(i, rainbowArr[i][0], rainbowArr[i][1], rainbowArr[i][2]);
	}
}

void shiftRainbowUP(uint32_t rainbowArr[NUM_PIXELS][3])
{
	uint32_t temp[3] = {rainbowArr[NUM_PIXELS][0],
						rainbowArr[NUM_PIXELS][1],
						rainbowArr[NUM_PIXELS][2]};

	for(int i = 0; i < NUM_PIXELS; i++)
	{
		for(int k = 0; k < 3; k++)
		{
			rainbowArr[i][k] = rainbowArr[i+1][k];
		}
	}
	for(int i = 0; i < 3; i++)
	{
		rainbowArr[0][i] = temp[i];
	}
}

void shiftRainbowDOWN(uint32_t rainbowArr[NUM_PIXELS][3])
{
	  uint32_t temp[3] = {rainbowArr[0][0],
			  	  	  	  rainbowArr[0][1],
						  rainbowArr[0][2]};

	  for(int i = NUM_PIXELS - 1; i > 0; i--)
	  {
		  for(int k = 0; k < 3; k++)
		  {
			  rainbowArr[i-1][k] = rainbowArr[i][k];
		  }
	  }
	  for(int i = 0; i < 3; i++)
	  {
		  rainbowArr[NUM_PIXELS][i] = temp[i];
	  }
}

void rainbow(uint32_t patternStartIndex)
{
	patternStartIndex = patternStartIndex % NUM_PIXELS;
	uint32_t colorPhaseStartIndex = patternStartIndex;
	for (colorPhase_E colorPhase = RED_TO_RED_GREEN; colorPhase <= NUM_COLOR_PHASES; colorPhase++)
	{
		uint32_t numPixelsPerPhase = (NUM_PIXELS / NUM_COLOR_PHASES) +
									((colorPhase < (NUM_PIXELS % NUM_COLOR_PHASES)) ? 1 : 0);
		uint32_t colorPhasePixelIndex = 0;
		while (colorPhasePixelIndex < numPixelsPerPhase)
		{
			uint32_t pixelIndex = (colorPhaseStartIndex + colorPhasePixelIndex) % NUM_PIXELS;
			switch (colorPhase)
			{
			case RED_TO_RED_GREEN:
				setPixelColor(pixelIndex, 255,
										  map(colorPhasePixelIndex, 0, numPixelsPerPhase, 0, 255),
										  0);
				break;

			case RED_GREEN_TO_GREEN:
				setPixelColor(pixelIndex, map(colorPhasePixelIndex, 0, numPixelsPerPhase, 255, 0),
										  255,
									      0);
				break;

			case GREEN_TO_GREEN_BLUE:
				setPixelColor(pixelIndex, 0,
										  255,
										  map(colorPhasePixelIndex, 0, numPixelsPerPhase, 0, 255));
				break;

			case GREEN_BLUE_TO_BLUE:
				setPixelColor(pixelIndex, 0,
										  map(colorPhasePixelIndex, 0, numPixelsPerPhase, 255, 0),
										  255);
				break;

			case BLUE_TO_BLUE_RED:
				setPixelColor(pixelIndex, map(colorPhasePixelIndex, 0, numPixelsPerPhase, 0, 255),
										  0,
										  255);
				break;

			case BLUE_RED_TO_RED:
				setPixelColor(pixelIndex, 255,
										  0,
										  map(colorPhasePixelIndex, 0, numPixelsPerPhase, 255, 0));
				break;

			default:
				break;
			}
			colorPhasePixelIndex++;
		}

		colorPhaseStartIndex += numPixelsPerPhase;
	}
}

void drawProgressBar(uint32_t percent, uint8_t red, uint8_t green, uint8_t blue)
{
	clear();
	if (percent > 100) percent =100;
	uint32_t numLeds = (percent * NUM_PIXELS) / 100;
	for (int ledIndex = 0; ledIndex < numLeds; ledIndex++)
	{
		setPixelColor(ledIndex, red, green, blue);
	}
}

//void drawColorProgressBar(uint32_t percent, uint8_t redStart, uint8_t greenStart, uint8_t blueStart,
//											uint8_t redEnd, uint8_t greenEnd, uint8_t blueEnd)
//{
//	clear();
//	if (percent > 100) percent =100;
//	uint32_t numLeds = (percent * NUM_PIXELS) / 100;
//	for (int ledIndex = 0; ledIndex < numLeds; ledIndex++)
//	{
//		uint8_t red   = map(ledIndex, 0, NUM_PIXELS, redStart, redEnd);
//		uint8_t green = map(ledIndex, 0, NUM_PIXELS, greenStart, greenEnd);
//		uint8_t blue  = map(ledIndex, 0, NUM_PIXELS, blueStart, blueEnd);
//		setPixelColor(ledIndex, red, green, blue);
//	}
//}

//void drawColorProgressBar(uint32_t percent, uint8_t redStart, uint8_t greenStart, uint8_t blueStart,
//											uint8_t redMid1, uint8_t greenMid1, uint8_t blueMid1,
//											uint8_t redMid2, uint8_t greenMid2, uint8_t blueMid2,
//											uint8_t redEnd, uint8_t greenEnd, uint8_t blueEnd)
//{
//	clear();
//	if (percent > 100) percent =100;
//	uint32_t numLeds = (percent * NUM_PIXELS) / 100;
//	for (int ledIndex = 0; ledIndex < numLeds / 3; ledIndex++)
//	{
//		uint8_t red   = map(ledIndex, 0, NUM_PIXELS, redStart, redMid1);
//		uint8_t green = map(ledIndex, 0, NUM_PIXELS, greenStart, greenMid1);
//		uint8_t blue  = map(ledIndex, 0, NUM_PIXELS, blueStart, blueMid1);
//		setPixelColor(ledIndex, red, green, blue);
//	}
//	for (int ledIndex = 0; ledIndex < 2 * (numLeds / 3); ledIndex++)
//	{
//		uint8_t red   = map(ledIndex, 0, NUM_PIXELS, redMid1, redMid2);
//		uint8_t green = map(ledIndex, 0, NUM_PIXELS, greenMid1, greenMid2);
//		uint8_t blue  = map(ledIndex, 0, NUM_PIXELS, blueMid1, blueMid2);
//		setPixelColor(ledIndex, red, green, blue);
//	}
//	for (int ledIndex = 0; ledIndex < numLeds; ledIndex++)
//	{
//		uint8_t red   = map(ledIndex, 0, NUM_PIXELS, redMid2, redEnd);
//		uint8_t green = map(ledIndex, 0, NUM_PIXELS, greenMid2, greenEnd);
//		uint8_t blue  = map(ledIndex, 0, NUM_PIXELS, blueMid2, blueEnd);
//		setPixelColor(ledIndex, red, green, blue);
//	}
//}

void rpmProgressBar(uint32_t percent, uint8_t redStart, uint8_t greenStart, uint8_t blueStart,
											uint8_t redEnd, uint8_t greenEnd, uint8_t blueEnd)
{
	clear();
	if (percent > 100) percent =100;
	uint32_t numLeds = (percent * NUM_PIXELS) / 100;
	uint32_t startGradient = NUM_PIXELS / 5;
	uint32_t endGradient = NUM_PIXELS - NUM_PIXELS / 5;
	for (int ledIndex = 0; ledIndex < numLeds; ledIndex++)
	{
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;
		if(ledIndex < startGradient)
		{
			red = redStart;
			green = greenStart;
			blue = blueStart;
		}
		else if(ledIndex > endGradient)
		{
			red = redEnd;
			green = greenEnd;
			blue = blueEnd;
		}
		else
		{
			red   = map(ledIndex, startGradient, endGradient, redStart, redEnd);
			green = map(ledIndex, startGradient, endGradient, greenStart, greenEnd);
			blue  = map(ledIndex, startGradient, endGradient, blueStart, blueEnd);
		}
		setPixelColor(NUM_PIXELS - ledIndex, red, green, blue);
	}
}

void drawSmoothRainbow(uint32_t start)
{
	uint32_t pixelStart = start;
	for (uint32_t led = 0; led < NUM_PIXELS; led++)
	{
		if (pixelStart + led < 255)
		{
			setPixelColor(led, 255, led + pixelStart, 0);
		}
		else if (pixelStart + led < 255*2)
		{
			setPixelColor(led, 255*2 - led -pixelStart, 255, 0);
		}
		else if (pixelStart + led < 255*3)
		{
			setPixelColor(led, 0, 255, led + pixelStart -255*2 );
		}
		else if (pixelStart + led < 255*4)
		{
			setPixelColor(led, 0, 255*4- led - pixelStart, 255);
		}
		else if (pixelStart + led < 255*5)
		{
			setPixelColor(led, led + pixelStart - 255*4, 0, 255);
		}
		else if (pixelStart + led < 255*6)
		{
			setPixelColor(led,255, 0, 255*6 - led -pixelStart);
		}
	}


}

