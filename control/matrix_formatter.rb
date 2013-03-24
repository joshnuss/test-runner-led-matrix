require 'rspec/core/formatters/base_formatter'
require 'drb/drb'

class MatrixFormatter < RSpec::Core::Formatters::BaseFormatter
  def initialize(output)
    @server = DRbObject.new_with_uri('druby://localhost:8787')
    super
  end

  def start(example_count)
    super
    @server.display_tests
  end

  def stop
    super
    @server.display_status
  end

  def example_started(example)
    @server.test(:start)
  end

  def example_passed(example)
    @server.test(:pass)
  end

  def example_failed(example)
    @server.test(:fail)
  end

  def example_pending(example)
    @server.test(:pending)
  end
end
