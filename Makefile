CXX := clang++
CXXFLAGS := -std=c++17

SOURCE_DIR := src
INCLUDE_DIR := src/gten
BUILD_DIR := build

TARGET := minichatgpt
SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS := $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

MODEL     := models/minchatgpt-md.gten
MODEL_URL := "https://huggingface.co/iangitonga/gten/resolve/main/minchatgpt-md.gten"
# MODEL_URL := "https://huggingface.co/iangitonga/gten/resolve/main/minchatgpt-lg.gten"

.PHONY: all

all: $(MODEL) $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(MODEL):
	mkdir -p models
	curl $(MODEL_URL) -o $(MODEL)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
