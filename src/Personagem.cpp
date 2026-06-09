#include "Personagem.hpp"
#include "InterfaceJogo.hpp"
#include <stdexcept>

// ============================================================================
// CONSTRUTOR
// ============================================================================

Personagem::Personagem(std::string nome, int hp, int defesa, int forca, int nivel) {

    if (nome.empty()) {
        throw std::invalid_argument("Erro: O nome do personagem nao pode ser vazio.");
    }
    if (hp <= 0) {
        throw std::invalid_argument("Erro: Pontos de vida (HP) devem ser maiores que zero.");
    }
    if (defesa <= 0) {
        throw std::invalid_argument("Erro: A defesa base deve ser maior que zero.");
    }
    if (forca <= 0) {
        throw std::invalid_argument("Erro: A forca base deve ser maior que zero.");
    }
    if (nivel <= 0) {
        throw std::invalid_argument("Erro: O nivel inicial deve ser maior que zero.");
    }
    _nome = nome;
    _hp = hp;
    _hpMax = hp;
    _defesaBase = defesa;
    _forcaBase = forca;
    _nivel = nivel;
}

// ============================================================================
// GERENCIAMENTO DE HABILIDADES
// ============================================================================

void Personagem::adicionarHabilidade(const Habilidade& habilidade) {
    _habilidades.push_back(habilidade);
}

const std::vector<Habilidade>& Personagem::getHabilidades() const {
    return _habilidades;
}

// ============================================================================
// SISTEMA DE COMBATE
// ============================================================================

void Personagem::receberDano(int dano, TipoHabilidade tipoDaHabilidade) {
 // Evita o processamento caso o golpe tenha valor nulo ou negativo
    if (dano <= 0) return;

    // 1. MITIGAÇÃO POR DEFESA (Fórmula percentual para amortecer o dano)
    float fatorMitigacao = 100.0f / (100.0f + this->_defesaBase);
    float danoCalculado = dano * fatorMitigacao;

    // 2. VERIFICAÇÃO DE ESCUDO ATIVO
    if (this->_escudoAtivo) {
        switch (tipoDaHabilidade) {
            case TipoHabilidade::ULTRA:
                // Ataques ULTRA perfuram a guarda completamente
                InterfaceJogo::exibirTexto("[PERIGO] O ataque é do tipo ULTRA! O escudo foi completamente ignorado!");
                break;

            case TipoHabilidade::DOT:
                // Efeitos contínuos agem de forma interna (veneno, sangramento), ignorando a barreira física
                InterfaceJogo::exibirTexto("[EFEITO] O dano contínuo (DoT) agiu por baixo do escudo!");
                break;

            default:
                // Ataques comuns (Físico, Especial) têm seu dano cortado pela metade
                danoCalculado *= 0.5f;
                InterfaceJogo::exibirTexto(this->getNome() + " posicionou o escudo e mitigou metade do impacto!");
                break;
        }
    }

    // 3. CONVERSÃO PARA INTEIRO E GARANTIA DE DANO MÍNIMO
    int danoFinal = static_cast<int>(danoCalculado);
    
    // Se o dano bruto era positivo, o dano real deve ser de pelo menos 1 (evita dano zero se a defesa for muito alta)
    if (danoFinal <= 0 && dano > 0) {
        danoFinal = 1;
    }

    // 4. APLICAÇÃO DIRETAMENTE NO HP
    this->_hp -= danoFinal;

    // Feedback textual customizado dependendo de como o dano foi sofrido
    if (tipoDaHabilidade == TipoHabilidade::DOT) {
        InterfaceJogo::exibirTexto(this->getNome() + " sofreu " + std::to_string(danoFinal) + " de dano por efeito contínuo!");
    } else {
        InterfaceJogo::exibirTexto(this->getNome() + " recebeu " + std::to_string(danoFinal) + " de dano real!");
    }

    // 5. GERENCIAMENTO DO ESTADO VITAL (Morte)
    if (this->_hp <= 0) {
        this->_hp = 0;       // Trava de segurança para não exibir vida negativa no HUD
        this->_vivo = false;  // Disjuntor que desliga as ações deste personagem no jogo
    }
}

