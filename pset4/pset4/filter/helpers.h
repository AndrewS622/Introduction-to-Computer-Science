#include "bmp.h"

// Find minimum of two values
int min(int a, int b);

// Find maximum of two values
int max(int a, int b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Create sepia-filtered image
void sepia(int height, int width, RGBTRIPLE image[height][width]);

// Return RGB values of neighbor for edge detection
RGBTRIPLE neighbor(int x, int y, int height, int width, RGBTRIPLE image[height][width]);

// Compute Soble operator sum of squares
RGBTRIPLE findgradient(int x, int y, int height, int width, RGBTRIPLE image[height][width]);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]);

// Return 1 if pixel is within the borders of the image
int isedge(int x, int y, int height, int width);

// Finds average of neighboring pixels for three channels
RGBTRIPLE findaverage(int x, int y, int height, int width, RGBTRIPLE image[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

