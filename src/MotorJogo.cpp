#include "MotorJogo.hpp"
#include "Inimigo.hpp"
#include "InimigoComum.hpp"
#include "InimigoIncomum.hpp"
#include "InimigoBoss.hpp"
#include "Combate.hpp"

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

        // Verifica se a escolha gera combate
        if (escolhaFeita.geraCombate) {
            if (!realizarCombate(escolhaFeita.tipoInimigo)) {
                processarDerrota();
                continue; // volta ao início do loop na cena do checkpoint
            }
        }

        // Verifica se a escolha concede item
        checarEventosEspeciais(escolhaFeita);

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

    std::unordered_map<int, std::string> nomesItens = {
    {101, "Cartão do Tyler Durden"},
    {102, "Sabonete Caseiro"},
    {103, "Kit de Primeiros Socorros"}
    };

    // =========================================================================
    // CENA 1: A Exaustão Inicial
    // =========================================================================
    Cena cena1(1,
        "\n==================================================\n"
        "                   INSÔNIA CRÔNICA            \n"
        "==================================================\n"
        "Faz semanas que você não dorme de verdade. Os dias se \n"
        "misturam em reuniões, fotocópias e um vazio que nenhum \n"
        "trabalho preenche. Você sente que precisa de alguma saída \n"
        "antes que a exaustão te apague por completo.\n"
        "--------------------------------------------------", true);

    Escolha escolha1A;
    escolha1A.texto       = "Procurar um grupo de apoio só para conseguir dormir";
    escolha1A.destinoID   = 2;
    escolha1A.geraCombate = false;
    escolha1A.tipoInimigo = "";
    escolha1A.itemGanhoID = 0;

    Escolha escolha1B;
    escolha1B.texto       = "Afogar as mágoas em um bar barato no fim da rua";
    escolha1B.destinoID   = 3;
    escolha1B.geraCombate = true;
    escolha1B.tipoInimigo = "TrabalhadorNoturno";
    escolha1B.itemGanhoID = 0;

    cena1.adicionarEscolha(escolha1A);
    cena1.adicionarEscolha(escolha1B);
    _roteiro.insert(std::make_pair(1, cena1));

    // =========================================================================
    // CENA 2: O Grupo de Apoio (Checkpoint)
    // =========================================================================
    Cena cena2(2,
        "\n==================================================\n"
        "                   TURISTAS DE LUTO          \n"
        "==================================================\n"
        "No porão de uma igreja, em um círculo de estranhos, você \n"
        "finalmente sente algo parecido com paz. Lá conhece Marla \n"
        "Singer, outra turista de luto que finge sofrer doenças que \n"
        "não tem. Por uma noite, ao menos, você consegue dormir.\n"
        "--------------------------------------------------", true);

    Escolha escolha2A;
    escolha2A.texto       = "Voltar para a rotina vazia, ainda sem rumo";
    escolha2A.destinoID   = 3;
    escolha2A.geraCombate = false;
    escolha2A.tipoInimigo = "";
    escolha2A.itemGanhoID = 0;

    cena2.adicionarEscolha(escolha2A);
    _roteiro.insert(std::make_pair(2, cena2));

    // =========================================================================
    // CENA 3: O Estranho do Avião
    // =========================================================================
    Cena cena3(3,
        "\n==================================================\n"
        "                   ENCONTRO ACASAL            \n"
        "==================================================\n"
        "Em um voo de trabalho você conhece Tyler Durden: carismático, \n"
        "imprevisível, dono de uma filosofia perigosa sobre liberdade e \n"
        "destruição. Ao desembarcar, descobre que sua mala foi destruída \n"
        "e sua casa, misteriosamente, explodiu. Tyler oferece um lugar \n"
        "para ficar — com uma condição.\n"
        "--------------------------------------------------", false);

    Escolha escolha3A;
    escolha3A.texto       = "Aceitar o convite de Tyler e ligar para ele";
    escolha3A.destinoID   = 4;
    escolha3A.geraCombate = false;
    escolha3A.tipoInimigo = "";
    escolha3A.itemGanhoID = 101; 

    _player.buffArma(10);

    cena3.adicionarEscolha(escolha3A);
    _roteiro.insert(std::make_pair(3, cena3));

    // =========================================================================
    // CENA 4: O Bar e o Convite (Checkpoint)
    // =========================================================================
    Cena cena4(4,
        "\n==================================================\n"
        "                 ME BATA O MAIS FORTE         \n"
        "==================================================\n"
        "Vocês se encontram em um bar decadente. Depois de algumas \n"
        "cervejas, Tyler faz um pedido estranho: que você o acerte com \n"
        "toda a força que tiver. No estacionamento, sob a luz fraca de \n"
        "um poste, a primeira luta do que viria a ser o Fight Club \n"
        "começa sem regras e sem motivo — só pela sensação de estar vivo.\n"
        "--------------------------------------------------", true);

    Escolha escolha4A;
    escolha4A.texto       = "Acertar o primeiro golpe e aceitar a briga";
    escolha4A.destinoID   = 5;
    escolha4A.geraCombate = true;
    escolha4A.tipoInimigo = "DesafianteDoBar";
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

     // =========================================================================
    // BANCO DE DADOS DE ITENS (Tabelas de Mapeamento)
    // =========================================================================

    // 1. Catálogo de Nomes: Associa o ID do item ao nome que aparecerá na tela
    const std::unordered_map<int, std::string> nomesItens = {
        {101, "Cartão do Tyler Durden"},
        {102, "Colete de Segurança"},
        {103, "Soco Ingles"},
        {104, "Porrete"},
        {105, "Faca"}
    };

    using AcaoBuff = std::function<void(Aventureiro&, int)>;

    // Agora o mapa guarda: { Função, Valor do Buff, Nome do Atributo }
    const std::unordered_map<int, std::tuple<AcaoBuff, int, std::string>> tabelaDeBuffs = {
        {101, {&Aventureiro::buffArma, 5, "Força"}},       
        {102, {&Aventureiro::buffArmadura, 3, "Defesa"}},   
        {103, {&Aventureiro::buffArma, 8, "Força"}},   
        {104, {&Aventureiro::buffArma, 9, "Força"}},       
        {105, {&Aventureiro::buffArma, 15, "Força"}}        
    };

int idItem = escolha.itemGanhoID;
    
    if (idItem <= 0) return;

    // 1. Busca o nome do item
    std::string nomeItem = "Item Desconhecido";
    auto itNome = nomesItens.find(idItem);
    if (itNome != nomesItens.end()) {
        nomeItem = itNome->second;
    }

    // 2. Busca e aplica o buff dinamicamente
    auto itBuff = tabelaDeBuffs.find(idItem);
    if (itBuff != tabelaDeBuffs.end()) {
        // Extrai os elementos da tupla
        auto acao = std::get<0>(itBuff->second);
        int valor = std::get<1>(itBuff->second);
        std::string tipoBuff = std::get<2>(itBuff->second); // "Força" ou "Defesa"
        
        // Exibe a mensagem customizada mostrando onde o bônus foi aplicado
        _interface.exibirTexto("\n[ITEM OBTIDO]: " + nomeItem + " (+" + std::to_string(valor) + " de " + tipoBuff + ")!");
        
        // Executa a função de buff no jogador
        acao(_player, valor);
    } else {
        _interface.exibirTexto("\n[ITEM OBTIDO]: " + nomeItem + "!");
    }
}