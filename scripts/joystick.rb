#! /usr/bin/env ruby
require 'orocos'
include Orocos

Orocos::CORBA.name_service.ip = "192.168.128.31"

Orocos.initialize
puts("starting")
# spawn deployment
Orocos.run 'controldev::JoystickTask' => 'Joystick' do

    joy = TaskContext.get 'Joystick'

    joy.configure
    joy.start

    
    while(1)
    end

end
