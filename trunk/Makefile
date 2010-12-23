client			:
				  @echo ##########
				  @echo # Client #
				  @echo ##########
				  make -j 5 -C src/client

rootkit			:
				  @echo ###########
				  @echo # Rootkit #
				  @echo ###########
				  @build /c /M 5

client_clean	:
				  @echo ##########
				  @echo # Client #
				  @echo ##########
				  make clean -j 5 -C src/client

client_fclean	:
				  @echo ##########
				  @echo # Client #
				  @echo ##########
				  make fclean -j 5 -C src/client

client_re		:
				  @echo ##########
				  @echo # Client #
				  @echo ##########
				  make re -j 5 -C src/client

all				: client rootkit

clean			: client_clean

fclean			: client_fclean

re				: client_re rootkit
