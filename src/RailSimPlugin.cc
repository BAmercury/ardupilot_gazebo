
#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/sensors/sensors.hh>
#include <ignition/math4/ignition/math/Vector3.hh>
#include <stdio.h>
#include <math.h>
#include "include/RailSimPlugin.hh"
#include "Profile.hh"

using namespace gazebo;

GZ_REGISTER_MODEL_PLUGIN(RailSim)


/*RailSimPlugin
    Loads model and optRailSimPluginionally sdf of the model
    Also adds subscribeRailSimPluginr for World update Events
*/
void RailSim::Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
{
    // Store pointers for the model
    this->model = _parent;
    this->link = this->model->GetChildLink("rail_system::rail_system::link");

    // Contact sensor (Currently not used)
    // Create a sensor
    //sensors::SensorManager *manager = sensors::SensorManager::Instance();
    //this->sensor =  std::dynamic_pointer_cast<sensors::ContactSensor>(manager->GetSensor(this->sensor_name));
    //his->sensor->SetActive(true);

    // Get World Pointer
    this->world_ptr = this->model->GetWorld();

    // Go through SDF parameters to setup motion profile
    if (_sdf->HasElement("motion_profile"))
    {
       gzdbg << "Detected SDF Type" << std::endl;
       sdf::ElementPtr motion_sdf = _sdf->GetElement("motion_profile");
       if (motion_sdf->HasElement("type"))
       {
            this->motion_type = motion_sdf->Get<int>("type");
            // Get the delay time for Gazebo to wait
            this->delay_time = motion_sdf->Get<double>("delay_time");
            gzdbg << "Motion Type: " << motion_type << std::endl;
            if (this->motion_type == 1) // Velocity Sine Wave
            {
                // Get the rest of the parameters
                this->amplitude = motion_sdf->Get<double>("amplitude");
                this->max_velocity = motion_sdf->Get<double>("max_velocity");
                this->frequency_w = this->max_velocity / this->amplitude;
                this->direction = motion_sdf->Get<ignition::math::Vector2d>("direction");
                // Take direction and turn it into 3D Vector
                this->direction_3d = ignition::math::Vector3d(this->direction.X(), this->direction.Y(), 0);
                this->forward_direction = motion_sdf->Get<int>("forward_direction");
                this->forward_velocity = motion_sdf->Get<double>("forward_velocity");
            }
            else if (this->motion_type == 4) // Step Input
            {
                // Get the rest of the parameters
                this->target1_pos = motion_sdf->Get<double>("Target1");
                this->target1_hold = motion_sdf->Get<double>("Target1Hold");
                this->origin_pose = motion_sdf->Get<ignition::math::Pose3d>("pose");


            }
        }
    }


    /*
        worldUpdateBegin event is fired at each physics iteration
        
        ConnectWorldBegin takes in a callback for the triggered event
    */
    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
        std::bind(&RailSim::OnUpdate, this, std::placeholders::_1));
}
// Called by the world update start event
void RailSim::OnUpdate(const common::UpdateInfo &_info)
{


    // Wait 50 seconds for Ardupilot to start up
    if (_info.simTime.Double() < this->delay_time)
    {
        // Apply no Velocity
        this->model->SetLinearVel(ignition::math::Vector3d(0,0,0));


        // If we are reading from some type of static motion profile, reset all the control variables
        // Also if we are using motion type 4, reset the timer values
        if (this->motion_type == 2 || this->motion_type == 4)
        {
            // Reset the control variables
            this->setup_bool = false;
        }
        else if (this->motion_type == 1)
        {
            // Set this initial position to have the Sine wave start with 0 in the middle
            ignition::math::Vector2d initial_pose = this->direction * (- this->amplitude);
            ignition::math::Pose3d initial_pose_set = ignition::math::Pose3d(initial_pose.X(), initial_pose.Y(), 0, 0, 0, 0);
            this->model->SetWorldPose(initial_pose_set);
        }
    }
    else
    {
        if (this->motion_type == 1) // Velocity Sine Wave
        {
            // Apply linear velocity to model
            // Velocity = -aw * sin(wt)

            double desired_vel = ((-this->amplitude * this->frequency_w)) * sin( (this->frequency_w * _info.simTime.Double() ));
            
            // Check to see if user wanted to add in forward velocity
            if (this->forward_direction != 0)
            {
                // Rotate the sine vector to a vector ortho to it (-1.5708 Radians)
                ignition::math::Vector3d fwd_vec =this->matrix_rot * this->direction_3d;
                // Now multiply both of these vectors by their velocities and add them together
                ignition::math::Vector3d sine_vec = this->direction_3d * desired_vel;
                fwd_vec = fwd_vec * this->forward_velocity;
                ignition::math::Vector3d output_vec = sine_vec + fwd_vec;
                this->model->SetLinearVel(output_vec);       

            }
            else
            {
                // Just apply the sine velocity in the desired direction
                this->model->SetLinearVel((this->direction_3d * desired_vel));
            }
        }
        else if (this->motion_type == 2)
        {
            // Read from Profile file and set position
            
            // Setup a counter to iterate throught the array. Update rate will be set by World Update Rate
            if (this->setup_bool == false)
            {
                // Get size of the array
                this->size = *(&Profile + 1) - Profile;
                this->index = 0;
                this->back_bool = false;
                this->current_time = 0.0;
                // Get the start time
                this->start_time = _info.simTime.Double();
                this->setup_bool = true;

            }

            // Get current time
            this->current_time = _info.simTime.Double(); // seconds

            // Update every period (in seconds)
            if (this->current_time - this->start_time >= this->period)
            {
                this->model->SetWorldPose(ignition::math::Pose3d(0, Profile[this->index], 0, 0, 0, 0));
                // Check to see if we are at the end of the profile
                if (this->index == this->size)
                {
                    this->back_bool = true;
                }
                else if( this->index == 0)
                {
                    this->back_bool = false;
                }
                if (this->back_bool)
                {   
                    //gzdbg << "Going Backwards in Array" << std::endl;
                    // Go backwards
                    this->index = this->index - 1;
                }
                else
                {
                    //gzdbg << "Going Forwards in Array" << std::endl;
                    this->index = this->index + 1;
                }

                this->start_time = this->current_time;
                    
            }
        }
        else if (this->motion_type == 3)
        {
            // Read from Profile file and set position
            
            // Setup a counter to iterate throught the array. Update rate will be set by World Update Rate
            if (this->setup_bool == false)
            {
                // Get size of the array
                this->size = *(&Profile + 1) - Profile;
                this->index = 0;
                this->back_bool = false;
                this->setup_bool = true;
            }

            //this->model->SetWorldPose(ignition::math::Pose3d(0, Profile[this->index], 0, 0, 0, 0));
            this->desired_position = Profile[this->index];
            //double desired_position = 4;
            //double desired_position = 10; // meters
            // Calculate force required to move to desired position
            this->vel = this->model->WorldLinearVel();
            this->vely = this->vel.Y();
            this->pos = this->model->WorldPose();
            this->posy = this->pos.Pos().Y();

            this->force = this->feed_forward_gain*desired_position - this->gain_k1*posy - this->gain_k2*vely; 
            //double force = 2250*desired_position - 2250*posy - 699*vely; // poles at -9, -5
            this->link->SetForce(ignition::math::Vector3d(0, force, 0));

            // Check to see if we are at the end of the profile
            if (this->index == this->size)
            {
                this->back_bool = true;
            }
            else if( this->index == 0)
            {
                this->back_bool = false;
            }


            if (this->back_bool)
            {   
                //gzdbg << "Going Backwards in Array" << std::endl;
                // Go backwards
                this->index = this->index - 1;
            }
            else
            {
                //gzdbg << "Going Forwards in Array" << std::endl;
                this->index = this->index + 1;
            }

        }
        else if (this->motion_type == 4)
        {

            // Initialize the timer values
            if (this->setup_bool == false)
            {
                this->current_time = 0.0;
                // Get the start time
                this->start_time = _info.simTime.Double();
                this->hold_control = false;
                this->setup_bool = true;
            }

            // Get current time
            this->current_time = _info.simTime.Double(); // seconds
            
            // Wait for target1_hold amount (in seconds) before moving
            // This will have the rail sim either wait at origin or wait at target1 depending on control logic variables set
            if (this->current_time - this->start_time >= this->target1_hold)
            {
                // if hold_control is true we want to be moving the target to the target 1
                if (this->hold_control == false)
                {
                    // Target1 Position
                    this->model->SetWorldPose(ignition::math::Pose3d(0, this->target1_pos, 0, 0, 0, 0));
                    this->hold_control = true;                   
                }
                else
                {
                    this->model->SetWorldPose(this->origin_pose);
                    this->hold_control = false;
                }
                // Restart the timer
                this->start_time = this->current_time;
            }
            

        }


    }
    
}



