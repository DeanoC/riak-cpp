cmake_minimum_required( VERSION 2.8 )


file( GLOB cpp_src_files riak/*.cxx riak/transports/*.cxx )
file( GLOB cpp_header_files riak/*.hxx riak/transports/*.hxx riak/*.proto )

file( GLOB proto_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} riak/*.proto )
PROTOBUF_GENERATE_CPP( proto_src_files proto_header_files ${proto_files} )
list( APPEND cpp_src_files ${proto_src_files} )
list( APPEND cpp_header_files ${proto_header_files} )

add_library( riak-cpp ${cpp_src_files} ${cpp_header_files} )

if( WIN32 )
set_property(
   TARGET riak-cpp 
   PROPERTY COMPILE_DEFINITIONS not=! and=&& or=||
   )
endif()
# make a nice hierachy
set_target_properties( riak-cpp PROPERTIES FOLDER "Libs" )
