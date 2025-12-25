# Используем базовый образ с GCC и CMake
FROM gcc:14.2.0

# Устанавливаем необходимые зависимости
RUN apt-get update && apt-get install -y \
    cmake wrk clangd strace python3-venv\
    && rm -rf /var/lib/apt/lists/*

# Устанавливаем рабочую директорию
WORKDIR /workspace

COPY requirements.txt /workspace/

# Создаем виртуальное окружение
RUN python3 -m venv /opt/venv

# Устанавливаем UV в виртуальное окружение
RUN /opt/venv/bin/pip install --upgrade pip
RUN /opt/venv/bin/pip install uv

# Добавляем виртуальное окружение в PATH
ENV PATH="/opt/venv/bin:${PATH}"

# Проверяем установку
RUN uv --version

RUN uv pip install -r requirements.txt