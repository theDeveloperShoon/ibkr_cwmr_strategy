# CMakeList.txt : CMake project for IBKR_CMAKE_MEAN_REVERSION, include source and define
# project specific logic here.
#
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

cmake_minimum_required (VERSION 3.25)
project (IBKR_CMAKE_MEAN_REVERSION LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Find vcpkg packages
find_package (protobuf CONFIG REQUIRED)
find_package (unofficial-intelrdfpmathlib CONFIG REQUIRED)
find_package (Threads REQUIRED)

set(TWSAPI_DIR "C:/TWS API/source/CppClient/client")

file(GLOB TWS_API_SOURCES 
	"${TWSAPI_DIR}/*.cpp"
	"${TWSAPI_DIR}/protobuf/*.cc"
)

#
add_library(ibkr_twsapi STATIC ${TWS_API_SOURCES})

target_include_directories(ibkr_twsapi SYSTEM PUBLIC ${TWSAPI_DIR} ${TWSAPI_DIR}/protobuf)
target_link_libraries(ibkr_twsapi PUBLIC protobuf::libprotobuf unofficial::intelrdfpmathlib::decimal128 Threads::Threads ws2_32)

if(MSVC)
	target_compile_options(ibkr_twsapi PRIVATE /W3)
else()
	target_compile_options(ibkr_twsapi PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# Add source to this project's executable.
add_executable(IBKR_CMAKE_MEAN_REVERSION
	"IBKR_CMAKE_MEAN_REVERSION.cpp"
	"IBKR_CMAKE_MEAN_REVERSION.h"
#	${TWS_API_SOURCES}
)

target_link_libraries(IBKR_CMAKE_MEAN_REVERSION PRIVATE ibkr_twsapi)

set_target_properties(IBKR_CMAKE_MEAN_REVERSION PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Include Directories and Link Libraries
#target_include_directories(IBKR_CMAKE_MEAN_REVERSION SYSTEM
#	PUBLIC 
#		${TWSAPI_DIR}
#		${TWSAPI_DIR}/protobuf
#)

#target_link_libraries(IBKR_CMAKE_MEAN_REVERSION 
#	PUBLIC 
#		protobuf::libprotobuf 
#		unofficial::intelrdfpmathlib::decimal128
#		Threads::Threads
#		ws2_32
#)

if(MSVC)
	target_compile_options(IBKR_CMAKE_MEAN_REVERSION PRIVATE /W4 /WX)
else()
	target_compile_options(IBKR_CMAKE_MEAN_REVERSION PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# TODO: Add tests and install targets if needed.
