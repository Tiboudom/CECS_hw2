##
## CSULB PROJECT, 2020
## Makefile
## File description:
## Makefile
##

DIR1	=	src/server

DIR2	=	src/client

DEST1	=	.

RM	=	rm -f

all:
	@$(MAKE) -C $(DIR1); mv $(DIR1)/server $(DEST1)
	@$(MAKE) -C $(DIR2); mv $(DIR2)/client $(DEST1)

server:
	@$(MAKE) -C $(DIR1); mv $(DIR1)/server $(DEST1)

client:
	@$(MAKE) -C $(DIR2); mv $(DIR2)/client $(DEST1)

clean:
	@(cd $(DIR1) && $(MAKE) $@)
	@(cd $(DIR2) && $(MAKE) $@)

fclean:
	@($(RM) server)
	@($(RM) client)
	@(cd $(DIR1) && $(MAKE) $@)
	@(cd $(DIR2) && $(MAKE) $@)

re:	fclean all

.PHONY: fclean re all server client
