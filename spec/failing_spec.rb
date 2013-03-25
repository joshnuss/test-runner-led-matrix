require 'spec_helper'

describe "all stuff that passes" do
  20.times do |n|
    it "should pass ##{n}" do
      sleep_random
      true.should be_true
    end
  end

  it "should fail" do
    sleep_random
    true.should be_false
  end

  20.times do |n|
    it "should pass ##{n}" do
      sleep_random
      true.should be_true
    end
  end
end
