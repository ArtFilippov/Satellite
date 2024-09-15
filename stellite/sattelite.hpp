#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <string>
#include <thread>
#include <chrono>

namespace ublas = boost::numeric::ublas;

class Sattelite
{
    
    ublas::vector<double> r_;
    ublas::vector<double> v_;
    int t_;
    double power_;
    std::string status;
    std::chrono::milliseconds time_step; 
    std::thread t;
public:
    Sattelite(ublas::vector<double> r, ublas::vector<double> v, int t, double power = 100.0);
    ~Sattelite();

    std::string get_data();
    int get_t();
private:
    void v_to_orbit();
    void r_step();
    void t_step();
    void power_step();
    void check_status();
};


