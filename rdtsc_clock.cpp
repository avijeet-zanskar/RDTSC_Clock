#include <iostream>

#include <x86intrin.h>
#include <cpuid.h>

#include "rdtsc_clock.hpp"

namespace RDTSC_Clock {
    static double GET_RDTSC_TICK_FREQ() {
        unsigned int eax_denominator, ebx_numerator, ecx_hz, edx;
        __get_cpuid(0x15, &eax_denominator, &ebx_numerator, &ecx_hz, &edx);
        return ((static_cast<double>(ecx_hz) * ebx_numerator) / eax_denominator) / 1e9;
    }

    double internal::RDTSC_TICK_FREQ;
    unsigned int temp;
    unsigned long long internal::initialTimestamp;
    unsigned long long internal::initialCycles;

    void init() {
        internal::RDTSC_TICK_FREQ = GET_RDTSC_TICK_FREQ();
        std::cout << "RDTSC_TICK_FREQ: " << internal::RDTSC_TICK_FREQ << '\n';
        internal::initialTimestamp  = std::chrono::system_clock::now().time_since_epoch().count();
        internal::initialCycles = __rdtscp(&temp);
    }

    unsigned long long now(unsigned long long elapsedCycles) {    
        return (internal::initialTimestamp + static_cast<unsigned long long>(static_cast<long double>(elapsedCycles - internal::initialCycles) / internal::RDTSC_TICK_FREQ));
    }
}