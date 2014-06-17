import os
import sys
import fnmatch

def is_windows():
    return sys.platform in ['win32', 'cygwin']
def is_mac():
    return sys.platform == 'darwin'
def is_linux():
    return sys.platform.startswith('linux')

for req_env in ['BOOST_INCLUDE_PATH', 'BOOST_LIB_PATH']:
    if req_env not in os.environ:
        if os.name == 'nt':
            raise EnvironmentError("Environment variable '%s' missing" % req_env)
        # Can't use pkg-config -- it's not implemented in bjam yet
        elif req_env == 'BOOST_INCLUDE_PATH':
            os.environ[req_env] = '/usr/include/boost'
        elif req_env == 'BOOST_LIB_PATH':
            if sys.maxsize > 2**32: # 64 bit
                os.environ[req_env] = '/usr/lib/x86_64-linux-gnu'
            else:
                os.environ[req_env] = '/usr/lib'

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
CLANG_CHECKS = ['clang', 'clang++']
# Determine which compiler is defaulted
if compiler == 'default':
    check = Environment()
    if is_windows():
        if 'mingw' in check._dict['TOOLS']:
            compiler = 'mingw'
        for tool in VS_CHECKS:
            if tool in check._dict['TOOLS']:
                compiler = 'msvc'
    elif is_mac():
        for tool in CLANG_CHECKS:
            if tool in check._dict['TOOLS']:
                compiler = 'clang'

    if compiler == 'default':
        for tool in GCC_CHECKS:
            if tool in check._dict['TOOLS']:
                compiler = 'gcc'

if compiler in VS_CHECKS:
    compiler = 'msvc'
    tools = ['mslink', 'msvc', 'mslib']
elif compiler == 'mingw':
    tools = [compiler]
    compiler = 'gcc'
elif compiler in GCC_CHECKS:
    tools = ['g++', 'gnulink']
    compiler = 'gcc'
else:
    tools = [compiler]

if compiler in ['msvc', 'gcc']:
    env = Environment(tools=tools)
elif compiler in ['clang', 'clang++']:
    env = Environment()
    env.static_libraries = 0
    env.static_bpl = 0
    cc = "clang"
    cxx = "clang++"
    env.Replace(
      CC=cc,
      SHCC=cc,
      CXX=cxx,
      LINK=cxx,
      SHCXX=cxx,
      SHLINK=cxx)
    env.c_link = cc
else:
    raise ValueError('Cannot setup environment for {} compiler'.format(compiler))

# Flags (see https://svn.boost.org/trac/boost/wiki/Guidelines/WarningsGuidelines) for good guidelines
cflags = []
if compiler == 'msvc':
    cflags.extend(['/EHsc', '/DNOMINMAX'])
    # Warning that should be treated as errors
    cflags.extend([
        '/we4288', # For-loop scoping
        '/we4238', # Don't take address of temporaries
        '/we4239', # Don't bind temporaries to non-const references (Stephan's "Evil Extension")
        '/we4346', # Require "typename" where the standard requires it.
        '/we4146'  # Unary minus operator applied to unsigned type, result still unsigned
    ])
    # Warning that should be ignored
    cflags.extend([
        '/wd4127', # Conditional expression is constant
        '/wd4324', # Structure was padded due to declspec(align())
        '/wd4503', # Decorated name length exceeded
        '/wd4725', # Inline assembly instruction that may not produce accurate results on some Pentium microprocessors
        '/wd4996'  # 'putenv': The POSIX name for this item is deprecated (but NOT by POSIX!)
    ])
    if mode == 'debug':
        cflags.append('/MTd')
    else:
        cflags.append('/MT')
    
if compiler == 'gcc' or compiler == 'clang':
    cflags.append('-fmessage-length=0')
    if mode == 'debug':
        cflags.append('-g')

if mode == 'debug':
    if compiler == 'msvc':
        cflags.extend(['/W4'])
    elif compiler == 'clang':
        cflags.extend(['-Wall', '-Wextra', '-Wpedantic', '-Wno-long-long'])
    elif compiler == 'gcc':
        cflags.extend(['-Wall', '-Wextra', '-pedantic', '-Wno-long-long'])
    else:
        cflags.extend(['-Wall'])
else:
    cflags.extend(['-Ox' if compiler == 'msvc' else '-O3'])

# Includes
static_link_libs = set(['boost_thread', 'boost_system', 'boost_unit_test_framework'])
no_link_libs = set(['boost_locale'])
non_boost_libs = set()
req_libs = static_link_libs | no_link_libs
if is_windows():
    # Windows is special and has different linking dependencies
    static_link_libs.add('boost_locale')
    no_link_libs.remove('boost_locale')
else:
    # We need pthread explcitly included because we're using static import
    non_boost_libs.add('pthread')

libs = list(non_boost_libs)
for lib in req_libs:
    # Build up the flags to append to the lib name
    lib_modes = ['mt']
    if mode == 'debug':
        if lib in static_link_libs:
            if compiler == 'msvc':
                lib_modes.append('sgd')
            else:
                lib_modes.append('sd')
        else:
            lib_modes.append('d')
    else:
        if lib in static_link_libs:
            lib_modes.append('s')

    if compiler == 'msvc':
        lib = 'lib' + lib
    libs.append('-'.join([lib] + lib_modes))

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
