.cpp.o:
	        $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

all:
	 $(MAKE) gobbler

clean:
	@rm *.o gobbler

