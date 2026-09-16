#include "cosine.h"
#include <cmath>
#include <stdexcept>

double cosine_similarity(const std::vector<double>& a, const std::vector<double>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("向量维度不匹配");
    }
    
    size_t n = a.size();
    double dot = 0.0, norm_a = 0.0, norm_b = 0.0;
    
    size_t i = 0;
    for (; i + 4 <= n; i += 4) {
        dot += a[i] * b[i] + a[i+1] * b[i+1] + a[i+2] * b[i+2] + a[i+3] * b[i+3];
        norm_a += a[i] * a[i] + a[i+1] * a[i+1] + a[i+2] * a[i+2] + a[i+3] * a[i+3];
        norm_b += b[i] * b[i] + b[i+1] * b[i+1] + b[i+2] * b[i+2] + b[i+3] * b[i+3];
    }
    
    for (; i < n; ++i) {
        dot += a[i] * b[i];
        norm_a += a[i] * a[i];
        norm_b += b[i] * b[i];
    }
    
    if (norm_a == 0.0 || norm_b == 0.0) return 0.0;
    
    return dot / (std::sqrt(norm_a) * std::sqrt(norm_b));
}
