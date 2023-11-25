#ifndef _HEADER_MARCO_
#define _HEADER_MARCO_

#ifndef SIMULATION
#include "cadmium/modeling/devs/atomic.hpp"
#else
#include <cadmium/core/modeling/atomic.hpp>
#endif


enum _PHASE_NAME_
{
    _PHASE_FIELDS_
};

namespace _NAMESPACE_NAME_
{
    struct _STATE_NAME_
    {
        double sigma;
        _PHASE_NAME_ phase;
        _STATE_NAME_(_PHASE_NAME_ phase) : sigma(), phase(phase) {}
    };

    std::ostream &operator<<(std::ostream &out, const _STATE_NAME_ &s)
    {
        return out;
    }

    class _MODEL_NAME_ : public Atomic<_STATE_NAME_>
    {
    public:
_PORT_DECLERATIONS_

        _MODEL_NAME_(const std::string &id)
            :
                Atomic<_STATE_NAME_>(id, _STATE_NAME_(_PHASE_NAME_::idle))
        {
_PORT_DEFINITIONS_
        }

        void internalTransition(_STATE_NAME_ &s) const override
        {
_INTERNAL_TRANSITION_
        }

        void externalTransition(_STATE_NAME_ &s, double e) const override
        {
_EXTERNAL_TRANSITION_
        }

        void output(const _STATE_NAME_ &s) const override {
_OUTPUT_
        }

        [[nodiscard]] double timeAdvance(const _STATE_NAME_ &s) const override
        {
            return s.sigma;
        }
    };
}

#endif
