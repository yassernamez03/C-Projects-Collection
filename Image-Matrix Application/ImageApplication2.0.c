#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Define the structure for an RGB image
typedef struct {
    int resolution;
    int length;   // L
    int height;   // H
    unsigned char ***img; // 3D array for RGB components
} RGBImage;

// Function to create an RGB image
RGBImage* createImage(int length, int height) {
    RGBImage *image = (RGBImage*)malloc(sizeof(RGBImage));
    image->length = length;
    image->height = height;
    image->resolution = length * height;
    
    // Allocate memory for the 3 color channels (R, G, B)
    image->img = (unsigned char***)malloc(3 * sizeof(unsigned char**));
    for (int c = 0; c < 3; c++) {
        image->img[c] = (unsigned char**)malloc(height * sizeof(unsigned char*));
        for (int i = 0; i < height; i++) {
            image->img[c][i] = (unsigned char*)malloc(length * sizeof(unsigned char));
        }
    }
    return image;
}

// Function to initialize the image with random values
void initializeImageRandom(RGBImage *image) {
    srand(time(0));
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < image->height; i++) {
            for (int j = 0; j < image->length; j++) {
                image->img[c][i][j] = rand() % 256; // Random value between 0-255
            }
        }
    }
}

// Function to print the RGB image
void printImage(RGBImage *image) {
    printf("\n--- Red Channel ---\n");
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->length; j++) {
            printf("%3d ", image->img[0][i][j]); // Red
        }
        printf("\n");
    }

    printf("\n--- Green Channel ---\n");
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->length; j++) {
            printf("%3d ", image->img[1][i][j]); // Green
        }
        printf("\n");
    }

    printf("\n--- Blue Channel ---\n");
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->length; j++) {
            printf("%3d ", image->img[2][i][j]); // Blue
        }
        printf("\n");
    }
}

// Function for gamma correction
void gammaCorrection(RGBImage *image, float gamma) {
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < image->height; i++) {
            for (int j = 0; j < image->length; j++) {
                float normalized = image->img[c][i][j] / 255.0;
                normalized = pow(normalized, gamma);
                image->img[c][i][j] = (unsigned char)(normalized * 255);
            }
        }
    }
}

// Function for image inversion
void invertImage(RGBImage *image) {
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < image->height; i++) {
            for (int j = 0; j < image->length; j++) {
                image->img[c][i][j] = 255 - image->img[c][i][j];
            }
        }
    }
}

// Function for convolution (generic filter)
void convolution(RGBImage *image, float kernel[3][3]) {
    for (int c = 0; c < 3; c++) {
        for (int i = 1; i < image->height - 1; i++) {
            for (int j = 1; j < image->length - 1; j++) {
                float sum = 0.0;
                for (int ki = -1; ki <= 1; ki++) {
                    for (int kj = -1; kj <= 1; kj++) {
                        sum += kernel[ki + 1][kj + 1] * image->img[c][i + ki][j + kj];
                    }
                }
                image->img[c][i][j] = (unsigned char)fmin(fmax(sum, 0), 255);
            }
        }
    }
}

// Function to apply Gaussian filter
void gaussianFilter(RGBImage *image) {
    float gaussianKernel[3][3] = {
        {1.0 / 16, 2.0 / 16, 1.0 / 16},
        {2.0 / 16, 4.0 / 16, 2.0 / 16},
        {1.0 / 16, 2.0 / 16, 1.0 / 16}
    };
    convolution(image, gaussianKernel);
}

// Function to save the image to a file
void saveImage(RGBImage *image, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    fprintf(file, "P3\n%d %d\n255\n", image->length, image->height);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->length; j++) {
            fprintf(file, "%d %d %d ", image->img[0][i][j], image->img[1][i][j], image->img[2][i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Image saved to %s\n", filename);
}

// Function to free the memory allocated for the image
void freeImage(RGBImage *image) {
    for (int c = 0; c < 3; c++) {
        for (int i = 0; i < image->height; i++) {
            free(image->img[c][i]);
        }
        free(image->img[c]);
    }
    free(image->img);
    free(image);
}

// Main menu function for the application
void displayMenu() {
    printf("\n--- Image Processing Menu ---\n");
    printf("1. Initialize Image\n");
    printf("2. Apply Gaussian Filter\n");
    printf("3. Apply Gamma Correction\n");
    printf("4. Invert Image\n");
    printf("5. Save Image\n");
    printf("6. Exit\n");
}

// Main function
int main() {
    int length, height;
    printf("\nEnter image length (width): ");
    scanf("%d", &length);
    printf("Enter image height: ");
    scanf("%d", &height);

    RGBImage *image = createImage(length, height);

    int choice;
    int exitFlag = 0;
    float gamma;

    while (!exitFlag) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Initializing image with random values...\n");
                initializeImageRandom(image);
                // printImage(image);
                break;

            case 2:
                printf("Applying Gaussian Filter...\n");
                gaussianFilter(image);
                // printImage(image);
                break;

            case 3:
                printf("Enter gamma value for correction: ");
                scanf("%f", &gamma);
                printf("Applying Gamma Correction with gamma = %.2f...\n", gamma);
                gammaCorrection(image, gamma);
                // printImage(image);
                break;

            case 4:
                printf("Inverting the image...\n");
                invertImage(image);
                // printImage(image);
                break;

            case 5:
                {
                    char filename[50];
                    printf("Enter the filename to save the image (e.g., output_image.ppm): ");
                    scanf("%s", filename);
                    saveImage(image, filename);
                }
                break;

            case 6:
                printf("Exiting the program...\n");
                exitFlag = 1;
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    freeImage(image);
    return 0;
}
