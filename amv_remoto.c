/**
 * @file amv_remoto.c
 * @brief Sistema de Controle de AMV (Aparelho de Mudança de Via) via Web e Físico.
 * @author Adaptado para BitDogLab
 * * Hardware: Raspberry Pi Pico W, Matriz 5x5 WS2812B, OLED I2C, Buzzer e LED RGB.
 * Funcionalidades: Servidor HTTP, Feedback Visual (LEDs/Matriz), Feedback Sonoro e Log Serial.
 */

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

// --- MAPEAMENTO DE HARDWARE (BITDOGLAB) ---
#define LED_RED_PIN     13         
#define LED_GRE_PIN     11         
#define BUTTON_A_PIN    5         
#define BUTTON_B_PIN    6         
#define BUZZER_PIN      21          
#define MATRIX_PIN      7           
#define NUM_PIXELS      25          

// --- CONFIGURAÇÕES I2C (OLED SSD1306) ---
#define I2C_PORT        i2c1          
#define I2C_SDA         14
#define I2C_SCL         15
#define OLED_ADDR       0x3C

// --- CONFIGURAÇÕES DE REDE ---
#define WIFI_SSID       "SEU WIFI"
#define WIFI_PASS       "sua senha"

// --- GLOBAIS DE SISTEMA ---
PIO pio_matrix = pio0;
uint sm_matrix = 0;
char status_message[64] = "Sistema Inicializado";
char http_response[2048];

// --- DICIONÁRIO DE FONTES 5x7 COMPLETO (A-Z + Símbolos) ---
const uint8_t fonte_5x7[128][5] = {
    [' '] = {0x00, 0x00, 0x00, 0x00, 0x00},
    ['!'] = {0x00, 0x00, 0x5F, 0x00, 0x00},
    ['+'] = {0x08, 0x08, 0x3E, 0x08, 0x08},
    ['-'] = {0x08, 0x08, 0x08, 0x08, 0x08},
    ['.'] = {0x00, 0x60, 0x60, 0x00, 0x00},
    ['>'] = {0x08, 0x1C, 0x3E, 0x08, 0x08},
    ['<'] = {0x08, 0x3E, 0x1C, 0x08, 0x08},
    ['0'] = {0x3E, 0x51, 0x49, 0x45, 0x3E},
    ['1'] = {0x00, 0x42, 0x7F, 0x40, 0x00},
    ['A'] = {0x7E, 0x11, 0x11, 0x11, 0x7E},
    ['B'] = {0x7F, 0x49, 0x49, 0x49, 0x36},
    ['C'] = {0x3E, 0x41, 0x41, 0x41, 0x22},
    ['D'] = {0x7F, 0x41, 0x41, 0x22, 0x1C},
    ['E'] = {0x7F, 0x49, 0x49, 0x49, 0x41},
    ['F'] = {0x7F, 0x09, 0x09, 0x09, 0x01},
    ['G'] = {0x3E, 0x41, 0x49, 0x49, 0x7A},
    ['H'] = {0x7F, 0x08, 0x08, 0x08, 0x7F},
    ['I'] = {0x00, 0x41, 0x7F, 0x41, 0x00},
    ['J'] = {0x20, 0x40, 0x41, 0x3F, 0x01},
    ['K'] = {0x7F, 0x08, 0x14, 0x22, 0x41},
    ['L'] = {0x7F, 0x40, 0x40, 0x40, 0x40},
    ['M'] = {0x7F, 0x02, 0x0C, 0x02, 0x7F},
    ['N'] = {0x7F, 0x04, 0x08, 0x10, 0x7F},
    ['O'] = {0x3E, 0x41, 0x41, 0x41, 0x3E},
    ['P'] = {0x7F, 0x09, 0x09, 0x09, 0x06},
    ['Q'] = {0x3E, 0x41, 0x51, 0x21, 0x5E},
    ['R'] = {0x7F, 0x09, 0x19, 0x29, 0x46},
    ['S'] = {0x46, 0x49, 0x49, 0x49, 0x31},
    ['T'] = {0x01, 0x01, 0x7F, 0x01, 0x01},
    ['U'] = {0x3F, 0x40, 0x40, 0x40, 0x3F},
    ['V'] = {0x1F, 0x20, 0x40, 0x20, 0x1F},
    ['W'] = {0x3F, 0x40, 0x38, 0x40, 0x3F},
    ['X'] = {0x63, 0x14, 0x08, 0x14, 0x63},
    ['Y'] = {0x07, 0x08, 0x70, 0x08, 0x07},
    ['Z'] = {0x61, 0x51, 0x49, 0x45, 0x43}
};

