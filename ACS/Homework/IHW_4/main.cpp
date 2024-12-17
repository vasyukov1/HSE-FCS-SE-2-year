#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <set>
#include <queue>
#include <unistd.h>

#define NUM_READERS_DEFAULT 100
#define NUM_BOOKS_DEFAULT 5000

std::ofstream out;
std::queue<int> librarian_queue;
std::queue<int> books_queue[NUM_BOOKS_DEFAULT];

pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t librarianCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t booksCond[NUM_BOOKS_DEFAULT];

bool book_requested[NUM_BOOKS_DEFAULT] = {false};
pthread_t books_threads[NUM_BOOKS_DEFAULT];

bool library_open = true;
int all_requests = 0;
int NUM_READERS = NUM_READERS_DEFAULT;
int NUM_BOOKS = NUM_BOOKS_DEFAULT;

// Проверяет, пусты ли очереди всех книг
bool isAllQueuesEmpty() {
    for (int i = 0; i < NUM_BOOKS; ++i) {
        if (!books_queue[i].empty()) {
            return false;
        }
    }
    return true;
}

// Генерация случайной книги
int randomBook() {
    return rand() % NUM_BOOKS;
}

// Поток обработки книги
void* bookReading(void* arg) {
    int id = *(int*)arg;
    delete (int*)arg;

    while (true) {
        pthread_mutex_lock(&queueMutex);

        // Пока очередь книги пуста и библиотека открыта, ждать
        while (books_queue[id].empty() && library_open) {
            pthread_cond_wait(&booksCond[id], &queueMutex);
        }
        // Если очередь книги пуста и библиотека закрыта, то завершаем работу книги
        if (books_queue[id].empty() && !library_open) {
            pthread_mutex_unlock(&queueMutex);
            break;
        }

        // Получаем читателя книги
        int reader = books_queue[id].front();
        books_queue[id].pop();

        std::cout << "Reader " << reader << " is reading book " << id << '\n';
        out  << "Reader " << reader << " is reading book " << id << '\n';
        pthread_mutex_unlock(&queueMutex);

        // Симуляция чтения книги
        usleep(1000 + rand() % 10 * 1000);

        pthread_mutex_lock(&queueMutex);
        std::cout << "Reader " << reader << " returned book " << id << '\n';
        out  << "Reader " << reader << " returned book " << id << '\n';
        pthread_mutex_unlock(&queueMutex);

        pthread_mutex_lock(&queueMutex);
        // Удаляем 1 запрос на чтение
        --all_requests;
        pthread_mutex_unlock(&queueMutex);
    }
    return nullptr;
}

// Поток библиотекаря
void* librarian(void* arg) {
    while (true) {
        pthread_mutex_lock(&queueMutex);

        // Пока очередь к библиотекарю пуста и библиотека открыта, ждать
        while (librarian_queue.empty() && library_open) {
            pthread_cond_wait(&librarianCond, &queueMutex);
        }

        // Если очередь к библиотекарю пуста и библиотека закрыта, ждать
        if (librarian_queue.empty() && !library_open) {
            pthread_mutex_unlock(&queueMutex);
            break;
        }

        // Получаем читателя
        int reader = librarian_queue.front();
        librarian_queue.pop();

        std::cout << "Librarian is seeing reader " << reader << '\n';
        out << "Librarian is seeing reader " << reader << '\n';
        pthread_mutex_unlock(&queueMutex);

        // Имитируем работу библиотекаря с читателем
        usleep(2000);

        // Читатель выбирает до 3 книг
        std::set<int> books_reserved;
        for (int i = 0; i < 3; ++i) {
            int book = randomBook();
            books_reserved.insert(book);
        }

        // Заполняем очередь к книге
        for (int book : books_reserved) {
            pthread_mutex_lock(&queueMutex);
            books_queue[book].push(reader);
            pthread_cond_signal(&booksCond[book]);
            ++all_requests;

            if (!book_requested[book]) {
                book_requested[book] = true;
                int* book_id = new int(book);
                pthread_create(&books_threads[book], nullptr, bookReading, book_id);
            }

            std::cout << "Librarian gave to reader " << reader << " book " << book << '\n';
            out << "Librarian gave to reader " << reader << " book " << book << '\n';
            pthread_mutex_unlock(&queueMutex);
        }
    }
    return nullptr;
}

// Чтение конфигурационного файла
void readConfigFile(const std::string& filename, int& readers, int& books) {
    std::ifstream config(filename);
    if (!config.is_open()) {
        throw std::runtime_error("Cannot open configuration file");
    }

    config >> readers >> books;
    config.close();
}

int main(int argc, char* argv[]) {
    srand(time(nullptr));

    bool use_config = false;
    std::string choice;
    std::cout << "Do you want to use your file? (Yes/No): ";
    std::cin >> choice;
    if (choice == "Yes") {
        std::cout << "Use data from file.\n";
        use_config = true;
    } else {
        std::cout << "Use automatic data.\n";
    }

    if (use_config) {
        if (argc != 3) {
            std::cout << "You can use: ./miltithreading [-f configFile]\nData in the file: NUM_READERS NUM_BOOKS\n";
            use_config = false;
        } else {
            // Обработка аргументов командной строки
            std::string output_filename;
            try {
                if (std::string(argv[1]) == "-f") {
                    std::string configFileName = argv[2];
                    readConfigFile(configFileName, NUM_READERS, NUM_BOOKS);
                }
            } catch (const std::runtime_error&) {
                use_config = false;
            }
        }
    }

    std::string filename;
    std::cout << "Input output file name: ";
    std::cin >> filename;

    out.open(filename, std::ios::out | std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return 1;
    }

    std::cout << "Library simulation started. Data will be written to " << filename << '\n';
    std::cout << "============================\n";

    pthread_t librarian_thread;
    int librarian_id = 1;

    for (int i = 0; i < NUM_BOOKS; ++i) {
        pthread_cond_init(&booksCond[i], nullptr);
    }

    pthread_create(&librarian_thread, nullptr, librarian, &librarian_id);

    for (int i = 1; i <= NUM_READERS; ++i) {
        pthread_mutex_lock(&queueMutex);
        librarian_queue.push(i);
        pthread_cond_signal(&librarianCond);
        pthread_mutex_unlock(&queueMutex);
        usleep(1000);
    }

    while (true) {
        pthread_mutex_lock(&queueMutex);
        if (all_requests == 0 && isAllQueuesEmpty()) {
            library_open = false;
            pthread_cond_broadcast(&librarianCond);
            for (int i = 0; i < NUM_BOOKS; ++i) {
                pthread_cond_broadcast(&booksCond[i]);
            }
            pthread_mutex_unlock(&queueMutex);
            break;
        }
        pthread_mutex_unlock(&queueMutex);
        usleep(500);
    }

    pthread_join(librarian_thread, nullptr);
    for (int i = 0; i < NUM_BOOKS; ++i) {
        if (book_requested[i]) {
            pthread_join(books_threads[i], nullptr);
        }
        pthread_cond_destroy(&booksCond[i]);
    }

    std::cout << "Library is now closed." << std::endl;
    out << "Library is now closed." << std::endl;

    out.close();
    return 0;
}

