#include "GenericTask.hpp"

using namespace controldev;

GenericTask::GenericTask(std::string const& name, TaskCore::TaskState initial_state)
    : GenericTaskBase(name, initial_state) {}



#define	SLIDER_MAX 250			/*   */
#define	SLIDER_MIN 5			/*   */
#define	VEL_MAX_PWM 0.25			/*   */
#define	VEL_MAX_PID 7.0			/*   */

#define	SLIDER_MASTER_VEL_LEFT 1			/*   */
#define	SLIDER_VEL_LEFT_FRONT 2			/*   */
#define	SLIDER_VEL_LEFT_REAR 3		/*   */

#define	SLIDER_MASTER_VEL_RIGHT 6			/*   */
#define	SLIDER_VEL_RIGHT_REAR 4			/*   */
#define	SLIDER_VEL_RIGHT_FRONT 5			/*   */

#define	SLIDER_ICR 0			/*   */



bool GenericTask::mapFromSliderbox(FourWheelCommand& cmd, RawCommand const& input)
{
	if (!(input.devices & controldev::DAI_SliderBox))
		return false;

	double sliderValues[7];
	for (int i = 0; i < 7; i++)
	{
		// Reads the slider value from MIN to MAX and scales it to 0 to 1
		sliderValues[i] = (( input.sliderValues[i]- SLIDER_MIN) / (SLIDER_MAX-SLIDER_MIN));

		// Rounds the data outside MIN and MAX
		if (sliderValues[i] > 1.0)
			sliderValues[i] = 1.0;
		else if (sliderValues[i] < 0.0)
			sliderValues[i] = 0.0;
	}

	/* OFF state */	
	if (! (input.sliderButtons & (1 << 3)) )/* Button 4 */
	{
		cmd = FourWheelCommand();
		return true;
	}

	float vel_max;   // Sets the velocity to be scaled in different modes

	/* Sets the control mode for each motor */
	for (int i=0;i<4;i++)
	{
		// Sets the mode based on status of Button 3
		if (input.sliderButtons & (1 << 2))  /* Button 3 */
		{
			/* PWM Mode */
			cmd.mode[i] = controldev::MODE_PWM;
			vel_max = VEL_MAX_PWM;
		}
		else
		{
			/* PID Mode */
			cmd.mode[i] = controldev::MODE_SPEED;
			vel_max = VEL_MAX_PID;
		}
	}

	/* Sets the driving mode and calulates the individual velocities*/

	/* Individual wheel and Master wheel set drive */
	if (input.sliderButtons & (1 << 1))   /* Button 2 */
	{

		cmd.target[3] = 2  * (sliderValues[SLIDER_MASTER_VEL_LEFT]  - 0.5) 
			* sliderValues[SLIDER_VEL_LEFT_FRONT]
			* vel_max;
		cmd.target[0] = 2  * (sliderValues[SLIDER_MASTER_VEL_LEFT]  - 0.5)
			* sliderValues[SLIDER_VEL_LEFT_REAR]
			* vel_max;
		cmd.target[1] = 2  * (sliderValues[SLIDER_MASTER_VEL_RIGHT] - 0.5)
			* sliderValues[SLIDER_VEL_RIGHT_REAR]
			* vel_max;
		cmd.target[2] = 2  * (sliderValues[SLIDER_MASTER_VEL_RIGHT] - 0.5)
			* sliderValues[SLIDER_VEL_RIGHT_FRONT]
			* vel_max;
	}
	/* ICR (Instantaneous Center of Rotation) drive */
	else
	{
		// If slider position is on the left half ... turn left
		if ( (sliderValues[SLIDER_ICR] ) <= 0.5 )					
		{
			// Master Vel * ICR position * max vel
			cmd.target[3] = 2 * (sliderValues[SLIDER_MASTER_VEL_LEFT] - 0.5) *
				4 * (sliderValues[SLIDER_ICR] - 0.25)
				* vel_max;
			cmd.target[0] = 2 * (sliderValues[SLIDER_MASTER_VEL_LEFT] - 0.5) *
				4 * (sliderValues[SLIDER_ICR] - 0.25)
				* vel_max;

			// Master vel * max vel
			cmd.target[1] = 2 * (sliderValues[SLIDER_MASTER_VEL_RIGHT] - 0.5) *
				vel_max;
			cmd.target[2] = 2 * (sliderValues[SLIDER_MASTER_VEL_RIGHT] - 0.5) *
				vel_max;
		}
		// If slider position is on the right half ... turn right
		else
		{
			// Master vel * max vel					
			cmd.target[3] = 2 * (sliderValues[SLIDER_MASTER_VEL_LEFT] - 0.5) *
				vel_max;
			cmd.target[0] = 2 * (sliderValues[SLIDER_MASTER_VEL_LEFT] - 0.5) *
				vel_max;

			// Master vel + ICR position * max vel
			cmd.target[1] = 2 * (sliderValues[SLIDER_MASTER_VEL_RIGHT] - 0.5) *
				4 * (-sliderValues[SLIDER_ICR] + 0.75)
				* vel_max;
			cmd.target[2] = 2 * (sliderValues[SLIDER_MASTER_VEL_RIGHT] - 0.5) *
				4 * (-sliderValues[SLIDER_ICR] + 0.75)
				* vel_max;
		}
	}


	if (input.sliderButtons & (1 << 0))   /* Button 1 */
	    cmd.sync = true;
	else 
	    cmd.sync = false;


	return true;
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See GenericTask.hpp for more detailed
// documentation about them.

// bool GenericTask::configureHook() { return true; }
// bool GenericTask::startHook() { return true; }

// void GenericTask::updateHook() {}

// void GenericTask::errorHook() {}
// void GenericTask::stopHook() {}
// void GenericTask::cleanupHook() {}

