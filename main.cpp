#include <mpi.h>


#include "tree.h"
#include "parser.h"
#include "worker.h"
#include "master.h"

int main(int argc, char **argv) {
#if 0
    int taille, rang, hostlen;
    char hostname[MPI_MAX_PROCESSOR_NAME] = {};
    double message = rang + 123;
    int TAG = 123456;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &taille);
    MPI_Comm_rank(MPI_COMM_WORLD, &rang);
    MPI_Get_processor_name(hostname, &hostlen);

    if (rang == 0) {
        MPI_Send(&message, 1, MPI_DOUBLE, (rang + 1) % taille, TAG, MPI_COMM_WORLD);
        MPI_Recv(&message, 1, MPI_DOUBLE, (taille - 1), TAG, MPI_COMM_WORLD, &status);
        printf("Message %g reçu ! %d parmi %d sur %s!\n", message, rang, taille, hostname);
    } else {
        MPI_Recv(&message, 1, MPI_DOUBLE, (rang - 1), TAG, MPI_COMM_WORLD, &status);
        printf("Message %g reçu ! %d parmi %d sur %s!\n", message, rang, taille, hostname);
        MPI_Send(&message, 1, MPI_DOUBLE, (rang + 1) % taille, TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();
#endif
	int taille, rang, hostlen;
	char hostname[MPI_MAX_PROCESSOR_NAME] = {};
	int TAG = 1;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &taille);
	MPI_Comm_rank(MPI_COMM_WORLD, &rang);
	MPI_Get_processor_name(hostname, &hostlen);

	if (rang == 0) {
		//Création de l'arbre
		RuleMap rules;
		Tree *tree = new Tree("Name", "", std::vector<std::string>());

		try {
			debug("Building rules list...");
			parseFile(rules, std::string(argv[1]));

			debug("Parsing rules tree...");
			createTree(tree, rules, std::string(argv[2]));
		} catch (std::string &s) {
			error(s);
		}

		if (taille == 1) {
			tree->process();
		} else {
			master(tree);
		}
	} else {
		worker(rang);
	}

	MPI_Finalize();
}

#if 0
void table(const char *title, const char *mode)
{
	int f, b;
	printf("\n\033[1m%s\033[m\n bg\t fg\n", title);
	for (b = 40; b <= 107; b++) {
		if (b == 48) b = 100;
		printf("%3d\t\033[%s%dm", b, mode, b);
		for (f = 30; f <= 97; f++) {
			if (f == 38) f = 90;
			printf("\033[%dm%3d ", f, f);
		}
		puts("\033[m");
	}
}


int main(void)
{
	int fg, bg, blink, inverse;

	table("normal ( ESC[22m or ESC[m )", "22;");
	table("bold ( ESC[1m )", "1;");
	table("faint ( ESC[2m ), not well supported", "2;");
	table("italic ( ESC[3m ), not well supported", "3;");
	table("underline ( ESC[4m ), support varies", "4;");
	table("blink ( ESC[5m )", "5;");
	table("inverted ( ESC[7m )", "7;");
	return 0;
}
#endif
