
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
            gzdbg << "Motion Type: " << motion_type << std::endl;
            if (motion_type == 1) // Velocity Sine Wave
            {
                // Get the rest of the parameters
                this->amplitude = motion_sdf->Get<double>("amplitude");
                this->max_velocity = motion_sdf->Get<double>("max_velocity");
                this->frequency_w = max_velocity / amplitude;
                this->direction = motion_sdf->Get<int>("direction");
            }
        }
        //gzdbg << "Amplitude: " << amplitude << std::endl;
        //gzdbg << "Max Velocity: " << max_velocity << std::endl;
       
       


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
    if (_info.simTime.Double() < 50)
    {
        // Apply no Velocity
        this->model->SetLinearVel(ignition::math::Vector3d(0,0,0));
    }
    else
    {
        if (motion_type == 1) // Velocity Sine Wave
        {
            // Apply linear velocity to model
            // Velocity = -aw * sin(wt)
            double desired_vel = (-amplitude * frequency_w) * sin( (frequency_w) * _info.simTime.Double());
            
            // Apply in the desired direction
            if (this->direction == 1)
            {
                this->model->SetLinearVel(ignition::math::Vector3d(desired_vel, 0, 0));
            }
            else if(this->direction == 2)
            {
                this->model->SetLinearVel(ignition::math::Vector3d(0, desired_vel, 0));
            }
            else if (this->direction == 3)
            {
                this->model->SetLinearVel(ignition::math::Vector3d(0, 0, desired_vel));
            }
            else // Default is in Y Direction
            {
                this->model->SetLinearVel(ignition::math::Vector3d(0, desired_vel, 0));
            }
            
            
            
            //gzdbg << "Cart Desired Speed: " << desired_vel << std::endl;
            //gzdbg << "Cart Relative Speed: " << this->model->RelativeLinearVel() << std::endl;
            //gzdbg << "Cart World Speed: " << this->model->WorldLinearVel() << std::endl;

        }
        else if (motion_type == 2)
        {
            // Read from Profile file and set position
            
            // Setup a counter to iterate throught the array. Update rate will be set by World Update Rate
            if (this->setup_bool == false)
            {
                // Get size of the array
                this->size = *(&Profile + 1) - Profile;
                this->index = 0;
                setup_bool = true;
            }

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
                gzdbg << "Going Backwards in Array" << std::endl;
                // Go backwards
                this->index = this->index - 1;
            }
            else
            {
                gzdbg << "Going Forwards in Array" << std::endl;
                this->index = this->index + 1;
            }


        }


    }
    
}



