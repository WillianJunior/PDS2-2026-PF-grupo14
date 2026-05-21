# =============================================================
# Makefile — RPG Textual em C++
# Targets: all | test | coverage | clean
# =============================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
COVFLAGS := --coverage -fprofile-arcs -ftest-coverage

# Diretórios
SRC_DIR   := src
INC_DIR   := include
TEST_DIR  := tests
BUILD_DIR := build
COV_DIR   := coverage

# Doctest (instalado via apt em /usr/include/doctest/doctest.h)
DOCTEST_INC := /usr/include

# Fontes de produção (todas as implementações, exceto main do jogo)
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Fontes de teste
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)

# Objetos de produção (com flags de cobertura)
PROD_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Objetos de teste
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/test_%.o, $(TEST_SRCS))

# Executável de testes
TEST_BIN := $(BUILD_DIR)/run_tests

# =============================================================
# TARGET PADRÃO
# =============================================================
.PHONY: all
all: $(TEST_BIN)

# =============================================================
# TARGET: test — compila e executa os testes, depois gera cobertura
# =============================================================
.PHONY: test
test: $(TEST_BIN)
	@echo ""
	@echo "============================================="
	@echo "  Executando testes de unidade (doctest)"
	@echo "============================================="
	./$(TEST_BIN) --success
	@echo ""
	@echo "============================================="
	@echo "  Gerando relatório de cobertura (gcovr)"
	@echo "============================================="
	@mkdir -p $(COV_DIR)
	gcovr --root . \
	      --exclude '$(TEST_DIR)/' \
	      --exclude '/usr/' \
	      --html-details $(COV_DIR)/index.html \
	      --print-summary \
	      -o $(COV_DIR)/coverage.txt
	@echo ""
	@echo "Relatório HTML gerado em: $(COV_DIR)/index.html"
	@echo "Resumo salvo em:          $(COV_DIR)/coverage.txt"

# =============================================================
# LINKAGEM DO EXECUTÁVEL DE TESTES
# =============================================================
$(TEST_BIN): $(PROD_OBJS) $(TEST_OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(COVFLAGS) $^ -o $@

# =============================================================
# COMPILAÇÃO DOS OBJETOS DE PRODUÇÃO (com cobertura)
# =============================================================
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(COVFLAGS) -I$(INC_DIR) -c $< -o $@

# =============================================================
# COMPILAÇÃO DOS OBJETOS DE TESTE
# =============================================================
$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(COVFLAGS) -I$(INC_DIR) -I$(DOCTEST_INC) -c $< -o $@

# =============================================================
# CRIAÇÃO DOS DIRETÓRIOS
# =============================================================
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(COV_DIR):
	mkdir -p $(COV_DIR)

# =============================================================
# TARGET: clean — remove artefatos gerados
# =============================================================
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(COV_DIR)
	@echo "Limpeza concluída."
