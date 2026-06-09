#include "MotorJogo.hpp"
#include "Inimigo.hpp"
#include "Combate.hpp"

// ============================================================================
// CONSTRUTOR (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

MotorJogo::MotorJogo(Aventureiro& player) 
    : _player(player),_idCenaAtual(1) {
        inicializarHistoria();
    }

// ============================================================================
// MÉTODOS PÚBLICOS (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

void MotorJogo::rodar() {
    bool jogoRodando = true;
    while(jogoRodando){
        Cena CenaAtual = _roteiro.at(_idCenaAtual);
        _interface.exibirTexto(CenaAtual.getTexto());
        if(CenaAtual.isCheckpoint()){
            _player.setIDCheckpoint(_idCenaAtual);
        }
        const std::vector<Escolha>& opcoes = CenaAtual.getEscolhas();
        if(opcoes.empty()){
            jogoRodando = false;
            break;
        }
        std::vector<std::string> textosDasOpcoes;
        for (const auto& escolha : opcoes) {
            textosDasOpcoes.push_back(escolha.texto); 
        }
    }
}

bool MotorJogo::realizarCombate(std::string tipoInimigo) { (void)tipoInimigo; return false; }

// ============================================================================
// MÉTODOS PRIVADOS (Estilo Esqueleto/TDD Red Compacto)
// ============================================================================

void MotorJogo::inicializarHistoria(){
    // ==========================================
    // CENA 1: O Início (Nó Inicial)
    // ==========================================
    Cena cena1(1, "Você acorda em uma taverna escura. O estalajadeiro aponta para a porta dos fundos e diz que há problemas nos arredores. O que você faz?", false);

    // Escolha A: Leva para uma emboscada (Gera Combate com Goblin)
    Escolha escolha1A;
    escolha1A.texto = "Sair pela porta dos fundos em direção ao beco";
    escolha1A.destinoID = 2;
    escolha1A.geraCombate = true;
    escolha1A.tipoInimigo = "Goblin";
    escolha1A.itemGanhoID = 0; // Nenhum item

    // Escolha B: Caminho seguro com recompensa (Ganha item e vai para a Vila)
    Escolha escolha1B;
    escolha1B.texto = "Conversar com o estalajadeiro e pedir suprimentos";
    escolha1B.destinoID = 3;
    escolha1B.geraCombate = false;
    escolha1B.tipoInimigo = "";
    escolha1B.itemGanhoID = 101; // Ganha uma Poção de Vida (ID 101)

    cena1.adicionarEscolha(escolha1A);
    cena1.adicionarEscolha(escolha1B);
    
    // Insere no mapa do jogo
    _roteiro.insert_or_assign(1, cena1);


    // ==========================================
    // CENA 2: O Beco Com Combate (Cena de Transição)
    // ==========================================
    Cena cena2(2, "Após derrotar o Goblin no beco lamacento, você avista os portões da Vila Segura logo à frente.", false);

    Escolha avançarBeco;
    avançarBeco.texto = "Correr em direção aos portões da vila";
    avançarBeco.destinoID = 3; // Encontra o outro caminho na Vila Segura
    avançarBeco.geraCombate = false;
    avançarBeco.tipoInimigo = "";
    avançarBeco.itemGanhoID = 0;

    cena2.adicionarEscolha(avançarBeco);
    _roteiro.insert_or_assign(2, cena2);


    // ==========================================
    // CENA 3: A Vila Segura (Cena de Checkpoint)
    // ==========================================
    // Terceiro parâmetro 'true' ativa o Checkpoint!
    Cena cena3(3, "Você entra na Vila Segura. O ambiente é calmo e os guardas protegem o local. Seu progresso foi salvo!", true);

    Escolha continuarJornada;
    continuarJornada.texto = "Seguir viagem em direção à toca do Dragão";
    continuarJornada.destinoID = 4;
    continuarJornada.geraCombate = false;
    continuarJornada.tipoInimigo = "";
    continuarJornada.itemGanhoID = 0;

    cena3.adicionarEscolha(continuarJornada);
    _roteiro.insert_or_assign(3, cena3);


    // ==========================================
    // CENA 4: O Covil (Cena Final / Fim de Jogo)
    // ==========================================
    // Cena sem escolhas adicionadas limpa o loop e encerra a execução no método rodar()
    Cena cena4(4, "Você chega ao topo da montanha e avista o horizonte. A prévia da sua jornada termina aqui por enquanto!", false);
    
    _roteiro.insert_or_assign(4, cena4);;
}

void MotorJogo::processarDerrota() {}