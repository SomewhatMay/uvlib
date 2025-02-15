# Remove the build directory if it exists
if (Test-Path -Path "build") {
    Remove-Item -Recurse -Force "build"
}

# Create the build directory and navigate into it
New-Item -ItemType Directory -Force -Name "build"
Set-Location -Path "build"

# Run cmake and ctest
cmake -G "MinGW Makefiles" .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=20
ctest -N
mingw32-make.exe

# Go back to the previous directory
Set-Location -Path ".."