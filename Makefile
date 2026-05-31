# ==========================================
# MAKEFILE BÁSICO (MULTIPLATAFORMA + DOCTEST)
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
# DETECÇÃO DO SISTEMA OPERACIONAL E COMANDOS
# ==========================================

ifeq ($(OS),Windows_NT)
    # Comandos para Windows (CMD/PowerShell)
    MKDIR = if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
    RM = if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
    TEST_EXE = $(BUILD_DIR)/exec_tests.exe
    
    # Cobertura no Windows: tenta usar 'python -m gcovr', se falhar tenta apenas 'gcovr'
    GCOVR_CMD = python -m gcovr --root . --exclude tests --print-summary || gcovr --root . --exclude tests --print-summary
    GCOVR_HTML_CMD = python -m gcovr -r . --object-directory=build --html-details -o build/coverage.html || gcovr -r . --object-directory=build --html-details -o build/coverage.html
else
    # Comandos para Linux/macOS
    MKDIR = mkdir -p $(BUILD_DIR)
    RM = rm -rf $(BUILD_DIR)
    TEST_EXE = ./$(BUILD_DIR)/exec_tests.exe
    
    # Cobertura no Linux: tenta 'python3 -m gcovr', depois 'python -m gcovr', e por fim 'gcovr' direto
    GCOVR_CMD = python3 -m gcovr --root . --exclude tests --print-summary || python -m gcovr --root . --exclude tests --print-summary || gcovr --root . --exclude tests --print-summary
    GCOVR_HTML_CMD = python3 -m gcovr -r . --object-directory=build --html-details -o build/coverage.html || python -m gcovr -r . --object-directory=build --html-details -o build/coverage.html || gcovr -r . --object-directory=build --html-details -o build/coverage.html
endif

# ==========================================
# ARQUIVOS-FONTE
# ==========================================

# Remove o main.cpp do jogo para não conflitar com o main do doctest
SRC_FILES = $(filter-out $(SRC_DIR)/main.cpp,$(wildcard $(SRC_DIR)/*.cpp))

# Objetos do sistema
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Arquivos de teste
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

# Objetos dos testes
TEST_OBJ = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/test_%.o,$(TEST_FILES))

# ==========================================
# TARGET PADRÃO
# ==========================================

all:
	@echo "Rode: make test"

# ==========================================
# COMPILAÇÃO DOS .CPP DO SISTEMA
# ==========================================

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) -c $< -o $@

# ==========================================
# COMPILAÇÃO DOS TESTES
# ==========================================

$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.cpp
	@$(MKDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ==========================================
# EXECUÇÃO DE TESTES E REPORT DE COBERTURA
# ==========================================

test: $(OBJ_FILES) $(TEST_OBJ)
	@echo "Linkando executavel..."
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) $(OBJ_FILES) $(TEST_OBJ) -o $(TEST_EXE)
	@echo ""
	@echo "========================================================"
	@echo "                  EXECUTANDO TESTES                     "
	@echo "========================================================"
	-$(TEST_EXE)
	@echo ""
	@echo "========================================================"
	@echo "                GERANDO COBERTURA (GCOVR)               "
	@echo "========================================================"
	@$(GCOVR_CMD)

# ==========================================
# GERAR COBERTURA EM HTML VISUAL
# ==========================================

coverage-html: $(OBJ_FILES) $(TEST_OBJ)
	@$(GCOVR_HTML_CMD)
	@echo "--------------------------------------------------------"
	@echo "Relatorio visual gerado em: build/coverage.html"
	@echo "--------------------------------------------------------"

# ==========================================
# LIMPEZA
# ==========================================

clean:
	@$(RM)

.PHONY: all test clean coverage-html