

```bash
CURL -X POST "http:/localhost:8080/accounts/create"
```


```bash
curl -X GET "http://localhost:8080/accounts/<account_id>/balance"
```

```bash
curl -X GET http://localhost:8080/orders
```

```bash
curl -X POST http://localhost:8080/accounts/top_up \
  -H "Content-Type: application/json" \
  -d '{"account_id":"<account_id>","amount":<amount>}'
```

```bash
curl -X POST http://localhost:8080/orders/create \
  -H "Content-Type: application/json" \
  -d '{
    "user_id":"<user_id>",
    "amount":<amount>,
    "description":"<description>"
  }'
```

```bash
curl -X GET http://localhost:8080/orders/<order_id>/status
```

# Software Design | BHW 3

Автор: Александр Васюков

## Описание проекта “Online Store: Orders & Payments”

Проект реализует базовую инфраструктуру интернет-магазина с асинхронным межсервисным взаимодействием. Система построена на микросервисной архитектуре, написана на языке Golang и предназначена для демонстрации устойчивой и масштабируемой обработки заказов и платежей.

### Архитектура

Проект состоит из трёх основных микросервисов:
1.	API Gateway – маршрутизирует запросы пользователей к нужным сервисам.
2.	Order Service – отвечает за создание заказов, просмотр списка заказов и статусов.
3.	Payments Service – отвечает за создание, пополнение и просмотр баланса счетов пользователей.

Межсервисное взаимодействие между Order Service и Payments Service реализовано через брокер сообщений Kafka, что обеспечивает асинхронность, устойчивость и возможность масштабирования.

### Используемые паттерны
- Transactional Outbox — для гарантированной доставки событий о заказах из Order Service в Kafka.
- Transactional Inbox/Outbox — для надёжной обработки платежей и событий статуса в Payments Service.
- Exactly-once semantics — при списании средств реализованы механизмы защиты от двойного списания.

### Базы данных

Используется PostgreSQL:
- Order Service:
▫	Таблица заказов: `id`, `user_id`, ‎⁠amount⁠, ‎⁠description⁠, ‎⁠status⁠, ‎⁠created_at⁠, ‎⁠updated_at⁠.
▫	Outbox-таблица для событий о заказах.
•	Payments Service:
▫	Таблица счетов: ‎⁠id⁠, ‎⁠user_id⁠, ‎⁠balance⁠, ‎⁠created_at⁠, ‎⁠updated_at⁠.
▫	Inbox/Outbox-таблицы для событий оплаты.

Асинхронная обработка заказов и оплат
1.	Пользователь создаёт заказ через Order Service.
2.	Order Service сохраняет заказ и событие в outbox, затем публикует его в Kafka.
3.	Payments Service обрабатывает событие из Kafka, проверяет наличие счёта и средств, выполняет списание (или отклоняет оплату), и публикует событие статуса обратно в Kafka.
4.	Order Service обновляет статус заказа на основании события оплаты.

Гарантии и конкурентность
•	Для предотвращения коллизий при параллельных операциях над счетом используются транзакции и атомарные SQL-инструкции.
•	Обеспечена семантика at most once (и реализованы элементы exactly once при списании средств).

Документация и тестирование
•	Для каждого микросервиса реализована Swagger-документация, доступная по адресу ‎⁠/swagger/index.html⁠.
•	Более 65% кода покрыто unit-тестами.
•	Все сервисы упакованы в Docker-контейнеры, запуск системы осуществляется через ‎⁠docker-compose.yml⁠.

Запуск проекта

Перед запуском заполните ‎⁠.env⁠ файлы для каждого сервиса.

Пример:cp .env.sample .env

Запуск системы:docker-compose up --build

Примеры curl-запросов

Работа со счетами

Создать счёт:curl -X POST "http://localhost:8080/accounts/create"

Пополнить счёт:curl -X POST http://localhost:8080/accounts/top_up \
-H "Content-Type: application/json" \
-d '{"account_id":"<account_id>","amount":<amount>}'

Посмотреть баланс:curl -X GET "http://localhost:8080/accounts/<account_id>/balance"

Работа с заказами

Создать заказ:curl -X POST http://localhost:8080/orders/create \
-H "Content-Type: application/json" \
-d '{
"user_id":"<user_id>",
"amount":<amount>,
"description":"<description>"
}'

Получить список заказов:curl -X GET http://localhost:8080/orders

Проверить статус заказа:curl -X GET http://localhost:8080/orders/<order_id>/status

Swagger

Для генерации и просмотра документации используйте:cd <MICROSERVICE_NAME>
swag init -g cmd/app/main.go -o internal/docs

Документация доступна по адресу: ‎⁠http://localhost:<PORT>/swagger/index.html⁠

Особенности
•	Гибкая микросервисная архитектура с асинхронным взаимодействием через Kafka.
•	Гарантированная доставка и обработка событий (Transactional Outbox/Inbox).
•	Устойчивость к сбоям и защита от двойного списания средств.
•	Простая интеграция и масштабирование новых сервисов.
•	Документация и тесты для проверки работоспособности.

Запуск и тестирование
•	Убедитесь, что все сервисы успешно стартовали через Docker Compose.
•	Проверьте работу всех API через Swagger или curl.
•	Для демонстрации асинхронности создайте заказ и проследите смену его статуса после обработки платежа.

Проект разработан в рамках контрольной работы №3 “Асинхронное межсервисное взаимодействие” (BHW 3, июнь 2025).