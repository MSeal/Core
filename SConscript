import os

Import('env', 'compiler', 'mode', 'build', 'cflags', 'libs', 'lib_includes', 'lib_sources')
local_env = env.Clone()

lib_target = 'core'

local_env.Append(LIBPATH=os.environ['BOOST_LIB_PATH'])
local_env.Append(LIBS=libs)
local_env.Append(CPPPATH=lib_includes)
local_env.Append(CPPDEFINES=['BOOST_ALL_NO_LIB'])
if mode == 'debug':
    local_env.Append(CPPDEFINES=['CORE_DEBUG'])
local_env.Append(CCFLAGS=cflags)

if build == 'test':
    if compiler == 'msvc':
        # Can't use .Append for this one... not sure why
        local_env["LINKFLAGS"] = ["/SUBSYSTEM:CONSOLE"]
    local_env.Program(target=lib_target, source=lib_sources)
elif build == 'shared':
    local_env.SharedLibrary(target=lib_target, source=lib_sources)
else:
    local_env.StaticLibrary(target=lib_target, source=lib_sources)
