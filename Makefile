CC = g++
CXXFLAGS = -Wall
SOURCE_DIR = ./src
OBJECT_DIR = ./obj
LEXER_DIR = lexer
GTYPES_DIR = gtypes
TARGET = Gurungi

SOURCES_WITHDIR = $(wildcard $(SOURCE_DIR)/$(LEXER_DIR)/*.cpp) $(wildcard $(SOURCE_DIR)/*.cpp)\
$(wildcard $(SOURCE_DIR)/$(GTYPES_DIR)/*.cpp)
SOURCES = $(notdir $(SOURCES_WITHDIR))
_OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS = $(patsubst %.o,$(OBJECT_DIR)/%.o,$(_OBJECTS))
DEPS = $(OBJECTS:.o=.d)

RESET = \033[0m
INFO = \033[36m [*] $(RESET)
ERROR = \033[31m [!] $(RESET)
SUCCEED = \033[32m [O] $(RESET)

all: Gurungi

$(OBJECT_DIR)/%.o : $(SOURCE_DIR)/$(LEXER_DIR)/%.cpp
	@echo "$(ERROR) Warning: this release is unstable."
	@echo "$(INFO) Making an object file from $< to $@... "
	@$(CC) $(CXXFLAGS) -c $< -o $@ -MD

$(OBJECT_DIR)/%.o : $(SOURCE_DIR)/$(GTYPES_DIR)/%.cpp
	@echo "$(INFO) Making an object file from $< to $@... "
	@$(CC) $(CXXFLAGS) -c $< -o $@ -MD

$(OBJECT_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	@echo "$(INFO) Making an object file from $< to $@... "
	@$(CC) $(CXXFLAGS) -c $< -o $@ -MD

$(TARGET) : $(OBJECTS)
	@echo "$(INFO) Linking object files $(OBJECTS) into $@..." 
	@$(CC) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)
	@echo "$(SUCCEED) Successfully built `pwd`/$(TARGET)."
	
.PHONY: clean
clean:
	@if [ "`ls $(OBJECT_DIR)`" = "" ]; then \
	echo "$(INFO) $(OBJECT_DIR) is empty. Skipping cleaning up $(OBJECT_DIR)."; \
	else \
	echo "$(INFO) Cleaning up $(OBJECT_DIR)"; \
	rm $(OBJECT_DIR)/*; \
	fi;
	
	@if [ -f "$(TARGET)" ]; then \
	echo "$(INFO) Cleaning up $(TARGET)";  \
	rm $(TARGET);  \
	else \
	echo "$(INFO) Cannot find $(TARGET). Skipping deleting $(TARGET)."; \
	fi;

	@echo "$(SUCCEED) Cleaning up has been completed.";
	

-include $(DEPS)
