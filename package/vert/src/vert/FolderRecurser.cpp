
#include <vert/FolderRecurser.hpp>

using namespace vert::FolderRecurser;

bool vert::FolderRecurser::foreach_pcall(
    std::filesystem::path const &t_dir,
    std::function<bool(std::filesystem::path const &)> callback) {
  std::filesystem::directory_iterator path_foreach(t_dir);
  for (auto &path_forentry : path_foreach) {
    std::filesystem::path path_child = path_forentry.path();
    if (callback(path_child)) {
      return true;
    }
    if (std::filesystem::is_directory(path_child)) {
      if (vert::FolderRecurser::foreach_pcall(path_child, callback)) {
        return true;
      }
    }
  }
  return false;
};

bool vert::FolderRecurser::foreach_popen(
    std::filesystem::path const &t_dir,
    std::function<bool(std::filesystem::path const &)> callback) {
  std::filesystem::directory_iterator path_foreach(t_dir);
  for (auto &path_forentry : path_foreach) {
    std::filesystem::path path_child = path_forentry.path();
    if (std::filesystem::is_directory(path_child)) {
      if (vert::FolderRecurser::foreach_popen(path_child, callback)) {
        return true;
      }
    }
    if (callback(path_child)) {
      return true;
    }
  }
  return false;
};
