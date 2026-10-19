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

// Schemes return delta of current state
State euler(const State& y, const double& dt) {
        return {y[1] * dt,
                f(y[0]) * dt};
}

State heun(const State& y, const double& dt) {
        State d = euler(y, dt);
        
        return {(2 * y[1] + d[1]) * dt/2,
                (f(y[0]) + f(y[0] + d[0])) * dt/2};
}

State runge_kutta(const State& y, const double& dt) {
        State k1 = {y[1], f(y[0])};
        State k2 = {y[1] + k1[1] * dt/2, f(y[0] + k1[0] * dt/2)};
        State k3 = {y[1] + k2[1] * dt/2, f(y[0] + k2[0] * dt/2)};
        State k4 = {y[1] + k3[1] * dt, f(y[0] + k3[0] * dt)};

        return {(k1[0] + 2 * k2[0] + 2 * k3[0] + k4[0]) * dt/6,
                (k1[1] + 2 * k2[1] + 2 * k3[1] + k4[1]) * dt/6};
}

State backward_euler(const State& y, const double& dt) {
        State d = {0, 0};
        d[0] = (y[1] * dt - y[0] * omega * omega * dt * dt) /
                (1 + omega * omega * dt * dt);
        d[1] = dt * f(y[0] + d[0]);

        return d;
}

State simple_add(State y, State d) {
        return {y[0] + d[0], y[1] + d[1]};
}

State c = {0.0, 0.0};
State kahan_add(State y, State d) {
        d = {d[0] - c[0], d[1] - c[1]};
        State res = {y[0] + d[0], y[1] + d[1]};
        c = {(res[0] - y[0]) - d[0], (res[1] - y[1]) - d[1]};

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
        auto summation_name = data.at("summation").get<std::string>();
        auto bound = data.at("bound").get<double>();
        auto count = data.at("count").get<double>();

        std::function<State(const State& y, const double& dt)> method;
        if (method_name == "euler")
                method = euler;
        else if (method_name == "heun")
                method = heun;
        else if (method_name == "rk4")
                method = runge_kutta;
        else if (method_name == "bw_eu")
                method = backward_euler;
        else {
                std::cout << "Unkown method: " << method_name <<
                        std::endl;
                return 2;
        }

        std::function<State(State y, State d)> summation;
        if (summation_name == "simple")
                summation = simple_add;
        else if (summation_name == "kahan")
                summation = kahan_add;
        else {
                std::cout << "Unkown summation: " << summation_name <<
                        std::endl;
                return 2;
        }
        
        omega = data.at("omega").get<double>();
        double x0 = data.at("x0").get<double>();
        double v0 = data.at("v0").get<double>();

        std::ofstream output(output_name);

        State d = {0, 0};
        State y = {x0, v0};

        double dt = bound/count;
        output << std::setprecision(17);
        for (int i = 0; i < count; i++) {
                output << i * dt << ' ' << y[0] << ' ' << y[1] << '\n';
                d = method(y, dt);
                y = summation(y, d);
        }

        output.close();

        return 0;
}
