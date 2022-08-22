project(
  'tasker',
  'cpp',
  default_options : ['cpp_std=c++20']
)

r = run_command('bash', '-c', 'git describe || echo "0.0.0"', check : true)
output = r.stdout().strip()
errortxt = r.stderr().strip()

conf = configuration_data()
conf.set('version', output)

inc = include_directories('src')
subdir('src')
