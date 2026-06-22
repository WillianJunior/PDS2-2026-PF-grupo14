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
    _bonusArma = 0; _bonusArmadura = 0; _xp = 0; _xpProxNivel = 60;
    _idCenaCheckpoint = 0; this->_escudoAtivo = false;
}  

// ============================================================================
// MÉTODOS DE COMBATE E AÇÕES
// ============================================================================

void Aventureiro::executarTurno(Personagem& alvo) { 
    // 1. GATILHO DE RODADA: Processa venenos e sangramentos acumulados
    this->processarEfeitosContinuos();

    // SE O DoT MATOU O HERÓI: Corta a execução aqui
    if (!this->estaVivo()) return; 

    // O escudo ativo da rodada anterior expira no começo do próprio turno
    this->_escudoAtivo = false;

    // Regeneração passiva de recursos do início do turno (+5 Energia, +1 MP)
    recuperarRecursos();

    // Lista de opções que alimentará o menu principal de combate
    std::vector<std::string> menuPrincipal = {
        "Ataque Básico (Grátis)",
        "Habilidades",
        "Erguer Escudo (Gasta: 6 Energia)",
        "Usar Frasco de Cura"
    };

    bool acaoRealizada = false;

    // Loop que mantém o menu aberto até que o jogador execute uma ação válida
    while (!acaoRealizada) {
        
        // Exibe o HUD formatado com os atributos do herói
        InterfaceJogo::exibirTexto("\n" + this->getDeclaracaoStatus());

        int escolhaPrincipal = InterfaceJogo::solicitarEscolha(menuPrincipal);

        switch (escolhaPrincipal) {
            
            case 1: // ⚔️ ATAQUE BÁSICO
                InterfaceJogo::exibirTexto("\n👊 [ATAQUE FÍSICO] " + this->getNome() + " avança para um Ataque Básico!");
                alvo.receberDano(this->getForcaTotal(), TipoHabilidade::FISICO);
                acaoRealizada = true;
                break;

            case 2: { // 📜 SUB-MENU DE HABILIDADES
                const std::vector<Habilidade>& habilidadesConhecidas = this->getHabilidades();

                if (habilidadesConhecidas.empty()) {
                    InterfaceJogo::exibirTexto("[AVISO] Você não conhece nenhuma habilidade ainda!");
                    break; 
                }

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

                if (escolhaHb == static_cast<int>(menuHabilidades.size())) {
                    break; 
                }

                try {
                    const Habilidade& hbSelecionada = habilidadesConhecidas.at(escolhaHb - 1);

                    if (this->_mp >= hbSelecionada.getCustoMP() && this->_energia >= hbSelecionada.getCustoEnergia()) {
                        
                        this->consumirMP(hbSelecionada.getCustoMP());
                        this->consumirEnergia(hbSelecionada.getCustoEnergia());
                        
                        // DIRECIONAMENTO TÁTICO CONFORME O TIPO INTERNO DA HABILIDADE:
                        if (hbSelecionada.getTipo() == TipoHabilidade::CURA) {
                            InterfaceJogo::exibirTexto("\n💚 [CURA] " + this->getNome() + " usa " + hbSelecionada.getNome() + "!");
                            this->receberCura(hbSelecionada.getValorBase());
                        } 
                        else if (hbSelecionada.getTipo() == TipoHabilidade::DOT) {
                            InterfaceJogo::exibirTexto("\n🩸 [EFEITO] " + this->getNome() + " usa " + hbSelecionada.getNome() + " e causa uma ferida aberta!");
                            alvo.aplicarDoT(hbSelecionada.getNome(), hbSelecionada.getValorBase(), hbSelecionada.getDuracaoEfeito());
                        } 
                        else if (hbSelecionada.getTipo() == TipoHabilidade::HOT) {
                            InterfaceJogo::exibirTexto("\n✨ [REGENERAÇÃO] " + this->getNome() + " usa " + hbSelecionada.getNome() + "!");
                            this->aplicarHoT(hbSelecionada.getNome(), hbSelecionada.getValorBase(), hbSelecionada.getDuracaoEfeito());
                        }      
                        else if (hbSelecionada.getTipo() == TipoHabilidade::SUPORTE) {
                            InterfaceJogo::exibirTexto("\n💪 [BUFF] " + this->getNome() + " foca suas intenções com " + hbSelecionada.getNome() + "!");
                            this->aplicarBuff(hbSelecionada.getNome(), hbSelecionada.getAtributoAlvo(), hbSelecionada.getValorBase(), hbSelecionada.getDuracaoEfeito());
                        }
                        else if (hbSelecionada.getTipo() == TipoHabilidade::DEBUFF) {
                            InterfaceJogo::exibirTexto("\n💀 [DEBUFF] " + this->getNome() + " prejudica o oponente usando " + hbSelecionada.getNome() + "!");
                            alvo.aplicarDebuff(hbSelecionada.getNome(), hbSelecionada.getAtributoAlvo(), hbSelecionada.getValorBase(), hbSelecionada.getDuracaoEfeito());
                        }
                        else if (hbSelecionada.getTipo() == TipoHabilidade::ULTRA) {
                            InterfaceJogo::exibirTexto("\n⚡💥 [ATAQUE ULTRA] " + this->getNome() + " libera todo o potencial com " + hbSelecionada.getNome() + "!");
                            alvo.receberDano(hbSelecionada.getValorBase(), hbSelecionada.getTipo());
                        }
                        else {
                            InterfaceJogo::exibirTexto("\n🔮 [ATAQUE ESPECIAL] " + this->getNome() + " usa " + hbSelecionada.getNome() + "!");
                            alvo.receberDano(hbSelecionada.getValorBase(), hbSelecionada.getTipo());
                        }
                        
                        acaoRealizada = true; 
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
                if (this->_energia >= 6) {
                    this->usarEscudo();
                    InterfaceJogo::exibirTexto("\n🛡️  [POSTURA] " + this->getNome() + " ergue o escudo se preparando para o pior!");
                    acaoRealizada = true;
                } else {
                    InterfaceJogo::exibirTexto("[ERRO] Energia insuficiente!");
                }
                break;

            case 4: // 🧪 USAR FRASCO DE CURA
                if (this->_frascos > 0) {
                    this->_frascos--; 
                    InterfaceJogo::exibirTexto("\n🧪 [CONSUMÍVEL] " + this->getNome() + " bebe rapidamente de seu frasco!");
                    this->receberCura(_hpMax * (0.4));
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
    _energia = std::min(_energiaMax, _energia + 8); 
    _mp = std::min(_mpMax, _mp + 2);                
    InterfaceJogo::exibirTexto("> Turno iniciado. Você recuperou 5 de Energia e 1 de MP.");
}

void Aventureiro::consumirMP(int qtd) { 
    if ((_mp - qtd) < 0) { _mp = 0; }
    else { _mp -= qtd; }    
}

void Aventureiro::consumirEnergia(int qtd) {     
    if ((_energia - qtd) < 0) { _energia = 0; }
    else { _energia -= qtd; }    
}

void Aventureiro::dormir() {
    _hp = _hpMax;           
    _mp = _mpMax;           
    _energia = _energiaMax; 
    _frascos = 3;           
    InterfaceJogo::exibirTexto("\n[RECONFORTO] Você descansa profundamente. Seus recursos foram totalmente restaurados!");
}

// ============================================================================
// MÉTODO PRIVADO
// ============================================================================

void Aventureiro::subirNivel() {
    this->_nivel++;
    this->_hpMax += 40; 
    this->_forcaBase += 16; 
    this->_defesaBase += 7; 

    this->_hp = this->_hpMax; 

    InterfaceJogo::exibirTexto("\n✨ SEU NÍVEL AUMENTOU PARA O NÍVEL " + std::to_string(this->_nivel) + "! ✨");

    switch (this->_nivel) {
        case 2: {
            Habilidade socoForte("Soco Forte", TipoHabilidade::FISICO, 35, 0, 23*_nivel, "", 0);
            this->adicionarHabilidade(socoForte);
            InterfaceJogo::exibirTexto("[NOVA HABILIDADE] Você aprendeu: Soco Forte!");

            Habilidade usarDipirona("Usar Dipirona", TipoHabilidade::HOT, 0, 10, 15*(1 + (0.7*_nivel)), "", 3);
            this->adicionarHabilidade(usarDipirona);
            InterfaceJogo::exibirTexto("[NOVA HABILIDADE] Você aprendeu: Usar Dipirona!");
            break;
        }

        case 3: {
            Habilidade perfurar("Perfurar", TipoHabilidade::DOT, 40, 5, _nivel* 30, "", 3);
            this->adicionarHabilidade(perfurar);
            InterfaceJogo::exibirTexto("[NOVA HABILIDADE] Você aprendeu: Perfurar!");

            Habilidade sprayDePimenta("Usar Spray de Pimenta", TipoHabilidade::DEBUFF, 0, 7, ((_nivel*0.5)+1) *15, "defesa", 2);
            this->adicionarHabilidade(sprayDePimenta);
            InterfaceJogo::exibirTexto("[NOVA HABILIDADE] Você aprendeu: Usar Spray de Pimenta!");
            break;           
        }

        case 4: {
            Habilidade aplicarAdrenalina("Aplicar Adrenalina", TipoHabilidade::CURA, 0, 8, (1+(0.6*_nivel))* 50, "", 0);
            this->adicionarHabilidade(aplicarAdrenalina);
            InterfaceJogo::exibirTexto("[NOVA HABILIDADE] Você aprendeu: Aplicar Adrenalina!");

            Habilidade focar("Focar", TipoHabilidade::SUPORTE, 25, 10, 12*_nivel, "força", 3);
            this->adicionarHabilidade(focar);
            InterfaceJogo::exibirTexto("[NOVA HABILIDADE] Você aprendeu: Focar!");
            break;
        }
        case 5: {
            Habilidade ataqueMental("ATAQUE MENTAL", TipoHabilidade::ULTRA, 60, 30, 65*_nivel, "", 0);
            this->adicionarHabilidade(ataqueMental);
            InterfaceJogo::exibirTexto("[NOVA HABILIDADE] Você aprendeu: ATAQUE MENTAL!");
            break;
        }
        default:
            break;
    }
}

// ============================================================================
// MÉTODOS DE PROGRESSÃO E BUFFS
// ============================================================================

void Aventureiro::ganharExperiencia(int qtd) {
    if (qtd <= 0) return;

    this->_xp += qtd;
    
    while (this->_xp >= _xpProxNivel) {
        this->_xp -= _xpProxNivel; 
        this->subirNivel();              
        _xpProxNivel = this->_nivel * 60; 
    }
}

void Aventureiro::buffArma(int valor) { 
    if (valor == 0) return;
    _bonusArma += valor;
}

void Aventureiro::buffArmadura(int valor) { 
    if (valor == 0) return;
    _bonusArmadura += valor;
}

std::string Aventureiro::getDeclaracaoStatus() const {
    std::string status = "==================================================\n";
    status += "  STATUS DE " + this->getNome() + " [Nível " + std::to_string(this->getNivel()) + "]\n";
    status += "==================================================\n";
    
    status += "  ❤️ HP       : " + std::to_string(this->getHP()) + " / " + std::to_string(this->getHPMax()) + "\n";
    status += "  ⚡ Energia  : " + std::to_string(this->_energia) + " / " + std::to_string(this->_energiaMax) + "\n";
    status += "  🔮 MP       : " + std::to_string(this->_mp) + " / " + std::to_string(this->_mpMax) + "\n";
    
    status += "--------------------------------------------------\n";
    
    status += "  ⚔️ Força    : " + std::to_string(this->_forcaBase) + " (+ " + std::to_string(this->_bonusArma) + " Arma)\n";
    status += "  🛡️ Defesa   : " + std::to_string(this->_defesaBase) + " (+ " + std::to_string(this->_bonusArmadura) + " Armadura)\n";
    
    status += "--------------------------------------------------\n";
    
    status += "  🧪 Frascos  : " + std::to_string(this->_frascos) + " restantes\n";
    status += "  ✨ XP       : " + std::to_string(this->_xp) + " / " + std::to_string(this->_xpProxNivel) + "\n";
    status += "==================================================";

    return status; 
}

// ============================================================================
// GETTERS E SETTERS DE ESTADO
// ============================================================================

int Aventureiro::getForcaTotal() const { 
    int forcaComEfeitos = Personagem::getForcaTotal();
    return forcaComEfeitos + _bonusArma;
}

int Aventureiro::getDefesa() const {
    int defesaComEfeitos = Personagem::getDefesa(); 
    return defesaComEfeitos + (_bonusArmadura); 
}

int Aventureiro::getIDCheckpoint() const { return _idCenaCheckpoint; }

void Aventureiro::setIDCheckpoint(int idCena) {
    if (idCena > 0) {
        this->_idCenaCheckpoint = idCena;
    }
}

int Aventureiro::getMP() const { return _mp; }
int Aventureiro::getMPMax() const { return _mpMax; }
int Aventureiro::getEnergia() const { return _energia; }
int Aventureiro::getEnergiaMax() const { return _energiaMax; }