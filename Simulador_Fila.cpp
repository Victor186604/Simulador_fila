#include <iostream>
#include <queue> 
#include <string>
#include <algorithm>
#include <sstream>

struct Paciente {
    std::string senha;
    char prioridade;
    int hora_chegada; 

    
    Paciente(std::string s, char p, int h)
        : senha(std::move(s)), prioridade(p), hora_chegada(h) {}
};


class Simulador {
private:
    
    std::queue<Paciente> fila_V; 
    std::queue<Paciente> fila_A; 
    std::queue<Paciente> fila_D; 
    std::queue<Paciente> fila_B; 

   
    int total_atendidos = 0; 
    int atendidos_V = 0;     
    int atendidos_A = 0;     
    int atendidos_D = 0;    
    int atendidos_B = 0;     
    int pico_lotacao = 0;    
    int espera_maxima = 0;   

  
    int tempo_para_minutos(int hh, int mm) const {
        return hh * 60 + mm;
    }

  
    void atualizar_pico_lotacao() {
        size_t total_atual = fila_V.size() + fila_A.size() + fila_D.size() + fila_B.size();
        if (total_atual > (size_t)pico_lotacao) {
            pico_lotacao = (int)total_atual;
        }
    }

public:
   

    void processar_chegada(const std::string& senha, char prioridade, int hh, int mm) {
        int tempo_minutos = tempo_para_minutos(hh, mm);
        Paciente novo_paciente(senha, prioridade, tempo_minutos);

        switch (prioridade) {
            case 'V':
                fila_V.push(novo_paciente);
                break;
            case 'A':
                fila_A.push(novo_paciente);
                break;
            case 'D':
                fila_D.push(novo_paciente);
                break;
            case 'B':
                fila_B.push(novo_paciente);
                break;
            default:
                
                break;
        }
        
        atualizar_pico_lotacao();
    }
        void processar_atendimento(int hh, int mm) {
    int tempo_atendimento = tempo_para_minutos(hh, mm);
    Paciente paciente_atendido("", ' ', 0);
    bool atendido = false;

    if (fila_V.empty() && fila_A.empty() && fila_D.empty() && fila_B.empty()) {
        printf("%02d %02d Sem pacientes aguardando atendimento\n", hh, mm);
        return;
    }

    if (!fila_V.empty()) {
        paciente_atendido = fila_V.front();
        fila_V.pop();
        atendidos_V++;
        atendido = true;
    } 
    else if (!fila_A.empty()) {
        paciente_atendido = fila_A.front();
        fila_A.pop();
        atendidos_A++;
        atendido = true;
    } 
    else if (!fila_D.empty()) {
        paciente_atendido = fila_D.front();
        fila_D.pop();
        atendidos_D++;
        atendido = true;
    } 
    else if (!fila_B.empty()) {
        paciente_atendido = fila_B.front();
        fila_B.pop();
        atendidos_B++;
        atendido = true;
    }

    if (atendido) {
        int tempo_espera = tempo_atendimento - paciente_atendido.hora_chegada;
        total_atendidos++;
        printf("%02d %02d %s\n", hh, mm, paciente_atendido.senha.c_str());
        espera_maxima = std::max(espera_maxima, tempo_espera);
    }
}
    
    void exibir_status() const {
        printf("V:%zu A:%zu D:%zu B:%zu | Atendidos:%d\n", 
               fila_V.size(), fila_A.size(), fila_D.size(), fila_B.size(), total_atendidos); 
    }
   
    void relatorio_final() const {
        printf("RELATÓRIO FINAL ---\n");
        printf("Total atendidos: %d\n", total_atendidos); 
        printf("Por prioridade: V=%d A=%d D=%d B=%d\n", atendidos_V, atendidos_A, atendidos_D, atendidos_B);
        printf("Pico de lotação: %d\n", pico_lotacao); 
        printf("Espera máxima: %d min\n", espera_maxima); 
    }
};
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string linha;
    Simulador simulador;

    while (std::getline(std::cin, linha)) {
        if (linha.empty()) continue;

        std::stringstream ss(linha);
        char comando;
        ss >> comando;

        if (comando == 'C') {
            std::string senha;
            char prioridade;
            int hh, mm;
            ss >> senha >> prioridade >> hh >> mm;
            simulador.processar_chegada(senha, prioridade, hh, mm);

        } else if (comando == 'A') {
            int hh, mm;
            ss >> hh >> mm;
            simulador.processar_atendimento(hh, mm);

        } else if (comando == 'D') {
            simulador.exibir_status();

        } else if (comando == 'Q') {
            simulador.relatorio_final();
            break;
        }
    }

    return 0;
}