#ifndef HABILIDADE_HPP
#define HABILIDADE_HPP

#include <string>

/** @enum TipoHabilidade @brief Define a natureza do efeito da habilidade. */
enum class TipoHabilidade { FISICO, ESPECIAL, ULTRA, CURA };

/**
 * @class Habilidade
 * @brief Objeto que contém os metadados de uma ação especial.
 */
class Habilidade {
public:
    Habilidade(std::string nome, TipoHabilidade tipo, int custoE, int custoM, int valorBase);

    std::string getNome() const;
    TipoHabilidade getTipo() const;
    int getCustoEnergia() const;
    int getCustoMP() const;
    int getValorBase() const;

private:
    std::string _nome;
    TipoHabilidade _tipo;
    int _custoEnergia;
    int _custoMP;
    int _valorBase;
};

#endif