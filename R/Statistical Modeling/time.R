# --------- Load packages and import data --------- #
# 1. Load packages
library(tidyverse)
library(forecast)
library(tseries)
library(dplyr)
library(tibble)
library(lmtest)  # For coeftest
library(nortest) # For additional normality tests
library(aTSA)    # For ARCH test
library(car)     # For QQ plots
library(FinTS)
library(MASS)


# 2. Import data
volume_values <- read_lines("CACONDE.txt") %>% 
  as.numeric() %>%
  enframe(name = "Index", value = "Volume")

ts_data <- ts(volume_values$Volume, start = c(2015, 1), frequency = 12)




# --------- Data exploration --------- #
# 3. Basic time‐series line plot
autoplot(ts_data) +
  labs(title = "Monthly Volume (2015–present)",
       x = "Year",
       y = "Volume") +
  theme_minimal()

# 4. Seasonal plots
ggseasonplot(ts_data, 
             year.labels = TRUE, 
             year.labels.left = TRUE) +
  labs(title = "Seasonal Plot: Volume by Month",
       y = "Volume") +
  theme_minimal()

# 5. Decomposition into trend + seasonal + remainder
decomp <- decompose(ts_data)
autoplot(decomp) +
  labs(title = "STL Decomposition of Volume",
       x = "Year") +
  theme_minimal()

# 6. tsdisplay: combines series, ACF, PACF in one view
tsdisplay(ts_data, lag.max = 36, main = "Time Series, ACF, PACF")




# --------- Tests for stationarity --------- #
# WITHOUT DIFFERENTIATION
# Augmented Dickey–Fuller test (null: unit root / non-stationary)
#Null hypothesis: the series has a unit root (i.e. is non-stationary).
#Alternative: the series is stationary.
#If p-value < 0.05, you reject the null → evidence in favor of stationarity.
adf_result <- adf.test(ts_data)

# WITH ONE DIFFERENTIATION
ts_data_diff_one <- diff(ts_data, differences = 1)
tsdisplay(ts_data_diff_one, lag.max = 36, main = "Time Series, ACF, PACF")
adf_result_one_diff <- adf.test(ts_data_diff_one)
print(adf_result_one_diff)

# Very clear seasonality after one differentiation also stationary just as the non differentiated data 
# (according to ADF test)




# -------- Preparation of train and test data --------- #
# Number of total observations
n <- length(ts_data)
h <- 6

# 1. Create training set: all but last 6
# train_ts <- window(ts_data, end = time(ts_data)[n - 6])
train_ts <- window(ts_data, end = c(2015 + (n - h - 1) %/% 12, (n - h - 1) %% 12 + 1))


# 2. Create validation (test) set: the last 6 obs
# valid_ts <- window(ts_data, start = time(ts_data)[n - 5])
valid_ts <- window(ts_data, start = c(2015 + (n - h) %/% 12, (n - h) %% 12 + 1))

lambda <- BoxCox.lambda(train_ts)              # Find optimal lambda
train_ts_trans <- BoxCox(train_ts, lambda)     # Transform training data
valid_ts_trans <- BoxCox(valid_ts, lambda)       # Transform test data

autoplot(train_ts, series = "Train") +
  autolayer(valid_ts, series = "Validation") +
  labs(title = "Train / Validation Split",
       x = "Year", y = "Volume") +
  scale_colour_manual(values = c("Train" = "black", "Validation" = "red")) +
  theme_minimal()




