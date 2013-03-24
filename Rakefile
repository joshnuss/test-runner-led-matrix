require 'rspec/core/rake_task'

RSpec::Core::RakeTask.new(:spec)

task :default => :spec

namespace :spec do
  %w(passing pending failing).each do |type|
    desc "run #{type} specs"
    task type do
      ENV['SPEC'] = "spec/#{type}_spec.rb"
      Rake::Task["spec"].invoke
    end
  end
end
