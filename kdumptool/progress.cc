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
#include <iomanip>
#include <iostream>

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "progress.h"
#include "debug.h"

#define NAME_MAXLENGTH 30
#define DEFAULT_WIDTH  80

using std::string;
using std::setw;
using std::cout;
using std::left;
using std::right;
using std::endl;
using std::flush;
using std::ostream;

//{{{ Terminal -----------------------------------------------------------------

// -----------------------------------------------------------------------------
Terminal::Size Terminal::getSize() const
    throw ()
{
    Terminal::Size sz;
    struct winsize winsize;

    int err = ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
    if (err != 0) {
        sz.width = 0;
        sz.height = 0;
        return sz;
    }

    sz.width = winsize.ws_col;
    sz.height = winsize.ws_row;

    return sz;
}

// -----------------------------------------------------------------------------
void Terminal::printLine(ostream &os) const
    throw ()
{
    int width = getSize().width - 1;

    for (int i = 0; i < width; i++)
        os << '-';
    os << endl;
}

//}}}
//{{{ TerminalProgress ---------------------------------------------------------

// -----------------------------------------------------------------------------
TerminalProgress::TerminalProgress(const string &name)
    throw ()
    : m_name(name), m_lastUpdate(0)
{
    // truncate the name
    if (m_name.size() > NAME_MAXLENGTH) {
        m_name = m_name.substr(0, NAME_MAXLENGTH);
    }

    Terminal t;
    m_linelen = t.getSize().width;
    if (m_linelen == 0)
        m_linelen = DEFAULT_WIDTH;

    // subtract 1 for the space between string and bar, 1 for the leading
    // '|' and 1 for the trailing '|', and one space free, 4 for ...%
    m_progresslen = m_linelen - NAME_MAXLENGTH - 8;
}

// -----------------------------------------------------------------------------
void TerminalProgress::start()
    throw ()
{
    clearLine();
    cout << "\r" << setw(NAME_MAXLENGTH) << left << m_name << " Starting.";
}

// -----------------------------------------------------------------------------
void TerminalProgress::progressed(unsigned long long current,
                                  unsigned long long max)
    throw ()
{
    int number_of_hashes;
    int number_of_dashes;
    int percent;

    time_t now = time(NULL);
    if (now <= m_lastUpdate)
        return;
    if (current == 0 && max == 0) {
        Debug::debug()->dbg("TerminalProgress::progressed: current==0 and max==0");
        return;
    }

    percent = current*100/max;
    number_of_hashes = int(double(current)/max*m_progresslen);

    // normalise
    if (number_of_hashes > m_progresslen)
        number_of_hashes = m_progresslen;
    if (number_of_hashes < 0)
        number_of_hashes = 0;

    number_of_dashes = m_progresslen - number_of_hashes;

    cout << "\r" << setw(NAME_MAXLENGTH) << left << m_name << " ";
    cout << "|";
    for (int i = 0; i < number_of_hashes; i++)
        cout << '#';
    for (int i = 0; i < number_of_dashes; i++)
        cout << '-';
    cout << "|";
    cout << setw(3) << right << percent << '%'  << flush;

    m_lastUpdate = now;
}

// -----------------------------------------------------------------------------
void TerminalProgress::stop(bool success)
    throw ()
{
    const char *finish_msg = success
        ? " Finished."
        : " Failed.";

    clearLine();
    cout << "\r" << setw(NAME_MAXLENGTH) << left << m_name << finish_msg;
    cout << endl;
}

// -----------------------------------------------------------------------------
void TerminalProgress::clearLine()
    throw ()
{
    cout << "\r" << setw(m_linelen) << " ";
}

//}}}

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
