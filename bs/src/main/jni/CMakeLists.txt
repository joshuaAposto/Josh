cmake_minimum_required(VERSION 3.10)
project(YourMod)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}
)

# ✅ TAMBAHKAN: Find cURL library
find_library(curl-lib curl)

# Add library
add_library(YourMod SHARED
    main.cpp
    DomainScanner.cpp
    NetworkHooks.cpp
    FireWall.cpp
    draw.cpp
    AntiDetection.cpp
    AntiTelemetry.cpp 
    # ... other existing files
)

# Set output name
set_target_properties(YourMod PROPERTIES OUTPUT_NAME "Encrypt")

# ✅ LINK LIBRARIES (DENGAN cURL)
target_link_libraries(YourMod
    log
    android
    EGL
    GLESv2
    GLESv3
    ${curl-lib}        # ✅ TAMBAHKAN INI untuk HTTP request
    # ... other libraries jika ada
)
