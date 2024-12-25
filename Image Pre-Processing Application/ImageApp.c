#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Define the une_imageRGB structure
typedef struct {
    int resolution; // Resolution R
    int width;      // Longueur L
    int height;     // Hauteur H
    unsigned char **img[3]; // Pointers to RGB components
} une_imageRGB;

// Function to allocate memory for an image
une_imageRGB *create_image(int resolution, int width, int height) {
    une_imageRGB *image = (une_imageRGB *)malloc(sizeof(une_imageRGB));
    image->resolution = resolution;
    image->width = width;
    image->height = height;

    for (int i = 0; i < 3; i++) {
        image->img[i] = (unsigned char **)malloc(height * sizeof(unsigned char *));
        for (int j = 0; j < height; j++) {
            image->img[i][j] = (unsigned char *)malloc(width * sizeof(unsigned char));
        }
    }

    return image;
}

// Function to free memory for an image
void free_image(une_imageRGB *image) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < image->height; j++) {
            free(image->img[i][j]);
        }
        free(image->img[i]);
    }
    free(image);
}

// Load a PPM image file
une_imageRGB *load_ppm(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    char format[3];
    fscanf(file, "%2s", format);
    if (strcmp(format, "P6") != 0) {
        fprintf(stderr, "Unsupported file format\n");
        fclose(file);
        return NULL;
    }

    int width, height, maxval;
    fscanf(file, "%d %d %d", &width, &height, &maxval);
    fgetc(file); // Consume the newline after the header

    une_imageRGB *image = create_image(maxval, width, height);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            for (int c = 0; c < 3; c++) {
                image->img[c][j][i] = fgetc(file);
            }
        }
    }

    fclose(file);
    return image;
}

// Save a PPM image file
void save_ppm(const char *filename, une_imageRGB *image) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "P6\n%d %d\n%d\n", image->width, image->height, image->resolution);
    for (int j = 0; j < image->height; j++) {
        for (int i = 0; i < image->width; i++) {
            for (int c = 0; c < 3; c++) {
                fputc(image->img[c][j][i], file);
            }
        }
    }

    fclose(file);
}

// Extract grayscale from an image component
une_imageRGB *extract_grayscale(une_imageRGB *image, int component) {
    une_imageRGB *gray_image = create_image(image->resolution, image->width, image->height);

    for (int j = 0; j < image->height; j++) {
        for (int i = 0; i < image->width; i++) {
            unsigned char value = image->img[component][j][i];
            gray_image->img[0][j][i] = gray_image->img[1][j][i] = gray_image->img[2][j][i] = value;
        }
    }

    return gray_image;
}

// Simple blur function
une_imageRGB *apply_blur(une_imageRGB *image) {
    une_imageRGB *blurred_image = create_image(image->resolution, image->width, image->height);

    for (int j = 1; j < image->height - 1; j++) {
        for (int i = 1; i < image->width - 1; i++) {
            for (int c = 0; c < 3; c++) {
                int sum = 0;
                for (int y = -1; y <= 1; y++) {
                    for (int x = -1; x <= 1; x++) {
                        sum += image->img[c][j + y][i + x];
                    }
                }
                blurred_image->img[c][j][i] = sum / 9;
            }
        }
    }

    return blurred_image;
}

// Edge detection function
une_imageRGB *apply_edge_detection(une_imageRGB *image) {
    une_imageRGB *edge_image = create_image(image->resolution, image->width, image->height);

    int kernel[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };

    for (int j = 1; j < image->height - 1; j++) {
        for (int i = 1; i < image->width - 1; i++) {
            for (int c = 0; c < 3; c++) {
                int sum = 0;
                for (int y = -1; y <= 1; y++) {
                    for (int x = -1; x <= 1; x++) {
                        sum += kernel[y + 1][x + 1] * image->img[c][j + y][i + x];
                    }
                }
                edge_image->img[c][j][i] = (unsigned char) (sum < 0 ? 0 : (sum > 255 ? 255 : sum));
            }
        }
    }

    return edge_image;
}

