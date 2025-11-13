# Используем базовый образ с GCC и CMake
FROM gcc:14.2.0

# Устанавливаем необходимые зависимости
RUN apt-get update && apt-get install -y \
    cmake wrk clangd\
    && rm -rf /var/lib/apt/lists/*

# Устанавливаем рабочую директорию
WORKDIR /workspace
