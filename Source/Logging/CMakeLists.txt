cmake_minimum_required(VERSION 4.0)

set(Logging_LOCAL_SOURCES
        Logging.cpp
)

set(Logging_PUBLIC_HEADERS
        Logging.h
)

add_library(Logging
        ${Logging_LOCAL_SOURCES}
)