// Sharpen function
une_imageRGB *apply_sharpen(une_imageRGB *image) {
    une_imageRGB *sharpen_image = create_image(image->resolution, image->width, image->height);

    int kernel[3][3] = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    };

    for (int j = 1; j < image->height - 1; j++) {
        for (int i = 1; i < image->width - 1; i++) {
            for (int c = 0; c < 3; c++) {
                int sum = 0;
                for (int y = -1; y <= 1; y++) {
                    for (int x = -1; x <= 1; x++) {
                        sum += kernel[y + 1][x + 1] * image->img[c][j + y][i + x];
                    }
                }
                sharpen_image->img[c][j][i] = (unsigned char) (sum < 0 ? 0 : (sum > 255 ? 255 : sum));
            }
        }
    }

    return sharpen_image;
}

// Brightness adjustment function
une_imageRGB *adjust_brightness(une_imageRGB *image, int brightness) {
    une_imageRGB *bright_image = create_image(image->resolution, image->width, image->height);

    for (int j = 0; j < image->height; j++) {
        for (int i = 0; i < image->width; i++) {
            for (int c = 0; c < 3; c++) {
                int value = image->img[c][j][i] + brightness;
                bright_image->img[c][j][i] = (unsigned char) (value < 0 ? 0 : (value > 255 ? 255 : value));
            }
        }
    }

    return bright_image;
}

// Contrast adjustment function
une_imageRGB *adjust_contrast(une_imageRGB *image, float contrast) {
    une_imageRGB *contrast_image = create_image(image->resolution, image->width, image->height);

    for (int j = 0; j < image->height; j++) {
        for (int i = 0; i < image->width; i++) {
            for (int c = 0; c < 3; c++) {
                int value = (int)((image->img[c][j][i] - 128) * contrast + 128);
                contrast_image->img[c][j][i] = (unsigned char) (value < 0 ? 0 : (value > 255 ? 255 : value));
            }
        }
    }

    return contrast_image;
}

// Function to open the image viewer
void display_image(const char *filename) {
#ifdef _WIN32
    ShellExecute(0, 0, filename, 0, 0, SW_SHOW);
#else
    char command[256];
    snprintf(command, sizeof(command), "xdg-open %s", filename);
    system(command);
#endif
}

int main() {
    const char *input_file = "input2.ppm";
    une_imageRGB *image = load_ppm(input_file);
    if (!image) {
        return 1;
    }

    int choice;
    do {
        printf("\n--- Image Processing Menu ---\n");
        printf("1. Extract Grayscale\n");
        printf("2. Apply Blur\n");
        printf("3. Edge Detection\n");
        printf("4. Sharpen\n");
        printf("5. Adjust Brightness\n");
        printf("6. Adjust Contrast\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        une_imageRGB *processed_image = NULL;
        char output_file[256];

        switch (choice) {
            case 1:
                processed_image = extract_grayscale(image, 0);
                snprintf(output_file, sizeof(output_file), "output_grayscale.ppm");
                break;
            case 2:
                processed_image = apply_blur(image);
                snprintf(output_file, sizeof(output_file), "output_blur.ppm");
                break;
            case 3:
                processed_image = apply_edge_detection(image);
                snprintf(output_file, sizeof(output_file), "output_edge.ppm");
                break;
            case 4:
                processed_image = apply_sharpen(image);
                snprintf(output_file, sizeof(output_file), "output_sharpen.ppm");
                break;
            case 5: {
                int brightness;
                printf("Enter brightness adjustment value (-255 to 255): ");
                scanf("%d", &brightness);
                processed_image = adjust_brightness(image, brightness);
                snprintf(output_file, sizeof(output_file), "output_brightness.ppm");
                break;
            }
            case 6: {
                float contrast;
                printf("Enter contrast adjustment value (e.g., 1.5 for higher contrast): ");
                scanf("%f", &contrast);
                processed_image = adjust_contrast(image, contrast);
                snprintf(output_file, sizeof(output_file), "output_contrast.ppm");
                break;
            }
            case 7:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        if (processed_image) {
            save_ppm(output_file, processed_image);
            display_image(output_file);
            free_image(processed_image);
            printf("Processed image saved to %s\n", output_file);
        }

    } while (choice != 7);

    free_image(image);
    return 0;
}