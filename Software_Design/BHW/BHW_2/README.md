# Software Design | BHW 2

Автор: Александр Васюков

## Описание проекта "Text scanner" (Word Cloud Generator)

Проект для анализа текстовых файлов (.txt), подсчёта хеша и составления Word Cloud с помощью [Quick Chart Word Cloud API](https://quickchart.io/wordcloud), написанный на языке программирования Golang.

### Архитектура

Проект состоит из трёх микросервисов:

1. API Gateway – отвечает за routing запросов.
2. Files Storing Service – отвечает за хранение и выдачу файлов.
3. File Analysis Service – отвечает за проведение анализа, хранение результатов и их выдачу.

Для каждого микросервиса создан свой Dockerfile, а также есть docker-compose для запуска всего проекта.

### Базы данных

Используется PostgreSQL:

- **Files Storing Service**:
    - БД содержит `id`, `name`, `hash`, `location` файлов.
    - Сами файлы сохраняются в локальное хранилище.

- **File Analysis Service**:
    - БД содержит `id`, `location`, результат анализа `word_count`, `paragraph_count`, `character_count`.
    - Картинки Word Cloud сохраняются в локальное хранилище.

### Тестирование

Тестирование API проводится с помощью Swagger.

---

## Запуск проекта

Перед запуском необходимо заполнить `.env` файлы для каждого сервиса и Docker Compose.

Скопируйте `.env.sample` и создайте свой `.env`:

```bash
cp .env.sample .env
```

## Swagger

Инициализация:
```bash
cd <MICROSERVICE_NAME>
swag init -g cmd/app/main.go -o internal/docs
```

Документацию Swagger можно посмотреть по ссылке `http://localhost:<PORT>/swagger/index.html`, где `<PORT>` - порт микросервиса.

### Запуск Docker Compose:
```bash
docker-compose up --build
```

## Curl-запросы:

Примеры запросов для каждого микросервиса.

1. API-Gateway (localhost:8080)

Загрузка файла:
```bash
curl -X POST -F "file=@<FILE_NAME>.txt" http://localhost:8080/upload
```

Получение текста загруженного файла по ID:
```bash
curl "http://localhost:8080/download?id=<id>"
```

Анализ файла по ID:
```bash
 curl -X GET "http://localhost:8080/analysis?id=<id>"
```

Получение результата анализа по ID:
```bash
curl -X GET "http://localhost:8080/get?id=<id>"
```

2. Files Storing Service (localhost:8081)

Загрузка файл:
```bash
curl -X POST -F "file=@<FILE_NAME>.txt" http://localhost:8081/store
```

Получение текста загруженного файла по ID:
```bash
curl "http://localhost:8081/download?id=<id>"
```

3. File Analysis Service (localhost:8082)

Анализ файла по ID:
```bash
 curl -X GET "http://localhost:8082/analysis?id=<id>"
```

Получение результата анализа по ID:
```bash
curl -X GET "http://localhost:8082/get?id=<id>"
```