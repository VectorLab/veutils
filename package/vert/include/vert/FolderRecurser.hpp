#pragma once

#include <filesystem>
#include <functional>

namespace vert{
namespace FolderRecurser{
bool foreach_pcall(std::filesystem::path const&, std::function<bool(std::filesystem::path const&)>);
bool foreach_popen(std::filesystem::path const&, std::function<bool(std::filesystem::path const&)>);
};
};

