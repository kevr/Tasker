
configure_file(
  input : 'config.hpp.in',
  output : 'config.hpp',
  configuration : conf
)

tasker_lib = static_library(
  'tasker',
  'projects/project.cpp',
  'tasks/task.cpp',
  'users/user.cpp',
  include_directories : inc,
)

tasker = executable(
  'tasker',
  'main.cpp',
  install : true,
  link_with : tasker_lib,
)

subdir('tests')
