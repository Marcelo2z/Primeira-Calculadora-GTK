# Calculadora em C++ com GTK

Projeto de uma calculadora gráfica desenvolvida em C++ utilizando a biblioteca GTK4.

## Funcionalidades

- Interface gráfica com botões
- Operações básicas:
  - Soma (+)
  - Subtração (-)
  - Multiplicação (*)
  - Divisão (/)
- Botão de limpar (C)
- Exibição do número atual no display
- Exibição da operação atual em um label separado

## Interface

> 

## Tecnologias utilizadas

- C++
- GTK4
- MSYS2 (UCRT64)

## Como executar

### Pré-requisitos

- Ter o GTK4 instalado via MSYS2

### Compilar

```bash
g++ main.cpp -o calculadora `pkg-config --cflags --libs gtk4`
