# Statistical Modeling Assignment – Time Series Forecasting of Caconde Reservoir

This project is a time series analysis and forecasting report developed as part of the MS1415 course **"Robusta Metoder"** at BTH. The study models the monthly proportion of useful water volume in the Caconde Reservoir (São Paulo, Brazil) from January 2015 to February 2024.

## Overview
The goal is to evaluate and compare time series models (AR, ARMA, SARMA, SARIMA) to forecast the reservoir levels. Each model was evaluated based on statistical fit and forecast accuracy.

### Key Findings
- **Best model fit**: `ARMA` (lowest AIC)
- **Best forecast accuracy**: `SARIMA` (lowest RMSE & MASE)
- **Most balanced performance**: `SARMA` (stable fit and reliable forecasting)

## Project Structure
- **Data Exploration**: Visualization, decomposition, stationarity checks  
- **Modeling**: AR, ARMA, SARMA, SARIMA with Box-Cox transformation  
- **Validation**: Residual analysis and statistical tests  
- **Comparison**: AIC, BIC, RMSE, MAPE, MASE

## Conclusion
The SARMA model showed the best trade-off between model complexity and forecast performance, making it the most reliable model for this dataset.
