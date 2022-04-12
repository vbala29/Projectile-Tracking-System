/**************************************************************************//**
* @file        LCD_GFX.c
* @ingroup 	   LCD_GFX
* @brief       Basic LCD Drawing Library for the St7735
*
* @details     Includes line, circle, square capability
*
*
* @copyright
* @author	   V. Bala
* @date        March 18, 2022
* @version	   1.0
*****************************************************************************/

#include "LCD_GFX.h"
#include "ST7735.h"
#include <math.h>

/******************************************************************************
* Local Functions
******************************************************************************/



/******************************************************************************
* Global Functions
******************************************************************************/

/**************************************************************************//**
* @fn			uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
* @brief		Convert RGB888 value to RGB565 16-bit color data
* @note
*****************************************************************************/
uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
    return ((((31*(red+4))/255)<<11) | (((63*(green+2))/255)<<5) | ((31*(blue+4))/255));
}

/**************************************************************************//**
* @fn			void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color)
* @brief		Draw a single pixel of 16-bit rgb565 color to the x & y coordinate
* @note
*****************************************************************************/
void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color) {
    LCD_setAddr(x,y,x,y);
    SPI_ControllerTx_16bit(color);
}

/**************************************************************************//**
* @fn			void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor)
* @brief		Draw a character starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor){
    uint16_t row = character - 0x20;		//Determine row of ASCII table starting at space
    int i, j;
    if ((LCD_WIDTH-x>7)&&(LCD_HEIGHT-y>7)){
        for(i=0;i<5;i++){
            uint8_t pixels = ASCII[row][i]; //Go through the list of pixels
            for(j=0;j<8;j++){
                if ((pixels>>j)&1==1){
                    LCD_drawPixel(x+i,y+j,fColor);
                }
                else {
                    LCD_drawPixel(x+i,y+j,bColor);
                }
            }
        }
    }
}


/******************************************************************************
* LAB 4 TO DO. COMPLETE THE FUNCTIONS BELOW.
* You are free to create and add any additional files, libraries, and/or
*  helper function. All code must be authentically yours.
******************************************************************************/

/**************************************************************************//**
* @fn			void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
* @brief		Draw a colored circle of set radius at coordinates
* @note
*****************************************************************************/
void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius,uint16_t color)
{
    LCD_setAddr(x0 - radius, y0 - radius, x0 + radius, y0 + radius);
    for (int xCurr = x0 - radius; xCurr <= x0 + radius; xCurr++) {
        for (int yCurr = y0 - radius; yCurr <= y0 + radius; yCurr++) {
            if ((xCurr - x0)*(xCurr - x0) + (yCurr - y0)*(yCurr - y0) <= radius*radius) {
                SPI_ControllerTx_16bit(color);
            } else {
                SPI_ControllerTx_16bit(rgb565(0xFF, 0xFF, 0xFF));
            }
        }
    }
}


/**************************************************************************//**
* @fn			void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
* @brief		Draw a line from and to a point with a color
* @note
*****************************************************************************/
void LCD_drawLine(short x0,short y0,short x1,short y1,uint16_t c)
{
    double slope = (((double) y1 - y0) / ((double) x1 - x0));
    for (int xCurr = x0; xCurr <= x1; xCurr++) {
        int yCurr = round(slope * (xCurr - x0) + (double) y0);
        LCD_setAddr(xCurr, yCurr, xCurr, yCurr);
        SPI_ControllerTx_16bit(c);
    }

}



/**************************************************************************//**
* @fn			void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
* @brief		Draw a colored block at coordinates
* @note
*****************************************************************************/
void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,uint16_t color)
{
    LCD_setAddr(x0, y0, x1, y1);
    for (int xCurr = x0; xCurr <= x1; xCurr++) {
        for (int yCurr = y0; yCurr <= y1; yCurr++) {
            SPI_ControllerTx_16bit(color);
        }
    }

}

/**************************************************************************//**
* @fn			void LCD_setScreen(uint16_t color)
* @brief		Draw the entire screen to a color
* @note
*****************************************************************************/
void LCD_setScreen(uint16_t color)
{
    LCD_setAddr(0, 0, LCD_WIDTH, LCD_HEIGHT);
    for (int xCurr = 0; xCurr <= LCD_WIDTH; xCurr++) {
        for (int yCurr = 0; yCurr <= LCD_HEIGHT; yCurr++) {
            SPI_ControllerTx_16bit(color);
        }
    }
}

/**************************************************************************//**
* @fn			void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
* @brief		Draw a string starting at the point with foreground and background colors
* @note
*****************************************************************************/
void LCD_drawString(uint8_t x, uint8_t y, char* str, uint16_t fg, uint16_t bg)
{
    while (*str) {
        LCD_drawChar(x, y, *str, fg, bg);
        str++;
        x+=8;
    }
}