#!/usr/bin/env Rscript

# ------------------------------------------------------------
# extract_results_metrics.R
# Extracts the current-clamp (IC) and voltage-clamp (VC) values
# used in dissertation Results section.
#
# Expected input files:
#   IC_raw_data.csv
#   VC_raw_data.csv
#
# Usage examples:
#   Rscript extract_results_metrics.R
#   Rscript extract_results_metrics.R /path/to/IC_raw_data.csv /path/to/VC_raw_data.csv
# ------------------------------------------------------------

args <- commandArgs(trailingOnly = TRUE)

ic_path <- if (length(args) >= 1) args[1] else "IC_raw_data.csv"
vc_path <- if (length(args) >= 2) args[2] else "VC_raw_data.csv"

out_dir <- "extracted_results"
if (!dir.exists(out_dir)) dir.create(out_dir, recursive = TRUE)

read_csv_base <- function(path) {
  if (!file.exists(path)) {
    stop(paste0("File not found: ", path,
                "\nEither place the file in the working directory or pass the full path to Rscript."))
  }
  read.csv(path, stringsAsFactors = FALSE, check.names = FALSE)
}

last_non_na <- function(x) {
  x <- suppressWarnings(as.numeric(x))
  x <- x[!is.na(x)]
  if (length(x) == 0) return(NA_real_)
  tail(x, 1)
}

first_non_na <- function(x) {
  x <- suppressWarnings(as.numeric(x))
  x <- x[!is.na(x)]
  if (length(x) == 0) return(NA_real_)
  x[1]
}

round_sig <- function(x, digits = 4) {
  ifelse(is.na(x), NA, signif(x, digits))
}

cat("Reading IC data from:", ic_path, "\n")
ic <- read_csv_base(ic_path)

cat("Reading VC data from:", vc_path, "\n")
vc <- read_csv_base(vc_path)

# ------------------------------------------------------------
# IC extraction
# ------------------------------------------------------------
ic_sweep <- subset(ic, row_type == "sweep")
ic_bin   <- subset(ic, row_type == "binned_mean")
ic_ov    <- subset(ic, row_type == "overlay_peak_0p50nA")

# 1) Maximum mean firing rate per condition
ic_max_firing_rate <- do.call(rbind, lapply(split(ic_sweep, ic_sweep$condition), function(df) {
  df <- df[!is.na(df$firing_rate_hz), ]
  row <- df[which.max(df$firing_rate_hz), ]
  data.frame(
    condition = row$condition,
    max_firing_rate_hz = row$firing_rate_hz,
    current_nA = row$current_nA,
    stringsAsFactors = FALSE
  )
}))

# 2) AP1 peaks at 0.50 and 0.90 nA
ic_ap1_peaks <- subset(
  ic_sweep,
  current_nA %in% c(0.50, 0.90),
  select = c("condition", "current_nA", "firing_rate_hz", "frequency_bin", "ap1_peak_mV")
)

# 3) Firing rate at 0.45 nA
ic_rates_0p45 <- subset(
  ic_sweep,
  current_nA == 0.45,
  select = c("condition", "current_nA", "firing_rate_hz", "frequency_bin")
)

# 4) WT F-I summary
wt_sweep <- subset(ic_sweep, condition == "WT" & !is.na(firing_rate_hz))
wt_nonzero <- subset(wt_sweep, firing_rate_hz > 0)
wt_first_spike <- wt_nonzero[which.min(wt_nonzero$current_nA), ]
wt_max <- wt_sweep[which.max(wt_sweep$firing_rate_hz), ]

ic_wt_fi_summary <- data.frame(
  min_firing_rate_hz = wt_first_spike$firing_rate_hz,
  lowest_current_with_spiking_nA = wt_first_spike$current_nA,
  max_firing_rate_hz = wt_max$firing_rate_hz,
  max_current_nA = wt_max$current_nA,
  stringsAsFactors = FALSE
)

# Include the final two current steps if present, useful for plateau/highest-current wording.
for (current_value in c(0.80, 0.85, 0.90)) {
  colname <- paste0("rate_at_", gsub("\\.", "p", sprintf("%.2f", current_value)), "_nA")
  row <- subset(wt_sweep, abs(current_nA - current_value) < 1e-9)
  ic_wt_fi_summary[[colname]] <- if (nrow(row) > 0) row$firing_rate_hz[1] else NA_real_
}

# 5) Binned AP amplitude, width and instantaneous frequency summaries
inst_cols <- paste0("inst_freq_isi", 1:10, "_hz")
existing_inst_cols <- inst_cols[inst_cols %in% names(ic_bin)]

ic_binned_summary <- do.call(rbind, lapply(seq_len(nrow(ic_bin)), function(i) {
  row <- ic_bin[i, ]
  inst_vals <- as.numeric(row[, existing_inst_cols, drop = TRUE])
  first_if <- first_non_na(inst_vals)
  last_if <- last_non_na(inst_vals)

  data.frame(
    condition = row$condition,
    frequency_bin = row$frequency_bin,
    n_sweeps_in_bin = row$n_sweeps_in_bin,

    ap1_amplitude_mV = row$ap1_amplitude_mV,
    ap5_amplitude_mV = row$ap5_amplitude_mV,
    amplitude_reduction_AP1_to_AP5_mV = row$ap1_amplitude_mV - row$ap5_amplitude_mV,

    ap1_third_width_ms = row$ap1_third_width_ms,
    ap5_third_width_ms = row$ap5_third_width_ms,
    width_increase_AP1_to_AP5_ms = row$ap5_third_width_ms - row$ap1_third_width_ms,

    inst_freq_first_hz = first_if,
    inst_freq_last_hz = last_if,
    inst_freq_reduction_hz = first_if - last_if,
    stringsAsFactors = FALSE
  )
}))

