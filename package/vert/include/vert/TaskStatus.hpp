#pragma once

namespace vert{
enum class TaskStatus : unsigned char {
  pending = 1,
  running = 2,
  finish = 3,
  done = 3,
  fail = 4
};
};

