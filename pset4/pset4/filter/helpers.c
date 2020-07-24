#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Find minimum of two values
int min(int a, int b)
{
    if (a > b)
    {
        return b;
    }
    return a;
}

// Find maximum of two values
int max(int a, int b)
{
    if (a < b)
    {
        return b;
    }
    return a;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = image[i][j];
            int avg = round((float)(pixel.rgbtBlue + pixel.rgbtGreen + pixel.rgbtRed) / 3);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    float center = (float)width / 2;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < center; j++)
        {
            RGBTRIPLE temp;
            temp.rgbtBlue = image[i][j].rgbtBlue;
            temp.rgbtGreen = image[i][j].rgbtGreen;
            temp.rgbtRed = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;

            image[i][width - j - 1].rgbtBlue = temp.rgbtBlue;
            image[i][width - j - 1].rgbtGreen = temp.rgbtGreen;
            image[i][width - j - 1].rgbtRed = temp.rgbtRed;
        }
    }
    return;
}

// Create sepia-filtered image
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = image[i][j];
            image[i][j].rgbtBlue = min(round(0.272 * pixel.rgbtRed + 0.534 * pixel.rgbtGreen + 0.131 * pixel.rgbtBlue), 255);
            image[i][j].rgbtGreen = min(round(0.349 * pixel.rgbtRed + 0.686 * pixel.rgbtGreen + 0.168 * pixel.rgbtBlue), 255);
            image[i][j].rgbtRed = min(round(0.393 * pixel.rgbtRed + 0.769 * pixel.rgbtGreen + 0.189 * pixel.rgbtBlue), 255);
        }
    }
}

// Return 1 if pixel is within the borders of the image
int isedge(int x, int y, int height, int width)
{
    if (x < 0 || x >= height || y < 0 || y >= width)
    {
        return 0;
    }
    return 1;
}

// Finds average of neighboring pixels for three channels
RGBTRIPLE findaverage(int x, int y, int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixel;
    float blue = 0x00;
    float green = 0x00;
    float red = 0x00;

    int num_neighbor = 3;

    // keep track of valid positions
    int valid_count = 0;
    int edge = 0;

    for (int i = -1; i < num_neighbor - 1; i++)
    {
        for (int j = -1; j < num_neighbor - 1; j++)
        {
            edge = isedge(x + i, y + j, height, width);
            if (edge)
            {
                valid_count++;
                blue += image[x + i][y + j].rgbtBlue;
                green += image[x + i][y + j].rgbtGreen;
                red += image[x + i][y + j].rgbtRed;
            }
        }
    }
    blue /= valid_count;
    green /= valid_count;
    red /= valid_count;

    pixel.rgbtBlue = round(blue);
    pixel.rgbtGreen = round(green);
    pixel.rgbtRed = round(red);
    return pixel;
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_original[height][width];
    // Store initial image since image matrix will be modified
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_original[i][j].rgbtBlue = image[i][j].rgbtBlue;
            image_original[i][j].rgbtGreen = image[i][j].rgbtGreen;
            image_original[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }

    RGBTRIPLE pixel;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixel = findaverage(i, j, height, width, image_original);
            image[i][j].rgbtBlue = pixel.rgbtBlue;
            image[i][j].rgbtGreen = pixel.rgbtGreen;
            image[i][j].rgbtRed = pixel.rgbtRed;
        }
    }

    return;
}

// Return RGB values of neighbor for edge detection
RGBTRIPLE neighbor(int x, int y, int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE pixel;

    // If outside of bounds of image, return all 0x00 for three colors
    // Otherwise, return stored value of neighbor structure
    if (x < 0 || x >= height || y < 0 || y >= width)
    {
        pixel.rgbtBlue = 0x00;
        pixel.rgbtRed = 0x00;
        pixel.rgbtGreen = 0x00;
    }
    else
    {
        pixel.rgbtBlue = image[x][y].rgbtBlue;
        pixel.rgbtGreen = image[x][y].rgbtGreen;
        pixel.rgbtRed = image[x][y].rgbtRed;

    }
    return pixel;
}

// Compute Soble operator sum of squares
RGBTRIPLE findgradient(int x, int y, int height, int width, RGBTRIPLE image[height][width])
{
    // x and y gradients
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // to keep track of x and y values independently
    int bluex = 0;
    int bluey = 0;
    int greenx = 0;
    int greeny = 0;
    int redx = 0;
    int redy = 0;

    // to combine by sum of squares
    int blue = 0;
    int green = 0;
    int red = 0;

    int num_neighbor = 3;

    RGBTRIPLE pixel;
    RGBTRIPLE result;

    // gets each neighbor value and updates current x and y totals for each color channel scaled by Gx and Gy
    for (int i = -1; i < num_neighbor - 1; i++)
    {
        for (int j = -1; j < num_neighbor - 1; j++)
        {
            pixel = neighbor(x + i, y + j, height, width, image);

            bluex += Gx[i + 1][j + 1] * (pixel.rgbtBlue);
            bluey += Gy[i + 1][j + 1] * (pixel.rgbtBlue);

            redx += Gx[i + 1][j + 1] * (pixel.rgbtRed);
            redy += Gy[i + 1][j + 1] * (pixel.rgbtRed);

            greenx += Gx[i + 1][j + 1] * (pixel.rgbtGreen);
            greeny += Gy[i + 1][j + 1] * (pixel.rgbtGreen);
        }
    }

    // Round square root of sum of squares
    blue = round(sqrt(pow(bluex, 2) + pow(bluey, 2)));
    red = round(sqrt(pow(redx, 2) + pow(redy, 2)));
    green = round(sqrt(pow(greenx, 2) + pow(greeny, 2)));

    // Ensure each value is between 0 and 255
    result.rgbtBlue = round(max(min(blue, 255), 0));
    result.rgbtRed = round(max(min(red, 255), 0));
    result.rgbtGreen = round(max(min(green, 255), 0));

    return result;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_original[height][width];

    // Store initial image as image matrix will be updated
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_original[i][j].rgbtBlue = image[i][j].rgbtBlue;
            image_original[i][j].rgbtGreen = image[i][j].rgbtGreen;
            image_original[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }

    RGBTRIPLE pixel;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixel = findgradient(i, j, height, width, image_original);
            image[i][j].rgbtBlue = pixel.rgbtBlue;
            image[i][j].rgbtGreen = pixel.rgbtGreen;
            image[i][j].rgbtRed = pixel.rgbtRed;
        }
    }

    return;
}
