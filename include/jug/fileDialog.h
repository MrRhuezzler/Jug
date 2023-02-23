#pragma once

#include <string>

namespace Jug
{
    class FileDialog
    {
    public:
        static std::string openFile(const char *filter);
        static std::string saveFile(const char *filter);
    };

} // namespace Jug
