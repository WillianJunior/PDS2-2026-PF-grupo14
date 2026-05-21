# ==========================================
# MAKEFILE BÁSICO (WINDOWS + DOCTEST)
# ==========================================

# Compilador
CXX = g++

# Flags
CXXFLAGS = -std=gnu++17 -Wall -Wextra -D_GNU_SOURCE -Iinclude \
-DDOCTEST_CONFIG_NO_MULTITHREADING \
-DDOCTEST_CONFIG_NO_WINDOWS_SEH
COVERAGE_FLAGS = --coverage -fprofile-arcs -ftest-coverage

# Diretórios
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# ==========================================
# ARQUIVOS-FONTE
# ==========================================

# Remove o main.cpp do jogo
SRC_FILES = $(filter-out $(SRC_DIR)/main.cpp,$(wildcard $(SRC_DIR)/*.cpp))

# Objetos do sistema
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Arquivos de teste
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

# Objetos dos testes
TEST_OBJ = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/test_%.o,$(TEST_FILES))

# Executável
TEST_EXE = $(BUILD_DIR)/exec_tests.exe

# ==========================================
# TARGET PADRÃO
# ==========================================

all:
	@echo Rode: make test

# ==========================================
# COMPILAÇÃO DOS .CPP DO SISTEMA
# ==========================================

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $< -o $@

# ==========================================
# COMPILAÇÃO DOS TESTES
# ==========================================

$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.cpp
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ==========================================
# TESTES
# ==========================================

test: $(OBJ_FILES) $(TEST_OBJ)
	@echo Linkando executavel...
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) $(OBJ_FILES) $(TEST_OBJ) -o $(TEST_EXE)

	@echo Executando testes...
	$(TEST_EXE)

	@echo Gerando cobertura...
	gcovr --root . --exclude tests --print-summary

# ==========================================
# LIMPEZA
# ==========================================

clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)

.PHONY: all test clean