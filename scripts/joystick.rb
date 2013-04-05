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
            if(raw.axisValue.size == 2)
                if(raw.axisValue[0].size == 3 and raw.axisValue[1].size == 1)
                    puts "#{raw.axisValue[0][0]} #{raw.axisValue[0][1]} #{raw.axisValue[0][2]} #{raw.axisValue[1][0]}"
                else
                    STDOUT.puts "Warning sizes ar invalud: #{raw.axisValue[0].size} #{raw.axisValue[1].size}"
                end
            else
                STDOUT.puts "Warning sizes ar invalud: #{raw.AxisValue.size}"
            end
        end
        if m2d = m2d_reader.read_new
            #puts "#{m2d.translation} #{m2d.rotation}"
        end
        sleep 0.1
    end

end
