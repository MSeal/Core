# Core
An application framework for C++ build on Boost.

## Description
Core is a heavily tested application framework designed to give users a wide range of
built in usable platform independent application components. These components include:

* Logging
	* Allows multiple loggers to be active at same time
	* Logs selectable by name
	* Implement single function of class to add new processing of log events
* Threading Management
	* What threads are active
	* Thread naming
	* Polite quit requests
* Threadsafe objects/containers
	* Flexible performance to match various use cases
	* Handles the easy to forget threading mistakes
	* Leaves complete control over event ordering/triggering
	* Abstracts containers/collections to lockable objects
	* Any object can be wrapped by lockwrap template
	* Resource locks maintaining lock ordering for arbitrary sets of locks
* Exceptions
	* Automatic line/function/cause exception generation macros
	* Treatable as boost exceptions or std exceptions
	* String message convertable
	* Coded by exception type/family
* Factories
	* Object generating factory templates
	* Various build options/constructor wrappers
	* Trackable object generation
* Enumeration objects
	* Extendible enumerations that can are bi-mapped to other objects
	* Easy string/value representation of enumerated values
* Loops
	* Many style of for loops for convenience of code minimization
	* Python style enumeration loops
	* Callback loops
* Strings
	* Conversion functionality for all common types to/from strings
	* Efficient stream capabilities for conversion to string
	* UTF-8, UTF-16, and UTF-32 aware string conversion capabilities
* Utilities
	* Various other minor functions

## Dependencies
* C++03 Compiler (Need to test C++98 functionality)
* Boost (>= 1.50)
* Compiler requirements:
	* class template partial specialization
	* function type parsing
	* SFINAE
	
### Known Compliant Compilers
* GCC 4.6

### Known Non-Compliant Compilers
* Visual C++ 5.0 or lower

### Dependency Installation
See mingwInstallNotes.htm for details about setting mingw up on Windows environments with
Eclipse.

#### Libraries:
With -mt extension to name
* boost_thread
* boost_system (required by thread)
* boost_chrono (required by thread)
* boost_locale
* boost_unit_test_framework (for test exe's)

#### Preprocessor:
* BOOST_THREAD_NO_LIB (for static thread library inclusion)
* CORE_DEBUG (to activate debug mode)

#### CDT Console Output:
* Add mingw/bin and mingw/msys/bin directories to PATH environment variable

### Fixes
#### Boost Build:
* Fix MinGW directory names in build.bat to true locations
* Change `set toolset=msvc` inf bootstrap.bat to `set toolset=gcc`  
	bootstrap  
	b2 --threading=multi --with-thread --with-locale --with-test --build-type=complete --link=static -j 4 stage  
#### Windows Thread Fixes (fixed in 1.54!):
* Ticket #4878  
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
	* container  
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
* Change function overload approach for pptypes to not have ambiguity warning
* Add comments to pptypes and stringutil
* Fix toString(typeid)
* Add utf iterators which step by character instead of by byte
* Change tracked factory to store shared pointers to objects
* Change file location macro to strip down to core relative directories

## Author
Author(s): Matthew Seal
Contributor(s): Caitlin Seal

## License
Pending!

#### (C) Copyright 2012, [Matthew Seal](http://???)