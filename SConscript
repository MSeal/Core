import os
import fnmatch

Import('env', 'project', 'mode', 'build', 'cflags')
local_env = env.Clone()

lib_target = 'core'

lib_includes = ['include', os.environ['BOOST_INCLUDE_PATH']]
if project == 'test':
    lib_includes.append('testing')

lib_sources = []
for root, dirnames, filenames in os.walk('src'):
    for filename in fnmatch.filter(filenames, '*.cpp'):
        lib_sources.append(os.path.join(root, filename))
# In case we're in a subdirectory -- check parent src
for root, dirnames, filenames in os.walk(os.path.join('..', 'src')):
    for filename in fnmatch.filter(filenames, '*.cpp'):
        lib_sources.append(os.path.join(root, filename))
if project == 'test':
    lib_sources.append(os.path.join('testing', 'full_test.cpp'))

req_libs = ['boost_thread', 'boost_system', 'boost_chrono', 'boost_locale', 
            'boost_unit_test_framework']
lib_modes = ['mt']
if mode == 'debug':
    lib_modes.append('d')

local_env.Append(LIBPATH=os.environ['BOOST_LIB_PATH'])
local_env.Append(LIBS=['-'.join([lib] + lib_modes) for lib in req_libs])
local_env.Append(CPPPATH=lib_includes)
local_env.Append(CPPDEFINES=['BOOST_THREAD_NO_LIB'])
if mode == 'debug':
    local_env.Append(CPPDEFINES=['CORE_DEBUG'])
local_env.Append(CCFLAGS=cflags)

if project == 'test':
    local_env.Program(target=lib_target, source=lib_sources)
elif build == 'shared':
    local_env.SharedLibrary(target=lib_target, source=lib_sources)
else:
    local_env.StaticLibrary(target=lib_target, source=lib_sources)
