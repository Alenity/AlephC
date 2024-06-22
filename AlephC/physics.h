#ifndef PHYSICS_H
#define PHYSICS_H


struct Mass
{
    double value = 0;
    const char* symbol = "m";
    const char* unit = "kg";
};

struct PD
{
    double value = 0;
    const char* symbol = "V";
    const char* unit = "V";
};

struct Current
{
    double value = 0;
    const char* symbol = "I";
    const char* unit = "A";
};

struct Charge
{
    double value = 0;
    const char* symbol = "Q";
    const char* unit = "C";
};

struct Resistivity
{
    double value = 0;
    const char* symbol = "ρ";
    const char* unit = "Ωm";
};

struct Energy
{
    double value = 0;
    const char* symbol = "E";
    const char* unit = "J";
};

struct Resistance
{
    double value = 0;
    const char* symbol = "R";
    const char* unit = "Ω";
};

struct CSA
{
    double value = 0;
    const char* symbol = "A";
    const char* unit = "m^2";
};

struct Capacitance
{
    double value = 0;
    const char* symbol = "C";
    const char* unit = "F";
};

struct Length
{
    double value = 0;
    const char* symbol = "l";
    const char* unit = "m";
};

struct Power
{
    double value = 0;
    const char* symbol = "P";
    const char* unit = "W";
};

struct ComponentProps
{
    Current current;
    PD potential_difference;
    Charge net_charge;
    Resistivity resistivity;
    Energy energy;
    Resistance resistance;
    CSA cross_sectional_area;
    Capacitance capacitance;
    Length length;
    Power power;
};


class PhysicSys
{
public:
    PhysicSys();

    void whatToCalc(ComponentProps properties)
    {
        
    }
private:
    double lightspeed = 3.0e8;
    double elementary_charge = 1.60e-19;
    double electron_rest_mass = 9.11e-31;
    double proton_rest_mass = 1.673e-27;
    double neutron_rest_mass = 1.675e-27;
    double alpha_rest_mass = 6.646e-27;
    double epsilon_naught = 8.854187818e-12;
    double pi = 3.1415926535;
    double planck_constant = 6.62607015e-34;
};

#endif