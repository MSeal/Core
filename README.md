# Core
An application framework for C++ build on Boost.

## Description
TODO

## Dependencies
* Boost (>= 1.50)

### Dependency Installation
####Libraries:
* boost_thread
* boost_system (required by thread)
* boost_chrono (required by thread)
* boost_unit_test_framework (for test exe's)
####Preprocessor:
* BOOST_THREAD_NO_LIB (for static thread library inclusion)
* CORE_DEBUG (to activate debug mode)
####CDT Console Output:
* Add mingw/bin and mingw/msys/bin directories to PATH environment variable
### Fixes
####Boost Build:
* Fix MinGW directory names in build.bat to true locations
* Change `set toolset=msvc` inf bootstrap.bat to `set toolset=gcc`  
	bootstrap  
	b2 --threading=multi --with-thread --build-type=complete --link=static -j 4 stage  
####Windows Thread Fixes:
Ticket #4878  
boost/thread/detail/config.hpp  
For MinGW (tested on gcc 4.6 with boost 1.50):  
	line 153: #   if defined(BOOST_THREAD_PLATFORM_WIN32)  
	line 154: #       if defined(BOOST_MSVC) || defined(BOOST_INTEL_WIN)  
should be followed by:  
	|| defined(__MINGW32__) || defined(MINGW32) || defined(BOOST_MINGW32) // fix for mingw  
		
## Repository Hierarchy
### include
All header hpp files.
* container
Custom containers and wrappers on existing containers. These do NOT include threadsafe 
containers (look in 'threading/container').
* detail
Hidden headers used for internal hacks/ugly configurations that the average user should
never need to see.
* threading
All thread related objects and functions. These include locks, threads, trackers, managers, 
etc...
	*container
	Containers which are threadsafe or thread aware.
### src
All source cpp files. The subdirectory hierarchy follows the include folder hierarchy.
### testing
All unit tests for the repo.

## Language Preferences
* Camel Case
	* When directly using boost, oftentimes revert to underscore style without renaming
	boost variables. Commonly used boost abstractions sometimes have Core typedef renames.
* Object Oriented (with a few exceptions)

## TODO
* Add full wchar support for logger/toString

## Author
Author(s): Matthew Seal
Contributor(s): Caitlin Seal

## License
Pending!

#### (C) Copyright 2012, [Matthew Seal](http://???)