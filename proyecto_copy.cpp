#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <cmath>
#include <chrono>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <thread>
#include <sys/sysinfo.h>


class Statistics {
private:
    int totalPedidos;
    std::chrono::high_resolution_clock::time_point startTime;

public:
    Statistics() : totalPedidos(0) {
        startTime = std::chrono::high_resolution_clock::now();
    }
    
    void incrementPedidos() {
        totalPedidos++;
    }
    
    double getElapsedTime() const {
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;
        return duration.count();
    }

   int getTotalPedidos() const {
        return totalPedidos;
    } 
};

using namespace std;

struct Plato {
    int codigo;
    string nombre;
    double precio;
    int cantidad;
};

struct Nodo {
    Plato dato;
    Nodo* siguiente;
};

class Pila {
private:
    Nodo* cima;
public:
    Pila() : cima(nullptr) {}

    bool estaVacia() {
        return cima == nullptr;
    }

    void push(Plato dato) {
        Nodo* nuevoNodo = new Nodo;
        nuevoNodo->dato = dato;
        nuevoNodo->siguiente = cima;
        cima = nuevoNodo;
    }

    void pop() {
        if (estaVacia()) {
            cout << "Pedidos recolectados" << endl;
        } else {
            Nodo* temp = cima;
            cima = cima->siguiente;
            delete temp;
        }
    }

    Plato* peek() {
        if (estaVacia()) {
            cout << "La pila está vacía. No se puede hacer peek." << endl;
            return nullptr;
        } else {
            return &(cima->dato);
        }
    }
};



class Pedido {
private:
    Pila pedidos;
    double impTotal = 0.0;
public:
    void agregarPlato(int codigo, string nombre, double precio, int cantidad) {
        Plato aux;
        aux.codigo = codigo;
        aux.nombre = nombre;
        aux.precio = precio;
        aux.cantidad = cantidad;
        pedidos.push(aux);
    };

    string generarBoleta() {
        double pago;
        string boleta = "+--------------------------------------------------------------+\n";
        boleta +=       "|                    Boleta de Venta                           |\n";
        boleta +=       "+--------------------------------------------------------------+\n";
        boleta +=       "| Codigo | Nombre               | Cantidad | Precio U. | Total |\n";
        boleta +=       "+--------------------------------------------------------------+\n";
        Pila pedidosTemporal;
        
        pedidosTemporal = pedidos;

        while (!pedidosTemporal.estaVacia()) {
            Plato* platoActual = pedidosTemporal.peek();
            if (platoActual != nullptr) {
                boleta += "| " + to_string(platoActual->codigo) + " ";
                boleta += "| " + platoActual->nombre;
                boleta += string(23 - platoActual->nombre.length(), ' ');
                boleta += "| " + to_string(platoActual->cantidad);
                boleta += string(10 - to_string(platoActual->cantidad).length(), ' ');
                boleta += "| " + to_string(platoActual->precio);
                boleta += string(12 - to_string(platoActual->precio).length(), ' ');
                boleta += "| " + to_string(platoActual->precio * platoActual->cantidad);
                boleta += string(10 - to_string(platoActual->precio * platoActual->cantidad).length(), ' ') + "|\n";
                
                impTotal += platoActual->precio * platoActual->cantidad;
            }
            pedidosTemporal.pop();
        }

        boleta += "+--------------------------------------------------------------+\n";
        boleta += "|   Monto total a pagar      | " + string(10 - to_string(impTotal).length(), ' ') + to_string(impTotal) + "|\n";
        boleta += "+--------------------------------------------------------------+\n";
        
        cout << boleta << endl;
        while(pago <= impTotal){
            cout << "Ingrese el monto para pagar la cuenta: " << endl;
            cin >> pago; 
            if(pago < impTotal){
                cout << "El monto es inferior a lo solicitado, por favor introduzca un valor mayor o igual al monto a pagar" << endl;
            }
        }
        double vuelto = pago - impTotal;
        boleta += "+--------------------------------------------------------------+\n";
        boleta += "|   Monto dado a pagar       | " + string(10 - to_string(pago).length(), ' ') + to_string(pago) + "|\n";
        boleta += "+--------------------------------------------------------------+\n";
        boleta += "+--------------------------------------------------------------+\n";
        boleta += "|   Vuelto a entregar        | " + string(10 - to_string(vuelto).length(), ' ') + to_string(vuelto) + "|\n";
        boleta += "+--------------------------------------------------------------+\n";
        boleta += "\n\nSistema de ventas EL GUSTO                                2024\n";

        return boleta;
    }

