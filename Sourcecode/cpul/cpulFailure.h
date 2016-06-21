#ifndef FAILURE_H
#define FAILURE_H

// Failure records the circumstances of a failing test.

#include <string>
#include <ostream>

class Failure
{
public:
    Failure(
        std::string theCondition,
        std::string theFileName,
        long theLineNumber);

    std::string condition;
    std::string fileName;
    long lineNumber;
};

std::ostream& operator<< (std::ostream& stream, Failure& failure);

#endif
