# https://stackoverflow.com/a/52036564

SOURCES := soln.cpp
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))
DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES))
CXXFLAGS := -Wall -Wextra -g -DDEBUG -I../library/

# .PHONY means these rules get executed even if
# files of those names exist.
.PHONY: all clean

all: soln

clean:
	$(RM) $(OBJECTS) $(DEPENDS) soln

soln: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@	

-include $(DEPENDS)

%.o: %.cpp Makefile
	$(CXX) $(WARNING) $(CXXFLAGS) -MMD -MP -c $< -o $@


run: soln
	./soln < input

reset: clean
	echo '#include "template.hpp"\nusing namespace std;\n\nint main() {\n\n}\n' > soln.cpp

generate: clean
	python3 ../library/preprocess.py soln.cpp > soln_upload.cpp
