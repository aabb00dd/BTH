# Movie Performance Prediction Using Machine Learning

This project analyzes the TMDB movie dataset to predict financial and quality metrics using machine learning models. It explores advanced techniques such as feature engineering, one-hot encoding, and sentiment analysis for title-based predictions. The project also evaluates the tradeoffs between model complexity and computational efficiency.

---

## Features

- **Data Exploration and Preprocessing**:
  - Comprehensive analysis of categorical and numerical features.
  - Identification of correlations between budget, revenue, and other predictors.
  - Handling missing values and normalization of numerical features.

- **Feature Engineering**:
  - Creation of binary and multi-class target variables for classification tasks.
  - One-hot encoding for high-dimensional categorical data.
  - Simplified feature engineering for reduced dimensionality and faster computation.

- **Sentiment Analysis**:
  - Implementation of NLTK-based sentiment analysis models for title-based predictions.
  - Non-NLTK approaches using statistical features like word length and special character counts.
  - Comparison of sentiment analysis methods for predictive performance.

- **Machine Learning Models**:
  - Implementation of XGBoost, Random Forest, and K-Nearest Neighbors classifiers.
  - Evaluation of models using stratified k-fold cross-validation.
  - Statistical analysis using Friedman and Nemenyi tests to compare model performance.

- **Hyperparameter Tuning**:
  - Optimization of XGBoost models for revenue, vote average, and profitability predictions.
  - Grid search for selecting the best parameters.

---

## What I Learned

- **Feature Engineering**:
  - How to transform categorical data into meaningful predictors using one-hot encoding and binary features.
  - The importance of dimensionality reduction for computational efficiency.

- **Sentiment Analysis**:
  - Leveraging text-based features to enhance predictive models.
  - Comparing traditional NLP techniques with statistical approaches for sentiment analysis.

- **Model Evaluation**:
  - The significance of cross-validation for robust model assessment.
  - Statistical methods for comparing model performance across multiple metrics.

- **Optimization**:
  - Balancing model complexity and computational efficiency.
  - Hyperparameter tuning to maximize predictive accuracy.

- **Insights into Movie Performance**:
  - Budget is the most important predictor for financial success.
  - English-speaking movies dominate international markets, influencing predictive features.
