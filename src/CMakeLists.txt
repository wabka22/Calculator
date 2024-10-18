set(target ${LIBRARY})

file(GLOB hdrs "*.h*" "${INCLUDE}/*.h*")
file(GLOB srcs "*.cpp")

include_directories("${INCLUDE}")

add_library(${target} STATIC ${srcs} ${hdrs})
