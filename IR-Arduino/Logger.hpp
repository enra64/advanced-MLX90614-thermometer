#ifndef IR_THERMO_LOGGER_H
#define IR_THERMO_LOGGER_H

#include <SdFat.h>

#include "my_config.h"

namespace Logger {
    SdFat SD;
    char logFileName[13];

#define SD_READ_BUFFER_SIZE 8 // 8 bytes should be enough for "XXX.XX,\0"
#define error(s) Serial.println(s)

    /// Append a single data to the sd card. No buffering is done. Precision of 2.
    /// \param entry the value that is to be appended.
    /// \return true if the operation succeeded, false otherwise
    bool append(float entry) {
        ofstream sdout(logFileName, O_APPEND);
        sdout.precision(2);

        if (!sdout.is_open()) {
            error("LOG_NO_OPEN1");
            return false;
        }

        sdout << entry << ',';
        sdout.close();
        return true;
    }

    /// This function reads a single csv entry
    /// \param sdin the ifstream of the csv file
    /// \param buffer buffer for reading from the file. must be of size SD_READ_BUFFER_SIZE
    /// \param [out] result csv result param
    /// \return
    bool readEntry(ifstream &sdin, float& result) {
        if (sdin.fail() || sdin.eof()){
            Serial.println("fail");
            return false;
        }
        sdin >> result;
        sdin.ignore();
        //Serial.print("read: ");Serial.println(result);
        return true;
    }

    void resetFile(ifstream &sdin) {
        sdin.clear();
        sdin.seekg(0, sdin.beg);
    }

    size_t countEntries(ifstream &sdin) {
        // allocate buffer for sd reading, and result values
        float value;

        // count entries...
        size_t entryCount = 0;
        while (readEntry(sdin, value)) entryCount++;

        Serial.print("entry count is: ");Serial.println(entryCount);

        // reset the file state
        //resetFile(sdin);

        return entryCount;
    }

    /// GetLog returns log data from the sd card. It will subsample the points if more than "requestedPoints" are in the log.
    /// This function assumes that there is only a very small amount of ram available, and is thus very sequential.
    ///
    /// \param requestedPoints amount of points that should be returned
    /// \param data pointer to an array of floats with a minimum length of "requestedPoints"
    /// \param [out] success whether a failure was found
    /// \return number of points that were found
    size_t getLog(float *data, size_t requestedPoints, bool &success) {
        ifstream sdin(logFileName);
        success = true;

        if (!sdin.is_open()){
            success = false;
            Serial.println(F("LOG_NO_OPEN getLog"));
            return 0;
        }

        size_t entryCount = countEntries(sdin);

        // iterate over all available points
        if (entryCount <= requestedPoints) {
            Serial.println(F("test"));
            size_t readEntries = 0;
            while (readEntries < entryCount && success){
                success &= readEntry(sdin, data[readEntries++]);
            }

            for (size_t i = 0; i < readEntries; i++)Serial.println(data[i]);Serial.println(F("finished buffer printing test"));

            sdin.close();
            return readEntries;
        }

        // subsample as follows: read a chunk of at most MAX_CHUNK_SIZE values, store average in data array, read next chunk
        // if there is no clean division available, use a +1 size for the first AVL_ENTRIES % CHUNK_SIZE entries
        size_t baseChunkSize = entryCount / requestedPoints;
        size_t extendedChunkCount = entryCount % baseChunkSize;
        float valueBuffer = -1;

        for (size_t i = 0; i < requestedPoints; i++) {
            float runningMean = 0;

            // read entire chunk; may be extended size if no clean division is available
            size_t chunkSize = i < extendedChunkCount ? baseChunkSize + 1 : baseChunkSize;
            for (size_t j = 0; j < chunkSize; j++) {
                readEntry(sdin, valueBuffer);
                runningMean = runningMean + ((valueBuffer - runningMean) / (j + 1));
            }

            //Serial.print("running mean ");Serial.print(i);Serial.print(" is ");Serial.println(runningMean);
            data[i] = runningMean;
        }

        sdin.close();
        return requestedPoints;
    }

    /// Get the value of the last entry.
    /// \param [out] lastValue last value
    /// \return true if a last value could be retrieved
    bool getLastEntry(float &lastValue) {
        // open input stream
        ifstream sdin(logFileName);
        if (!sdin.is_open()) {
            error("LOG_NO_OPEN lastEntry");
            return false;
        }

        // seek to the last value
        sdin.seekg(sdin.end);
        sdin.seekg(5, sdin.beg);

        // read and return the last entry
        bool retVal = readEntry(sdin, lastValue);

        sdin.close();

        return retVal;
    }

    void chooseLogFile() {
        for (uint16_t i = 0; i < UINT16_MAX; i++) {
            itoa(i, logFileName, 10);
            if (!SD.exists(logFileName))
                break;
        }
    }

    void init() {
        SD.begin(SPI_CS_SD);
        chooseLogFile();
    }
}

#endif