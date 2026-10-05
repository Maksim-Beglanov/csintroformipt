#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

double omega;
double f(double x) {
        return -omega * omega * x;
}

using State = std::vector<double>;

State euler(State y, double dt) {
        State res = {y[0] + dt, y[1], y[2]};
        res[1] = y[1] + y[2] * dt;
        res[2] = y[2] + f(y[1]) * dt;

        return res;
}

State heun(State y, double dt) {
        State _y = euler(y, dt);
        
        State res = {y[0] + dt, y[1], y[2]};
        res[1] = y[1] + (y[2] + _y[2]) * dt/2;
        res[2] = y[2] + (f(y[1]) + f(_y[1])) * dt/2;

        return res;
}

State runge_kutta(State y, double dt) {
        State k1 = {y[2], f(y[1])};
        State k2 = {y[2] + k1[1] * dt/2, f(y[1] + k1[0] * dt/2)};
        State k3 = {y[2] + k2[1] * dt/2, f(y[1] + k2[0] * dt/2)};
        State k4 = {y[2] + k3[1] * dt, f(y[1] + k3[0] * dt)};

        State res = {y[0] + dt, y[1], y[2]};
        res[1] += (k1[0] + 2 * k2[0] + 2 * k3[0] + k4[0]) * dt/6;
        res[2] += (k1[1] + 2 * k2[1] + 2 * k3[1] + k4[1]) * dt/6;

        return res;
}

int main(int argc, char *argv[]) {
        if (argc != 2) {
                std::cout << "Using ./prog settings.json" << std::endl;
                return 1;
        }
        std::ifstream settings(argv[1]);
        json data = json::parse(settings);
        settings.close();

        auto output_name = data.at("output").get<std::string>();
        auto method_name = data.at("method").get<std::string>();
        auto bound = data.at("bound").get<double>();
        auto count = data.at("count").get<double>();

        std::function<State(State y, double dt)> method;
        if (method_name == "euler")
                method = euler;
        else if (method_name == "heun")
                method = heun;
        else if (method_name == "runge-kutta")
                method = runge_kutta;
        else {
                std::cout << "Unkown method: " << method_name <<
                        std::endl;
                return 2;
        }
        
        omega = data.at("omega").get<double>();
        double x0 = data.at("x0").get<double>();
        double v0 = data.at("v0").get<double>();

        std::ofstream output(output_name);

        State y = {0.0, x0, v0};

        double dt = bound/count;
        for (int i = 0; i < count; i++) {
                output << y[0] << ' ' << y[1] << ' ' << y[2] << '\n';
                y = method(y, dt);
        }

        output.close();

        return 0;
}
