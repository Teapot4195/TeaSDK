//
// Created by teapot on 03/11/23.
//

#ifndef TEASDK_LIBX86_H
#define TEASDK_LIBX86_H

#include <fstream>

namespace libx86 {
    class IFStreamInstructionEncoder {
    public:
        explicit IFStreamInstructionEncoder(std::ifstream stream);

    private:
        std::ofstream ostream;
    };
}

#endif //TEASDK_LIBX86_H
