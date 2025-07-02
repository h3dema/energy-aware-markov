#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <unordered_map>

#include "state.h"
using namespace std;

// name of the output file (no extesion)
#define OUTPUT_FNAME "markov_chain_results"

void print_usage(const std::string& program_name) {
    std::cerr << "Usage: " << program_name << " [-d0 delta0] [-p p]\n";
    std::cerr << "Both parameters are optional. Default: delta0=0.7, p=0.1\n";
    std::cerr << "Examples:\n";
    std::cerr << "  " << program_name << " -d0 0.5 -p 0.2\n";
    std::cerr << "  " << program_name << " -p 0.2\n";
    std::cerr << "  " << program_name << " -d0 0.3\n";
    std::cerr << "  " << program_name << "\n";
}

bool parse_double(const std::string& s, double& value) {
    std::istringstream iss(s);
    return (iss >> value) && (iss.eof());
}

void parse_arguments(int argc, char* argv[], double& delta0, double& p, int& n_steps) {
    std::unordered_map<std::string, double*> arg_map = {
        {"-d0", &delta0},
        {"-p", &p},
    };
    // TODO: -n is not returning a valid value
    for (int i = 1; i < argc; i += 2) {
        if (i + 1 >= argc) {
            throw std::runtime_error("Missing value for argument " + std::string(argv[i]));
        }

        std::string key = argv[i];
        std::string val = argv[i+1];

        if (key == "-n") {
            try {
                n_steps = std::stoi(val);
                if (n_steps <= 0) {
                    throw std::runtime_error("-n must be a positive integer");
                }
            } catch (...) {
                throw std::runtime_error("Invalid value for -n: " + val);
            }
            continue;
        }

        auto it = arg_map.find(key);
        if (it == arg_map.end()) {
            throw std::runtime_error("Unknown argument: " + key);
        }

        double value;
        if (!parse_double(val.c_str(), value)) {
            throw std::runtime_error("Invalid value for " + key +
                                   ": " + std::string(argv[i+1]));
        }

        if ((key == "-d0" || key == "-p") && (value <= 0 || value >= 1)) {
            throw std::runtime_error(key + " must be between 0 and 1 (exclusive)");
        }

        *(it->second) = value;
    }
}

void simulate_markov_chain(int n_steps, double delta0, double delta1, double p) {
    EnergyStateMachine mc(delta0, delta1);
    std::vector<std::tuple<int, State, State>> results;

    for (int i = 0; i < n_steps; ++i) {
        State before = mc.current_state();
        State after = mc.step();
        results.emplace_back(i, before, after);
    }

    std::ofstream outfile(std::string(OUTPUT_FNAME) + ".csv");
    outfile << "step,state_before,state_after\n";

    for (const auto& record : results) {
        int step;
        State before, after;
        std::tie(step, before, after) = record;

        outfile << step << ","
                << (before == State::ACTIVE ? "active" : "inactive") << ","
                << (after == State::ACTIVE ? "active" : "inactive") << "\n";
    }

    outfile.close();

    std::ofstream outparams(std::string(OUTPUT_FNAME) + ".txt");
    outparams << "Simulation completed in " << n_steps << " steps. Results saved to markov_chain_results.csv\n";
    outparams << "Parameters used: delta0 = " << delta0 << ", delta1 = " << delta1 << ", p = " << p << std::endl;
    outparams.close();

    std::cout << "delta0 = " << delta0 << std::endl;
    std::cout << "delta1 = " << delta1 << std::endl;
    std::cout << "p = " << p << std::endl;

}


int main(int argc, char* argv[]) {
    double p = 0.492063492;
    double delta0 = 0.4;
    int n_steps = 100;

    try {
        parse_arguments(argc, argv, delta0, p, n_steps);
        // Calculate delta1 from p = delta0/(delta0 + delta1)
        // nota all combinations of delta0 and p are valid
        double delta1 = std::min(1.0, delta0 * (1 - p) / p);

        simulate_markov_chain(n_steps, delta0, delta1, p);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n\n";
        print_usage(argv[0]);
        return 1;
    }
    std::cout << "End " << n_steps << " steps." << std::endl;
    return 0;
}