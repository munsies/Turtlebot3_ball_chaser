#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/manan/catkin_ws3/src/turtlebot3/turtlebot3_example"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/manan/catkin_ws3/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/manan/catkin_ws3/install/lib/python2.7/dist-packages:/home/manan/catkin_ws3/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/manan/catkin_ws3/build" \
    "/usr/bin/python2" \
    "/home/manan/catkin_ws3/src/turtlebot3/turtlebot3_example/setup.py" \
     \
    build --build-base "/home/manan/catkin_ws3/build/turtlebot3/turtlebot3_example" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/manan/catkin_ws3/install" --install-scripts="/home/manan/catkin_ws3/install/bin"
