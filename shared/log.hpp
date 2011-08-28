
#include <sstream>



///When a panic situation is detected this handler will be called.
typedef void (*LogHandler)(const std::string& s);

#define TRACE(s) _MSG("trace: ",s)
#define WARN(s)  _MSG("WARN:  ",s)
#define FIXME(s) _MSG("fixme: ",s)
#define ERROR(s) _MSG("ERROR: ",s)

#define _MSG(t,s) {\
        stringstream ss;ss <<t << __FUNCTION__ << "(" <<__LINE__<<") in " <<__FILE__<< " "  << s << endl;\
        _log_message(ss.str());\
    }

void setLogHandler(LogHandler logHandler);

void _log_message(const std::string& s);
