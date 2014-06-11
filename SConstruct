import os

for req_env in ['BOOST_INCLUDE_PATH', 'BOOST_LIB_PATH']:
    if req_env not in os.environ:
        raise EnvironmentError("Environment variable '%s' missing" % req_env)

expected_modes = ['debug', 'release']
mode = ARGUMENTS.get('mode', 'release')
if not (mode in expected_modes):
    raise EnvironmentError("Unexpected mode '%s' found -- use one of %s" % (mode, expected_modes))
build = ARGUMENTS.get('build', 'static')
mode = ARGUMENTS.get('mode', 'release')
compiler = ARGUMENTS.get('compiler', 'default')
boost_version = ARGUMENTS.get('boost-ver', '1_54').replace('.', '_')
boost_compiler = ARGUMENTS.get('boost-comp', '')


VS_CHECKS = ['msvc', 'mslink', 'vs', 'vc']
GCC_CHECKS = ['gnu', 'gcc', 'g++', 'mingw']
# Determine which compiler is defaulted
if compiler == 'default':
    check = Environment()
    for tool in check._dict['TOOLS']:
        if tool in VS_CHECKS:
            compiler = tool
            break
        if tool in GCC_CHECKS:
            compiler = tool
            break
# TODO match version numbers as well
if compiler in VS_CHECKS:
    compiler = 'msvc'
    tools = ['mslink', 'msvc', 'mslib']
    if not boost_compiler:
        boost_compiler = 'vc110' # An example default
elif compiler in GCC_CHECKS:
    tools = [compiler]
    compiler = 'gnu'
    if not boost_compiler:
        boost_compiler = 'mgw46' # An example default
else:
    tools = [compiler]

variant_dir = compiler + (mode.title() + 'Test' if build == 'test' else mode.title() + build.title())
src_dir = 'testing' if build == 'test' else 'src'

env = Environment(tools=tools)

cflags = []
if compiler == 'gnu':
    cflags.extend(['-c', '-fmessage-length=0'])
    if mode == 'debug':
        cflags.append('-g')
if compiler == 'msvc':
    cflags.extend(['/EHsc',
        '/wd4503', '/wd4820', '/wd4512', '/wd4625', '/wd4626', '/wd4619', '/wd4668', '/wd4435'])

if mode == 'debug':
    cflags.extend(['-Wall'])
else:
    cflags.extend(['-Ox' if compiler == 'msvc' else '-O3'])

Export('env', 'mode', 'build', 'cflags', 'boost_version', 'boost_compiler')

# Put all .sconsign files in one place
env.SConsignFile()

SConscript('SConscript', variant_dir=variant_dir, src=src_dir, duplicate=0)
Clean('.', variant_dir)
