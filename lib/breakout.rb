# ext
require "breakout/ext"

# modules
require "breakout/version"

module Breakout
  def self.detect(series, min_size: 30, method: "multi", alpha: 2, beta: nil, degree: 1, percent: nil, exact: true)
    raise ArgumentError, "min_size must be at least 2" if min_size < 2
    raise ArgumentError, "beta and percent cannot be passed together" unless beta.nil? || percent.nil?
    raise ArgumentError, "alpha must be between 0 and 2" if alpha < 0 || alpha > 2
    raise ArgumentError, "degree must be 0, 1, or 2" unless [0, 1, 2].include?(degree)
    raise ArgumentError, "Bad method" unless ["amoc", "multi"].include?(method)

    return [] if series.size < min_size

    if series.is_a?(Hash)
      sorted = series.sort_by { |k, _| k }
      z = sorted.map(&:last)
    else
      z = series
    end

    res = _detect(z, min_size, method, alpha, beta, degree, percent, exact)
    res.map! { |i| sorted[i][0] } if series.is_a?(Hash)
    res
  end
end
