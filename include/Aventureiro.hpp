#ifndef AVENTUREIRO_HPP
#define AVENTUREIRO_HPP

#include "Personagem.hpp"
#include "Habilidade.hpp"
#include <vector>

/**
 * @class Aventureiro
 * @brief Representa o personagem controlado pelo jogador, com sistema de progressão e recursos.
 */
class Aventureiro : public Personagem {
public:
    Aventureiro(std::string nome, int hp, int defesa, int forca);

    /** @brief Sobrescreve o turno para permitir interação via menu. */
    void executarTurno(Personagem& alvo) override;
    
    /** @brief Implementa a lógica de dano considerando o estado do Escudo. */
    void receberDano(int dano) override;

    /** @brief Retorna defesa base + bônus de armadura. */
    int getDefesa() const override;

    /** @brief Recupera uma pequena parcela de MP e Energia no início do turno. */
    void recuperarRecursos();
    
    /** @brief Deduz pontos de Mana ao usar habilidades mágicas. */
    void consumirMP(int qtd);
    
    /** @brief Deduz pontos de Energia ao usar habilidades físicas ou defesa. */
    void consumirEnergia(int qtd);

    /** @brief Ativa o multiplicador de defesa para o próximo ataque. */
    void usarEscudo();
    
    /** @brief Tenta usar um frasco de cura (consome 1 de 3). */
    bool usarFrasco();
    
    /** @brief Restaura recursos totais e redefine o checkpoint. */
    void dormir();

    /** @brief Adiciona uma nova habilidade ao vetor de conhecimentos do herói. */
    void aprenderHabilidade(Habilidade hb);

    /** @brief Adiciona XP e verifica se houve level up. */
    bool ganharExperiencia(int qtd);

    /** @brief Incrementa permanentemente o bônus de dano. */
    void buffArma(int valor);

    /** @brief Incrementa permanentemente o bônus de defesa. */
    void buffArmadura(int valor);

    // Getters de estado
    int getForcaTotal() const;
    int getIDCheckpoint() const;
    const std::vector<Habilidade>& getHabilidades() const;
    int getMP() const;
    int getMPMax() const;
    int getEnergia() const;
    int getEnergiaMax() const;

    void setIDCheckpoint(int id); // Para atualizar quando passar por uma cena de save

private:
    int _energia, _energiaMax;
    int _mp, _mpMax;
    int _frascos;
    int _forcaBase;
    int _bonusArma;
    int _bonusArmadura;
    int _xp, _xpProxNivel;
    int _idCenaCheckpoint;
    bool _escudoAtivo;

    std::vector<Habilidade> _habilidadesConhecidas;

    /** @brief Aumenta atributos internos ao subir de nível. */
    void subirNivel();
};

#endif  