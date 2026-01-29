#include <iostream>     // Entrada y salida estándar
#include <omp.h>        // Librería OpenMP para paralelismo
#include <cstdlib>      // rand(), srand()
#include <ctime>        // time()

// Función que imprime los primeros m elementos de un arreglo
void imprimeArreglo(float* d, int m);

int main()
{
    // Número de elementos de los arreglos
    int N = 10000000;

    // Tamaño del bloque (chunk) para la distribución del for paralelo
    int chunk = 100;

    // Cantidad de elementos a mostrar en pantalla
    int mostrar = 20;

    //  Variables auxiliares
    int i;
    int opcion;

    //  Impresión de menú, y selección de tipo de ejecución
    std::cout << "Suma de Arreglos en Paralelo utilizando OpenMP\n";
    std::cout << "\nComo se llena el arreglo?\n";
    std::cout << "1) Aleatorio\n";
    std::cout << "2) Manual\n";
    std::cout << "Opcion: ";
    std::cin >> opcion;

    // Llenado manual de los arreglos
    if (opcion == 2)
    {
        std::cout << "\nCuantos valores desea por arreglo?: ";
        std::cin >> N;
    }

    // Arreglos dinámicos: Esto se hace de esta forma para evitar crear un arreglo de 10,000,000 cuando la entrada es manual y
    //  solo se requieren n cantidad de espacios.
    float* a = new float[N];
    float* b = new float[N];
    float* c = new float[N];

    if (opcion == 2)
    {
        // Si el arreglo es manual y son menos de 25 elementos por arreglo, se muestran todos los valores
        mostrar = (N <= 25) ? N : 25;

        //  Input de los valores
        for (i = 0; i < N; i++)
        {
            std::cout << "A[" << i << "]: ";
            std::cin >> a[i];
            std::cout << "B[" << i << "]: ";
            std::cin >> b[i];
        }
    }
    // Llenado aleatorio de los arreglos
    else
    {
        std::cout << "Se generaran " << N << " elementos aleatorios por arreglo.\n";
        srand((unsigned)time(NULL));

        for (i = 0; i < N; i++)
        {
            a[i] = (float)(rand() % 100);  // Valores entre 0 y 99
            b[i] = (float)(rand() % 100);
        }
    }

    std::cout << "\nNumero de chunks: " << chunk << ".\n";

    /*
        FOR PARALELO CON OPENMP
        Cada iteración del ciclo for se reparte entre los hilos disponibles.
        La operación c[i] = a[i] + b[i] es independiente para cada i,
        por lo que es adecuada para paralelización.
    */
    #pragma omp parallel for shared(a, b, c, chunk) private(i) schedule(static, chunk)
    for (i = 0; i < N; i++)
    {
        c[i] = a[i] + b[i];
    }

    // Impresión de resultados para verificar la suma
    std::cout << "\nPrimeros " << mostrar << " valores del arreglo A:\n";
    imprimeArreglo(a, mostrar);

    std::cout << "Primeros " << mostrar << " valores del arreglo B:\n";
    imprimeArreglo(b, mostrar);

    std::cout << "Primeros " << mostrar << " valores del arreglo C = A + B:\n";
    imprimeArreglo(c, mostrar);

    delete[] a;
    delete[] b;
    delete[] c;

    return 0;
}

// Función que imprime los primeros m elementos de un arreglo
void imprimeArreglo(float* d, int m)
{
    for (int x = 0; x < m; x++)
        std::cout << d[x] << " - ";
    std::cout << "\n";
}