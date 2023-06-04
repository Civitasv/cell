add_library(cell cell.c log.c)

target_include_directories(cell PUBLIC ${PROJECT_SOURCE_DIR}/include)
