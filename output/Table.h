#ifndef Table_H
#define Table_H

#ifndef SIMULATION
#include "cadmium/modeling/devs/atomic.hpp"
#else
#include <cadmium/core/modeling/atomic.hpp>
#endif

#include <cadmium/lib/iestream.hpp>


using namespace cadmium::lib;

enum class TablePhase
{
    	idle,
	ready
};

namespace cadmium::iot
{
    struct TableState
    {
        double sigma;
        TablePhase phase;
        TableState(TablePhase phase) : sigma(std::numeric_limits<double>::infinity()), phase(phase) {}
    };

    std::ostream &operator<<(std::ostream &out, const TableState &s)
    {
        return out;
    }

    class Table : public Atomic<TableState>
    {
    public:
	Port<int> in5;
	Port<int> out4;

        Table(const std::string &id)
            :
                Atomic<TableState>(id, TableState(TablePhase::idle))
        {
		in5 = addInPort<int>("in5");
		out4 = addOutPort<int>("out4");
        }

        void internalTransition(TableState &s) const override
        {
		switch(s.phase) {
			case(TablePhase::ready):
				s.phase = TablePhase::idle;
				s.sigma = std::numeric_limits<double>::infinity();
				break;
		}
        }

        void externalTransition(TableState &s, double e) const override
        {
			if(in5->getBag().back()) {
				if(in5->getBag().back() == 1 && s.phase == TablePhase::idle) {
					s.phase = TablePhase::ready;
					s.sigma = 15;
				}
			}
        }

        void output(const TableState &s) const override {
		if(s.phase == TablePhase::ready) { out4->addMessage(3); }

        }

        [[nodiscard]] double timeAdvance(const TableState &s) const override
        {
            return s.sigma;
        }
    };
}

#endif
