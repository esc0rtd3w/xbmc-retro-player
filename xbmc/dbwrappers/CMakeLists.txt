set(SOURCES Database.cpp
            DatabaseQuery.cpp
            dataset.cpp
            qry_dat.cpp
            DynamicDatabase.cpp
            sqlitedataset.cpp)

set(HEADERS Database.h
            DatabaseQuery.h
            dataset.h
            qry_dat.h
            DynamicDatabase.h
            sqlitedataset.h)

if(MYSQLCLIENT_FOUND)
  list(APPEND SOURCES mysqldataset.cpp)
  list(APPEND HEADERS mysqldataset.h)
endif()

core_add_library(dbwrappers)
