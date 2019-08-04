MKDEP=$(CXX) -MM

SRCS=GeneticReal.cpp GeneticPool.cpp AbsGenetic.cpp People.cpp PeoplePool.cpp random.cpp $(SRCS_FITNESS)
HDRS=GeneticReal.h GeneticPool.h AbsGenetic.h People.h PeoplePool.h AbsPeople.h Fitness.h SortInvert.h Sort.h random.h $(HDRS_FITNESS)
OUTS = $(SRCS:.cpp=.o)
CFLAGS=-c

genetic: $(OUTS)
	ar rcs libgenetic.a $(OUTS)

%.o : %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f $(OUTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 
