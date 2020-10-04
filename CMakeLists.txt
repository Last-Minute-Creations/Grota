cmake_minimum_required(VERSION 2.8.5)
project(grota LANGUAGES C)

# ACE
add_subdirectory(ACE ace)

add_executable(
	grota.elf main.c game.c 
	
) # Compiler emits .elf file, we need hunk for Amiga

target_compile_options(grota.elf PRIVATE -Wall -Wextra)

target_link_libraries(grota.elf ace)
target_compile_definitions(ace PUBLIC ACE_DEBUG ACE_DEBUG_UAE)

add_custom_command( # So call elf2hunk
	TARGET grota.elf POST_BUILD
	COMMAND ${ELF2HUNK} grota.elf grota.exe
)

set(RES_DIR ${CMAKE_CURRENT_LIST_DIR}/res) # st�d bierzemy pliki do konwersji
set(DATA_DIR ${CMAKE_CURRENT_BINARY_DIR}/data) # tu maj� l�dowa�
file(MAKE_DIRECTORY ${DATA_DIR}) #za���my data





set(PLT_PATH ${DATA_DIR}/grota.plt) #tak si� b�dzie nazywa� docelowa paleta
convertPalette(grota.elf ${RES_DIR}/grota.pal ${PLT_PATH}) # z res/grota.pal do build/data/grota.plt

convertBitmaps(
	TARGET grota.elf PALETTE ${PLT_PATH}
	INTERLEAVED SOURCES
		${RES_DIR}/grota.png 
	DESTINATIONS
		${DATA_DIR}/grota.bm 
)



file(
	COPY ${RES_DIR}/placeholdermod.mod
	DESTINATION ${DATA_DIR}
)
