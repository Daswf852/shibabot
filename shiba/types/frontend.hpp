#pragma once

namespace Shiba {

    class Frontend {
        public:
            virtual ~Frontend() = default;
            virtual void Start() = 0;
            virtual void Stop() = 0;
    };

}