# --------- HELPER FOR ALL VALIDATION STUFF --------- #
# Function to perform residual analysis for a single AR model
validate_ar_model <- function(model, model_name) {
  cat("\n==================================================\n")
  cat(paste0("Validation for ", model_name), "\n")
  cat("==================================================\n")
  
  # Extract residuals
  res <- residuals(model)
  std_res <- res / sd(res)  # Standardized residuals
  
  # 1. QQ plots for standardized and raw residuals
  par(mfrow=c(1,2))
  qqPlot(res, main=paste0("QQ Plot - Raw Residuals (", model_name, ")"),
         xlab="Theoretical Quantiles", ylab="Sample Quantiles")
  qqPlot(std_res, main=paste0("QQ Plot - Std Residuals (", model_name, ")"),
         xlab="Theoretical Quantiles", ylab="Sample Quantiles")
  par(mfrow=c(1,1))
  
  # 2. ACF of residuals (visual check for whiteness)
  acf_res <- acf(res, main=paste0("ACF of Residuals (", model_name, ")"), 
                 lag.max=36, plot=TRUE)
  
  # 2.5 Histogram of residuals
  hist(res)
  
  # 3. Box-Ljung test for residual autocorrelation (whiteness)
  cat("\n1. Box-Ljung Test for Residual Whiteness\n")
  box_test <- Box.test(res, lag=min(10, length(res)/5), type="Ljung-Box", fitdf=length(model$coef)-1)
  # print(box_test)
  if (box_test$p.value > 0.05) {
    cat("✓ Nonsignificant autocorrelation\n")
  } else {
    cat("✗ Signifcant autocorrelation\n")
  }
  
  
  # --------- Normality tests --------- #
  cat("\n2.1 Jarque-Bera Test for Normality\n")
  jb_test <- jarque.bera.test(res)
  print(jb_test)
  if (jb_test$p.value > 0.05) {
    cat("✓ \n")
  } else {
    cat("✗ \n")
  }
  
  cat("\n2.2 Shapiro-Wilks Test for Normality\n")
  sw_test <- shapiro.test(res)
  print(sw_test)
  if (sw_test$p.value > 0.05) {
    cat("✓ \n")
  } else {
    cat("✗ \n")
  }
  
  cat("\n2.3 Anderson-Darling Test for Normality\n")
  ad_test <- ad.test(res)
  print(ad_test)
  if (ad_test$p.value > 0.05) {
    cat("✓ \n")
  } else {
    cat("✗ \n")
  }
  
  
  # --------- Heteroscedasticity tests (constant variance) --------- #
  # 5. ARCH test for heteroscedasticity (constant variance)
  cat("\n3. ARCH Test for Constant Variance\n")
  arch_test <- FinTS::ArchTest(res, lags = 10)
  # print(arch_test)
  if (arch_test$p.value > 0.05) {
    cat("✓ Constant variance\n")
  } else {
    cat("✗ Non constant variance\n")
  }
  
  
  # --------- AIC and BIC --------- #
  # 6. Information criteria
  cat("\n4. Information Criteria\n")
  cat(paste0("AIC: ", round(AIC(model), 4), "\n"))
  cat(paste0("BIC: ", round(BIC(model), 4), "\n"))
  cat(paste0("AICc: ", round(model$aicc, 4), "\n"))
  
  # Summary assessment
  cat("\n5. Overall Assessment\n")
  passed_tests <- 0
  if (box_test$p.value > 0.05) passed_tests <- passed_tests + 1
  if (jb_test$p.value > 0.05) passed_tests <- passed_tests + 1
  if (sw_test$p.value > 0.05) passed_tests <- passed_tests + 1
  if (ad_test$p.value > 0.05) passed_tests <- passed_tests + 1
  if (arch_test$p.value > 0.05) passed_tests <- passed_tests + 1
  
  if (passed_tests >= 3) {
    cat(paste0("Model passes ", passed_tests, "/3 diagnostic tests.\n"))
    return(TRUE)
  } else {
    cat("✗ Model fails all diagnostic tests! n")
    return(FALSE)
  }
}




# --------- AR --------- #
# Fit AR models of different orders using Arima()
ar1 <- Arima(train_ts, order=c(1,0,0))
ar2 <- Arima(train_ts, order=c(2,0,0))
ar3 <- Arima(train_ts, order=c(3,0,0))
ar6 <- Arima(train_ts, order=c(6,0,0))

# See if covariates are significant
coeftest(ar1) # All significant
coeftest(ar2) # All significant
coeftest(ar3) # Still only ar2
coeftest(ar6) # Still only ar2

# Validate AR models
validate_ar_model(ar1, "AR1")
validate_ar_model(ar2, "AR2") 
validate_ar_model(ar3, "AR3") 
validate_ar_model(ar6, "AR6")

# None pass normality so lets test using boxcox transformation
ar1_coxed <- Arima(train_ts_trans, order = c(1,0,0))
ar2_coxed <- Arima(train_ts_trans, order = c(2,0,0))
ar3_coxed <- Arima(train_ts_trans, order = c(3,0,0))
ar6_coxed <- Arima(train_ts_trans, order = c(6,0,0), fixed = c(NA, 0, NA, 0, 0, 0, NA))

# Validate significance
coeftest(ar1_coxed) # All significant
coeftest(ar2_coxed) # All significant
coeftest(ar3_coxed) # Still only ar2
coeftest(ar6_coxed) # Still only ar2

