// rice
#include <rice/rice.hpp>
#include <rice/stl.hpp>

std::vector<int> EDM_multi(const std::vector<double>& Z, int min_size = 24, double beta = 0, int degree = 0);
std::vector<int> EDM_percent(const std::vector<double>& Z, int min_size = 24, double percent = 0, int degree = 0);
std::vector<int> EDM_tail(std::vector<double>& Z, int min_size = 24, double alpha = 2, double quant = 0.5);
std::vector<int> EDMX(const std::vector<double>& Z, int min_size = 24, double alpha = 2);

extern "C"
void Init_ext() {
  auto rb_mBreakout = Rice::define_module("Breakout");

  rb_mBreakout
    .define_singleton_function(
      "_detect",
      [](std::vector<double> z, int min_size, const std::string& method, double alpha, std::optional<double> beta, int degree, std::optional<double> percent, bool exact) {
        auto minmax = std::minmax_element(z.begin(), z.end());
        auto min = *minmax.first;
        auto max = *minmax.second;
        auto diff = max - min;
        if (diff == 0) {
          // constant series
          return Rice::Array();
        }
        for (auto i = 0; i < z.size(); i++) {
          z[i] = (z[i] - min) / diff;
        }

        std::vector<int> res;
        if (method == "amoc") {
          if (exact) {
            res = EDMX(z, min_size, alpha);
          } else {
            res = EDM_tail(z, min_size, alpha);
          }
        } else {
          if (percent.has_value()) {
            res = EDM_percent(z, min_size, *percent, degree);
          } else {
            res = EDM_multi(z, min_size, beta.value_or(0.008), degree);
          }
        }

        auto a = Rice::Array();
        for (auto v : res) {
          a.push(v);
        }
        return a;
      });
}
