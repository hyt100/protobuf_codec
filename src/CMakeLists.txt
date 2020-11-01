file(GLOB_RECURSE src ${CMAKE_CURRENT_SOURCE_DIR}/*)
add_library(ProtobufCodec STATIC ${src})
