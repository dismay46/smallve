/*

  smallve GPL Source Code
  Copyright (C) 2014 Yury Shatilin.

  This file is part of the smallve GPL Source Code (smallve Source Code).

  smallve Source Code is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  smallve Source Code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with smallve Source Code. If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef LOGGER_H
#define LOGGER_H

// TODO: singleton

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "imessagewriter.h"

namespace smle {

using std::string;

enum class LoggerLevel : int {
    High = 0,	// output messages, errors, warnings
    Average, 	// output errors, warnings
    Weak,		// output errors
    Off,		// disable output
};

class Logger final
{
private:
    Logger(IMessageWriter *_writer);

    Logger(const Logger & logger) = delete;

    Logger& operator=(Logger & logger) = delete;

    std::string mLastMessage;

    static const std::string WARR_PREFIX;
    static const std::string MESS_PREFIX;
    static const std::string ERR_PREFIX;

    template<class T>
    void write(std::string prefix, const T & mess);

    LoggerLevel mLevel = LoggerLevel::High;

    IMessageWriter * mWriter = nullptr;
public:
    static Logger & instance();

    template<class T>
    Logger & operator<< (const T & mess);

    template<class T>
    void errorWrite(const T & mess);

    template<class T>
    void warningWrite(const T & mess);

    template<class T>
    void messageWrite(const T & mess);

    ~Logger();

    LoggerLevel getLevel() const;
    void setLevel(const LoggerLevel &value);

    inline const std::string & getLastMessage() const;
};

const std::string &Logger::getLastMessage() const
{
    return mLastMessage;
}

template<class T>
void Logger::write(std::string prefix, const T &mess)
{
    if (mWriter != nullptr) {
        std::stringstream ss;
        ss << prefix << ": " << mess << std::endl;
        mLastMessage = ss.str();
        mWriter->write(ss.str());
    }
}


template <class T>
void Logger::messageWrite(const T & obj)
{
    if (mLevel <= LoggerLevel::High) {
        write(MESS_PREFIX,  obj);
    }
}

template <class T>
void Logger::warningWrite(const T & obj)
{
    if (mLevel <= LoggerLevel::Average ) {
        write(WARR_PREFIX,  obj);
    }
}

template <class T>
void Logger::errorWrite(const T & obj)
{
    if (mLevel <= LoggerLevel::Weak) {
        write(ERR_PREFIX,  obj);
    }
}

template<class T>
Logger &Logger::operator<<(const T & obj)
{
    messageWrite(obj);
    return *this;
}

} // namespace smle

#endif // LOGGER_H
