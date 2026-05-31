#include "Aventureiro.hpp"
#include <iostream>
#include <stdexcept>
#include "InterfaceJogo.hpp"

// ============================================================================
// CONSTRUTOR
// ============================================================================

Aventureiro::Aventureiro(std::string nome, int hp, int defesa, int forca)
    : Personagem(nome, hp, defesa, forca, 1) { 
    
    _energia = 100; _energiaMax = 100; _mp = 50; _mpMax = 50; _frascos = 3;
    _bonusArma = 0; _bonusArmadura = 0; _xp = 0; _xpProxNivel = 100;
    _idCenaCheckpoint = 0; _escudoAtivo = false;
}

// ============================================================================
// MÉTODOS DE COMBATE E AÇÕES (Estilo Esqueleto/TDD Red)
// ============================================================================

void Aventureiro::executarTurno(Personagem& alvo) {
// 1. GATILHO DE RODADA: Processa venenos e sangramentos acumulados antes de qualquer escolha
    this->processarEfeitosContinuos();

    // SE O DoT MATOU O HERÓI: Corta a execução aqui para impedir que um personagem morto jogue
    if (!this->estaVivo()) return; 

    // O escudo ativo da rodada anterior expira no começo do próprio turno
    this->_escudoAtivo = false;

    // Regeneração passiva de recursos do início do turno (+5 Energia, +1 MP)
    recuperarRecursos();

    // Lista de opções que alimentará o menu principal de combate
    std::vector<std::string> menuPrincipal = {
        "Ataque Básico (Grátis)",
        "Habilidades",
        "Erguer Escudo (Gasta: 5 Energia)",
        "Usar Frasco de Cura"
    };

    bool acaoRealizada = false;

    // Loop que mantém o menu aberto até que o jogador execute uma ação válida
    while (!acaoRealizada) {
        
        // Exibe o HUD formatado com os atributos do herói usando o getDeclaracaoStatus
        InterfaceJogo::exibirTexto("\n" + this->getDeclaracaoStatus());

        int escolhaPrincipal = InterfaceJogo::solicitarEscolha(menuPrincipal);

        switch (escolhaPrincipal) {
            
            case 1: // ⚔️ ATAQUE BÁSICO
                // Causa dano imediato baseado na força e define a categoria como FISICO
                alvo.receberDano(this->getForcaTotal(), TipoHabilidade::FISICO);
                acaoRealizada = true;
                break;

            case 2: { // 📜 SUB-MENU DE HABILIDADES (Escopo isolado por chaves)
                const std::vector<Habilidade>& habilidadesConhecidas = this->getHabilidades();

                // Bloqueio de segurança caso o jogador tente acessar sem ter técnicas no vetor
                if (habilidadesConhecidas.empty()) {
                    InterfaceJogo::exibirTexto("[AVISO] Você não conhece nenhuma habilidade ainda!");
                    break; 
                }

                // Monta a lista visual de habilidades ocultando a categoria interna (TipoHabilidade)
                std::vector<std::string> menuHabilidades;
                for (const auto& hb : habilidadesConhecidas) {
                    std::string textoHabilidade = hb.getNome() + " (Dano/Efeito: " + std::to_string(hb.getValorBase()) + ")";
                    
                    if (hb.getCustoMP() > 0) {
                        textoHabilidade += " [Gasta: " + std::to_string(hb.getCustoMP()) + " MP]";
                    }
                    if (hb.getCustoEnergia() > 0) {
                        textoHabilidade += " [Gasta: " + std::to_string(hb.getCustoEnergia()) + " Energia]";
                    }
                    menuHabilidades.push_back(textoHabilidade);
                }
                menuHabilidades.push_back("<- Voltar");

                int escolhaHb = InterfaceJogo::solicitarEscolha(menuHabilidades);

                // Permite ao jogador desistir e voltar para o menu anterior sem perder o turno
                if (escolhaHb == static_cast<int>(menuHabilidades.size())) {
                    break; 
                }

                // Bloco try-catch para capturar possíveis erros de índice no vetor
                try {
                    const Habilidade& hbSelecionada = habilidadesConhecidas.at(escolhaHb - 1);

                    // Validação física e mágica de recursos duais antes de conjurar
                    if (this->_mp >= hbSelecionada.getCustoMP() && this->_energia >= hbSelecionada.getCustoEnergia()) {
                        
                        // Deduz os custos das barras do aventureiro
                        this->consumirMP(hbSelecionada.getCustoMP());
                        this->consumirEnergia(hbSelecionada.getCustoEnergia());
                        
                        InterfaceJogo::exibirTexto(this->getNome() + " usa " + hbSelecionada.getNome() + "!");
                        
                        // DIRECIONAMENTO TÁTICO CONFORME O TIPO INTERNO DA HABILIDADE:
                        if (hbSelecionada.getTipo() == TipoHabilidade::CURA) {
                            // Cura foca no próprio conjurador
                            this->receberCura(hbSelecionada.getValorBase());
                        } 
                        else if (hbSelecionada.getTipo() == TipoHabilidade::DOT) {
                            // Se for DoT, injeta o status duradouro no inimigo para agir nos turnos seguintes
                            // Passa o Nome, o Dano por Turno e define a duração fixa (ex: 3 rodadas)
                            alvo.aplicarDoT(hbSelecionada.getNome(), hbSelecionada.getValorBase(), hbSelecionada.getDuracaoEfeito());
                        } 
                        else {
                            // Ataques tradicionais (Fisico, Especial, Ultra) resolvem o dano na hora
                            alvo.receberDano(hbSelecionada.getValorBase(), hbSelecionada.getTipo());
                        }
                        
                        acaoRealizada = true; // Valida o encerramento da rodada
                    } else {
                        InterfaceJogo::exibirTexto("[ERRO] Recursos insuficientes para esta habilidade!");
                    }
                } 
                catch (const std::out_of_range& e) {
                    InterfaceJogo::exibirTexto("[SISTEMA - ERRO CRÍTICO] Tentativa de acessar uma habilidade inválida.");
                    InterfaceJogo::exibirTexto("Sua ação foi resetada. Por favor, escolha novamente.");
                }
                break;
            }

            case 3: // 🛡️ ERGUER ESCUDO
                // Ativa a postura defensiva que mitiga 50% de ataques comuns no turno do inimigo
                if (this->_energia >= 5) {
                    this->consumirEnergia(5);
                    this->_escudoAtivo = true; 
                    InterfaceJogo::exibirTexto(this->getNome() + " ergue o escudo se preparando para o pior!");
                    acaoRealizada = true;
                } else {
                    InterfaceJogo::exibirTexto("[ERRO] Energia insuficiente!");
                }
                break;

            case 4: // 🧪 USAR FRASCO DE CURA
                // Consome um consumível para restaurar pontos de vida de forma imediata
                if (this->_frascos > 0) {
                    this->_frascos--; 
                    this->receberCura(40); 
                    acaoRealizada = true;
                } else {
                    InterfaceJogo::exibirTexto("[ERRO] Sem frascos disponíveis!");
                }
                break;

            default: 
                InterfaceJogo::exibirTexto("[SISTEMA] Opção inválida selecionada.");
                break;
        }
    }
}

