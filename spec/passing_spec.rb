require 'spec_helper'

describe "all stuff that passes" do
  100.times do |n|
    it "should pass ##{n}" do
      sleep_random
      true.should be_true
    end
  end
end
