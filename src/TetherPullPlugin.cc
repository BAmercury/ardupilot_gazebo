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


GZ_REGISTER_MODEL_PLUGIN(TetherPullPlugin)

void TetherPullPlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
{
    // Store pointers to model and sdf
    this->model = _model;

    // Get the world pointer
    this->world_ptr = this->model->GetWorld();


    // // Setup socket TCP client connection
    // if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    // {
    //     gzerr << "Tether Pull Client socket failed to create" << std::endl;

    // }

    // server_addr.sin_family = AF_INET;
    // server_addr.sin_port = htons(this->server_port);

    // // Convert the IPv4 address from text to binary
    // if (inet_pton(AF_INET, this->server_ip, &server_addr.sin_addr) <= 0)
    // {
    //     gzerr << "Tether Pull Client Socket: Invalid Address Given" << std::endl;

    // }

    // // Connect the client to the socket now
    // if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
    // {
    //     gzerr << "Tether Pull Client Socket: Connection Failed" << std::endl;

    // }
    // Callback for world update events
    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
        std::bind(&TetherPullPlugin::OnUpdate, this));






}


void TetherPullPlugin::OnUpdate()
{


    // Check setpoint from the socket
    //this->valread = read(this->sock, this->buffer, 1024);
    // Buffer is the setpoint, convert string to double
    //this->setpoint = atof(buffer);
    this->setpoint = 10; // meters per second

    //gzdbg << "Setpoint is: " << setpoint << std::endl;

    // Get vehicle horizontal velocity
    ignition::math::Vector3d vel = this->model->WorldLinearVel();
    
    // Apply ff_gain to the input
    double output = this->setpoint * this->ff_gain;
    // Apply state feedback
    output = output - (this->state_fdbk_gain * vel.Z());
    gzdbg << "Force being applied: " << output << "Vel: " << vel.Z() << std::endl;



    // Apply zero force to the drone
    gazebo::physics::LinkPtr link = this->model->GetChildLink(this->drone_base_link_name);
    link->AddForce(ignition::math::Vector3d(0,0,output)); // newtons


    

}