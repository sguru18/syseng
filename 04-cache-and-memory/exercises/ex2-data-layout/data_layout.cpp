/* TODO: add your #include lines */

// ─── Part 1: Array of Structs (AoS) ──────────────────────────────────────────
//
// Memory layout for N=3 particles:
//   [x0 y0 z0 vx0 vy0 vz0 mass0 | x1 y1 z1 vx1 vy1 vz1 mass1 | ...]
//
// To process only positions (x, y, z) the CPU loads every field into cache,
// wasting 16 of 28 bytes per particle on velocity and mass.

struct ParticleAoS {
    float x, y, z;
    float vx, vy, vz;
    float mass;
};

// TODO: Iterate over all n particles. For each, compute sqrt(x*x + y*y + z*z)
//       and add it to a running sum. Call bench::do_not_optimize(sum) inside
//       the loop to prevent the compiler from hoisting or eliminating work.
//       Return the final sum.
float sum_distances_aos(const ParticleAoS* particles, size_t n) {
    return 0.0f;
}

// ─── Part 2: Struct of Arrays (SoA) ──────────────────────────────────────────
//
// Memory layout for N=3 particles:
//   x:    [x0 x1 x2 ...]
//   y:    [y0 y1 y2 ...]
//   z:    [z0 z1 z2 ...]
//   ...
//
// Processing positions only touches x, y, z arrays -- no wasted bandwidth.
// Consecutive x values are adjacent so a single cache line holds ~16 of them.

struct ParticlesSoA {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    std::vector<float> mass;
    size_t n;
};

// TODO: Same computation as sum_distances_aos but reading from particles.x[i],
//       particles.y[i], particles.z[i]. Call bench::do_not_optimize(sum) inside
//       the loop. Return the final sum.
float sum_distances_soa(const ParticlesSoA& particles) {
    return 0.0f;
}

// ─── Part 3: SIMD-vectorized SoA (ARM NEON) ──────────────────────────────────
//
// Process 4 particles per loop iteration. Reference intrinsics:
//
//   float32x4_t vx  = vld1q_f32(&particles.x[i]);   // load 4 floats
//   float32x4_t sq  = vmulq_f32(vx, vx);             // element-wise x*x
//   float32x4_t sum = vaddq_f32(sq_x, sq_y);         // element-wise add
//   float32x4_t sr  = vsqrtq_f32(sum);               // element-wise sqrt
//   float        s  = vaddvq_f32(sr);                 // horizontal sum
//
// TODO: Process 4 particles per iteration with NEON intrinsics.
//       For the tail (n % 4 remaining particles), fall back to scalar arithmetic.
//       Return the total sum of distances.
float sum_distances_simd(const ParticlesSoA& particles) {
    return 0.0f;
}

// ─── main ─────────────────────────────────────────────────────────────────────
int main() {
    const size_t N = 1'000'000;

    // TODO: Seed a random engine (e.g. std::mt19937) and fill N particles with
    //       positions in [-1.0f, 1.0f] and velocities in [-0.1f, 0.1f].
    //       Populate both `aos` (std::vector<ParticleAoS>) and `soa` (ParticlesSoA).

    // TODO: Run all three versions and verify they agree within 1%:
    //       float r_aos = sum_distances_aos(aos.data(), N);
    //       float r_soa = sum_distances_soa(soa);
    //       float r_sim = sum_distances_simd(soa);
    //       assert: std::abs(r_soa - r_aos) / r_aos < 0.01f  (etc.)

    // TODO: Benchmark all three using manual timing or bench::median_ns.
    //       Use at least 5 warm-up runs before the timed run.
    //       Compute ns_per_particle = elapsed_ns / N.
    //       Compute speedup = aos_ns_per / xxx_ns_per.

    printf("%-14s %14s %12s\n", "Version",     "ns/particle", "Speedup");
    printf("%-14s %14s %12s\n", "-----------", "-----------", "-------");
    // TODO: printf("%-14s %14.2f %11s\n",  "AoS",      aos_ns_per,  "-");
    // TODO: printf("%-14s %14.2f %10.1fx\n", "SoA",    soa_ns_per,  soa_speedup);
    // TODO: printf("%-14s %14.2f %10.1fx\n", "SoA+SIMD", sim_ns_per, sim_speedup);

    return 0;
}
