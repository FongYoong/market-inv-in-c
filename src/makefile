CC = g++
CFLAGS  = -Wall
FILE_NAME  = marketinv.exe

default: marketinv

marketinv:
	$(CC) $(CFLAGS) -o $(FILE_NAME) main.cpp InventoryManager.cpp InputManager.cpp FileManager.cpp UI.cpp -lcomdlg32

# -LC:/mingw/lib -lcomdlg32 -lole32