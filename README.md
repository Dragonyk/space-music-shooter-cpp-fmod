# 🚀 Space Shooter Rhythm

**Space Shooter Rhythm** é um jogo 2D desenvolvido em **C++**, utilizando **OpenGL** e **FMOD**. Ele combina jogabilidade de tiro espacial com sincronização de elementos visuais e de jogo com a música de fundo. Controle sua nave, desvie de asteroides, destrua inimigos e sinta a batida da ação!

---

## 🎮 Características

- Movimentação fluida do jogador com teclado
- Disparo com o mouse sincronizado ao ritmo
- Sistema de partículas e explosões
- Colisões com inimigos, projéteis e asteroides
- Feedback visual da música usando espectros com FMOD
- Modo tela cheia ativado pela tecla `P`
- Sistema de invulnerabilidade temporária ao ser atingido
- Interface responsiva e adaptativa ao redimensionamento

---

---

## 🕹️ Controles

| Tecla / Ação | Função                    |
|--------------|----------------------------|
| `W` `A` `S` `D` | Mover a nave (cima, esquerda, baixo, direita) |
| Clique esquerdo do mouse | Atirar |
| Movimento do mouse       | Direcionar o tiro |
| `P`                      | Alternar tela cheia |

---

## 🔧 Requisitos

Antes de compilar, certifique-se de ter:

- **Compilador C++ compatível (Visual Studio recomendado)**
- **Bibliotecas OpenGL instaladas (GL, GLU, GLUT)**
- **FMOD Studio API baixada e configurada**

> 💡 Dica: Use o Visual Studio Community com suporte à C++ Desktop Development.

---

## 🛠️ Compilação no Windows

1. Baixe e instale o [Visual Studio Community](https://visualstudio.microsoft.com/pt-br/vs/community/).
2. Baixe a **FMOD Studio API**: [fmod.com/download](https://www.fmod.com/download)
3. Crie um novo projeto em C++ no Visual Studio.
4. Adicione todos os arquivos `.cpp` e `.h` ao projeto.
5. Configure os diretórios:
   - **Include Directories**: adicione a pasta `fmod/include`
   - **Library Directories**: adicione a pasta `fmod/lib`
6. Linke a biblioteca:
   - Vá em `Project Properties > Linker > Input` e adicione: `fmod_vc.lib`
7. Certifique-se de colocar o `fmod.dll` (32 ou 64 bits) na mesma pasta do `.exe` do jogo.
8. Compile e execute!

---

## 🎵 Personalização da Trilha Sonora

Para usar uma música personalizada:

1. Coloque um arquivo `.mp3` na pasta do projeto.
2. No arquivo `main.cpp`, substitua o valor da variável:

```cpp
const char* music_name = "sua_musica.mp3";
