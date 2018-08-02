#include "infra.h"

#include <util/util.h>

namespace infra {
    std::string getFunctionFile(message::FunctionCall &call) {
        std::string projRoot = util::getEnvVar("PROJ_ROOT", "/usr/local/code/faasm");
        std::string filePath = projRoot + "/wasm/" + call.user() + "/" + call.function() + "/function.wasm";

        std::cout << "FP " << filePath;

        return filePath;
    }
}