# Validate coxed AR models
validate_ar_model(ar1_coxed, "AR1 Coxed")
validate_ar_model(ar2_coxed, "AR2 Coxed")
validate_ar_model(ar3_coxed, "AR3 Coxed")
validate_ar_model(ar6_coxed, "AR6 Coxed")

# Forecast next 6 periods with the coxed AR models
forecast_ar1_coxed <- forecast::forecast(ar1_coxed, h = 6)
forecast_ar2_coxed <- forecast::forecast(ar2_coxed, h = 6)
forecast_ar3_coxed <- forecast::forecast(ar3_coxed, h = 6)
forecast_ar6_coxed <- forecast::forecast(ar6_coxed, h = 6)

# Plot forecast vs validation data
autoplot(forecast_ar1_coxed) +
  autolayer(valid_ts_trans, series = "Validation") +
  labs(title = "AR(1) with box-cox transformation",
       x = "Year", y = "Volume") +
  scale_colour_manual(values = c("Forecast" = "blue", "Validation" = "red")) +
  theme_minimal()

autoplot(forecast_ar2_coxed) +
  autolayer(valid_ts_trans, series = "Validation") +
  labs(title = "AR(2) with box-cox transformation",
       x = "Year", y = "Volume") +
  scale_colour_manual(values = c("Forecast" = "blue", "Validation" = "red")) +
  theme_minimal()

autoplot(forecast_ar3_coxed) +
  autolayer(valid_ts_trans, series = "Validation") +
  labs(title = "AR(3) with box-cox transformation",
       x = "Year", y = "Volume") +
  scale_colour_manual(values = c("Forecast" = "blue", "Validation" = "red")) +
  theme_minimal()

autoplot(forecast_ar6_coxed) +
  autolayer(valid_ts_trans, series = "Validation") +
  labs(title = "AR(6) with box-cox transformation and only significant covariates (1,3, intercept)",
       x = "Year", y = "Volume") +
  scale_colour_manual(values = c("Forecast" = "blue", "Validation" = "red")) +
  theme_minimal()

# Summary of them too
summary(ar1_coxed)
summary(ar2_coxed)
summary(ar3_coxed)
summary(ar6_coxed)




# --------- ARMA with seasonal shit --------- #
# Full cosine wave with 12-month period (monthly seasonality)
cos_t <- cos(2 * pi * (1:length(ts_data)) / 12)
sin_t <- sin(2 * pi * (1:length(ts_data)) / 12)

matrix_sincos = cbind(cos_t, sin_t)

# Split cosine regressor to match training and validation sets
xreg_train <- matrix_sincos[1:length(train_ts),]
xreg_test  <- matrix_sincos[(length(train_ts) + 1):length(ts_data),]

# Fit ARMA(1,2) model with cosine seasonal regressor
fit3_v1 <- Arima(train_ts, order = c(1, 0, 2), xreg = xreg_train[,2])
fit3_v2 <- Arima(train_ts, order = c(2, 0, 6), xreg = xreg_train[,2], fixed=c(NA, NA, NA, NA, 0, 0, NA, NA, NA, NA))
fit3_v3 <- Arima(train_ts, order = c(2, 0, 3), xreg = xreg_train[,2])

# Coeftest to see significance
coeftest(fit3_v1)
coeftest(fit3_v2)
coeftest(fit3_v3)

# Validate residuals
validate_ar_model(fit3_v1, "ARMA(1,0,2)+SEASONAL")
validate_ar_model(fit3_v2, "ARMA(2,0,6)+SEASONAL-Bad Covariates")
validate_ar_model(fit3_v3, "ARMA(2,0,3)+SEASONAL(only sin)")

# Test with box-cox transformation
fit3_v1_coxed = Arima(train_ts_trans, order = c(1, 0, 2), xreg = xreg_train[,2])
fit3_v2_coxed = Arima(train_ts_trans, order = c(2, 0, 3), xreg = xreg_train)
fit3_v3_coxed = Arima(train_ts_trans, order = c(2, 0, 2), xreg = xreg_train, fixed = c(NA, 0, 0, NA, NA, NA, NA))

# Coeftest to see significance
coeftest(fit3_v1_coxed)
coeftest(fit3_v2_coxed)
coeftest(fit3_v3_coxed)

