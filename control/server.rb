require 'bundler'
Bundler.setup(:default)

require 'drb/drb'
require 'serialport'

# The URI for the server to connect to
SERVICE_URI = "druby://localhost:8787"

class MatrixDisplayServer
  BAUD_RATE = 9600

  def initialize(device)
    @port = SerialPort.new(device, BAUD_RATE)
  end

  def display_tests
    message(:mode, :test)
  end

  def display_status
    message(:mode, :suite)
  end

  def suite(command) 
    message(:suite, command)
  end

  def test(command) 
    message(:test, command)
  end

private
  def message(namespace, command)
    data = [namespace, command].map(&:to_s).map(&:upcase).join(',')

    puts data

    @port.write(data)

    nil
  end
end

DRb.start_service(SERVICE_URI, MatrixDisplayServer.new(ARGV[0]))
DRb.thread.join
