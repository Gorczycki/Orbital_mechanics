#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;

vector<double> unit_vector(vector<double> pointing_vector_m);
double unit_magnitude(vector<double> pointing_vector_m);
vector<double> initial_acceleration(long double earth_mass_kg, long double gravit_constant, double moon_radius_distance_m, vector<double> coordinates_m);
vector<double> verlet_position(vector<double> coordinates_m, vector<double> velocity_vector_mS, vector<double> accel, double time_delta);
vector<double> acceleration(long double earth_mass_kg, long double gravit_constant, double moon_radius_distance_m, vector<double> coordinates_m);
vector<double> verlet_velocity(vector<double> velocity_vector_mS, vector<double> accel, vector<double> accelerations, double time_delta);

int main()
{
    long double earth_mass_kg = 5.9722E24L;
    //cout.precision(25);
    long double moon_mass_kg = 7.348E22L;
    long double gravit_constant = 6.67430E-11L;
    double earth_radius_m = 6378137;
    double moon_radius_m = 1737400;
    double time_frame = 1000000;
    double time_delta = 10;
    vector<vector<double>> velocities;
    vector<vector<double>> radii;
    vector<double> magn_dist_vec;
    vector<vector<double>> accelerations;

    //initial direction, coordinates, hypotenuse(magnitude of distance)
    vector<double> initial_direction = {0,-1};
    vector<double> direction_current = initial_direction;
    vector<double> unit_direction = unit_vector(direction_current); 
    vector<double> initial_coordinates_m = {384400000, 0};
    vector<double> coordinates_m = initial_coordinates_m;
    //double moon_radius_distance_m = unit_magnitude(coordinates_m);
    double moon_radius_distance_m = sqrt(pow(coordinates_m[0], 2) + pow(coordinates_m[1], 2));

    //inital velocity, initial velocity with direction as scalar
    double initial_velocity_mS = 1023.056;
    //double initial_velocity_mS = 2000;
    vector<double> initial_velocity_vector_mS = {unit_direction[0] * initial_velocity_mS, unit_direction[1] * initial_velocity_mS};
    vector<double> velocity_vector_mS = initial_velocity_vector_mS;

    //earth's gravitational acceleration on the moon, dependent on r and v
    vector<double> initial_accel = initial_acceleration(earth_mass_kg, gravit_constant, moon_radius_distance_m, coordinates_m);
    vector<double> accel = initial_accel;

    //store initial values:
    velocities.push_back(velocity_vector_mS);
    radii.push_back(coordinates_m);
    double magn_dist = moon_radius_distance_m;
    magn_dist_vec.push_back(magn_dist);
    //magn_dist = 0;
    accelerations.push_back(accel);


    for(int i = 1; i<time_frame; i++)
    {
        vector<double> coordinates_m_1 = verlet_position(radii[i-1], velocities[i-1], accelerations[i-1], time_delta);
        //distance magnitude:
        magn_dist = unit_magnitude(coordinates_m_1);
        if(magn_dist <= (earth_radius_m + moon_radius_m))
        {
            //cout<<"Earth and moon collide at moon center coordinates!"<<endl;
            cout<<"Earth and moon collide at moon center coordinates:"<<coordinates_m_1[0]<<", "<<coordinates_m_1[1]<<endl;
            break;
        }
        vector<double> accel_1 = acceleration(earth_mass_kg, gravit_constant, magn_dist, coordinates_m_1);
        vector<double> veloc_1 = verlet_velocity(velocities[i-1], accel_1, accelerations[i-1], time_delta);

        radii.push_back(coordinates_m_1);
        accelerations.push_back(accel_1);
        velocities.push_back(veloc_1);
    }

    //converting to km from m:
    for(int i = 0; i<radii.size(); i++)
        for(int j = 0; j<radii[i].size(); j++)
            {
                radii[i][j] = radii[i][j] / 1000;
            }


    std::ofstream output_file("output.csv");
    if (!output_file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return 1;
    }

    //int n = 0;
    for (int i = 0; i < radii.size(); i++) {
        for (int j = 0; j < radii[i].size(); j++) 
        {
            //n++;
            output_file << std::fixed << std::setprecision(2) << radii[i][j];
            if (j == radii[i].size() - 1)
                output_file << "\n";
            else 
                output_file << ", ";
        }
    }

    output_file.close();
    std::cout << "Data saved to output.csv successfully." << std::endl;

    return 0;
}


vector<double> unit_vector(vector<double> pointing_vector_m)
{
    double x_direc = pointing_vector_m[0];
    double y_direc = pointing_vector_m[1];
    double denominator = sqrt(pow(x_direc,2) + pow(y_direc,2));

    double x_direc_unit = x_direc / denominator;
    double y_direc_unit = y_direc / denominator;

    vector<double> radius_unit_m;
    radius_unit_m.push_back(x_direc_unit);
    radius_unit_m.push_back(y_direc_unit);

    return radius_unit_m;
}

double unit_magnitude(vector<double> pointing_vector_m)
{
    double x_direc = pointing_vector_m[0];
    double y_direc = pointing_vector_m[1];
    double denominator = sqrt(pow(x_direc,2) + pow(y_direc,2));
    //double unit_magn = denominator;

    return denominator;
}

vector<double> initial_acceleration(long double earth_mass_kg, long double gravit_constant, double moon_radius_distance_m, vector<double> coordinates_m)
{
    vector<double> accel = {};
    accel.push_back(0);
    accel.push_back(0);
    accel[0] = ((-(gravit_constant * earth_mass_kg * coordinates_m[0]))/(pow(moon_radius_distance_m,3)));
    accel[1] = ((-(gravit_constant * earth_mass_kg * coordinates_m[1]))/(pow(moon_radius_distance_m,3)));

    return accel;
}

vector<double> acceleration(long double earth_mass_kg, long double gravit_constant, double moon_radius_distance_m, vector<double> coordinates_m)
{
    vector<double> accel = {};
    accel.push_back(0);
    accel.push_back(0);
    accel[0] = ((-(gravit_constant * earth_mass_kg * coordinates_m[0]))/(pow(moon_radius_distance_m,3)));
    accel[1] = ((-(gravit_constant * earth_mass_kg * coordinates_m[1]))/(pow(moon_radius_distance_m,3)));

    return accel;
}

vector<double> verlet_position(vector<double> coordinates_m, vector<double> velocity_vector_mS, vector<double> accel, double time_delta)
{
    vector<double> pos_vec = {};
    pos_vec.push_back(0);
    pos_vec.push_back(0);
    pos_vec[0] = (coordinates_m[0] + (velocity_vector_mS[0]*time_delta) + 0.5*accel[0]*(pow(time_delta,2)));
    pos_vec[1] = (coordinates_m[1] + (velocity_vector_mS[1]*time_delta) + 0.5*accel[1]*(pow(time_delta,2)));

    return pos_vec;
}


vector<double> verlet_velocity(vector<double> velocity_vector_mS, vector<double> accel, vector<double> accelerations, double time_delta)
{
    vector<double> veloc_vec = {};
    veloc_vec.push_back(0);
    veloc_vec.push_back(0);
    veloc_vec[0] = velocity_vector_mS[0] + 0.5*(accel[0] + accelerations[0])*time_delta;
    veloc_vec[1] = velocity_vector_mS[1] + 0.5*(accel[1] + accelerations[1])*time_delta;

    return veloc_vec;
}


//vector<double> accel = initial_accel;
//vector<double> position = verlet_position(coordinates_m, velocity_vector_mS, accel, time_delta);