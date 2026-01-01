# Face Image Statistical Analysis

This project performs statistical analysis on grayscale face images to identify and compare individuals based on the mean and standard deviation of pixel intensity values. It computes similarity distances between test images and known individuals using basic image statistics.

---

## Features

- Loads grayscale face images from a directory structure.
- Computes per-image and per-person average mean and standard deviation of intensity values.
- Compares test images to known individuals using Euclidean distance in mean/std space.
- Outputs a comparison table of distances from test images to each person.
- Visualizes mean vs. standard deviation for each individual and their group average.

## What I Learned

- How to apply basic statistical analysis (mean, standard deviation) to image data.
- Using Euclidean distance as a simple similarity metric in image classification.
- Handling image processing with OpenCV and NumPy.
- Visualizing image data distributions using Matplotlib.
