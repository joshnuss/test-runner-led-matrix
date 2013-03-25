require 'spec_helper'

describe "all stuff that passes" do
  20.times do |n|
    it "should pass ##{n}" do
      sleep_random
      true.should be_true
    end
  end

  it "should be pending"

  it "should also be pending" do
    sleep_random
    pending
  end

  20.times do |n|
    it "should pass ##{n}" do
      sleep_random
      true.should be_true
    end
  end
end
