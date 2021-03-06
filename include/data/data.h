#pragma once

#include <faasm/matrix.h>
#include <faasm/sgd.h>
#include <shared_mutex>

using namespace faasm;

namespace data {
    class SparseMatrixFileSerialiser : public SparseMatrixSerialiser {
    public:
        using SparseMatrixSerialiser::SparseMatrixSerialiser;

        void writeToFile(const std::string &directory);

        static SparseMatrix<double> readFromFiles(const std::string &directory);
    };

    void runPool(const SgdParams &params, int epoch);

    void clear();

    double getRMSE(const SgdParams &p);

    std::vector<double> getErrors();

    void run(int epoch, int batchNumber);
}
