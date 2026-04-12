# AMV VIRTUAL

# APRESENTAÇÃO
Um projeto de hardware de código aberto desenvolvido para promover uma operação ferroviária de forma segura e eficiente. O mesmo projeto faz parte do projeto final do Programa de capacitação em Sistemas Embarcados (EMBARCATECH) - segundo ciclo 2026.


# O AMV VIRTUAL

O AMV Virtual é uma ferramenta de hardware e software que simula o controle e monitoramento da mudança de vias ferreas através de um AMV (Aparelho de Mudança de Via). Ele integra o controle remoto físico que controla o AMV e uma representação simples do controle e monitoramento virtual, simulando o controle do movimento dentro do CCO (Centro de Controle Operacional).

# OBJETIVO DO AMV VIRTUAL

O objetivo do projeto é propor uma ideia de um equipamento para ser usado para mover uma via ferroviária de um lado para o outro de forma remota sem precisar, necessariamente, se deslocar a um pátio de manobras e movimentar de forma manual AMVs espalhados em diversos pontos, as vezes sob condições adversas (SALVO SE HOUVER PROBLEMAS DE QUEDA DE ENERGIA QUE, NESTE CASO EM ESPECÍFICO EXIGEM A MOVIMENTAÇÃO MANUAL). Este equipamento também pode ser utilizada de forma virtual e remotamente, diretamente no CCO. A proposta é criar uma forma de movimentação ferroviária sem precisar acionar equipes de manutenção de via permanente para acionar manualmente, além de criar camadas de segurança em vias ferroviarias sem expor o operador aos riscos de acidentes e revolucionar tecnologia ferroviária.

# INSTRUÇÕES DE INSTALAÇÃO E EXECUÇÃO

Todo o código-fonte do AMV VIRTUAL está inserida no arquivo AMV_Virtual_Definitivo no link do github (https://github.com/wilkeamerico/AMV_Virtual_Definitivo). Para instalar o código-fonte, baixar o arquivo no Github e compilar diretamente no VSCode já com a extensão do Raspberry PI PICO instalado e siga os seguintes passos:  

 - Compilar projeto (COMPILE PROJECT);

<img width="1268" height="381" alt="image" src="https://github.com/user-attachments/assets/8bce3284-6f9d-43f2-9abc-612ca7abe9cb" />


 - Ativar BOOTSEL e em seguida o RESET no BITDOGLAB e clicar em RUN PROJECT (USB). Quando carreger em 100%, é por que o programa foi carregado com sucesso e já pode usar com sucesso!

<img width="1311" height="387" alt="image" src="https://github.com/user-attachments/assets/5ba0fd9d-5cf1-40e0-9b08-4f9f6d2910b9" />


- Logo quando carregado vai aparecer na tela OLED do Bitdoglab a seguinte mensagem, significando que o programa foi gravado;
  
  AMV VIRTUAL
  EMBARCATECH
  BY WILKE
  
![WhatsApp Image 2026-04-11 at 22 30 41](https://github.com/user-attachments/assets/d7eb6fbc-d243-47b0-a0d1-a31fa54300fb)

![WhatsApp Image 2026-04-12 at 00 04 30](https://github.com/user-attachments/assets/abb4e3b3-a394-4611-a6f3-46296fa14c29)




![WhatsApp Image 2026-04-12 at 00 04 30 (1)](https://github.com/user-attachments/assets/cf96dba1-100b-4d15-a8c4-2763096b6358)


  

Para usar o AMV VIRTUAL de forma como controle remoto, siga da seguinte forma:
- APERTAR BOTÃO A: ACENDE SINAL VERDE E SINALIZA QUE O AMV ESTÁ EM "NORMAL" (QUANDO A AGULHA DO AMV ESTÁ NO MESMO SENTIDO DO TRILHO), COM DISPLAY OLED COM SETA PARA DIREITA E EMITE SINAL SONORO;

![WhatsApp Image 2026-04-12 at 00 11 12](https://github.com/user-attachments/assets/e492b7dd-31f7-4622-a841-442ed7367bd0)

- APERTAR BOTÃO B: DESLIGA LED VERDE E ACENDE SINAL VERMELHA E SINALIZA QUE O AMV ESTÁ EM "REVERSO" (QUANDO A AGULHA DO AMV ESTÁ NO SENTIDO CONTRÁRIO A DO TRILHO), COM DISPLAY OLED COM SETA PARA ESQUERDA E TAMBÉM EMITE SINAL SONORO;

![WhatsApp Image 2026-04-11 at 23 31 08](https://github.com/user-attachments/assets/86e9b9c4-289e-4122-bbb3-01ffe804406e)

Para usar de forma remota com acesso a internet (WEB),siga da seguinte forma:
- Ir ao SERIAL MONITOR e clicar em INICIAR MONITORAMENTO;

<img width="1041" height="311" alt="image" src="https://github.com/user-attachments/assets/5318239a-7f5e-49f4-b916-d47f2c50e2e1" />

- Em seguida clicar no botão de RESET do bitdoglab. Fazendo isso aparece a seguinte mensagem:

<img width="1301" height="415" alt="image" src="https://github.com/user-attachments/assets/7fce8e8c-7d05-4f77-afa1-29e395e06e42" />

AQUI VAI GERAR O NÚMERO DO IP QUE VAI USÁ-LA PARA ACESSAR NA PÁGINA WEB.
OBS! ANTES DE COMPILAR, PRIMEIRO CADASTRAR NO CÓDIGO-FONTE O WIFI (SSID E PASS), CONFORME EXEMPLO ABAIXO:

<img width="494" height="103" alt="image" src="https://github.com/user-attachments/assets/72182035-032d-486d-965b-d0e879955072" />

 - A página virtual vai aparecer desta forma:

<img width="1366" height="768" alt="image" src="https://github.com/user-attachments/assets/b74629a7-2464-4dde-ab20-60d91bb5c4e6" />

Dessa forma você consegue acessar o AMV como se tivesse acessando dentro do sistema logado no CCO e pode ser executado da mesma forma como se fosse feito externo. Na própria tela você ainda pode identificar se o LED VERDE ou LED VERMELHO vai estar ligado, conforme exemplo abaixo:

<img width="1030" height="480" alt="image" src="https://github.com/user-attachments/assets/38451c61-b5c1-4eff-81f0-9a59d7406e47" />

OBS: Todo o registro do de ligação dos LEDs REMOTO e LIGADO também vai estar registrado no SERIAL MONITOR como registro do movimento do AMV.

<img width="708" height="234" alt="image" src="https://github.com/user-attachments/assets/36a4c4b5-a0f6-4ddb-b2ea-75438e3ff198" />

O VÍDEO DE DEMONSTRAÇÃO COMPLETA PODE SER ACESSADO NA PÁGINA WEB:
https://www.youtube.com/watch?v=270NCCUAxH8

 
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

### PRINCIPAIS COMPONENTES PARA A INSTALAÇÃO DO AMV VIRTUAL – BitDogLab V7 ###

- **MCU board:** Raspberry Pi Pico H/W or Pico 2 (RP2350)
- **LEDs:** RGB LED (common cathode) + 5×5 WS2812B matrix  
- **Inputs:** Buttons A/B + Reset, analog joystick (VRx/VRy + SW)  
- **Audio:** Passive buzzers A and B (stereo capable)  
- **Display:** OLED 128×64 via I²C (GPIO2 = SDA, GPIO3 = SCL)
- **Power:** USB 5V, battery charger, **INA226** for monitoring (BMS)
- **PÁGINA WEB**



