
configure_file(
  input : 'config.hpp.in',
  output : 'config.hpp',
  configuration : conf
)

tasker_sources = [
  'projects/project.cpp',
  'tasks/task.cpp',
  'users/user.cpp',
  'config/keybinds.cpp',
  'config/config.cpp',
  'env.cpp',
  'utility.cpp',
  'logging.cpp',
]

build_tests = get_option('build_tests')

if build_tests
  tasker_test_lib = static_library(
    'tasker_test',
    [
      tasker_sources,
      'testing.cpp',
    ],
    cpp_args : '-DTESTING',
    include_directories : [inc, fmt_inc],
  )
  tasker_test_dep = declare_dependency(
    dependencies : [boost_po_dep, fmt_dep],
    link_with : [tasker_test_lib],
    compile_args : ['-DTESTING'],
    include_directories : [inc, fmt_inc],
  )

  stub_lib = static_library(
    'stubs-static',
    'stubs/ncurses.cpp',
    include_directories : inc
  )
  stub_dep = declare_dependency(
    include_directories : inc,
    link_with : [stub_lib],
    compile_args : '-DTESTING',
  )

  subdir('tests')
endif

build_bin = get_option('build_bin')

if build_bin
  tasker_lib = static_library(
    'tasker',
    tasker_sources,
    include_directories : [inc, fmt_inc],
  )

  tasker_dep = declare_dependency(
    dependencies : [boost_po_dep, fmt_dep],
    link_with : [tasker_lib],
    include_directories : [inc, fmt_inc],
  )

  ncurses_lib = static_library(
    'ncurses-static',
    'ext/ncurses.cpp',
    dependencies : ncurses_dep,
    include_directories : inc
  )

  tasker = executable(
    'tasker',
    'main.cpp',
    install : true,
    dependencies : [tasker_dep, fmt_dep],
    link_with : [ncurses_lib],
  )
endif
