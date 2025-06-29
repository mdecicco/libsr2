#pragma once
#include <libsr2/types.h>
#include <libsr2/ai/aiAgent.h>

namespace sr2 {
    class ambientAgent : public aiAgent {
        public:
            ambientAgent();
            virtual ~ambientAgent();

            mat3x4f maybeInitialTransform;
        
        private:
    };
};