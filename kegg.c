#include <elim.h>
typedef struct measurementResult {
    double *variables;
    double y;
    double error;
} MeasurementResult;

typedef struct measurement {
    int varnum;
    int count;
    MeasurementResult *results;
} Measurement;

void fit(Measurement data, int maxpow) {

}
