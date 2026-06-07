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
    int danoFinal = dano - getDefesa();
    if (danoFinal < 0) danoFinal = 0;
    
    _hp -= danoFinal;
    if (_hp < 0) _hp = 0;
}

void Personagem::receberCura(int valor) {
    _hp += valor;
    if (_hp > _hpMax) _hp = _hpMax;
}

void Personagem::aplicarDoT(std::string nome, int dano, int duracao){(void)nome;(void)dano;(void)duracao;}

void Personagem::processarEfeitosContinuos() {
    // SE O PERSONAGEM JÁ MORREU OU NÃO TEM NENHUM DOT NA LISTA:
    // Sai da função imediatamente para economizar processamento.
    if (!this->_vivo || this->_dotsAtivos.empty()) return;

    // CRIAMOS UM ITERADOR (it) PARA VARRER O VETOR DE DOTS:
    // Usamos um laço 'while' em vez de um 'for' comum porque vamos deletar elementos 
    // de dentro do vetor enquanto andamos por ele, e o 'while' nos dá controle total disso.
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
    return _vivo;
}

std::string Personagem::getNome() const {
    return _nome;
}

int Personagem::getNivel() const {
    return _nivel;
}