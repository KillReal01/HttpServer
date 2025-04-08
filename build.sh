#!/bin/bash

# Default target architecture (can be overridden by an argument)
TARGET_ARCH="${1:-arm32}"

# Set the cross-compiler prefix based on the architecture
case "${TARGET_ARCH}" in
    arm32)
        CROSS_COMPILE=arm-linux-gnueabihf-
        ;;
    arm64)
        CROSS_COMPILE=aarch64-linux-gnu-
        ;;
    x86)
        CROSS_COMPILE=""
        ;;
    *)
        printf "%s\n" "Unknown target architecture: ${TARGET_ARCH}. Supported architectures are arm32, arm64, and x86."
        exit 1
        ;;
esac

CMAKE_GENERATOR="Unix Makefiles"

# Project directory
PROJECT_DIR=$(pwd)

# Build directory
BUILD_DIR="build/${TARGET_ARCH}"

# Check if the cross-compiler is installed
if [ "${TARGET_ARCH}" != "x86" ]; then
    if ! command -v ${CROSS_COMPILE}g++ &> /dev/null
    then
        printf "%s\n" "Cross-compiler ${CROSS_COMPILE}g++ not found. Please ensure it is installed."
        exit 1
    fi
else
    # For x86 architecture, ensure g++ is available locally
    if ! command -v g++ &> /dev/null
    then
        printf "%s\n" "g++ not found. Please ensure it is installed."
        exit 1
    fi
fi

# Create the build directory if it doesn't exist
if [ ! -d "${BUILD_DIR}" ]; then
    printf "%s\n" "Creating build directory: ${BUILD_DIR}"
    mkdir -p "${BUILD_DIR}"
fi

# Change to the build directory
cd "${BUILD_DIR}"

# Run CMake to configure the project with the cross-compiler
printf "%s\n" "Running CMake to configure the project for ${TARGET_ARCH}..."
cmake ${PROJECT_DIR} \
    -DCMAKE_C_COMPILER=${CROSS_COMPILE}gcc \
    -DCMAKE_CXX_COMPILER=${CROSS_COMPILE}g++ \
    -G "${CMAKE_GENERATOR}"

# Check if CMake configuration was successful
if [ $? -ne 0 ]; then
    printf "%s\n" "Error during CMake configuration. Please check the settings."
    exit 1
fi

# Build the project
printf "%s\n" "Building the project for ${TARGET_ARCH}..."
make -j$(nproc)

# Check if the build was successful
if [ $? -eq 0 ]; then
    printf "%s\n" "Build completed successfully for ${TARGET_ARCH}!"
else
    printf "%s\n" "Error during the build process."
    exit 1
fi

# Return to the root project directory
cd ${PROJECT_DIR}
