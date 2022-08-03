
#pragma once

#include "pax_codecs.h"
#include "pax_gfx.h"

extern pax_buf_t *benchmark_buf;

typedef bool (*benchfunc_t)(pax_buf_t *buf, int iteration, void *args);

typedef struct {
	float    per_second;
	double   average_time;
	int      total_iterations;
	uint64_t runtime_us;
	uint64_t runtime_ms;
	bool     success;
} benchres_t;

// Calibrate the benchmark system to improve accuracy.
void bench_calibrate();
// Run a benchmark for about 2 seconds or at least 10 iterations.
benchres_t bench_run(benchfunc_t func, void *args);
// Dump benchmark results to log.
void bench_log_res(const char *TAG, benchres_t res);
