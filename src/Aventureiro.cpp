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
    _bonusArma = 1; _bonusArmadura = 1; _xp = 0; _xpProxNivel = 60;
    _idCenaCheckpoint = 0; this->_escudoAtivo = false;
    
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
                        else if (hbSelecionada.getTipo() == TipoHabilidade::HOT) {
                            // Se for DoT, injeta o status duradouro no inimigo para agir nos turnos seguintes
                            // Passa o Nome, o Dano por Turno e define a duração fixa (ex: 3 rodadas)
                            this->aplicarHoT(hbSelecionada.getNome(), hbSelecionada.getValorBase(), hbSelecionada.getDuracaoEfeito());
                        }      
                        
                        else if (hbSelecionada.getTipo() == TipoHabilidade::SUPORTE) {
                            // Aplica o Buff estatístico no próprio jogador
                            this->aplicarBuff(hbSelecionada.getNome(), hbSelecionada.getAtributoAlvo(), hbSelecionada.getValorBase(), hbSelecionada.getDuracaoEfeito());
                        }
                            else if (hbSelecionada.getTipo() == TipoHabilidade::DEBUFF) {
                            // [Aplica o Debuff estatístico mitigador no inimigo
                            alvo.aplicarDebuff(hbSelecionada.getNome(), hbSelecionada.getAtributoAlvo(), hbSelecionada.getValorBase(), hbSelecionada.getDuracaoEfeito());
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
                if (this->_energia >= 6) {
                    this->usarEscudo();
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
                    this->receberCura(_hpMax * (0.25)); 
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
void Aventureiro::usarEscudo() {
    this->_escudoAtivo = true;
    this->consumirEnergia(6);

}
void Aventureiro::recuperarRecursos() {
    _energia = std::min(_energiaMax, _energia + 5); // +5 de Energia
    _mp = std::min(_mpMax, _mp + 1);                // +1 de MP
    
    InterfaceJogo::exibirTexto("> Turno iniciado. Você recuperou 5 de Energia e 1 de MP.");
}
void Aventureiro::consumirMP(int qtd) { 
    if ((_mp - qtd) < 0){_mp = 0;}
    else{_mp -= qtd;}    
}
void Aventureiro::consumirEnergia(int qtd) {     
    if ((_energia - qtd) < 0){_energia = 0;}
    else{_energia -= qtd;}    }
void Aventureiro::dormir() {
    _hp = _hpMax;           // Restaura a saúde
    _mp = _mpMax;           // Mecânica de MP: Restaurado ao dormir
    _energia = _energiaMax; // Restaura o fôlego
    _frascos = 3;           // Ciclo de Sobrevivência: Reabastece os consumíveis
    
    InterfaceJogo::exibirTexto("\n[RECONFORTO] Você descansa profundamente. Seus recursos foram totalmente restaurados!");}

// ============================================================================
// MÉTODO PRIVADO
// ============================================================================

void Aventureiro::subirNivel() {

        // 1. Progressão Aritmética Base (Aumento de atributos)
    this->_nivel++;
    this->_hpMax += 40; // Ganha +15 de vida máxima por nível
    this->_forcaBase += 5; // Ganha +3 de ataque base por nível
    this->_defesaBase += 4; // Ganha +2 de defesa base por nível

    // Restaura completamente a saúde do herói como bônus
    this->_hp = this->_hpMax; 

    InterfaceJogo::exibirTexto("\n✨ SEU NÍVEL AUMENTOU PARA O NÍVEL " + std::to_string(this->_nivel) + "! ✨");

    // 2. Distribuição de Novas Habilidades em Níveis Específicos
    switch (this->_nivel) {
        case 2: {
            Habilidade socoForte("Soco Forte", TipoHabilidade::FISICO, 3 , 0 , 30 ,"", 0);

            
            this->adicionarHabilidade(socoForte);
            InterfaceJogo::exibirTexto("[NOVA HABILIDADE] Você aprendeu: Golpe Esmagador!");
            break;
        }
        case 4: {
            Habilidade aplicarAdrenalina("Aplicar Adrenalina", TipoHabilidade::CURA, 2, 3, 50,"", 0);
          
            this->adicionarHabilidade(aplicarAdrenalina);
            InterfaceJogo::exibirTexto("[NOVA HABILIDADE] Você aprendeu: Escudo Divino!");
            break;
        }
        case 5: {

            Habilidade ataqueMental("ATAQUE MENTAL", TipoHabilidade::ULTRA, 6 , 0 , 75 ,"", 0);

            this->adicionarHabilidade(ataqueMental);
            InterfaceJogo::exibirTexto("[NOVA HABILIDADE] Você aprendeu: Meteoro Arcano!");
            break;
        }
        default:
            // Níveis que dão apenas atributos brutos
            break;
    }
}

// ============================================================================
// MÉTODOS DE PROGRESSÃO E BUFFS
// ============================================================================

void Aventureiro::ganharExperiencia(int qtd) {
    if (qtd <= 0) return;

    this->_xp += qtd;
    
    // Enquanto o XP for suficiente para subir (trata o caso de ganhar muito XP de uma vez)
    while (this->_xp >= _xpProxNivel) {
        this->_xp -= _xpProxNivel; // Deduz o XP gasto
        this->subirNivel();             // Executa a aritmética de atributos
        
        // Atualiza o próximo limite para o novo nível
        _xpProxNivel = this->_nivel * 60; 
    }
}
void Aventureiro::buffArma(int valor) { 
    _bonusArma += 1.0f + (static_cast<float>(valor) / 100.0f);
 }
void Aventureiro::buffArmadura(int valor) { 
    _bonusArmadura += 1.0f + (static_cast<float>(valor) / 100.0f);

 }
std::string Aventureiro::getDeclaracaoStatus() const {
    std::string status = "==================================================\n";
    status += "  STATUS DE " + this->getNome() + " [Nível " + std::to_string(this->getNivel()) + "]\n";
    status += "==================================================\n";
    
    // Barras de Vida, Energia e MP
    status += "  ❤️ HP      : " + std::to_string(this->getHP()) + " / " + std::to_string(this->getHPMax()) + "\n";
    status += "  ⚡ Energia : " + std::to_string(this->_energia) + " / " + std::to_string(this->_energiaMax) + "\n";
    status += "  🔮 MP      : " + std::to_string(this->_mp) + " / " + std::to_string(this->_mpMax) + "\n";
    
    status += "--------------------------------------------------\n";
    
    // Atributos de Combate atualizados (já considerando bônus de equipamentos)
    status += "  ⚔️ Força   : " + std::to_string(this->_forcaBase) + " (+ " + std::to_string(this->_bonusArma) + " Arma)\n";
    status += "  🛡️ Defesa  : " + std::to_string(this->_defesaBase) + " (+ " + std::to_string(this->_bonusArmadura) + " Armadura)\n";
    
    status += "--------------------------------------------------\n";
    
    // Itens e Progressão
    status += "  🧪 Frascos : " + std::to_string(this->_frascos) + " restantes\n";
    status += "  ✨ XP      : " + std::to_string(this->_xp) + " / " + std::to_string(this->_xpProxNivel) + "\n";
    status += "==================================================";

    return status; }

// ============================================================================
// GETTERS E SETTERS DE ESTADO
// ============================================================================

int Aventureiro::getForcaTotal() const { 
// 1. Pega a força base somada/subtraída pelos Buffs/Debuffs de Personagem
    int forcaComEfeitos = Personagem::getForcaTotal();
    std::cout << "Personagem::getForcaTotal\n";

    // 2. Aplica o modificador permanente da arma do Aventureiro
    return forcaComEfeitos * _bonusArma;
}

int Aventureiro::getDefesa() const {
    // 1. Pega a defesa já calculada com os Buffs/Debuffs temporários da classe base
    int defesaComEfeitos = Personagem::getDefesa(); 
    
    // 2. Aplica as regras exclusivas do Aventureiro (ex: multiplicador de armadura)
    // Se o seu _bonusArmadura for um multiplicador (ex: 1.20 para +20%):
    return defesaComEfeitos * _bonusArmadura; 
}

int Aventureiro::getIDCheckpoint() const { 
    return _idCenaCheckpoint; }
void Aventureiro::setIDCheckpoint(int idCena) {
    // Evita salvar um ID inválido ou negativo por segurança
    if (idCena > 0) {
        this->_idCenaCheckpoint = idCena;
    }
}int Aventureiro::getMP() const { return _mp; }
int Aventureiro::getMPMax() const { 
    return _mpMax; }
int Aventureiro::getEnergia() const { 
    return _energia; }
int Aventureiro::getEnergiaMax() const { 
    return _energiaMax; }

