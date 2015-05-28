#include "Remote.hpp"
#include <iostream>
#include <base/samples/CommandSamples.hpp>

using namespace controldev;

Remote::Remote(std::string const& name) :
    RemoteBase(name)
{
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Remote.hpp for more detailed
// documentation about them.

bool Remote::configureHook()
{

    return true;
}
// bool Remote::startHook() { return true; }

void Remote::updateHook()
{
    base::commands::Motion2D mcmd;

    canbus::Message msg;
    int currentbit;
   
    while (_canInputDevice.read(msg) == RTT::NewData){
        currentbit = 3;
        current_sequence_number = msg.data[0]>>5;

        if (!current_sequence_number){
            buttons = (int) (msg.data[0]);
            sliders = (int) (msg.data[1] >> 4);
            resolution = (((int) ((msg.data[1] & 0xC)>> 2))+1)*8;
	    //resolution = (int) (msg.data[1] &&0xC); 
            next_sequenznumber = 0;
            payloadbits = (5 + buttons + 4 + 2 + sliders*resolution);
            if (payloadbits%61)
                expected_sequences = payloadbits/61+1;
            else
                expected_sequences = payloadbits/61;
            currentbit = 14;
            current_button = 0;
            current_slider = 0;
            rcmd.axisValue.clear();
            rcmd.buttonValue.clear();
            rcmd.axisValue.push_back(std::vector<double>());
	    rcmd.axisValue.at(0).resize(sliders,0.0);
            rcmd.buttonValue.resize(buttons,0.0);
	    buffered_char = 0;

        }

        if (next_sequenznumber != current_sequence_number || current_sequence_number >= expected_sequences){
            bad_packages++;
            buffered_char = 0;
            buffered_charx = 0;
            current_slider=0;
            current_button=0;
            break;
        }



        while (currentbit < 64){
	    buffered_char = (buffered_char << 1) | ((msg.data[(currentbit)/8] >> (7-(currentbit%8))) & 0x01);
            buffered_charx++;
            if (current_button < buttons){
                rcmd.buttonValue.at(current_button) = (bool) buffered_char;
                printf("Writing to button: %i %i\n",current_button,buffered_char);
                buffered_char = 0;
                buffered_charx = 0;
                current_button++;
            } else if (current_slider < sliders){
                if (buffered_charx == (resolution)){
                    rcmd.axisValue.at(0).at(current_slider) = (buffered_char - 127)/100.0;
                    printf("Writing to axis value: %i %i %f\n",current_slider,buffered_char, rcmd.axisValue.at(0).at(current_slider));
                    buffered_char = 0;
                    buffered_charx = 0;
                    current_slider++;
                }
            } else {
                break;
            }
	    currentbit++;
            
        }
        /*
        for (int i; i < 6; i++){
            if (rcmd.buttonValue.at(i)){
            } else {
            }
        }
        */
        next_sequenznumber++;
        if (next_sequenznumber == expected_sequences){
            /*
            //hau raus
            struct base::actuators::Command cmd;
            cmd.resize(4);    
            if (rcmd.axisValue.at(0).size() >= 4){
                for (int i = 0; i < 4; i++){
                    cmd.target.at(i) =  (double) (((int) rcmd.axisValue.at(0).at(i))-127)/260;
                    cmd.mode.at(i) = base::actuators::DM_PWM;
                } 
            }
            _motor_command.write(cmd);
            */
            rcmd.time = base::Time::now();
            /*
	    for (int i = 0; i < sliders; i++){
                    rcmd.axisValue.at(0).at(i)=  (double) (((int) rcmd.axisValue.at(0).at(i))-127)/100;
                    printf("Writing out axis value: %i %i\n",i,rcmd.axisValue.at(0).at(i));
            }
            */
           _raw_command.write(rcmd);
        }else {
             return;
        }
    }
    // Send raw command
    //_raw_command.write(rcmd);
}


// void Remote::errorHook() {}
// void Remote::stopHook() {}
// void Remote::cleanupHook() {}

