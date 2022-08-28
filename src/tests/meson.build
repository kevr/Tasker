gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')

test_deps = [
  tasker_test_dep,
  gtest_dep,
  gtest_main_dep,
  stub_dep,
  gmock_dep,
  fmt_dep
]

main_test = executable(
  'main.test',
  'main.test.cpp',
  dependencies : test_deps,
)
test('main test', main_test)

project_test = executable(
  'project.test',
  'project.test.cpp',
  dependencies : test_deps,
)
test('project test', project_test)

task_test = executable(
  'task.test',
  'task.test.cpp',
  dependencies : test_deps,
)
test('task test', task_test)

user_test = executable(
  'user.test',
  'user.test.cpp',
  dependencies : test_deps,
)
test('user test', user_test)

ncurses_test = executable(
  'ncurses.test',
  'ncurses.test.cpp',
  dependencies : test_deps,
)
test('ncurses test', ncurses_test)

tui_test = executable(
  'tui.test',
  'tui.test.cpp',
  dependencies : test_deps,
)
test('tui test', tui_test)

window_test = executable(
  'window.test',
  'window.test.cpp',
  dependencies : test_deps,
)
test('window test', window_test)

config_test = executable(
  'config.test',
  'config.test.cpp',
  dependencies : test_deps,
)
test('config test', config_test)

utility_test = executable(
  'utility.test',
  'utility.test.cpp',
  dependencies : test_deps,
)
test('utility test', utility_test)

env_test = executable(
  'env.test',
  'env.test.cpp',
  dependencies : test_deps,
)
test('env test', env_test)

logging_test = executable(
  'logging.test',
  'logging.test.cpp',
  dependencies : test_deps,
)
test('logging test', logging_test)
