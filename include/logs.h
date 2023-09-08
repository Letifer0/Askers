#pragma once

#include <fstream>
#include <chrono>

namespace logs {
    enum log_type {
        ERROR,
        WARNING,
        NOTHING
    };

    class Logs {
        private:
            std::fstream logs;
            bool log_start;
            
            void start() {
                auto now = std::chrono::system_clock::now();
                std::time_t end_time = std::chrono::system_clock::to_time_t(now);
                char* buffer = new char[200];
                ctime_s(buffer, 200, &end_time);

                logs << "$ => new log | date => " << buffer;
                delete [] buffer;

                log_start = true;
            } 


        public:
            Logs(const char* path) {
                log_start = false;
                
                logs.open(path, std::fstream::app | std::fstream::out);

                if (!logs.is_open()) {
                    return;
                }
            };

            ~Logs() {};

            void end() {
                if (log_start) {
                    logs << "\n$ => end log" << "\n\n";
                }
                logs.close();
            }
            
            void add(log_type type) {};

            template<typename First, typename... Args>
            void add(log_type type, First arg, Args... args) {
                if(!log_start){
                    start();
                }

                switch (type) {
                    case log_type::ERROR:
                        logs << "\n    " << "!! [ERR] --> " << "\n        " << arg;
                        break;
                    case log_type::WARNING:
                        logs << "\n    " << "& [WRNG] --> " << "\n        " << arg;
                        break;
                    default:
                        logs << arg;
                        break;
                }

                add(NOTHING, args...);
            };
    };
};