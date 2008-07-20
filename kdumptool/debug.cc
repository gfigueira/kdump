/*
 * (c) 2008, Bernhard Walle <bwalle@suse.de>, SUSE LINUX Products GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
#include <sstream>

#include "debug.h"

// -----------------------------------------------------------------------------
Debug *Debug::m_instance = NULL;

using std::memset;
using std::strcat;
using std::stringstream;
using std::strcat;
using std::strlen;
using std::string;
using std::endl;

#define ANSI_COLOR_NORMAL   "\e[0m"
#define ANSI_COLOR_RED      "\e[31m"
#define ANSI_COLOR_GREEN    "\e[32m"
#define ANSI_COLOR_YELLOW   "\e[33m"

// -----------------------------------------------------------------------------
Debug *Debug::debug()
{
    if (!m_instance)
        m_instance = new Debug();

    return m_instance;
}

// -----------------------------------------------------------------------------
Debug::Debug()
    : m_debuglevel(DL_NONE), m_handle(stderr), m_useColor(false),
      m_useColorAuto(true)
{}

// -----------------------------------------------------------------------------
void Debug::setLevel(Debug::Level level)
{
    m_debuglevel = level;
}

// -----------------------------------------------------------------------------
void Debug::setFileHandle(FILE *handle)
{
    if (!handle)
        m_handle = stderr;
    else
        m_handle = handle;
}

// -----------------------------------------------------------------------------
FILE *Debug::getFileHandle() const
{
    return m_handle;
}

// -----------------------------------------------------------------------------
void Debug::dbg(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vmsg(DL_DEBUG, msg, valist);
    va_end(valist);
}

// -----------------------------------------------------------------------------
void Debug::info(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vmsg(DL_INFO, msg, valist);
    va_end(valist);
}

// -----------------------------------------------------------------------------
void Debug::trace(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vmsg(DL_TRACE, msg, valist);
    va_end(valist);
}

// -----------------------------------------------------------------------------
void Debug::dbg(const string &string)
{
    return dbg(string.c_str());
}

// -----------------------------------------------------------------------------
void Debug::info(const string &string)
{
    return info(string.c_str());
}

// -----------------------------------------------------------------------------
void Debug::trace(const string &string)
{
    return trace(string.c_str());
}


// -----------------------------------------------------------------------------
void Debug::msg(Debug::Level level, const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vmsg(level, msg, valist);
    va_end(valist);
}

// -----------------------------------------------------------------------------
void Debug::vmsg(Debug::Level level, const char *msg, va_list list)
{
    // if the global debug level is too small, then just do nothing
    if (level < m_debuglevel)
        return;

    stringstream newss;

    // prepend dump level
    switch (level) {
        case DL_TRACE:
            if (useColor())
                newss << ANSI_COLOR_GREEN;
            newss << "TRACE: ";
            break;

        case DL_INFO:
            if (useColor())
                newss <<  ANSI_COLOR_RED;
            newss << "INFO: ";
            break;

        case DL_DEBUG:
            if (useColor())
                newss << ANSI_COLOR_YELLOW;
            newss << "DEBUG: ";
            break;

        default:    // make the compiler happy
            break;
    }
    newss << msg;

    // append "all attributes off" if we use color
    if (useColor())
        newss << ANSI_COLOR_NORMAL;

    // append '\n' if there's no one at the end
    string news = newss.str();
    if (news[news.size()-1] != '\n')
        newss << endl;

    vfprintf(m_handle, newss.str().c_str(), list);
    fflush(m_handle);
}

// -----------------------------------------------------------------------------
Debug::Level Debug::getLevel() const
{
    return m_debuglevel;
}

// -----------------------------------------------------------------------------
bool Debug::isDebugEnabled() const
{
    return m_debuglevel < DL_NONE;
}

// -----------------------------------------------------------------------------
void Debug::setUseColor(bool useColor)
{
    m_useColor = useColor;
    m_useColorAuto = false;
}

// -----------------------------------------------------------------------------
bool Debug::useColor() const
{
    if (m_useColorAuto)
        return isatty(STDERR_FILENO) && m_handle == stderr;
    else
        return m_useColor;
}

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
