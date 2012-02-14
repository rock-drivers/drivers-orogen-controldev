#! /usr/bin/env ruby
require 'orocos'
include Orocos

BASE_DIR = File.expand_path('..', File.dirname(__FILE__))
ENV['PKG_CONFIG_PATH'] = "#{File.join(BASE_DIR, 'build')}:#{ENV['PKG_CONFIG_PATH']}"
Orocos.initialize

puts("starting")
# spawn deployment
Orocos::Process.run 'orogen_default_controldev__SteeringWheelTask' do |p|
    last_gear = nil

    steeringWheel = p.task('orogen_default_controldev__SteeringWheelTask')

    steeringWheel.configure
    steeringWheel.start

    gear_reader = steeringWheel.raw_command.reader()

    while(1)
        sleep(0.1)
        
        cur_gear = gear_reader.read()
        
        if(cur_gear)
            #mask out other buttons
            gear = cur_gear.joyButtons # & 0x43F000
            case gear
            when 0
                puts("No gear")
            when (1 << 12)
                puts("Gear one")
            when (1 << 13)
                puts("Gear two")
            when (1 << 14)
                puts("Gear three")
            when (1 << 15)
                puts("Gear four")
            when (1 << 16)
                puts("Gear five")
            when (1 << 17)
              puts("Gear six")
            when (1 << 22)
                puts("Gear reverse")
            end
        end
    end
end
