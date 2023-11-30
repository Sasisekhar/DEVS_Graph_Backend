#ifndef Bar_H
#define Bar_H

#ifndef SIMULATION
#include "cadmium/modeling/devs/atomic.hpp"
#else
#include <cadmium/core/modeling/atomic.hpp>
#endif

#include <cadmium/lib/iestream.hpp>


using namespace cadmium::lib;

enum class BarPhase
{
    	idle,
	ready
};

namespace cadmium::iot
{
    struct BarState
    {
        double sigma;
        BarPhase phase;
        BarState(BarPhase phase) : sigma(std::numeric_limits<double>::infinity()), phase(phase) {}
    };

    std::ostream &operator<<(std::ostream &out, const BarState &s)
    {
        return out;
    }

    class Bar : public Atomic<BarState>
    {
    public:
	Port<int> in4;
	Port<int> out3;

        Bar(const std::string &id)
            :
                Atomic<BarState>(id, BarState(BarPhase::idle))
        {
		in4 = addInPort<int>("in4");
		out3 = addOutPort<int>("out3");
        }

        void internalTransition(BarState &s) const override
        {
		switch(s.phase) {
			case(BarPhase::ready):
				s.phase = BarPhase::idle;
				s.sigma = std::numeric_limits<double>::infinity();
				break;
		}
        }

        void externalTransition(BarState &s, double e) const override
        {
			if(in4->getBag().back()) {
				if(in4->getBag().back() == 1 && s.phase == BarPhase::idle) {
					s.phase = BarPhase::ready;
					s.sigma = 4;
				}
			}
        }

        void output(const BarState &s) const override {
		if(s.phase == BarPhase::ready) { out3->addMessage(2); }

        }

        [[nodiscard]] double timeAdvance(const BarState &s) const override
        {
            return s.sigma;
        }
    };
}

#endif
