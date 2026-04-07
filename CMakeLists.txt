cmake_minimum_required(VERSION 3.23)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# =========================
# 🔵 QUERY ENGINE (app.exe)
# =========================
add_executable(app
    src/main.cpp
    src/query.cpp
    src/heapOptmization.cpp
    src/deserialization.cpp
    src/tokenizer.cpp
    src/frequency.cpp
    src/sqlite3.c   # 🔥 ADD THIS
)

target_include_directories(app PRIVATE include)

# =========================
# 🔴 INDEX BUILDER (processDB.exe)
# =========================
add_executable(processDB
    src/processdb.cpp
    src/tokenizer.cpp
    src/frequency.cpp
    src/serialization.cpp
    src/deserialization.cpp
    src/sqlite3.c
)

target_include_directories(processDB PRIVATE include)