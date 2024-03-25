#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp = (BYTE) round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = temp;
            image[i][j].rgbtGreen = temp;
            image[i][j].rgbtRed = temp;
        }
    }
    return;
}

void swap_RGBTRIPLE(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE temp = *a;
    (*a).rgbtBlue = (*b).rgbtBlue;
    (*a).rgbtGreen = (*b).rgbtGreen;
    (*a).rgbtRed = (*b).rgbtRed;

    (*b).rgbtBlue = temp.rgbtBlue;
    (*b).rgbtGreen = temp.rgbtGreen;
    (*b).rgbtRed = temp.rgbtRed;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            swap_RGBTRIPLE(&image[i][j], &image[i][width - 1 - j]);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    LONG t = 0x00;
    RGBTRIPLE temp;
    temp.rgbtBlue = 0x00;
    temp.rgbtGreen = 0x00;
    temp.rgbtRed = 0x00;
    RGBTRIPLE fake_image[height + 2][width + 2];
    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            fake_image[i][j] = temp;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fake_image[i + 1][j + 1] = image[i][j];
        }
    }
    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            t = fake_image[i - 1][j - 1].rgbtBlue + fake_image[i - 1][j].rgbtBlue + fake_image[i - 1][j + 1].rgbtBlue +
                fake_image[i][j - 1].rgbtBlue + fake_image[i][j].rgbtBlue + fake_image[i][j + 1].rgbtBlue +
                fake_image[i + 1][j - 1].rgbtBlue + fake_image[i + 1][j].rgbtBlue + fake_image[i + 1][j + 1].rgbtBlue;

            if ((i == 1 && j == 1) || (i == 1 && j == width) || (i == height && j == 1) || (i == height && j == width))
                image[i - 1][j - 1].rgbtBlue = (BYTE) round(t / 4.0);
            else if (i == 1 || j == 1 || i == height || j == width)
                image[i - 1][j - 1].rgbtBlue = (BYTE) round(t / 6.0);
            else
                image[i - 1][j - 1].rgbtBlue = (BYTE) round(t / 9.0);

            t = fake_image[i - 1][j - 1].rgbtGreen + fake_image[i - 1][j].rgbtGreen + fake_image[i - 1][j + 1].rgbtGreen +
                fake_image[i][j - 1].rgbtGreen + fake_image[i][j].rgbtGreen + fake_image[i][j + 1].rgbtGreen +
                fake_image[i + 1][j - 1].rgbtGreen + fake_image[i + 1][j].rgbtGreen + fake_image[i + 1][j + 1].rgbtGreen;

            if ((i == 1 && j == 1) || (i == 1 && j == width) || (i == height && j == 1) || (i == height && j == width))
                image[i - 1][j - 1].rgbtGreen = (BYTE) round(t / 4.0);
            else if (i == 1 || j == 1 || i == height || j == width)
                image[i - 1][j - 1].rgbtGreen = (BYTE) round(t / 6.0);
            else
                image[i - 1][j - 1].rgbtGreen = (BYTE) round(t / 9.0);

            t = fake_image[i - 1][j - 1].rgbtRed + fake_image[i - 1][j].rgbtRed + fake_image[i - 1][j + 1].rgbtRed +
                fake_image[i][j - 1].rgbtRed + fake_image[i][j].rgbtRed + fake_image[i][j + 1].rgbtRed +
                fake_image[i + 1][j - 1].rgbtRed + fake_image[i + 1][j].rgbtRed + fake_image[i + 1][j + 1].rgbtRed;

            if ((i == 1 && j == 1) || (i == 1 && j == width) || (i == height && j == 1) || (i == height && j == width))
                image[i - 1][j - 1].rgbtRed = (BYTE) round(t / 4.0);
            else if (i == 1 || j == 1 || i == height || j == width)
                image[i - 1][j - 1].rgbtRed = (BYTE) round(t / 6.0);
            else
                image[i - 1][j - 1].rgbtRed = (BYTE) round(t / 9.0);
        }
    }
}

