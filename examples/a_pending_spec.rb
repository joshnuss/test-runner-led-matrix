describe "all stuff that passes" do
  20.times do |n|
    it "should pass ##{n}" do
      true.should be_true
    end
  end

  it "should be pending"

  it "should also be pending" do
    pending
  end

  20.times do |n|
    it "should pass ##{n}" do
      true.should be_true
    end
  end
end
