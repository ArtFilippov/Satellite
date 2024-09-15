#include "satellite.hpp"


#include <chrono>
#include <format>
using namespace std::chrono;

constexpr milliseconds TIME_STEP = 5ms; 
constexpr double DT = 5e-3;
constexpr double CHARGING_PER_SECOND = 0.37;
constexpr double DISCHARGE_PER_SECOND = 0.64;

constexpr double ZERO_KELVIN = -273; // градусов Цельсия
constexpr double LOWER_T = 0; // градусов Кельвина
constexpr double UPPER_T = 3e3 - ZERO_KELVIN;
constexpr double CRITICAL_CHARGE_LVL = 20;

constexpr double T_OF_SUN = 2e6;
constexpr double R_OF_SUN = 7e5;
constexpr double DISTANCE_FROM_EARTH_TO_SUN = 150e6;

Satellite::Satellite(ublas::vector<double> r, ublas::vector<double> v, double t, double power) : r_(r), v_(v), t_(t), power_(power)
{
    v_to_orbit();
    simulation = std::thread(&Satellite::run, this);
}

Satellite::~Satellite()
{
    r_ = ublas::vector<double>(3);
    simulation.join();
}

std::string Satellite::get_data()
{
    return std::format("status: {}\ncoordinates: {:.3f} {:.3f} {:.3f}\nspeed: {:.3f} {:.3f} {:.3f}\ntemperature: {:.0f}\nbattery charge: {:.2f}\n", status, r_(0), r_(1), r_(2), v_(0), v_(1), v_(2), t_ + ZERO_KELVIN, power_);
}

double Satellite::get_t()
{
    return t_;
}


void Satellite::v_to_orbit()
{
    double v = ublas::inner_prod(v_, v_);
    v_ -= r_ * ublas::inner_prod(r_, v_) / ublas::inner_prod(r_, r_);
    v_ *= v / ublas::inner_prod(v_, v_);
}

void Satellite::r_step()
{
    double r = std::sqrt(ublas::inner_prod(r_, r_));
    r_ += v_ * DT;
}

void Satellite::t_step()
{
    ublas::vector<double> sun(3);
    sun(0) = DISTANCE_FROM_EARTH_TO_SUN;
    auto r_sun = r_ - sun;
    t_ = T_OF_SUN * R_OF_SUN * R_OF_SUN / ublas::inner_prod(r_sun, r_sun);
}

void Satellite::power_step()
{
    power_ += (CHARGING_PER_SECOND - DISCHARGE_PER_SECOND) * DT;
}

void Satellite::check_status()
{
    status = "";
    if (t_ <= LOWER_T) {
        status += "It's too cold. ";
    } 

    if (t_ >= UPPER_T) {
        status += "It's too hot. ";
    }

    if (power_ < CRITICAL_CHARGE_LVL) {
        status += "Low battery. ";
    }

    if (status.empty()) {
        status = "Everything is normal.";
    }
}


void Satellite::run()
{
    while (ublas::inner_prod(r_, r_) && power_ > 0) {
        std::this_thread::sleep_for(TIME_STEP);

        r_step();
        v_to_orbit();
        t_step();
        power_step();

        check_status();
    }
}
