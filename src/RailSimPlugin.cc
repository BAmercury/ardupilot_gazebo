
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

    // Create a sensor
    sensors::SensorManager *manager = sensors::SensorManager::Instance();
    this->sensor =  std::dynamic_pointer_cast<sensors::ContactSensor>(manager->GetSensor(this->sensor_name));
    this->sensor->SetActive(true);

    // Get World Pointer
    this->world_ptr = this->model->GetWorld();

    // Go through SDF parameters to setup motion profile
    if (_sdf->HasElement("motion_type"))
    {
        sdf::ElementPtr motion_type = _sdf->GetElement("motion_type");
        
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

    // Check if drone has landed on the platform
    msgs::Contacts contacts;
    contacts = this->sensor->Contacts();
    for (unsigned int i = 0; i < contacts.contact_size(); ++i)
    {
        // If contact is made between platform and drone, pause the simulation
        if (contacts.contact(i).collision1() == this->drone_coll_name)
        {
            gzdbg << "Collision has been detected"<< std::endl;
            this->model->SetLinearVel(ignition::math::Vector3d(0,0,0));
            this->world_ptr->SetPaused(true);
            contacted = true;
            break;

        }
        else if (contacted == false)
        {
            // Apply Linear Velocity to the model
            // Velocity = -aw * sin(wt)physics::WorldPtr _parent, sd
            double desired_vel = (-amplitude * frequency_w) * sin( (frequency_w) * _info.simTime.Double());

            // Should oscillate between 0.6096:-0.6096 m/s ? 
            desired_vel = desired_vel * 0.2;
            //gzdbg << "Cart Scaled Speed: " << desired_vel << std::endl;
            this->model->SetLinearVel(ignition::math::Vector3d(0, desired_vel, 0));

        }


    }
}




