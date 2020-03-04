SET( DEFINE
)
SET( INCLUDE
core
)
SET( LINK
core
)

create_project(CONSOLE DEFINE INCLUDE LINK)

if(MSVC)
	add_definitions(/await)
	add_definitions(/std:c++17)
endif()