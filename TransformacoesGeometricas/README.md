Atividade 2

# Compilação
Cada exercício está separado em um arquivo .cpp e todos utilizam o header "tvector.h" que está implementado em tvector.cpp (linkado como uma biblioteca estática pela Solução EstudosOpenGL).
O arquivo de cada exercício possui o nome {i}_{Exercicio}.cpp, sendo "i" o número do exercício e "Exercicio" o nome referente.

# Instruções por exercício

## 1. Transformação de Escala Uniforme
Faça um programa que desenhe um triângulo e aplique transformação geométrica de escala uniforme conforme o seguinte:

* Aumentar o tamanho após clicar no botão esquerdo do mouse

* Diminur o tamanho após clicar no botão direito do mouse

## 2. Transformação de Rotação
Faça um programa que desenhe um quadrado ou retângulo com a primitiva GL_TRIANGLE_STRIP. Em seguida, aplica a transformação geométrica de rotação conforme o seguinte:

* Rotacionar para esquerda após clicar na tecla seta esquerda

* Rotacionar para direita após clicar na tecla seta direita

## 3. Transformação de Translação
Faça um programa que desenho um círculo e aplica a transformação geométrica de translação conforme o seguinte:

Mover para cima, baixo, esquerda e direita por meio das teclas W, S, A e D, respectivamente.

## 4. Transformação Dupla
Faça um programa que desenhe dois objetos diferentes na tela (exemplo: triângulo e quadrado). Aplique uma transformação diferente para cada objeto.

Para aplicar as transformações é necessário usar as teclas 'w', 'a', 's' e 'd'.
O círculo sofrerá transformações de translação e o quadrado de escala.

## 5. Todas as Transformações
Faça um programa que desenhe um objeto de sua preferência e aplique as três transformações geométricas (translação, rotação e escala) a partir de eventos de teclado e mouse (de sua preferência), conforme o seguinte:

Construa uma única matriz de transformação a partir da multiplicação das matrizes anteriores.

Para translação foram usadas as teclas 'w', 'a', 's' e 'd';
Para rotação foram usadas as teclas 'q' e 'e';
Para escala foi usado o "scroll" do mouse;