# Находим путь до папки с заголовочными файлами и записываем в ${LIBEVENT_INCLUDE_DIR}
find_path(LIBEVENT_INCLUDE_DIR event.h
        PATHS
        /usr/local
        /opt
        PATH_SUFFIXES
        include
        )

# Находим бинарные файлы библиотеки и записываем в ${LIBEVENT_LIB}
find_library(LIBEVENT_LIB
        NAMES
        event
        PATHS
        /usr/local
        /opt
        PATH_SUFFIXES
        lib
        lib64
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
        LIBEVENT_LIB
        LIBEVENT_INCLUDE_DIR
)