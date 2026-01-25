//
// Created by fightinghawks18 on 1/25/26.
//

#ifndef ARTUS_MESSENGER_H
#define ARTUS_MESSENGER_H

#include <format>
#include <iostream>
#include <string>
#include <vector>

namespace Artus::Core {
    /// @brief Differentiates the importance of a message
    enum class MessageSeverity {
        Low, ///< Message isn't important, probably to inform
        Medium, ///< Message is semi-important, probably to warn
        High, ///< Message is important, probably an error
        Critical, ///< Message is very important, probably a fatal error
    };

    /// @brief A message that was logged
    /// @see Logger
    struct Message {
        std::string message;
        MessageSeverity severity;
    };

    /// @brief Prints messages to the console,
    /// and holds onto it for later access and usage by others
    class Logger {
    public:
        static Logger& GetSingletonInstance() {
            static Logger logger;
            return logger;
        }

        template <typename ...Args>
        void Output(MessageSeverity severity, std::string_view format, Args&&... args) {
            auto message = std::format(format, std::forward<Args>(args)...);
            switch (severity) {
            case MessageSeverity::Low:
                std::cout << message << std::endl;
                break;
            case MessageSeverity::Medium:
            case MessageSeverity::High:
            case MessageSeverity::Critical:
                std::cerr << message << std::endl;
                break;
            }

            mMessages.push_back({message, severity});
        }

        [[nodiscard]] const std::vector<Message>& GetLogs() { return mMessages; }
    private:
        std::vector<Message> mMessages;
    };
}

#define AR_LOG(fmt, ...) Artus::Core::Logger::GetSingletonInstance().Output(Artus::Core::MessageSeverity::Low, fmt, ##__VA_ARGS__)
#define AR_WARN(fmt, ...) Artus::Core::Logger::GetSingletonInstance().Output(Artus::Core::MessageSeverity::Medium, fmt, ##__VA_ARGS__)
#define AR_ERR(fmt, ...) Artus::Core::Logger::GetSingletonInstance().Output(Artus::Core::MessageSeverity::High, fmt, ##__VA_ARGS__)
#define AR_PANIC(fmt, ...) Artus::Core::Logger::GetSingletonInstance().Output(Artus::Core::MessageSeverity::Critical, fmt, ##__VA_ARGS__)

#endif // ARTUS_MESSENGER_H
