#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include "6bianxing.h" // Assuming .h contains the necessary declarations

int main(int argc, char** argv) {
    // Default IP address Robotino 4
    std::string ip_addr = "192.168.0.1";

    // Connect
    std::cout << "Connecting...";
    MyCom com;
    com.setAddress(ip_addr.c_str());
    com.connectToServer(true);
    if (!com.isConnected()) {
        std::cout << std::endl << "Could not connect to " << com.address() << std::endl;
        rec::robotino::api2::shutdown();
        exit(1);
    }
    else {
        std::cout << "success" << std::endl;
    }

    // Initialize omni drive and motors
    OmniDrive omniDrive;
    Motor motor[3];
    for (unsigned int i = 0; i < 3; ++i) {
        motor[i].setMotorNumber(i);
    }

    // Constants
    const unsigned int move_duration_ms = 3000; // Move duration in milliseconds
    const unsigned int rotation_duration_ms = 1000; // Rotation duration in milliseconds
    const unsigned int sleep_ms = 100; // Sleep time in milliseconds
    const float speed = 0.1f; // Speed in m/s
    const float rotation_speed = PI / 3 / (rotation_duration_ms / 1000.0f); // Rotation speed in rad/s
    const int num_cycles = 6; // Number of cycles

    // Start time
    unsigned int start_time = com.msecsElapsed();
    unsigned int time = com.msecsElapsed();

    // Loop for each cycle
    for (int cycle = 0; cycle < num_cycles; ++cycle) {
        // Move 10cm forward
        double start_move_time = time;
        while ((time - start_move_time) <= move_duration_ms) {
            double t = (time - start_move_time) / move_duration_ms; // Time ratio for current movement
            double x_speed = speed * cos(0); // Move straight ahead
            double y_speed = speed * sin(0);

            // Set omni drive velocity
            omniDrive.setVelocity(x_speed, y_speed, 0.0f);

            // Print motor positions, velocities, and currents
            std::cout << " | " << std::setw(11) << (time - start_time) << " | ";
            for (size_t i = 0; i < 3; ++i) {
                double position = motor[i].actualPosition();
                std::cout << std::setw(11) << position << " | ";
            }
            std::cout << std::endl;

            // Sleep for a specified duration
            rec::robotino::api2::msleep(sleep_ms);
            time = com.msecsElapsed();
        }

        // Stop
        omniDrive.setVelocity(0.0f, 0.0f, 0.0f);
        rec::robotino::api2::msleep(sleep_ms);

        // Rotate 60 degrees to the left
        double start_rotation_time = time;
        while ((time - start_rotation_time) <= rotation_duration_ms) {
            double t = (time - start_rotation_time) / rotation_duration_ms; // Time ratio for rotation
            double theta = rotation_speed * t; // Angle increment

            // Set omni drive velocity for rotation
            omniDrive.setVelocity(0.0f, 0.0f, rotation_speed);

            // Print motor positions, velocities, and currents
            std::cout << " | " << std::setw(11) << (time - start_time) << " | ";
            for (size_t i = 0; i < 3; ++i) {
                double position = motor[i].actualPosition();
                std::cout << std::setw(11) << position << " | ";
            }
            std::cout << std::endl;

            // Sleep for a specified duration
            rec::robotino::api2::msleep(sleep_ms);
            time = com.msecsElapsed();
        }
    }

    // Disconnect from server and shutdown
    com.disconnectFromServer();
    rec::robotino::api2::shutdown();

    return 0;
}

