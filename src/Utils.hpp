//
// Created by Henry Roeland on 09/08/2022.
//

#ifndef ARTIBIN_UTILS_HPP
#define ARTIBIN_UTILS_HPP

#include "oatpp/core/concurrency/SpinLock.hpp"
#include "oatpp/core/Types.hpp"

#include <unordered_map>
#include <string>

class StaticFilesManager {
private:
    oatpp::String m_basePath;
    oatpp::concurrency::SpinLock m_lock;
    std::unordered_map<oatpp::String, oatpp::String> m_cache;
private:
    oatpp::String getExtension(const oatpp::String& filename);
public:

    StaticFilesManager(const oatpp::String& basePath)
            : m_basePath(basePath)
    {}

    oatpp::String getFile(const oatpp::String& path);
    oatpp::String guessMimeType(const oatpp::String& filename);

};

oatpp::String formatText(const char* text, ...);
v_int64 getMillisTickCount();

std::string GetEnv( const std::string & var );

#endif //ARTIBIN_UTILS_HPP
