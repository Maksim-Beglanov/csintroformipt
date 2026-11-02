#ifndef ABSTRACT_SOLVER_HPP
#define ABSTRACT_SOLVER_HPP

#include <vector>
#include <fstream>
#include <cstddef>

// Physical state vector
using State = std::vector<double>;

// Differential equation: dY/dt = F(t, Y)
class Field {
public:
    virtual State operator()(const double& t,
                             const State& Y) const = 0;

    virtual ~Field() = default;
};

// Numerical integration scheme
// Returns the increment dY for one time step
class Scheme {
public:
    virtual State operator()(const double& t,
                             const State& Y,
                             const Field& F,
                             const double& dt) const = 0;

    virtual ~Scheme() = default;
};

class Euler : public Scheme {
public:
    State operator()(const double& t,
                     const State& Y,
                     const Field& F,
                     const double& dt) const override;
};


class Heun : public Scheme {
public:
    State operator()(const double& t,
                     const State& Y,
                     const Field& F,
                     const double& dt) const override;
};


class RungeKutta : public Scheme {
public:
    State operator()(const double& t,
                     const State& Y,
                     const Field& F,
                     const double& dt) const override;
};

// State update: Y_new = Summation(Y, dY)
class Summation {
public:
    virtual State operator()(State& Y,
                             const State& dY) = 0;

    virtual ~Summation() = default;
};

class NaiveSum : public Summation {
public:
    State operator()(State& Y,
                     const State& dY) override;
};


class KahanSum : public Summation {
public:
    State operator()(State& Y,
                     const State& dY) override;

private:
    State correction;
};

// Output interface
class Dump {
public:
    virtual void operator()(const double& t,
                            const State& Y,
                            std::ofstream& output) const = 0;

    virtual ~Dump() = default;
};

class CSVDump : public Dump {
public:
    void operator()(const double& t,
                    const State& Y,
                    std::ofstream& output) const override;
};


class BinaryDump : public Dump {
public:
    void operator()(const double& t,
                    const State& Y,
                    std::ofstream& output) const override;
};

class Solver {
public:
    Solver(State Y0,
           const Field& F,
           const Scheme& Sch,
           Summation& Smt,
           const Dump& D);

    void solve(std::ofstream& output,
               const double& bound,
               const std::size_t& count);

private:
    State Y;

    const Field& F;
    const Scheme& Sch;
    Summation& Smt;
    const Dump& D;
};

#endif
