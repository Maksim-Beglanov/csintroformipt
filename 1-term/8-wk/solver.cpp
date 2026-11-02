#include "../abstract-solver/abstract-solver.hpp"
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class PendulumField : public Field {
public:
        PendulumField(const double& omega, const double& gamma) :
                omega(omega), gamma(gamma) {}

        State operator()(const double& t, const State& Y) const override {
                return {
                        Y[1],
                        -omega * omega * Y[0] - 2 * gamma * Y[1]
                };
        }

private:
        double omega;
        double gamma;
};

void unknown_name(const std::string& type,
                               const std::string& name) {
        throw std::invalid_argument("Unknown " + type + ": " + name);
}

int main(int argc, char *argv[]) {
        if (argc != 2)
                throw std::invalid_argument(
                        "Expected ./prog \"settings.json\""
                );
        std::ifstream settings;
        try {
                settings.open(argv[1]);
        } catch (const std::ios_base::failure&) {
                throw std::runtime_error(
                        "Cannot open settings file: " + 
                        std::string(argv[1])
                );
        }
        if (!settings) {
                throw std::runtime_error(
                        "No such file exists: " +
                        std::string(argv[1])
                );
        }
        json data = json::parse(settings);
        settings.close();

        auto scheme_name = data.at("scheme").get<std::string>();
        std::unique_ptr<Scheme> shm;
        if (scheme_name == "euler")
                shm = std::make_unique<Euler>();
        else if (scheme_name == "heun")
                shm = std::make_unique<Heun>();
        else if (scheme_name == "rk4")
                shm = std::make_unique<RungeKutta>();
        else
                unknown_name("scheme", scheme_name);

        auto summation_name = data.at("summation").get<std::string>();
        std::unique_ptr<Summation> smt;
        if (summation_name == "simple")
                smt = std::make_unique<NaiveSum>();
        else if (summation_name == "kahan")
                smt = std::make_unique<KahanSum>();
        else
                unknown_name("summation", summation_name);

        auto output_name = data.at("output").get<std::string>();
        auto bin_flag = data.at("binary").get<bool>();
        std::unique_ptr<Dump> dmp;
        if (bin_flag)
                dmp = std::make_unique<BinaryDump>();
        else
                dmp = std::make_unique<CSVDump>();
        std::ofstream output(output_name,
                        bin_flag ? std::ios::binary : std::ios::out);

        auto omega = data.at("omega").get<double>();
        auto gamma = data.at("gamma").get<double>();
        PendulumField f(omega, gamma);

        double x0 = data.at("x0").get<double>();
        double v0 = data.at("v0").get<double>();

        auto bound = data.at("bound").get<double>();
        auto count = data.at("count").get<std::size_t>();

        Solver s({x0, v0}, f, *shm, *smt, *dmp);
        s.solve(output, bound, count);
}
