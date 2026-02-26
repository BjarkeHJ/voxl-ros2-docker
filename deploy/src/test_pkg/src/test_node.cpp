#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class TestNode : public rclcpp::Node {
public:
    TestNode() : Node("test_node") {
        test_pub_ = this->create_publisher<std_msgs::msg::String>("/test_topic", 10);
        test_timer_ = this->create_wall_timer(
            std::chrono::duration<double, std::milli>(1/10),
            std::bind(&TestNode::publish_msg, this)
        );

        RCLCPP_INFO(this->get_logger(), "TestNode Initialized...");
    }

private:
    void publish_msg();
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr test_pub_;
    rclcpp::TimerBase::SharedPtr test_timer_;
};

void TestNode::publish_msg() {
    std_msgs::msg::String msg;
    msg.data = "Hello ARM64!";
    test_pub_->publish(msg);
}

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TestNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

