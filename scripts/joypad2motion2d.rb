#! /usr/bin/env ruby
require 'orocos'
include Orocos

Orocos.initialize

maxSpeed = 0.4 #SeekurJr=1.5 m/s
maxJoyRotSpeed = 50 # degree/s

Orocos.run 'controldev::JoyPadTask' => 'joypad', 'controldev::GenericRawToMotion2D' => 'raw2motion' do

joypad = TaskContext.get 'joypad'
raw2motion = TaskContext.get 'raw2motion'

# joystick configuration
joypad.device = "/dev/input/js0"
raw2motion.maxSpeed = maxSpeed # m/s
raw2motion.maxRotationSpeed = maxJoyRotSpeed * Math::PI/180 # rad/s
raw2motion.translation_axis = [0,1]
raw2motion.rotation_axis = [0,0]
raw2motion.translation_axis_deadzone = 0.05
raw2motion.rotation_axis_deadzone = 0.05

joypad.raw_command.connect_to raw2motion.raw_command

joypad.configure
raw2motion.configure

joypad.start
raw2motion.start

Orocos.watch(joypad, raw2motion)

end
