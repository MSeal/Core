import os

expected_modes = ['debug', 'release']
mode = ARGUMENTS.get('mode', 'release')
if not (mode in expected_modes):
    raise EnvironmentError("Unexpected mode '%s' found -- use one of %s" % (mode, expected_modes))

for req_env in ['BOOST_INCLUDE_PATH', 'BOOST_LIB_PATH']:
    if req_env not in os.environ:
        raise EnvironmentError("Environment variable '%s' missing" % req_env)

env = Environment(tools=['mingw'])
if mode == 'debug':
    cflags = ['-g','-O2', '-Wall', '-c', '-fmessage-length=0']
else:
    cflags = ['-g','-O3', '-Wall', '-c', '-fmessage-length=0']
Export('env', 'mode', 'cflags')

SConscript('SConscript', variant_dir=mode, src='src', duplicate=0)
Clean('.', mode)
