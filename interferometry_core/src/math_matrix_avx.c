#include <immintrin.h>
#include <stddef.h>

// Computes a 4x4 Transformation Matrix multiplication using AVX instructions
// This accelerates the 6-Degrees-of-Freedom (6DoF) spatial calculations for the mirrors
void multiply_4x4_matrices_avx(const float* A, const float* B, float* C) {
    for (int i = 0; i < 4; ++i) {
        // Broadcast the elements of the current row of A
        __m128 a0 = _mm_set1_ps(A[i * 4 + 0]);
        __m128 a1 = _mm_set1_ps(A[i * 4 + 1]);
        __m128 a2 = _mm_set1_ps(A[i * 4 + 2]);
        __m128 a3 = _mm_set1_ps(A[i * 4 + 3]);

        // Load the rows of B
        __m128 b0 = _mm_loadu_ps(&B[0 * 4]);
        __m128 b1 = _mm_loadu_ps(&B[1 * 4]);
        __m128 b2 = _mm_loadu_ps(&B[2 * 4]);
        __m128 b3 = _mm_loadu_ps(&B[3 * 4]);

        // Multiply and accumulate
        __m128 r = _mm_mul_ps(a0, b0);
        r = _mm_fmadd_ps(a1, b1, r); // Fused multiply-add
        r = _mm_fmadd_ps(a2, b2, r);
        r = _mm_fmadd_ps(a3, b3, r);

        // Store the result row into C
        _mm_storeu_ps(&C[i * 4], r);
    }
}
