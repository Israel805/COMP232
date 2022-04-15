#Minimum allowed version of cmake for this configuration.
cmake_minimum_required(VERSION 3.8)

#Project name is mandatory
project(cilisp)

#Creating an executable
add_executable(cilisp)

#Forcing all compilation to be with C11 Standard
set_property(TARGET cilisp PROPERTY C_STANDARD 11)
set_property(TARGET cilisp PROPERTY C_STANDARD_REQUIRED ON)

#Turning on MANY warnings
target_compile_options(cilisp PRIVATE -Wall)

#Allow folders/files in ./scr to be includable to all .c/.h files
target_include_directories(cilisp PRIVATE src)

#Allow bison-flex-output directory to be #include-able to all .c/.h files
target_include_directories(cilisp PRIVATE src/bison-flex-output)

#include Flex and Bison in this project
find_package(FLEX)
find_package(BISON)

#Create a Flex target and a Bison target that output .c files
#These targets (unlike cilisp) don't create executables nor libraries
FLEX_TARGET(lexer src/cilisp.l ${CMAKE_SOURCE_DIR}/src/bison-flex-output/lexer.c COMPILE_FLAGS)
BISON_TARGET(parser src/cilisp.y ${CMAKE_SOURCE_DIR}/src/bison-flex-output/parser.c VERBOSE COMPILE_FLAGS)

#Make the bison target also generate a .h file
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

#Add all the source files to cilisp target
target_sources(cilisp PRIVATE src/cilisp.c)
target_sources(cilisp PRIVATE ${FLEX_lexer_OUTPUTS})
target_sources(cilisp PRIVATE ${BISON_parser_OUTPUTS})

#Link the math library to cilisp because math.h needs it :/
target_link_libraries(cilisp m)