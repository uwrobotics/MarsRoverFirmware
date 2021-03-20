#include "units.h"
#include "Logger.h"
#include <cmath>

// These are various namespaces defined that make units esaier to work with
using namespace units::length;
using namespace units::force;
using namespace units::current;
using namespace units::angular_velocity;
using namespace units::literals;
using namespace units::angle;
using namespace units::time;
using namespace units::current;
using namespace units::impedance;
using namespace units::voltage;
using namespace units::mass;

int main() {

    // Testing angular velocity in degrees per second and angle in degrees

    degrees_per_second_t speed = degree_t(5)/second_t(1.0); // creates an angular velocity type object with velocity of 5 degrees/second

    // Multiplying 5 degrees/sec by 5 seconds yields 25 degrees
    // Not explicitly declaring it as degree_t would assume it as radians, which is the SI unit
    degree_t degreesMoved = speed * second_t(5); 

    // Printing both the angular velocity and the degrees subtended: 
    Utility::logger << speed << std::endl;
    Utility::logger << degreesMoved << std::endl;

    radian_t angleInRad = radian_t(2*M_PI); // Create angle object in radians
    degree_t angleInDeg = degree_t(angleInRad); // Can be cast to degrees

    Utility::logger << angleInRad << std::endl; // prints 6.28319 rad
    Utility::logger << angleInDeg << std::endl; // prints 360 degrees

    // Testing current in Amperes

    ampere_t currentIn = ampere_t(3); // Creating 3A current
    ohm_t resistance = ohm_t(4); // resistance through the load
    auto voltageThrough = currentIn * resistance; // automatically casted to voltage since V = IR

    Utility::logger << voltageThrough << std::endl; // prints a voltage object

    // Testing force in Newtons

    newton_t forceIn = newton_t(5); // Create a 5 newton force
    kilogram_t massOfObject = kilogram_t(10); // applied on a 10 kg object
    auto accelerationIn = forceIn/massOfObject; // automatically casted to m/s^2 (acceleration)
    Utility::logger << accelerationIn << std::endl; 
} 