void Personagem::receberCura(int valor) {
    _hp += valor;
    if (_hp > _hpMax) _hp = _hpMax;
}

void Personagem::aplicarDoT(std::string nome, int dano, int duracao) {
    // 1. TRAVA DE SEGURANÇA
    // Se o personagem já estiver morto, não faz sentido aplicar um efeito de dano contínuo.
    if (!this->_vivo) return;

    // 2. CRIAÇÃO E INSTANCIAÇÃO DO EFEITO
    // Criamos uma variável do tipo 'EfeitoDoT' (a struct que declaramos no cabeçalho)
    // e preenchemos os seus campos com os dados que o método recebeu por parâmetro.
    EfeitoDoT novoDot;
    novoDot.tipo = TipoHabilidade::DOT; // Define o tipo obrigatoriamente como DOT para a lógica do escudo
    novoDot.danoPorTurno = dano;
    novoDot.turnosRestantes = duracao;
    novoDot.nomeEfeito = nome;

    // 3. PERSISTÊNCIA NA MEMÓRIA
    // Joga o efeito recém-criado para o final do vetor '_dotsAtivos'.
    // A partir deste momento, o personagem "lembrará" que está sob esse efeito.
    this->_dotsAtivos.push_back(novoDot);

    // 4. FEEDBACK VISUAL
    // Exibe na tela do jogo que o status foi aplicado com sucesso e por quanto tempo vai durar.
    InterfaceJogo::exibirTexto("[STATUS] " + this->getNome() + " foi afetado por " + nome + 
                               " (" + std::to_string(dano) + " de dano/turno por " + 
                               std::to_string(duracao) + " turnos)!");
}

void Personagem::processarEfeitosContinuos() {
    // SE O PERSONAGEM JÁ MORREU OU NÃO TEM NENHUM DOT NA LISTA:
    // Sai da função imediatamente para economizar processamento.
    if (!this->_vivo || this->_dotsAtivos.empty()) return;

    auto it = this->_dotsAtivos.begin();
    
    while (it != this->_dotsAtivos.end()) {
        
        // EXIBE NA TELA O FEEDBACK VISUAL DO RPG:
        // Diz ao jogador qual efeito está machucando o personagem naquele instante.
        InterfaceJogo::exibirTexto("\n[EFEITO ATIVO] " + it->nomeEfeito + " está agindo em " + this->getNome() + "!");
        
        // O GATILHO CRÍTICO:
        // Chamamos o método 'receberDano' original da classe. Passamos o valor guardado 
        // no DoT e avisamos que o tipo é 'DOT'. O 'receberDano' vai processar a defesa, 
        // ignorar o escudo e deduzir o HP do personagem bem aqui.
        this->receberDano(it->danoPorTurno, TipoHabilidade::DOT);
        
        // CONTROLE DE TEMPO:
        // Reduz em 1 o número de turnos que ainda restam para esse efeito sumir.
        it->turnosRestantes--;

        // SE O EFEITO CHEGOU AO FIM (ZEROU OS TURNOS):
        if (it->turnosRestantes <= 0) {
            // Avisa o jogador que o personagem se livrou daquele efeito.
            InterfaceJogo::exibirTexto("O efeito de " + it->nomeEfeito + " em " + this->getNome() + " expirou.");
            
            // REMOVE O DOT DO VETOR COM SEGURANÇA:
            // O 'erase' deleta o efeito da memória. Ele nos devolve a nova posição correta 
            // para onde o iterador 'it' deve apontar, evitando que o ponteiro aponte para o além (lixo).
            it = this->_dotsAtivos.erase(it); 
        } 
        // SE O EFEITO AINDA VAI DURAR MAIS TURNOS:
        else {
            // Apenas avança o iterador para analisar o próximo DoT da lista na próxima repetição.
            ++it; 
        }
    }
}



int Personagem::getDefesa() const {
    return _defesaBase;
}

// ============================================================================
// GETTERS
// ============================================================================

int Personagem::getHP() const {
    return _hp;
}

int Personagem::getHPMax() const {
    return _hpMax;
}

bool Personagem::estaVivo() const {
    return this->_hp > 0;
}

std::string Personagem::getNome() const {
    return _nome;
}

int Personagem::getNivel() const {
    return _nivel;
}