#include <gtk/gtk.h>
#include <cstdio>
#include <cstdlib>

// Display principal
static GtkWidget* display = NULL;

// Label que mostra a operação atual
static GtkWidget* label_operacao = NULL;

// Estado da calculadora
static double primeiro_numero = 0.0;
static char operacao = '\0';
static bool nova_operacao = false;

// Atualiza o texto do label da operação
static void atualizar_label_operacao() {
    char texto[50];

    if (operacao == '\0') {
        gtk_label_set_text(GTK_LABEL(label_operacao), "Resultado da operação:");
    } else {
        snprintf(texto, sizeof(texto), "%g %c", primeiro_numero, operacao);
        gtk_label_set_text(GTK_LABEL(label_operacao), texto);
    }
}

// Função chamada ao clicar em um botão
static void on_button_clicker(GtkButton* btn, gpointer user_data) {
    const char* texto_botao = gtk_button_get_label(btn);
    const char* texto_atual = gtk_editable_get_text(GTK_EDITABLE(display));

    char novo_texto[256];

    // Botão C para limpar o display
    if (g_strcmp0(texto_botao, "C") == 0) {
        gtk_editable_set_text(GTK_EDITABLE(display), "0");
        primeiro_numero = 0.0;
        operacao = '\0';
        nova_operacao = false;
        atualizar_label_operacao();
        return;
    }

    // Operadores
    if (g_strcmp0(texto_botao, "+") == 0 ||
        g_strcmp0(texto_botao, "-") == 0 ||
        g_strcmp0(texto_botao, "*") == 0 ||
        g_strcmp0(texto_botao, "/") == 0) {

        primeiro_numero = atof(texto_atual);
        operacao = texto_botao[0];
        nova_operacao = true;
        atualizar_label_operacao();
        return;
    }

    // Operações
    if (g_strcmp0(texto_botao, "=") == 0) {
        double segundo_numero = atof(texto_atual);
        double resultado = 0.0;

        switch (operacao) {
            case '+':
                resultado = primeiro_numero + segundo_numero;
                break;
            case '-':
                resultado = primeiro_numero - segundo_numero;
                break;
            case '*':
                resultado = primeiro_numero * segundo_numero;
                break;
            case '/':
                if (segundo_numero != 0) {
                    resultado = primeiro_numero / segundo_numero;
                } else {
                    gtk_editable_set_text(GTK_EDITABLE(display), "Erro");
                    operacao = '\0';
                    nova_operacao = false;
                    atualizar_label_operacao();
                    return;
                }
                break;
            default:
                return;
        }

        snprintf(novo_texto, sizeof(novo_texto), "%g", resultado);
        gtk_editable_set_text(GTK_EDITABLE(display), novo_texto); 

        primeiro_numero = resultado;
        operacao = '\0';
        nova_operacao = false;
        atualizar_label_operacao(); // Exibe o resultado na tela
        return;
    }

    // Se começou uma nova operação, o próximo número substitui o display
    if (nova_operacao) {
        gtk_editable_set_text(GTK_EDITABLE(display), texto_botao);
        nova_operacao = false;
        return;
    }

    // Se display está em 0, substitui
    if (g_strcmp0(texto_atual, "0") == 0) {
        snprintf(novo_texto, sizeof(novo_texto), "%s", texto_botao);
    } else {
        snprintf(novo_texto, sizeof(novo_texto), "%s%s", texto_atual, texto_botao);
    }

    gtk_editable_set_text(GTK_EDITABLE(display), novo_texto);
}

// Monta a interface
static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget* window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Calculadora");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 400);

    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    gtk_widget_set_margin_top(box, 20);
    gtk_widget_set_margin_bottom(box, 20);
    gtk_widget_set_margin_start(box, 20);
    gtk_widget_set_margin_end(box, 20);

    // Label da operação
    label_operacao = gtk_label_new("Aguardando operador...");
    gtk_widget_set_halign(label_operacao, GTK_ALIGN_START);

    // Display
    display = gtk_entry_new();
    gtk_editable_set_text(GTK_EDITABLE(display), "0");
    gtk_editable_set_editable(GTK_EDITABLE(display), FALSE);

    // Grid
    GtkWidget* grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);

    GtkWidget* buttons[5][4] = {
        {gtk_button_new_with_label("7"), gtk_button_new_with_label("8"), gtk_button_new_with_label("9"), gtk_button_new_with_label("/")},
        {gtk_button_new_with_label("4"), gtk_button_new_with_label("5"), gtk_button_new_with_label("6"), gtk_button_new_with_label("*")},
        {gtk_button_new_with_label("1"), gtk_button_new_with_label("2"), gtk_button_new_with_label("3"), gtk_button_new_with_label("-")},
        {gtk_button_new_with_label("0"), gtk_button_new_with_label("."), gtk_button_new_with_label("="), gtk_button_new_with_label("+")},
        {gtk_button_new_with_label("C"), NULL, NULL, NULL}
    };

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            if (buttons[i][j] != NULL) {
                if (i == 4 && j == 0) {
                    gtk_grid_attach(GTK_GRID(grid), buttons[i][j], 0, 4, 4, 1);
                } else {
                    gtk_grid_attach(GTK_GRID(grid), buttons[i][j], j, i, 1, 1);
                }

                g_signal_connect(buttons[i][j], "clicked", G_CALLBACK(on_button_clicker), NULL);
            }
        }
    }

    gtk_box_append(GTK_BOX(box), label_operacao);
    gtk_box_append(GTK_BOX(box), display);
    gtk_box_append(GTK_BOX(box), grid);

    gtk_window_set_child(GTK_WINDOW(window), box);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char* argv[]) {
    GtkApplication* app = gtk_application_new(
        "com.example.calculadora",
        G_APPLICATION_DEFAULT_FLAGS
    );

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}