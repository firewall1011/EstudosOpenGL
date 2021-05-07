Atividade 2

# Compila��o
Cada exerc�cio est� separado em um arquivo .cpp e todos utilizam o header "tvector.h" que est� implementado em tvector.cpp (linkado como uma biblioteca est�tica pela Solu��o EstudosOpenGL).
O arquivo de cada exerc�cio possui o nome {i}_{Exercicio}.cpp, sendo "i" o n�mero do exerc�cio e "Exercicio" o nome referente.

# Instru��es por exerc�cio

## 1. Transforma��o de Escala Uniforme
Fa�a um programa que desenhe um tri�ngulo e aplique transforma��o geom�trica de escala uniforme conforme o seguinte:

* Aumentar o tamanho ap�s clicar no bot�o esquerdo do mouse

* Diminur o tamanho ap�s clicar no bot�o direito do mouse

## 2. Transforma��o de Rota��o
Fa�a um programa que desenhe um quadrado ou ret�ngulo com a primitiva GL_TRIANGLE_STRIP. Em seguida, aplica a transforma��o geom�trica de rota��o conforme o seguinte:

* Rotacionar para esquerda ap�s clicar na tecla seta esquerda

* Rotacionar para direita ap�s clicar na tecla seta direita

## 3. Transforma��o de Transla��o
Fa�a um programa que desenho um c�rculo e aplica a transforma��o geom�trica de transla��o conforme o seguinte:

Mover para cima, baixo, esquerda e direita por meio das teclas W, S, A e D, respectivamente.

## 4. Transforma��o Dupla
Fa�a um programa que desenhe dois objetos diferentes na tela (exemplo: tri�ngulo e quadrado). Aplique uma transforma��o diferente para cada objeto.

Para aplicar as transforma��es � necess�rio usar as teclas 'w', 'a', 's' e 'd'.
O c�rculo sofrer� transforma��es de transla��o e o quadrado de escala.

## 5. Todas as Transforma��es
Fa�a um programa que desenhe um objeto de sua prefer�ncia e aplique as tr�s transforma��es geom�tricas (transla��o, rota��o e escala) a partir de eventos de teclado e mouse (de sua prefer�ncia), conforme o seguinte:

Construa uma �nica matriz de transforma��o a partir da multiplica��o das matrizes anteriores.

Para transla��o foram usadas as teclas 'w', 'a', 's' e 'd';
Para rota��o foram usadas as teclas 'q' e 'e';
Para escala foi usado o "scroll" do mouse;