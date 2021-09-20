# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sdincbud <sdincbud@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/04 18:02:25 by edjubert          #+#    #+#              #
#    Updated: 2020/06/28 18:15:36 by sdincbud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

HOSTTYPE 		?= 		$(shell uname -m)_$(shell uname -s)
MALLOC			:=		libft_malloc_$(HOSTTYPE).so

#==============================================================================#
#------------------------------------------------------------------------------#
#                                  DIRECTORIES                                 #

SRC_DIR_MALLOC	:=		./srcs/
INC_DIR			:=		./includes
OBJ_DIR_MALLOC	:=		./obj
LIBFT_FOLDER	:=		./libft
LIBFT			:=		$(LIBFT_FOLDER)/libft.a
INC_LIB			:=		./libft/includes
INC_FILES		:=		./libft/includes/libft.h	\
						./includes/malloc.h

#==============================================================================#
#------------------------------------------------------------------------------#
#                                   MALLOC                                    #

SRC_MALLOC		:=		malloc.c	\
						chunk.c		\
						page.c		\
						page_tools.c\
						tools.c		\
						free.c		\
						show_mem.c	\
						realloc.c

OBJ_MALLOC		:=		$(addprefix $(OBJ_DIR_MALLOC)/,$(SRC_MALLOC:.c=.o))
NB_MALLOC		:=		$(words $(SRC_MALLOC))
INDEX_MALLOC	:=		0

#==============================================================================#
#------------------------------------------------------------------------------#
#                               COMPILER & FLAGS                               #

GCC				:=		gcc
FLAGS			:=		-Wall						\
						-Wextra						\
						-Werror

#==============================================================================#
#------------------------------------------------------------------------------#
#                                     RULES                                    #

.PHONY:					all
all:					makelib $(MALLOC)

.PHONY:					makelib
makelib :
	@ make -C $(LIBFT_FOLDER)

$(MALLOC): $(OBJ_MALLOC)
	@ $(GCC) -shared $(FLAGS) -L$(LIBFT_FOLDER) -lft -o $(MALLOC) $^ $(LIBRARY)
	@ln -sf $(MALLOC) libft_malloc.so
	@ printf '\033[32m[ 100%% ] %-15s\033[92m%-30s\033[32m%s\n\033[0m' \
		"Compilation of " $(MALLOC) " is done ---"

$(OBJ_DIR_MALLOC)/%.o:	$(SRC_DIR_MALLOC)/%.c $(INC_FILES)
	@ mkdir -p $(OBJ_DIR_MALLOC)
	@ $(eval DONE = $(shell echo $$(($(INDEX_MALLOC) * 20 / $(NB_MALLOC)))))
	@ $(eval PERCENT = $(shell \
		echo $$(($(INDEX_MALLOC) * 100 / $(NB_MALLOC)))))
	@ $(eval TO_DO = $(shell echo "$@"))
	@ $(GCC) $(FLAGS) -fPIC -I$(INC_LIB) -I$(INC_DIR) -c $< -o $@
	@ printf "                                                          \r"
	@ printf "\033[33m[ %3d%% ] %s\t%s\r\033[0m" $(PERCENT) $(MALLOC) $@
	@ $(eval INDEX = $(shell echo $$(($(INDEX_MALLOC) + 1))))

.PHONY:					clean
clean:
	@ /bin/rm -rf $(OBJ_DIR_MALLOC)
	@ make -C $(LIBFT_FOLDER) clean --no-print-directory
	@ printf '\033[91m[ KILL ] %-15s\033[31m%-30s\033[91m%s\n\033[0m' \
		"CLEAN  of " $(MALLOC) " is done ---"

.PHONY:					fclean
fclean:					clean
	@ /bin/rm -rf $(MALLOC)
	@ make -C $(LIBFT_FOLDER) fclean --no-print-directory
	@ printf '\033[91m[ KILL ] %-15s\033[31m%-30s\033[91m%s\n\033[0m' \
		"FCLEAN of " $(MALLOC) " is done ---"

.PHONY:					re
re:						fclean all
