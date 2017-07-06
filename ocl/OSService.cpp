#include <rtt/os/fosi.h>
#include <rtt/Service.hpp>
#include <rtt/plugin/ServicePlugin.hpp>
#include <cstdlib>

#ifndef WIN32
#include <sys/wait.h>
#endif

// setEnvString and isEnv needed to be implemented because we
// can't provide a getenv() implementation on windows without leaking
// memory.
#ifdef WIN32
std::string getEnvString(const char *name)
{
    char c[32767];
    if (GetEnvironmentVariable(name, c, sizeof (c)) > 0) {
        return string(c);
    }
    return 0;
}

bool isEnv(const char* name)
{
    char c;
    // returns zero if not found and non-zero for needed buffer length.
    if (GetEnvironmentVariable(name, &c, sizeof (c)) > 0) return true;
    return false;
}

int setenv(const char *name, const char *value, int overwrite)
{
  if (overwrite == 0)
  {
    char c;
    if (GetEnvironmentVariable(name, &c, sizeof (c)) > 0) return 0;
  }

  if (SetEnvironmentVariable(name, value) != 0) return 0;
  return -1;
}
#else
std::string getEnvString(const char *name)
{
    std::string ret;
    char* c = getenv(name);
    if (c)
        return std::string(c);
    return ret;
}
bool isEnv(const char* name)
{
    if( getenv(name) ) return true;
    return false;
}
#endif

#include <rtt/os/startstop.h>

namespace OCL
{

    /**
     * A service that provides access to some useful Operating System functions.
     * Can be loaded in scripts by writing 'requires("os")' on top of the file.
     */
    class OSService: public RTT::Service
    {
    public:
        OSService(RTT::TaskContext* parent) :
            RTT::Service("os", parent)
        {
            doc("A service that provides access to some useful Operating System functions.");
            // add the operations
            addOperation("getenv", &OSService::getenv, this).doc(
                    "Returns the value of an environment variable. If it is not set, returns the empty string.").arg("name",
                    "The name of the environment variable to read.");
            addOperation("isenv", &OSService::isenv, this).doc(
                    "Checks if an environment variable exists").arg("name",
                            "The name of the environment variable to check.");
            addOperation("setenv", &OSService::setenv, this).doc(
                    "Sets an environment variable.").arg("name",
                            "The name of the environment variable to write.").arg("value","The text to set.");
            addOperation("argc", &OSService::argc, this).doc("Returns the number of arguments, given to this application.");
            addOperation("argv", &OSService::argv, this).doc("Returns the arguments as a sequence of strings, given to this application.");
            addOperation("sleep", &OSService::sleep, this).doc("Suspend execution of the calling thread")
                    .arg("seconds", "Sleep for x seconds");
            addOperation("usleep", &OSService::usleep, this).doc("Suspend execution of the calling thread")
                    .arg("microseconds", "Sleep for x microseconds");
            addOperation("nanosleep", &OSService::nanosleep, this).doc("Suspend execution of the calling thread")
                    .arg("seconds", "Sleep for x seconds")
                    .arg("nanoseconds", "Sleep for x nanoseconds");
            addOperation("execute", &OSService::execute, this).doc("Execute a shell command");
        }

        int argc(void)
        {
            return __os_main_argc();
        }

        std::vector<std::string> argv(void)
        {
            int a = argc();
            char** args = __os_main_argv();
            std::vector<std::string> ret(a);
            for (int i = 0; i != a; ++i)
                ret[i] = std::string(args[i]);
            return ret;
        }

        std::string getenv(const std::string& arg)
        {
            return getEnvString(arg.c_str());
        }

        bool isenv(const std::string& arg)
        {
            return isEnv(arg.c_str());
        }

        bool setenv(const std::string& arg, const std::string& value)
        {
            return ::setenv(arg.c_str(), value.c_str(), 1) == 0;
        }

        int sleep(unsigned int seconds)
        {
            TIME_SPEC rqtp, rmtp;
            rqtp.tv_sec = seconds;
            rqtp.tv_nsec = 0;
            return rtos_nanosleep(&rqtp, &rmtp);
        }

        int usleep(unsigned int microseconds)
        {
            TIME_SPEC rqtp, rmtp;
            rqtp.tv_sec  = (microseconds / 1000000u);
            rqtp.tv_nsec = (microseconds % 1000000u) * 1000u;
            return rtos_nanosleep(&rqtp, &rmtp);
        }

        int nanosleep(unsigned int seconds, unsigned int nanoseconds)
        {
            TIME_SPEC rqtp, rmtp;
            rqtp.tv_sec = seconds;
            rqtp.tv_nsec = nanoseconds;
            return rtos_nanosleep(&rqtp, &rmtp);
        }

        int execute(const std::string& command)
        {
#ifdef WIN32
            return system(command.c_str());
#else
            int status = system(command.c_str());
            if (status < 0) {
                return status;
            } else {
                if (!WIFEXITED(status)) {
                    return -1;
                } else {
                    return WEXITSTATUS(status);
                }
            }
#endif
        }
    };
}

ORO_SERVICE_NAMED_PLUGIN( OCL::OSService, "os")

