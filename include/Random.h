//
// Created by sergey on 9/13/26.
//

#ifndef SIMULATION_RANDOM_H
#define SIMULATION_RANDOM_H
#include <random>
#include <chrono>

// Partly copied from https://www.learncpp.com/cpp-tutorial/global-random-numbers-random-h/
namespace Sim::Random {

inline std::mt19937 generate() {
    std::random_device rd{};

    std::seed_seq ss {
        static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
        rd(), rd(), rd(), rd(), rd(), rd(), rd()
    };

    return std::mt19937{ss};
}

inline std::mt19937 mt {generate()};

template<std::integral T>
T get(T min, T max) {
    return std::uniform_int_distribution{min, max}(mt);
}

// Example usage: get<R>(min, max)
template<std::integral R, std::integral T, std::integral U>
R get(T min, U max) {
    return get<R>(static_cast<R>(min), static_cast<R>(max));
}


}
#endif //SIMULATION_RANDOM_H
