SRC=$(wildcard *.cc)
OBJ=$(SRC:%.cc=%.o)
TARGET:=libldbnet.so


CXXFLAGS := -g -O2 -fPIC -I../
LDFLAGS := 

$(TARGET): $(OBJ) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared $^ -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) $< -c -o $@

%.d: %.cc
	@$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

sinclude $(SRC:.cc=.d)

.PHONY: clean

clean:
	@rm $(TARGET) $(OBJ) *.d -f

