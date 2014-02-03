APP_NAME  := knittuk

CC        := g++
LD        := g++

CFLAGS    := -Wall -Wextra -Werror -O2
LDFLAGS    := -lsfml-graphics -lsfml-window -lsfml-system

MODULES   := jeu gui ia
SRC_DIR   := src $(addprefix src/,$(MODULES))
BUILD_DIR := build $(addprefix build/,$(MODULES))

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst src/%.cpp,build/%.o,$(SRC))
#INCLUDES  := $(addprefix -I,$(SRC_DIR))
INCLUDES  := -Isrc/

vpath %.cpp src

define make-goal
$1/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $$@ $$<
endef

.PHONY: all checkdirs clean doc

all: checkdirs build/$(APP_NAME)

build/$(APP_NAME): $(OBJ)
	$(LD) $^ $(LDFLAGS) -o $@

checkdirs: $(BUILD_DIR)

doc:
	mkdir -p doc
	doxygen Doxyfile

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf doc

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
