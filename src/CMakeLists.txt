include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. SRC)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_library(${PROJECT_NAME} STATIC ${SRC})