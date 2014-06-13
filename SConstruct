import os
import fnmatch

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

# Compiler
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

if compiler in VS_CHECKS:
    compiler = 'msvc'
    tools = ['mslink', 'msvc', 'mslib']
elif compiler in GCC_CHECKS:
    tools = [compiler]
    compiler = 'gnu'
else:
    tools = [compiler]

env = Environment(tools=tools)

# Flags
cflags = []
if compiler == 'gnu':
    cflags.extend(['-c', '-fmessage-length=0'])
    if mode == 'debug':
        cflags.append('-g')
if compiler == 'msvc':
    cflags.append('/EHsc')
    if mode == 'debug':
        cflags.append('/MTd')
    else:
        cflags.append('/MT')
    cflags.extend(['/wd4503', '/wd4820', '/wd4512', '/wd4625', '/wd4626', '/wd4619', '/wd4668', '/wd4435'])
elif compiler == 'gnu':
    cflags.extend(['-c', '-fmessage-length=0'])
    if mode == 'debug':
        cflags.append('-g')

if mode == 'debug':
    cflags.extend(['-Wall'])
else:
    cflags.extend(['-Ox' if compiler == 'msvc' else '-O3'])

# Includes
req_libs = ['boost_thread', 'boost_system', 'boost_locale', 'boost_unit_test_framework']
if compiler == 'msvc':
    req_libs = ['lib' + req for req in req_libs]

lib_modes = ['mt']
if mode == 'debug':
    if compiler == 'msvc':
        lib_modes.append('sd')
    else:
        lib_modes.append('sgd')
else:
    lib_modes.append('s')

libs = ['-'.join([lib] + lib_modes) for lib in req_libs]

lib_includes = ['include', os.environ['BOOST_INCLUDE_PATH']]
if build == 'test':
    lib_includes.append('testing')

lib_sources = []
for root, dirnames, filenames in os.walk('src'):
    for filename in fnmatch.filter(filenames, '*.cpp'):
        lib_sources.append(os.path.join(root, filename))
# In case we're in a subdirectory -- check parent src
for root, dirnames, filenames in os.walk(os.path.join('..', 'src')):
    root = os.path.join(*root.split(os.sep)[1:])
    for filename in fnmatch.filter(filenames, '*.cpp'):
        lib_sources.append(os.path.join(root, filename))
if build == 'test':
    lib_sources.append(os.path.join('testing', 'full_test.cpp'))

# Build Directories
variant_dir = compiler + (mode.title() + 'Test' if build == 'test' else mode.title() + build.title())
src_dir = 'testing' if build == 'test' else 'src'

Export('env', 'compiler', 'mode', 'build', 'cflags', 'libs', 'lib_includes', 'lib_sources')

# Put all .sconsign files in one place
env.SConsignFile()

SConscript('SConscript', variant_dir=variant_dir, src=src_dir, duplicate=0)
Clean('.', variant_dir)
