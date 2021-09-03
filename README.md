# Breakout

:fire: [BreakoutDetection](https://github.com/twitter/BreakoutDetection) for Ruby

Learn more about [how it works](https://blog.twitter.com/engineering/en_us/a/2014/breakout-detection-in-the-wild)

[![Build Status](https://github.com/ankane/breakout/workflows/build/badge.svg?branch=master)](https://github.com/ankane/breakout/actions)

## Installation

Add this line to your application’s Gemfile:

```ruby
gem 'breakout-detection'
```

## Getting Started

Detect breakouts in a time series

```ruby
series = {
  Date.parse("2020-01-01") => 100,
  Date.parse("2020-01-02") => 150,
  Date.parse("2020-01-03") => 136,
  # ...
}

Breakout.detect(series)
```

Works great with [Groupdate](https://github.com/ankane/groupdate)

```ruby
series = User.group_by_day(:created_at).count
Breakout.detect(series)
```

Series can also be an array without times (the index is returned)

```ruby
series = [100, 150, 136, ...]
Breakout.detect(series)
```

## Options

Pass options - default values below

```ruby
Breakout.detect(
  series,
  min_size: 30,        # minimum observations between breakouts
  method: "multi",     # multi or amoc (at most one change)
  degree: 1,           # degree of the penalization polynomial (multi only)
  beta: 0.008,         # penalization term (multi only)
  percent: nil,        # minimum percent change in goodness of fit statistic (multi only)
  alpha: 2,            # weight of the distance between observations (amoc only)
  exact: true          # exact or approximate median (amoc only)
)
```

## Credits

This library uses the C++ code from the [BreakoutDetection](https://github.com/twitter/BreakoutDetection) R package and is available under the same license.

## References

- [Leveraging Cloud Data to Mitigate User Experience from ‘Breaking Bad’](https://arxiv.org/abs/1411.7955)

## History

View the [changelog](https://github.com/ankane/breakout/blob/master/CHANGELOG.md)

## Contributing

Everyone is encouraged to help improve this project. Here are a few ways you can help:

- [Report bugs](https://github.com/ankane/breakout/issues)
- Fix bugs and [submit pull requests](https://github.com/ankane/breakout/pulls)
- Write, clarify, or fix documentation
- Suggest or add new features

To get started with development:

```sh
git clone https://github.com/ankane/breakout.git
cd breakout
bundle install
bundle exec rake compile
bundle exec rake test
```
