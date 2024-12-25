#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Define the RGB image structure
typedef struct {
    int width;
    int height;
    int resolution;
    unsigned char ***data; // 3D array for RGB components
} RGBImage;

// Function to create an image
RGBImage *createImage(int width, int height, int resolution) {
    RGBImage *img = (RGBImage *)malloc(sizeof(RGBImage));
    img->width = width;
    img->height = height;
    img->resolution = resolution;

    img->data = (unsigned char ***)malloc(3 * sizeof(unsigned char **));
    for (int c = 0; c < 3; c++) {
        img->data[c] = (unsigned char **)malloc(height * sizeof(unsigned char *));
        for (int i = 0; i < height; i++) {
            img->data[c][i] = (unsigned char *)malloc(width * sizeof(unsigned char));
        }
    }
    return img;
}

// Function to initialize image data with random values
void initializeRandom(RGBImage *img) {
    srand(time(NULL));
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < img->height; i++) {
            for (int j = 0; j < img->width; j++) {
                img->data[c][i][j] = rand() % 256; // Random value between 0 and 255
            }
        }
    }
}

// Function to apply Gaussian filter
void applyGaussianFilter(RGBImage *img) {
    const float kernel[3][3] = {
        {1 / 16.0, 1 / 8.0, 1 / 16.0},
        {1 / 8.0,  1 / 4.0, 1 / 8.0},
        {1 / 16.0, 1 / 8.0, 1 / 16.0}
    };

    for (int c = 0; c < 3; c++) {
        for (int i = 1; i < img->height - 1; i++) {
            for (int j = 1; j < img->width - 1; j++) {
                float newValue = 0.0;
                for (int ki = -1; ki <= 1; ki++) {
                    for (int kj = -1; kj <= 1; kj++) {
                        newValue += img->data[c][i + ki][j + kj] * kernel[ki + 1][kj + 1];
                    }
                }
                img->data[c][i][j] = (unsigned char)newValue;
            }
        }
    }
}

// Function to apply gamma correction
void applyGammaCorrection(RGBImage *img, float gamma) {
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < img->height; i++) {
            for (int j = 0; j < img->width; j++) {
                float normalized = img->data[c][i][j] / 255.0; // Normalize to [0, 1]
                float corrected = pow(normalized, gamma);
                img->data[c][i][j] = (unsigned char)(corrected * 255.0); // Rescale to [0, 255]
            }
        }
    }
}

// Function to invert the image colors
void applyInversion(RGBImage *img) {
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < img->height; i++) {
            for (int j = 0; j < img->width; j++) {
                img->data[c][i][j] = 255 - img->data[c][i][j]; // Invert the color
            }
        }
    }
}

// Function to convert the image to grayscale
void convertToGrayscale(RGBImage *img) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            // Calculate grayscale value using a weighted average
            unsigned char grayValue = (unsigned char)(
                0.3 * img->data[0][i][j] + // Red channel
                0.59 * img->data[1][i][j] + // Green channel
                0.11 * img->data[2][i][j]   // Blue channel
            );
            img->data[0][i][j] = grayValue; // Red
            img->data[1][i][j] = grayValue; // Green
            img->data[2][i][j] = grayValue; // Blue
        }
    }
}

// Function to apply convolution to an image
void applyConvolution(RGBImage *img, float kernel[3][3]) {
    unsigned char ***temp = (unsigned char ***)malloc(3 * sizeof(unsigned char **));
    for (int c = 0; c < 3; c++) {
        temp[c] = (unsigned char **)malloc(img->height * sizeof(unsigned char *));
        for (int i = 0; i < img->height; i++) {
            temp[c][i] = (unsigned char *)malloc(img->width * sizeof(unsigned char));
        }
    }

    for (int c = 0; c < 3; c++) { // Apply convolution to each color channel
        for (int i = 1; i < img->height - 1; i++) {
            for (int j = 1; j < img->width - 1; j++) {
                float sum = 0.0;
                for (int ki = -1; ki <= 1; ki++) {
                    for (int kj = -1; kj <= 1; kj++) {
                        sum += img->data[c][i + ki][j + kj] * kernel[ki + 1][kj + 1];
                    }
                }
                temp[c][i][j] = (unsigned char)fmin(fmax(sum, 0), 255); // Clamp to [0, 255]
            }
        }
    }

    // Copy the result back to the original image
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < img->height; i++) {
            for (int j = 0; j < img->width; j++) {
                img->data[c][i][j] = temp[c][i][j];
            }
            free(temp[c][i]);
        }
        free(temp[c]);
    }
    free(temp);
}

// Function to print the image (RGB values)
void printImage(RGBImage *img) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            // Print the RGB values for each pixel
            printf("(%d, %d, %d) ", 
                img->data[0][i][j], // Red
                img->data[1][i][j], // Green
                img->data[2][i][j]  // Blue
            );
        }
        printf("\n");
    }
}

// Function to free image memory
void freeImage(RGBImage *img) {
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < img->height; i++) {
            free(img->data[c][i]);
        }
        free(img->data[c]);
    }
    free(img->data);
    free(img);
}

// Main program
int main() {
    int width = 4, height = 4, resolution = 1; // Smaller image for printing
    RGBImage *image = createImage(width, height, resolution);

    // Initialize the image with random values
    initializeRandom(image);
    printf("Image initialized with random values.\n");
    printImage(image);

    // // Apply Gaussian filter
    // applyGaussianFilter(image);
    // printf("Gaussian filter applied.\n");

    // // Apply gamma correction
    // float gamma = 2.2;
    // applyGammaCorrection(image, gamma);
    // printf("Gamma correction applied with gamma = %.2f.\n");

    // // Print the original processed image
    // printf("Original Processed Image:\n");
    // printImage(image);

    // // Apply inversion
    // applyInversion(image);
    // printf("Image colors inverted.\n");
    // printf("Inverted Image:\n");
    // printImage(image);

    // // Convert to grayscale
    // convertToGrayscale(image);
    // printf("Image converted to grayscale.\n");
    // printf("Grayscale Image:\n");
    // printImage(image);

    // // Apply edge detection filter
    // float edgeKernel[3][3] = {
    //     {-1, -1, -1},
    //     {-1, 8, -1},
    //     {-1, -1, -1}
    // };
    // applyConvolution(image, edgeKernel);
    // printf("\nAfter Applying Edge Detection Filter:\n");
    // printImage(image);


    // Free memory
    freeImage(image);
    printf("Memory freed and program completed.\n");

    return 0;
}
