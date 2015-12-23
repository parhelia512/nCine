find_package(Git)
if(GIT_EXECUTABLE)
	execute_process(
		COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_REV_COUNT
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	
	execute_process(
		COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_SHORT_HASH
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	
	execute_process(
		COMMAND ${GIT_EXECUTABLE} log -1 --format=%ad --date=short
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_LAST_COMMIT_DATE
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	
	string(REPLACE "-" ";" GIT_LAST_COMMIT_DATE ${GIT_LAST_COMMIT_DATE})
	list(GET GIT_LAST_COMMIT_DATE 0 NCINE_MAJOR_VERSION)
	list(GET GIT_LAST_COMMIT_DATE 1 NCINE_MINOR_VERSION)
	set(NCINE_PATCH_VERSION "r${GIT_REV_COUNT}-${GIT_SHORT_HASH}")
else()
	string(TIMESTAMP NCINE_MAJOR_VERSION "%Y")
	string(TIMESTAMP NCINE_MINOR_VERSION "%m")
	string(TIMESTAMP NCINE_PATCH_VERSION "%d")
endif()

set(NCINE_VERSION "${NCINE_MAJOR_VERSION}.${NCINE_MINOR_VERSION}.${NCINE_PATCH_VERSION}")
mark_as_advanced(NCINE_MAJOR_VERSION NCINE_MINOR_VERSION NCINE_PATCH_VERSION NCINE_VERSION)