project(
  'tasker',
  'cpp',
  default_options : ['cpp_std=c++20']
)

fmt_proj = subproject(
  'fmt',
  default_options : ['default_library=static']
)
fmt_inc = fmt_proj.get_variable('fmt_inc')
fmt_dep = fmt_proj.get_variable('fmt_dep')

boost_po_dep = dependency(
  'boost',
  modules : ['program_options', 'system'],
  required : true
)
ncurses_dep = dependency('ncurses', required : true)

conf = configuration_data()
version = get_option('version')
conf.set('version', version)
message('Version = ' + version)

inc = include_directories('src')
subdir('src')