# Validate residuals
validate_ar_model(fit3_v1_coxed, "ARMA(1,0,2)+SEASONAL-COXED")
validate_ar_model(fit3_v2_coxed, "ARMA(2,0,3)+SEASONAL-COXED")
validate_ar_model(fit3_v3_coxed, "ARMA(2,0,2)+SEASONAL-COXED")

# Forecast next 6 periods
forecast_fit3_v1_coxed <- forecast::forecast(fit3_v1_coxed, xreg = xreg_test[,2], h = 6)
forecast_fit3_v2_coxed <- forecast::forecast(fit3_v2_coxed, xreg = xreg_test, h = 6)
forecast_fit3_v3_coxed <- forecast::forecast(fit3_v3_coxed, xreg = xreg_test, h = 6)

# Plot forecast vs actual for 1,0,2
autoplot(forecast_fit3_v1_coxed) +
  autolayer(valid_ts_trans, series = "Validation") +
  labs(title = "ARMA(1,0,2) with Sine Seasonality and box-cox",
       x = "Year", y = "Volume") +
  scale_colour_manual(values = c("Forecast" = "blue", "Validation" = "red")) +
  theme_minimal()

# Plot forecast vs actual for 2,0,3
autoplot(forecast_fit3_v2_coxed) +
  autolayer(valid_ts_trans, series = "Validation") +
  labs(title = "ARMA(2,0,6) with Cosine and Sine Seasonality and box-cox",
       x = "Year", y = "Volume") +
  scale_colour_manual(values = c("Forecast" = "blue", "Validation" = "red")) +
  theme_minimal()

# Plot forecast vs actual for 2,0,2
autoplot(forecast_fit3_v3_coxed) +
  autolayer(valid_ts_trans, series = "Validation") +
  labs(title = "ARMA(2,0,2) with Cosine and Sine Seasonality and box-cox - insignificant covariates",
       x = "Year", y = "Volume") +
  scale_colour_manual(values = c("Forecast" = "blue", "Validation" = "red")) +
  theme_minimal()

# Summary for stats
summary(fit3_v1_coxed)
summary(fit3_v2_coxed)
summary(fit3_v3_coxed)




# --------- SARMA model --------- #
# Best sarma according to auto.arima
fit4 <- Arima(train_ts, order = c(2, 0, 2), seasonal = list(order = c(0, 1, 1), period = 12))

# Coeftest to see significance
coeftest(fit4)

# Validate
validate_ar_model(fit4, "SARMA(2,0,2)(0,1,1)[12]")

# Did not pass normality test. Lets apply box-cox here too
fit4_coxed <- Arima(train_ts_trans, order = c(2, 0, 2), seasonal = list(order = c(1, 0, 1), period = 12), fixed = c(NA, NA, 0, NA, NA, NA, NA))

# Validate significance
coeftest(fit4_coxed)

# Validate residuals
validate_ar_model(fit4_coxed, "SARMA(2,0,2)(1,0,1)[12]")

# Forecast next 6 months
forecast_fit4_coxed <- forecast::forecast(fit4_coxed, h = 6)

# Plot forecast vs validation data
autoplot(forecast_fit4_coxed) +
  autolayer(valid_ts_trans, series = "Validation") +
  labs(title = "SARMA(2,0,2)(1,0,1)[12] Forecast with box-cox",
       x = "Year", y = "Volume") +
  scale_colour_manual(values = c("Forecast" = "blue", "Validation" = "red")) +
  theme_minimal()

# Summary for stats
summary(fit4_coxed)


# --------- SARIMA (the other model) -------- #
fit5_sarima_coxed <- Arima(train_ts_trans, order = c(2, 0, 0), seasonal = list(order = c(2, 2, 1), period = 12))

# Coef test to see significance
coeftest(fit5_sarima_coxed)

# Validate dat ARIMA UwU
validate_ar_model(fit5_sarima_coxed, "SARIMA")

# Forecast next 6 periods
forecast_fit5_sarima_coxed <- forecast::forecast(fit5_sarima_coxed, h = 6)

# Plot forecast vs validation set
autoplot(forecast_fit5_sarima_coxed) +
  autolayer(valid_ts_trans, series = "Validation") +
  labs(title = "SARIMA",
       x = "Year", y = "Volume") +
  scale_colour_manual(values = c("Forecast" = "blue", "Validation" = "red")) +
  theme_minimal()

# Summary for stats
summary(fit5_sarima_coxed)
