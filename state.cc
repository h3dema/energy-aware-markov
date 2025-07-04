#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <unordered_map>

#include "state.h"


EnergyStateMachine::EnergyStateMachine(double d0 = 0.7, double d1 = 0.1)
    : delta0(d0), delta1(d1), state(State::INACTIVE) {
    std::random_device rd;
    generator = std::mt19937(rd());
    distribution = std::uniform_real_distribution<double>(0.0, 1.0);
}

State EnergyStateMachine::current_state() const {
    return state;
}

void EnergyStateMachine::set_delta0(double d0) {
    delta0 = d0;
}

void EnergyStateMachine::set_delta1(double d1) {
    delta1 = d1;
}

void EnergyStateMachine::set_state(State new_state) {
    state = new_state;
}

State EnergyStateMachine::step() {
    double random_value = distribution(generator);
    State new_state = state;

    if (state == State::ACTIVE) {
        // State is ACTIVE
        // Transition to INACTIVE with probability delta1
        if (random_value < delta1) {
            new_state = State::INACTIVE;
        }
    } else {
        // State is INACTIVE
        // Transition to ACTIVE with probability delta0
        if (random_value < delta0) {
            new_state = State::ACTIVE;
        }
    }

    state = new_state;
    return new_state;
}
