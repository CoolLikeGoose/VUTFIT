CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I./src -I./tests
BUILD_DIR := obj
SRC_DIR := src
TEST_DIR := tests

MAIN_SRC := $(SRC_DIR)/snmp2otel.cpp
MAIN_OBJ := $(BUILD_DIR)/snmp2otel.o
MAIN_BIN := snmp2otel

TEST_BIN := run_tests

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))

all: $(MAIN_BIN)

$(MAIN_BIN): $(OBJS)
	@echo "Linking $@ ..."
	$(CXX) $(CXXFLAGS) $^ -o $@

tests: $(OBJS) $(TEST_OBJS)
	@echo "Linking $@ ..."
	$(CXX) $(CXXFLAGS) $^ -o $(TEST_BIN)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Compiling $< ..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Compiling $< ..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(MAIN_BIN) $(TEST_BIN)

run: $(MAIN_BIN)
	./$(MAIN_BIN)

run-tests: tests
	./$(TEST_BIN)

ifeq ($(OS),Windows_NT)
	CLEAN = rmdir /S /Q $(BUILD_DIR) & del /Q $(MAIN_BIN).exe $(TEST_BIN).exe 2>nul || true
	run: $(MAIN_BIN)
		$(MAIN_BIN).exe
	run-tests: tests
		$(TEST_BIN).exe
	clean:
		@$(CLEAN)
endif

# -t target [-C community] -o oids_file -e endpoint [-i interval] [-r retries] [-T timeout] [-p port] [-v]
# -t target [-C community] -o oids_file -e endpoint [-i interval] [-r retries] [-T timeout] [-p port] [-v]
# .\snmp2otel.exe -t 8.8.8.8 -o oids.txt -e http://localhost:4318/v1/metrics