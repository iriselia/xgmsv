SET( DEFINE
)
SET( INCLUDE
asio
core
docopt
imgui
)
SET( LINK
asio
core
docopt
imgui
)

create_project(CONSOLE DEFINE INCLUDE LINK)

if(MSVC)
	add_definitions(/await)
	add_definitions(/std:c++17)
endif()