#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <string>
#include <thread>

namespace ublas = boost::numeric::ublas;

class Satellite
{
    
    ublas::vector<double> r_;
    ublas::vector<double> v_;
    double t_;
    double power_;
    std::string status;
    std::thread simulation;
public:
    Satellite(ublas::vector<double> r, ublas::vector<double> v, double t = 0, double power = 100.0);
    ~Satellite();

    std::string get_data();
    double get_t();
private:
    void v_to_orbit();
    void r_step();
    void t_step();
    void power_step();
    void check_status();

    void run();
};


