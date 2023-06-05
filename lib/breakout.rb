# ext
require "breakout/ext"

# modules
require_relative "breakout/version"

module Breakout
  class << self
    def detect(series, min_size: 30, method: "multi", alpha: 2, beta: nil, degree: 1, percent: nil, exact: true)
      raise ArgumentError, "min_size must be at least 2" if min_size < 2
      raise ArgumentError, "beta and percent cannot be passed together" unless beta.nil? || percent.nil?
      raise ArgumentError, "alpha must be between 0 and 2" if alpha < 0 || alpha > 2
      raise ArgumentError, "degree must be 0, 1, or 2" unless [0, 1, 2].include?(degree)
      raise ArgumentError, "method must be amoc or multi" unless ["amoc", "multi"].include?(method)

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

    def plot(series, breakouts)
      require "vega"

      data =
        if series.is_a?(Hash)
          series.map { |k, v| {x: iso8601(k), y: v, breakout: breakouts.include?(k)} }
        else
          series.map.with_index { |v, i| {x: i, y: v, breakout: breakouts.include?(i)} }
        end

      if series.is_a?(Hash)
        x = {field: "x", type: "temporal"}
        x["scale"] = {type: "utc"} if series.keys.first.is_a?(Date)
      else
        x = {field: "x", type: "quantitative"}
      end

      Vega.lite
        .data(data)
        .layer([
          {
            mark: {type: "line"},
            encoding: {
              x: x,
              y: {field: "y", type: "quantitative", scale: {zero: false}},
              color: {value: "#fa9088"}
            }
          },
          {
            transform: [{"filter": "datum.breakout == true"}],
            mark: {type: "rule"},
            encoding: {
              x: x,
              color: {value: "#19c7ca"},
              strokeWidth: {value: 2},
              strokeDash: {value: [6, 6]}
            }
          }
        ])
        .config(axis: {title: nil, labelFontSize: 12})
    end

    private

    def iso8601(v)
      if v.is_a?(Date)
        v.strftime("%Y-%m-%d")
      else
        v.strftime("%Y-%m-%dT%H:%M:%S.%L%z")
      end
    end
  end
end
