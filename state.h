#ifndef __STATE_H__
#define __STATE_H__

enum class State { ACTIVE, INACTIVE };


class EnergyStateMachine {
private:
    State state;
    double delta0;
    double delta1;
    std::mt19937 generator;
    std::uniform_real_distribution<double> distribution;

public:

    EnergyStateMachine(double d0, double d1);
    State current_state() const;
    void set_state(State new_state);
    State step();

};


#endif // __STATE_H__