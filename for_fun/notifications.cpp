#include "notifications.h"

#if defined(__APPLE__) || defined(__MACH__)
Notification::Notification (const std::string &name, const std::string &message, int timeout) {
    std::string safename(name), safemessage(message);
    std::replace(safename.begin(), safename.end(), '\'', '`');
    std::replace(safemessage.begin(), safemessage.end(), '\'', '`');
    std::string command = "osascript -e 'display notification \"" + safemessage + "\" with title \"Timer\"";
    command += " subtitle \"" + safename + "\" sound name \"Hero\"';";
    std::cout << command << std::endl;
    execlp( "/bin/sh", "sh", "-c", command.c_str(), (char*)0 );
}
#elif defined(__linux__) || defined(__linux) || defined(linux)
Notification::Notification (const std::string &name, const std::string &message, int timeout) {
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
#endif