CXX := g++
CXXFLAGS := -std=c++17

SOURCE_DIR := src
INCLUDE_DIRS := src/gten src/fpga
INCLUDE_FLAGS := $(addprefix -I, $(INCLUDE_DIRS))
BUILD_DIR := build

TARGET := minichatgpt
SOURCES := $(shell find $(SOURCE_DIR) -name "*.cpp")
OBJECTS := $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

MODEL     := models/minchatgpt-md.gten
MODEL_URL := "https://huggingface.co/iangitonga/gten/resolve/main/minchatgpt-md.gten"
# MODEL_URL := "https://huggingface.co/iangitonga/gten/resolve/main/minchatgpt-lg.gten"

.PHONY: all

all: $(MODEL) $(TARGET)

$(MODEL):
	mkdir -p models
	curl $(MODEL_URL) -o $(MODEL)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

reinstall: clean all