// --- DRIVER PIO PARA MATRIZ WS2812B ---
static const uint16_t ws2812_instructions[] = {0x6221, 0x1123, 0x1400, 0xa442};
static const struct pio_program ws2812_prog = { .instructions = ws2812_instructions, .length = 4, .origin = -1 };

static inline void ws2812_init(PIO pio, uint sm, uint pin) {
    uint offset = pio_add_program(pio, &ws2812_prog);
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);

    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset, offset + 3);
    sm_config_set_sideset(&c, 1, false, false);
    sm_config_set_sideset_pins(&c, pin);
    sm_config_set_out_shift(&c, false, true, 24);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    sm_config_set_clkdiv(&c, clock_get_hz(clk_sys) / (800000 * 10.0f));
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

// --- FUNÇÕES DO DISPLAY OLED (BARE-METAL) ---
void oled_cmd(uint8_t c) { uint8_t b[2]={0x00,c}; 
i2c_write_blocking(I2C_PORT, OLED_ADDR, b, 2, false); }
void oled_data(uint8_t d) { uint8_t b[2]={0x40,d}; 
i2c_write_blocking(I2C_PORT, OLED_ADDR, b, 2, false); }

void oled_init() {
    sleep_ms(100); oled_cmd(0xAE); oled_cmd(0xD5); oled_cmd(0x80); oled_cmd(0xA8); 
    oled_cmd(0x3F); oled_cmd(0xD3); oled_cmd(0x00); oled_cmd(0x40); oled_cmd(0x8D); 
    oled_cmd(0x14); oled_cmd(0x20); oled_cmd(0x00); oled_cmd(0xA1); oled_cmd(0xC8); 
    oled_cmd(0xDA); oled_cmd(0x12); oled_cmd(0x81); oled_cmd(0xCF); oled_cmd(0xD9); 
    oled_cmd(0xF1); oled_cmd(0xDB); oled_cmd(0x40); oled_cmd(0xA4); oled_cmd(0xA6); 
    oled_cmd(0xAF);
}

void oled_clear() {
    for(int p=0; p<8; p++) { 
        oled_cmd(0xB0+p); 
        oled_cmd(0x00); 
        oled_cmd(0x10); 
        for(int c=0; c<128; c++) oled_data(0x00); 
    }
}

void oled_print(const char *t, uint8_t p, uint8_t col) {
    oled_cmd(0xB0 + p); oled_cmd(col & 0x0F); oled_cmd(0x10 | (col >> 4));
    while(*t) {
        char c = (*t >= 'a' && *t <= 'z') ? (*t - 32) : *t; // Converte para maiúsculo
        for(int i=0; i<5; i++) oled_data(fonte_5x7[(uint8_t)c][i]);
        oled_data(0x00); t++;
    }
}

// --- FUNÇÕES DE FEEDBACK DE HARDWARE ---
void set_matrix_color(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
    for (int i = 0; i < NUM_PIXELS; i++) pio_sm_put_blocking(pio_matrix, sm_matrix, color << 8u);
}

void trigger_buzzer(uint ms) {
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_clkdiv(slice, 125.0f); pwm_set_wrap(slice, 500);
    pwm_set_chan_level(slice, pwm_gpio_to_channel(BUZZER_PIN), 250);
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM); pwm_set_enabled(slice, true);
    sleep_ms(ms); pwm_set_enabled(slice, false);
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_SIO); gpio_put(BUZZER_PIN, 0);
}

// --- LÓGICA DE ESTADOS AMV ---
void set_state_normal() {
    printf("\n[SISTEMA] Comando Recebido: NORMAL\n");
    printf("[SERIAL] Botao A (Fisico) ou Web Acionado. Verde ON\n");
    gpio_put(LED_GRE_PIN, 1); gpio_put(LED_RED_PIN, 0);
    set_matrix_color(0, 50, 0); // Verde
    oled_clear(); oled_print("NORMAL --->", 3, 30);
    strcpy(status_message, "BOTAO A: ESTADO NORMAL ATIVADO");
    trigger_buzzer(1500);
}

