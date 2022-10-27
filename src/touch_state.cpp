#include "touch_state.hpp"


using std::cout;


int main(int argc, char *argv[]){
    //Init
    // DeviceSpace* Omni = new DeviceSpace;
    
    HHD touch_device = hdInitDevice("Default Device");//hdGetCurrentDevice();

    hdEnable(HD_FORCE_OUTPUT);
    HDErrorInfo  error;
    error = hdGetError();
    cout << hdGetErrorString(error.errorCode) << "  Error  "<< std::endl;
    /*******************************************************************************************/
    //ROS init
    ros::init(argc, argv, "touch_device");
    ros::NodeHandle n;

    // Publishers
    ros::Publisher pose_pub = n.advertise<geometry_msgs::Pose>("/touch/pose", 10);
    ros::Publisher joint_pub = n.advertise<geometry_msgs::Vector3>("/touch/joint_states", 10);
    ros::Publisher force_pub = n.advertise<geometry_msgs::Wrench>("/touch/force", 10);

    // Subscribers
    Listener force_sub;
    ros::Subscriber fsub = n.subscribe("touch/force_cmd", 10, &Listener::forceCallback, &force_sub);

    ros::Rate loop_rate(200);
    
    /*******************************************************************************************/
    // Message init
    touch_msgs::TouchState ts;
    geometry_msgs::Pose p;
    geometry_msgs::Wrench w;
    geometry_msgs::Vector3 joint_states;
    
    hdBeginFrame(touch_device);


    static HDfloat max_force;
    hdGetFloatv(HD_NOMINAL_MAX_FORCE, &max_force);
 
    //Initialize variables to hold state data
    HDfloat position[3];
    hdGetFloatv(HD_CURRENT_POSITION, position);

    HDfloat velocity[3];
    hdGetFloatv(HD_CURRENT_VELOCITY, velocity);

    // HDfloat e_tmat[16];
    HDfloat e_angles[3]; // end effector gimbal angles
    hdGetFloatv(HD_CURRENT_GIMBAL_ANGLES, e_angles);

    HDfloat joint_angles[3];
    hdGetFloatv(HD_CURRENT_JOINT_ANGLES, joint_angles);

    HDfloat force[3];
    hdGetFloatv(HD_CURRENT_FORCE, force);
    HDfloat force_cmd[3];

    hdEndFrame(touch_device);
    error = hdGetError();
    cout << hdGetErrorString(error.errorCode) << "  Error  "<< std::endl;
    /*******************************************************************************************/
    // Main loop
    
    while (ros::ok()){
        // Pose publisher
        hdBeginFrame(touch_device);

        hdGetFloatv(HD_CURRENT_POSITION, position);
        hdGetFloatv(HD_CURRENT_GIMBAL_ANGLES, e_angles);

        p.position.x = position[0];
        p.position.y = position[1];
        p.position.z = position[2];

        p.orientation.x = e_angles[0];
        p.orientation.y = e_angles[1];
        p.orientation.z = e_angles[2];

        pose_pub.publish(p);

        // Joint publisher
        hdGetFloatv(HD_CURRENT_JOINT_ANGLES, joint_angles);
        joint_states.x = joint_angles[0];
        joint_states.y = joint_angles[1];
        joint_states.z = joint_angles[2];

        joint_pub.publish(joint_states);

        // Force publisher
        hdGetFloatv(HD_CURRENT_FORCE, force);
        w.force.x = force[0];
        w.force.y = force[1];
        w.force.z = force[2];

        force_pub.publish(w);
        
        // Send desired forces to Touch
        force_sub.getForce(force_cmd);
        
        hdSetFloatv(HD_CURRENT_FORCE, force_cmd);
        hdEndFrame(touch_device);

        ros::spinOnce();
        loop_rate.sleep();
    }

    hdDisable(HD_FORCE_OUTPUT);
    hdDisableDevice(touch_device);

    return 0;
}