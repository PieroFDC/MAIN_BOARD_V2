#ifndef __FILTER_HPP
#define __FILTER_HPP

class Filter {
private:
    int bufferSize;             // Size of the circular buffer
    float* buffer;              // Circular buffer
    int currentIndex = 0;       // Current index of the circular buffer
    float exponentialSmoothingFactor; // Smoothing factor for the filter
    float previousMeasurement = 0; // Private variable to store the previous measurement

    float calculateAverage() {
        float sum = 0;
        for (int i = 0; i < bufferSize; i++) {
        sum += buffer[i];
        }
        return sum / bufferSize;
    }

    float movingAverageFilter(float measurement) {
        if (measurement != buffer[currentIndex]) {
        buffer[currentIndex] = measurement;
        currentIndex = (currentIndex + 1) % bufferSize;  // Move to the next circular index
        }

        float average = calculateAverage();
        return average;
    }

    float exponentialMovingAverageFilter(float measurement) {
        float smoothedValue = (exponentialSmoothingFactor * measurement) + ((1 - exponentialSmoothingFactor) * previousMeasurement);
        previousMeasurement = smoothedValue;
        return smoothedValue;
    }

public:
    Filter(int size, float smoothingFactor) : bufferSize(size), exponentialSmoothingFactor(smoothingFactor) {
        buffer = new float[bufferSize];
    }

    ~Filter() {
        delete[] buffer;
    }

    float filterData(float measurement) {
        float movingAverage = movingAverageFilter(measurement);
        if (currentIndex == 0) {
            float result = exponentialMovingAverageFilter(movingAverage);
            return result;
        } else {
            return movingAverage;
        }
    }
};

#endif //__FILTER_HPP