# 6) IC overlay peaks at 0.50 nA for Ca and BK currents
wanted_ic_signals <- c(
  "ca_total", "cav12", "cav13", "cav21", "cav22", "cav32",
  "bk_total", "bk12", "bk21", "bk22"
)

ic_overlay_peaks <- subset(
  ic_ov,
  signal %in% wanted_ic_signals,
  select = c("condition", "signal", "peak_value", "peak_time_ms")
)

# ------------------------------------------------------------
# VC extraction
# ------------------------------------------------------------
# column names match the current VC_raw_data.csv generated by the dissertation VC script.
ca_metrics <- c("total_ca_peak", "cav12_peak", "cav21_peak", "cav22_peak")
bk_late_metrics <- c("bk_total_late", "bk12_late", "bk21_late", "bk22_late")
bk_peak_metrics <- c("bk_total_peak", "bk12_peak", "bk21_peak", "bk22_peak")

extract_vc_extreme <- function(df, metric, mode = c("min", "max")) {
  mode <- match.arg(mode)
  vals <- df[[metric]]
  if (all(is.na(vals))) {
    return(data.frame(value = NA_real_, voltage_mV = NA_real_))
  }
  idx <- if (mode == "min") which.min(vals) else which.max(vals)
  data.frame(value = vals[idx], voltage_mV = df$v_step_mV[idx])
}

vc_peak_calcium <- do.call(rbind, lapply(ca_metrics, function(metric) {
  do.call(rbind, lapply(unique(vc$condition), function(cond) {
    df <- subset(vc, condition == cond)
    ext <- extract_vc_extreme(df, metric, mode = "min")
    data.frame(
      metric = metric,
      condition = cond,
      value_mA_per_cm2 = ext$value,
      voltage_mV = ext$voltage_mV,
      stringsAsFactors = FALSE
    )
  }))
}))

vc_late_bk <- do.call(rbind, lapply(bk_late_metrics, function(metric) {
  do.call(rbind, lapply(unique(vc$condition), function(cond) {
    df <- subset(vc, condition == cond)
    ext <- extract_vc_extreme(df, metric, mode = "max")
    data.frame(
      metric = metric,
      condition = cond,
      value_mA_per_cm2 = ext$value,
      voltage_mV = ext$voltage_mV,
      stringsAsFactors = FALSE
    )
  }))
}))

# peak BK values, not necessarily reported in the final text.
existing_bk_peak_metrics <- bk_peak_metrics[bk_peak_metrics %in% names(vc)]
vc_peak_bk <- do.call(rbind, lapply(existing_bk_peak_metrics, function(metric) {
  do.call(rbind, lapply(unique(vc$condition), function(cond) {
    df <- subset(vc, condition == cond)
    ext <- extract_vc_extreme(df, metric, mode = "max")
    data.frame(
      metric = metric,
      condition = cond,
      value_mA_per_cm2 = ext$value,
      voltage_mV = ext$voltage_mV,
      stringsAsFactors = FALSE
    )
  }))
}))

# ------------------------------------------------------------
# Write outputs
# ------------------------------------------------------------
write.csv(ic_max_firing_rate, file.path(out_dir, "IC_max_firing_rate_by_condition.csv"), row.names = FALSE)
write.csv(ic_ap1_peaks, file.path(out_dir, "IC_AP1_peaks_0p50_0p90.csv"), row.names = FALSE)
write.csv(ic_rates_0p45, file.path(out_dir, "IC_rates_at_0p45nA.csv"), row.names = FALSE)
write.csv(ic_wt_fi_summary, file.path(out_dir, "IC_WT_FI_summary.csv"), row.names = FALSE)
write.csv(ic_binned_summary, file.path(out_dir, "IC_binned_AP_metrics.csv"), row.names = FALSE)
write.csv(ic_overlay_peaks, file.path(out_dir, "IC_overlay_peaks_0p50nA.csv"), row.names = FALSE)
write.csv(vc_peak_calcium, file.path(out_dir, "VC_peak_calcium_currents.csv"), row.names = FALSE)
write.csv(vc_late_bk, file.path(out_dir, "VC_late_BK_currents.csv"), row.names = FALSE)
if (!is.null(vc_peak_bk) && nrow(vc_peak_bk) > 0) {
  write.csv(vc_peak_bk, file.path(out_dir, "VC_peak_BK_currents_optional_check.csv"), row.names = FALSE)
}

# Combined text-friendly summary for quick inspection.
cat("\n================ IC: max firing rate ================\n")
print(ic_max_firing_rate)

cat("\n================ IC: AP1 peaks at 0.50 and 0.90 nA ================\n")
print(ic_ap1_peaks)

cat("\n================ IC: rates at 0.45 nA ================\n")
print(ic_rates_0p45)

cat("\n================ IC: WT F-I summary ================\n")
print(ic_wt_fi_summary)

cat("\n================ IC: binned AP metrics ================\n")
print(ic_binned_summary)

cat("\n================ IC: overlay peaks at 0.50 nA ================\n")
print(ic_overlay_peaks)

cat("\n================ VC: peak calcium currents ================\n")
print(vc_peak_calcium)

cat("\n================ VC: late BK currents ================\n")
print(vc_late_bk)

cat("\nDone. Extracted CSV tables were written to:", normalizePath(out_dir), "\n")
