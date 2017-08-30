#ifndef IR_THERMO_LOGGER_H
#define IR_THERMO_LOGGER_H

#include <SdFat.h>

#include "my_config.h"
#include "FloatPrinter.h"

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
        // if we reached the EOF before, but without reading over file boundaries, we should find out here
        if(sdin.eof()) return false;

        float tmpReadTarget;
        sdin >> tmpReadTarget;
        sdin.ignore();

        // check if we tried to read beyond file boundaries
        if (sdin.fail())
            return false;

        result = tmpReadTarget;
        return true;
    }

    bool readEntry(ifstream& sdin, int16_t& result){
        static float buffer;
        if(!readEntry(sdin, buffer))
            return false;

        result = static_cast<int16_t>(buffer);
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

        // reset the file state
        resetFile(sdin);

        return entryCount;
    }

    /// GetLog returns log data from the sd card. It will subsample the points if more than "requestedPoints" are in the log.
    /// This function assumes that there is only a very small amount of ram available, and is thus very sequential.
    ///
    /// \param data pointer to an array of int32_t's with a minimum length of "requestedPoints"
    /// \param requestedPoints amount of points that should be returned
    /// \param retrievedPoints [out] number of points that were found
    /// \return success true if no failure was detected
    bool getLog(int16_t *data, size_t requestedPoints, size_t& retrievedPoints) {
        ifstream sdin(logFileName);

        if (!sdin.is_open()){
            retrievedPoints = 0;
            Serial.println(F("LOG_NO_OPEN getLog"));
            return false;
        }

        size_t entryCount = countEntries(sdin);

        // iterate over all available points if they all fit into the buffer
        bool success = true;
        retrievedPoints = 0;
        if (entryCount <= requestedPoints) {
            while (retrievedPoints < entryCount && success)
                success &= readEntry(sdin, data[retrievedPoints++]);

            sdin.close();
            return true;
        }

        // subsample as follows: read a chunk of at most MAX_CHUNK_SIZE values, store average in data array, read next chunk
        // if there is no clean division available, use a +1 size for the first AVL_ENTRIES % CHUNK_SIZE entries
        size_t baseChunkSize = entryCount / requestedPoints;
        size_t extendedChunkCount = entryCount % baseChunkSize;
        float valueBuffer = -1;

        for (retrievedPoints = 0; retrievedPoints < requestedPoints; retrievedPoints++) {
            float runningMean = 0;

            // read entire chunk; may be extended size if no clean division is available
            size_t chunkSize = retrievedPoints < extendedChunkCount ? baseChunkSize + 1 : baseChunkSize;
            //Serial_printf("chunk size should be: ", chunkSize);
            for (size_t j = 0; j < chunkSize; j++) {
                readEntry(sdin, valueBuffer);
                runningMean = runningMean + ((valueBuffer - runningMean) / (j + 1));
            }

            data[retrievedPoints] = static_cast<int32_t>(round(runningMean));
        }



        sdin.close();
        return true;
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

        // seek to before the last comma
        sdin.seekg(-2, sdin.end);

        // search to find the position of the second-to-last comma
        while(true) {
            // break if at file start position
            if(sdin.tellg() == 0)
                break;

            int currentChar = sdin.get();

            // sd at end or no char available -> bad
            if(currentChar == -1 || sdin.eof() || sdin.fail())
                return false;

            // found the comma!
            if (currentChar == ',')
                break;

            // no comma, go backwards some more; have to go <<in front of>> the one before the one that was just read in
            sdin.seekg(-2, sdin.cur);
        }

        // read and return the last entry
        bool retVal = readEntry(sdin, lastValue);
        sdin.close();
        return retVal;
    }

    bool chooseLogFile() {
        for (uint16_t i = 0; i < UINT16_MAX; i++) {
            itoa(i, logFileName, 10);
            if (!SD.exists(logFileName))
                return true;
        }
        return false;
    }

    bool init() {
        bool success = true;
        success &= SD.begin(SPI_CS_SD);
        success &= chooseLogFile();
        return success;
    }
}

#endif