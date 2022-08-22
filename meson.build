project(
  'tasker',
  'cpp',
  default_options : ['cpp_std=c++20']
)

run_command('git', 'fetch', '--tags', '--all', check : true)

r = run_command('bash', '-c', 'git describe || echo "0.0.0"', check : true)
output = r.stdout().strip()

conf = configuration_data()
conf.set('version', output)

inc = include_directories('src')
subdir('src')
