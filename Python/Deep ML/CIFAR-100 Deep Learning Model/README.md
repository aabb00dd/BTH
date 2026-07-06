# CIFAR-100 Deep Learning Model Benchmark

## Overview

This project focuses on image classification using PyTorch. The main goal was to train a custom convolutional neural network from scratch and compare its performance against established pretrained models.

The models were trained and evaluated on CIFAR-100, a dataset containing 100 image classes.

---

## Models Compared

- Custom CNN
- ResNet-50
- VGG-19
- DenseNet-121
- EfficientNet-B0

---

## Method

The custom CNN was built with four convolutional blocks and eight convolutional layers in total. Each block used convolution, batch normalization, ReLU activation, and max pooling.

The pretrained models were fine-tuned and compared under the same classification task. All images were resized to `224 × 224` pixels and normalized using ImageNet mean and standard deviation.

Training used:

- CrossEntropyLoss
- Adam optimizer
- Learning rate: `0.001`
- Batch size: `128`
- Maximum epochs: `50`
- Early stopping patience: `5`

---

## Results

| Model | Test Accuracy | F1-score |
|---|---:|---:|
| Custom CNN | 55.58% | 0.5543 |
| ResNet-50 | 74.09% | 0.7389 |
| VGG-19 | 41.03% | 0.4070 |
| DenseNet-121 | 74.03% | 0.7410 |
| EfficientNet-B0 | 76.33% | 0.7605 |

EfficientNet-B0 achieved the best overall result, followed closely by ResNet-50 and DenseNet-121. The custom CNN performed lower than the pretrained models but still showed that it could learn meaningful visual features from scratch.

---

## Key Findings

- Transfer learning clearly outperformed the custom CNN.
- EfficientNet-B0 gave the best balance between accuracy and generalization.
- ResNet-50 and DenseNet-121 also performed strongly.
- VGG-19 underperformed compared to the other pretrained models.
- The custom CNN showed mild overfitting but still learned useful image features.
- Early stopping, dropout, batch normalization, and data augmentation helped reduce overfitting.

---

## Optimization Component

The project also included a gradient descent implementation for minimizing the function:

```text
f(x, y) = (x - 3)^2 + (y + 2)^2
```

Gradient descent successfully converged close to the true minimum at:

```text
x = 3
y = -2
f(x, y) = 0
```

This part demonstrated basic understanding of optimization, derivatives, learning rate, convergence, and loss curves.

---

## What This Project Demonstrates

This project demonstrates practical experience with:

- Deep learning for image classification
- CNN design and training
- Transfer learning with pretrained architectures
- Model evaluation using accuracy and F1-score
- Overfitting analysis
- Training and validation curve interpretation
- PyTorch implementation
- Mathematical optimization using gradient descent

