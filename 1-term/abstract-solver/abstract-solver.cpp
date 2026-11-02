#include "abstract-solver.hpp"

#include <stdexcept>
#include <utility>

namespace {

void check_sizes(const State& Y, const State& dY) {
    if (Y.size() != dY.size())
        throw std::invalid_argument(
            "State and increment have different dimensions"
        );
}

}

State Euler::operator()(const double& t,
                        const State& Y,
                        const Field& F,
                        const double& dt) const {
    State k = F(t, Y);

    for (double& value : k)
        value *= dt;

    return k;
}

State Heun::operator()(const double& t,
                       const State& Y,
                       const Field& F,
                       const double& dt) const {
    State k1 = F(t, Y);

    State buffer(Y.size());

    // Predictor
    for (std::size_t i = 0; i < Y.size(); ++i)
        buffer[i] = Y[i] + k1[i] * dt;

    // Corrector
    State k2 = F(t + dt, buffer);

    State dY(Y.size());

    for (std::size_t i = 0; i < Y.size(); ++i)
        dY[i] = (k1[i] + k2[i]) * dt / 2;

    return dY;
}

State RungeKutta::operator()(const double& t,
                             const State& Y,
                             const Field& F,
                             const double& dt) const {
    State k1 = F(t, Y);

    State buffer(Y.size());

    // k2
    for (std::size_t i = 0; i < Y.size(); ++i)
        buffer[i] = Y[i] + k1[i] * dt / 2;

    State k2 = F(t + dt / 2, buffer);

    // k3
    for (std::size_t i = 0; i < Y.size(); ++i)
        buffer[i] = Y[i] + k2[i] * dt / 2;

    State k3 = F(t + dt / 2, buffer);

    // k4
    for (std::size_t i = 0; i < Y.size(); ++i)
        buffer[i] = Y[i] + k3[i] * dt;

    State k4 = F(t + dt, buffer);

    // Final increment
    State dY(Y.size());

    for (std::size_t i = 0; i < Y.size(); ++i)
        dY[i] = dt / 6 *
            (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);

    return dY;
}

State NaiveSum::operator()(State& Y,
                           const State& dY) {
    check_sizes(Y, dY);

    State result(Y.size());

    for (std::size_t i = 0; i < Y.size(); ++i)
        result[i] = Y[i] + dY[i];

    return result;
}

State KahanSum::operator()(State& Y,
                           const State& dY) {
    check_sizes(Y, dY);

    if (correction.size() != Y.size())
        correction.assign(Y.size(), 0.0);

    State result(Y.size());

    for (std::size_t i = 0; i < Y.size(); ++i) {
        double d = dY[i] - correction[i];

        result[i] = Y[i] + d;

        correction[i] = (result[i] - Y[i]) - d;
    }

    return result;
}

void CSVDump::operator()(const double& t,
                         const State& Y,
                         std::ofstream& output) const {
    output << t;

    for (double value : Y)
        output << ' ' << value;

    output << '\n';
}

void BinaryDump::operator()(const double& t,
                            const State& Y,
                            std::ofstream& output) const {
    output.write(
        reinterpret_cast<const char*>(&t),
        sizeof(double)
    );

    output.write(
        reinterpret_cast<const char*>(Y.data()),
        Y.size() * sizeof(double)
    );
}

Solver::Solver(State Y0,
               const Field& F,
               const Scheme& Sch,
               Summation& Smt,
               const Dump& D)
    : Y(std::move(Y0)),
      F(F),
      Sch(Sch),
      Smt(Smt),
      D(D) {}

void Solver::solve(std::ofstream& output,
                   const double& bound,
                   const std::size_t& count) {
    if (count == 0)
        throw std::invalid_argument(
            "Number of steps must be positive"
        );

    double dt = bound / count;

    for (std::size_t i = 0; i < count; ++i) {
        double t = i * dt;

        // Write current state
        D(t, Y, output);

        // Calculate increment
        State dY = Sch(t, Y, F, dt);

        // Update state
        Y = Smt(Y, dY);
    }
}
