# Copied almost verbatim from https://github.com/llvm/llvm-project/blob/main/clang/examples/PrintFunctionNames/CMakeLists.txt


cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(no_underscore VERSION 1.0.0 LANGUAGES CXX)

find_package(LLVM REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

if( NOT MSVC )
  if( NOT LLVM_REQUIRES_RTTI )
    if( NOT LLVM_REQUIRES_EH )
		#      set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/PrintFunctionNames.exports)
    endif()
  endif()
endif()

add_llvm_library(NoUnderscore MODULE NoUnderscore.cc PLUGIN_TOOL clang)

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS
    Support
  )
  clang_target_link_libraries(NoUnderscore PRIVATE
    clangAST
    clangBasic
    clangFrontend
    )
endif()
