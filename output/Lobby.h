#ifndef Lobby_H
#define Lobby_H

#ifndef SIMULATION
#include "cadmium/modeling/devs/atomic.hpp"
#else
#include <cadmium/core/modeling/atomic.hpp>
#endif

#include <cadmium/lib/iestream.hpp>


using namespace cadmium::lib;

enum class LobbyPhase
{
    		idle,
	bar,
	table
};

namespace cadmium::iot
{
    struct LobbyState
    {
        double sigma;
        LobbyPhase phase;
        LobbyState(LobbyPhase phase) : sigma(std::numeric_limits<double>::infinity()), phase(phase) {}
    };

    std::ostream &operator<<(std::ostream &out, const LobbyState &s)
    {
        return out;
    }

    class Lobby : public Atomic<LobbyState>
    {
    public:
		Port<int> in1;
	Port<int> out1;
	Port<int> out2;

        Lobby(const std::string &id)
            :
                Atomic<LobbyState>(id, LobbyState(LobbyPhase::idle))
        {
				in1 = addInPort<int>("in1");
		out1 = addOutPort<int>("out1");
		out2 = addOutPort<int>("out2");
        }

        void internalTransition(LobbyState &s) const override
        {
		switch(s.phase) {
			case(LobbyPhase::bar):
				s.phase = LobbyPhase::idle;
				s.sigma = std::numeric_limits<double>::infinity();
				break;
			case(LobbyPhase::table):
				s.phase = LobbyPhase::idle;
				s.sigma = std::numeric_limits<double>::infinity();
				break;
		}
        }

        void externalTransition(LobbyState &s, double e) const override
        {
			if(in1->getBag().back()) {
				if(in1->getBag().back() == 1 && s.phase == LobbyPhase::idle) {
					s.phase = LobbyPhase::bar;
					s.sigma = 5;
				}
				if(in1->getBag().back() == 2 && s.phase == LobbyPhase::idle) {
					s.phase = LobbyPhase::table;
					s.sigma = 7;
				}
			}
        }

        void output(const LobbyState &s) const override {
		if(s.phase == LobbyPhase::bar) { out1->addMessage(1); }
		if(s.phase == LobbyPhase::table) { out2->addMessage(1); }

        }

        [[nodiscard]] double timeAdvance(const LobbyState &s) const override
        {
            return s.sigma;
        }
    };
}

#endif
