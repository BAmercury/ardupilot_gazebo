
#include "include/AnglePlugin.hh"


using namespace gazebo;

GZ_REGISTER_WORLD_PLUGIN(AnglePlugin)

void AnglePlugin::Load(physics::WorldPtr _world, sdf::ElementPtr _sdf)
{

    if (_sdf->HasElement("target"))
    {
        this->model_target_name = _sdf->GetElement("target")->Get<std::string>();
    }
    if (_sdf->HasElement("update_rate"))
    {
        this->update_rate = _sdf->GetElement("update_rate")->Get<int>();
        //gzdbg << update_rate << std::endl;
        this->period = 1.0/this->update_rate;
    }
    // Log data to CSV file if user wants too
    if (_sdf->HasElement("debug_log"))
    {
       if (_sdf->GetElement("debug_log")->Get<int>())
       {
           // Enable data logging, start new CSV file
           myFile.open("data.csv");
           this->enable_log = true;
       } 
       
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
        this->drone_pose = this->model_drone->WorldPose();
        this->target_pose = this->model_target->WorldPose();

        // Find relative distance (In gazebo world frame) in meters
        this->rel_pos.X(this->target_pose.Pos().X() - this->drone_pose.Pos().X());
        this->rel_pos.Y(this->target_pose.Pos().Y() - this->drone_pose.Pos().Y());
        this->rel_pos.Z(this->drone_pose.Pos().Z());

        // Convert from Gazebo NWU to Ardupilot NED, flip 180 degrees along the roll axis
        this->rel_pos_NED = this->gazeboXYZTONED.Rot().RotateVectorReverse(rel_pos);
        // Construct rotation matrix (From Gazebo World frame to Vehicle body frame)
        this->drone_angle = this->drone_pose.Rot().Euler(); // Roll, Pitch, yaw
        double sin_roll_align = sin(this->drone_angle.X());
        double cos_roll_align = cos(this->drone_angle.X());
        double sin_pitch_align = sin(this->drone_angle.Y());
        double cos_pitch_align = cos(this->drone_angle.Y());
        double sin_yaw_align = sin(this->drone_angle.Z());
        double cos_yaw_align = cos(this->drone_angle.Z());

        this->Rx = ignition::math::Matrix3d(
            1, 0, 0,
            0, cos_roll_align, -sin_roll_align,
            0, sin_roll_align, cos_roll_align
        );
        this->Ry = ignition::math::Matrix3d(
            cos_pitch_align, 0, sin_pitch_align,
            0, 1, 0,
            -sin_pitch_align, 0, cos_pitch_align
        );
        this->Rz = ignition::math::Matrix3d(
            cos_yaw_align, -sin_yaw_align, 0,
            sin_yaw_align, cos_yaw_align, 0,
            0, 0, 1
        );
        // Rotation from gazebo world frame to gazebo body frame
        this->rot = this->Rx * this->Ry * this->Rz;
        ignition::math::Vector3d corrected_pos = this->rot * this->rel_pos_NED;

        corrected_pos = corrected_pos / corrected_pos.Z();
        // Get angles
        //float angle_x = atan2(corrected_pos.X(),  corrected_pos.Z()); // In Radians
        //float angle_y = atan2(corrected_pos.Y(), corrected_pos.Z());

        // Publish Data
        DataPacket packet;
       
        packet.timestamp = static_cast<uint64_t>
            (1.0e3 * this->current_time);
        packet.num_targets = static_cast<uint16_t>(1);
        packet.pos_x = corrected_pos.Y();
        packet.pos_y = -corrected_pos.X();
        packet.size_x = static_cast<float>(1);
        packet.size_y = static_cast<float>(1);
        // Send UDP packet to server
        sendto(this->sock_fd, 
        reinterpret_cast<raw_type *>(&packet),
        sizeof(packet), 0,
        (struct sockaddr *)&this->servaddr, sizeof(this->servaddr) 
        );


        // Log data to CSV file if enabled
        // Going to log Gazebo ground truth, Rel Pos Gazebo and NED, Drone angles, and final data packet
        if (this->enable_log)
        {
            myFile << this->drone_pose.Pos().X() << "," << this->drone_pose.Pos().Y() 
            << "," << this->drone_pose.Pos().Z() << "," << this->rel_pos.X() << ","
            << this->rel_pos.Y() << "," << this->rel_pos.Z() << "," << this->rel_pos_NED.X()
            << "," << this->rel_pos_NED.Y() << "," << this->rel_pos_NED.Z() << "," 
            << this->drone_angle.X() << "," << this->drone_angle.Y() << "," << this->drone_angle.Z()
            << "," << packet.pos_x << "," << packet.pos_y << "," <<packet.timestamp << std::endl;
        }

        // Save start time
        this->start_time = this->current_time;
    }

}
