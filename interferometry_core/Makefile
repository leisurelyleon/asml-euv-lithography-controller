# ============================================================================
# ASML EUV LITHOGRAPHY: Interferometry Core Build Script
# ============================================================================

CC = gcc
CFLAGS = -O3 -march=native -mavx2 -mfma -Wall -Wextra -Werror -I./src
LDFLAGS = -lm

SRCS = src/zeiss_mirror_actuators.c src/optical_sensor_fusion.c src/pid_vibration_damping.c src/math_matrix_avx.c
OBJS = $(SRCS:.c=.o)
TARGET = interferometry_core.a

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $^
	@echo "========================================================="
	@echo " [SUCCESS] Zeiss Interferometry Core Compiled"
	@echo "========================================================="

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
	@echo "[CLEAN] Interferometry artifacts removed."
