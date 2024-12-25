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

// Function to save the image as a PPM file
void saveImageAsPPM(RGBImage *img, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Write PPM header
    fprintf(file, "P3\n");
    fprintf(file, "%d %d\n", img->width, img->height);
    fprintf(file, "255\n");

    // Write pixel data
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            fprintf(file, "%d %d %d ", 
                img->data[0][i][j], // Red
                img->data[1][i][j], // Green
                img->data[2][i][j]  // Blue
            );
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Image saved as %s\n", filename);
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
    int width = 256, height = 256, resolution = 72;
    RGBImage *image = createImage(width, height, resolution);

    // Initialize the image with random values
    initializeRandom(image);
    printf("Image initialized with random values.\n");

    // Apply Gaussian filter
    applyGaussianFilter(image);
    printf("Gaussian filter applied.\n");

    // Apply gamma correction
    float gamma = 2.2;
    applyGammaCorrection(image, gamma);
    printf("Gamma correction applied with gamma = %.2f.\n");

    // Save image to file
    saveImageAsPPM(image, "output.ppm");

    // Free memory
    freeImage(image);
    printf("Memory freed and program completed.\n");

    return 0;
}
