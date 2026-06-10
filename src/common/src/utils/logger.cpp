#include <common/diagnostic/diagnostic.h>
#include <common/utils/logger.h>

namespace common::utils
{
    void Logger::set_prefix(std::string_view _prefix) noexcept {
        profile.prefix = _prefix;
    }
    std::string_view Logger::get_prefix() const noexcept {
        return profile.prefix;
    }
    void Logger::remove_prefix() noexcept {
        profile.prefix = "";
    }
    void Logger::set_name(std::string_view _name) noexcept {
				if(!name.empty()) {
						profiles[name] = std::move(profile);
				}
        name = _name;
				if(profiles.contains(name))
						profile = profiles.at(name);
				else profile = LoggerProfile::default_();
    }
    std::string_view Logger::get_name() const noexcept {
        return name;
    }

		void Logger::add_profile(const std::string& _name, LoggerProfile _profile) noexcept {
				if(!_name.empty()) {
						profiles[_name] = _profile;
				}
		}

		void Logger::set_profile(const std::string& _name, LoggerProfile _profile) noexcept {
				if(!_name.empty()) profiles[_name] = _profile;
				
				name = _name;
				profile = _profile;
		}

		void Logger::set_profile(LoggerProfile _profile) noexcept {
				profile = _profile;
		}

		Logger::LoggerProfile Logger::get_profile() const noexcept {
				return profile;
		}

    Logger::LogLevel Logger::get_level() const  noexcept {
        return profile.level;
    }
    void Logger::set_level(LogLevel lvl) noexcept {
        profile.level = lvl;
    }
    void Logger::add_level(LogLevel lvl) noexcept {
        profile.level |= lvl;
    }
    void Logger::sub_level(LogLevel lvl) noexcept {
        profile.level = static_cast<LogLevel>(
            static_cast<uint8_t>(profile.level) & ~static_cast<uint8_t>(lvl));
    }

    diagnostic::InterError Logger::stream_null() const {
        return diagnostic::InterError("logger error: stream is null");
    }
    diagnostic::InterError Logger::stream_bad() const {
        return diagnostic::InterError("logger error: stream is bad");
    }

    void Logger::set_infostream(std::unique_ptr<streams::OutputStream> stream) noexcept {
        profile.infostream = std::move(stream);
    }
    void Logger::set_errstream(std::unique_ptr<streams::OutputStream> stream) noexcept {
        profile.errstream = std::move(stream);
    }

    void Logger::check_infostream() const {
        if(!profile.infostream) throw stream_null();
        if(profile.infostream->bad()) throw stream_bad();
    }
    bool Logger::check_errstream() const {
        return  profile.errstream && !profile.errstream->bad();
    }
    
    void Logger::debug(const std::string& line) {
        if(!(profile.level & Logger::LogLevel::DEBUG)) return;
        check_infostream();
        profile.infostream->write_line("{}{}[{}{}DEBUG] {}", 
						profile.prefix, std::string(profile.prefix.length() > 0,' '), name, std::string(name.length() > 0,' '), line);
    }
    void Logger::log(const std::string& line) {
        if(!(profile.level & Logger::LogLevel::INFO)) return;
        check_infostream();
        profile.infostream->write_line("{}{}[{}{}INFO] {}", profile.prefix, std::string(profile.prefix.length() > 0,' '), name, std::string(name.length() > 0,' '), line);
    }
    void Logger::warn(const std::string& line) {
        if(!(profile.level & Logger::LogLevel::WARN)) return;
        if(check_errstream()) profile.errstream->write_line("{}{}[{}{}WARN] {}", profile.prefix, std::string(profile.prefix.length() > 0,' '), name, std::string(name.length() > 0,' '), line);
        else {  
            check_infostream();
            profile.infostream->write_line("{}{}[{}{}WARN] {}", profile.prefix, std::string(profile.prefix.length() > 0,' '), name, std::string(name.length() > 0,' '), line);
        } 
    }
    void Logger::error(const std::string& line) {
        if(!(profile.level & Logger::LogLevel::ERROR)) return;
        if(check_errstream()) profile.errstream->write_line("{}{}[{}{}ERROR] {}", profile.prefix, std::string(profile.prefix.length() > 0,' '), name, std::string(name.length() > 0,' '), line);
        else {  
            check_infostream();
            profile.infostream->write_line("{}{}[{}{}ERROR] {}", profile.prefix, std::string(profile.prefix.length() > 0,' '), name, std::string(name.length() > 0,' '), line);
        } 
    }
}
