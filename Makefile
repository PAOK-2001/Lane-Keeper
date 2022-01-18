CC      = g++
PROJECT = DetectLanes
SRC     = laneDetector.cpp
LIBS    = `pkg-config --cflags --libs opencv4`
$(PROJECT) : $(SRC)
	$(CC) $(SRC) -o $(PROJECT) $(LIBS)