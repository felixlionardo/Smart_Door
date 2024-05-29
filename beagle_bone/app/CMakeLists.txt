# Build the app, using the HAL

include_directories(include)
file(GLOB MY_SOURCES "src/*.c")
add_executable(smart_door ${MY_SOURCES})

# Make use of the HAL library
find_package(Threads REQUIRED)
target_link_libraries(smart_door LINK_PRIVATE hal Threads::Threads)


# Copy executable to final location (change `hello_world` to project name as needed)
add_custom_command(TARGET smart_door POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:smart_door>"
     "~/cmpt433/public/Final Project/smart_door" 
  COMMENT "Copying ARM executable to public NFS directory")
