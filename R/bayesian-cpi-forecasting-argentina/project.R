library(AER)
library(dplyr)
library(ggplot2)
library(brms)
library(bayesplot)
library(posterior)

# ---------------------------------------------------------
# Data Loading & Preprocessing
# ---------------------------------------------------------
data("ArgentinaCPI", package = "AER")

# Create main dataframe
df <- data.frame(
  time = as.numeric(time(ArgentinaCPI)),
  cpi  = as.numeric(ArgentinaCPI)
) %>%
  mutate(
    log_cpi = log(cpi),
    # Standardize for stable sampling (critical for brms)
    time_s = as.numeric(scale(time)),
    log_cpi_s = as.numeric(scale(log_cpi))
  )

# Store scaling factors for back-transformation
scales <- list(
  log_mean = mean(df$log_cpi), log_sd = sd(df$log_cpi),
  time_mean = mean(df$time),   time_sd = sd(df$time)
)

# ---------------------------------------------------------
# Exploratory Data Analysis (EDA)
# ---------------------------------------------------------
# Raw CPI
p1 <- ggplot(df, aes(x = time, y = cpi)) +
  geom_line() + labs(title = "Argentina CPI (Raw Scale)", y = "CPI")
print(p1)

# Raw Distribution
p2 <- ggplot(df, aes(x = cpi)) +
  geom_histogram(bins = 30, fill = "gray40") + labs(title = "Distribution of CPI (Raw)")
print(p2)

# Log CPI
p3 <- ggplot(df, aes(x = time, y = log_cpi)) +
  geom_line() + labs(title = "Argentina CPI (Log Scale)", y = "log(CPI)")
print(p3)

# Log Distribution
p4 <- ggplot(df, aes(x = log_cpi)) +
  geom_histogram(bins = 30, fill = "steelblue", alpha = 0.8) + 
  labs(title = "Distribution of log(CPI)")
print(p4)

# ---------------------------------------------------------
# Model 1: Linear Trend
# ---------------------------------------------------------
fit_lin <- brm(
  log_cpi_s ~ time_s,
  data = df,
  prior = c(prior(normal(0, 1), class = "Intercept"),
            prior(normal(0, 1), class = "b"),
            prior(student_t(3, 0, 1), class = "sigma")),
  chains = 4, iter = 4000, warmup = 1000, seed = 123
)

# ---------------------------------------------------------
# Linear Model Diagnostics
# ---------------------------------------------------------
# Figure 5: Parameter Summaries
# This checks convergence (fuzzy caterpillars) and posterior shape
plot(fit_lin) 

# Posterior Predictive Checks
pp_check(fit_lin) + labs(title = "Linear Model PPC (Density)")
pp_check(fit_lin, type = "hist") + labs(title = "Linear Model PPC (Histograms)")

# Residual Analysis
res_lin <- residuals(fit_lin)[, "Estimate"]

# Residual Histogram
ggplot(data.frame(res = res_lin), aes(x = res)) +
  geom_histogram(bins = 20, fill = "gray40", alpha = 0.7) +
  labs(title = "Residual Distribution (Linear Model)", x = "Standardized Residuals")

# ACF Plot
acf(res_lin, main = "ACF of Residuals (Linear Model)")

# Visual Fit Check
df$fitted_lin <- scales$log_mean + scales$log_sd * fitted(fit_lin)[, "Estimate"]
ggplot(df, aes(x = time)) +
  geom_line(aes(y = log_cpi)) +
  geom_line(aes(y = fitted_lin), color = "red", linetype = "dashed") +
  labs(title = "Observed vs Linear Fit", subtitle = "Systematic deviation at the end")

# ---------------------------------------------------------
# Model 2: Spline Trend
# ---------------------------------------------------------
fit_spline <- brm(
  log_cpi_s ~ s(time_s),
  data = df,
  prior = c(prior(normal(0, 1), class = "Intercept"),
            prior(student_t(3, 0, 1), class = "sigma")),
  control = list(adapt_delta = 0.99, max_treedepth = 15),
  chains = 4, iter = 4000, warmup = 1000, seed = 789
)

# ---------------------------------------------------------
# Spline Model Diagnostics
# ---------------------------------------------------------
# Posterior Predictive Checks
pp_check(fit_spline) + labs(title = "Spline Model PPC (Density)")
pp_check(fit_spline, type = "hist") + labs(title = "Spline Model PPC (Histograms)")

# Visual Comparison: Linear vs Spline
df$fitted_spline <- scales$log_mean + scales$log_sd * fitted(fit_spline)[, "Estimate"]

ggplot(df, aes(x = time)) +
  geom_line(aes(y = log_cpi, color = "Observed"), size = 1) +
  geom_line(aes(y = fitted_lin, color = "Linear (Underfit)"), linetype = "dashed") +
  geom_line(aes(y = fitted_spline, color = "Spline (Better)"), linetype = "solid") +
  scale_color_manual(values = c("Observed" = "black", "Linear (Underfit)" = "red", "Spline (Better)" = "blue")) +
  labs(title = "Fig 13: Model Comparison (Linear vs Spline)", y = "log(CPI)")

# ---------------------------------------------------------
# Forecasting (Using the Spline Model)
# ---------------------------------------------------------
# Create future time points (next 5 quaters)
h_quarters <- 5
future_time <- max(df$time) + (1:h_quarters) * 0.25
new_df <- data.frame(time = future_time) %>%
  mutate(time_s = (time - scales$time_mean) / scales$time_sd)

# Predictions (Spline)
pred_s <- posterior_predict(fit_spline, newdata = new_df)
pred_cpi <- exp(scales$log_mean + scales$log_sd * pred_s)

# Mean Trend (Spline)
epred_s <- posterior_epred(fit_spline, newdata = new_df)
epred_cpi <- exp(scales$log_mean + scales$log_sd * epred_s)

# Forecast Table
forecast_tab <- data.frame(
  Time = future_time,
  Forecast_Mean = colMeans(epred_cpi),
  Lower_95 = apply(pred_cpi, 2, quantile, 0.025),
  Upper_95 = apply(pred_cpi, 2, quantile, 0.975)
)

print("Forecast (Next 5 Quarters) --- Spline Model:")
print(forecast_tab)

# Final Forecast Plot
ggplot() +
  geom_line(data = df, aes(x = time, y = cpi), size = 1) +
  geom_ribbon(data = forecast_tab, aes(x = Time, ymin = Lower_95, ymax = Upper_95), 
              fill = "blue", alpha = 0.2) +
  geom_line(data = forecast_tab, aes(x = Time, y = Forecast_Mean), 
            color = "blue", linetype = "dotdash", size = 1) +
  labs(title = "Argentina CPI Forecast (Spline Model)", 
       subtitle = "Blue ribbon = 95% Predictive Interval; Dot-dash = Mean Trend",
       y = "CPI", x = "Time")
