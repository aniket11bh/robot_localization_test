#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>
#include <robot_localization/navsat_conversions.hpp>
#include <robot_localization/navsat_transform.hpp>

class LatLongPublisherNode : public rclcpp::Node
{
public:
    // ----------------------------------------------------------------------------
    LatLongPublisherNode() : Node("lat_long_publisher")
    {
        init();
    }

    // ----------------------------------------------------------------------------
    void init()
    {
        to_ll_client_ = this->create_client<robot_localization::srv::ToLL>("toLL");
        from_ll_client_ = this->create_client<robot_localization::srv::FromLL>("fromLL");

        while (!to_ll_client_->wait_for_service(std::chrono::seconds(1)))
        {
            RCLCPP_INFO_STREAM(get_logger(), "Waiting for toLL service");
        }

        while (!from_ll_client_->wait_for_service(std::chrono::seconds(1)))
        {
            RCLCPP_INFO_STREAM(get_logger(), "Waiting for fromLL service");
        }
    }

    void fromLLCallback(rclcpp::Client<robot_localization::srv::FromLL>::SharedFuture future)
    {
        auto result = future.get();
        auto point = result->map_point;

        float gps_odom_x = point.x;
        float gps_odom_y = point.y;
        float gps_odom_z = point.z;
    }

    void toLLCallback(rclcpp::Client<robot_localization::srv::ToLL>::SharedFuture future)
    {
        auto result = future.get();
        auto point = result->ll_point;

        sensor_msgs::msg::NavSatFix msg;
        msg.latitude = point.latitude;
        msg.longitude = point.longitude;
        msg.altitude = point.altitude;
    }

private:
    rclcpp::Client<robot_localization::srv::ToLL>::SharedPtr to_ll_client_;
    rclcpp::Client<robot_localization::srv::FromLL>::SharedPtr from_ll_client_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<LatLongPublisherNode>();

    try
    {
        rclcpp::spin(node);
    }
    catch (const std::exception &e)
    {
        RCLCPP_ERROR(node->get_logger(), "Unhandled exception: %s", e.what());
        return -1;
    }

    rclcpp::shutdown();
    return 0;
}