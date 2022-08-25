gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')

test_deps = [gtest_dep, gtest_main_dep, stub_dep, gmock_dep]
test_mock_deps = test_deps + [gmock_dep]

main_test = executable(
  'main.test',
  'main.test.cpp',
  dependencies : test_deps,
  link_with : [tasker_lib],
  include_directories : inc,
)
test('main test', main_test)

project_test = executable(
  'project.test',
  'project.test.cpp',
  dependencies : test_deps,
  link_with : [tasker_lib],
  include_directories : inc,
)
test('project test', project_test)

task_test = executable(
  'task.test',
  'task.test.cpp',
  dependencies : test_deps,
  link_with : [tasker_lib],
  include_directories : inc,
)
test('task test', task_test)

user_test = executable(
  'user.test',
  'user.test.cpp',
  dependencies : test_deps,
  link_with : [tasker_lib],
  include_directories : inc,
)
test('user test', user_test)

ncurses_test = executable(
  'ncurses.test',
  'ncurses.test.cpp',
  dependencies : test_deps,
  link_with : [tasker_lib],
  include_directories : inc,
)
test('ncurses test', ncurses_test)

tui_test = executable(
  'tui.test',
  'tui.test.cpp',
  dependencies : test_deps,
  link_with : [tasker_lib],
  include_directories : inc,
)
test('tui test', tui_test)

window_test = executable(
  'window.test',
  'window.test.cpp',
  dependencies : test_deps,
  link_with : [tasker_lib],
  include_directories : inc,
)
test('window test', window_test)