void Aventureiro::receberDano(int dano, TipoHabilidade tipoDaHabilidade) {

}
int Aventureiro::getDefesa() const { return 0; }
void Aventureiro::recuperarRecursos() {
    _energia = std::min(_energiaMax, _energia + 5); // +5 de Energia
    _mp = std::min(_mpMax, _mp + 1);                // +1 de MP
    
    InterfaceJogo::exibirTexto("> Turno iniciado. Você recuperou 5 de Energia e 1 de MP.");
}
void Aventureiro::consumirMP(int qtd) { (void)qtd; }
void Aventureiro::consumirEnergia(int qtd) { (void)qtd; }
void Aventureiro::usarEscudo() {}
bool Aventureiro::usarFrasco() { return false; }
void Aventureiro::dormir() {
    _hp = _hpMax;           // Restaura a saúde
    _mp = _mpMax;           // Mecânica de MP: Restaurado ao dormir
    _energia = _energiaMax; // Restaura o fôlego
    _frascos = 3;           // Ciclo de Sobrevivência: Reabastece os consumíveis
    
    InterfaceJogo::exibirTexto("\n[RECONFORTO] Você descansa profundamente. Seus recursos foram totalmente restaurados!");}

// ============================================================================
// MÉTODOS DE PROGRESSÃO E BUFFS
// ============================================================================

void Aventureiro::aprenderHabilidade(Habilidade hb) { (void)hb; }
bool Aventureiro::ganharExperiencia(int qtd) { (void)qtd; return false; }
void Aventureiro::buffArma(int valor) { (void)valor; }
void Aventureiro::buffArmadura(int valor) { (void)valor; }
std::string Aventureiro::getDeclaracaoStatus() const { return ""; }

// ============================================================================
// GETTERS E SETTERS DE ESTADO
// ============================================================================

int Aventureiro::getForcaTotal() const { return 0; }
int Aventureiro::getIDCheckpoint() const { return 0; }
void Aventureiro::setIDCheckpoint(int id) { (void)id; }
int Aventureiro::getMP() const { return 0; }
int Aventureiro::getMPMax() const { return 0; }
int Aventureiro::getEnergia() const { return 0; }
int Aventureiro::getEnergiaMax() const { return 0; }

const std::vector<Habilidade>& Aventureiro::getHabilidades() const { 
    return _habilidadesConhecidas; 
}

// ============================================================================
// MÉTODO PRIVADO
// ============================================================================

void Aventureiro::subirNivel() {}