# Spacecraft Telemetry Anomaly Detection with LSTM Autoencoders

## Overview

This project applies unsupervised anomaly detection to spacecraft sensor data. Since the data is sequential, LSTM autoencoders were used to learn normal time-series behavior and detect abnormal sequences using reconstruction error.

The project compares:

- Simple LSTM Autoencoder
- Deep LSTM Autoencoder
- K-Means baseline model

The models were evaluated using precision, recall, F1-score, ROC-AUC, and anomaly visualizations over time.

---

## Dataset

The project uses the NASA SMAP/MSL anomaly detection dataset.

For this experiment, only channel `A-1` was used:

- Training time steps: `2880`
- Test time steps: `8640`
- Features per time step: `25`
- Sequence length: `50`
- Training sequences: `2831`
- Test sequences: `8591`

The training data was treated as normal system behavior, while the test data contained both normal and anomalous behavior.

---

## Method

The data was preprocessed by:

- Checking and handling missing values
- Scaling features with `StandardScaler`
- Fitting the scaler only on training data to avoid data leakage
- Creating fixed-length time-series sequences
- Converting time-step labels into sequence-level labels

The LSTM autoencoders were trained to reconstruct input sequences. Sequences with high reconstruction error were classified as anomalies.

K-Means was used as a simpler unsupervised baseline by flattening each sequence into a vector and calculating the distance to the nearest cluster center.

---

## Models

### Simple LSTM Autoencoder

- One LSTM encoder layer
- One latent layer
- One LSTM decoder layer
- Hidden size: `64`
- Latent size: `32`

### Deep LSTM Autoencoder

- Two LSTM encoder layers
- Two LSTM decoder layers
- Hidden size: `128`
- Latent size: `64`
- Dropout: `0.2`

### K-Means Baseline

- Number of clusters: `5`
- `n_init = 20`
- Anomaly score based on distance to nearest cluster center

---

## Training Configuration

- Framework: PyTorch
- Loss function: Mean Squared Error
- Optimizer: Adam
- Learning rate: `0.001`
- Batch size: `64`
- Epochs: `50`
- Sequence length: `50`
- Features per time step: `25`

---

## Results

| Model | Threshold | Precision | Recall | F1-score | ROC-AUC |
|---|---|---:|---:|---:|---:|
| Simple LSTM Autoencoder | 98th percentile | 0.0694 | 0.3412 | 0.1153 | 0.8671 |
| Deep LSTM Autoencoder | 97th percentile | 0.0599 | 0.5176 | 0.1073 | 0.8233 |
| K-Means Baseline | 97th percentile | 0.0589 | 0.5176 | 0.1058 | 0.7970 |

The simple LSTM autoencoder achieved the best overall result, with the highest F1-score and ROC-AUC. The deep LSTM autoencoder found more true anomalies but also produced more false positives.

---

## Key Findings

- LSTM autoencoders can detect anomalies in time-series telemetry data.
- Reconstruction error is useful for identifying abnormal sequences.
- The simple LSTM autoencoder performed better than the deeper model in this experiment.
- The deep LSTM model achieved higher recall but lower precision.
- K-Means performed surprisingly close to the deep LSTM model in F1-score.
- LSTM models achieved better ROC-AUC than K-Means because they can model temporal structure.
- Class imbalance made anomaly detection difficult, since less than 1% of test sequences were anomalies.

---

## Limitations

- Only one dataset channel was used.
- Precision was low because of many false positives.
- Threshold selection had a strong effect on performance.
- Percentile-based thresholds may not generalize well to all channels.
- The models should not be used for automatic decisions without human review.

---

## Future Improvements

- Test the models on more SMAP/MSL channels
- Improve threshold tuning
- Try more advanced anomaly detection methods
- Add domain-specific validation
- Combine reconstruction error with other anomaly scores
- Use attention-based or transformer-based sequence models
- Improve false-positive reduction

## What This Project Demonstrates

This project demonstrates practical experience with:

- Time-series anomaly detection
- LSTM autoencoder design
- Unsupervised learning
- Reconstruction-error-based anomaly scoring
- Baseline model comparison
- Threshold tuning
- Evaluation with imbalanced data
- Precision, recall, F1-score, and ROC-AUC analysis
- PyTorch model training
- Data preprocessing without leakage