int kernel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}, kernel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    LONG Gx, Gy, G;
    RGBTRIPLE temp;
    temp.rgbtBlue = 0x00;
    temp.rgbtGreen = 0x00;
    temp.rgbtRed = 0x00;
    RGBTRIPLE fake_image[height + 2][width + 2];
    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            fake_image[i][j] = temp;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fake_image[i + 1][j + 1] = image[i][j];
        }
    }

    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            Gx = (fake_image[i - 1][j - 1].rgbtBlue * kernel_x[0][0]) + (fake_image[i - 1][j].rgbtBlue * kernel_x[0][1]) +
                 (fake_image[i - 1][j + 1].rgbtBlue * kernel_x[0][2]) + (fake_image[i][j - 1].rgbtBlue * kernel_x[1][0]) +
                 (fake_image[i][j].rgbtBlue * kernel_x[1][1]) + (fake_image[i][j + 1].rgbtBlue * kernel_x[1][2]) +
                 (fake_image[i + 1][j - 1].rgbtBlue * kernel_x[2][0]) + (fake_image[i + 1][j].rgbtBlue * kernel_x[2][1]) +
                 (fake_image[i + 1][j + 1].rgbtBlue * kernel_x[2][2]);
            Gy = (fake_image[i - 1][j - 1].rgbtBlue * kernel_y[0][0]) + (fake_image[i - 1][j].rgbtBlue * kernel_y[0][1]) +
                 (fake_image[i - 1][j + 1].rgbtBlue * kernel_y[0][2]) + (fake_image[i][j - 1].rgbtBlue * kernel_y[1][0]) +
                 (fake_image[i][j].rgbtBlue * kernel_y[1][1]) + (fake_image[i][j + 1].rgbtBlue * kernel_y[1][2]) +
                 (fake_image[i + 1][j - 1].rgbtBlue * kernel_y[2][0]) + (fake_image[i + 1][j].rgbtBlue * kernel_y[2][1]) +
                 (fake_image[i + 1][j + 1].rgbtBlue * kernel_y[2][2]);
            G = round(sqrt((double) (Gx * Gx + Gy * Gy)));
            if (G > 0xff)
                G = 0xff;
            image[i - 1][j - 1].rgbtBlue = (BYTE) G;

            Gx = (fake_image[i - 1][j - 1].rgbtGreen * kernel_x[0][0]) + (fake_image[i - 1][j].rgbtGreen * kernel_x[0][1]) +
                 (fake_image[i - 1][j + 1].rgbtGreen * kernel_x[0][2]) + (fake_image[i][j - 1].rgbtGreen * kernel_x[1][0]) +
                 (fake_image[i][j].rgbtGreen * kernel_x[1][1]) + (fake_image[i][j + 1].rgbtGreen * kernel_x[1][2]) +
                 (fake_image[i + 1][j - 1].rgbtGreen * kernel_x[2][0]) + (fake_image[i + 1][j].rgbtGreen * kernel_x[2][1]) +
                 (fake_image[i + 1][j + 1].rgbtGreen * kernel_x[2][2]);
            Gy = (fake_image[i - 1][j - 1].rgbtGreen * kernel_y[0][0]) + (fake_image[i - 1][j].rgbtGreen * kernel_y[0][1]) +
                 (fake_image[i - 1][j + 1].rgbtGreen * kernel_y[0][2]) + (fake_image[i][j - 1].rgbtGreen * kernel_y[1][0]) +
                 (fake_image[i][j].rgbtGreen * kernel_y[1][1]) + (fake_image[i][j + 1].rgbtGreen * kernel_y[1][2]) +
                 (fake_image[i + 1][j - 1].rgbtGreen * kernel_y[2][0]) + (fake_image[i + 1][j].rgbtGreen * kernel_y[2][1]) +
                 (fake_image[i + 1][j + 1].rgbtGreen * kernel_y[2][2]);
            G = round(sqrt((double) (Gx * Gx + Gy * Gy)));
            if (G > 0xff)
                G = 0xff;
            image[i - 1][j - 1].rgbtGreen = (BYTE) G;

            Gx = (fake_image[i - 1][j - 1].rgbtRed * kernel_x[0][0]) + (fake_image[i - 1][j].rgbtRed * kernel_x[0][1]) +
                 (fake_image[i - 1][j + 1].rgbtRed * kernel_x[0][2]) + (fake_image[i][j - 1].rgbtRed * kernel_x[1][0]) +
                 (fake_image[i][j].rgbtRed * kernel_x[1][1]) + (fake_image[i][j + 1].rgbtRed * kernel_x[1][2]) +
                 (fake_image[i + 1][j - 1].rgbtRed * kernel_x[2][0]) + (fake_image[i + 1][j].rgbtRed * kernel_x[2][1]) +
                 (fake_image[i + 1][j + 1].rgbtRed * kernel_x[2][2]);
            Gy = (fake_image[i - 1][j - 1].rgbtRed * kernel_y[0][0]) + (fake_image[i - 1][j].rgbtRed * kernel_y[0][1]) +
                 (fake_image[i - 1][j + 1].rgbtRed * kernel_y[0][2]) + (fake_image[i][j - 1].rgbtRed * kernel_y[1][0]) +
                 (fake_image[i][j].rgbtRed * kernel_y[1][1]) + (fake_image[i][j + 1].rgbtRed * kernel_y[1][2]) +
                 (fake_image[i + 1][j - 1].rgbtRed * kernel_y[2][0]) + (fake_image[i + 1][j].rgbtRed * kernel_y[2][1]) +
                 (fake_image[i + 1][j + 1].rgbtRed * kernel_y[2][2]);
            G = round(sqrt((double) (Gx * Gx + Gy * Gy)));
            if (G > 0xff)
                G = 0xff;
            image[i - 1][j - 1].rgbtRed = (BYTE) G;
        }
    }
}
