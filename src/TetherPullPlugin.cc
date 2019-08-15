#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/sensors/sensors.hh>
#include <ignition/math4/ignition/math/Vector3.hh>
#include <stdio.h>
#include <math.h>

#include "include/TetherPullPlugin.hh"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using raw_type = void;


using namespace gazebo;


void TetherPullPlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
{
    // Store pointers to model and sdf
    this->model = _model;

    // Get the world pointer
    this->world_ptr = this->model->GetWorld();

    // Get the contact sensor
    sensors::SensorManager *manager = sensors::SensorManager::Instance();
    this->sensor = std::dynamic_pointer_cast<sensors::ContactSensor>(manager->GetSensor(this->sensor_name));

    // Setup socket TCP client connection
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        gzerr << "Tether Pull Client socket failed to create" << std::endl;

    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->server_port);

    // Convert the IPv4 address from text to binary
    if (inet_pton(AF_INET, this->server_ip, &server_addr.sin_addr) <= 0)
    {
        gzerr << "Tether Pull Client Socket: Invalid Address Given" << std::endl;

    }

    // Connect the client to the socket now
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
    {
        gzerr << "Tether Pull Client Socket: Connection Failed" << std::endl;

    }
    // Callback for world update events
    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
        std::bind(&TetherPullPlugin::OnUpdate, this, std::placeholders::_1));



}


void TetherPullPlugin::OnUpdate(const common::UpdateInfo &_info)
{

    // Check to see if flag as been triggered and that the drone has not made contact with the platform yet
    // If flag is triggered, update the control system output
    // otherwise set the force output to 0

    // Check if contact between drone and railsim has occured
    msgs::Contacts contacts;
    contacts = this->sensor->Contacts();

    for (unsigned int i = 0; i < contacts.contact_size(); ++i)
    {
        if (contacts.contact(i).collision1() == this->drone_coll_name)
        {
            this->contacted = true;
            break;
        }
        else
        {
            this->contacted = false;
        }
        
    }

    // Check socket for incoming data
    valread = read(sock, buffer, 1024);
    std::string message = buffer;
    gzdbg << "Tether System Recieved Message: " << message << std::endl;

    if (buffer == "flag")
    {
        enable_control = true;
    }


    // Force Pull Output
    if (this->enable_control == true && this->contacted == false)
    {

    }

}