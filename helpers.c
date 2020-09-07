#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // get value of red green blue from original image
            int r = image[i][j].rgbtRed;
            int g = image[i][j].rgbtGreen;
            int b = image[i][j].rgbtBlue;

            // find the average
            int avg = (int)round((r + g + b) / 3.0);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // swap opposite of index j
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // temporary value of new image
    RGBTRIPLE tmp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // variable for sum value of color from original image
            int red = 0, green = 0, blue = 0;
            int cnt = 0;
            for (int r = i - 1; r <= i + 1; r++)
            {
                for (int c = j - 1; c <= j + 1; c++)
                {
                    if (0 <= r && r < height && 0 <= c && c < width)
                    {
                        red += image[r][c].rgbtRed;
                        green += image[r][c].rgbtGreen;
                        blue += image[r][c].rgbtBlue;
                        cnt++;
                    }
                }
            }
            tmp[i][j].rgbtRed = (int)round(1.0 * red / cnt);
            tmp[i][j].rgbtGreen = (int)round(1.0 * green / cnt);
            tmp[i][j].rgbtBlue = (int)round(1.0 * blue / cnt);
        }
    }

    // transfer blue image to original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // temporary value of new image
    RGBTRIPLE tmp[height][width];
    int Gx[3][3] = {{-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}

    };
    int Gy[3][3] = {{-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // variable for convolutional matrix
            int redX = 0, greenX = 0, blueX = 0;
            int redY = 0, greenY = 0, blueY = 0;
            for (int r = i - 1; r <= i + 1; r++)
            {
                for (int c = j - 1; c <= j + 1; c++)
                {
                    if (0 <= r && r < height && 0 <= c && c < width)
                    {
                        // calculate channel R G B in x-axis
                        redX += image[r][c].rgbtRed * Gx[r - i + 1][c - j + 1];
                        greenX += image[r][c].rgbtGreen * Gx[r - i + 1][c - j + 1];
                        blueX += image[r][c].rgbtBlue * Gx[r - i + 1][c - j + 1];

                        // calculate channel R G B in y-axis
                        redY += image[r][c].rgbtRed * Gy[r - i + 1][c - j + 1];
                        greenY += image[r][c].rgbtGreen * Gy[r - i + 1][c - j + 1];
                        blueY += image[r][c].rgbtBlue * Gy[r - i + 1][c - j + 1];
                    }
                }
            }

            int new_red = (int)round(sqrt(redX * redX + redY * redY));
            new_red = new_red > 255 ? 255 : new_red;

            int new_green = (int)round(sqrt(greenX * greenX + greenY * greenY));
            new_green = new_green > 255 ? 255 : new_green;

            int new_blue = (int)round(sqrt(blueX * blueX + blueY * blueY));
            new_blue = new_blue > 255 ? 255 : new_blue;

            tmp[i][j].rgbtRed = new_red;
            tmp[i][j].rgbtGreen = new_green;
            tmp[i][j].rgbtBlue = new_blue;
        }
    }

    // transfer blue image to original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }
    return;
}