    double importeTotal() {
        Pila pedidosTemporal;
        pedidosTemporal = pedidos;
        double impTotal = 0.0;

        while (!pedidosTemporal.estaVacia()) {
            Plato* platoActual = pedidosTemporal.peek();
            if (platoActual != nullptr) {
                impTotal += platoActual->precio * platoActual->cantidad;
            }
            pedidosTemporal.pop();
        }
        return impTotal;
    }


    void crearNuevaPila() {
        pedidos = Pila();
        impTotal = 0;
    }

    void guardarBoletaEnArchivo(string boleta) {
        ofstream archivo("boletas.txt", ios::app);
        if (archivo.is_open()) {
            archivo << boleta << endl;
            archivo.close();
            cout << "Boleta guardada exitosamente en el archivo 'boletas.txt'" << endl;
        } else {
            cout << "No se pudo abrir el archivo para guardar la boleta." << endl;
        }
    }

    void leerBoletas() {
        ifstream archivo("boletas.txt");
        if (archivo.is_open()) {
            string linea;
            while (getline(archivo, linea)) {
                cout << linea << endl;
            }
        archivo.close();
        } else {
            cout << "No se pudo abrir el archivo para leer las boletas." << endl;
        }
}
    
};

void presentarMenu(const vector<Plato>& carta) {
    cout << "+------------------------------------------+" << endl;
    cout << "|                    MENU                  |" << endl;
    cout << "+------------------------------------------+" << endl;
    cout << "| Codigo | Nombre                | Precio  |" << endl;
    cout << "+------------------------------------------+" << endl;
    for (const Plato& plato : carta) {
        cout << "| " << setw(6) << plato.codigo << " | " << setw(20) << left << plato.nombre << " | " << setw(10) << fixed << setprecision(2) << right << plato.precio << "|" << endl;
    }
    cout << "+------------------------------------------+" << endl;
}
void generarArchivoCSV(const Statistics& stats, double cpuUsage, double memoryUsage, int totalPedidos, double tiempoTranscurrido) {
    ofstream archivo("estadisticas.csv", ios::app);
    if (archivo.is_open()) {
        archivo << tiempoTranscurrido << "," << totalPedidos << "," << cpuUsage << "," << memoryUsage << endl;
        archivo.close();

        cout << "Estadisticas registradas en 'estadisticas.csv'." << endl;
    } else {
        cout << "No se pudo abrir el archivo para guardar las estadisticas." << endl;
    }
}

void generarGrafica() {
    ofstream script("grafica_script.gp");
    if (script.is_open()) {
        script << "set datafile separator ','\n";
        script << "set terminal png\n";
        script << "set output 'grafica.png'\n";
        script << "plot 'estadisticas.csv' using 1:2 title 'Uso de CPU' with lines, '' using 1:3 title 'Uso de Memoria' with lines\n";
        script.close();
    } else {
        cout << "No se pudo abrir el archivo de script de Gnuplot." << endl;
    }
};

