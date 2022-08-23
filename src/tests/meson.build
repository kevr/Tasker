gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')

test_deps = [gtest_dep, gtest_main_dep, stub_dep]
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
