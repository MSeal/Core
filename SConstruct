import os

for req_env in ['BOOST_INCLUDE_PATH', 'BOOST_LIB_PATH']:
    if req_env not in os.environ:
        raise EnvironmentError("Environment variable '%s' missing" % req_env)

expected_modes = ['debug', 'release']
mode = ARGUMENTS.get('mode', 'release')
if not (mode in expected_modes):
    raise EnvironmentError("Unexpected mode '%s' found -- use one of %s" % (mode, expected_modes))
project = ARGUMENTS.get('project', 'core')
build = ARGUMENTS.get('build', 'static')
mode = ARGUMENTS.get('mode', 'release')
compiler = ARGUMENTS.get('compiler', 'mingw')
variant_dir = mode.title() + build.title() if project == 'core' else mode.title() + 'Test'
src_dir = 'testing' if project == 'test' else 'src'

env = Environment(tools=[compiler])
if mode == 'debug':
    cflags = ['-g','-O2', '-Wall', '-c', '-fmessage-length=0']
else:
    cflags = ['-g','-O3', '-c', '-fmessage-length=0']
Export('env', 'mode', 'project', 'build', 'cflags')

# Put all .sconsign files in one place
env.SConsignFile()

SConscript('SConscript', variant_dir=variant_dir, src=src_dir, duplicate=0)
Clean('.', variant_dir)
