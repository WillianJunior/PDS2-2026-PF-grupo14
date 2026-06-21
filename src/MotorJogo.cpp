#include "MotorJogo.hpp"
#include "Inimigo.hpp"
#include "InimigoComum.hpp"
#include "InimigoIncomum.hpp"
#include "InimigoBoss.hpp"
#include "Combate.hpp"
#include <stdexcept>
#include <memory>

// ============================================================================
// CONSTRUTOR
// ============================================================================

MotorJogo::MotorJogo(Aventureiro& player)
    : _player(player), _idCenaAtual(1) {
    inicializarHistoria();
}

// ============================================================================
// MÉTODOS PÚBLICOS
// ============================================================================

void MotorJogo::rodar() {
    bool jogoRodando = true;

    while (jogoRodando) {

        // Programação defensiva: valida ID antes de acessar o mapa
        if (_roteiro.find(_idCenaAtual) == _roteiro.end()) {
            throw std::runtime_error(
                "MotorJogo::rodar — ID de cena inválido: "
                + std::to_string(_idCenaAtual));
        }

        const Cena& cenaAtual = _roteiro.at(_idCenaAtual);
        _interface.exibirTexto(cenaAtual.getTexto());

        // Marca checkpoint se a cena for ponto de salvamento
        if (cenaAtual.isCheckpoint()) {
            _player.setIDCheckpoint(_idCenaAtual);
        }

        const std::vector<Escolha>& opcoes = cenaAtual.getEscolhas();

        // Sem escolhas = cena final, encerra o loop
        if (opcoes.empty()) {
            jogoRodando = false;
            break;
        }

        // Monta lista de textos para exibir ao jogador
        std::vector<std::string> textosDasOpcoes;
        for (const auto& escolha : opcoes) {
            textosDasOpcoes.push_back(escolha.texto);
        }

        const Escolha& escolhaFeita = opcoes.at((_interface.solicitarEscolha(textosDasOpcoes)-1));

        // Verifica se a escolha concede item
        checarEventosEspeciais(escolhaFeita);

        // Verifica se a escolha gera combate
        if (escolhaFeita.geraCombate) {
            if (!realizarCombate(escolhaFeita.tipoInimigo)) {
                processarDerrota();
                continue; // volta ao início do loop na cena do checkpoint
            }
        }

        // Avança para a próxima cena
        _idCenaAtual = escolhaFeita.destinoID;
    }
}

bool MotorJogo::realizarCombate(std::string tipoInimigo) {
    // Programação defensiva: rejeita strings inválidas antes de instanciar
    if (tipoInimigo.empty()) {
        throw std::invalid_argument(
            "MotorJogo::realizarCombate — tipoInimigo não pode ser vazio.");
    }

    // RAII: unique_ptr garante desalocação automática sem new/delete manual
    std::unique_ptr<Inimigo> inimigoAtual = criarInimigo(tipoInimigo);

    _interface.exibirTexto("Um " + tipoInimigo + " selvagem apareceu!");

    Combate combate(_player, *inimigoAtual);
    return combate.iniciar();
}

// ============================================================================
// MÉTODOS PRIVADOS
// ============================================================================

std::unique_ptr<Inimigo> MotorJogo::criarInimigo(const std::string& tipoInimigo) {
    if (tipoInimigo == "DesafianteDoBar")
        return std::make_unique<DesafianteDoBar>("Desafiante do Bar", 1);
    if (tipoInimigo == "TrabalhadorNoturno")
        return std::make_unique<TrabalhadorNoturno>("Trabalhador Noturno", 1);
    if (tipoInimigo == "SegurancaDeBalada")
        return std::make_unique<SegurancaDeBalada>("Segurança de Balada", 1);
    if (tipoInimigo == "RecrutaDoutrinado")
        return std::make_unique<RecrutaDoutrinado>("Recruta Doutrinado", 2);
    if (tipoInimigo == "GuardaPatrimonial")
        return std::make_unique<GuardaPatrimonial>("Guarda Patrimonial", 2);
    if (tipoInimigo == "SabotadorRival")
        return std::make_unique<SabotadorRival>("Sabotador Rival", 2);
    if (tipoInimigo == "TylerDurden")
        return std::make_unique<TylerDurden>("Tyler Durden", 5);

    throw std::invalid_argument(
        "MotorJogo::criarInimigo — tipo desconhecido: " + tipoInimigo);
}

