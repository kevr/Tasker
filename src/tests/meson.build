gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')

test_deps = [
  tasker_test_dep,
  gtest_dep,
  gtest_main_dep,
  stub_dep,
  fmt_dep
]
mock_deps = test_deps + [gmock_dep]

main_test = executable(
  'main.test',
  'main.test.cpp',
  dependencies : mock_deps,
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
  dependencies : mock_deps,
)
test('ncurses test', ncurses_test)

tui_test = executable(
  'tui.test',
  'tui.test.cpp',
  dependencies : mock_deps,
)
test('tui test', tui_test)

window_test = executable(
  'window.test',
  'window.test.cpp',
  dependencies : mock_deps,
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

pane_test = executable(
  'pane.test',
  'pane.test.cpp',
  dependencies : mock_deps,
)
test('pane test', pane_test)

tui_project_test = executable(
  'tui_project.test',
  'tui_project.test.cpp',
  dependencies : mock_deps,
)
test('tui_project test', tui_project_test)

board_test = executable(
  'board.test',
  'board.test.cpp',
  dependencies : mock_deps,
)
test('board test', board_test)

context_test = executable(
  'context.test',
  'context.test.cpp',
  dependencies : mock_deps,
)
test('context test', context_test)

task_list_test = executable(
  'task_list.test',
  'task_list.test.cpp',
  dependencies : mock_deps,
)
test('task_list test', task_list_test)
