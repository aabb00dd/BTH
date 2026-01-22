# Bayesian CPI Forecasting – Argentina

This project analyzes the quarterly Consumer Price Index (CPI) time series for Argentina using **Bayesian regression models**. The goal is to model long-term inflation dynamics and generate short-term forecasts while properly accounting for uncertainty.

The analysis compares a **Bayesian linear trend model** with a **Bayesian spline trend model**, highlighting the limitations of linear assumptions in periods of rapidly accelerating inflation.

---

## Overview

Argentina’s CPI exhibits extreme growth and strong nonlinearity toward the end of the observed period. To address this, the analysis:

- Models CPI on the logarithmic scale
- Applies Bayesian time series regression
- Evaluates model fit using posterior diagnostics
- Produces probabilistic forecasts with uncertainty intervals

---

## Data

- **Dataset:** `ArgentinaCPI` from the `AER` R package  
- **Frequency:** Quarterly  
- **Period:** 1970 Q1 – 1989 Q4  
- **Observations:** 80  

The raw CPI series is highly right-skewed and spans several orders of magnitude, motivating a log transformation prior to modeling.

---

## Methodology

### Preprocessing
- Log transformation of CPI
- Standardization (mean 0, standard deviation 1)
- Back-transformation for interpretation on the original scale

### Models
1. **Bayesian Linear Trend**
   - Gaussian likelihood
   - Weakly informative priors
   - Serves as a baseline model

2. **Bayesian Spline Trend**
   - Smooth nonlinear trend using splines
   - Captures accelerating inflation dynamics
   - Improved fit during late 1980s hyperinflation

### Inference
- Implemented using `brms` (Stan backend)
- 4 chains, 4000 iterations, 1000 warmup
- Sampler tuned (`adapt_delta`, `max_treedepth`) to ensure convergence

---

## Diagnostics and Evaluation

- Trace plots and effective sample sizes
- Posterior predictive checks (density and histograms)
- Residual analysis and autocorrelation (ACF)
- Model comparison: linear vs spline fitted means

The spline model shows superior fit and better captures changes in growth rate over time.

---

## Forecasting

- Forecast horizon: **5 quarters ahead**
- Forecasts generated from the spline model
- Both **predictive intervals** (future observations) and **mean intervals** (latent trend) are reported

Results indicate explosive CPI growth consistent with historical hyperinflation dynamics.

---

## Key Findings

- Linear trend models underestimate accelerating inflation
- Bayesian spline models better capture nonlinear dynamics
- Uncertainty grows rapidly in high-inflation regimes
- Bayesian forecasting provides transparent uncertainty quantification
