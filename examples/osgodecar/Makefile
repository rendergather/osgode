all: osgodecar




clean:
	rm -fr *.o *~ osgodecar




osgodecar: Car.o Car_init.o Car_handle.o HUD.o osgodecar.o
	g++ Car.o Car_init.o Car_handle.o HUD.o osgodecar.o -o osgodecar -losgODEUtil -losgDB -losg -lode -losgViewer -lpSound




.cpp.o:
	g++ -Wall -c $< -o $@
