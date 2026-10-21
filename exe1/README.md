# exe1 — Mutex e recurso compartilhado

- Arquivo: `exe1/main.c`
- Conceito: mutex no FreeRTOS

## Objetivo

As tasks `task_a` e `task_b` compartilham a saída serial: ambas usam `printf`.
Como cada mensagem é impressa em duas partes e há uma troca de contexto entre
elas, sem sincronização as mensagens podem aparecer misturadas.

Crie e use um mutex para garantir que apenas uma task por vez use o `printf`.

## Tarefa

1. Crie o mutex em `main` chamado de `xPrintfMutex`.
2. Em cada task, obtenha o mutex antes do primeiro `printf`.
3. Libere o mutex somente após o segundo `printf`.
4. Use `xSemaphoreTake` e `xSemaphoreGive`.

O mutex deve permanecer tomado durante o `vTaskDelay` entre os dois `printfs`,
pois esse intervalo faz parte da seção crítica da mensagem.

## Resultado esperado

As mensagens podem alternar entre as tasks, mas suas duas linhas não devem se
misturar:

```text
Task A: inicio da mensagem
Task A: fim da mensagem
Task B: inicio da mensagem
Task B: fim da mensagem
```
