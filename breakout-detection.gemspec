require_relative "lib/breakout/version"

Gem::Specification.new do |spec|
  spec.name          = "breakout-detection"
  spec.version       = Breakout::VERSION
  spec.summary       = "Breakout detection for Ruby"
  spec.homepage      = "https://github.com/ankane/breakout-ruby"
  spec.license       = "GPL-2.0-or-later"

  spec.author        = "Andrew Kane"
  spec.email         = "andrew@ankane.org"

  spec.files         = Dir["*.{md,txt}", "{ext,lib}/**/*"]
  spec.require_path  = "lib"
  spec.extensions    = ["ext/breakout/extconf.rb"]

  spec.required_ruby_version = ">= 3.2"

  spec.add_dependency "rice", ">= 4.7"
end
