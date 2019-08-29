
#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/sensors/sensors.hh>
#include <ignition/math4/ignition/math/Vector3.hh>
#include <stdio.h>
#include <math.h>
#include "include/RailSimPlugin.hh"

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
            }
        }
        gzdbg << "Amplitude: " << amplitude << std::endl;
        gzdbg << "Max Velocity: " << max_velocity << std::endl;
       
       


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
            this->model->SetLinearVel(ignition::math::Vector3d(0, desired_vel, 0));
            gzdbg << "Cart Desired Speed: " << desired_vel << std::endl;
            gzdbg << "Cart Relative Speed: " << this->model->RelativeLinearVel() << std::endl;
            gzdbg << "Cart World Speed: " << this->model->WorldLinearVel() << std::endl;

        }


    }
    
}



