#! /usr/bin/env ruby
require 'orocos'
include Orocos

Orocos.initialize

Orocos.run 'controldev::GraupnerMC20Task' => 'mc20' do

    mc20 = TaskContext.get 'mc20'

    mc20.io_port = "serial:///dev/ttyUSB0:115200"
    mc20.configure
    mc20.start

    
    while(1)
    end

end
