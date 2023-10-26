//
// Created by teapot on 26/10/23.
//

#ifndef TEASDK_TARGET_H
#define TEASDK_TARGET_H

namespace libutils::target {
    /**
     * @brief gets the target architecture that this library is currently being built for
     *
     * @return target for this library
     */
    consteval const char *getTargetArch();
}

#endif //TEASDK_TARGET_H
