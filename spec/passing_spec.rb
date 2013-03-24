describe "all stuff that passes" do
  100.times do |n|
    it "should pass ##{n}" do
      true.should be_true
    end
  end
end
