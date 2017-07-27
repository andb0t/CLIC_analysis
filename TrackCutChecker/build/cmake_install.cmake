# Install script for directory: /afs/cern.ch/user/a/amaier/CLIC/TrackCutChecker

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/afs/cern.ch/user/a/amaier/CLIC/TrackCutChecker")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTrackCutChecker.so.0.1.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTrackCutChecker.so.0.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTrackCutChecker.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "/afs/cern.ch/user/a/amaier/CLIC/TrackCutChecker/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/DD4hep/HEAD/lib:/cvmfs/clicdp.cern.ch/software/ROOT/6.08.00/x86_64-slc6-gcc62-opt/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/Marlin/HEAD/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/lcio/v02-07p1/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/gear/HEAD/lib:/cvmfs/clicdp.cern.ch/software/CLHEP/2.3.1.1/x86_64-slc6-gcc62-opt/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/ilcutil/HEAD/lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/afs/cern.ch/user/a/amaier/CLIC/TrackCutChecker/build/lib/libTrackCutChecker.so.0.1.0"
    "/afs/cern.ch/user/a/amaier/CLIC/TrackCutChecker/build/lib/libTrackCutChecker.so.0.1"
    "/afs/cern.ch/user/a/amaier/CLIC/TrackCutChecker/build/lib/libTrackCutChecker.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTrackCutChecker.so.0.1.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTrackCutChecker.so.0.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libTrackCutChecker.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/DD4hep/HEAD/lib:/cvmfs/clicdp.cern.ch/software/ROOT/6.08.00/x86_64-slc6-gcc62-opt/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/Marlin/HEAD/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/lcio/v02-07p1/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/gear/HEAD/lib:/cvmfs/clicdp.cern.ch/software/CLHEP/2.3.1.1/x86_64-slc6-gcc62-opt/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/ilcutil/HEAD/lib::::::::::::::::::::::::::::::::::::::::::::::::::::"
           NEW_RPATH "/afs/cern.ch/user/a/amaier/CLIC/TrackCutChecker/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/DD4hep/HEAD/lib:/cvmfs/clicdp.cern.ch/software/ROOT/6.08.00/x86_64-slc6-gcc62-opt/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/Marlin/HEAD/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/lcio/v02-07p1/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/gear/HEAD/lib:/cvmfs/clicdp.cern.ch/software/CLHEP/2.3.1.1/x86_64-slc6-gcc62-opt/lib:/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-24/x86_64-slc6-gcc62-opt/ilcutil/HEAD/lib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/afs/cern.ch/user/a/amaier/CLIC/TrackCutChecker/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
