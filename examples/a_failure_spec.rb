describe "all stuff that passes" do
  20.times do |n|
    it "should pass ##{n}" do
      true.should be_true
    end
  end

  it "should fail" do
    true.should be_false
  end

  20.times do |n|
    it "should pass ##{n}" do
      true.should be_true
    end
  end
end
