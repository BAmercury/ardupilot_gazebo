
#include "include/AnglePlugin.hh"


using namespace gazebo;

GZ_REGISTER_WORLD_PLUGIN(AnglePlugin)

void AnglePlugin::Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
{

    if (_sdf->HasElement("target"))
    {
        sdf::ElementPtr elem = _sdf->GetElement("target");
        this->model_target_name = elem->Get<std::string>();
    }

    // Setup UDP Client

    // Create socket file descriptor
    if ((this->sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        gzerr << "Socket Creation Failed" << std::endl;

    }

    this->servaddr.sin_family = AF_INET;
    this->servaddr.sin_port = htons(this->server_port);
    this->servaddr.sin_addr.s_addr = INADDR_ANY; // Connect to this host itself

    // Get Gazebo World Pointer
    this->world = _world;
    // Get Model Pointers for the target and drone
    this->model_drone = this->world->ModelByName("iris");
    this->model_target = this->world->ModelByName(this->model_target_name);








    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
        std::bind(&AnglePlugin::OnUpdate, this, std::placeholders::_1));
}

void AnglePlugin::OnUpdate(const common::UpdateInfo &_info)
{
    // Declare timer variables
    if (this->setup_bool == false)
    {
        this->current_time = 0.0;
        this->start_time = _info.simTime.Double(); // In seconds
        this->setup_bool = true;
    }

    // Get current time
    this->current_time = _info.simTime.Double();

    // Update every period
    if (this->current_time - this->start_time >= this->period)
    {
        // Get position vectors of both objects
        ignition::math::Pose3d drone_pose = this->model_drone->WorldPose();
        ignition::math::Pose3d target_pose = this->model_target->WorldPose();

        // Find relative distance
        float x_rel = target_pose.Pos().X() - drone_pose.Pos().X();
        float y_rel = target_pose.Pos().Y() - drone_pose.Pos().Y();
        // Get Height
        float height = drone_pose.Pos().Z(); 
        //int height = 1;

        // Get angles
        float angle_x = atan2(x_rel, height); // In Radians
        float angle_y = atan2(y_rel, height);

        // Publish Data
        DataPacket packet;
        packet.num_targets = static_cast<uint16_t>(1);
        packet.timestamp = static_cast<uint64_t>(this->current_time * 1.0e3);
        packet.pos_x = -(angle_y);
        packet.pos_y = -(angle_x);
        packet.size_x = static_cast<float>(1);
        packet.size_y = static_cast<float>(1);

        sendto(this->sock_fd, 
        reinterpret_cast<raw_type *>(&packet),
        sizeof(packet), 0,
        (struct sockaddr *)&this->servaddr, sizeof(this->servaddr) 
        );
        gzdbg << "Angle X: " << angle_x << std::endl;
        gzdbg << "Angle Y: " << angle_y << std::endl;


        // Save start time
        this->start_time = this->current_time;
    }

}