void set_state_reverso() {
    printf("\n[SISTEMA] Comando Recebido: REVERSO\n");
    printf("[SERIAL] Botao B (Fisico) ou Web Acionado. Vermelho ON\n");
    gpio_put(LED_GRE_PIN, 0); gpio_put(LED_RED_PIN, 1);
    set_matrix_color(50, 0, 0); // Vermelho
    oled_clear(); oled_print("<--- REVERSO", 3, 30);
    strcpy(status_message, "BOTAO B: ESTADO REVERSO ATIVADO");
    trigger_buzzer(1500);
}

// --- SERVIDOR WEB ---
void build_http_response() {
    snprintf(http_response, sizeof(http_response),
        "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>"
        "<style>body{font-family:Arial; text-align:center; background:#1a1a1a; color:white; padding:50px;}"
        ".status-box{background:#333; padding:20px; border-radius:10px; border:2px solid #555; margin-bottom:30px;}"
        "button{padding:20px 40px; margin:10px; font-weight:bold; border-radius:10px; border:none; cursor:pointer; font-size:16px;}"
        ".btn-v{background:#2ecc71; color:black;} .btn-r{background:#e74c3c; color:white;}</style></head>"
        "<body><h1>CONTROLE AMV - BITDOGLAB</h1>"
        "<div class='status-box'><h3>AVISO: %s</h3></div>"
        "<a href='/verde'><button class='btn-v'>ACIONAR NORMAL (VERDE)</button></a>"
        "<a href='/vermelho'><button class='btn-r'>ACIONAR REVERSO (VERMELHO)</button></a>"
        "</body></html>", status_message);
}

static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) { tcp_close(tpcb); return ERR_OK; }
    char *req = (char *)p->payload;
    if (strstr(req, "GET /verde")) set_state_normal();
    else if (strstr(req, "GET /vermelho")) set_state_reverso();
    build_http_response();
    tcp_write(tpcb, http_response, strlen(http_response), TCP_WRITE_FLAG_COPY);
    pbuf_free(p); return ERR_OK;
}

static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, http_callback); return ERR_OK;
}

// --- INICIALIZAÇÃO E LOOP PRINCIPAL ---
int main() {
    stdio_init_all();
    sleep_ms(2000);
    printf("\r\n========================================\r\n");
    printf("   SISTEMA AMV BITDOGLAB INICIALIZADO     \r\n");
    printf("========================================\r\n");

    // Init Matriz
    ws2812_init(pio_matrix, sm_matrix, MATRIX_PIN);
    set_matrix_color(0, 0, 0);

    // Init OLED
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA); 
    gpio_pull_up(I2C_SCL);
    oled_init(); 
    oled_clear();
    oled_print("EMBARCATECH", 4, 10); 
    oled_print("AMV VIRTUAL", 3, 20);
    oled_print("BY WILKE", 5, 10);

    // Init Wi-Fi
    if (cyw43_arch_init()) return 1;
    cyw43_arch_enable_sta_mode();
    printf("[WIFI] Tentando conectar em: %s...\n", WIFI_SSID);
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("[ERRO] Wi-Fi falhou.\n");
    } else {
        printf("[WIFI] Conectado! IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
    }

    // Init GPIOs
    gpio_init(LED_GRE_PIN); 
    gpio_set_dir(LED_GRE_PIN, GPIO_OUT);

    gpio_init(LED_RED_PIN); 
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);

    gpio_init(BUTTON_A_PIN); 
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN); 
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);

    // Init Servidor
    struct tcp_pcb *pcb = tcp_new();
    tcp_bind(pcb, IP_ADDR_ANY, 80);
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, connection_callback);

    while (true) {
        // Leitura botões físicos (Pressionado = 0 na BitDogLab)
        if (gpio_get(BUTTON_A_PIN) == 0) { 
            set_state_normal();
            while(gpio_get(BUTTON_A_PIN) == 0) sleep_ms(10); // Debounce manual

        } else if (gpio_get(BUTTON_B_PIN) == 0) {
            set_state_reverso();
            while(gpio_get(BUTTON_B_PIN) == 0) sleep_ms(10); // Debounce manual
        }
        sleep_ms(50);
    }
}
