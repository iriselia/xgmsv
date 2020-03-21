SET( DEFINE
)
SET( INCLUDE
asio
docopt
imgui
simple_web_server
)
SET( LINK
asio
docopt
imgui
simple_web_server
)
create_project(STATIC DEFINE INCLUDE LINK)

if(MSVC)
	add_definitions(/await)
	add_definitions(/experimental:module)
	add_definitions(/std:c++latest)
endif()