#include <libnotify/notify.h>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <cmath>
#include <iostream>
#include <random>
#include <csignal>

/*
 compile as:
    `pkg-config --cflags --libs libnotify` -std=c++17
*/

#define TERMINAL_MAX_LENGTH 120
#define MAXIMAL_PROGRESS_BAR_LENGTH TERMINAL_MAX_LENGTH - 6

std::string notification;

int intRand(const int & min, const int & max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
    return dist6(rng);
}

class MagicTerminalString {
    int length;
    std::string spaces, last;
    bool flushed;
    bool changed;
public:
    MagicTerminalString (): changed(false), length(0), flushed(false) {};
    MagicTerminalString (const std::string &s, int length = 20): length(length), spaces(length, ' '), changed(true) {
        last = (s + spaces).substr(0, length);
        printf("%s\r", last.c_str());
        fflush(stdout);
    }
    void flush() {
        printf("%s\n", last.c_str());
        fflush(stdout);
        last.clear();
        spaces.clear();
        length = 0;
        flushed = true;
        changed = true;
    }
    void update(const std::string &s) {
        if (s.length() > length && s.length() < TERMINAL_MAX_LENGTH) {
            length = s.length();
            spaces.assign(length, ' ');
        }
        last = (s + spaces).substr(0, length);
        printf("%s\r", last.c_str());
        flushed = false;
        changed = true;
        fflush(stdout);
    }
    ~MagicTerminalString () {
        if (!flushed && changed) {
            printf("\r%s\n", last.c_str());
            fflush(stdout);
        }
    }
};

struct Notification {
    Notification (const std::string &name, const std::string &message, int timeout = 10000) {
        notify_init("Sample");
        NotifyNotification* n = notify_notification_new (name.c_str(), 
                                    message.c_str(),
                                    0);
        notify_notification_set_timeout(n, timeout); // 10 seconds

        if (!notify_notification_show(n, 0)) 
        {
            std::cerr << "showing has failed" << std::endl;
            exit(-1);
        }
    }
};

class Timer {
    int dd, hh, mm, ss;
    unsigned s;
public:
    Timer (unsigned seconds = 0): s(seconds) {
        dd = seconds / (60 * 60 * 24);
        seconds -= dd * (60 * 60 * 24);
        hh = seconds / (60 * 60);
        seconds -= hh * (60 * 60);
        mm = seconds / 60;
        seconds -= mm * 60;
        ss = seconds;
    }
    unsigned time () const {
        return s;
    }
    Timer& operator-- () {
        --s;
        if (ss == 0) {
            if (mm == 0) {
                if (hh == 0) {
                    if (dd == 0) {
                        throw std::logic_error("Time cannot be negative");
                    } else {
                        --dd;
                        ss = 59;
                        mm = 59;
                        hh = 59;
                    }
                } else {
                    --hh;
                    ss = 59;
                    mm = 59;
                }
            } else {
                --mm;
                ss = 59;
            }
        } else {
            --ss;
        }
        return (*this);
    }
    Timer& operator++ () {
        ++s;
        if (ss == 59) {
            if (mm == 59) {
                if (hh == 23) {
                    ++dd;
                    hh = 0;
                    mm = 0;
                    ss = 0;
                } else {
                    ++hh;
                    mm = 0;
                    ss = 0;
                }
            } else {
                ++mm;
                ss = 0;
            }
        } else {
            ++ss;
        }
        return *this;
    }
    bool null () const {
        return (!dd && !hh && !mm && !ss);
    }
    std::string repr () const {
        std::string out;
        if (dd)
            out = std::to_string(dd) + " d ";
        if (hh)
            out += std::to_string(hh) + " h ";
        if (mm)
            out += std::to_string(mm) + " m ";
        out += std::to_string(ss) + " s";
        return out;
    }
};

Timer t;

class ProgressBar {
    double current;
    int cur, maxload;
    std::string s;
    char sym, end;
public:
    ProgressBar (const double &percent, 
                const int &maxload, 
                char symbol = '#',
                char end = '.'): current(percent), sym(symbol), end(end) {
        ProgressBar::maxload = std::min(maxload, MAXIMAL_PROGRESS_BAR_LENGTH);
        cur = std::min(int(round(current * maxload)), maxload);
        std::string proc = std::to_string(std::min(int(percent * 100), 100));
        s = std::string(cur, sym) + std::string(maxload - cur , end);
        s += " " + proc + "%";
    }
    std::string show () const {
        return s;
    }
    void update (const double &percent) {
        cur = std::min(int(round(percent * maxload)), maxload);
        std::string proc = std::to_string(std::min(int(percent * 100), 100));
        s = std::string(cur, sym) + std::string(maxload - cur , end);
        s += " " + proc + "%";
    }
};

void signal_handler (int signal_num) { 
    if (!fork()) {
        std::cout << "caught a signal" << std::endl;
        sleep(t.time());
        Notification n("You've set a timer:", notification);
    }
    // terminate program   
    exit(signal_num);   
} 

int main(int argc, char *argv[])
{
    signal(SIGINT, signal_handler);
    MagicTerminalString x;
    if (argc > 1) {
        std::string arg(argv[1]);
        if (arg == "timer") {
            //TIMER:
            unsigned time = 10U;
            if (argc > 2) {
                time = std::stoul(argv[2]);
                if (argc > 3) {
                    for (int i = 3; i < argc; ++i) {
                        notification += argv[i];
                        notification += " ";
                    }
                } else {
                    notification = "Here is notification";
                }
            }
            t = Timer(time);
            for (; !t.null(); --t) {
                x.update("Time left: " + t.repr());
                sleep(1);
            }
            x.flush();
            Notification n("You've set a timer:", notification);
        } else if (arg == "progress") {
            //PROGRESS BAR:
            unsigned maxload = (argc > 1) ? std::stoul(argv[2]) : 10;
            double perc = 0.0;
            ProgressBar pb(perc, maxload);
            x.update(pb.show());
            while (perc < 1.0) {
                perc += double(intRand(0, 30)) / 100;
                pb.update(perc);
                sleep(1);
                x.update(pb.show());
            }
        }
    } else { 
        std::cout << "USE AS:" << std::endl;
        std::cout << "./prog [arg 1] [arg 2]" << std::endl;
        std::cout << "arg 1 - goal of this programm: either \"progress\" for " <<
                    "random progress bar or \"timer\" for timer." << std::endl;
        std::cout << "arg 2 is optional and is the time in seconds for timer or length of progress bar in symbols" << std::endl;
    }
    exit(0);
}
