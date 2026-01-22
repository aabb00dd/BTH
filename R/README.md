# R Statistical Modeling Projects

This repository contains R projects developed as part of coursework in **statistical modeling, time series analysis, and Bayesian inference**. The projects focus on applying theoretical concepts to real-world datasets using reproducible, script-based workflows in R.

---

## About the Projects

The purpose of these projects is to explore and apply core concepts in time series modeling, forecasting, and uncertainty quantification. The work spans both **classical time series methods** and **modern Bayesian approaches**, with an emphasis on diagnostics, interpretation, and model comparison.

---

## Project Areas

### 1. Classical Time Series Modeling
Projects in this category focus on traditional stochastic time series models and diagnostics, including:

- AR, ARMA, SARMA, and SARIMA models  
- Stationarity testing (ADF)  
- Residual diagnostics (Ljung–Box, ARCH tests)  
- Variance stabilization using Box–Cox transformations  
- Time series decomposition (trend, seasonal, residual components)  
- Model evaluation using AIC, BIC, RMSE, MAPE, and MASE  

---

### 2. Bayesian Time Series Modeling and Forecasting
This repository also includes a Bayesian analysis of macroeconomic time series data:

**Bayesian Trend Modeling and Forecasting of Argentina CPI**
- Quarterly CPI data (1970 Q1 – 1989 Q4)
- Log-transformation and standardization for numerical stability
- Bayesian linear trend model as a baseline
- Bayesian spline trend model to capture nonlinear inflation dynamics
- Model evaluation using posterior diagnostics and posterior predictive checks
- Probabilistic forecasting with predictive and mean uncertainty intervals

This project highlights the limitations of linear trend assumptions during periods of accelerating inflation and demonstrates how Bayesian spline models provide more realistic forecasts and uncertainty estimates.

---

## Tools & Technologies

- R
- brms (Stan backend)
- AER
- ggplot2
- Bayesian statistics
- Classical time series analysis

---

## Learning Outcomes

Through these projects, I gained hands-on experience in:

- Building and evaluating time series models in R  
- Applying both frequentist and Bayesian modeling approaches  
- Diagnosing model assumptions and residual behavior  
- Interpreting uncertainty in forecasts  
- Translating statistical results into meaningful real-world insights  
