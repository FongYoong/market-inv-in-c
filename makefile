FILE_NAME  = marketinv.exe
BUILD_DIR = build

all:
	cd src && make
	if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	move "src\$(FILE_NAME)" "$(BUILD_DIR)\"

run:
	$(BUILD_DIR)/$(FILE_NAME)