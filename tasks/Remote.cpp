#include "Remote.hpp"
#include <iostream>
#include <base/samples/CommandSamples.hpp>

using namespace controldev;

Remote::Remote(std::string const& name) :
    RemoteBase(name)
{
}


int Remote::getFileDescriptor()
{
    return -1;
}

bool Remote::updateRawCommand(RawCommand& rcmd_out)
{
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
            rcmd.axisValue.resize(sliders,0.0);
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
                    rcmd.axisValue.at(current_slider) = (buffered_char - 127)/100.0;
                    printf("Writing to axis value: %i %i %f\n",current_slider,buffered_char, rcmd.axisValue.at(current_slider));
                    buffered_char = 0;
                    buffered_charx = 0;
                    current_slider++;
                }
            } else {
                break;
            }
            currentbit++;
            
        }
        next_sequenznumber++;
        if (next_sequenznumber == expected_sequences){
            rcmd.time = base::Time::now();
            /*
            for (int i = 0; i < sliders; i++){
                    rcmd.axisValue.at(0).at(i)=  (double) (((int) rcmd.axisValue.at(0).at(i))-127)/100;
                    printf("Writing out axis value: %i %i\n",i,rcmd.axisValue.at(0).at(i));
            }
            */
            
            rcmd = rcmd_out;
            return true;
        }else {
             return false;
        }
    }
    
    return false;
}