double obtenerUsoCPU() {
    std::ifstream statFile("/proc/stat");
    std::string line;
    
    if (!statFile) {
        return 0.0; // Error al leer el archivo
    }

    std::getline(statFile, line);
    statFile.close();

    long user, nice, sys, idle, iowait, irq, softirq, steal, guest, guest_nice;
    sscanf(line.c_str(), "cpu %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld", &user, &nice, &sys, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);

    long totalIdle = idle + iowait;
    long totalNonIdle = user + nice + sys + irq + softirq + steal + guest + guest_nice;
    long total = totalIdle + totalNonIdle;

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Esperar un momento para calcular la diferencia
    std::ifstream statFile2("/proc/stat");
    std::string line2;
    std::getline(statFile2, line2);
    statFile2.close();

    long user2, nice2, sys2, idle2, iowait2, irq2, softirq2, steal2, guest2, guest_nice2;
    sscanf(line2.c_str(), "cpu %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld", &user2, &nice2, &sys2, &idle2, &iowait2, &irq2, &softirq2, &steal2, &guest2, &guest_nice2);

    long totalIdle2 = idle2 + iowait2;
    long totalNonIdle2 = user2 + nice2 + sys2 + irq2 + softirq2 + steal2 + guest2 + guest_nice2;
    long total2 = totalIdle2 + totalNonIdle2;

    double cpuPercentage = (1.0 - (static_cast<double>(totalIdle2 - totalIdle) / (total2 - total))) * 100.0;
    return cpuPercentage;
};

double obtenerUsoMemoria() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    long long totalMem = 0, freeMem = 0, buffers = 0, cached = 0;

    if (!meminfo) {
        return 0.0; // Error al leer el archivo
    }

    while (std::getline(meminfo, line)) {
        if (line.find("MemTotal:") == 0) {
            std::istringstream iss(line);
            iss >> line >> totalMem;
        } else if (line.find("MemFree:") == 0) {
            std::istringstream iss(line);
            iss >> line >> freeMem;
        } else if (line.find("Buffers:") == 0) {
            std::istringstream iss(line);
            iss >> line >> buffers;
        } else if (line.find("Cached:") == 0) {
            std::istringstream iss(line);
            iss >> line >> cached;
            break; // Detener el ciclo después de leer la memoria caché
        }
    }

    meminfo.close();

    long long usedMem = totalMem - (freeMem + buffers + cached);

    double memoryPercentage = (static_cast<double>(usedMem) / totalMem) * 100.0;
    return memoryPercentage;
}

