#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/sensors/sensors.hh>
#include <ignition/math4/ignition/math/Vector3.hh>
#include <stdio.h>
#include <math.h>

#include "include/TetherPullPlugin.hh"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>
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
        std::bind(&TetherPullPlugin::OnUpdate, this));






}


void TetherPullPlugin::OnUpdate()
{


    // Check setpoint from the socket
    //this->valread = read(this->sock, this->buffer, 4);
    // Buffer is the setpoint, convert string to double
    //std::string s(buffer, sizeof(buffer));
    //this->setpoint = std::stod(s);
    //int temp = this->setpoint;
    this->setpoint = -2;
    //int base = 1;
    //while(temp)
    //{
    //    int last_digit = temp % 10;
    //    temp = temp / 10;
    //    this->setpoint += last_digit * base;
    //    base = base * 2;
    //}
    //gzdbg << "String: " << s << "Setpoint: " << this->setpoint << std::endl;


    //gzdbg << "Setpoint is: " << setpoint << std::endl;

    // Get vehicle horizontal velocity
    ignition::math::Vector3d vel = this->model->RelativeLinearVel();
    
    // Apply ff_gain to the input
    this->output_force = (this->setpoint) * this->ff_gain;
    // Apply state feedback
    this->output_force = this->output_force - (this->state_fdbk_gain * vel.Z());
    gzdbg << "Setpoint: " << this->setpoint << " Actual: " << vel.Z() << " Force Being applied: " << this->output_force << std::endl;



    // Apply zero force to the drone
    gazebo::physics::LinkPtr link = this->model->GetChildLink(this->drone_base_link_name);
    link->AddForce(ignition::math::Vector3d(0,0,this->output_force)); // newtons


    

}