void MotorJogo::inicializarHistoria() {
    // ==========================================
    // CENA 1: A Taverna (Nó Inicial)
    // ==========================================
    Cena cena1(1,
        "Você acorda em uma taverna escura. O estalajadeiro aponta para a porta "
        "dos fundos e diz que há problemas nos arredores. O que você faz?", false);

    Escolha escolha1A;
    escolha1A.texto      = "Sair pela porta dos fundos em direção ao beco";
    escolha1A.destinoID  = 2;
    escolha1A.geraCombate = true;
    escolha1A.tipoInimigo = "DesafianteDoBar";
    escolha1A.itemGanhoID = 0;

    Escolha escolha1B;
    escolha1B.texto      = "Conversar com o estalajadeiro e pedir suprimentos";
    escolha1B.destinoID  = 3;
    escolha1B.geraCombate = false;
    escolha1B.tipoInimigo = "";
    escolha1B.itemGanhoID = 101;

    cena1.adicionarEscolha(escolha1A);
    cena1.adicionarEscolha(escolha1B);
    _roteiro.insert(std::make_pair(1, cena1));

    // ==========================================
    // CENA 2: O Beco (Pós-combate)
    // ==========================================
    Cena cena2(2,
        "Após vencer o desafiante no beco lamacento, você avista os portões "
        "da Vila Segura logo à frente.", false);

    Escolha escolha2A;
    escolha2A.texto      = "Correr em direção aos portões da vila";
    escolha2A.destinoID  = 3;
    escolha2A.geraCombate = false;
    escolha2A.tipoInimigo = "";
    escolha2A.itemGanhoID = 0;

    cena2.adicionarEscolha(escolha2A);
    _roteiro.insert(std::make_pair(2, cena2));

    // ==========================================
    // CENA 3: Vila Segura (Checkpoint)
    // ==========================================
    Cena cena3(3,
        "Você entra na Vila Segura. O ambiente é calmo e os guardas protegem "
        "o local. Seu progresso foi salvo!", true);

    Escolha escolha3A;
    escolha3A.texto      = "Seguir viagem em direção ao covil do chefe";
    escolha3A.destinoID  = 4;
    escolha3A.geraCombate = false;
    escolha3A.tipoInimigo = "";
    escolha3A.itemGanhoID = 0;

    cena3.adicionarEscolha(escolha3A);
    _roteiro.insert(std::make_pair(3, cena3));

    // ==========================================
    // CENA 4: O Covil (Combate Final)
    // ==========================================
    Cena cena4(4,
        "Você chega ao covil. Uma figura imponente bloqueia o caminho.", false);

    Escolha escolha4A;
    escolha4A.texto      = "Enfrentar Tyler Durden";
    escolha4A.destinoID  = 5;
    escolha4A.geraCombate = true;
    escolha4A.tipoInimigo = "TylerDurden";
    escolha4A.itemGanhoID = 0;

    cena4.adicionarEscolha(escolha4A);
    _roteiro.insert(std::make_pair(4, cena4));

    // ==========================================
    // CENA 5: Fim de Jogo
    // ==========================================
    Cena cena5(5,
        "Você chega ao topo da montanha e avista o horizonte. "
        "A jornada termina aqui por enquanto!", false);

    // Sem escolhas — encerra o loop em rodar()
    _roteiro.insert(std::make_pair(5, cena5));
}

void MotorJogo::processarDerrota() {
    _interface.exibirTexto(
        "\nVocê foi derrotado! Retornando ao último checkpoint...");

    // Restaura recursos do jogador
    _player.dormir();

    // Retorna à última cena segura
    _idCenaAtual = _player.getIDCheckpoint();
}

void MotorJogo::checarEventosEspeciais(const Escolha& escolha) {
    // Por ora só trata item — expandir quando Aventureiro expor mais métodos
    if (escolha.itemGanhoID > 0) {
        _interface.exibirTexto(
            "Você obteve um item! (ID: "
            + std::to_string(escolha.itemGanhoID) + ")");
        _player.buffArma(5); // bônus simbólico até sistema de itens ser implementado
    }
}