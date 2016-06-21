#include "cpul/cpulFailure.h"

Failure::Failure(
    std::string theCondition,
    std::string theFileName,
    long theLineNumber)
    : condition(theCondition),
    fileName(theFileName),
    lineNumber(theLineNumber)
{
}

std::ostream& operator<<(
    std::ostream& stream,
    Failure& failure)
{
    stream << std::endl
           << failure.fileName
           << "(" << failure.lineNumber << ") : "
           << "Failure: " << failure.condition;
    return stream;
}