int main() {
    int cantidad;
    Pedido ordenes;
    Statistics stats;


    ifstream archivo("menu.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo" << endl;
        return 1;
    }
    vector<Plato> carta;
    string linea;
    while (getline(archivo, linea)) {
        int pos = linea.find("|");
        if (pos == string::npos) {
            cout << "Linea invalida: " << linea << endl;
            continue;
        }
        string codigo = linea.substr(0, pos);
        linea = linea.substr(pos + 1);
        pos = linea.find("|");
        if (pos == string::npos) {
            cout << "Linea invalida: " << linea << endl;
            continue;
        }
        string nombre = linea.substr(0, pos);
        string precioStr = linea.substr(pos + 1);
        try {
            double precio = stod(precioStr);
            Plato plato;
            plato.codigo = stoi(codigo);
            plato.nombre = nombre;
            plato.precio = precio;
            plato.cantidad = 1;
            carta.push_back(plato);
        } catch (const invalid_argument& e) {
            cout << "Error en formato de precio: " << e.what() << endl;
        } catch (const out_of_range& e) {
            cout << "Precio fuera de rango: " << e.what() << endl;
        } catch (...) {
            cout << "Error desconocido al leer el precio" << endl;
        }
    }
    archivo.close();

    int opcion1, opcion2 = 0;
    string boleta;
    double cpuUsageBoleta, memoryUsageBoleta, tiempoTranscurridoBoleta;
    int totalPedidosBoleta = stats.getTotalPedidos(); // Obtener el total de pedidos para esta boleta

    do {
        try{
            cout << "+-----------------------------------------+" << endl;
            cout << "|   Bienvenido al restaurante El Gusto    |" << endl;
            cout << "+-----------------------------------------+" << endl;
            cout << "| 1. Presentar el menu                    |" << endl;
            cout << "| 2. Realizar Pedido                      |" << endl;
            cout << "| 3. Generar boleta                       |" << endl;
            cout << "| 4. Boletas Generadas                    |" << endl;
            cout << "| 5. Salir                                |" << endl;
            cout << "+-----------------------------------------+" << endl;
            cin >> opcion1;

            if (cin.fail()) {
                cout << "Entrada invalida. Debe ingresar un numero." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
        
            switch (opcion1) {
                case 1:
                    
                    presentarMenu(carta);
                    cin.ignore();
                    cout << "\nPulsa Enter para Volver al menu anterior...";
                    cin.get();
                    break;
                case 2:
                    cout << "\n-------------------------------------------------------------";
                    cout << "\n Tomese su tiempo para escoger en el Menu " << endl;
                    while (opcion2 != 1) {
                        presentarMenu(carta);
                        cout << "Ingrese el codigo del platillo que desea ordenar: " << endl;
                        int code, cantidad;
                        cin >> code;
                        if (cin.fail()) {
                            cout << "Entrada invalida. Debe ingresar un numero." << endl;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            continue;
                        }
                        cout << "Cuantos platillos desea usted: " << endl;
                        cin >> cantidad;
                        if (cin.fail()) {
                            cout << "Entrada invalida. Debe ingresar un numero." << endl;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            continue;
                        }
                        bool found = false;
                        for (const Plato& plato : carta) {
                            if (plato.codigo == code) {
                                ordenes.agregarPlato(plato.codigo, plato.nombre, plato.precio, cantidad);
                                cout << "Platillo agregado a su orden." << endl;
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            cout << "Codigo de platillo no encontrado." << endl;
                        }
                        stats.incrementPedidos();
                        cout << "¿Desea ordenar otro platillo? yes(0) / no (1)" << endl;
                        cin >> opcion2;
                        if (opcion2 == 1){
                            break;
                        }
                    }
                    if (opcion2 == 1){
                        cout << "Genere su boleta de pago para que vuelva a solicitar otro pedido." << endl;
                    }
                    break;

                case 3:
                    boleta =  ordenes.generarBoleta();
                    cout << boleta <<endl;
                    cpuUsageBoleta = obtenerUsoCPU(); // Debes implementar esta función para medir el uso de CPU
                    memoryUsageBoleta = obtenerUsoMemoria(); // Debes implementar esta función para medir el uso de memoria
                    totalPedidosBoleta = stats.getTotalPedidos(); // Obtener el total de pedidos para esta boleta
                    tiempoTranscurridoBoleta = stats.getElapsedTime(); // Obtener el tiempo transcurrido para esta boleta
                    generarArchivoCSV(stats, cpuUsageBoleta, memoryUsageBoleta, totalPedidosBoleta, tiempoTranscurridoBoleta); 
                    opcion2 = 0;
                    ordenes.guardarBoletaEnArchivo(boleta);
                    ordenes.crearNuevaPila();
                    boleta = "";
                    cpuUsageBoleta = 0;
                    memoryUsageBoleta = 0;
                    totalPedidosBoleta = 0;
                    tiempoTranscurridoBoleta = 0;
                    break;
                case 4:
                    cout <<"Lista de la boletas de ventas realizadas\n"<<endl;
                    ordenes.leerBoletas();
                    cin.ignore();
                    cout << "\nPulsa Enter para Volver al menu anterior...";
                    cin.get();
                    break;

                case 5:
                    cout << "Adios, Vuelva pronto" << endl;
                    break;

                default:
                    cout << "Opcion no valida. Intente de nuevo" << endl;
                    break;
            }
        }catch (const exception& e) {
            cout << "Error: " << e.what() << ". Saliendo..." << endl;
            break;

        }catch (...) {
            cout << "Error desconocido. Saliendo..." << endl;
            break;
        }
    } while (opcion1 != 5);

    generarGrafica();
    system("gnuplot grafica_script.gp");
    return 0;
}

