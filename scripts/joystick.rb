#! /usr/bin/env ruby
require 'orocos'
include Orocos

Orocos.initialize

puts("starting")
# spawn deployment
Orocos.run 'controldev::JoystickTask' => 'Joystick' do

    joy = TaskContext.get 'Joystick'

    joy.configure
    joy.start

    raw_reader = joy.raw_command.reader
    m2d_reader = joy.motion_command.reader
    
    while(1)
        if raw = raw_reader.read_new
            puts "#{raw.joyFwdBack} #{raw.joyLeftRight} #{raw.joyRotation} #{raw.joyThrottle}"
        end
        if m2d = m2d_reader.read_new
            #puts "#{m2d.translation} #{m2d.rotation}"
        end
        sleep 0.1
    end

end
