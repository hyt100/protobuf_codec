include_directories(${PROJECT_ROOT_PATH}/src/)
#add_definitions("-Wall -g")

add_executable(run demo.cc addressbook.pb.cc addressbook.pb.h)
target_link_libraries(run ProtobufCodec)
target_link_libraries(run protobuf)
target_link_libraries(run pthread)
