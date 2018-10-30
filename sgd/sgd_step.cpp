#include "faasm.h"
#include "sgd_constants.h"
#include "counter.h"
#include "matrix.h"

namespace faasm {
    int exec(FaasmMemory *memory) {
        const uint8_t *input = memory->getInput();
        const int *inputParams = reinterpret_cast<const int *>(input);

        int workerIdx = inputParams[0];
        int startIdx = inputParams[1];
        int endIdx = inputParams[2];
        int nWeights = inputParams[3];

        const double learningRate = 0.1;

        // Load the current weights
        MatrixXd weights = readMatrixFromState(memory, WEIGHTS_KEY, 1, nWeights);

        // Get corresponding training data for inputs and outputs
        MatrixXd inputs = readMatrixColumnsFromState(memory, INPUTS_KEY, startIdx, endIdx, nWeights);
        MatrixXd outputs = readMatrixColumnsFromState(memory, OUTPUTS_KEY, startIdx, endIdx, 1);

        // Work out what the current weights would actually give us
        MatrixXd actual = weights * inputs;

        // Work out the error
        MatrixXd error = actual - outputs;

        // Work out the step size
        MatrixXd steps = (2.0 * learningRate) * error * inputs.transpose();

        // Perform updates to weights
        for (int i = 0; i < nWeights; i++) {
            double stepSize = steps(0, i);

            // Ignore (effectively) zero gradients
            if (abs(stepSize) <= 0.00001) {
                continue;
            }

            // Make the update
            weights(0, i) -= stepSize;
            writeMatrixStateElement(memory, WEIGHTS_KEY, weights, 0, i);
        }

        // Persist the overall squared error
        char batchKey[10];
        sprintf(batchKey, "batch-%i", workerIdx);

        double squaredError = calculateSquaredError(actual, outputs);
        auto squaredErrorBytes = reinterpret_cast<uint8_t *>(&squaredError);
        memory->writeState(batchKey, squaredErrorBytes, sizeof(double));

        return 0;
    }
}